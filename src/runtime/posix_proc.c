#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "posix_leaves.h"
#include "runtime/rt/rt_arena.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *name; int sig; } px_sig_t;
static const px_sig_t px_sigs[] = { { "SIGABRT", SIGABRT }, { "SIGALRM", SIGALRM }, { "SIGBUS", SIGBUS }, { "SIGCHLD", SIGCHLD }, { "SIGCLD", SIGCHLD }, { "SIGCONT", SIGCONT }, { "SIGFPE", SIGFPE },
    { "SIGHUP", SIGHUP }, { "SIGILL", SIGILL }, { "SIGINT", SIGINT }, { "SIGIO", SIGIO }, { "SIGIOT", SIGABRT }, { "SIGKILL", SIGKILL }, { "SIGPIPE", SIGPIPE }, { "SIGPOLL", SIGPOLL }, { "SIGPROF",
    SIGPROF }, { "SIGPWR", SIGPWR }, { "SIGQUIT", SIGQUIT }, { "SIGSEGV", SIGSEGV }, { "SIGSTOP", SIGSTOP }, { "SIGSYS", SIGSYS }, { "SIGTERM", SIGTERM }, { "SIGTRAP", SIGTRAP }, { "SIGTSTP",
    SIGTSTP }, { "SIGTTIN", SIGTTIN }, { "SIGTTOU", SIGTTOU }, { "SIGURG", SIGURG }, { "SIGUSR1", SIGUSR1 }, { "SIGUSR2", SIGUSR2 }, { "SIGVTALRM", SIGVTALRM }, { "SIGWINCH", SIGWINCH }, { "SIGXCPU",
    SIGXCPU }, { "SIGXFSZ", SIGXFSZ }, };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int px_signal_number(DESCR_t d) {
    if (d.v == DT_I) return d.i >= 0 && d.i <= 50 ? (int) d.i : -1;
    if (d.v != DT_S) return -1;
    for (size_t i = 0; i < sizeof px_sigs / sizeof *px_sigs; i++) if (!strcmp(px_sigs[i].name, rt_cstr_d(d))) return px_sigs[i].sig;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *px_signal_name(int sig) { for (size_t i = 0; i < sizeof px_sigs / sizeof *px_sigs; i++) if (px_sigs[i].sig == sig) return px_sigs[i].name; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UFORK(DESCR_t *args, int nargs) { pid_t p; (void) args; px_begin(); if (nargs != 0) return FAILDESCR; fflush(NULL); p = fork(); return p < 0 ? px_fail() : INTVAL((int64_t) p); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UWAIT(DESCR_t *args, int nargs) {
    int st = 0, opt = 0;
    pid_t r, pid = nargs >= 1 && args[0].v == DT_I ? (pid_t) args[0].i : -1;
    const char *o = nargs >= 2 && args[1].v == DT_S ? rt_cstr_d(args[1]) : "";
    const char *sn;
    if (nargs >= 1 && args[0].v != DT_I && !IS_NULL_fn(args[0])) return nargs == 1 ? args[0] : FAILDESCR;
    px_begin();
    for (; *o; o++) { if (*o == 'n') opt |= WNOHANG; else if (*o == 'u') opt |= WUNTRACED; }
    r = waitpid(pid, &st, opt);
    if (r < 0) return px_fail();
    if (r == 0) { errno = EAGAIN; return px_fail(); }
    if (WIFSTOPPED(st)) { sn = px_signal_name(WSTOPSIG(st)); return px_fmt("%d stopped:%s", (int) r, sn ? sn : "(null)"); }
    if (WIFSIGNALED(st)) { sn = px_signal_name(WTERMSIG(st)); return px_fmt("%d terminated:%s%s", (int) r, sn ? sn : "(null)", WCOREDUMP(st) ? ":core" : ""); }
    return WIFEXITED(st) ? px_fmt("%d exited:%d", (int) r, WEXITSTATUS(st)) : px_fmt("???");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UKILL(DESCR_t *args, int nargs) {
    int sig = nargs == 2 ? px_signal_number(args[1]) : -1;
    px_begin();
    if (nargs != 2 || args[0].v != DT_I || sig < 0) return FAILDESCR;
    if (sig == 0) { errno = EINVAL; return px_fail(); }
    return kill((pid_t) args[0].i, sig) == 0 ? NULVCL : px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24UEXEC(DESCR_t *args, int nargs) {
    const char *f = nargs >= 1 ? px_str(args[0]) : NULL;
    char **av = (char **) rt_heap_alloc_c((size_t) (nargs + 1) * sizeof *av);
    px_begin();
    if (!f) return FAILDESCR;
    for (int i = 1; i < nargs; i++) { if (args[i].v != DT_S) return FAILDESCR; av[i - 1] = (char *) rt_cstr_d(args[i]); }
    av[nargs > 0 ? nargs - 1 : 0] = NULL;
    fflush(NULL);
    execvp(f, av);
    return px_fail();
}
