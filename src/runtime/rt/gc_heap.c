#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <link.h>
#include "zeta_choices.h"
#include "rt_slab.h"
#include "rt_arena.h"
#include "gc_heap.h"
#include "descr.h"
/* GC-0 (ARCH-ZETA-LOCAL-STORAGE §6e) — scrip-owned bump heap, SIL title-word headers, libgc COEXISTENCE.
 * Coexistence contract (why this is sound with zero collector): scrip-heap blocks are not libgc objects, so
 * libgc never frees them; a libgc conservative scan that sees a pointer INTO this arena ignores it (non-heap);
 * and the ONLY family migrated here (DT_S string payloads) is ATOMIC — no pointers inside — so a scrip-heap
 * block can never be the sole holder of a libgc object's liveness. Families whose payloads can hold libgc
 * pointers (ARBLK/TBBLK/DATINST/VCELL DESCR cells) must NOT migrate before their GC-5 row lands with either
 * full residency or GC_add_roots registration. Nothing here frees, so dangling is impossible; garbage simply
 * accumulates until GC-1..3 land — the SIL way: bump until exhaustion, then storage regeneration (§6a). */
_Static_assert(sizeof(rt_hblk_t) == 16, "rt_hblk_t must be one 16-byte title unit");
static char *g_hp_arena = (char *)0;
static char *g_hp_top = (char *)0;
static char *g_hp_end = (char *)0;
static char *g_hp_win = (char *)0;
static char *g_hp_wend = (char *)0;
static long  g_hp_blocks = 0;
static int   g_hp_report_reg = 0;
static void gc_static_segs_init(void);
int g_gc_pending;
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
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_init(void)
{
    long mb = (long)ZC_HEAP_MB;
    /* TR-2: slab-pool backing (was private mmap). Contiguity is load-bearing here —
     * the linear TITLE WALK (rt_gcheap_verify / mark-sweep) strides block-to-block
     * across the whole region by size header, which only works on one contiguous span. */
    g_hp_arena = (char *)rt_slab_region((size_t)mb << 20);
    if (!g_hp_arena) { fprintf(stderr, "[ZHP] heap arena slab failed (%ld MB) — lower ZC_HEAP_MB\n", mb); abort(); }
    g_hp_top = g_hp_arena; g_hp_end = g_hp_arena + ((size_t)mb << 20);
    gc_static_segs_init();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *rt_gcheap_carve(char *at, uint64_t total, uint16_t type)
{
    /* Carve a block from the LOW end of a bump region [at, region_end); caller guarantees fit and, for the fill window, rewrites the remainder's HB_FILL title so the linear title walk stays valid. */
    rt_hblk_t *h = (rt_hblk_t *)at;
    h->fwd = 0; h->size = (uint32_t)total; h->type = type; h->flags = HBF_TTL;
    memset((void *)(h + 1), 0, (size_t)(total - sizeof(rt_hblk_t)));
    g_hp_blocks += 1;
    return (void *)(h + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes)
{
    /* Allocation order: (1) main bump at g_hp_top; (2) the FILL WINDOW — a secondary bump region installed by the collector inside the largest HB_FILL gap, needed when a conservative pin holds the
     * heap TOP at exhaustion time (the pinned block is near-always the allocating expression's own in-flight operand, so the top cannot retreat and all reclaimed space lands BELOW it — discovered by
     * the 213/214 exhaustion tortures, 2026-07-05); (3) regenerate, recompute both, retry; (4) honest bomb. Window carves rewrite the remainder fill title in step, keeping rt_gcheap_verify green.
     * s90 NOTE: a window-FIRST variant was built and WITHDRAWN — it exposed (not caused) the malloc→WS reference hole in the s90 finding: interned names live in HB_WS blocks reachable only from
     * raw-malloc'd IR, so HB_WS stays blanket-pinned until the WS-class split; with WS pinned, window-first had no remaining motivation and the SIL top-bump order stands. */
    uint64_t total = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull);
    void *r;
    static long stress_n = -1, stress_c = 0;
    if (!g_hp_report_reg) { g_hp_report_reg = 1; atexit(rt_gcheap_report); }
    if (!g_hp_arena) rt_gcheap_init();
    if (stress_n < 0) { const char *e = getenv("SCRIP_GC_STRESS"); stress_n = e ? atol(e) : 0; }
    if (stress_n > 0 && ++stress_c >= stress_n) { stress_c = 0; g_gc_pending = 1; }
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
char *rt_str_alloc(long n)
{
    /* THE DT_S entry point (GC-5 strings row, landed with GC-0 as the Lon-directed proof family):
     * n characters + NUL. Zero-initialized on BOTH paths — manual pin 3's "all words within a block must be
     * properly filled in", discharged mechanically the ZC_INIT_ZERO way. Fallback = libgc atomic, intact. */
    long want = (n < 0 ? 0 : n) + 1;
    _Static_assert(DT_S == 1, "value-world heap types carry DTYPE_t verbatim");
    return (char *)rt_gcheap_alloc((uint16_t)DT_S, (uint64_t)want);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_str_dup(const char *s)
{
    /* GC-5 strings-row TAIL: the mechanical GC_strdup replacement flagged in ARCH-ZETA-LOCAL-STORAGE.md
     * §6e. Reuses rt_str_alloc so both heap paths (scrip-owned / libgc-atomic fallback) stay in sync automatically. */
    if (!s) s = "";
    long n = (long)strlen(s);
    char *b = rt_str_alloc(n);
    if (b) memcpy(b, s, (size_t)n);
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GC-U-6 slice 1 (s84) → GC-U-7 ROOTS (s90): THE WORKSPACE lives IN the collected span — rt_ws_* on rt_gcheap_alloc(HB_WS): ONE reserved-VA span, ONE cursor (g_hp_top), uniform rt_hblk_t titles,
 * gc_collect_ex the one collector over everything. HB_WS remains BLANKET-PINNED at reset — s90 MEASURED why it must (window-first probe): interned names and registry payloads live in HB_WS blocks
 * whose ONLY references sit in raw-malloc'd IR/driver structures no root layer can see; un-pinning HB_WS awaits the WS-class split (DESCR-only by_name results → collectable class; malloc-referenced
 * registries/names → immortal class; the TR-3 ROOT+EDGE rows already name the sides, TR-5's census the mechanism). HB_ZBLK is UN-PINNED and registration-governed: both clients cover themselves by
 * construction (rt_zh_bump_slow registers each refill window pin+range; coexpr stacks register at carve, unregister at destroy, retitle HB_FILL dead) — dead coexpr stacks now reclaim. Marked
 * HB_WS/HB_ZBLK blocks are PROMOTED to HBF_PIN at forward (fwd=self, never slid, never adjusted-through); their payloads get the transitive conservative scan (closes the s88 latent gap: DESCRs
 * inside WS blocks reference DT_S payloads nothing else roots). The grow-only realloc's old-size decode reads the rt_hblk_t (size - 16). */
void *rt_ws_alloc(size_t n)
{
    return rt_gcheap_alloc((uint16_t)HB_WS, (uint64_t)(n ? n : 1));
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
    { size_t n = strlen(s); char *q = (char *)rt_ws_alloc(n + 1); memcpy(q, s, n + 1); return q; }
}
/*====================================================================================================================================================================================================*/
/* GC-1 MARK + GC-2 ADJUST + GC-3 SLIDE (ARCH-ZETA-LOCAL-STORAGE §6a/§6b/§6e) — the SIL 3-stage storage regeneration, v1 scope = the DT_S strings family (the only resident family after GC-0).
 * TWO ROOT LAYERS. PRECISE (tag-driven, §6b: DESCR_t.v discriminates — marked AND adjusted): NV buckets incl. bound GVA cells (core_gc_roots), _var_reg cells, g_call_args window (rt_gc_root_args),
 * drive_val + frame_stack env/return_val + gen svals + scan_stack sigma/subj (gen_gc_roots — restored FROM MEMORY at scan-leave, hence adjustable), and full aggregate tracing through libgc-owned
 * ARBLK/TBBLK/DATINST/VCELL/NAMEPTR cells (they do not move; the DESCR cells INSIDE them adjust). CONSERVATIVE (marked and PINNED via HBF_PIN, fwd=self, NEVER moved, never adjusted-through — the
 * manual pin-3 XNBLK precedent): the live ζ chain (mode-4 has no zls maps at runtime; conservative-pin is the sound v1 until ZB-4 emits layouts), the C stack + setjmp register spill (covers rt-helper
 * locals holding raw payload ptrs across the triggering alloc — what makes allocation-site collection sound, §6d), and the CURRENT scan subject (Σ lives in r13, unrewritable — D10 pin-cell realized
 * as pin-the-block; OUTER subjects restore through scan_stack and adjust precisely). SLIDE runs a dest cursor; pins are barriers; sub-pin gaps get HB_FILL titles so the linear walk stays verifiable
 * and compact away next cycle. Dedup/cycle hash makes every cell adjust EXACTLY once (double-adjust is corruption — new addresses land inside the arena). GC-U-7 (s90) retired the §6b-finding-ii
 * coexpr DECLINE: every scrip_coctx_t is enumerated (registry in rt_coexpr.c) — gc_spill (callee-saved regs spilled by scrip_coswitch just before every suspension) + xmit + entry pkg regs are
 * scanned conservatively, each live stack window is a registered pin + conservative range, the suspended main [stack] region scans in full when a coexpr thread collects, and the current thread's
 * anchor..top scan is window-aware. Registered pins/ranges (rt_gc_root_pin_add/rt_gc_root_range_add) + the writable-PT_LOAD statics scan (dl_iterate_phdr: exe + libscrip) run at EVERY collect, and
 * gateway collects are now conservative too — raw HB_WS pointers in C locals across gateway seams must survive the un-pin (the fill window absorbs the residual top-pin ratchet risk, measured s90).
 * Triggers: exhaustion in rt_gcheap_alloc (regenerate-then-retry, the SIL way), SCRIP_GC_STRESS=N every-N-allocs, and rt_gc_collect() exported for GC-7's COLLECT(). */
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
static long g_gc_nblk = 0;
static void **g_gc_hs = (void **)0;
static long g_gc_hcap = 0, g_gc_hn = 0;
static DESCR_t **g_gc_cells = (DESCR_t **)0;
static long g_gc_ncell = 0, g_gc_ccap = 0;
static const char ***g_gc_raws = (const char ***)0;
static long g_gc_nraw = 0, g_gc_rcap = 0;
static int g_gc_in = 0;
static long g_gc_runs = 0, g_gc_interior = 0;
static char *g_gc_stktop = (char *)0;
static long gc_collect_ex(int cons_stack);
static DESCR_t *g_gc_shield_arr = (DESCR_t *)0;
static int g_gc_shield_n = 0;
static const char **g_gc_shield_r = (const char **)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0)
{
    if (!g_gc_pending) return;
    g_gc_pending = 0;
    g_gc_shield_arr = arr; g_gc_shield_n = n; g_gc_shield_r = r0;
    gc_collect_ex(2);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_hblk_t *gc_blk_of(const char *p)
{
    long lo = 0, hi = g_gc_nblk - 1;
    if (!p || p < g_hp_arena || p >= g_hp_top) return (rt_hblk_t *)0;
    while (lo <= hi) { long m = (lo + hi) >> 1; char *b = (char *)g_gc_idx[m]; if (p < b) hi = m - 1; else if (p >= b + g_gc_idx[m]->size) lo = m + 1; else return g_gc_idx[m]; }
    return (rt_hblk_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_pin_ptr(const char *p)
{
    rt_hblk_t *h = gc_blk_of(p);
    if (h) h->flags |= (HBF_MARK | HBF_PIN);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_cons_scan_t(const char *lo, const char *hi, int ws_only)
{
    const char *p = (const char *)(((uintptr_t)lo + 7u) & ~(uintptr_t)7u);
    for (; p + 8 <= hi; p += 8) { const char *q = *(const char *const *)p;
        if (!ws_only) rt_gc_pin_ptr(q);
        else { rt_hblk_t *h = gc_blk_of(q); if (h && (h->type == HB_WS || h->type == HB_ZBLK)) h->flags |= (HBF_MARK | HBF_PIN); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_cons_scan(const char *lo, const char *hi)
{
    gc_cons_scan_t(lo, hi, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_visit_raw(const char **loc)
{
    rt_hblk_t *h = gc_blk_of(*loc);
    if (!h) return;
    h->flags |= HBF_MARK;
    if (!gc_hins((void *)loc)) return;
    if (g_gc_nraw == g_gc_rcap) { g_gc_rcap = g_gc_rcap ? g_gc_rcap * 2 : 1024; g_gc_raws = (const char ***)realloc((void *)g_gc_raws, (size_t)g_gc_rcap * sizeof(*g_gc_raws)); if (!g_gc_raws) abort(); }
    g_gc_raws[g_gc_nraw++] = loc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_visit_descr(DESCR_t *d)
{
    if (!d) return;
    switch ((int)d->v) {
    case DT_S: case DT_SNUL: {
        rt_hblk_t *h = gc_blk_of(d->s);
        if (!h) return;
        h->flags |= HBF_MARK;
        if (d->s != (char *)(h + 1)) g_gc_interior++;
        if (!gc_hins((void *)d)) return;
        if (g_gc_ncell == g_gc_ccap) { g_gc_ccap = g_gc_ccap ? g_gc_ccap * 2 : 4096; g_gc_cells = (DESCR_t **)realloc((void *)g_gc_cells, (size_t)g_gc_ccap * sizeof(*g_gc_cells)); if (!g_gc_cells) abort(); }
        g_gc_cells[g_gc_ncell++] = d;
        return; }
    case DT_A: {
        ARBLK_t *a = d->arr;
        if (!a || !gc_hins((void *)a) || !a->data) return;
        { long n = (long)(a->hi - a->lo + 1); if (a->ndim == 2) n *= (long)(a->hi2 - a->lo2 + 1); if (n < 0) n = 0; for (long i = 0; i < n; i++) rt_gc_visit_descr(&a->data[i]); }
        return; }
    case DT_T: {
        TBBLK_t *t = d->tbl;
        if (!t || !gc_hins((void *)t)) return;
        rt_gc_visit_descr(&t->dflt);
        for (int b = 0; b < TABLE_BUCKETS; b++) for (TBPAIR_t *e = t->buckets[b]; e; e = e->next) { rt_gc_visit_descr(&e->key_descr); rt_gc_visit_descr(&e->val); }
        return; }
    case DT_DATA: {
        DATINST_t *u = d->u;
        if (!u || !gc_hins((void *)u) || !u->fields || !u->type) return;
        if (u->type->nfields == 3 && u->type->fields && u->type->fields[0] && !strcmp(u->type->fields[0], "frame_elems")) {
            long n = (long)u->fields[1].i; DESCR_t *el = (u->fields[0].v == DT_DATA) ? (DESCR_t *)u->fields[0].ptr : NULL;
            if (el && n > 0) for (long i = 0; i < n; i++) rt_gc_visit_descr(&el[i]);
            rt_gc_visit_descr(&u->fields[2]);
            return; }
        for (int i = 0; i < u->type->nfields; i++) rt_gc_visit_descr(&u->fields[i]);
        return; }
    case DT_N: {
        if (d->slen == 2) { VCELL_t *vc = (VCELL_t *)d->p; if (!vc || !gc_hins((void *)vc)) return; rt_gc_visit_descr(&vc->key_d); rt_gc_visit_descr(&vc->sv); if (vc->cellp) rt_gc_visit_descr(vc->cellp); return; }
        if (d->slen == 1) { DESCR_t *tc = (DESCR_t *)d->ptr; if (tc && gc_hins((void *)tc)) rt_gc_visit_descr(tc); return; }
        { rt_hblk_t *h = gc_blk_of(d->s); if (h) { h->flags |= HBF_MARK; if (gc_hins((void *)d)) { if (g_gc_ncell == g_gc_ccap) { g_gc_ccap = g_gc_ccap ? g_gc_ccap * 2 : 4096; g_gc_cells = (DESCR_t **)realloc((void *)g_gc_cells, (size_t)g_gc_ccap * sizeof(*g_gc_cells)); if (!g_gc_cells) abort(); } g_gc_cells[g_gc_ncell++] = d; } } }
        return; }
    default: return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
void rt_gc_root_range_add(const char *lo, const char *hi)
{
    if (g_gc_rrng_n == g_gc_rrng_cap) { g_gc_rrng_cap = g_gc_rrng_cap ? g_gc_rrng_cap * 2 : 64;
        g_gc_rrng = (struct gc_rng_t *)realloc((void *)g_gc_rrng, (size_t)g_gc_rrng_cap * sizeof(*g_gc_rrng)); if (!g_gc_rrng) abort(); }
    g_gc_rrng[g_gc_rrng_n].lo = lo; g_gc_rrng[g_gc_rrng_n].hi = hi; g_gc_rrng_n++;
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
    /* ONE census, ON MAIN, at heap init — s90 probe measured __dl_iterate_phdr faulting when first driven lazily inside a collect running on a coexpr pthread; the segment set is load-time fixed. */
    if (g_gc_nseg >= 0) return;
    g_gc_nseg = 0;
    dl_iterate_phdr(gc_phdr_cb, (void *)0);
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
static void gc_zeta_frame(char *lo, char *hi)
{
    char *p = lo;
    while (p + 8 <= hi) {
        if (p + 16 <= hi) { DESCR_t *d = (DESCR_t *)p; rt_hblk_t *h = (d->v == DT_S || (d->v == DT_N && d->slen == 0)) ? gc_blk_of(d->s) : (rt_hblk_t *)0;
            if (h && d->s == (char *)(h + 1) && (d->slen == 0xFFFFFFFFu || (uint64_t)d->slen < (uint64_t)h->size)) { rt_gc_visit_descr(d); p += 16; continue; } }
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_collect_ex(int cons_stack)
{
    extern void core_gc_roots(void); extern void gen_gc_roots(void); extern void rt_gc_root_args(void);
    jmp_buf jb; char anchor; long nlive = 0, npin = 0, nfill = 0, before_b, after_b; char *dest; rt_hblk_t **liveo; uint64_t *livef; long li = 0;
    if (g_gc_in || !g_hp_arena) return 0;
    g_gc_in = 1; before_b = (long)(g_hp_top - g_hp_arena);
    g_hp_win = (char *)0; g_hp_wend = (char *)0;
    g_gc_nblk = 0; { char *p = g_hp_arena; while (p < g_hp_top) { g_gc_nblk++; p += ((rt_hblk_t *)p)->size; } }
    g_gc_idx = (rt_hblk_t **)malloc((size_t)g_gc_nblk * sizeof(*g_gc_idx)); if (!g_gc_idx && g_gc_nblk) abort();
    { char *p = g_hp_arena; long i = 0; while (p < g_hp_top) { rt_hblk_t *h = (rt_hblk_t *)p; h->flags &= (uint16_t)~(HBF_MARK | HBF_PIN);
        if (h->type == HB_WS) h->flags |= (uint16_t)(HBF_MARK | HBF_PIN); h->fwd = 0; g_gc_idx[i++] = h; p += h->size; } }
    g_gc_hn = 0; if (g_gc_hs) memset(g_gc_hs, 0, (size_t)g_gc_hcap * sizeof(void *));
    g_gc_ncell = 0; g_gc_nraw = 0; g_gc_interior = 0;
    for (long i = 0; i < g_gc_rpin_n; i++) rt_gc_pin_ptr(g_gc_rpin[i]);
    for (long i = 0; i < g_gc_rrng_n; i++) if (g_gc_rrng[i].lo < g_gc_rrng[i].hi) gc_cons_scan(g_gc_rrng[i].lo, g_gc_rrng[i].hi);
    gc_static_segs_init();
    for (long i = 0; i < g_gc_nseg; i++) if (g_gc_segs[i].lo < g_gc_segs[i].hi) gc_cons_scan_t((const char *)g_gc_segs[i].lo, (const char *)g_gc_segs[i].hi, 1);
    { char *chi; gc_coexpr_roots(&chi);
      if (cons_stack) { int wso = (cons_stack == 2); setjmp(jb); gc_cons_scan_t((const char *)&jb, (const char *)&jb + sizeof jb, wso);
        { char *lo = &anchor, *hi = chi ? chi : gc_stack_top(); if (lo < hi) gc_cons_scan_t((const char *)lo, (const char *)hi, wso); } } }
    gc_root_zeta();
    core_gc_roots(); gen_gc_roots(); rt_gc_root_args();
    for (int si = 0; si < g_gc_shield_n; si++) rt_gc_visit_descr(&g_gc_shield_arr[si]);
    if (g_gc_shield_r) rt_gc_visit_raw(g_gc_shield_r);
    { char *scanned = (char *)calloc((size_t)(g_gc_nblk ? g_gc_nblk : 1), 1); int changed = 1; if (!scanned) abort();
      while (changed) { changed = 0;
        for (long i = 0; i < g_gc_nblk; i++) { rt_hblk_t *h = g_gc_idx[i];
            if (scanned[i] || h->type != HB_WS || !(h->flags & (HBF_MARK | HBF_PIN))) continue;
            scanned[i] = 1; changed = 1; gc_cons_scan((const char *)(h + 1), (const char *)h + h->size); } }
      free(scanned); }
    dest = g_hp_arena;
    for (long i = 0; i < g_gc_nblk; i++) { rt_hblk_t *h = g_gc_idx[i];
        if ((h->flags & HBF_MARK) && (h->type == HB_WS || h->type == HB_ZBLK)) h->flags |= HBF_PIN;
        if (h->flags & HBF_PIN) { h->fwd = (uint64_t)h; nlive++; npin++; }
        else if (h->flags & HBF_MARK) { h->fwd = (uint64_t)dest; dest += h->size; nlive++; }
        else h->fwd = 0;
        if (h->flags & HBF_PIN) dest = (char *)h + h->size; }
    for (long i = 0; i < g_gc_ncell; i++) { DESCR_t *d = g_gc_cells[i]; rt_hblk_t *h = gc_blk_of(d->s); if (h && h->fwd && h->fwd != (uint64_t)h) d->s = (char *)((rt_hblk_t *)h->fwd + 1) + (d->s - (char *)(h + 1)); }
    for (long i = 0; i < g_gc_nraw; i++) { const char **loc = g_gc_raws[i]; rt_hblk_t *h = gc_blk_of(*loc); if (h && h->fwd && h->fwd != (uint64_t)h) *loc = (const char *)((rt_hblk_t *)h->fwd + 1) + (*loc - (const char *)(h + 1)); }
    liveo = (rt_hblk_t **)malloc((size_t)(nlive ? nlive : 1) * sizeof(*liveo)); livef = (uint64_t *)malloc((size_t)(nlive ? nlive : 1) * sizeof(*livef)); if (!liveo || !livef) abort();
    for (long i = 0; i < g_gc_nblk; i++) if (g_gc_idx[i]->fwd) { liveo[li] = g_gc_idx[i]; livef[li] = g_gc_idx[i]->fwd; li++; }
    dest = g_hp_arena;
    for (long i = 0; i < li; i++) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size;
        if ((char *)livef[i] == (char *)h) { if (dest < (char *)h) { rt_hblk_t *fl = (rt_hblk_t *)dest; fl->fwd = 0; fl->size = (uint32_t)((char *)h - dest); fl->type = HB_FILL; fl->flags = HBF_TTL; nfill++;
            if ((long)fl->size > (long)(g_hp_wend - g_hp_win)) { g_hp_win = dest; g_hp_wend = (char *)h; } } dest = (char *)h + sz; }
        else { memmove((void *)livef[i], (void *)h, (size_t)sz); dest = (char *)livef[i] + sz; } }
    g_hp_top = dest; g_hp_blocks = nlive + nfill;
    for (long i = 0; i < li; i++) { rt_hblk_t *nh = (rt_hblk_t *)livef[i]; nh->fwd = 0; nh->flags = (uint16_t)((nh->flags | HBF_TTL) & ~(HBF_MARK | HBF_PIN)); }
    after_b = (long)(g_hp_top - g_hp_arena);
    rt_gcheap_verify();
    g_gc_runs++;
    if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZGC] regeneration #%ld: blocks %ld->%ld (pinned %ld, fill %ld) bytes %ld->%ld reclaimed %ld win=%ld cells=%ld raws=%ld interior=%ld\n", g_gc_runs, g_gc_nblk, nlive, npin, nfill, before_b, after_b, before_b - after_b, (long)(g_hp_wend - g_hp_win), g_gc_ncell, g_gc_nraw, g_gc_interior);
    free(liveo); free(livef); free(g_gc_idx); g_gc_idx = (rt_hblk_t **)0;
    g_gc_in = 0;
    return before_b - after_b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_collect(void)
{
    return gc_collect_ex(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* TR-4 s67: the two counters the &STORAGE/&COLLECTIONS keywords and the FREESPACE arm re-point at now that libgc's GC_get_* queries are gone. */
long rt_gcheap_free(void) { return (long)(g_hp_end - g_hp_top); }
long rt_gc_runs_count(void) { return g_gc_runs; }
