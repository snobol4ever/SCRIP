#define _GNU_SOURCE
#include <signal.h>
#include <ucontext.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_stack_overflow_sig(int sig, siginfo_t *si, void *uctx)
{
    ucontext_t *uc = (ucontext_t *)uctx;
    uintptr_t fault = (uintptr_t)si->si_addr, rsp = (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
    pthread_attr_t attr; void *lo_p = NULL; size_t sz = 0; int have = 0;
    if (pthread_getattr_np(pthread_self(), &attr) == 0) { have = (pthread_attr_getstack(&attr, &lo_p, &sz) == 0); pthread_attr_destroy(&attr); }
    if (have) { uintptr_t lo = (uintptr_t)lo_p, hi = lo + (uintptr_t)sz, guard = 16UL * 1024 * 1024;
        if (fault < lo && fault + guard >= lo && rsp + guard >= lo && rsp <= hi) {
            static const char msg[] = "scrip: runtime error: ERROR 246 -- stack overflow (unbounded or too-deep recursion exhausted the call stack)\n";
            write(2, msg, sizeof msg - 1); _exit(1); } }
    signal(sig, SIG_DFL);
    raise(sig);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_stack_overflow_init(void)
{
    void *sp = malloc(65536);
    if (!sp) return;
    stack_t ss; ss.ss_sp = sp; ss.ss_size = 65536; ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) != 0) return;
    struct sigaction sa; memset(&sa, 0, sizeof sa); sa.sa_sigaction = rt_stack_overflow_sig; sa.sa_flags = SA_SIGINFO | SA_ONSTACK; sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
}
