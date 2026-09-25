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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_stack_overflow_sig(int sig, siginfo_t *si, void *uctx)
{
    ucontext_t *uc = (ucontext_t *)uctx;
    uintptr_t fault = (uintptr_t)si->si_addr, rsp = (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
    pthread_attr_t attr; void *lo_p = NULL; size_t sz = 0; int have = 0;
    if (rt_gc_stale_addr_report(si->si_addr, (void *)uc->uc_mcontext.gregs[REG_RIP])) { signal(sig, SIG_DFL); raise(sig); }
    { extern int rt_gc_poison_reg_report(const long long *gregs, int n, void *ip); if (rt_gc_poison_reg_report((const long long *)uc->uc_mcontext.gregs, (int)NGREG, (void *)uc->uc_mcontext.gregs[REG_RIP])) { signal(sig, SIG_DFL); raise(sig); } }
    if (pthread_getattr_np(pthread_self(), &attr) == 0) { have = (pthread_attr_getstack(&attr, &lo_p, &sz) == 0); pthread_attr_destroy(&attr); }
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
__attribute__((constructor)) static void rt_stack_overflow_init(void)
{
    void *sp = ct_alloc(65536);
    if (!sp) return;
    stack_t ss; ss.ss_sp = sp; ss.ss_size = 65536; ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) != 0) return;
    struct sigaction sa; memset(&sa, 0, sizeof sa); sa.sa_sigaction = rt_stack_overflow_sig; sa.sa_flags = SA_SIGINFO | SA_ONSTACK; sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
}
