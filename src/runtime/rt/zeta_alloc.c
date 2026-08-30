#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "zeta_choices.h"
#include "rt_slab.h"
#include "rt_arena.h"
#include "zeta_alloc.h"
#define ZLS_HDR 16L
#define ZLS_ZB_BYTES ((size_t)ZC_ZBLOCK_KB << 10)
#define ZBF_WS 1L
static rt_arena_t g_zls_zb;
static int g_zls_zb_up = 0;
static void *g_zls_cur = (void *)0;
static long  g_zls_allocs = 0;
static long  g_zls_releases = 0;
static long  g_zls_nonhead = 0;
static long  g_zls_bytes = 0;
static int   g_zls_report_reg = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_zls_report(void)
{
    long depth = 0; void *it = g_zls_cur; while (it) { depth += 1; it = ((void **)((char *)it - ZLS_HDR))[0]; }
    if (!getenv("SCRIP_ZETA_TELEM") && !getenv("SCRIP_ZLS_LIFO_PROBE")) return;
    fprintf(stderr, "[ZLS] chain_depth=%ld live=%ld %s\n", depth, g_zls_allocs - g_zls_releases, depth == (g_zls_allocs - g_zls_releases) ? "COHERENT" : "ORPHANED");
    fprintf(stderr, "[ZLS] zblock=%ldKB allocs=%ld releases=%ld live=%ld nonhead=%ld bytes=%ld\n",
            (long)ZC_ZBLOCK_KB, g_zls_allocs, g_zls_releases, g_zls_allocs - g_zls_releases, g_zls_nonhead, g_zls_bytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_alloc(long bytes)
{
    long sz = (bytes + 15L) & ~15L;
    long flag = 0L;
    char *base;
    extern int g_gc_pending; extern long rt_gc_collect(void);
    if (g_gc_pending) { g_gc_pending = 0; rt_gc_collect(); }
    if (!g_zls_report_reg) { g_zls_report_reg = 1; atexit(rt_zls_report); }
    if ((size_t)(ZLS_HDR + sz) <= ZLS_ZB_BYTES) {
        if (!g_zls_zb_up) { rt_arena_init(&g_zls_zb, A_COEXPR); g_zls_zb_up = 1; }
        base = (char *)rt_arena_zblock_get(&g_zls_zb, ZLS_ZB_BYTES);
    } else {
        base = (char *)rt_ws_alloc((size_t)(ZLS_HDR + sz));
        flag = ZBF_WS;
    }
    if (!base) { fprintf(stderr, "[ZLS] activation alloc failed (%ld bytes)\n", ZLS_HDR + sz); abort(); }
    memset(base, 0, (size_t)(ZLS_HDR + sz));
    ((void **)base)[0] = g_zls_cur;
    ((long *)base)[1] = sz | flag;
    g_zls_cur = (void *)(base + ZLS_HDR);
    g_zls_allocs += 1;
    g_zls_bytes += (long)(ZLS_HDR + sz);
    return (void *)(base + ZLS_HDR);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_frames_head(void) { return g_zls_cur; }
void *rt_zls_frame_prev(void *fb) { return fb ? ((void **)((char *)fb - ZLS_HDR))[0] : (void *)0; }
long rt_zls_frame_size(void *fb) { return fb ? (((long *)((char *)fb - ZLS_HDR))[1] & ~15L) : 0L; }
static int rt_zls_poison(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_ZLS_POISON"); p = e ? (atoi(e) != 0) : 1; } return p; }
static int rt_zls_reltrace(void) { static int p = -1; if (p < 0) p = getenv("SCRIP_ZLS_RELEASE_TRACE") != NULL; return p; }
static int rt_zls_arbtrace(void) { static int p = -1; if (p < 0) p = getenv("SCRIP_ARBNO_STEP1_TRACE") != NULL; return p; }
static int rt_zls2_tron(void) { static int p = -1; if (p < 0) p = getenv("SCRIP_ZLS2_TRACE") != NULL; return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls_release(void *fb)
{
    char *base;
    if (!fb) return;
    base = (char *)fb - ZLS_HDR;
    if (rt_zls_reltrace()) fprintf(stderr, "[ZLS-RELEASE] fb=%p sz=%ld fn=%p (call #%ld)\n", fb, ((long *)base)[1], ((void **)fb)[0], g_zls_releases + 1);
    g_zls_releases += 1;
    if (fb == g_zls_cur) { g_zls_cur = ((void **)base)[0]; }
    else { void *it = g_zls_cur; g_zls_nonhead += 1; while (it && rt_zls_frame_prev(it) != fb) it = rt_zls_frame_prev(it); if (!it) return; ((void **)((char *)it - ZLS_HDR))[0] = ((void **)base)[0]; }
    { long rawsz = ((long *)base)[1]; long psz = rawsz & ~15L;
      if (rt_zls_poison()) memset(base, 0xDD, (size_t)(ZLS_HDR + psz));
      if (!(rawsz & ZBF_WS)) rt_arena_zblock_put(&g_zls_zb, base); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_mark(void) { if (rt_zls_reltrace()) fprintf(stderr, "[ZLS-MARK] returning g_zls_cur=%p\n", g_zls_cur); return g_zls_cur; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls_release_to(void *mark)
{
    if (rt_zls_reltrace()) fprintf(stderr, "[ZLS-RELEASE-TO] called with mark=%p, g_zls_cur=%p\n", mark, g_zls_cur);
    while (g_zls_cur != mark) {
        void *cur = g_zls_cur;
        if (!cur) break;
        rt_zls_release(cur);
    }
}
static void *g_zls_arbno_step1_carrier = (void *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls_arbno_step1_store(void *p) { if (rt_zls_arbtrace()) fprintf(stderr, "[ARBNO-S1] STORE %p (prev carrier was %p)\n", p, g_zls_arbno_step1_carrier); g_zls_arbno_step1_carrier = p; }
void *rt_zls_arbno_step1_load(void) { if (rt_zls_arbtrace()) fprintf(stderr, "[ARBNO-S1] LOAD  %p\n", g_zls_arbno_step1_carrier); return g_zls_arbno_step1_carrier; }
static char *g_zls2_lo = (char *)0;
static char *g_zls2_hi = (char *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls2_init(void)
{
    if (!g_zls2_hi) {
        long mb = (long)ZC_ZLS2_MB;
        g_zls2_lo = (char *)rt_slab_region((size_t)mb << 20);
        if (!g_zls2_lo) { fprintf(stderr, "[ZLS2] arena slab failed (%ld MB) — lower ZC_ZLS2_MB\n", mb); abort(); }
        g_zls2_hi = g_zls2_lo + ((size_t)mb << 20);
    }
    return (void *)g_zls2_hi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls2_lo(void) { return (void *)g_zls2_lo; }
char *g_zls2_cur = (char *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls2_push(long k)
{
    if (!g_zls2_cur) { rt_zls2_init(); g_zls2_cur = g_zls2_hi; }
    if (g_zls2_cur - k < g_zls2_lo) { fprintf(stderr, "[ZLS2] arena exhausted on push(%ld) — raise ZC_ZLS2_MB\n", k); abort(); }
    g_zls2_cur -= k;
    if (rt_zls2_tron()) fprintf(stderr, "[ZLS2] PUSH %ld -> cur=%p (used=%ld)\n", k, (void *)g_zls2_cur, (long)(g_zls2_hi - g_zls2_cur));
    return (void *)g_zls2_cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_zls2_poison(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_ZLS2_POISON"); p = e ? (atoi(e) != 0) : 1; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls2_pop(long k)
{
    if (!g_zls2_cur || g_zls2_cur + k > g_zls2_hi) { fprintf(stderr, "[ZLS2] pop(%ld) past arena top — unbalanced push/pop (LIFO discipline violated)\n", k); abort(); }
    if (rt_zls2_poison()) memset(g_zls2_cur, 0xDD, (size_t)k);
    g_zls2_cur += k;
    if (rt_zls2_tron()) fprintf(stderr, "[ZLS2] POP  %ld -> cur=%p (used=%ld)\n", k, (void *)g_zls2_cur, (long)(g_zls2_hi - g_zls2_cur));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls2_mark(void)
{
    if (!g_zls2_cur) { rt_zls2_init(); g_zls2_cur = g_zls2_hi; }
    if (rt_zls2_tron()) fprintf(stderr, "[ZLS2] MARK cur=%p\n", (void *)g_zls2_cur);
    return (void *)g_zls2_cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls2_release_to(void *mark)
{
    char *m = (char *)mark;
    if (!m) return;
    if (m < g_zls2_cur) { fprintf(stderr, "[ZLS2] release_to(%p) BELOW cursor %p — LIFO discipline violated\n", mark, (void *)g_zls2_cur); abort(); }
    if (m > g_zls2_hi)  { fprintf(stderr, "[ZLS2] release_to(%p) past arena top %p\n", mark, (void *)g_zls2_hi); abort(); }
    if (rt_zls2_tron() && m != g_zls2_cur) fprintf(stderr, "[ZLS2] RELEASE_TO %p (reclaimed %ld)\n", mark, (long)(m - g_zls2_cur));
    if (m != g_zls2_cur && rt_zls2_poison()) memset(g_zls2_cur, 0xDD, (size_t)(m - g_zls2_cur));
    g_zls2_cur = m;
}
int g_zeta_mode = (int)ZC_ZETA;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
