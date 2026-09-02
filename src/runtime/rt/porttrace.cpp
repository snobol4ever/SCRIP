#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "emit.h"
#include "portcount.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plt_mode(void) {
    if (!g_emit.pl_trace_mode) {
        const char * e = getenv("SCRIP_PL_TRACE"); int armed = e && *e && *e != '0'; long v = armed ? strtol(e, 0, 10) : 0;
        g_emit.pl_trace_mode = v > 0 ? (int)v : armed ? 1 : -1;
    }
    return g_emit.pl_trace_mode;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((force_align_arg_pointer)) void plt_report(void) { rt_port_counts_report("PL-TRACE profile (Byrd four-port hits, both modes)", 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned long * plt_lastn(long uid) {
    if (uid < 0) return NULL;
    if (uid >= g_emit.pl_trace_lastn_cap) {
        long n = uid + 1024; unsigned long * t = (unsigned long *)realloc(g_emit.pl_trace_lastn, (size_t)n * sizeof *t);
        if (!t) return NULL;
        memset(t + g_emit.pl_trace_lastn_cap, 0, (size_t)(n - g_emit.pl_trace_lastn_cap) * sizeof *t);
        g_emit.pl_trace_lastn = t; g_emit.pl_trace_lastn_cap = n;
    }
    return &g_emit.pl_trace_lastn[uid];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plt_push(long uid, unsigned long n) {
    if (g_emit.pl_trace_sp + 1 > g_emit.pl_trace_stk_cap) {
        long c = g_emit.pl_trace_stk_cap ? g_emit.pl_trace_stk_cap * 2 : 256; long * t = (long *)realloc(g_emit.pl_trace_stk, (size_t)c * 2 * sizeof *t);
        if (!t) return;
        g_emit.pl_trace_stk = t; g_emit.pl_trace_stk_cap = c;
    }
    g_emit.pl_trace_stk[g_emit.pl_trace_sp * 2] = uid; g_emit.pl_trace_stk[g_emit.pl_trace_sp * 2 + 1] = (long)n; g_emit.pl_trace_sp++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plt_find(long uid) { for (long i = g_emit.pl_trace_sp - 1; i >= 0; i--) if (g_emit.pl_trace_stk[i * 2] == uid) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" __attribute__((force_align_arg_pointer)) void rt_pl_port_trace(const char * stem, const char * target, long ev, long uid, long ball) {
    int mode = plt_mode(); if (mode < 0) return;
    int port = (int)(ev & 3L);
    if (mode & 2) {
        if (!g_emit.pl_trace_atexit) { g_emit.pl_trace_atexit = 1; atexit(plt_report); }
        uint64_t * c = rt_port_counts_cell((int)uid, port, stem); if (c) (*c)++;
    }
    if (!(mode & 1)) return;
    unsigned long n = 0; long depth = g_emit.pl_trace_sp; unsigned long * ln = plt_lastn(uid);
    if (port == 0) { n = ++g_emit.pl_trace_n; if (ln) *ln = n; plt_push(uid, n); }
    else if (port == 1) { n = ln ? *ln : 0; plt_push(uid, n); }
    else { long i = plt_find(uid); if (i >= 0) { n = (unsigned long)g_emit.pl_trace_stk[i * 2 + 1]; depth = i; g_emit.pl_trace_sp = i; } else n = ln ? *ln : 0; }
    const char * pn = port == 0 ? "Call" : port == 1 ? "Redo" : port == 2 ? "Exit" : "Fail";
    if (target && port == 3 && ball) fprintf(stderr, "(%lu) %ld %s: %s -> %s r15=0x%lx\n", n, depth, pn, stem ? stem : "?", target, (unsigned long)ball);
    else if (target)                 fprintf(stderr, "(%lu) %ld %s: %s -> %s\n", n, depth, pn, stem ? stem : "?", target);
    else                             fprintf(stderr, "(%lu) %ld %s: %s\n", n, depth, pn, stem ? stem : "?");
}
