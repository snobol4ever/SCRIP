#define _GNU_SOURCE
#include <signal.h>
#include "ct_arena.h"
#include "gc_heap.h"
#include <ucontext.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/mman.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_stack_overflow_sig(int sig, siginfo_t *si, void *uctx)
{
    ucontext_t *uc = (ucontext_t *)uctx;
    uintptr_t fault = (uintptr_t)si->si_addr, rsp = (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
    pthread_attr_t attr; void *lo_p = NULL; size_t sz = 0; int have = 0;
    if (rt_gc_stale_addr_report(si->si_addr, (void *)uc->uc_mcontext.gregs[REG_RIP])) { signal(sig, SIG_DFL); raise(sig); }
    { extern int rt_gc_poison_reg_report(const long long *gregs, int n, void *ip); if (rt_gc_poison_reg_report((const long long *)uc->uc_mcontext.gregs, (int)NGREG, (void *)uc->uc_mcontext.gregs[REG_RIP])) { signal(sig, SIG_DFL); raise(sig); } }
    { extern void rt_gc_main_stack_bounds(char **, char **); char *mlo = NULL, *mhi = NULL; rt_gc_main_stack_bounds(&mlo, &mhi);
      if (mlo && mhi && rsp <= (uintptr_t)mhi && rsp + 16UL * 1024 * 1024 >= (uintptr_t)mlo) { lo_p = mlo; sz = (size_t)(mhi - mlo); have = 1; } }
    if (!have && pthread_getattr_np(pthread_self(), &attr) == 0) { have = (pthread_attr_getstack(&attr, &lo_p, &sz) == 0); pthread_attr_destroy(&attr); }
    if (have) { uintptr_t lo = (uintptr_t)lo_p, hi = lo + (uintptr_t)sz, guard = 16UL * 1024 * 1024;
        if (fault < lo && fault + guard >= lo && rsp + guard >= lo && rsp <= hi) {
            static const char msg[] = "scrip: runtime error: ERROR 246 -- stack overflow (unbounded or too-deep recursion exhausted the call stack)\n";
            write(2, msg, sizeof msg - 1); _exit(1); } }
    signal(sig, SIG_DFL);
    raise(sig);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_STACK_DEFAULT_BYTES (4L * 1024L * 1024L)
_Static_assert(RT_STACK_DEFAULT_BYTES == 4194304L, "THE DEFAULTS ARE SPITBOL'S (Lon 2026-09-25, in-chat to the ceo, verbatim: \"Let's set our default stack size and heap size for SCRIP to be the same as SPITBOL.\"; ceo CEO-1261): sbl prints its option defaults as -d128m -i1m -m16m -s4m, so a program gets a 4 MB stack unless its -s switch or SCRIP_STACK says otherwise; the budget counts from the frame that applies it and mode 3 adds the trampoline's reservation for the top-level slot region on top (rt_stack_budget_apply)");
long rt_stack_budget_bytes(long sw_bytes)
{
    long b = RT_STACK_DEFAULT_BYTES;
    const char *e = getenv("SCRIP_STACK");
    if (e && e[0]) { char *ep = NULL; long ev = strtol(e, &ep, 10); if (ev > 0) { if (ep && (*ep == 'k' || *ep == 'K')) ev *= 1024L; else if (ep && (*ep == 'm' || *ep == 'M')) ev *= 1024L * 1024L; b = ev; } }
    if (sw_bytes > 0) b = sw_bytes;
    return b;
}
void rt_stack_budget_apply(long budget, long extra)
{
    pthread_attr_t attr; void *lo_p = NULL; size_t sz = 0; char here = 0; struct rlimit rl;
    if (pthread_getattr_np(pthread_self(), &attr) != 0) return;
    { int ok = (pthread_attr_getstack(&attr, &lo_p, &sz) == 0); pthread_attr_destroy(&attr); if (!ok) return; }
    { uintptr_t hi = (uintptr_t)lo_p + (uintptr_t)sz, sp = (uintptr_t)&here; long want = (long)(hi > sp ? hi - sp : 0) + extra + budget;
      want = (want + 4095L) & ~4095L;
      if (getrlimit(RLIMIT_STACK, &rl) != 0) return;
      if (rl.rlim_max != RLIM_INFINITY && (rlim_t)want > rl.rlim_max) want = (long)rl.rlim_max;
      rl.rlim_cur = (rlim_t)want; (void)setrlimit(RLIMIT_STACK, &rl); (void)here; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_STACK_HUGE (2UL * 1024UL * 1024UL)
_Static_assert(RT_STACK_HUGE == 2097152UL, "A COMPILED PROGRAM RUNS ON A STACK IT MAPS ITSELF (ceo 2026-09-25, CEO-1265; Lon 2026-09-25 10:06, in-chat to the ceo: \"continue getting all demos and benchmarks running faster than SPITBOL\"): the process stack grows down a 4 KB page at a time, each first touch a fault of about 11 K kernel cycles, and a growsdown mapping can never take a transparent huge page because its lower end is never 2 MB aligned -- the calculator demo touched 48 MB of stack in 12,312 faults, two thirds of its whole run. The emitted main calls rt_main_stack_adopt first: it maps the -s budget (SPITBOL's -s4m unless the command line or SCRIP_STACK says otherwise) rounded up to 2 MB at a 2 MB boundary, MADV_HUGEPAGE, one PROT_NONE guard page below it, and main moves rsp to its top and never returns to the old stack. The collector finds the region from its own cached stack top (rt_gc_stack_top_adopt), the overflow handler from the mapping holding that top, and core_lib_init no longer raises RLIMIT_STACK for a program that adopted. Measured: calculator-1 58 ms -> 33 ms, 13,675 faults -> 1,459, output unchanged. A failed map returns 0 and the program stays on the stack it was given");
char *rt_main_stack_adopt(void)
{
    extern long rt_sw_stack_bytes(void); extern int core_stack_floor_raised; extern void rt_gc_stack_top_adopt(char *);
    long budget = rt_stack_budget_bytes(rt_sw_stack_bytes());
    size_t sz = ((size_t)budget + RT_STACK_HUGE - 1) & ~(RT_STACK_HUGE - 1), pg = 4096, total = sz + RT_STACK_HUGE + pg;
    char *m = (char *)mmap(NULL, total, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_STACK, -1, 0);
    if (m == (char *)MAP_FAILED) return (char *)0;
    { char *lo = (char *)(((uintptr_t)m + pg + RT_STACK_HUGE - 1) & ~(uintptr_t)(RT_STACK_HUGE - 1)), *hi = lo + sz, *end = m + total;
      if (mprotect(lo - pg, pg, PROT_NONE) != 0) { munmap(m, total); return (char *)0; }
      if (lo - pg > m) munmap(m, (size_t)(lo - pg - m));
      if (end > hi) munmap(hi, (size_t)(end - hi));
      (void)madvise(lo, sz, MADV_HUGEPAGE);
      rt_gc_stack_top_adopt(hi); core_stack_floor_raised = 1;
      return hi - 8; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_stack_overflow_init(void)
{
    void *sp = ct_alloc(65536);
    if (!sp) return;
    stack_t ss; ss.ss_sp = sp; ss.ss_size = 65536; ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) != 0) return;
    struct sigaction sa; memset(&sa, 0, sizeof sa); sa.sa_sigaction = rt_stack_overflow_sig; sa.sa_flags = SA_SIGINFO | SA_ONSTACK; sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
}
