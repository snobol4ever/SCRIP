/* rtx_icn_profile.c — RTX-0d-ICN ROUND 2: the signature-free dynamic ranker.
 *
 * WHY THIS EXISTS. The LD_PRELOAD interposer (tools/rtx_icn_interpose.c) needs one
 * hand-written prototype per symbol, and a wrong one compiles clean and corrupts the
 * ABI silently — that mistake was made on rt_arg_stage (int,DESCR_t vs void*,long) in
 * s210-ICN. It also only ever sees symbols someone already SUSPECTED, which is the
 * exact bias that made the static rank wrong. This instrument needs NO signatures and
 * sees EVERY C function in libscrip_rt.so, so it cannot inherit either defect.
 *
 * HOW. libscrip_rt.so is rebuilt with -finstrument-functions (via RT_OPT, no Makefile
 * edit). GCC then emits a call to __cyg_profile_func_enter on entry to every C
 * function. That symbol is undefined in the .so, so the dynamic linker binds it to
 * whatever is preloaded — this file. We count by function address and symbolize once,
 * at exit, with dladdr.
 *
 * ⚠ COUNTS ONLY, NEVER TIME. Instrumentation inflates every body, so the wall clock
 *   under this build is meaningless and no ratio may be quoted from it. Counts and
 *   their SCALING are unaffected, and ranking is what this rung owes.
 *
 * ⚠ MODE 3 COMPILES AND RUNS IN ONE PROCESS, so emitter/parser functions are counted
 *   too. They are filtered out downstream by intersecting with the symbols actually
 *   defined in src/runtime/ objects. Do not read the raw dump as a runtime rank.
 *
 * ⚠ NO MALLOC, NO LOCKS IN THE HOOK. The table is BSS, open-addressed, atomically
 *   incremented — the hook runs before any constructor we control and on coexpr
 *   pthreads.
 *
 * Build: gcc -O2 -fPIC -shared -o rtx_icn_profile.so rtx_icn_profile.c -ldl
 * Use:   RTXP_OUT=f.tsv LD_PRELOAD=./rtx_icn_profile.so ./scrip --run prog.icn
 *
 * Authors: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#define NO_INSTR __attribute__((no_instrument_function))

#define TBITS 17
#define TSIZE (1u << TBITS)
#define TMASK (TSIZE - 1u)

static void    *k_fn[TSIZE];
static uint64_t k_ct[TSIZE];

static NO_INSTR inline uint32_t hsh(void *p)
{
    uint64_t x = (uint64_t)(uintptr_t)p;
    x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 29;
    return (uint32_t)x & TMASK;
}

/* GCC calls this on entry to every instrumented function. */
void NO_INSTR __cyg_profile_func_enter(void *this_fn, void *call_site)
{
    (void)call_site;
    uint32_t i = hsh(this_fn);
    for (uint32_t n = 0; n < TSIZE; n++) {
        void *cur = __atomic_load_n(&k_fn[i], __ATOMIC_RELAXED);
        if (cur == this_fn) {
            __atomic_fetch_add(&k_ct[i], 1, __ATOMIC_RELAXED);
            return;
        }
        if (cur == NULL) {
            void *exp = NULL;
            if (__atomic_compare_exchange_n(&k_fn[i], &exp, this_fn, 0,
                                            __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
                __atomic_fetch_add(&k_ct[i], 1, __ATOMIC_RELAXED);
                return;
            }
            continue;              /* lost the race; re-inspect this slot */
        }
        i = (i + 1) & TMASK;       /* open addressing */
    }
}

void NO_INSTR __cyg_profile_func_exit(void *this_fn, void *call_site)
{
    (void)this_fn; (void)call_site;
}

__attribute__((destructor)) static void NO_INSTR rtxp_dump(void)
{
    const char *path = getenv("RTXP_OUT");
    FILE *f = path ? fopen(path, "w") : stderr;
    if (!f) f = stderr;
    for (uint32_t i = 0; i < TSIZE; i++) {
        if (!k_fn[i] || !k_ct[i]) continue;
        /* ⛔ dladdr resolves .dynsym ONLY — it cannot name `static` functions, and
         * s210-ICN measured 42-64%% of all traffic sitting in them. Emit the
         * LOAD-BASE-RELATIVE OFFSET so the caller can symbolize against the full
         * .symtab (nm), which does contain locals. Name is a hint only. */
        Dl_info di;
        const char *nm = "?";
        unsigned long long off = 0;
        if (dladdr(k_fn[i], &di)) {
            if (di.dli_sname) nm = di.dli_sname;
            off = (unsigned long long)((char *)k_fn[i] - (char *)di.dli_fbase);
        }
        fprintf(f, "%llu\t%s\t0x%llx\n", (unsigned long long)k_ct[i], nm, off);
    }
    if (f != stderr) fclose(f);
}
