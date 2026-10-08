#include <cstdio>
#include "ct_arena.h"
#include <cstdlib>
#include <cstring>
#include "emit.h"
#include "portcount.h"
#include "rt_diag.h"
#if RT_DIAG
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
        long n = uid + 1024; unsigned long * t = (unsigned long *)ct_grow(g_emit.pl_trace_lastn, (size_t)n * sizeof *t);
        if (!t) return NULL;
        memset(t + g_emit.pl_trace_lastn_cap, 0, (size_t)(n - g_emit.pl_trace_lastn_cap) * sizeof *t);
        g_emit.pl_trace_lastn = t; g_emit.pl_trace_lastn_cap = n;
    }
    return &g_emit.pl_trace_lastn[uid];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" __attribute__((force_align_arg_pointer)) void rt_pl_port_trace(const char * stem, const char * target, long ev, long uid, long ball) {
    int mode = plt_mode(); if (mode < 0) return;
    int port = (int)(ev & 3L);
    if (mode & 2) {
        if (!g_emit.pl_trace_atexit) { g_emit.pl_trace_atexit = 1; atexit(plt_report); }
        uint64_t * c = rt_port_counts_cell((int)uid, port, stem); if (c) (*c)++;
    }
    if (!(mode & 1)) return;
    unsigned long n = 0; unsigned long * ln = plt_lastn(uid); long depth = g_emit.pl_trace_depth;
    if (port == 0) { n = ++g_emit.pl_trace_n; if (ln) *ln = n; g_emit.pl_trace_depth++; }
    else if (port == 1) { n = ln ? *ln : 0; g_emit.pl_trace_depth++; }
    else { n = ln ? *ln : 0; if (g_emit.pl_trace_depth > 0) depth = --g_emit.pl_trace_depth; }
    const char * pn = port == 0 ? "Call" : port == 1 ? "Redo" : port == 2 ? "Exit" : "Fail";
    if (target && port == 3 && ball) fprintf(stderr, "(%lu) %ld %s: %s -> %s r15=0x%lx\n", n, depth, pn, stem ? stem : "?", target, (unsigned long)ball);
    else if (target) fprintf(stderr, "(%lu) %ld %s: %s -> %s\n", n, depth, pn, stem ? stem : "?", target);
    else fprintf(stderr, "(%lu) %ld %s: %s\n", n, depth, pn, stem ? stem : "?");
}
#else
extern "C" __attribute__((force_align_arg_pointer)) void rt_pl_port_trace(const char * stem, const char * target, long ev, long uid, long ball) {
    (void)stem; (void)target; (void)ev; (void)uid; (void)ball;
}
#endif
