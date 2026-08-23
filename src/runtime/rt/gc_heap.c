#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <link.h>
#include <time.h>
#include "zeta_choices.h"
#include "rt_slab.h"
#include "rt_arena.h"
#include "gc_heap.h"
#include "descr.h"
#include "pin_va.h"
_Static_assert(sizeof(rt_hblk_t) == 16, "rt_hblk_t must be one 16-byte title unit");
typedef struct rt_hp_fr_t { char *top; char *end; long blocks; int armed; int _pad; char *virgin; int zfull; int _pad2; } rt_hp_fr_t;
rt_hp_fr_t g_hp_fr = { (char *)0, (char *)0, 0, 0, 0, (char *)0, -1, 0 };
_Static_assert(sizeof(rt_hp_fr_t) == 48, "RTX-2 extends the PL-SINK-3 cell; 0/8/16/24 stay put");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, top)    ==  0, "PL-SINK-3 bakes g_hp_fr.top @0");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, end)    ==  8, "PL-SINK-3 bakes g_hp_fr.end @8");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, blocks) == 16, "PL-SINK-3 bakes g_hp_fr.blocks @16");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, armed)  == 24, "PL-SINK-3 bakes g_hp_fr.armed @24");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, virgin) == 32, "RTX-2 bakes g_hp_fr.virgin @32");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, zfull)  == 40, "RTX-2 bakes g_hp_fr.zfull @40");
#define g_hp_top    (g_hp_fr.top)
#define g_hp_end    (g_hp_fr.end)
#define g_hp_blocks (g_hp_fr.blocks)
#define g_hp_virgin (g_hp_fr.virgin)
static char *g_hp_arena = (char *)0;
static char *g_hp_gcline = (char *)0;
static char *g_hp_win = (char *)0;
static char *g_hp_wend = (char *)0;
__attribute__((visibility("hidden"))) char *g_wsi_base = (char *)0;
__attribute__((visibility("hidden"))) char *g_wsi_ws = (char *)0;
__attribute__((visibility("hidden"))) char *g_wsi_wss = (char *)0;
__attribute__((visibility("hidden"))) char *g_wsi_end = (char *)0;
__attribute__((visibility("hidden"))) long  g_wsi_blocks = 0;
static int   g_hp_report_reg = 0;
static void gc_static_segs_init(void);
int g_gc_pending;
typedef struct { char *owner; long len; int gva_n; int off; } rt_sxt_fr_t;
__attribute__((visibility("hidden"))) rt_sxt_fr_t g_sxt_fr = { (char *)0, 0, 0, -1 };
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, owner) ==  0, "rtx_str.S bakes g_sxt_fr.owner @0");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, len)   ==  8, "rtx_str.S bakes g_sxt_fr.len @8");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, gva_n) == 16, "rtx_str.S bakes g_sxt_fr.gva_n @16");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, off)   == 20, "rtx_str.S bakes g_sxt_fr.off @20");
#define g_sxt_owner (g_sxt_fr.owner)
#define g_sxt_len   (g_sxt_fr.len)
#define g_sxt_gva_n (g_sxt_fr.gva_n)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sxt_gva_count(int n) { g_sxt_gva_n = n; }
void rt_sxt_break(const char *s) { if (s && s == g_sxt_owner) g_sxt_owner = (char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sxt_note(char *s, long len)
{
    g_sxt_owner = (char *)0;
    if (!s || !g_hp_arena || len < 0) return;
    if (s < g_hp_arena + sizeof(rt_hblk_t) || s >= g_hp_top) return;
    rt_hblk_t *h = ((rt_hblk_t *)s) - 1;
    if ((h->flags & HBF_TTL) && h->type == (uint16_t)DT_S && (char *)h + h->size == g_hp_top) { g_sxt_owner = s; g_sxt_len = len; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_sxt_match(const char *s)
{
    if (g_sxt_fr.off < 0) { const char *e = getenv("SCRIP_SXT_OFF"); g_sxt_fr.off = (e && *e && *e != '0') ? 1 : 0; }
    if (g_sxt_fr.off) return -1;
    return (s && s == g_sxt_owner) ? g_sxt_len : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_sxt_extend(char *s, long al, long bl)
{
    if (!s || s != g_sxt_owner || al != g_sxt_len || al < 0 || bl < 0) return (char *)0;
    rt_hblk_t *h = ((rt_hblk_t *)s) - 1;
    if (!(h->flags & HBF_TTL) || h->type != (uint16_t)DT_S || (char *)h + h->size != g_hp_top) { g_sxt_owner = (char *)0; return (char *)0; }
    DESCR_t *gv = (DESCR_t *)RT_GVA_VA;
    int refs = 0;
    { int k = 0, gn = g_sxt_gva_n;
      for (; k + 4 <= gn; k += 4) {
        if (__builtin_expect(gv[k].s == s, 0)     && gv[k].v == DT_S     && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
        if (__builtin_expect(gv[k + 1].s == s, 0) && gv[k + 1].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
        if (__builtin_expect(gv[k + 2].s == s, 0) && gv[k + 2].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
        if (__builtin_expect(gv[k + 3].s == s, 0) && gv[k + 3].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; } }
      for (; k < gn; k++) if (gv[k].s == s && gv[k].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; } }
    uint64_t want = sizeof(rt_hblk_t) + (((uint64_t)(al + bl + 1) + 15u) & ~15ull);
    if (want > h->size) {
        uint64_t d = want - h->size;
        if (g_hp_top + d > g_hp_end) { g_sxt_owner = (char *)0; return (char *)0; }
        h->size = (uint32_t)want;
        g_hp_top += d;
    }
    { uint64_t pay = want - sizeof(rt_hblk_t); uint64_t used = (uint64_t)(al + bl + 1); if (pay > used) memset(s + used, 0, (size_t)(pay - used)); }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gcheap_verify(void)
{
    long n = 0;
    char *p = g_hp_arena;
    while (p && p < g_hp_top) {
        rt_hblk_t *h = (rt_hblk_t *)p;
        if (!(h->flags & HBF_TTL) || (h->size & 15u) || h->size < 2 * sizeof(rt_hblk_t) || p + h->size > g_hp_top) {
            fprintf(stderr, "[ZHP] heap verify FAILED at +%ld: flags=%x size=%u type=%u (corrupt title)\n", (long)(p - g_hp_arena), (unsigned)h->flags, (unsigned)h->size, (unsigned)h->type);
            abort();
        }
        n++; p += h->size;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_report(void)
{
    if (!getenv("SCRIP_ZETA_TELEM")) return;
    long live = rt_gcheap_verify();
    fprintf(stderr, "[ZHP] ZC_HEAP_STRINGS=%d arena=%dMB blocks=%ld(alloc'd)=%ld(walked) bytes=%ld verify=OK\n", (int)ZC_HEAP_STRINGS, (int)ZC_HEAP_MB, g_hp_blocks, live, g_hp_arena ? (long)(g_hp_top - g_hp_arena) : 0L);
    fprintf(stderr, "[WSI] island=%dMB blocks=%ld ws_bytes=%ld wss_bytes=%ld\n", (int)ZC_WSI_MB, g_wsi_blocks, g_wsi_base ? (long)(g_wsi_ws - g_wsi_base) : 0L, g_wsi_base ? (long)(g_wsi_end - g_wsi_wss) : 0L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_init(void)
{
    long mb = (long)ZC_HEAP_MB;
    { const char *e = getenv("SCRIP_HEAP_MB"); if (e && *e) { long v = atol(e); if (v >= 1 && v <= 4096) mb = v; } }
    g_hp_arena = (char *)rt_slab_region((size_t)mb << 20);
    if (!g_hp_arena) { fprintf(stderr, "[ZHP] heap arena slab failed (%ld MB) — lower ZC_HEAP_MB\n", mb); abort(); }
    g_hp_top = g_hp_arena; g_hp_end = g_hp_arena + ((size_t)mb << 20);
    { const char *nh = getenv("SCRIP_NOHUGE"); if (!(nh && *nh && *nh != '0')) { uintptr_t a = ((uintptr_t)g_hp_arena + 0x1FFFFFu) & ~(uintptr_t)0x1FFFFFu, e = ((uintptr_t)g_hp_end) & ~(uintptr_t)0x1FFFFFu; if (e > a) madvise((void *)a, (size_t)(e - a), MADV_HUGEPAGE); } }
    g_hp_virgin = g_hp_arena;
    g_hp_gcline = g_hp_arena + (((size_t)mb << 20) >> 1);
    gc_static_segs_init();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *rt_gcheap_carve(char *at, uint64_t total, uint16_t type)
{
    rt_hblk_t *h = (rt_hblk_t *)at;
    h->fwd = 0; h->size = (uint32_t)total; h->type = type; h->flags = HBF_TTL;
    uint64_t pay = total - sizeof(rt_hblk_t);
    if (g_hp_fr.zfull < 0) { const char *e = getenv("SCRIP_ZSKIP_OFF"); g_hp_fr.zfull = (e && *e && *e != '0') ? 1 : 0; }
    { const int zfull = g_hp_fr.zfull;
    int fresh = !zfull && at >= g_hp_virgin;
    if (at >= g_hp_virgin && at + total > g_hp_virgin) g_hp_virgin = at + total;
    if (fresh) { }
    else if (!zfull && pay > 32 && (type == (uint16_t)DT_S || type == HB_WSC)) memset((char *)(h + 1) + (pay - 32), 0, 32);
    else memset((void *)(h + 1), 0, (size_t)pay);
    g_hp_blocks += 1;
    return (void *)(h + 1); }
}
static long g_ah_tn[512]; static long g_ah_tb[512]; static struct { void *ra; uint16_t type; long n; long b; } g_ah_ra[4096]; static int g_ah_on = -1; static int g_ah_reg = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_alloc_hist_report(void)
{
    fprintf(stderr, "[AH] per-type (type n bytes):\n");
    for (int t = 0; t < 512; t++) if (g_ah_tn[t]) fprintf(stderr, "[AH] T %d %ld %ld\n", t, g_ah_tn[t], g_ah_tb[t]);
    fprintf(stderr, "[AH] per-callsite (ra type n bytes):\n");
    for (int i = 0; i < 4096; i++) if (g_ah_ra[i].n) fprintf(stderr, "[AH] R %p %d %ld %ld\n", g_ah_ra[i].ra, (int)g_ah_ra[i].type, g_ah_ra[i].n, g_ah_ra[i].b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_alloc_hist_on(void)
{
    if (g_ah_on < 0) { const char *e = getenv("SCRIP_ALLOC_HIST"); g_ah_on = (e && *e && *e != '0') ? 1 : 0; if (g_ah_on && !g_ah_reg) { g_ah_reg = 1; atexit(rt_alloc_hist_report); } }
    return g_ah_on;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_alloc_hist_init(void) { (void)rt_alloc_hist_on(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_alloc_hist_ra(void *ra, uint16_t type, uint64_t bytes)
{
    unsigned h = (unsigned)(((uintptr_t)ra >> 3) ^ (uintptr_t)type) & 4095u;
    for (unsigned k = 0; k < 4096; k++) { unsigned i = (h + k) & 4095u;
        if (!g_ah_ra[i].n) { g_ah_ra[i].ra = ra; g_ah_ra[i].type = type; g_ah_ra[i].n = 1; g_ah_ra[i].b = (long)bytes; return; }
        if (g_ah_ra[i].ra == ra && g_ah_ra[i].type == type) { g_ah_ra[i].n += 1; g_ah_ra[i].b += (long)bytes; return; } }
}
static int g_alloc_detax = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *c_rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes)
{
    if (g_alloc_detax == 1 && g_ah_on <= 0) { uint64_t tf = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull); if (g_hp_top + tf <= g_hp_end) { void *rf = rt_gcheap_carve(g_hp_top, tf, type); g_hp_top += tf; return rf; } }
    if (g_ah_on > 0) { unsigned t = (unsigned)type & 511u; g_ah_tn[t] += 1; g_ah_tb[t] += (long)payload_bytes; }
    uint64_t total = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull);
    void *r;
    static long stress_n = -1, stress_c = 0;
    if (!g_hp_report_reg) { g_hp_report_reg = 1; atexit(rt_gcheap_report); }
    if (!g_hp_arena) rt_gcheap_init();
    if (stress_n < 0) { const char *e = getenv("SCRIP_GC_STRESS"); stress_n = e ? atol(e) : 0; }
    if (stress_n > 0 && ++stress_c >= stress_n) { stress_c = 0; g_gc_pending = 1; }
    { static long since = 0, budget = -1;
      if (budget < 0) { const char *e = getenv("SCRIP_GC_BUDGET_MB"); long mb = e ? atol(e) : 0; budget = mb > 0 ? (mb << 20) : 0; }
      if (!g_alloc_detax) g_alloc_detax = (stress_n == 0 && budget == 0 && g_ah_on <= 0 && g_hp_arena && g_hp_report_reg) ? 1 : -1;
      g_hp_fr.armed = (g_alloc_detax == 1 && g_ah_on <= 0) ? 1 : 0;
      if (budget) { since += (long)total; if (since >= budget && (g_hp_top - g_hp_arena) * 2 >= (g_hp_end - g_hp_arena)) { since = 0; g_gc_pending = 2; } } }
    if (g_hp_top + total > g_hp_end && g_hp_win + total > g_hp_wend) rt_gc_collect();
    if (g_hp_top + total <= g_hp_end) { r = rt_gcheap_carve(g_hp_top, total, type); g_hp_top += total; return r; }
    if (g_hp_win + total <= g_hp_wend) {
        uint64_t avail = (uint64_t)(g_hp_wend - g_hp_win);
        if (avail - total == sizeof(rt_hblk_t)) total += sizeof(rt_hblk_t);
        r = rt_gcheap_carve(g_hp_win, total, type);
        g_hp_win += total;
        if (g_hp_win < g_hp_wend) { rt_hblk_t *fl = (rt_hblk_t *)g_hp_win; fl->fwd = 0; fl->size = (uint32_t)(g_hp_wend - g_hp_win); fl->type = HB_FILL; fl->flags = HBF_TTL; }
        return r;
    }
    fprintf(stderr, "[ZHP] heap exhausted (%d MB, %ld blocks) after storage regeneration — raise ZC_HEAP_MB or build with -DZC_HEAP_STRINGS=0\n", (int)ZC_HEAP_MB, g_hp_blocks);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *c_rt_str_alloc(long n)
{
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)DT_S, 0);
    long want = (n < 0 ? 0 : n) + 1;
    _Static_assert(DT_S < HB_ZCOL, "value-world heap types carry DTYPE_t verbatim: DT_S is the ONLY DTYPE_t ever passed as a block type (every other caller passes HB_*), so the invariant is that it can never be mistaken for one -- it is NOT that DT_S holds any particular value. This assert read DT_S == 1 until s230, which pinned an incidental number instead of the property its own message names, and therefore fired on the TAG-3 class-bit renumber while the property still held.");
    return (char *)rt_gcheap_alloc((uint16_t)DT_S, (uint64_t)want);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_str_dup(const char *s)
{
    if (!s) s = "";
    long n = (long)strlen(s);
    char *b = rt_str_alloc(n);
    if (b) memcpy(b, s, (size_t)n);
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_wsi_init(void)
{
    long mb = (long)ZC_WSI_MB;
    g_wsi_base = (char *)rt_slab_region((size_t)mb << 20);
    if (!g_wsi_base) { fprintf(stderr, "[WSI] workspace island slab failed (%ld MB) — lower ZC_WSI_MB\n", mb); abort(); }
    g_wsi_ws = g_wsi_base; g_wsi_end = g_wsi_base + ((size_t)mb << 20); g_wsi_wss = g_wsi_end;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_ws_alloc(size_t n)
{
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_WS, (uint64_t)n);
    if (!g_wsi_base) rt_wsi_init();
    { uint64_t total = sizeof(rt_hblk_t) + ((((uint64_t)(n ? n : 1)) + 15u) & ~15ull);
      if ((uint64_t)(g_wsi_wss - g_wsi_ws) < total) { fprintf(stderr, "[WSI] workspace island exhausted (%d MB, %ld blocks) — raise ZC_WSI_MB\n", (int)ZC_WSI_MB, g_wsi_blocks); abort(); }
      { rt_hblk_t *h = (rt_hblk_t *)g_wsi_ws; h->fwd = 0; h->size = (uint32_t)total; h->type = HB_WS; h->flags = HBF_TTL; if (g_hp_fr.zfull < 0) { const char *ze = getenv("SCRIP_ZSKIP_OFF"); g_hp_fr.zfull = (ze && *ze && *ze != '0') ? 1 : 0; } if (g_hp_fr.zfull) memset((void *)(h + 1), 0, (size_t)(total - sizeof(rt_hblk_t)));
        g_wsi_ws += total; g_wsi_blocks += 1; return (void *)(h + 1); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_ws_realloc(void *p, size_t n)
{
    if (!p) return rt_ws_alloc(n);
    { rt_hblk_t *h = (rt_hblk_t *)p - 1; size_t old = (size_t)h->size - sizeof(rt_hblk_t);
      if (n <= old) return p;
      { void *q = rt_ws_alloc(n); memcpy(q, p, old); return q; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_ws_strdup(const char *s)
{
    if (!s) return (char *)0;
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_WSS, 0);
    if (!g_wsi_base) rt_wsi_init();
    { size_t n = strlen(s); uint64_t total = sizeof(rt_hblk_t) + (((uint64_t)(n + 1) + 15u) & ~15ull);
      if ((uint64_t)(g_wsi_wss - g_wsi_ws) < total) { fprintf(stderr, "[WSI] workspace island exhausted (%d MB, %ld blocks) — raise ZC_WSI_MB\n", (int)ZC_WSI_MB, g_wsi_blocks); abort(); }
      { char *at = g_wsi_wss - total; rt_hblk_t *h = (rt_hblk_t *)at; h->fwd = 0; h->size = (uint32_t)total; h->type = HB_WSS; h->flags = HBF_TTL; memcpy((void *)(h + 1), s, n + 1);
        g_wsi_wss = at; g_wsi_blocks += 1; return (char *)(h + 1); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_ws_alloc_c(size_t n)
{
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_WSC, 0);
    return rt_gcheap_alloc((uint16_t)HB_WSC, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_plj_alloc(size_t n)
{
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_PLJ, (uint64_t)n);
    return rt_gcheap_alloc((uint16_t)HB_PLJ, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *c_rt_agg_alloc(int kind, size_t n)
{
    uint16_t ty = (uint16_t)(HB_AGGV + (kind < 0 ? 0 : (kind > 2 ? 2 : kind)));
    if (g_alloc_detax != 1 && rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), ty, (uint64_t)n);
    return rt_gcheap_alloc(ty, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_ws_strdup_c(const char *s)
{
    if (!s) return (char *)0;
    { size_t n = strlen(s); char *q = (char *)rt_ws_alloc_c(n + 1); memcpy(q, s, n + 1); return q; }
}
#include <setjmp.h>
#include "../core/core.h"
#include "rt_coexpr.h"
long g_scrip_coexpr_live;
static const char **g_gc_rpin = (const char **)0;
static long g_gc_rpin_n = 0, g_gc_rpin_cap = 0;
static struct gc_rng_t { const char *lo, *hi; } *g_gc_rrng = (struct gc_rng_t *)0;
static long g_gc_rrng_n = 0, g_gc_rrng_cap = 0;
static struct gc_seg_t { char *lo, *hi; } *g_gc_segs = (struct gc_seg_t *)0;
static long g_gc_nseg = -1, g_gc_seg_cap = 0;
static rt_hblk_t **g_gc_idx = (rt_hblk_t **)0;
static rt_hblk_t **g_gc_idxbuf = (rt_hblk_t **)0;
static long g_gc_icap = 0;
static rt_hblk_t *g_gc_mhead = (rt_hblk_t *)0;
static rt_hblk_t **g_gc_liveo = (rt_hblk_t **)0;
static uint64_t *g_gc_livef = (uint64_t *)0;
static long g_gc_lcap = 0;
static long g_gc_nblk = 0;
static uint32_t *g_gc_pmap = (uint32_t *)0;
static void **g_gc_hs = (void **)0;
static long g_gc_hcap = 0, g_gc_hn = 0;
typedef struct gc_slot_t { rt_hblk_t *hloc; uintptr_t off; } gc_slot_t;
static gc_slot_t *g_gc_slots = (gc_slot_t *)0;
static long g_gc_nslot = 0, g_gc_scap = 0;
static int g_gc_in = 0;
static long g_gc_runs = 0, g_gc_interior = 0;
static char *g_gc_stktop = (char *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_walk_fold(void) { const char *e = getenv("SCRIP_GC_WALKFOLD"); return (e && *e == (char)48) ? 0 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_live_grow(long need) { if (need < g_gc_lcap) return; g_gc_lcap = g_gc_lcap ? g_gc_lcap : 4096; while (g_gc_lcap <= need) g_gc_lcap *= 2;
    g_gc_liveo = (rt_hblk_t **)realloc((void *)g_gc_liveo, (size_t)g_gc_lcap * sizeof(*g_gc_liveo)); g_gc_livef = (uint64_t *)realloc((void *)g_gc_livef, (size_t)g_gc_lcap * sizeof(*g_gc_livef)); if (!g_gc_liveo || !g_gc_livef) abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double gc_walk_ns(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return (double)t.tv_sec * 1e9 + (double)t.tv_nsec; }
static long gc_collect_ex(int cons_stack);
static char *g_gc_seam_sp = (char *)0;
static DESCR_t *g_gc_shield_arr = (DESCR_t *)0;
static int g_gc_shield_n = 0;
static const char **g_gc_shield_r = (const char **)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Called only through the rt_gc_point_arr asm veneer (rt_asm_helpers.S), which parks all six callee-saved registers above `floor` so the seam scan repairs them and the pops restore the repaired values. */
void rt_gc_point_arr_c(DESCR_t *arr, int n, const char **r0, char *floor)
{
    int pv = g_gc_pending;
    if (!pv && g_hp_gcline && g_hp_top > g_hp_gcline) pv = 1;
    if (!pv) return;
    g_gc_pending = 0;
    g_gc_shield_arr = arr; g_gc_shield_n = n; g_gc_shield_r = r0;
    g_gc_seam_sp = floor;
    gc_collect_ex(0);
    g_gc_seam_sp = (char *)0;
    g_gc_shield_arr = (DESCR_t *)0; g_gc_shield_n = 0; g_gc_shield_r = (const char **)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point(DESCR_t *d0, const char **r0)
{
    rt_gc_point_arr(d0, d0 ? 1 : 0, r0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_hins(void *p)
{
    if (g_gc_hn * 10 >= g_gc_hcap * 7) {
        long ncap = g_gc_hcap ? g_gc_hcap * 2 : 4096; void **nh = (void **)calloc((size_t)ncap, sizeof(void *));
        if (!nh) { fprintf(stderr, "[ZGC] visited-set alloc failed\n"); abort(); }
        for (long i = 0; i < g_gc_hcap; i++) if (g_gc_hs[i]) { uint64_t h = ((uint64_t)g_gc_hs[i] >> 4) * 0x9E3779B97F4A7C15ull; long s = (long)(h & (uint64_t)(ncap - 1)); while (nh[s]) s = (s + 1) & (ncap - 1); nh[s] = g_gc_hs[i]; }
        free(g_gc_hs); g_gc_hs = nh; g_gc_hcap = ncap;
    }
    { uint64_t h = ((uint64_t)p >> 4) * 0x9E3779B97F4A7C15ull; long s = (long)(h & (uint64_t)(g_gc_hcap - 1));
      while (g_gc_hs[s]) { if (g_gc_hs[s] == p) return 0; s = (s + 1) & (g_gc_hcap - 1); }
      g_gc_hs[s] = p; g_gc_hn++; return 1; }
}
static char *g_gc_pmap_top = (char *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_hblk_t *gc_blk_of(const char *p)
{
    if (!p || p < g_hp_arena || p >= g_hp_top || !g_gc_idx) return (rt_hblk_t *)0;
    if (g_gc_pmap && p < g_gc_pmap_top) { long i = (long)g_gc_pmap[(size_t)(p - g_hp_arena) >> 9]; if (i < g_gc_nblk) { rt_hblk_t *h = g_gc_idx[i]; while (i + 1 < g_gc_nblk && (char *)h + h->size <= p) h = g_gc_idx[++i]; if ((char *)h <= p && p < (char *)h + h->size) return h; } }
    { long lo = 0, hi = g_gc_nblk - 1; while (lo <= hi) { long m = (lo + hi) >> 1; char *b = (char *)g_gc_idx[m]; if (p < b) hi = m - 1; else if (p >= b + g_gc_idx[m]->size) lo = m + 1; else return g_gc_idx[m]; } return (rt_hblk_t *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_mark_blk(rt_hblk_t *h, uint16_t addf)
{
    uint16_t old = h->flags;
    h->flags = (uint16_t)(old | HBF_MARK | addf);
    if (!(old & HBF_MARK) && (h->type == HB_WS || h->type == HB_PLJ || HB_IS_AGG(h->type))) { h->fwd = (uint64_t)(uintptr_t)g_gc_mhead; g_gc_mhead = h; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_pin_ptr(const char *p)
{
    rt_hblk_t *h = gc_blk_of(p);
    if (h) gc_mark_blk(h, HBF_PIN);
}
static int g_gc_scan_tag = 0; static long g_gc_pin_tag[8][16]; static void *g_gc_pin_src[32][2]; static int g_gc_pin_src_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_cons_scan_t(const char *lo, const char *hi, int ws_only)
{
    const char *p = (const char *)(((uintptr_t)lo + 7u) & ~(uintptr_t)7u);
    for (; p + 8 <= hi; p += 8) { const char *q = *(const char *const *)p;
        if (!ws_only) rt_gc_pin_ptr(q);
        else { rt_hblk_t *h = gc_blk_of(q); if (h && (h->type == HB_ZBLK || h->type == HB_WSC || h->type == HB_PLJ || HB_IS_AGG(h->type))) {
            if (!(h->flags & HBF_PIN) && h->type >= 200 && h->type < 216) { g_gc_pin_tag[g_gc_scan_tag & 7][h->type - 200] += 1;
                if (g_gc_pin_src_n < 32) { g_gc_pin_src[g_gc_pin_src_n][0] = (void *)p; g_gc_pin_src[g_gc_pin_src_n][1] = (void *)(uintptr_t)h->type; g_gc_pin_src_n++; } }
            gc_mark_blk(h, HBF_PIN); } } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_cons_scan(const char *lo, const char *hi)
{
    gc_cons_scan_t(lo, hi, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Lon s262: registries hold (block, offset), never a raw interior address -- a slot inside a sliding block must be repaired in whatever copy of that block survives, so its identity cannot be its address. */
static void gc_slot_reg(void *loc)
{
    const char *v = *(const char *const *)loc;
    if (!gc_blk_of(v)) return;
    if (!gc_hins((void *)((uintptr_t)loc | 1))) return;
    if (g_gc_nslot == g_gc_scap) { g_gc_scap = g_gc_scap ? g_gc_scap * 2 : 4096; g_gc_slots = (gc_slot_t *)realloc((void *)g_gc_slots, (size_t)g_gc_scap * sizeof(*g_gc_slots)); if (!g_gc_slots) abort(); }
    { rt_hblk_t *hl = gc_blk_of((const char *)loc);
      if (hl) { g_gc_slots[g_gc_nslot].hloc = hl; g_gc_slots[g_gc_nslot].off = (uintptr_t)((char *)loc - (char *)(hl + 1)); }
      else { g_gc_slots[g_gc_nslot].hloc = (rt_hblk_t *)0; g_gc_slots[g_gc_nslot].off = (uintptr_t)loc; }
      g_gc_nslot++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_visit_raw(const char **loc)
{
    rt_hblk_t *h = gc_blk_of(*loc);
    if (!h) return;
    gc_mark_blk(h, 0);
    gc_slot_reg((void *)loc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_mark_agg(const void *p) { rt_hblk_t *h = gc_blk_of((const char *)p); if (h) gc_mark_blk(h, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_tbblk(struct _TBBLK_t *t);
static void gc_visit_vcell(VCELL_t *vc)
{
    gc_mark_agg(vc);
    if (vc->key) gc_mark_agg(vc->key);
    gc_slot_reg((void *)&vc->key);
    gc_slot_reg((void *)&vc->tbl);
    gc_slot_reg((void *)&vc->cellp);
    if (vc->cellp) gc_mark_agg((const void *)vc->cellp);
    if (vc->tbl && gc_hins((void *)vc->tbl)) gc_visit_tbblk(vc->tbl);
    rt_gc_visit_descr(&vc->key_d); rt_gc_visit_descr(&vc->sv);
    if (vc->cellp) rt_gc_visit_descr(vc->cellp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_tbblk(struct _TBBLK_t *t)
{
/*⭐⭐ NOTHING HERE IS PINNED, AND THAT IS THE POINT (Lon, 2026-08-23 s262).
  His ruling, and the correction that produced this block: *"GC handles sliding pointers, they just never need to be
  PINNED."*  Pointers are FINE.  A table points at its bucket blocks, a bucket's entries point at key strings, and
  every one of those may relocate freely -- what a data structure must never do is require that a block STAY PUT.
  ⛔ AN EARLIER CUT OF THIS FUNCTION PINNED THE BLOCKS A TABLE OWNS, and it was wrong twice over: it re-introduced
  the pin path Lon had just deleted, and it did so to protect a dependency that should not have existed.  The pin is
  gone; the cure is registration.  rt_gc_visit_raw hands the collector the LOCATION of each outbound pointer, so the
  slide pass REWRITES it in place -- which is what "downstream and easily slidable" means in practice.
  ⛔ WHAT IS STILL THE COLLECTOR'S BUG, and is hq_C's lane as of s263: both fixup registries (g_gc_cells, g_gc_raws)
  store INTERIOR addresses -- &e->key_descr lives inside an entry block -- so when the block holding a registered
  location is itself moved, the repair writes through a stale address.  Registries must hold (block, offset).  That
  defect is PRE-EXISTING and measured: at the parent commit, with chains still in place, table_variety.sno printed
  its correct check value and then died in rt_gcheap_verify with "corrupt title".  Chained tables survived
  table_access only BY ACCIDENT -- an entry that happened to sit in a C-stack slot picked up HBF_PIN from the
  conservative scan.  Nothing structural was protecting them; removing the pin path merely stopped hiding it. */
    gc_mark_agg(t);
    rt_gc_visit_descr(&t->dflt);
    for (int b = 0; b < TABLE_BUCKETS; b++) {
        TBBUCK_t *bk = t->buckets[b];
        if (!bk) continue;
        rt_gc_visit_raw((const char **)&t->buckets[b]);   /*⭐ DOWNSTREAM AND SLIDABLE (Lon s262): register the LOCATION so the slide rewrites it -- never pin the block so it cannot move */
        for (unsigned i = 0; i < bk->len; i++) {
            TBPAIR_t *e = &bk->ent[i];
            if (e->key) rt_gc_visit_raw((const char **)&e->key);
            rt_gc_visit_descr(&e->key_descr); rt_gc_visit_descr(&e->val);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_visit_descr(DESCR_t *d)
{
    if (!d) return;
    switch ((int)d->v) {
    case DT_S: case DT_SNUL: {
        rt_hblk_t *h = gc_blk_of(d->s);
        if (!h) return;
        gc_mark_blk(h, 0);
        if (d->s != (char *)(h + 1)) g_gc_interior++;
        gc_slot_reg((void *)&d->s);
        return; }
    case DT_A: {
        ARBLK_t *a = d->arr;
        gc_slot_reg((void *)&d->arr);
        if (!a || !gc_hins((void *)a) || !a->data) return;
        gc_slot_reg((void *)&a->data);
        { long n = (long)(a->hi - a->lo + 1); if (a->ndim == 2) n *= (long)(a->hi2 - a->lo2 + 1); if (n < 0) n = 0; for (long i = 0; i < n; i++) rt_gc_visit_descr(&a->data[i]); }
        return; }
    case DT_T: {
        TBBLK_t *t = d->tbl;
        gc_slot_reg((void *)&d->tbl);
        if (!t || !gc_hins((void *)t)) return;
        gc_visit_tbblk(t);
        return; }
    case DT_DATA: {
        DATINST_t *u = d->u;
        gc_slot_reg((void *)&d->u);
        if (!u || !gc_hins((void *)u) || !u->fields || !u->type) return;
        gc_slot_reg((void *)&u->fields);
        if ((u->type->nfields == 3 || u->type->nfields == 4) && u->type->fields && u->type->fields[0] && !strcmp(u->type->fields[0], "frame_elems")) {
            long n = (long)u->fields[1].i; DESCR_t *el = (u->fields[0].v == DT_DATA) ? (DESCR_t *)u->fields[0].ptr : NULL;
            if (el && n > 0) for (long i = 0; i < n; i++) rt_gc_visit_descr(&el[i]);
            rt_gc_visit_descr(&u->fields[2]);
            return; }
        for (int i = 0; i < u->type->nfields; i++) rt_gc_visit_descr(&u->fields[i]);
        return; }
    case DT_N: {
        if (d->slen == 2) { VCELL_t *vc = (VCELL_t *)d->p; gc_slot_reg((void *)&d->p); if (!vc || !gc_hins((void *)vc)) return; gc_visit_vcell(vc); return; }
        if (d->slen == 1) { DESCR_t *tc = (DESCR_t *)d->ptr; gc_slot_reg((void *)&d->ptr); if (tc) gc_mark_agg((const void *)tc); if (tc && gc_hins((void *)tc)) rt_gc_visit_descr(tc); return; }
        { rt_hblk_t *h = gc_blk_of(d->s); if (h) { gc_mark_blk(h, 0); gc_slot_reg((void *)&d->s); } }
        return; }
    default: return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long g_gc_rpin_ss = 0, g_gc_rrng_ss = 0;
void rt_gc_root_pin_add(const char *p)
{
    if (g_gc_rpin_n == g_gc_rpin_cap) { g_gc_rpin_cap = g_gc_rpin_cap ? g_gc_rpin_cap * 2 : 64;
        g_gc_rpin = (const char **)realloc((void *)g_gc_rpin, (size_t)g_gc_rpin_cap * sizeof(*g_gc_rpin)); if (!g_gc_rpin) abort(); }
    g_gc_rpin[g_gc_rpin_n++] = p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_pin_del(const char *p)
{
    for (long i = 0; i < g_gc_rpin_n; i++) if (g_gc_rpin[i] == p) { g_gc_rpin[i] = g_gc_rpin[--g_gc_rpin_n]; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_pin_add_seamsafe(const char *p)
{
    rt_gc_root_pin_add(p); g_gc_rpin_ss++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add(const char *lo, const char *hi)
{
    if (g_gc_rrng_n == g_gc_rrng_cap) { g_gc_rrng_cap = g_gc_rrng_cap ? g_gc_rrng_cap * 2 : 64;
        g_gc_rrng = (struct gc_rng_t *)realloc((void *)g_gc_rrng, (size_t)g_gc_rrng_cap * sizeof(*g_gc_rrng)); if (!g_gc_rrng) abort(); }
    g_gc_rrng[g_gc_rrng_n].lo = lo; g_gc_rrng[g_gc_rrng_n].hi = hi; g_gc_rrng_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add_seamsafe(const char *lo, const char *hi)
{
    rt_gc_root_range_add(lo, hi); g_gc_rrng_ss++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_del(const char *lo)
{
    for (long i = 0; i < g_gc_rrng_n; i++) if (g_gc_rrng[i].lo == lo) { g_gc_rrng[i] = g_gc_rrng[--g_gc_rrng_n]; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_phdr_cb(struct dl_phdr_info *info, size_t sz, void *data)
{
    const char *nm = info->dlpi_name;
    (void)sz; (void)data;
    if (nm && nm[0] && !strstr(nm, "libscrip")) return 0;
    for (int i = 0; i < (int)info->dlpi_phnum; i++) { const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD || !(ph->p_flags & PF_W)) continue;
        if (g_gc_nseg == g_gc_seg_cap) { g_gc_seg_cap = g_gc_seg_cap ? g_gc_seg_cap * 2 : 16;
            g_gc_segs = (struct gc_seg_t *)realloc((void *)g_gc_segs, (size_t)g_gc_seg_cap * sizeof(*g_gc_segs)); if (!g_gc_segs) abort(); }
        g_gc_segs[g_gc_nseg].lo = (char *)(info->dlpi_addr + ph->p_vaddr); g_gc_segs[g_gc_nseg].hi = g_gc_segs[g_gc_nseg].lo + ph->p_memsz; g_gc_nseg++; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_static_segs_init(void)
{
    if (g_gc_nseg >= 0) return;
    g_gc_nseg = 0;
    dl_iterate_phdr(gc_phdr_cb, (void *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gcheap_warmup(void)
{
    gc_static_segs_init();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_stack_region(char **lo, char **hi)
{
    FILE *f = fopen("/proc/self/maps", "r"); char ln[256]; unsigned long a = 0, b = 0;
    if (f) { while (fgets(ln, sizeof ln, f)) if (strstr(ln, "[stack]")) { sscanf(ln, "%lx-%lx", &a, &b); break; } fclose(f); }
    *lo = (char *)a; *hi = b ? (char *)b : (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *gc_stack_top(void)
{
    if (g_gc_stktop) return g_gc_stktop;
    { char *lo, *hi; gc_stack_region(&lo, &hi); g_gc_stktop = hi ? hi : (char *)&lo; }
    return g_gc_stktop;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_coexpr_roots(char **cur_hi)
{
    pthread_t self = pthread_self(), mainthr; scrip_coctx_t *c;
    *cur_hi = (char *)0;
    if (!scrip_co_main_known(&mainthr)) return;
    if (!pthread_equal(self, mainthr)) { char *slo, *shi; gc_stack_region(&slo, &shi); if (slo && shi && slo < shi) gc_cons_scan((const char *)slo, (const char *)shi);
        { scrip_coctx_t *r = scrip_co_gc_root(); gc_cons_scan((const char *)r->gc_spill, (const char *)r->gc_spill + sizeof r->gc_spill);
          gc_cons_scan((const char *)r->xmit, (const char *)r->xmit + sizeof r->xmit); } }
    for (c = scrip_co_gc_head(); c; c = c->gc_next) {
        gc_cons_scan((const char *)c->gc_spill, (const char *)c->gc_spill + sizeof c->gc_spill);
        gc_cons_scan((const char *)c->xmit, (const char *)c->xmit + sizeof c->xmit);
        if (c->entry_arg) gc_cons_scan((const char *)c->entry_arg + 8, (const char *)c->entry_arg + 56);
        { char *lo, *hi; if (scrip_co_stack_of(c, &lo, &hi) && lo < hi) { if (c->alive && pthread_equal(self, c->thread)) *cur_hi = hi; else gc_cons_scan((const char *)lo, (const char *)hi); } }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_in_wsi(const char *q)
{
    return q && g_wsi_base && q >= (const char *)g_wsi_base && q < (const char *)g_wsi_ws && !((uintptr_t)q & 7u);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_zeta_frame(char *lo, char *hi)
{
    char *p = lo;
    while (p + 8 <= hi) {
        if (p + 16 <= hi) { DESCR_t *d = (DESCR_t *)p; rt_hblk_t *h = (d->v == DT_S || (d->v == DT_N && d->slen == 0)) ? gc_blk_of(d->s) : (rt_hblk_t *)0;
            if (h && d->s == (char *)(h + 1) && (d->slen == 0xFFFFFFFFu || (uint64_t)d->slen < (uint64_t)h->size)) { rt_gc_visit_descr(d); p += 16; continue; }
            if (d->v == DT_T) { rt_hblk_t *th = gc_blk_of((const char *)d->tbl); if (th && th->type == HB_AGGT && (char *)d->tbl == (char *)(th + 1)) { rt_gc_visit_descr(d); p += 16; continue; } }
            if (d->v == DT_N && d->slen == 2) { rt_hblk_t *vh = gc_blk_of((const char *)d->p); if (vh && vh->type == HB_AGGV && (char *)d->p == (char *)(vh + 1)) { rt_gc_visit_descr(d); p += 16; continue; } }
            if (d->v == DT_A && gc_in_wsi((const char *)d->arr)) { rt_gc_visit_descr(d); p += 16; continue; }
            if (d->v == DT_DATA && gc_in_wsi((const char *)d->u)) { rt_gc_visit_descr(d); p += 16; continue; } }
        { const char **loc = (const char **)p; if (gc_blk_of(*loc)) rt_gc_visit_raw(loc); }
        p += 8;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_root_zeta(void)
{
    extern void *rt_zls_frames_head(void); extern void *rt_zls_frame_prev(void *fb); extern long rt_zls_frame_size(void *fb);
    void *fb = rt_zls_frames_head();
    while (fb) { long sz = rt_zls_frame_size(fb); gc_zeta_frame((char *)fb, (char *)fb + sz); fb = rt_zls_frame_prev(fb); }
}
static long g_gc_cas_bytes = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_root_cas(void)
{
    extern int rt_cas_live_span(int, void **, size_t *);
    static int unroot = -1;
    if (unroot < 0) { const char *e = getenv("SCRIP_GC_UNROOT"); unroot = (e && strstr(e, "cas")) ? 1 : 0; }
    g_gc_cas_bytes = 0;
    if (unroot) return;
    { void *b = 0; size_t n = 0; for (int i = 0; rt_cas_live_span(i, &b, &n); i++) if (b && n) { gc_zeta_frame((char *)b, (char *)b + n); g_gc_cas_bytes += (long)n; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_collect_ex(int cons_stack)
{
    extern void core_gc_roots(void); extern void gen_gc_roots(void); extern void rt_gc_root_args(void); extern void rt_gc_ws_roots(void); extern int rt_scan_active(void); extern int rt_value_trail_mark(void);
    jmp_buf jb; char anchor; long nlive = 0, npin = 0, nfill = 0, before_b, after_b; char *dest; rt_hblk_t **liveo; uint64_t *livef; long li = 0; int pz = 0; long nforeign = 0;
    long w_cnt = 0, w_idx = 0, w_pmg = 0, w_fwd = 0, w_liv = 0, w_sld = 0, w_vfy = 0, w_cel = 0, w_raw = 0, w_mov = 0; int w_tel = getenv("SCRIP_ZETA_TELEM") ? 1 : 0;
    double n_cnt = 0, n_idx = 0, n_mrk = 0, n_fwd = 0, n_liv = 0, n_sld = 0, n_vfy = 0, n_fix = 0, n_t0 = 0, n_all = w_tel ? gc_walk_ns() : 0;
    g_sxt_owner = (char *)0;
    if (g_gc_in || !g_hp_arena) return 0;
    g_gc_in = 1; before_b = (long)(g_hp_top - g_hp_arena);
    g_hp_win = (char *)0; g_hp_wend = (char *)0;
    n_t0 = w_tel ? gc_walk_ns() : 0;
    g_gc_nblk = 0;
    if (!gc_walk_fold()) { char *p = g_hp_arena; while (p < g_hp_top) { g_gc_nblk++; p += ((rt_hblk_t *)p)->size; } }
    if (w_tel) { w_cnt = g_gc_nblk; n_cnt = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    if (g_gc_nblk > g_gc_icap) { g_gc_icap = g_gc_icap ? g_gc_icap : 4096; while (g_gc_icap < g_gc_nblk) g_gc_icap *= 2;
        g_gc_idxbuf = (rt_hblk_t **)realloc((void *)g_gc_idxbuf, (size_t)g_gc_icap * sizeof(*g_gc_idxbuf)); if (!g_gc_idxbuf) abort(); }
    g_gc_idx = g_gc_idxbuf;
    { char *p = g_hp_arena; long i = 0; int fold = gc_walk_fold(); if (!g_gc_pmap) { g_gc_pmap = (uint32_t *)malloc((((size_t)(g_hp_end - g_hp_arena)) >> 9) * sizeof(uint32_t)); if (!g_gc_pmap) abort(); } while (p < g_hp_top) { rt_hblk_t *h = (rt_hblk_t *)p;
        if (fold && i >= g_gc_icap) { g_gc_icap = g_gc_icap ? g_gc_icap * 2 : 4096; g_gc_idxbuf = (rt_hblk_t **)realloc((void *)g_gc_idxbuf, (size_t)g_gc_icap * sizeof(*g_gc_idxbuf)); if (!g_gc_idxbuf) abort(); g_gc_idx = g_gc_idxbuf; }
        h->flags &= (uint16_t)~(HBF_MARK | HBF_PIN);
        if (h->type == HB_ZBLK || h->type == HB_PLJ) nforeign++;
        h->fwd = 0; g_gc_idx[i] = h; { char *e = p + h->size; char *gs0 = g_hp_arena + (((size_t)(p - g_hp_arena) + 511u) & ~(size_t)511u); if (w_tel && e > gs0) w_pmg += (long)((e - gs0 + 511) >> 9);
            for (char *gs = gs0; gs < e; gs += 512) g_gc_pmap[(size_t)(gs - g_hp_arena) >> 9] = (uint32_t)i; } i++; p += h->size; } if (fold) g_gc_nblk = i; g_gc_pmap_top = g_hp_top; }
    if (w_tel) { w_idx = g_gc_nblk; n_idx = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    g_gc_mhead = (rt_hblk_t *)0;
    { static int legacy_env = -1; if (legacy_env < 0) { const char *e = getenv("SCRIP_GC_LEGACY"); legacy_env = (e && *e && *e != '0') ? 1 : 0; }
      pz = (cons_stack == 0 && !legacy_env && nforeign == 0 && !rt_scan_active() && !g_scrip_coexpr_live && rt_value_trail_mark() == 0 && g_gc_rpin_n == g_gc_rpin_ss && g_gc_rrng_n == g_gc_rrng_ss);
      if (cons_stack == 0 && !pz) cons_stack = 1; }
    g_gc_hn = 0; if (g_gc_hs) memset(g_gc_hs, 0, (size_t)g_gc_hcap * sizeof(void *));
    g_gc_nslot = 0; g_gc_interior = 0;
    if (!pz) { for (long i = 0; i < g_gc_rpin_n; i++) rt_gc_pin_ptr(g_gc_rpin[i]);
      for (long i = 0; i < g_gc_rrng_n; i++) if (g_gc_rrng[i].lo < g_gc_rrng[i].hi) gc_cons_scan(g_gc_rrng[i].lo, g_gc_rrng[i].hi);
      if (g_wsi_base && g_wsi_ws > g_wsi_base) gc_cons_scan((const char *)g_wsi_base, (const char *)g_wsi_ws); }
    rt_gc_ws_roots();
    { static int blanket = -1; if (blanket < 0) { const char *e = getenv("SCRIP_GC_STATICS_BLANKET"); blanket = (e && *e && *e != '0') ? 1 : 0; }
      if (blanket && !pz) { gc_static_segs_init();
        g_gc_scan_tag = 1;
        for (long i = 0; i < g_gc_nseg; i++) if (g_gc_segs[i].lo < g_gc_segs[i].hi) gc_cons_scan_t((const char *)g_gc_segs[i].lo, (const char *)g_gc_segs[i].hi, 1); } }
    if (!pz) { char *chi; gc_coexpr_roots(&chi);
      if (cons_stack) { int wso = (cons_stack == 2); g_gc_scan_tag = 2; setjmp(jb); gc_cons_scan_t((const char *)&jb, (const char *)&jb + sizeof jb, wso);
        { char *lo = &anchor, *hi = chi ? chi : gc_stack_top(); g_gc_scan_tag = 3; if (lo < hi) gc_cons_scan_t((const char *)lo, (const char *)hi, wso); g_gc_scan_tag = 0; } } }
    gc_root_zeta();
    gc_root_cas();
    { static int cov = -1; if (cov < 0) { const char *e = getenv("SCRIP_GC_COVERAGE"); cov = (e && *e && *e != '0') ? 1 : 0; }
      if (cov) fprintf(stderr, "[GC-COV] pins=%ld ranges=%ld cas_scanned_bytes=%ld pz=%d cons_stack=%d\n", g_gc_rpin_n, g_gc_rrng_n, g_gc_cas_bytes, pz, cons_stack); }
    core_gc_roots(); gen_gc_roots(); rt_gc_root_args();
    if (pz) { extern uint64_t rtccb[32]; for (int ci = 0; ci < 32; ci++) rt_gc_visit_raw((const char **)&rtccb[ci]); }
    if (pz && g_gc_seam_sp) { char *sst = gc_stack_top(); if (g_gc_seam_sp < sst) gc_zeta_frame(g_gc_seam_sp, sst); }
    for (int si = 0; si < g_gc_shield_n; si++) rt_gc_visit_descr(&g_gc_shield_arr[si]);
    if (g_gc_shield_r) rt_gc_visit_raw(g_gc_shield_r);
    { int wl; long walked = 0, nscan = 0, rounds = 0; { const char *e = getenv("SCRIP_GC_WORKLIST"); wl = (e && *e == (char)48) ? 0 : 1; }
      if (wl) { while (g_gc_mhead) { rt_hblk_t *h = g_gc_mhead; g_gc_mhead = (rt_hblk_t *)(uintptr_t)h->fwd; h->fwd = 0; walked++; nscan++;
            if (h->type == HB_WS || h->type == HB_PLJ) { gc_cons_scan((const char *)(h + 1), (const char *)h + h->size); continue; }
            if (h->type == HB_AGGV) { gc_visit_vcell((VCELL_t *)(h + 1)); continue; }
            if (h->type == HB_AGGB) continue;   /*⭐ s262 bucket index: {hkey, entry*} records, no descriptors -- its entries are marked by gc_visit_tbblk */
            if (h->type == HB_AGGP) { TBPAIR_t *e = (TBPAIR_t *)(h + 1); if (e->key) gc_mark_agg(e->key);
                rt_gc_visit_descr(&e->key_descr); rt_gc_visit_descr(&e->val); continue; }
            if (h->type == HB_AGGT) { struct _TBBLK_t *t = (struct _TBBLK_t *)(h + 1); if (gc_hins((void *)t)) gc_visit_tbblk(t); continue; } } }
      else { char *scanned; int changed = 1;
        while (g_gc_mhead) { rt_hblk_t *h = g_gc_mhead; g_gc_mhead = (rt_hblk_t *)(uintptr_t)h->fwd; h->fwd = 0; }
        scanned = (char *)calloc((size_t)(g_gc_nblk ? g_gc_nblk : 1), 1); if (!scanned) abort();
        while (changed) { changed = 0; rounds++;
          for (long i = 0; i < g_gc_nblk; i++) { rt_hblk_t *h = g_gc_idx[i]; walked++;
              if (scanned[i] || !(h->flags & (HBF_MARK | HBF_PIN))) continue;
              if (h->type == HB_WS || h->type == HB_PLJ) { scanned[i] = 1; changed = 1; nscan++; gc_cons_scan((const char *)(h + 1), (const char *)h + h->size); continue; }
              if (h->type == HB_AGGV) { scanned[i] = 1; changed = 1; nscan++; gc_visit_vcell((VCELL_t *)(h + 1)); continue; }
              if (h->type == HB_AGGB) { scanned[i] = 1; changed = 1; nscan++; continue; }   /*⭐ s262 bucket index -- see the worklist arm */
              if (h->type == HB_AGGP) { TBPAIR_t *e = (TBPAIR_t *)(h + 1); scanned[i] = 1; changed = 1; nscan++; if (e->key) gc_mark_agg(e->key);
                  rt_gc_visit_descr(&e->key_descr); rt_gc_visit_descr(&e->val); continue; }
              if (h->type == HB_AGGT) { struct _TBBLK_t *t = (struct _TBBLK_t *)(h + 1); scanned[i] = 1; changed = 1; nscan++; if (gc_hins((void *)t)) gc_visit_tbblk(t); continue; } } }
        free((void *)scanned); }
      if (w_tel) { n_mrk = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); fprintf(stderr, "[ZGC-MARK] arm=%s titles-walked=%ld blocks-scanned=%ld rounds=%ld nblk=%ld\n", wl ? "WL" : "FX", walked, nscan, rounds, g_gc_nblk); n_t0 = gc_walk_ns(); }
    }
    dest = g_hp_arena;
    { long pws = 0, pwsc = 0, pzb = 0, pval = 0, dwsc = 0, pagg = 0, dagg = 0; int fold = gc_walk_fold();
    if (fold) { gc_live_grow(0); liveo = g_gc_liveo; livef = g_gc_livef; }
    for (long i = 0; i < g_gc_nblk; i++) { rt_hblk_t *h = g_gc_idx[i];
        /* ⛔ TYPE-BASED PIN REMOVED (Lon s262: "Completely remove the PIN path.  Let's see what breaks.").  It was gated on !pz, i.e. it only ever fired when the collector could NOT be precise -- a fallback for imprecision, not a design.  Marked blocks now relocate like any other live block. */
        if (h->type == HB_WSC && !(h->flags & HBF_PIN)) dwsc++;
        if (HB_IS_AGG(h->type) && !(h->flags & HBF_PIN)) dagg++;
        /* ⛔ THE PIN ARM IS GONE (Lon s262).  It mapped a pinned block to itself (h->fwd = h) so the compactor left it in place; every live block now relocates. */
        if (h->flags & HBF_MARK) { h->fwd = (uint64_t)dest; dest += h->size; nlive++; }
        else h->fwd = 0;
        if (fold && h->fwd) { if (li >= g_gc_lcap) { gc_live_grow(li); liveo = g_gc_liveo; livef = g_gc_livef; } liveo[li] = h; livef[li] = h->fwd; li++; }
        if (h->flags & HBF_PIN) dest = (char *)h + h->size; }
    if (w_tel) { w_fwd = g_gc_nblk; n_fwd = gc_walk_ns() - n_t0; fprintf(stderr, "[ZGC]   pin-classes ws=%ld wsc=%ld agg=%ld zblk=%ld val=%ld  wsc-dead=%ld agg-dead=%ld\n", pws, pwsc, pagg, pzb, pval, dwsc, dagg); n_t0 = gc_walk_ns(); } }
    for (long i = 0; i < g_gc_nslot; i++) { gc_slot_t *sl = &g_gc_slots[i]; const char **loc = sl->hloc ? (const char **)((char *)(sl->hloc + 1) + sl->off) : (const char **)sl->off;
        rt_hblk_t *h = gc_blk_of(*loc); if (h && h->fwd && h->fwd != (uint64_t)h) *loc = (const char *)((rt_hblk_t *)h->fwd + 1) + (*loc - (const char *)(h + 1)); }
    if (w_tel) { w_cel = g_gc_nslot; w_raw = 0; n_fix = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    if (!gc_walk_fold()) { gc_live_grow(nlive); liveo = g_gc_liveo; livef = g_gc_livef;
        for (long i = 0; i < g_gc_nblk; i++) if (g_gc_idx[i]->fwd) { liveo[li] = g_gc_idx[i]; livef[li] = g_gc_idx[i]->fwd; li++; }
        if (w_tel) { w_liv = g_gc_nblk; n_liv = gc_walk_ns() - n_t0; } }
    if (w_tel) n_t0 = gc_walk_ns();
    dest = g_hp_arena;
    for (long i = 0; i < li; i++) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size;
        if ((char *)livef[i] == (char *)h) { if (dest < (char *)h) { rt_hblk_t *fl = (rt_hblk_t *)dest; fl->fwd = 0; fl->size = (uint32_t)((char *)h - dest); fl->type = HB_FILL; fl->flags = HBF_TTL; nfill++;
            if ((long)fl->size > (long)(g_hp_wend - g_hp_win)) { g_hp_win = dest; g_hp_wend = (char *)h; } } dest = (char *)h + sz; }
        else { memmove((void *)livef[i], (void *)h, (size_t)sz); dest = (char *)livef[i] + sz; if (w_tel) w_mov += (long)sz; } }
    g_hp_top = dest; g_hp_blocks = nlive + nfill;
    for (long i = 0; i < li; i++) { rt_hblk_t *nh = (rt_hblk_t *)livef[i]; nh->fwd = 0; nh->flags = (uint16_t)((nh->flags | HBF_TTL) & ~(HBF_MARK | HBF_PIN)); }
    after_b = (long)(g_hp_top - g_hp_arena);
    if (w_tel) { w_sld = li; n_sld = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    rt_gcheap_verify();
    if (w_tel) { w_vfy = nlive + nfill; n_vfy = gc_walk_ns() - n_t0; }
    g_gc_runs++;
    if (getenv("SCRIP_ZETA_TELEM")) {
      for (int tg = 0; tg < 8; tg++) for (int ty = 0; ty < 16; ty++) if (g_gc_pin_tag[tg][ty]) fprintf(stderr, "[ZGC-PIN] tag=%d type=%d n=%ld\n", tg, 200 + ty, g_gc_pin_tag[tg][ty]);
      for (int si = 0; si < g_gc_pin_src_n; si++) fprintf(stderr, "[ZGC-SRC] word_at=%p type=%ld\n", g_gc_pin_src[si][0], (long)(uintptr_t)g_gc_pin_src[si][1]);
      memset(g_gc_pin_tag, 0, sizeof g_gc_pin_tag); g_gc_pin_src_n = 0; }
    if (w_tel) fprintf(stderr, "[ZGC-WALK] arm=%s nblk=%ld | count=%ld/%.0fus index=%ld/%.0fus pmap-gran=%ld fwd=%ld/%.0fus live=%ld/%.0fus | mark=%.0fus fixup=%ld+%ld/%.0fus slide=%ld/%.0fus moved=%ldB verify=%ld/%.0fus | walk-floor=%ld titles %.0fus of %.0fus total\n",
        gc_walk_fold() ? "FOLD" : "LEGACY", g_gc_nblk, w_cnt, n_cnt / 1e3, w_idx, n_idx / 1e3, w_pmg, w_fwd, n_fwd / 1e3, w_liv, n_liv / 1e3, n_mrk / 1e3, w_cel, w_raw, n_fix / 1e3, w_sld, n_sld / 1e3, w_mov, w_vfy, n_vfy / 1e3,
        w_cnt + w_idx + w_fwd + w_liv + w_vfy, (n_cnt + n_idx + n_fwd + n_liv + n_vfy) / 1e3, (gc_walk_ns() - n_all) / 1e3);
    if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZGC] regeneration #%ld (%s): blocks %ld->%ld (pinned %ld, fill %ld) bytes %ld->%ld reclaimed %ld win=%ld slots=%ld interior=%ld\n", g_gc_runs, pz ? "PZ" : "LG", g_gc_nblk, nlive, npin, nfill, before_b, after_b, before_b - after_b, (long)(g_hp_wend - g_hp_win), g_gc_nslot, g_gc_interior);
    g_hp_gcline = g_hp_top + ((g_hp_end - g_hp_top) >> 1);
    g_gc_idx = (rt_hblk_t **)0;
    g_gc_in = 0;
    return before_b - after_b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_collect(void)
{
    return gc_collect_ex(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gcheap_free(void) { return (long)(g_hp_end - g_hp_top); }
long rt_gc_runs_count(void) { return g_gc_runs; }
