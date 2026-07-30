/* rtx_icn_selftime.c — THE SLOWNESS-RANKED QUEUE, IN ONE PASS.
 *
 * WHY: Lon's directive s218 replaces the s214 small-to-large ordering key with
 * SLOWEST-FIRST, and adds "do not waste time re-measuring; measure once."
 * tools/rtx_icn_profile.c is the right shape (signature-free, sees every C body via
 * -finstrument-functions) but it is COUNTS ONLY and says so. A count is not a cost:
 * this ladder has six recorded falsifications of exactly that inference.
 *
 * WHAT THIS ADDS: SELF cycles, via a per-thread shadow stack.
 *   enter: push {fn, tsc, kids=0}
 *   exit : total = tsc - entry;  self = total - kids;  bucket[fn] += self;
 *          parent.kids += total
 * Recursion falls out of the stack discipline for free; self-time is charged once.
 *
 * ⭐ THE PROPERTY THAT MAKES THIS THE RIGHT INSTRUMENT FOR *THIS* LADDER:
 *   -finstrument-functions instruments C and only C. The 32 symbols already in
 *   src/runtime/**\/*.S are invisible to it by construction. So the ranking IS the
 *   ranking of UNPORTED C — step 0(e) is satisfied mechanically, not by a grep that
 *   can miss a Greek codepoint (the s214 trap).
 *
 * ⚠ HONEST LIMITS — READ BEFORE QUOTING ANY NUMBER FROM THIS.
 *  (a) ORDINAL, NOT RATIO. Two rdtsc reads plus a hook pair cost ~50-90 cycles per
 *      call. We calibrate that (see CAL below) and subtract n_calls*overhead, but the
 *      correction is a mean, not exact. A tiny leaf called 10M times is still
 *      over-charged relative to a fat body called 10 times. ⇒ USE THE ORDER. DO NOT
 *      QUOTE A SHARE, AND NEVER DERIVE A SPEEDUP TARGET FROM IT.
 *  (b) -O0, per FACT RULE O2-DIRECTED-ONLY. -O0 frame ceremony is part of what is
 *      being measured, and it is also part of what a port removes, so this is the
 *      right level for THIS decision -- but it is not a -O2 statement.
 *  (c) MODE 3 COMPILES AND RUNS IN ONE PROCESS, so emitter/parser/lower bodies are
 *      timed too. Filtered downstream by intersecting with runtime-role objects.
 *      Do not read the raw dump as a runtime rank.
 *  (d) rdtsc is not serialized. Fine at this granularity; noise is not the ranking's
 *      problem, (a) is.
 *
 * NO MALLOC / NO LOCKS in the hooks: BSS table, open-addressed, __thread stack.
 *
 * Build: gcc -O2 -fPIC -shared -o /tmp/selftime.so /tmp/rtx_icn_selftime.c -ldl
 * Use:   RTXS_OUT=f.tsv LD_PRELOAD=/tmp/selftime.so ./scrip --run prog.icn
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>

#define NO_INSTR __attribute__((no_instrument_function))
#define NSLOT (1u << 16)
#define NDEPTH 8192

typedef struct { void *fn; uint64_t self; uint64_t calls; } slot_t;
static slot_t tab[NSLOT];
typedef struct { void *fn; uint64_t t0; uint64_t kids; } frame_t;
static __thread frame_t stk[NDEPTH];
static __thread int sp;
static __thread int overflow;
static uint64_t cal_cycles;

static NO_INSTR inline uint64_t tsc(void)
{
    uint32_t a, d;
    __asm__ __volatile__("rdtsc" : "=a"(a), "=d"(d));
    return ((uint64_t)d << 32) | a;
}

static NO_INSTR inline slot_t *slot(void *fn)
{
    uint64_t h = (uint64_t)(uintptr_t)fn;
    h ^= h >> 33; h *= 0xff51afd7ed558ccdULL; h ^= h >> 29;
    uint32_t i = (uint32_t)(h & (NSLOT - 1));
    for (uint32_t n = 0; n < NSLOT; n++) {
        uint32_t k = (i + n) & (NSLOT - 1);
        if (tab[k].fn == fn) return &tab[k];
        if (tab[k].fn == NULL) { tab[k].fn = fn; return &tab[k]; }
    }
    return &tab[0];
}

NO_INSTR void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
    (void)call_site;
    if (sp >= NDEPTH) { overflow++; return; }
    stk[sp].fn = this_fn; stk[sp].kids = 0; stk[sp].t0 = tsc(); sp++;
}

NO_INSTR void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
    (void)call_site;
    uint64_t now = tsc();
    if (sp <= 0) return;
    sp--;
    uint64_t total = now - stk[sp].t0;
    uint64_t self  = total - stk[sp].kids;
    if (self > total) self = 0;
    slot_t *s = slot(stk[sp].fn);
    s->self += self; s->calls++;
    if (sp > 0) stk[sp - 1].kids += total;
}

/* CAL: cost of one enter/exit pair, measured in this process so the correction
 * tracks the actual machine rather than a constant baked in at authoring time. */
static NO_INSTR void calibrate(void)
{
    uint64_t best = ~0ULL;
    for (int r = 0; r < 32; r++) {
        uint64_t a = tsc();
        for (int i = 0; i < 1000; i++) {
            __cyg_profile_func_enter((void *)&calibrate, 0);
            __cyg_profile_func_exit((void *)&calibrate, 0);
        }
        uint64_t b = tsc();
        if (b - a < best) best = b - a;
    }
    cal_cycles = best / 1000;
    slot_t *s = slot((void *)&calibrate);
    s->self = 0; s->calls = 0;
}

__attribute__((constructor)) static NO_INSTR void init(void) { calibrate(); }

__attribute__((destructor)) static NO_INSTR void dump(void)
{
    const char *out = getenv("RTXS_OUT");
    if (!out) return;
    FILE *f = fopen(out, "a");
    if (!f) return;
    for (uint32_t i = 0; i < NSLOT; i++) {
        if (!tab[i].fn || !tab[i].calls) continue;
        uint64_t ovh = tab[i].calls * cal_cycles;
        uint64_t adj = tab[i].self > ovh ? tab[i].self - ovh : 0;
        Dl_info di;
        const char *nm = (dladdr(tab[i].fn, &di) && di.dli_sname) ? di.dli_sname : "?";
        fprintf(f, "%s\t%llu\t%llu\t%llu\n", nm,
                (unsigned long long)adj, (unsigned long long)tab[i].self,
                (unsigned long long)tab[i].calls);
    }
    fprintf(f, "#CAL\t%llu\n", (unsigned long long)cal_cycles);
    fclose(f);
}
