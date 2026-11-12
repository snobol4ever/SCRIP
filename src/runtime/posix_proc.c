#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "driver/driver_globals.h"
#include "posix_leaves.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/rt_list_view.h"
#include "runtime/core/g_runtime.h"
#include "runtime/core/g_runtime_misc.h"
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
DESCR_t bt_ICNx24UFORK(DESCR_t *args, int nargs) {
    pid_t p;
    (void) args;
    px_begin();
    if (nargs != 0) return FAILDESCR;
    fflush(NULL);
    setvbuf(stdout, NULL, _IOLBF, 0);
    p = fork();
    return p < 0 ? px_fail() : INTVAL((int64_t) p);
}
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
static void px_sig_pend(int sig) { __atomic_fetch_or(&g_rt_misc.pending_signals, 1ULL << (sig - 1), __ATOMIC_SEQ_CST); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USIGTRAP(DESCR_t *args, int nargs) {
    int sig = nargs == 2 ? px_signal_number(args[0]) : -1;
    struct sigaction sa = { 0 };
    px_begin();
    if (nargs != 2 || sig <= 0 || sig > 64 || sig == SIGKILL || sig == SIGSTOP) return FAILDESCR;
    sa.sa_handler = IS_NULL_fn(args[1]) ? SIG_DFL : px_sig_pend;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    return sigaction(sig, &sa, NULL) != 0 ? px_fail() : INTVAL(sig);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USIGTAKE(DESCR_t *args, int nargs) {
    unsigned long long p;
    int b;
    (void) args;
    if (nargs != 0) return FAILDESCR;
    p = __atomic_load_n(&g_rt_misc.pending_signals, __ATOMIC_SEQ_CST);
    if (p == 0) return FAILDESCR;
    b = __builtin_ctzll(p);
    __atomic_fetch_and(&g_rt_misc.pending_signals, ~(1ULL << b), __ATOMIC_SEQ_CST);
    return INTVAL(b + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USIGNAME(DESCR_t *args, int nargs) {
    const char *name;
    if (nargs != 1 || args[0].v != DT_I) return FAILDESCR;
    name = px_signal_name((int) args[0].i);
    return name ? px_fmt("%s", name) : FAILDESCR;
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
    setvbuf(stdout, NULL, _IOLBF, 0);
    execvp(f, av);
    return px_fail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pp_redir(DESCR_t d, int out, int *fd, int *own) {
    *fd = -1;
    *own = 0;
    if (IS_NULL_fn(d)) return 1;
    if (d.v == DT_FH) { FILE *fp = fh_get((int) d.i); if (!fp) return 0; fflush(fp); *fd = fileno(fp); return 1; }
    if (d.v != DT_S || IS_CSET_fn(d)) return 0;
    *fd = open(rt_cstr_d(d), out ? O_WRONLY | O_CREAT | O_TRUNC : O_RDONLY, 0600);
    *own = *fd >= 0;
    return *fd >= 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pp_arg(DESCR_t d) { return d.v == DT_S && !IS_CSET_fn(d) ? rt_cstr_d(d) : d.v == DT_I ? px_fmt("%lld", (long long) d.i).s : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_ICNx24USYSTEM(DESCR_t *args, int nargs) {
    int fd[3] = { -1, -1, -1 }, own[3] = { 0, 0, 0 }, n = 0, st = 0, nowait = 0;
    DESCR_t *el;
    char **av = NULL;
    const char *cmd = NULL;
    pid_t pid;
    px_begin();
    if (nargs < 1) return FAILDESCR;
    if (rt_lv_is_list(args[0], &el, &n)) {
        av = (char **) rt_heap_alloc_c((size_t) (n + 1) * sizeof *av);
        for (int i = 0; i < n; i++) if (!(av[i] = (char *) pp_arg(el[i]))) return FAILDESCR;
        av[n] = NULL;
        if (n == 0) return FAILDESCR;
    } else if (!(cmd = pp_arg(args[0]))) return FAILDESCR;
    if (nargs >= 5) nowait = args[4].v == DT_I ? args[4].i != 0 : args[4].v == DT_S && !strcmp(rt_cstr_d(args[4]), "nowait");
    for (int i = 0; i < 3; i++) {
        DESCR_t d = i + 1 < nargs ? args[i + 1] : NULVCL;
        if (i == 2 && fd[1] >= 0 && own[1] && d.v == DT_S && args[2].v == DT_S && !strcmp(rt_cstr_d(d), rt_cstr_d(args[2]))) { fd[2] = fd[1]; continue; }
        if (!pp_redir(d, i > 0, &fd[i], &own[i])) { for (int j = 0; j < i; j++) if (own[j]) close(fd[j]); return px_fail(); }
    }
    fflush(NULL);
    setvbuf(stdout, NULL, _IOLBF, 0);
    pid = fork();
    if (pid == 0) {
        for (int i = 0; i < 3; i++) if (fd[i] >= 0 && fd[i] != i) dup2(fd[i], i);
        close_range(3, ~0U, 0);
        if (av) execvp(av[0], av);
        else execl("/bin/sh", "sh", "-c", cmd, (char *) 0);
        _exit(127);
    }
    for (int i = 0; i < 3; i++) if (own[i]) close(fd[i]);
    if (pid < 0) return px_fail();
    if (nowait) return INTVAL((int64_t) pid);
    while (waitpid(pid, &st, 0) < 0) if (errno != EINTR) return px_fail();
    return INTVAL(WIFEXITED(st) ? WEXITSTATUS(st) : st);
}
