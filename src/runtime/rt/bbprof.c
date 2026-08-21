#define _GNU_SOURCE
#include <stdio.h>
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
typedef struct { uintptr_t lo, hi; int32_t nid, kind, uid; uint64_t direct, viac; } bbprof_e;
static bbprof_e *g_tab; static int g_n, g_cap, g_sorted, g_armed;
static bbprof_e *g_live_tab; static int g_live_n, g_late_n;
static uint64_t g_c_samples, g_unattr, g_total;
typedef struct { uintptr_t pc; uint64_t n; } bbprof_pc;
#define BBPROF_PC_CAP 2048
static bbprof_pc g_pcs[BBPROF_PC_CAP];
static char g_altstack[65536];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bbprof_on(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_BBPROF"); p = (e && *e && *e != '0') ? 1 : 0; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bbprof_record(int nid, int kind, int uid, void *lo, void *hi)
{
    if (!bbprof_on() || !lo || hi <= lo) return;
    if (g_n == g_cap) { g_cap = g_cap ? g_cap * 2 : 1024; g_tab = (bbprof_e *)realloc(g_tab, (size_t)g_cap * sizeof(bbprof_e)); if (!g_tab) { g_cap = g_n = 0; return; } }
    g_tab[g_n].lo = (uintptr_t)lo; g_tab[g_n].hi = (uintptr_t)hi; g_tab[g_n].nid = nid; g_tab[g_n].kind = kind; g_tab[g_n].uid = uid; g_tab[g_n].direct = 0; g_tab[g_n].viac = 0; g_n++;
    g_sorted = 0;
    if (g_armed) g_late_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_cmp(const void *a, const void *b) { uintptr_t x = ((const bbprof_e *)a)->lo, y = ((const bbprof_e *)b)->lo; return x < y ? -1 : x > y ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_find(uintptr_t pc)
{
    int lo = 0, hi = g_live_n - 1;
    while (lo <= hi) { int m = (lo + hi) >> 1; if (pc < g_live_tab[m].lo) hi = m - 1; else if (pc >= g_live_tab[m].hi) lo = m + 1; else return m; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbprof_pc_note(uintptr_t pc)
{
    uint32_t h = (uint32_t)((pc >> 4) * 2654435761u) & (BBPROF_PC_CAP - 1);
    for (int i = 0; i < BBPROF_PC_CAP; i++) { uint32_t k = (h + (uint32_t)i) & (BBPROF_PC_CAP - 1);
        if (g_pcs[k].pc == pc) { g_pcs[k].n++; return; }
        if (g_pcs[k].pc == 0) { g_pcs[k].pc = pc; g_pcs[k].n = 1; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bbprof_sig(int sig, siginfo_t *si, void *uctx)
{
    (void)sig; (void)si;
    ucontext_t *uc = (ucontext_t *)uctx;
    uintptr_t rip = (uintptr_t)uc->uc_mcontext.gregs[REG_RIP];
    uintptr_t rsp = (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
    g_total++;
    int i = bbprof_find(rip);
    if (i >= 0) { g_live_tab[i].direct++; return; }
    g_c_samples++;
    bbprof_pc_note(rip);
    uintptr_t *w = (uintptr_t *)(rsp & ~(uintptr_t)7);
    for (int d = 0; d < 512; d++) { uintptr_t v; v = w[d]; int j = bbprof_find(v); if (j >= 0) { g_live_tab[j].viac++; return; } }
    g_unattr++;
}
static timer_t g_timer; static int g_timer_ok;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bbprof_start(void)
{
    if (!bbprof_on() || g_armed || g_n == 0) return;
    qsort(g_tab, (size_t)g_n, sizeof(bbprof_e), bbprof_cmp);
    g_sorted = 1; g_live_tab = g_tab; g_live_n = g_n; g_late_n = 0;
    stack_t ss; ss.ss_sp = g_altstack; ss.ss_size = sizeof g_altstack; ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) != 0) { perror("bbprof: sigaltstack"); return; }
    struct sigaction sa; memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = bbprof_sig; sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGPROF, &sa, NULL) != 0) { perror("bbprof: sigaction"); return; }
    long us = 500; { const char *e = getenv("SCRIP_BBPROF_US"); if (e && *e) { long v = atol(e); if (v >= 50 && v <= 100000) us = v; } }
    struct sigevent sev; memset(&sev, 0, sizeof sev);
    sev.sigev_notify = SIGEV_SIGNAL; sev.sigev_signo = SIGPROF;
    if (timer_create(CLOCK_THREAD_CPUTIME_ID, &sev, &g_timer) == 0) {
        struct itimerspec its; its.it_interval.tv_sec = 0; its.it_interval.tv_nsec = us * 1000L; its.it_value = its.it_interval;
        if (timer_settime(g_timer, 0, &its, NULL) == 0) g_timer_ok = 1; else perror("bbprof: timer_settime");
    }
    if (!g_timer_ok) {
        struct itimerval it; it.it_interval.tv_sec = 0; it.it_interval.tv_usec = us; it.it_value = it.it_interval;
        if (setitimer(ITIMER_PROF, &it, NULL) != 0) { perror("bbprof: setitimer"); return; }
    }
    g_armed = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_rank(const void *a, const void *b)
{
    const bbprof_e *x = *(const bbprof_e *const *)a, *y = *(const bbprof_e *const *)b;
    uint64_t xs = x->direct + x->viac, ys = y->direct + y->viac;
    return xs < ys ? 1 : xs > ys ? -1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bbprof_pc_rank(const void *a, const void *b) { uint64_t x = ((const bbprof_pc *)a)->n, y = ((const bbprof_pc *)b)->n; return x < y ? 1 : x > y ? -1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bbprof_report(void)
{
    if (!g_armed) return;
    if (g_timer_ok) { struct itimerspec z; memset(&z, 0, sizeof z); timer_settime(g_timer, 0, &z, NULL); }
    else { struct itimerval it; memset(&it, 0, sizeof it); setitimer(ITIMER_PROF, &it, NULL); }
    g_armed = 0;
    { const char *e = getenv("SCRIP_BBPROF_MAP"); if (e && *e == '1') { extern const char *bb_op_name(int);
        for (int i = 0; i < g_n; i++) fprintf(stderr, "[BBMAP] %#lx %#lx %s %d %d\n", (unsigned long)g_tab[i].lo, (unsigned long)g_tab[i].hi, bb_op_name(g_tab[i].kind), g_tab[i].nid, g_tab[i].uid); } }
    if (g_total == 0) { fprintf(stderr, "[BBPROF] 0 samples\n"); return; }
    extern const char *bb_op_name(int);
    bbprof_e **rank = (bbprof_e **)malloc((size_t)g_n * sizeof(bbprof_e *));
    for (int i = 0; i < g_n; i++) rank[i] = &g_tab[i];
    qsort(rank, (size_t)g_n, sizeof(bbprof_e *), bbprof_rank);
    uint64_t viac_total = g_c_samples - g_unattr, box_total = 0;
    for (int i = 0; i < g_n; i++) box_total += g_tab[i].direct + g_tab[i].viac;
    fprintf(stderr, "[BBPROF] samples=%llu boxes=%d box-attributed=%llu (%.1f%%: direct-in-box %llu + via-C %llu) unattributed=%llu late-boxes=%d\n",
        (unsigned long long)g_total, g_live_n, (unsigned long long)box_total, 100.0 * (double)box_total / (double)g_total,
        (unsigned long long)(box_total - viac_total), (unsigned long long)viac_total, (unsigned long long)g_unattr, g_late_n);
    fprintf(stderr, "[BBPROF] %7s %7s %7s  %-28s %6s %6s  %s\n", "total%", "direct", "via-C", "IR kind", "nid", "uid", "range");
    int shown = 0;
    for (int i = 0; i < g_n && shown < 40; i++) {
        uint64_t s = rank[i]->direct + rank[i]->viac;
        if (s == 0) break;
        fprintf(stderr, "[BBPROF] %6.2f%% %7llu %7llu  %-28s %6d %6d  [%#lx,%#lx)\n",
            100.0 * (double)s / (double)g_total, (unsigned long long)rank[i]->direct, (unsigned long long)rank[i]->viac,
            bb_op_name(rank[i]->kind), rank[i]->nid, rank[i]->uid, (unsigned long)rank[i]->lo, (unsigned long)rank[i]->hi);
        shown++;
    }
    { uint64_t by_kind_n[512]; memset(by_kind_n, 0, sizeof by_kind_n); int mk = 0;
      for (int i = 0; i < g_n; i++) { int k = g_tab[i].kind; if (k >= 0 && k < 512) { by_kind_n[k] += g_tab[i].direct + g_tab[i].viac; if (k > mk) mk = k; } }
      fprintf(stderr, "[BBPROF] -- by IR kind --\n");
      for (int pass = 0; pass < 12; pass++) { int best = -1; uint64_t bv = 0;
          for (int k = 0; k <= mk; k++) if (by_kind_n[k] > bv) { bv = by_kind_n[k]; best = k; }
          if (best < 0 || bv == 0) break;
          fprintf(stderr, "[BBPROF] %6.2f%%  %s\n", 100.0 * (double)bv / (double)g_total, bb_op_name(best));
          by_kind_n[best] = 0; } }
    qsort(g_pcs, BBPROF_PC_CAP, sizeof(bbprof_pc), bbprof_pc_rank);
    fprintf(stderr, "[BBPROF] -- top C sites (%llu samples outside boxes; via-C above re-attributes %llu of them) --\n",
        (unsigned long long)g_c_samples, (unsigned long long)(g_c_samples - g_unattr));
    { char exe[512]; ssize_t el = readlink("/proc/self/exe", exe, sizeof exe - 1); if (el <= 0) el = 0; exe[el] = 0;
      char cmd[4096]; int cl = snprintf(cmd, sizeof cmd, "addr2line -f -C -e '%s'", exe[0] ? exe : "/proc/self/exe");
      int npc = 0; for (int i = 0; i < 15 && g_pcs[i].n > 0; i++) { cl += snprintf(cmd + cl, sizeof cmd - (size_t)cl, " %#lx", (unsigned long)g_pcs[i].pc); npc++; }
      char names[15][96]; for (int i = 0; i < 15; i++) names[i][0] = 0;
      FILE *fp = npc ? popen(cmd, "r") : NULL;
      if (fp) { char l1[256], l2[256]; for (int i = 0; i < npc; i++) { if (!fgets(l1, sizeof l1, fp) || !fgets(l2, sizeof l2, fp)) break; l1[strcspn(l1, "\n")] = 0; if (l1[0] && l1[0] != '?') snprintf(names[i], sizeof names[i], "%s", l1); } pclose(fp); }
      for (int i = 0; i < npc; i++) {
        Dl_info di; const char *nm = names[i][0] ? names[i] : "?"; uintptr_t off = 0;
        if (!names[i][0] && dladdr((void *)g_pcs[i].pc, &di) && di.dli_sname) { nm = di.dli_sname; off = g_pcs[i].pc - (uintptr_t)di.dli_saddr; }
        fprintf(stderr, "[BBPROF] %6.2f%%  %s+%#lx (%#lx)\n", 100.0 * (double)g_pcs[i].n / (double)g_total, nm, (unsigned long)off, (unsigned long)g_pcs[i].pc);
      } }
    free(rank);
}
