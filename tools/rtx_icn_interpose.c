/* rtx_icn_interpose.c — RTX-0d-ICN step-0(d) counting instrument.
 *
 * LD_PRELOAD interposer. Counts calls and accumulates INCLUSIVE rdtsc cycles for
 * the ICON-RTX candidate symbols, and reports each as a share of whole-process
 * cycles.
 *
 * WHY THIS IS ABI-SAFE WITHOUT PROJECT HEADERS:
 *   DESCR_t is 16 B / two INTEGER-class eightbytes (ARCH-ICON-RTX.md §2, verified
 *   sizeof==16). A struct of two uint64_t is therefore ABI-identical: passed in a
 *   register pair, returned in rdx:rax. We never inspect the value, only pass it
 *   through, so no field layout knowledge is needed.
 *
 * WHY LD_PRELOAD REACHES MODE-3 EMITTED CODE:
 *   bb_call.cpp:291 takes &rt_call_arr inside libscrip_rt.so (-fPIC) and bakes the
 *   resulting address into the emitted blob. Taking the address of an exported
 *   global in a shared object goes through the GOT, so the dynamic linker resolves
 *   it at load time and a preloaded definition wins. The blob therefore calls US.
 *   (This is also why the count is trustworthy for mode 3 specifically.)
 *
 * ⚠ INCLUSIVE, NOT SELF. The cycle share covers everything the symbol reaches.
 *    Splitting portable dispatch prologue from callee work is a SEPARATE job
 *    (GOAL-ICON-RTX.md inbox gap #1) and this instrument does not do it.
 *
 * Build: gcc -O2 -fPIC -shared -o rtx_icn_interpose.so rtx_icn_interpose.c -ldl
 * Use:   RTXI_TAG=name LD_PRELOAD=./rtx_icn_interpose.so ./scrip --run prog.icn
 *
 * Authors: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>

typedef struct { uint64_t lo, hi; } D16;   /* ABI-identical to DESCR_t */

static inline uint64_t tsc(void)
{
    uint32_t a, d;
    __asm__ __volatile__("rdtsc" : "=a"(a), "=d"(d));
    return ((uint64_t)d << 32) | a;
}

/* ---- one row per interposed symbol ---------------------------------------- */
#define NSYM 5
static const char *sym_name[NSYM] = {
    "rt_call_arr", "rt_call_arr_gen", "rt_arg_stage", "rt_call_proc_descr",
    "rt_assign_var"   /* POSITIVE CONTROL: RTX-1b proved this hot (4M stores) */
};
static uint64_t sym_calls[NSYM];
static uint64_t sym_cycles[NSYM];

static uint64_t t_start, t_end;

__attribute__((constructor)) static void rtxi_start(void) { t_start = tsc(); }

__attribute__((destructor)) static void rtxi_report(void)
{
    t_end = tsc();
    uint64_t total = t_end - t_start;
    const char *tag = getenv("RTXI_TAG");
    if (!tag) tag = "-";
    FILE *f = stderr;
    const char *out = getenv("RTXI_OUT");
    if (out) { FILE *g = fopen(out, "a"); if (g) f = g; }
    for (int i = 0; i < NSYM; i++) {
        double pct = total ? (100.0 * (double)sym_cycles[i] / (double)total) : 0.0;
        fprintf(f, "RTXI\t%s\t%s\tcalls=%llu\tcycles=%llu\ttotal=%llu\tshare=%.3f%%\n",
                tag, sym_name[i],
                (unsigned long long)sym_calls[i],
                (unsigned long long)sym_cycles[i],
                (unsigned long long)total, pct);
    }
    if (f != stderr) fclose(f);
}

/* ---- interposed bodies ----------------------------------------------------- */
/* Each: resolve real once, count, time inclusively, pass through untouched. */

D16 rt_call_arr(const char *fn, D16 *args, int nargs)
{
    static D16 (*real)(const char *, D16 *, int);
    if (!real) real = (D16(*)(const char *, D16 *, int))dlsym(RTLD_NEXT, "rt_call_arr");
    uint64_t t0 = tsc();
    D16 r = real(fn, args, nargs);
    sym_cycles[0] += tsc() - t0;
    sym_calls[0]++;
    return r;
}

D16 rt_call_arr_gen(const char *fn, D16 *args, int nargs, int64_t *cell)
{
    static D16 (*real)(const char *, D16 *, int, int64_t *);
    if (!real) real = (D16(*)(const char *, D16 *, int, int64_t *))dlsym(RTLD_NEXT, "rt_call_arr_gen");
    uint64_t t0 = tsc();
    D16 r = real(fn, args, nargs, cell);
    sym_cycles[1] += tsc() - t0;
    sym_calls[1]++;
    return r;
}

/* rt_arg_stage / rt_call_proc_descr — signatures READ FROM THE TREE, not guessed:
 *   src/runtime/rt/rt.c:603  void    rt_arg_stage(int idx, DESCR_t v)
 *   src/runtime/rt/rt.c:643  DESCR_t rt_call_proc_descr(const char *name, int nargs)
 * NOTE rt_arg_stage takes the descriptor BY VALUE (a register pair, rsi:rdx), not by
 * pointer. Modelling it as (void*, long) compiles cleanly and corrupts the ABI
 * silently — that mistake was made and caught here. */
void rt_arg_stage(int idx, D16 v)
{
    static void (*real)(int, D16);
    if (!real) real = (void(*)(int, D16))dlsym(RTLD_NEXT, "rt_arg_stage");
    uint64_t t0 = tsc();
    real(idx, v);
    sym_cycles[2] += tsc() - t0;
    sym_calls[2]++;
}

D16 rt_call_proc_descr(const char *name, int nargs)
{
    static D16 (*real)(const char *, int);
    if (!real) real = (D16(*)(const char *, int))dlsym(RTLD_NEXT, "rt_call_proc_descr");
    uint64_t t0 = tsc();
    D16 r = real(name, nargs);
    sym_cycles[3] += tsc() - t0;
    sym_calls[3]++;
    return r;
}

/* ---- POSITIVE CONTROL (s201 discipline) ------------------------------------
 * DESCR_t rt_assign_var(DESCR_t var, DESCR_t val)  — src/runtime/rtx/rtx_icnvar.S:56
 * Already ASM. Interposed here ONLY to validate that this instrument counts and
 * SCALES on a symbol independently proven hot (RTX-1b-ICN, +12.11% median).
 * If this row is flat too, the instrument is broken and every null it reports is
 * uninformative. RTLD_NEXT still reaches the asm body, so the gate is unaffected. */
typedef struct { uint64_t a, b, c, d; } D32;
D16 rt_assign_var(D16 var, D16 val)
{
    static D16 (*real)(D16, D16);
    if (!real) real = (D16(*)(D16, D16))dlsym(RTLD_NEXT, "rt_assign_var");
    uint64_t t0 = tsc();
    D16 r = real(var, val);
    sym_cycles[4] += tsc() - t0;
    sym_calls[4]++;
    return r;
}
