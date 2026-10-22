#define _GNU_SOURCE
#include <stdio.h>
#include "g_runtime.h"
#include "ct_arena.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>
#include <ucontext.h>
#include <time.h>
#include <dlfcn.h>
#include <unistd.h>
#include "bbprof.h"
#include "rt_diag.h"
#if RT_DIAG
typedef struct g_bbprof_e { uintptr_t lo, hi; int32_t nid, kind, uid; uint64_t direct, viac; } bbprof_e;
typedef struct g_bbprof_pc bbprof_pc;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bbprof_on(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_BBPROF"); p = (e && *e && *e != '0') ? 1 : 0; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bbprof_record(int nid, int kind, int uid, void *lo, void *hi) {
    if (!bbprof_on() || !lo || hi <= lo) return;
    if (g_runtime.bbprof.n == g_runtime.bbprof.cap) {
        g_runtime.bbprof.cap = g_runtime.bbprof.cap ? g_runtime.bbprof.cap * 2 : 1024;
        g_runtime.bbprof.tab = (bbprof_e *)ct_grow(g_runtime.bbprof.tab, (size_t)g_runtime.bbprof.cap * sizeof(bbprof_e));
        if (!g_runtime.bbprof.tab) { g_runtime.bbprof.cap = g_runtime.bbprof.n = 0; return; }
    }
    g_runtime.bbprof.tab[g_runtime.bbprof.n].lo = (uintptr_t)lo;
    g_runtime.bbprof.tab[g_runtime.bbprof.n].hi = (uintptr_t)hi;
    g_runtime.bbprof.tab[g_runtime.bbprof.n].nid = nid;
    g_runtime.bbprof.tab[g_runtime.bbprof.n].kind = kind;
    g_runtime.bbprof.tab[g_runtime.bbprof.n].uid = uid;
    g_runtime.bbprof.tab[g_runtime.bbprof.n].direct = 0;
    g_runtime.bbprof.tab[g_runtime.bbprof.n].viac = 0;
    g_runtime.bbprof.n++;
    g_runtime.bbprof.sorted = 0;
    if (g_runtime.bbprof.armed) g_runtime.bbprof.late_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbp_skip_line(FILE *fp) { int c, k = 0; while ((c = fgetc(fp)) != EOF && c != 10) k = 1; return (c == EOF && !k) ? -1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t bbp_exe_len(void) { for (size_t cap = 256;; cap *= 2) { char b[cap]; ssize_t el = readlink("/proc/self/exe", b, cap); if (el < 0) return 0; if ((size_t)el < cap) return el; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_cmp(const void *a, const void *b) { uintptr_t x = ((const bbprof_e *)a)->lo, y = ((const bbprof_e *)b)->lo; return x < y ? -1 : x > y ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_find(uintptr_t pc) {
    int lo = 0, hi = g_runtime.bbprof.live_n - 1;
    while (lo <= hi) { int m = (lo + hi) >> 1; if (pc < g_runtime.bbprof.live_tab[m].lo) hi = m - 1; else if (pc >= g_runtime.bbprof.live_tab[m].hi) lo = m + 1; else return m; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbprof_pc_note(uintptr_t pc) {
    uint32_t h = (uint32_t)((pc >> 4) * 2654435761u) & (BBPROF_PC_CAP - 1);
    for (int i = 0; i < BBPROF_PC_CAP; i++) {
        uint32_t k = (h + (uint32_t)i) & (BBPROF_PC_CAP - 1);
        if (g_runtime.bbprof.pcs[k].pc == pc) { g_runtime.bbprof.pcs[k].n++; return; }
        if (g_runtime.bbprof.pcs[k].pc == 0) { g_runtime.bbprof.pcs[k].pc = pc; g_runtime.bbprof.pcs[k].n = 1; return; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbprof_sig(int sig, siginfo_t *si, void *uctx) {
    (void)sig;
    (void)si;
    ucontext_t *uc = (ucontext_t *)uctx;
    uintptr_t rip = (uintptr_t)uc->uc_mcontext.gregs[REG_RIP];
    uintptr_t rsp = (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
    g_runtime.bbprof.total++;
    int i = bbprof_find(rip);
    if (i >= 0) { g_runtime.bbprof.live_tab[i].direct++; return; }
    g_runtime.bbprof.c_samples++;
    bbprof_pc_note(rip);
    uintptr_t *w = (uintptr_t *)(rsp & ~(uintptr_t)7);
    for (int d = 0; d < 512; d++) { uintptr_t v; v = w[d]; int j = bbprof_find(v); if (j >= 0) { g_runtime.bbprof.live_tab[j].viac++; return; } }
    g_runtime.bbprof.unattr++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bbprof_start(void) {
    if (!bbprof_on() || g_runtime.bbprof.armed || g_runtime.bbprof.n == 0) return;
    qsort(g_runtime.bbprof.tab, (size_t)g_runtime.bbprof.n, sizeof(bbprof_e), bbprof_cmp);
    g_runtime.bbprof.sorted = 1;
    g_runtime.bbprof.live_tab = g_runtime.bbprof.tab;
    g_runtime.bbprof.live_n = g_runtime.bbprof.n;
    g_runtime.bbprof.late_n = 0;
    stack_t ss;
    ss.ss_sp = g_runtime.bbprof.altstack;
    ss.ss_size = sizeof g_runtime.bbprof.altstack;
    ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) != 0) { perror("bbprof: sigaltstack"); return; }
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = bbprof_sig;
    sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGPROF, &sa, NULL) != 0) { perror("bbprof: sigaction"); return; }
    long us = 500;
    { const char *e = getenv("SCRIP_BBPROF_US"); if (e && *e) { long v = atol(e); if (v >= 50 && v <= 100000) us = v; } }
    struct sigevent sev;
    memset(&sev, 0, sizeof sev);
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGPROF;
    if (timer_create(CLOCK_THREAD_CPUTIME_ID, &sev, &g_runtime.bbprof.timer) == 0) {
        struct itimerspec its;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = us * 1000L;
        its.it_value = its.it_interval;
        if (timer_settime(g_runtime.bbprof.timer, 0, &its, NULL) == 0) g_runtime.bbprof.timer_ok = 1;
        else perror("bbprof: timer_settime");
    }
    if (!g_runtime.bbprof.timer_ok) {
        struct itimerval it;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_usec = us;
        it.it_value = it.it_interval;
        if (setitimer(ITIMER_PROF, &it, NULL) != 0) { perror("bbprof: setitimer"); return; }
    }
    g_runtime.bbprof.armed = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_rank(const void *a, const void *b) {
    const bbprof_e *x = *(const bbprof_e *const *)a, *y = *(const bbprof_e *const *)b;
    uint64_t xs = x->direct + x->viac, ys = y->direct + y->viac;
    return xs < ys ? 1 : xs > ys ? -1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_pc_rank(const void *a, const void *b) { uint64_t x = ((const bbprof_pc *)a)->n, y = ((const bbprof_pc *)b)->n; return x < y ? 1 : x > y ? -1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bbprof_report(void) {
    if (!g_runtime.bbprof.armed) return;
    if (g_runtime.bbprof.timer_ok) {
        struct itimerspec z;
        memset(&z, 0, sizeof z);
        timer_settime(g_runtime.bbprof.timer, 0, &z, NULL);
    } else {
        struct itimerval it;
        memset(&it, 0, sizeof it);
        setitimer(ITIMER_PROF, &it, NULL);
    }
    g_runtime.bbprof.armed = 0;
    {
        const char *e = getenv("SCRIP_BBPROF_MAP");
        if (e && *e == '1') {
            extern const char *bb_op_name(int);
            for (int i = 0; i < g_runtime.bbprof.n; i++)
                fprintf(stderr, "[BBMAP] %#lx %#lx %s %d %d\n", (unsigned long)g_runtime.bbprof.tab[i].lo, (unsigned long)g_runtime.bbprof.tab[i].hi, bb_op_name(g_runtime.bbprof.tab[i].kind),
                g_runtime.bbprof.tab[i].nid, g_runtime.bbprof.tab[i].uid) ;
        }
    }
    if (g_runtime.bbprof.total == 0) { fprintf(stderr, "[BBPROF] 0 samples\n"); return; }
    extern const char *bb_op_name(int);
    bbprof_e **rank = (bbprof_e **)ct_alloc((size_t)g_runtime.bbprof.n * sizeof(bbprof_e *));
    for (int i = 0; i < g_runtime.bbprof.n; i++) rank[i] = &g_runtime.bbprof.tab[i];
    qsort(rank, (size_t)g_runtime.bbprof.n, sizeof(bbprof_e *), bbprof_rank);
    uint64_t viac_total = g_runtime.bbprof.c_samples - g_runtime.bbprof.unattr, box_total = 0;
    for (int i = 0; i < g_runtime.bbprof.n; i++) box_total += g_runtime.bbprof.tab[i].direct + g_runtime.bbprof.tab[i].viac;
    fprintf(stderr, "[BBPROF] samples=%llu boxes=%d box-attributed=%llu (%.1f%%: direct-in-box %llu + via-C %llu) unattributed=%llu late-boxes=%d\n", (unsigned long long)g_runtime.bbprof.total,
        g_runtime.bbprof.live_n, (unsigned long long)box_total, 100.0 * (double)box_total / (double)g_runtime.bbprof.total, (unsigned long long)(box_total - viac_total),
        (unsigned long long)viac_total, (unsigned long long)g_runtime.bbprof.unattr, g_runtime.bbprof.late_n);
    fprintf(stderr, "[BBPROF] %7s %7s %7s  %-28s %6s %6s  %s\n", "total%", "direct", "via-C", "IR kind", "nid", "uid", "range");
    int shown = 0;
    for (int i = 0; i < g_runtime.bbprof.n && shown < 40; i++) {
        uint64_t s = rank[i]->direct + rank[i]->viac;
        if (s == 0) break;
        fprintf(stderr, "[BBPROF] %6.2f%% %7llu %7llu  %-28s %6d %6d  [%#lx,%#lx)\n", 100.0 * (double)s / (double)g_runtime.bbprof.total, (unsigned long long)rank[i]->direct,
            (unsigned long long)rank[i]->viac, bb_op_name(rank[i]->kind), rank[i]->nid, rank[i]->uid, (unsigned long)rank[i]->lo, (unsigned long)rank[i]->hi);
        shown++;
    }
    {
        int mk = 0;
        for (int i = 0; i < g_runtime.bbprof.n; i++) if (g_runtime.bbprof.tab[i].kind > mk) mk = g_runtime.bbprof.tab[i].kind;
        uint64_t by_kind_n[mk + 1];
        memset(by_kind_n, 0, sizeof by_kind_n);
        for (int i = 0; i < g_runtime.bbprof.n; i++) { int k = g_runtime.bbprof.tab[i].kind; if (k >= 0) by_kind_n[k] += g_runtime.bbprof.tab[i].direct + g_runtime.bbprof.tab[i].viac; }
        fprintf(stderr, "[BBPROF] -- by IR kind --\n");
        for (int pass = 0; pass < 12; pass++) {
            int best = -1;
            uint64_t bv = 0;
            for (int k = 0; k <= mk; k++) if (by_kind_n[k] > bv) { bv = by_kind_n[k]; best = k; }
            if (best < 0 || bv == 0) break;
            fprintf(stderr, "[BBPROF] %6.2f%%  %s\n", 100.0 * (double)bv / (double)g_runtime.bbprof.total, bb_op_name(best));
            by_kind_n[best] = 0;
        }
    }
    qsort(g_runtime.bbprof.pcs, BBPROF_PC_CAP, sizeof(bbprof_pc), bbprof_pc_rank);
    fprintf(stderr, "[BBPROF] -- top C sites (%llu samples outside boxes; via-C above re-attributes %llu of them) --\n", (unsigned long long)g_runtime.bbprof.c_samples,
        (unsigned long long)(g_runtime.bbprof.c_samples - g_runtime.bbprof.unattr));
    {
        ssize_t xl = bbp_exe_len();
        char exe[xl + 1];
        ssize_t el = readlink("/proc/self/exe", exe, (size_t)xl + 1);
        if (el <= 0 || el > xl) el = 0;
        exe[el] = 0;
        char cmd[fmt_len("addr2line -f -C -e '%s'", exe[0] ? exe : "/proc/self/exe") + 15 * 24];
        int cl = snprintf(cmd, sizeof cmd, "addr2line -f -C -e '%s'", exe[0] ? exe : "/proc/self/exe");
        int npc = 0;
        for (int i = 0; i < 15 && g_runtime.bbprof.pcs[i].n > 0; i++) { cl += snprintf(cmd + cl, sizeof cmd - (size_t)cl, " %#lx", (unsigned long)g_runtime.bbprof.pcs[i].pc); npc++; }
        FILE *fp = npc ? popen(cmd, "r") : NULL;
        int live = fp ? 1 : 0;
        for (int i = 0; i < npc; i++) {
            Dl_info di;
            const char *nm = "?";
            uintptr_t off = 0;
            int c = live ? fgetc(fp) : EOF;
            if (c == EOF) live = 0;
            if (live && c != '?' && c != 10) {
                fprintf(stderr, "[BBPROF] %6.2f%%  ", 100.0 * (double)g_runtime.bbprof.pcs[i].n / (double)g_runtime.bbprof.total);
                while (c != EOF && c != 10) { fputc(c, stderr); c = fgetc(fp); }
                fprintf(stderr, "+%#lx (%#lx)\n", 0UL, (unsigned long)g_runtime.bbprof.pcs[i].pc);
                if (c == EOF || bbp_skip_line(fp) < 0) live = 0;
                continue;
            }
            if (live && c != 10 && bbp_skip_line(fp) < 0) live = 0;
            if (live && bbp_skip_line(fp) < 0) live = 0;
            if (dladdr((void *)g_runtime.bbprof.pcs[i].pc, &di) && di.dli_sname) { nm = di.dli_sname; off = g_runtime.bbprof.pcs[i].pc - (uintptr_t)di.dli_saddr; }
            fprintf(stderr, "[BBPROF] %6.2f%%  %s+%#lx (%#lx)\n", 100.0 * (double)g_runtime.bbprof.pcs[i].n / (double)g_runtime.bbprof.total, nm, (unsigned long)off,
                (unsigned long)g_runtime.bbprof.pcs[i].pc);
        }
        if (fp) pclose(fp);
    }
    ct_drop(rank);
}
#else
int bbprof_on(void) { return 0; }
void bbprof_record(int nid, int kind, int uid, void *lo, void *hi) { (void)nid; (void)kind; (void)uid; (void)lo; (void)hi; }
void bbprof_start(void) { }
void bbprof_report(void) { }
#endif
