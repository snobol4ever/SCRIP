#include "rt_pl_trail.h"
#include <string.h>
#include "ct_arena.h"
#include "ct_vec.h"
#include <stdlib.h>
#include "frame_layout.h"
#include "zeta_depth.h"
#include "gc_frame_map.h"
#include "ast.h"
#include "g_lower.h"
#include "call_forms.h"
extern const char * bb_op_name(IR_e k);
extern int is_global(const char *);
extern int rt_proc_is_registered(const char *);
extern int rt_proc_is_generator(const char *);
int zls_g_block_args(const IR_graph_t * g);
int zls_g_det_block(const IR_graph_t * g);
int zls_g_frame_bytes(const IR_graph_t * g);
int zls_g_block_base(const IR_graph_t * g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_callee_is_gen(const IR_t * nd) { const char * fn = IR_LIT(nd).sval; return fn && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn); }
#define FL_FC_SYNTH 0x7F000
typedef struct { const IR_t * nd; int scope_id; int off; int loff; int live; int aoff; } zls_entry_t;
typedef struct { int scope_id; int off; int size; unsigned char kind; unsigned char audit; const char * what; const IR_t * nd; int next; } zls_pfield_t;
typedef struct { const char * name; int off; } zls_vslot_t;
typedef struct { const IR_graph_t * g; const char * name; int start_n; const IR_t * anchor; } zls_mark_t;
typedef struct {
    const IR_graph_t * g;
    const char * name;
    int first_scope;
    int n_scopes;
    int nslots;
    int region;
    int resume_off;
    int zeta_mark_off;
    int locals_off;
    int first_vslot;
    int n_vslots;
    struct zls_reuse_s * reuse;
    int n_reuse;
    int scratch_pool;
    const IR_t * scratch_hit_w;
    const IR_t * scratch_hit_t;
    int own_name;
    int gen;
    int hf;
    int fregion;
    int kbase;
    unsigned char * kmap;
    int kmap_n;
} zls_graph_t;
#define ze ((zls_entry_t *)g_lower.ir.ze_v.p)
#define zf ((zls_pfield_t *)g_lower.ir.zf_v.p)
int zls_redisp_off(const IR_t * nd);
void zls_redisp_mark(const IR_t * nd, int off);
static int zls_redisp_node(const IR_t * nd);
static void zls_redisp_fields(int scope_id, int at, const IR_t * nd);
typedef struct { const IR_t * nd; int scope; int end; uint32_t gen; int head; int tail; } znb_slot_t;
static znb_slot_t * znb_probe(cv_t * v, const IR_t * nd, int scope) {
    uint64_t h = ((((uint64_t)(uintptr_t)nd >> 4) ^ ((uint64_t)(uint32_t)scope * 0x9e3779b97f4a7c15ull)) * 0xff51afd7ed558ccdull) & (v->len - 1);
    for (;;) { znb_slot_t * t = &CV_AT(*v, znb_slot_t, h); if (t->gen != g_lower.ir.znb_gen || (t->nd == nd && t->scope == scope)) return t; h = (h + 1) & (v->len - 1); }
}
static int zfc_ok(int f, const IR_t * nd, int scope) { return f >= 0 && f < g_lower.ir.zf_n && zf[f].nd == nd && zf[f].scope_id == scope; }
static int zfc_next(int f) { int n = zf[f].next; return n > f && zfc_ok(n, zf[f].nd, zf[f].scope_id) ? n : -1; }
static void zfc_link(znb_slot_t * t, int f) {
    int last = zfc_ok(t->head, t->nd, t->scope) && t->head < f ? t->head : -1;
    if (zfc_ok(t->tail, t->nd, t->scope) && t->tail < f) last = t->tail;
    else while (last >= 0 && zfc_next(last) >= 0 && zfc_next(last) < f) last = zfc_next(last);
    if (last < 0) t->head = f;
    else zf[last].next = f;
    t->tail = f;
}
static void znb_note(const IR_t * nd, int scope, int end, int f) {
    if ((uint64_t)(g_lower.ir.znb_n + 1) * 2 > g_lower.ir.znb.len) {
        uint32_t nc = g_lower.ir.znb.len ? g_lower.ir.znb.len * 2 : 256;
        cv_t o = g_lower.ir.znb, n = { 0, 0, 0, 0 };
        n.p = ct_zalloc(nc, sizeof(znb_slot_t));
        n.len = nc;
        n.cap = nc;
        n.esz = (uint32_t)sizeof(znb_slot_t);
        g_lower.ir.znb = n;
        for (uint32_t i = 0; i < o.len; i++) { znb_slot_t q = CV_AT(o, znb_slot_t, i); if (q.gen == g_lower.ir.znb_gen) *znb_probe(&g_lower.ir.znb, q.nd, q.scope) = q; }
    }
    znb_slot_t * t = znb_probe(&g_lower.ir.znb, nd, scope);
    if (t->gen != g_lower.ir.znb_gen) {
        t->nd = nd;
        t->scope = scope;
        t->end = end;
        t->gen = g_lower.ir.znb_gen;
        t->head = t->tail = f;
        g_lower.ir.znb_n++;
    } else {
        if (end > t->end) t->end = end;
        zfc_link(t, f);
    }
}
#define zs ((zls_scope_t *)g_lower.ir.zs_v.p)
#define zg ((zls_graph_t *)g_lower.ir.zg_v.p)
#define zv ((zls_vslot_t *)g_lower.ir.zv_v.p)
#define zm ((zls_mark_t *)g_lower.ir.zm_v.p)
#define zx ((int *)g_lower.ir.zx_v.p)
typedef struct zls_reuse_s { const IR_t * nd; int cls; int w; int last; int gpos; const IR_t * guard; int nread; int pooled; int llo; int lhi; int direct; } zls_reuse_t;
#define ZR_CANDIDATE 0
#define ZR_SELF 1
#define ZR_GUARD 2
#define ZR_UNPLACED 3
#define ZR_LOOP 4
#define ZR_DYNAMIC 5
#define ZR_READER 6
#define ZR_DETLEAF 7
#define ZR_NONE -1
static void zls_reuse_plan(const IR_graph_t * g, zls_reuse_t * rec, int * ncp_out, int * nloop_out);
static int zls_reuse_detleaf(const IR_t * c);
static int zls_reuse_straight(IR_e op);
typedef struct { const IR_t * p; int i; } zls_pi_t;
static const char ** zls_nix_slot(const char ** ix, int cap, const char * nm) {
    uint32_t m = (uint32_t)cap - 1;
    for (uint32_t i = ir_name_hash(nm) & m;; i = (i + 1) & m) if (!ix[i] || strcmp(ix[i], nm) == 0) return &ix[i];
}
typedef struct { int * rdk; int * rdm; int ** lw; int * nlw; int * maxlast; int dl_w; } zls_ds_t;
static int zls_direct_slot(const IR_graph_t * g, const zls_reuse_t * rec, const char * rb, int nl, const int * mstart, int i, int dl_w, zls_ds_t * ds);
static void zls_ds_build(zls_ds_t * ds, const IR_graph_t * g, const char * rb, int dl_w);
static void zls_ds_drop(zls_ds_t * ds);
static int zls_wi_cmp(const void * a, const void * b);
typedef struct { const IR_t * nd; int min_off; int span; int * zq; int nzq; } zls_ageom_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_reset(void) {
    for (int i = 0; i < g_lower.ir.zg_n; i++) {
        if (zg[i].reuse) { ct_drop(zg[i].reuse); zg[i].reuse = (struct zls_reuse_s *)0; zg[i].n_reuse = 0; }
        if (zg[i].own_name) { ct_drop((void *)zg[i].name); zg[i].name = (const char *)0; zg[i].own_name = 0; }
    }
    g_lower.ir.ze_n = 0;
    g_lower.ir.zf_n = 0;
    g_lower.ir.zs_n = 0;
    g_lower.ir.zg_n = 0;
    if (g_lower.ir.zgh_cap) memset(g_lower.ir.zgh, 0, (size_t)g_lower.ir.zgh_cap * sizeof(zgh_t));
    g_lower.ir.zv_n = 0;
    g_lower.ir.zm_n = 0;
    g_lower.ir.zx_n = 0;
    g_lower.ir.zx_sorted = 0;
    g_lower.ir.za_n = 0;
    g_lower.ir.znb_gen++;
    g_lower.ir.znb_n = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    cv_reserve(&g_lower.ir.zm_v, (uint32_t)sizeof(zls_mark_t), (uint64_t)g_lower.ir.zm_n + 1, "zm");
    zm[g_lower.ir.zm_n++] = (zls_mark_t){ g, name, g->n, (const IR_t *)0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark_anchor(const IR_graph_t * g, const char * name, const IR_t * anchor) {
    if (!g || !name) return;
    cv_reserve(&g_lower.ir.zm_v, (uint32_t)sizeof(zls_mark_t), (uint64_t)g_lower.ir.zm_n + 1, "zm");
    zm[g_lower.ir.zm_n++] = (zls_mark_t){ g, name, g->n, anchor };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_group_count(const IR_graph_t * g) { int c = 0; for (int i = 0; i < g_lower.ir.zm_n; i++) if (zm[i].g == g) c++; return c; }
const IR_t * zls_g_group_anchor(const IR_graph_t * g, int k) {
    int c = 0;
    for (int i = 0; i < g_lower.ir.zm_n; i++) if (zm[i].g == g) { if (c == k) return zm[i].anchor; c++; }
    return (const IR_t *)0;
}
static uint32_t zgh_hash(const IR_graph_t * g) { return (uint32_t)((((uint64_t)(uintptr_t)g) >> 4) * 0xff51afd7ed558ccdull >> 32); }
static void zgh_put(const IR_graph_t * g, int i) {
    uint32_t m = g_lower.ir.zgh_cap - 1, h = zgh_hash(g) & m;
    for (; g_lower.ir.zgh[h].i1; h = (h + 1) & m) if (g_lower.ir.zgh[h].g == g) return;
    g_lower.ir.zgh[h].g = g;
    g_lower.ir.zgh[h].i1 = i + 1;
}
static void zgh_note(int i) {
    if ((uint64_t)g_lower.ir.zg_n * 2 > (uint64_t)g_lower.ir.zgh_cap) {
        uint32_t c = g_lower.ir.zgh_cap ? g_lower.ir.zgh_cap * 2 : 1024;
        while ((uint64_t)c < (uint64_t)g_lower.ir.zg_n * 2 + 2) c *= 2;
        g_lower.ir.zgh = (zgh_t *)ct_zalloc(c, sizeof(zgh_t));
        g_lower.ir.zgh_cap = c;
        for (int k = 0; k < g_lower.ir.zg_n; k++) zgh_put(zg[k].g, k);
        return;
    }
    zgh_put(zg[i].g, i);
}
static zls_graph_t * zls_g_find(const IR_graph_t * g) {
    if (!g_lower.ir.zg_n || !g_lower.ir.zgh_cap) return (zls_graph_t *)0;
    uint32_t m = g_lower.ir.zgh_cap - 1, h = zgh_hash(g) & m;
    for (; g_lower.ir.zgh[h].i1; h = (h + 1) & m) if (g_lower.ir.zgh[h].g == g) return &zg[g_lower.ir.zgh[h].i1 - 1];
    return (zls_graph_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_graph_name(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    zls_graph_t * r = zls_g_find(g);
    if (r) { if (r->own_name) { ct_drop((void *)r->name); r->own_name = 0; } r->name = name; if (r->first_scope >= 0 && r->first_scope < g_lower.ir.zs_n) zs[r->first_scope].name = name; return; }
    cv_reserve(&g_lower.ir.zg_v, (uint32_t)sizeof(zls_graph_t), (uint64_t)g_lower.ir.zg_n + 1, "zg");
    zg[g_lower.ir.zg_n] = (zls_graph_t){ g, name, -1, 0, 0, 0, -1, -1, 0, 0, 0, (struct zls_reuse_s *)0, 0, -1, (const IR_t *)0, (const IR_t *)0 };
    g_lower.ir.zg_n++;
    zgh_note(g_lower.ir.zg_n - 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * zls_graph_name_get(const IR_graph_t * g) { zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0; return r ? r->name : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zx_cmp(const void * a, const void * b) { const zls_entry_t * x = &ze[*(const int *)a]; const zls_entry_t * y = &ze[*(const int *)b]; return (x->nd > y->nd) - (x->nd < y->nd); }
static void zx_settle(void) {
    int s = (g_lower.ir.zx_sorted <= g_lower.ir.zx_n) ? g_lower.ir.zx_sorted : 0, t = g_lower.ir.zx_n - s;
    if (t > 1) qsort(zx + s, (size_t)t, sizeof(int), zx_cmp);
    if (s > 0 && t > 0 && ze[zx[s - 1]].nd > ze[zx[s]].nd) {
        cv_reserve(&g_lower.ir.zx_tail, (uint32_t)sizeof(int), (uint64_t)t, "g_zx_tail");
        int * tl = (int *)g_lower.ir.zx_tail.p;
        memcpy(tl, zx + s, (size_t)t * sizeof(int));
        int i = s - 1, j = t - 1, k = g_lower.ir.zx_n - 1;
        while (j >= 0) { if (i >= 0 && ze[zx[i]].nd > ze[tl[j]].nd) zx[k--] = zx[i--]; else zx[k--] = tl[j--]; }
    }
    g_lower.ir.zx_sorted = g_lower.ir.zx_n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zfc_first(const IR_t * nd, int scope) {
    const znb_slot_t * t;
    if (!nd || !g_lower.ir.znb.len) return -1;
    t = znb_probe(&g_lower.ir.znb, nd, scope);
    return t->gen == g_lower.ir.znb_gen && zfc_ok(t->head, nd, scope) ? t->head : -1;
}
static const zls_entry_t * zx_find(const IR_t * nd) {
    int lo = 0, hi = g_lower.ir.zx_n - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (ze[zx[m]].nd == nd) return &ze[zx[m]]; if (ze[zx[m]].nd < nd) lo = m + 1; else hi = m - 1; }
    return (const zls_entry_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_field(int scope_id, int off, int size, int kind, int audit, const char * what, const IR_t * nd) {
    cv_reserve(&g_lower.ir.zf_v, (uint32_t)sizeof(zls_pfield_t), (uint64_t)g_lower.ir.zf_n + 1, "zf");
    zf[g_lower.ir.zf_n++] = (zls_pfield_t){ scope_id, off, size, (unsigned char)kind, (unsigned char)audit, what, nd, -1 };
    if (nd) znb_note(nd, scope_id, off + size, g_lower.ir.zf_n - 1);
}
static int zls_fence_widen(const IR_graph_t * g) { return zls_g_resumable_frame(g); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_locals_shifted(IR_e op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_entry(const IR_t * nd, int scope_id, int off) {
    cv_reserve(&g_lower.ir.ze_v, (uint32_t)sizeof(zls_entry_t), (uint64_t)g_lower.ir.ze_n + 1, "ze");
    ze[g_lower.ir.ze_n] = (zls_entry_t){ nd, scope_id, off, off + (zls_locals_shifted(ir_kind0(nd)) ? 16 : 0), 1, -1 };
    cv_reserve(&g_lower.ir.zx_v, (uint32_t)sizeof(int), (uint64_t)g_lower.ir.zx_n + 1, "zx");
    zx[g_lower.ir.zx_n++] = g_lower.ir.ze_n;
    g_lower.ir.ze_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_act_kind(const IR_graph_t * g, const IR_t * nd) {
    if (!zls_g_resumable_frame(g)) return ZK_RAW;
    if (nd->op == IR_CALL_PROC_STAGED) return ZK_PTR_FRAME;
    if (nd->op == IR_CALL_VALUE && IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "goal")) return ZK_PTR_FRAME;
    return ZK_RAW;
}
static int zls_act8_kind(const IR_graph_t * g, const IR_t * nd) {
    if (g && g->cell_frame) return (nd->op == IR_PROC_GEN || nd->op == IR_CALL_PROC_STAGED || nd->op == IR_CALL_VALUE) ? ZK_PTR_FRAME_H : ZK_RAW;
    return ZK_PTR_CODE;
}
static int zls_g_pinned_frame(const IR_graph_t * g);
static int zls_edge_retains(const IR_graph_t * g, const IR_t * nd);
static int zls_grant_locals(const IR_graph_t * g, const IR_t * nd, int scope_id, int off) {
    if (lower_call_node_is_curmov(nd)) {
        zls_field(scope_id, off, 8, ZK_RAW, 0, "call.cursor (r14, the scan cursor, banked at alpha by a by-name tab or move and restored at beta: the reversible effect undone on resume)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "call.cursor pad", nd);
        return 1;
    }
    switch (ir_kind0(nd)) {
        case IR_TO_BY:
        if (g && g->spine_cells) {
            zls_field(scope_id, off, 16, ZK_DESCR, 0, "toby.cur (the running value, a copy of from)", nd);
            zls_field(scope_id, off + 16, 16, ZK_DESCR, 0, "toby.to (a copy of the limit operand)", nd);
            zls_field(scope_id, off + 32, 16, ZK_DESCR, 0, "toby.by (a copy of the step operand)", nd);
            zls_field(scope_id, off + 48, 8, ZK_RAW, 0, "toby.I counter", nd);
            zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "toby.pad", nd);
            return 4;
        }
        case IR_TO:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "to.I counter", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "to.limit", nd);
        if (nd->op == IR_TO && ir_range_kind_is(nd, "db")) {
            zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "to.db G (the generation the enumeration took at alpha, ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 B)", nd);
            zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "to.db pad (unused)", nd);
            if (!zls_g_pinned_frame(g)) return 2;
            zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "to.db prev B (D1: the header at op_off+24 banks it at +24, restored when the enumeration is exhausted)", nd);
            zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "to.db boundary (D1: rsp at the open as a packed RAW word, the header's +32)", nd);
            return 3;
        }
        if (!ir_range_trailed(nd) || !zls_g_pinned_frame(g)) return 1;
        zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "to.hdr+8 (unused; D1: a trailed range's header is at op_off+24, its mark in to.limit's word)", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "to.hdr+16 (unused)", nd);
        zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "to.prev B (D1: the header's +24, restored when the range is exhausted)", nd);
        zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "to.boundary (D1: rsp at the open as a packed RAW word, the header's +32)", nd);
        return 3;
        case IR_MAKE_LIST:
        { for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "list.elem", nd); return 0 + nd->n_operands; }
        case IR_SCAN_ENTER:
        zls_field(scope_id, off, 16, ZK_DESCR, 0,
            "scan.leave out3 sigma cell (the OUTER subject pointer as a DT_S cell {tag, 0, sigma}, held across the whole inner scan body and reloaded into r13 at leave: a collected-heap pointer the "
            "walker relocates as a cell, so a generator frame suspended off the chain keeps it through the spine sweep -- ARCH-GC 13.5, the PTR_GC word of CTO-88 converted at STEP B)", nd);
        zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "scan.leave out3 delta", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "scan.leave out3 Delta", nd);
        return 2;
        case IR_MATCH_BEGIN:
        zls_field(scope_id, off + 4, 4, ZK_RAW, 0, "head.cursor pad (unused upper half of the quad)", nd);
        zls_field(scope_id, off, 4, ZK_RAW, 0,
            "head.cursor (ZB-FC-3d granted: the LIVE anchor lives in HEAD's self-pushed 32B rsp cell at [rsp+0] via the op_fc_wbytes window; this FLAT +0 then holds the RELEASE-stashed match START r"
            "ead by IR_MATCH_REPLACE -- same logical offset, window-disambiguated, so REPLACE's template is unchanged both paths)", nd);
        zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "head.zeta_mark (BB-OWNED-zeta statement-scope saved the rsp mark() pointer; ZB-FC-3d granted: cell-resident at [rsp+8])", nd);
        zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0,
            "head.zls2_mark (retired ALLOC-port era: saved the rsp mark() cursor; released by head's own omega-choke on failure / IR_MATCH_END on success — the RSP-carve twin of head.zeta_mark, wi"
            "dened to a second quad because the first quad's padding is spent.  ZB-FC-3d granted: cell-resident at [rsp+16] holding the PRE-PUSH rsp, so the S10e unwind releases HEAD's cell and ever"
            "y suspended pattern cell in one mov)", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0,
            "head.end (SN4-REPL: end cursor stashed by IR_MATCH_END when the statement carries a replacement, read by IR_MATCH_REPLACE; ZB-FC-3d: FLAT on both paths -- post-unwind lifetime)", nd);
        zls_field(scope_id, off + 32, 8, ZK_RAW, 0,
            "head.dcap_mark (REG-6 PEND-PROMOTE: α saves live-r12 pend top = this match's MARK; ω/RELEASE truncate r12 from it — the cell [RT_DCAP_TOP] is now seed-source only, prologue-read, ne"
            "ver written mid-match; ZK_RAW — points into the base-pinned dcap island, never GC-moved.  ZB-FC-3d: FLAT on both paths -- RELEASE's post-unwind pump reads it after the match dies)", nd)
            ;
        zls_field(scope_id, off + 40, 8, ZK_RAW, 0,
            "head.incoming____ (DEAD at REG-2 — ___ is no longer the pend cursor, nothing saves or restores here; slot left ALLOCATED v1 so op_off accounting does not ripple, reclaim is a named fo"
            "llow-up.  Re-tenants at REG-3 as the frame-___ era's saved-outer-___ if the wire-header [+24] route changes)", nd);
        zls_field(scope_id, off + 48, 8, ZK_RAW, 0,
            "head.sigma_save (PATCTX, Lon directive 2026-07-29: HEAD saves the OUTER match's Σ=r13 at α before rt_match_enter installs the new subject; BOTH exits restore -- head's own omega-choke"
            " on failure, release_pump's tail on success -- and re-sync the C-side Σ/Σlen mirror via rt_match_ctx_restore, so nested matches (deferred *F() evaluation, EVAL/CODE, pump-committed *V"
            "AR proc bodies that themselves match) are LIFO-sound by construction.  ZK_RAW deliberately: Σ is register-resident today and registers are GC-invisible regardless; when ZHEAP moves str"
            "ings THIS slot is exactly where a suspended subject becomes a findable root -- retag interior-PTR_GC in that rung.  KNOWN BYPASS, named not hidden: pat_seal kills (ABORT, FENCE-seal) ju"
            "mp straight to fJ and skip the omega-choke, so they skip this restore the same way they already skip the zls unwind and CAS pop -- the ___ match-frame rung is what closes that class)",
            nd);
        zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "head.delta_save (PATCTX: outer δ=r14)", nd);
        zls_field(scope_id, off + 64, 8, ZK_RAW, 0, "head.Delta_save (PATCTX: outer Δ=r15)", nd);
        zls_field(scope_id, off + 72, 8, ZK_RAW, 0,
            "head.capgen_save (PATCTX-2: the OUTER match g_cap_gen id, read at alpha before rt_match_enter draws a fresh id from the monotonic well; both exits restore it through rt_match_ctx_restor"
            "e -- nest1 autopsy: the inner match stamp invalidated the outer SAVE bracket, pop no-opd, top returned 0, R captured [0,end).  Occupies the former pad quad, so the 5-quad grant and ever"
            "y downstream offset are unchanged)", nd);
        return 5;
        case IR_MATCH_SPAN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "span.cnt/cur", nd);
        return 1;
        case IR_MATCH_BAL:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "bal.n/start/depth", nd);
        return 1;
        case IR_MATCH_BREAK:
        case IR_MATCH_BREAKX:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "break.cnt/cur", nd);
        return 1;
        case IR_MATCH_FENCE0:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "fence0.watermark (RESERVED — own-extent whack rung; unread today)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "fence0.pad (unused)", nd);
        return 1;
        case IR_MATCH_FENCE1:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "fence.watermark (α-saved rsp; σ/φ glue restores)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "fence.cap_top (α-saved r12, the conditional-capture stack top; β restores it, so a FENCE backtracked into drops the captures its body recorded)",
            nd);
        zls_field(scope_id, off + 16, 4, ZK_RAW, 0, "fence.cursor_tag (α writes 0, DT_SNUL, so the quad holding the cursor can never read as a DESCR)", nd);
        zls_field(scope_id, off + 20, 4, ZK_RAW, 0, "fence.cursor (α-saved r14d; β restores it before handing its predecessor the backtrack, which reads the cursor it left at its own γ)", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "fence.pad (unused)", nd);
        zls_field(scope_id, off + 32, 8, ZK_RAW, 0,
            "fence.watermark_u2 (the second α-saved rsp the U-2 frame form writes and both releases read, H29-FIX: inside this grant, never a neighbour's cell)", nd);
        zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "fence.pad (unused)", nd);
        return 3;
        case IR_MATCH_ARB:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "arb.cnt/cur (matched-length +0 4B, saved-start +4 4B)", nd);
        zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0,
            "arb.zls2 activation block ptr (save-slot-in-frame, retired ALLOC-port era: reuses this node's existing pad, same reuse precedent as IR_MATCH_BEGIN.zeta_mark; block itself is a separate "
            "RSP-carve allocation, header +0 chains the previous activation's ptr)", nd);
        return 1;
        case IR_MATCH_REM:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "match.cursor save", nd);
        return 1;
        case IR_MATCH_DEFER:
        zls_field(scope_id, off, 16, ZK_RAW, 0,
            "defer.pad (ZS-2 jmp-entry, Lon s58: the fn/frame cell pair is DELETED — the blob is a jmp-entered new activation that self-allocates on rsp with a 32B wire header, so there is nothing"
            " to stash and nothing to guard; quad KEPT at 16B so no later node's offset shifts. s137 OVER-SEAL: when IR_t.seal, quad +0 is REPURPOSED as the fence-demarked sync watermark — α stam"
            "ps rsp there, the γ/ω glues and β bulk-restore it; +8 stays pad)", nd);
        return 1;
        case IR_MATCH_VALUE:
        return 0;
        case IR_MATCH_TAB:
        case IR_MATCH_RTAB:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "tab.cursor save (+0 4B r14d saved at α, restored at β; +4 pad)", nd);
        return 1;
        case IR_MATCH_ARBNO:
        if (IR_LIT(nd).ival == 1) {
            zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno.owner quad: entry/yield/i/cap (4x4B; SN4-NARY-ARBNO one-node form)", nd);
            zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0,
                "arbno.COLLECTION ptr (rt_zcol_push-grown per-iteration elements: 16B header {prev_view, saved_delta} + body slot window; the rsp flavor = linked frame chain + explicit count in the "
                "header — Lon ruling 2026-07-12, lands at ZB-ITER under FL_ARBNO_STACK)", nd);
            zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "arbno.saved_rsp (alpha saves rsp here; exhaust L(2) restores it)", nd);
            zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "arbno.pad (unused sixth quad of the 3-slot grant)", nd);
            zls_field(scope_id, off + 32, 8, ZK_RAW, 0,
                "arbno.saved_outer____ (W-1c.2: unconditional — chain-beta always saves MATCH_BEGIN ___ into this slot before repointing ___ as element view; exhaust L(2) always restores it; view-"
                "restores at sigma/phi re-derive ___ from rsp unconditionally)", nd);
            return 3;
        }
        return 0;
        case IR_MATCH_ASSIGN_SAVE:
        zls_field(scope_id, off, 8, ZK_PTR_GC, 0, "capture.stack ws u32[] ([0]=cap, frames from [1]; box α-push/β-pop)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "capture.stack gen(+8,4B)/sp(+12,4B)", nd);
        return 1;
        case IR_MATCH_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "alt.entry cursor save (+0 4B r14d; +4 4B dead — was alt_i, killed by ALT-FLAT s202 address dispatch)", nd);
        zls_field(scope_id, off + 8, 8, ZK_PTR_CODE, 0,
            "alt.resume continuation (ALT-FLAT s202: each arm's sigma stub stores its own resume trampoline address via lea rip; beta is one indirect jmp — the alt_i cmp-chain is dead.  Retenants "
            "the old dcap-pad quad)", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "alt.pad (unused fourth quad of the 2-slot grant)", nd);
        zls_field(scope_id, off + 16, 8, ZK_PTR_CODE, 0,
            "alt.next-entry continuation (ALT-FLAT s202: alpha and each entry stub store the NEXT arm's entry-stub address; the fail-advance is delta-restore + one indirect jmp — the entry cmp-cha"
            "in is dead)", nd);
        return 2;
        case IR_SCAN_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scanalt.entry δ save (+16 from box base, 4B r14d) + dcap height (+20, 4B)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scanalt.alt_i live-alternative index (+24, 4B; α=0, na_f ++; β dispatches) (+28 pad)", nd);
        return 2;
        case IR_SCAN:
        if (nd) {
            zls_field(scope_id, off, 16, ZK_DESCR, 0,
                "scan.suspend-leave live subject cell: a DT_S cell {tag, slen = the inner subject's carried length, s = the subject pointer rt_scan_live_subj banks across the suspension}, which rt_s"
                "can_reenter_live takes back at resume -- a collected-heap pointer the walker relocates as a cell, so a generator suspended off the chain keeps it through the spine sweep (ARCH-GC 13"
                ".5, the PTR_GC word converted at STEP B)", nd);
            zls_field(scope_id, off + 16, 8, ZK_RAW, 0,
                "scan.suspend-leave bank: the inner delta, 4B at +0 (icon-scan-env-value-residue slice 3: per-activation env round-trip across suspension -- the shared scan_saved stack cannot carry "
                "it, +1 leak per exhaustion-through-scan measured)", nd);
            zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "scan.suspend-leave bank pad (unused)", nd);
            return 2;
        }
        return 0;
        case IR_SCAN_TAB:
        case IR_SCAN_MOVE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.r14 data-backtrack save", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.pad (unused)", nd);
        return 1;
        case IR_BOUND:
        zls_field(scope_id, off, 8, ZK_RAW, 0,
            "bound.saved rsp (Op_Mark: bounded-expression entry frontier; IR_UNMARK restores it, discarding abandoned retained-suspension FC carves — interp.r Op_Unmark rsp=efp-1)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "bound.saved rsp at entry -- the gamma-fence release frontier (rung 9, CEO-690)", nd);
        if (!zls_fence_widen(g)) return 1;
        zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "bound.saved B at entry, banked BEFORE the header open makes B this bound -- the commit and the failure arm restore it (D1)", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "bound.pad (unused)", nd);
        zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "bound.boundary (D1: rsp at the open as a packed RAW word, the header's +32 the trail test reads through B)", nd);
        zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "bound.pad2 (unused)", nd);
        return 3;
        case IR_SCAN_UPTO:
        case IR_SCAN_FIND:
        case IR_SCAN_MATCH:
        case IR_SCAN_BAL:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.cursor", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.len/counter", nd);
        return 1;
        case IR_INITIAL:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "initial.pad (unused low half)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "initial.once flag (0->1)", nd);
        return 1;
        case IR_ITERATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "iterate.index i (alpha=0, beta inc)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "iterate.pad (unused)", nd);
        return 1;
        case IR_LIMIT_GATE:
        if (!(g && g->home_frame)) return 0;
        zls_field(scope_id, off, 8, ZK_RAW, 0, "limit.counter (the gate's record on a home-frame graph: the gate zeroes it at alpha, the IR_LIMIT that names the gate as an operand counts here)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "limit.resumed flag (the gate's record: the limit's beta sets it before resuming the generator, its alpha consumes it)", nd);
        if (zls_home_why(nd))
            zls_field(scope_id, off + 16, 8, ZK_RAW, 0,
            "limit.mark (rsp at a homed gate's alpha: a spent limit leaves through the gate's beta, which restores it past the frame of the generator it cuts)", nd);
        return 1;
        case IR_LIMIT:
        if (g && g->home_frame) return 0;
        zls_field(scope_id, off, 8, ZK_RAW, 0, "limit.counter", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0,
            "limit.resumed flag (+24 from box base; beta sets before resuming the generator, alpha consumes and clears it so a FRESH entry zeroes limit.counter -- see bb_limit.cpp)", nd);
        return 1;
        case IR_REPALT:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "repalt.yielded flag (clear/yield/test)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "repalt.pad (unused)", nd);
        return 1;
        case IR_REV_ASSIGN:
        case IR_REV_ASSIGN_VAR:
        zls_field(scope_id, off, 16, ZK_DESCR, 0, "revasg.saved old value (beta restore; LIVE across suspension — GC must trace)", nd);
        return 1;
        case IR_REV_SWAP:
        zls_field(scope_id, off, 16, ZK_DESCR, 0, "revswap.saved lhs old (beta restore; LIVE across suspension — GC must trace)", nd);
        zls_field(scope_id, off + 16, 16, ZK_DESCR, 0, "revswap.saved rhs old (beta restore; LIVE across suspension — GC must trace)", nd);
        zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "revswap.delta spill (in-scan r14 round-trip)", nd);
        zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "revswap.Delta spill (in-scan r15, read-only len)", nd);
        zls_field(scope_id, off + 48, 8, ZK_RAW, 0,
            "revswap.lhs cell pointer parked across the rhs NV_PTR_fn lookup (global operand only; a raw interior pointer into the name table, never a heap object -- ZK_RAW so GC does not trace it)",
            nd);
        zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "revswap.pad (unused eighth quad of the 4-slot grant; registered CEO-820 so the frame map has no unmapped word)", nd);
        return 4;
        case IR_KW_ICON:
        case IR_KW_ICON_GEN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "kw.gen counter", nd);
        return 1;
        case IR_KW_SNOBOL4:
        return 0;
        case IR_DEREF:
        case IR_ASSIGN_VAR:
        case IR_RANDOM:
        case IR_SWAP_VAR:
        return 0;
        case IR_KW_ASSIGN:
        case IR_KW_ASSIGN_SNOBOL4:
        return 0;
        case IR_CREATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "coexpr.handle ctx* (malloc'd non-GC — never trace/relocate)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "coexpr.handle pad (unwritten)", nd);
        zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "coexpr.marshal r12 (copied out by scrip_coexpr_create; dead at return)", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "coexpr.marshal r13", nd);
        zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "coexpr.marshal r14", nd);
        zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "coexpr.marshal r15", nd);
        zls_field(scope_id, off + 48, 8, ZK_RAW, 0, "coexpr.marshal rbx", nd);
        zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "coexpr.marshal ___", nd);
        return 4;
        case IR_ASSIGN:
        case IR_STATEMENT_BEGIN:
        case IR_STATEMENT_END:
        case IR_STMT_MARK:
        case IR_SETEXIT_TEST:
        case IR_LINE_MARK:
        case IR_STATEMENT:
        return 0;
        case IR_CLEANUP:
        if (IR_LIT(nd).ival == CLEANUP_BANK) {
            zls_field(scope_id, off, 16, ZK_RAW, 0, "cleanup.bank pad (unused)", nd);
            zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "cleanup.bank -- B at the entry of a cut scope with no mark (an inline call/1), read by a scoped cut's probe at +16 as a mark's banked B is",
                nd);
            zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "cleanup.bank pad (unused)", nd);
            return 2;
        }
        if (IR_LIT(nd).ival != CLEANUP_OPEN) return 0;
        zls_field(scope_id, off + CLEANUP_REC_LINK, 8, ZK_RAW, 0,
            "cleanup.link -- the next older setup_call_cleanup record, a stack address the trail header's cleanup cell chains through, never a heap pointer", nd);
        zls_field(scope_id, off + CLEANUP_REC_BS, 8, ZK_RAW, 0, "cleanup.B_s -- r13 banked at OPEN, after the construct's mark: G's exit is deterministic when r13 reads it again", nd);
        zls_field(scope_id, off + CLEANUP_REC_STATE, 8, ZK_RAW, 0, "cleanup.state -- 0 DONE (a zeroed frame reads DONE), 1 ACTIVE while G runs, 2 PENDING after a nondeterministic exit", nd);
        zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "cleanup.pad (unused)", nd);
        zls_field(scope_id, off + CLEANUP_REC_C, 16, ZK_DESCR, 0, "cleanup.C -- the cleanup goal as a term, built at OPEN, run by '$scc_cut' when a cut drops the record", nd);
        return 3;
        case IR_TABLE_HDR:
        zls_field(scope_id, off, 16, ZK_DESCR, 0, "table.hdr address -- {DT_I, the header base}, the argument the open leaf reads and makes the current tabling header", nd);
        for (int k = 0; k < TABLE_HDR_CELLS; k++)
            zls_field(scope_id, off + 16 + 16 * k, 16, ZK_DESCR, 0,
            "table.hdr cell -- cell 0 the banked previous header as {DT_I, address}, cells 1-9 the evaluation state a tabled call owns (in progress, raise, phase, new answer, dependency, semi-naive "
            "context, consumer position, highest position, sliced tables); a zero tag reads as not owned here", nd);
        return 1 + TABLE_HDR_CELLS;
        case IR_GATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0,
            "gate.live arm index (+16 from box base; IR_GATE_ARM banks its own position in this gate's operand list at each arm's success, the box beta dispatches on it over compile-time wired ports"
            "). NOT A CODE ADDRESS: the stored resume target this slot used to hold died with the two label ops on Lon's order 2026-09-13 (CEO-693, 'There are no labels'), and the field is ZK_RAW ra"
            "ther than ZK_PTR_CODE for that reason -- a collector that sees a code pointer here is reading a small integer. Granted at rung 5 -- both templates had ALWAYS addressed [op_off+16], whic"
            "h is this field, while the kind fell through to default and was granted only its 16-byte result, so the gate write landed 8 bytes past the grant. Harmless with ONE such box live and a c"
            "ore dump with two, because the second box's region began where the first's gate was still being written", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "gate.pad (unused)", nd);
        return 1;
        case IR_DISJUNCTION:
        if (nd->op == IR_DISJUNCTION && nd->n_operands > 0) {
            zls_field(scope_id, off, 8, ZK_RAW, 0,
                "disj.alt_i live-alternative index (+16 from box base; nary self-state, MOVE_LABEL-ERAD: α=0, φ-glue ++, β dispatches; value DESCR = the box result slot at [base], option-B per-ar"
                "m copy in σ-glue) (+24 pad)", nd);
            zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "disj.mark (the trail mark; D1: +0 of this disjunction's own choice header)", nd);
            if (!zls_g_pinned_frame(g)) return 1;
            zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "disj.hdr+8 (unused)", nd);
            zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "disj.hdr+16 (unused)", nd);
            zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "disj.prev B (D1: the header's +24, restored when the last alternative is dispatched)", nd);
            zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "disj.boundary (D1: rsp at the open as a packed RAW word, the header's +32 the trail test reads through B)", nd);
            return 3;
        }
        zls_field(scope_id, off, 8, ZK_RAW, 0, "gate.live arm index (operand-less disjunction shares the gate box)", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "gate.pad (unused)", nd);
        return 1;
        case IR_CALL_BUILTIN_GEN:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0, "callgen.resume position (alpha=0, runtime writes next start)", nd);
        zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "callgen.pad (unused)", nd);
        return 1 + nd->n_operands;
        case IR_PROC_GEN:
        case IR_CALL_VALUE:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * nd->n_operands, 8, zls_act_kind(g, nd), 0,
            "callgen.act +0 (ZK_RAW, or ZK_PTR_FRAME on a pinned caller's Prolog arm: the spine arm writes 0 at alpha and 1 once its epilogue has run, or 2 for the whole life of a call that opened a"
            " NON-RESUMABLE callee -- a plain procedure return"
            "s once and releases, so beta fails forward on 2 instead of resuming (ceo 2026-09-19, CVSPINE_t in descr.h); at the gamma landing it holds the callee's RETAINED frame base (rax) or 0 whe"
            "n the callee released -- a machine-stack address, never a heap block; measured from bb_call_proc_staged.cpp; an Icon value call's n2 road banks 3 | cells<<8 | ftc<<32 and turns"
            " it to 4 at its first gamma, its +8 then the callee's H -- bb_call_value.cpp)", nd);
        zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, zls_act8_kind(g, nd), 0,
            "callgen.act +8 (WRITTEN, not a pad: ZK_PTR_FRAME_H on an Icon cells graph -- H, the callee's retained generator frame, and ZERO on every other landing and at alpha; else the callee's gr"
            "aph beta at the gamma landing, or the saved rsp on the LCO/forwarding arms; read back on beta to resume the callee -- bb_call_proc_staged.cpp)", nd);
        return 1 + nd->n_operands;
        default:
        if (ir_kind0(nd) == IR_CALL || ir_is_call_kind(ir_kind0(nd))) {
            if (ir_kind0(nd) == IR_CALL_PROC_STAGED) {
                if (!zls_callee_is_gen(nd) && !(g->home_frame && zls_g_resumable_frame(g) && zls_edge_retains(g, nd))) return 0;
                zls_field(scope_id, off, 8, zls_act_kind(g, nd), 0,
                    "callgen.act +0 (ZK_RAW, or ZK_PTR_FRAME on a pinned caller's Prolog arm: the spine arm writes 0 at alpha and 1 once its epilogue has run, or 2 for the whole life of a call that "
                    "opened a NON-RESUMABLE callee -- a plain procedur"
                    "e returns once and releases, so beta fails forward on 2 instead of resuming (ceo 2026-09-19, CVSPINE_t in descr.h); at the gamma landing it holds the callee's RETAINED frame bas"
                    "e (rax) or 0 when the callee released -- a machine-stack address, never a heap block; measured from bb_call_proc_staged.cpp)", nd);
                zls_field(scope_id, off + 8, 8, zls_act8_kind(g, nd), 0,
                    "callgen.act +8 (WRITTEN, not a pad: ZK_PTR_FRAME_H on an Icon cells graph -- H, the callee's retained generator frame, and ZERO on every other landing and at alpha; else the cal"
                    "lee's graph beta at the gamma landing, or the saved rsp on the LCO/forwarding arms; read back on beta to resume the callee -- bb_call_proc_staged.cpp)", nd);
                return 1;
            }
            for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
            {
                const char * cmn = IR_LIT(nd).sval;
                if (zls_redisp_node(nd)) { if (zls_redisp_off(nd) < 0) zls_redisp_fields(scope_id, off + 16 * nd->n_operands, nd); return nd->n_operands + RK_REDISP_REC_SLOTS; }
            }
            {
                const char * cmn = IR_LIT(nd).sval;
                if (cmn && (!strcmp(cmn, "tab") || !strcmp(cmn, "move"))) {
                    zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0,
                        "scan.saved_delta — ICN-BYNAME-CURSOR-RESTORE: a cursor-mover (tab/move, and =s == tab(match(s))) reached by-name through rt_call_arr has no inline bb_scan_tab body, so it "
                        "also had no saved-δ slot and its β degenerated to a bare jmp ω — the backtrack never restored &pos. This quad is that slot; bb_call_byname_str writes r14 here at α and"
                        " reloads it in β, mirroring bb_scan_tab's restore-δ-and-FAIL port. Same extra-quad shape as callgen.act above.", nd);
                    zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "scan.saved_delta pad (unused)", nd);
                    return 1 + nd->n_operands;
                }
            }
            return nd->n_operands;
        }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_is_wiring(IR_e op) {
    return op == IR_GOTO || op == IR_GATE_ARM || op == IR_GOTO_DEFERRED || op == IR_SUCCEED || op == IR_FAIL || op == IR_RETURN || op == IR_SUSPEND || op == IR_CORET || op == IR_COFAIL ||
        op == IR_CUT || op == IR_MATCH_END || op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_STMT_MARK || op == IR_SETEXIT_TEST || op == IR_LINE_MARK ||
        op == IR_MATCH_LIT || op == IR_MATCH_ABORT;
}
static int zls_locals_shifted(IR_e op) {
    return op == IR_BOUND || op == IR_MATCH_BEGIN || op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARB || op == IR_MATCH_BAL || op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_MATCH_ARBNO ||
        op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE ||
        op == IR_MATCH_ASSIGN_SAVE || op == IR_SCAN_ENTER || op == IR_INITIAL || op == IR_CLEANUP;
}
int fc_arm_member(const IR_t * nd);
int fc_leaf_cell(const IR_t * nd, long * k);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_cells_on(void) { return 1; }
int fc_cells_active(void) { return fc_cells_on(); }
static int zls_fc_cell(const IR_t * nd) {
    if (!fc_cells_on()) return 0;
    if (!nd) return 0;
    { extern int fc_arm_member(const IR_t *); if (fc_arm_member(nd)) return 0; }
    switch (ir_kind0(nd)) {
        case IR_MATCH_SPAN:
        case IR_MATCH_TAB:
        case IR_MATCH_RTAB:
        case IR_MATCH_BREAK:
        case IR_MATCH_BREAKX:
        case IR_MATCH_BAL:
        case IR_MATCH_REM:
        case IR_MATCH_ARB:
        return 16;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_no_result(const IR_t * nd) { return zls_is_wiring(ir_kind0(nd)) || (nd->op == IR_DEFINE && ir_define_ch8_role(nd) >= 1 && ir_define_ch8_role(nd) <= 3); }
static int zls_grant(const IR_graph_t * g, const IR_t * nd, int scope_id, int off) {
    if (zls_no_result(nd)) return 0;
    if (nd->op == IR_CLEANUP && IR_LIT(nd).ival != CLEANUP_OPEN && IR_LIT(nd).ival != CLEANUP_BANK) return 0;
    zls_entry(nd, scope_id, off);
    zls_field(scope_id, off, 16, ZK_DESCR, 0, "result", nd);
    if (zls_fc_cell(nd)) { ze[g_lower.ir.ze_n - 1].loff = FL_FC_SYNTH; return 1; }
    return 1 + zls_grant_locals(g, nd, scope_id, off + 16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_elide_ok(IR_e op) {
    return op == IR_UNIFY_CONST || op == IR_UNIFY_FIRST || op == IR_UNIFY_VALUE || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_LEN ||
        op == IR_MATCH_LIT || op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_CMP_TEST || op == IR_ASSIGN;
}
static int zls_writes_no_result(IR_e op) { return op == IR_UNIFY_CONST || op == IR_UNIFY_FIRST || op == IR_UNIFY_VALUE; }
static int zls_s4_ok(IR_e op) {
    return op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_BAL || op == IR_MATCH_ALTERNATE ||
        op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const IR_t ** k; int * v; size_t cap; } zls_nidx_t;
static void zls_nidx_build(zls_nidx_t * m, const IR_graph_t * g) {
    size_t cap = 16;
    while (cap < (size_t)g->n * 2 + 2) cap <<= 1;
    m->k = (const IR_t **)ct_zalloc(cap, sizeof(*m->k));
    m->v = (int *)ct_alloc(sizeof(int) * cap);
    m->cap = cap;
    for (int i = 0; i < g->n; i++) {
        const IR_t * nd = g->all[i];
        if (!nd) continue;
        size_t h = ((size_t)(uintptr_t)nd >> 4) & (cap - 1);
        while (m->k[h] && m->k[h] != nd) h = (h + 1) & (cap - 1);
        if (!m->k[h]) { m->k[h] = nd; m->v[h] = i; }
    }
}
static int zls_reuse_index(const zls_nidx_t * m, const IR_t * nd) {
    size_t h = ((size_t)(uintptr_t)nd >> 4) & (m->cap - 1);
    while (m->k[h]) { if (m->k[h] == nd) return m->v[h]; h = (h + 1) & (m->cap - 1); }
    return -1;
}
static int zls_unify_kind(IR_e op) { return op == IR_UNIFY_CONST || op == IR_UNIFY_STRUCT || op == IR_UNIFY_FIRST || op == IR_UNIFY_VALUE; }
static void zls_mark_value_refs(const IR_graph_t * g, char * live, char * uonly) {
    zls_nidx_t nx;
    zls_nidx_build(&nx, g);
    for (int k = 0; k < g->n; k++) {
        const IR_t * c = g->all[k];
        if (!c) continue;
        if (c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_FENCE0 || c->op == IR_MATCH_FENCE1 || c->op == IR_GATE_ARM || c->op == IR_GATE) continue;
        for (int j = 0; j < c->n_operands; j++) {
            const IR_t * p = c->operands[j];
            if (!p) continue;
            if (j == 0 && (c->op == IR_MATCH_ASSIGN_COND || c->op == IR_MATCH_ASSIGN_IMM)) continue;
            if (uonly) { int i = zls_reuse_index(&nx, p); if (i >= 0) { if (zls_unify_kind(ir_kind0(c)) && ir_kind0(p) != IR_UNIFY_STRUCT) { if (!uonly[i]) uonly[i] = 1; } else uonly[i] = 2; } }
            { int i = zls_reuse_index(&nx, p); if (i >= 0) live[i] = 1; }
        }
    }
    ct_drop(nx.k);
    ct_drop(nx.v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant_elide(const IR_graph_t * g, const IR_t * nd, int scope_id, int off, int live, int * scratch_off) {
    if (zls_no_result(nd)) return 0;
    if (!live && zls_elide_ok(ir_kind0(nd))) {
        if (*scratch_off < 0) {
            *scratch_off = off;
            zls_entry(nd, scope_id, off);
            ze[g_lower.ir.ze_n - 1].live = 0;
            zls_field(scope_id, off, 16, ZK_DESCR, 0, "result (SLOT-ELIDE shared dead-result scratch — every later dead leaf in this graph aliases here)", nd);
            return 1;
        }
        zls_entry(nd, scope_id, *scratch_off);
        ze[g_lower.ir.ze_n - 1].live = 0;
        return 0;
    }
    if (!live && zls_s4_ok(ir_kind0(nd))) {
        zls_entry(nd, scope_id, off);
        ze[g_lower.ir.ze_n - 1].loff = off;
        ze[g_lower.ir.ze_n - 1].live = 0;
        if (zls_fc_cell(nd)) { ze[g_lower.ir.ze_n - 1].loff = FL_FC_SYNTH; return 0; }
        return zls_grant_locals(g, nd, scope_id, off);
    }
    int ei = g_lower.ir.ze_n;
    int n = zls_grant(g, nd, scope_id, off);
    if (g_lower.ir.ze_n > ei) ze[ei].live = live;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_result_live(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->live : 1; }
int zls_node_off(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->off : -0x40000000; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_scope_new(int parent, int klass, const char * name) {
    cv_reserve(&g_lower.ir.zs_v, (uint32_t)sizeof(zls_scope_t), (uint64_t)g_lower.ir.zs_n + 1, "zs");
    zs[g_lower.ir.zs_n] = (zls_scope_t){ g_lower.ir.zs_n, parent, klass, name, -1, 0, 0x7fffffff, 0 };
    return g_lower.ir.zs_n++;
}
int fc_alt_fpmax(const IR_t *);
int fc_alt_extent(const IR_t *, int *, int *);
int fc_geom(const IR_t *, long *);
int emit_patzeta_lookup(const char *, int *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fct_defer_susp(const IR_t * nd) {
    if (!nd || (nd->op != IR_MATCH_DEFER) || nd->seal != 2) return -1;
    const char * pn = 0;
    for (int j = 0; j < nd->n_operands; j++) {
        const IR_t * o = nd->operands[j];
        if (o && o->op == IR_LIT_STRING && IR_LIT(o).sval && !strncmp(IR_LIT(o).sval, "PAT$", 4)) { pn = IR_LIT(o).sval; break; }
    }
    int susp = 0;
    if (!pn || !emit_patzeta_lookup(pn, &susp) || susp <= 0) return -1;
    return susp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fct_rsp_range(IR_graph_t * g, int k0, int k1) {
    int rsp = 0;
    for (int j = k0; j < k1 && j < g->n; j++) {
        IR_t * x = g->all[j];
        if (!x) continue;
        int op = (int)ir_kind0(x);
        if (op == IR_ASSIGN || op == IR_GOTO || op == IR_GOTO_DEFERRED || (op == IR_DEFINE && ir_define_sr_citizen(x)) || op == IR_MATCH_BEGIN || op == IR_MATCH_END || op == IR_MATCH_REPLACE ||
            op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_STMT_MARK || op == IR_SETEXIT_TEST || op == IR_LINE_MARK || op == IR_MATCH_LIT || op == IR_MATCH_LEN ||
            op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE ||
            op == IR_MATCH_ALTERNATE || op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_BOUND || op == IR_UNMARK || op == IR_CONJUNCTION || op == IR_CUT || op == IR_GATE_ARM ||
            op == IR_RETURN || (op == IR_DISJUNCTION && x->n_operands == 0) || (op == IR_MATCH_DEFER && x->pat_static && IR_LIT(x).sval && !strncmp(IR_LIT(x).sval, "PATV$", 5))) continue;
        if (op == IR_MATCH_ALTERNATE) { int _b = 0, _e = 0; if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > j + 1) j = _e - 1; } continue; }
        rsp += 16;
    }
    return rsp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fct_fp_range(IR_graph_t * g, int k0, int k1) {
    int fp = 0;
    long fck = 0;
    for (int j = k0; j < k1 && j < g->n; j++) {
        IR_t * x = g->all[j];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) { int _b = 0, _e = 0; if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > j + 1) j = _e - 1; } continue; }
        if ((x->op == IR_MATCH_DEFER) && g_lower.ir.fct_pricing) { int s = fct_defer_susp(x); if (s > 0) fp += s; continue; }
        if (fc_geom(x, &fck)) fp += (int)fck;
    }
    return fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_slot_census(IR_graph_t * g) {
    if (g_lower.ir.zls_slot_census_on < 0) { const char * e = getenv("SCRIP_SLOT_CENSUS"); g_lower.ir.zls_slot_census_on = (e && *e == '1') ? 1 : 0; }
    if (!g_lower.ir.zls_slot_census_on || !g) return;
    extern int zls_result_off(const IR_t *);
    int G = 0, L = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || zls_is_wiring(ir_kind0(nd)) || zls_result_off(nd) < 0) continue;
        G++;
        int ref = 0;
        for (int k = 0; k < g->n && !ref; k++) {
            IR_t * c = g->all[k];
            if (!c || c == nd) continue;
            if (c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_ARBNO || c->op == IR_MATCH_FENCE1 || c->op == IR_SCAN_ALTERNATE || c->op == IR_REPALT || c->op == IR_GATE_ARM) continue;
            for (int j = 0; j < c->n_operands; j++) if (c->operands[j] == nd) { ref = 1; break; }
        }
        if (ref) L++;
    }
    g_lower.ir.zls_slot_census_tg += G;
    g_lower.ir.zls_slot_census_tl += L;
    g_lower.ir.zls_slot_census_tn++;
    fprintf(stderr, "[SLOT-CENSUS] g=%p n=%d result_quads=%d value_refd=%d dead_floor=%d rq_bytes %d -> %d | TOTALS graphs=%ld quads=%ld refd=%ld dead=%ld bytes %ld -> %ld\n", (void *)g, g->n, G, L,
        G - L, G * 16, (L + 1) * 16, g_lower.ir.zls_slot_census_tn, g_lower.ir.zls_slot_census_tg, g_lower.ir.zls_slot_census_tl, g_lower.ir.zls_slot_census_tg - g_lower.ir.zls_slot_census_tl,
        g_lower.ir.zls_slot_census_tg * 16, (g_lower.ir.zls_slot_census_tl + g_lower.ir.zls_slot_census_tn) * 16);
}
void zls_fct_finalize(IR_graph_t * g, int late);
void fc_vlit_register(const IR_t *);
void fc_vread_register(const IR_t *, int);
void fc_vbinop_register(const IR_t *);
int is_global(const char *);
void fc_vwpop_register(const IR_t *, long);
void fc_subj_register(const IR_t *);
int zc_nofc(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_vvar_ok(const IR_graph_t * g, const IR_t * r) {
    const char * vn = IR_LIT(r).sval;
    return vn && vn[0] != '&' && ((is_global(vn) && !graph_has_local(g, vn)) || !strcmp(vn, "write") || !strcmp(vn, "writes"));
}
static int fc_vbinop_ok(long long v) { return v == 0 || v == 1 || v == 2 || v == 3 || v == 4 || v == 11 || v == 18; }
int fc_vbinop_active(const IR_t * nd);
int fc_vlit_active(const IR_t * nd);
int fc_vread_fp(const IR_t * nd);
int fc_vdj_active(const IR_t * nd);
static int zls_hf_resident(const IR_graph_t * g, const IR_t * nd);
extern int planner_bound_spine(const IR_graph_t * g, const IR_t * nd);
static int fc_vunop_ok(const IR_t * nd) { return nd->op == IR_UNOP && nd->n_operands == 1 && ((int)IR_LIT(nd).ival == TT_MNS || (int)IR_LIT(nd).ival == TT_PLS); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_call_ok(const IR_t * nd) { (void)nd; return 0; }
static void fc_vpost_push(cv_t * pv, int * pn, const IR_t * nd) { cv_reserve(pv, (uint32_t)sizeof(const IR_t *), (uint64_t)*pn + 1, "post"); CV_AT(*pv, const IR_t *, *pn) = nd; (*pn)++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_vtree_scan(const IR_graph_t * g, const IR_t * nd, cv_t * pv, int * pn, int depth) {
    if (!nd || depth > 24) return 0;
    if (nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || (nd->op == IR_VAR && fc_vvar_ok(g, nd))) { fc_vpost_push(pv, pn, nd); return 1; }
    if (fc_vunop_ok(nd) && fc_vtree_scan(g, nd->operands[0], pv, pn, depth + 1)) { fc_vpost_push(pv, pn, nd); return 1; }
    if (fc_call_ok(nd) && g_lower.ir.fcc_gfence && nd->operands[0] &&
        (nd->operands[0]->op == IR_LIT_INTEGER || nd->operands[0]->op == IR_LIT_STRING || (nd->operands[0]->op == IR_VAR && fc_vvar_ok(g, nd->operands[0])))) {
        fc_vpost_push(pv, pn, nd->operands[0]);
        fc_vpost_push(pv, pn, nd);
        return 1;
    }
    if (nd->op == IR_BINOP && nd->n_operands == 2 && fc_vbinop_ok((long long)IR_LIT(nd).ival) && fc_vtree_scan(g, nd->operands[0], pv, pn, depth + 1) &&
        fc_vtree_scan(g, nd->operands[1], pv, pn, depth + 1)) {
        fc_vpost_push(pv, pn, nd);
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zls_pas_display_name(int lvl) {
    if (!g_lower.ir.zls_pas_display_name_init) {
        for (int i = 0; i < 13; i++) snprintf(g_lower.ir.zls_pas_display_name_names[i], sizeof g_lower.ir.zls_pas_display_name_names[i], "__pas_display_%d", i + 4);
        g_lower.ir.zls_pas_display_name_init = 1;
    }
    return (lvl >= 4 && lvl <= 16) ? g_lower.ir.zls_pas_display_name_names[lvl - 4] : (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant_at(const IR_graph_t * g, const IR_t * nd, int i, int sc, int root, const zls_reuse_t * rec, int dl_base, const char * lv, const char * uo, int * scratch_off, int off) {
    if (rec[i].pooled >= 0) { zls_entry(nd, sc, rec[i].pooled); if (dl_base >= 0 && zls_reuse_detleaf(nd)) ze[g_lower.ir.ze_n - 1].aoff = dl_base; return 0; }
    if (g_lower.ir.zls_build_eon && uo[i] == 1 && rec[i].w < 0 && (nd->op == IR_VAR_REF || nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_ATOM || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL))
        return 0;
    if (dl_base >= 0 && zls_reuse_detleaf(nd) && sc == root) {
        if (g_lower.ir.zls_build_eon && !lv[i]) {
            if (*scratch_off < 0) {
                *scratch_off = off;
                zls_entry(nd, sc, off);
                ze[g_lower.ir.ze_n - 1].live = 0;
                ze[g_lower.ir.ze_n - 1].aoff = dl_base;
                zls_field(sc, off, 16, ZK_DESCR, 0, "result (SLOT-ELIDE shared dead-result scratch — every later dead leaf in this graph aliases here)", nd);
                return 1;
            }
            zls_entry(nd, sc, *scratch_off);
            ze[g_lower.ir.ze_n - 1].live = 0;
            ze[g_lower.ir.ze_n - 1].aoff = dl_base;
            return 0;
        }
        zls_entry(nd, sc, off);
        ze[g_lower.ir.ze_n - 1].live = g_lower.ir.zls_build_eon ? lv[i] : 1;
        ze[g_lower.ir.ze_n - 1].aoff = dl_base;
        zls_field(sc, off, 16, ZK_DESCR, 0, "result", nd);
        return 1;
    }
    return g_lower.ir.zls_build_eon ? zls_grant_elide(g, nd, sc, off, lv[i], scratch_off) : zls_grant(g, nd, sc, off);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const IR_t * nd; int c; } zls_ovn_t;
static int zls_ovn_cmp(const void * a, const void * b) { const IR_t * x = ((const zls_ovn_t *)a)->nd, * y = ((const zls_ovn_t *)b)->nd; return x < y ? -1 : x > y ? 1 : 0; }
static int zls_ovn_clause(const zls_ovn_t * v, int n, const IR_t * nd) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (v[m].nd == nd) return v[m].c; if (v[m].nd < nd) lo = m + 1; else hi = m - 1; }
    return -1;
}
static int zls_ov_shares(const IR_graph_t * g) {
    int na = g->n_alts, al[na + 1], n = 0;
    ir_clause_bounds(g, al, na);
    int lo = al[0], hi = al[na];
    zls_ovn_t v[hi - lo > 0 ? hi - lo : 1];
    for (int c = 0; c < na; c++) for (int i = al[c]; i < al[c + 1]; i++) if (g->all[i]) v[n++] = (zls_ovn_t){ g->all[i], c };
    qsort(v, (size_t)n, sizeof v[0], zls_ovn_cmp);
    for (int c = 0; c < na; c++) for (int i = al[c]; i < al[c + 1]; i++) {
        const IR_t * x = g->all[i];
        if (!x) continue;
        int o = x->γ.node ? zls_ovn_clause(v, n, x->γ.node) : -1;
        if (o >= 0 && o != c) return 1;
        o = x->ω.node ? zls_ovn_clause(v, n, x->ω.node) : -1;
        if (o >= 0 && o != c) return 1;
        for (int j = 0; j < x->n_operands; j++) { o = x->operands[j] ? zls_ovn_clause(v, n, x->operands[j]) : -1; if (o >= 0 && o != c) return 1; }
    }
    return 0;
}
static int zls_overlay(IR_graph_t * g, int base, int k0, int root, const char * rb, const zls_reuse_t * rec, int dl_base, const char * lv, const char * uo, int * scratch_off, int mode) {
    int na = g->n_alts, al[na + 1];
    ir_clause_bounds(g, al, na);
    int lo = al[0], hi = al[na];
    if (*scratch_off < 0 && g_lower.ir.zls_build_eon && mode != 1) {
        int need = 0;
        for (int i = lo; i < hi && !need; i++) {
            const IR_t * nd = g->all[i];
            if (!nd || !rb[i] || rec[i].pooled >= 0 || lv[i]) continue;
            if ((dl_base >= 0 && zls_reuse_detleaf(nd)) || (!zls_is_wiring(ir_kind0(nd)) && zls_elide_ok(ir_kind0(nd)))) need = 1;
        }
        if (need) {
            *scratch_off = base + k0 * 16;
            zls_field(root, *scratch_off, 16, ZK_DESCR, 0, "result (SLOT-ELIDE shared dead-result scratch, below the clause overlay so every clause's dead leaves alias one slot)", (const IR_t *)0);
            k0++;
        }
    }
    long S = 0;
    for (int i = lo; i < hi; i++) {
        const IR_t * nd = g->all[i];
        if (!nd || !rb[i] || (mode == 1 && !zls_hf_resident(g, nd)) || (mode == 2 && zls_hf_resident(g, nd))) continue;
        int a = g_lower.ir.zf_n, b = g_lower.ir.ze_n, x = g_lower.ir.zx_n, st = *scratch_off;
        S += zls_grant_at(g, nd, i, root, root, rec, dl_base, lv, uo, &st, base + k0 * 16);
        g_lower.ir.zf_n = a;
        g_lower.ir.ze_n = b;
        g_lower.ir.zx_n = x;
    }
    int seq = S <= 0 || S > (1L << 16);
    unsigned short km[seq ? 1 : S];
    unsigned char occ[seq ? 1 : S];
    memset(km, 0, sizeof km);
    int kend = k0;
    if (mode != 2) g->alt_zlo = base + k0 * 16;
    for (int c = 0; c < na; c++) {
        int ce = 0, low = 0;
        memset(occ, 0, sizeof occ);
        for (int i = al[c]; i < al[c + 1]; i++) {
            const IR_t * nd = g->all[i];
            if (!nd || !rb[i] || (mode == 1 && !zls_hf_resident(g, nd)) || (mode == 2 && zls_hf_resident(g, nd))) continue;
            int a = g_lower.ir.zf_n, b = g_lower.ir.ze_n, x = g_lower.ir.zx_n, st = *scratch_off, pr = base + k0 * 16;
            int n = zls_grant_at(g, nd, i, root, root, rec, dl_base, lv, uo, &st, pr);
            int nf = g_lower.ir.zf_n - a, s0 = -1;
            int poff[nf > 0 ? nf : 1], psz[nf > 0 ? nf : 1], pk[nf > 0 ? nf : 1];
            for (int f = 0; f < nf; f++) { poff[f] = zf[a + f].off - pr; psz[f] = zf[a + f].size; pk[f] = zf[a + f].kind; }
            g_lower.ir.zf_n = a;
            g_lower.ir.ze_n = b;
            g_lower.ir.zx_n = x;
            if (n <= 0) { zls_grant_at(g, nd, i, root, root, rec, dl_base, lv, uo, scratch_off, pr); continue; }
            for (int s = seq ? -1 : low; s >= 0 && s + n <= S && s0 < 0; s++) {
                int ok = 1;
                for (int j = 0; j < n && ok; j++) if (occ[s + j]) ok = 0;
                for (int f = 0; f < nf && ok; f++) for (int y = poff[f]; y < poff[f] + psz[f] && ok; y += 4) {
                    long rel = (long)s * 16 + y, sl = rel / 16;
                    if (rel < 0 || sl >= S) continue;
                    unsigned have = (km[sl] >> (4 * ((rel % 16) / 4))) & 15u;
                    if (have && have != (unsigned)pk[f] + 1u) ok = 0;
                }
                if (ok) s0 = s;
            }
            if (s0 < 0) { seq = 1; s0 = kend - k0 > ce ? kend - k0 : ce; }
            int got = zls_grant_at(g, nd, i, root, root, rec, dl_base, lv, uo, scratch_off, base + (k0 + s0) * 16);
            if (!seq) {
                for (int j = 0; j < got && s0 + j < S; j++) occ[s0 + j] = 1;
                for (int f = 0; f < nf; f++) for (int y = poff[f]; y < poff[f] + psz[f]; y += 4) {
                    long rel = (long)s0 * 16 + y, sl = rel / 16;
                    if (rel >= 0 && sl < S) km[sl] = (unsigned short)(km[sl] | (((unsigned)pk[f] + 1u) << (4 * ((rel % 16) / 4))));
                }
                while (low < S && occ[low]) low++;
            }
            if (s0 + got > ce) ce = s0 + got;
            if (k0 + ce > kend) kend = k0 + ce;
        }
        if (mode != 2) g->alt_zhi[c] = base + (k0 + ce) * 16;
    }
    return kend;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_redisp_node(const IR_t * nd) {
    const char * cmn = (nd && (ir_kind0(nd) == IR_CALL || ir_is_call_kind(ir_kind0(nd)))) ? IR_LIT(nd).sval : (const char *)0;
    return (cmn && (!strcmp(cmn, "meth_call") || !strcmp(cmn, "callsame") || !strcmp(cmn, "nextsame") || !strcmp(cmn, "callwith") || !strcmp(cmn, "nextwith"))) ? 1 : 0;
}
static void zls_redisp_fields(int scope_id, int at, const IR_t * nd) {
    zls_field(scope_id, at, 16, ZK_DESCR, 0, "redisp.self (the method call's redispatch record, CEO-1552: the invocant, for callsame/nextsame; lives as long as the call, chained from one head cell)",
        nd);
    zls_field(scope_id, at + 16, 16, ZK_DESCR, 0, "redisp.mname (the method name)", nd);
    zls_field(scope_id, at + 32, 8, ZK_RAW, 0, "redisp.prev (the chain link to the enclosing dispatch's record)", nd);
    zls_field(scope_id, at + 40, 8, ZK_RAW, 0, "redisp.cname (the invocant's class name, interned)", nd);
    zls_field(scope_id, at + 48, 8, ZK_RAW, 0, "redisp.found_idx | nargs (two 32-bit halves)", nd);
    zls_field(scope_id, at + 56, 8, ZK_RAW, 0, "redisp.args (a pointer into this frame's own argv: self, mname, the rest)", nd);
    zls_field(scope_id, at + 64, 8, ZK_RAW, 0, "redisp.skip_mname | local (two 32-bit halves)", nd);
    zls_field(scope_id, at + 72, 16 * RK_REDISP_REC_SLOTS - 72, ZK_RAW, 0, "redisp pad (the record is RK_REDISP_REC_BYTES; the grant is whole slots)", nd);
}
static int zls_hf_key_base(const IR_graph_t * g, int fregion, int base) {
    int kt = ((zls_g_pinned_frame(g) ? (g->root_graph ? FLAT_FRAME_ALLOWANCE_ROOT : FLAT_FRAME_ALLOWANCE_PINNED) : FLAT_FRAME_ALLOWANCE) + fregion + 8 * g->standing_cells + 15) & ~15;
    int ft = kt + (g->nparams + g->nlocals) * 16;
    int bb = !g->cell_frame ? kt : zls_g_has_suspend(g) ? ((ft + 15) & ~15) + 72 : ft;
    int top = zls_g_block_args(g) ? bb + 16 * g->nparams : kt;
    return top > base ? (top + 15) & ~15 : base;
}
static int zls_node_var_names(const IR_t * nd, const char ** vns) {
    int nvn = 0;
    if (nd->op == IR_ASSIGN) vns[nvn++] = IR_LIT(nd).sval;
    else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) vns[nvn++] = IR_LIT(nd->operands[1]).sval;
    else if (nd->op == IR_REV_SWAP) {
        vns[nvn++] = IR_LIT(nd).sval;
        if (nd->n_operands > 0 && nd->operands[0]) vns[nvn++] = IR_LIT(nd->operands[0]).sval;
    } else if (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_UNIFY_FIRST || nd->op == IR_UNIFY_VALUE) vns[nvn++] = IR_LIT(nd).sval;
    return nvn;
}
typedef struct {
    int n;
    int nd;
    int * dj;
    unsigned long long * rg;
    int * fo;
    int * fa;
    int * ro;
    int * ra;
    unsigned char * exc;
    unsigned char * seed;
    unsigned char * mf;
    unsigned char * mb;
    int * stk;
    zls_nidx_t nx;
} zls_vflow_t;
static int zls_vflow_succ(const IR_graph_t * g, const zls_nidx_t * nx, int i, int k) {
    const IR_t * x = g->all[i];
    const IR_t * t = !x ? (const IR_t *)0 : k == 0 ? x->γ.node : k == 1 ? x->ω.node : k - 2 < x->n_operands ? x->operands[k - 2] : (const IR_t *)0;
    int j = t ? zls_reuse_index(nx, t) : -1;
    return j == i ? -1 : j;
}
static int zls_vflow_gamma_to(const zls_vflow_t * f, const IR_graph_t * g, int i) { const IR_t * t = g->all[i] ? g->all[i]->γ.node : (const IR_t *)0; return t ? zls_reuse_index(&f->nx, t) : -1; }
static int zls_vflow_operand_to(const zls_vflow_t * f, const IR_graph_t * g, int i, int k) {
    const IR_t * t = (g->all[i] && k < g->all[i]->n_operands) ? g->all[i]->operands[k] : (const IR_t *)0;
    return t ? zls_reuse_index(&f->nx, t) : -1;
}
static void zls_vflow_regions(zls_vflow_t * f, const IR_graph_t * g) {
    int n = f->n, nd = 0;
    unsigned long long bef = 1ULL << 62;
    f->dj = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1));
    for (int i = 0; i < n; i++) { const IR_t * d = g->all[i]; if (d && d->op == IR_DISJUNCTION && IR_LIT(d).ival > 0 && d->n_operands >= 2 * (int)IR_LIT(d).ival) f->dj[nd++] = i; }
    f->nd = nd;
    f->rg = (unsigned long long *)ct_zalloc((size_t)(nd * n + 1), sizeof(unsigned long long));
    if (nd == 0) return;
    int * po = (int *)ct_zalloc((size_t)(n + 2), sizeof(int)), * pa = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1)), * fill = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1));
    unsigned char * ps = (unsigned char *)ct_alloc((size_t)n);
    for (int i = 0; i < n; i++) { int j = zls_vflow_gamma_to(f, g, i); if (j >= 0 && j != i) po[j + 1]++; }
    for (int i = 0; i < n; i++) po[i + 1] += po[i];
    for (int i = 0; i < n; i++) fill[i] = po[i];
    for (int i = 0; i < n; i++) { int j = zls_vflow_gamma_to(f, g, i); if (j >= 0 && j != i) pa[fill[j]++] = i; }
    for (int k = 0; k < nd; k++) {
        const IR_t * d = g->all[f->dj[k]];
        unsigned long long * m = f->rg + (size_t)k * n;
        for (int a = 0, na = (int)IR_LIT(d).ival, top = 0; a < na && a < 62; a++, top = 0) {
            unsigned long long bit = 1ULL << a;
            int s = d->operands[2 * a] ? zls_reuse_index(&f->nx, d->operands[2 * a]) : -1;
            if (s >= 0 && s != f->dj[k]) { m[s] |= bit; f->stk[top++] = s; }
            while (top > 0) {
                int x = f->stk[--top];
                for (int e = -1, ne = g->all[x]->n_operands; e < ne; e++) {
                    int y = e < 0 ? zls_vflow_gamma_to(f, g, x) : zls_vflow_operand_to(f, g, x, e);
                    if (y >= 0 && y != f->dj[k] && !(m[y] & bit)) { m[y] |= bit; f->stk[top++] = y; }
                }
            }
        }
        memset(ps, 0, (size_t)n);
        for (int top = 0, y = zls_vflow_gamma_to(f, g, f->dj[k]); ; ) {
            if (y >= 0 && y != f->dj[k] && !ps[y]) { ps[y] = 1; f->stk[top++] = y; }
            if (top == 0) break;
            y = zls_vflow_gamma_to(f, g, f->stk[--top]);
        }
        for (int top = 0, x = f->dj[k]; ; ) {
            for (int e = po[x]; e < po[x + 1]; e++) {
                int y = pa[e];
                if (y == f->dj[k] || m[y] || ps[y]) continue;
                m[y] |= bef;
                f->stk[top++] = y;
                for (int q = 0; q < g->all[y]->n_operands; q++) { int o = zls_vflow_operand_to(f, g, y, q); if (o >= 0 && o != f->dj[k] && !m[o] && !ps[o]) m[o] |= bef; }
            }
            if (top == 0) break;
            x = f->stk[--top];
        }
    }
    for (int k = 0; k < nd; k++) {
        const unsigned long long * m = f->rg + (size_t)k * n;
        for (int j = 0; j < n; j++) if ((m[j] & ((1ULL << 62) - 1)) && f->exc[j]) { zls_frame_resident_mark(g->all[f->dj[k]]); break; }
    }
    ct_drop(po);
    ct_drop(pa);
    ct_drop(fill);
    ct_drop(ps);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_vflow_build(zls_vflow_t * f, const IR_graph_t * g) {
    extern int planner_span_retains(const IR_graph_t *, int, int);
    int n = g->n, m = 0;
    f->n = n;
    zls_nidx_build(&f->nx, g);
    f->fo = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1));
    f->ro = (int *)ct_zalloc((size_t)(n + 2), sizeof(int));
    f->exc = (unsigned char *)ct_alloc((size_t)n);
    f->seed = (unsigned char *)ct_alloc((size_t)n);
    f->mf = (unsigned char *)ct_alloc((size_t)n);
    f->mb = (unsigned char *)ct_alloc((size_t)n);
    f->stk = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1));
    for (int i = 0; i < n; i++) {
        f->fo[i] = m;
        f->exc[i] = g->all[i] ? (unsigned char)(planner_span_retains(g, i, i) ? 1 : 0) : 0;
        for (int k = 0, e = g->all[i] ? 2 + g->all[i]->n_operands : 0; k < e; k++) { int j = zls_vflow_succ(g, &f->nx, i, k); if (j >= 0) { m++; f->ro[j + 1]++; } }
    }
    f->fo[n] = m;
    f->fa = (int *)ct_alloc(sizeof(int) * (size_t)(m + 1));
    f->ra = (int *)ct_alloc(sizeof(int) * (size_t)(m + 1));
    for (int i = 0; i < n; i++) f->ro[i + 1] += f->ro[i];
    {
        int * fill = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1));
        for (int i = 0; i < n; i++) fill[i] = f->ro[i];
        for (int i = 0, w = 0; i < n; i++) {
            for (int k = 0, e = g->all[i] ? 2 + g->all[i]->n_operands : 0; k < e; k++) { int j = zls_vflow_succ(g, &f->nx, i, k); if (j >= 0) { f->fa[w++] = j; f->ra[fill[j]++] = i; } }
        }
        ct_drop(fill);
    }
    zls_vflow_regions(f, g);
}
static void zls_vflow_drop(zls_vflow_t * f) {
    ct_drop(f->fo);
    ct_drop(f->fa);
    ct_drop(f->ro);
    ct_drop(f->ra);
    ct_drop(f->exc);
    ct_drop(f->seed);
    ct_drop(f->mf);
    ct_drop(f->mb);
    ct_drop(f->stk);
    ct_drop(f->dj);
    ct_drop(f->rg);
    ct_drop(f->nx.k);
    ct_drop(f->nx.v);
}
static void zls_vflow_close(const zls_vflow_t * f, const int * o, const int * a, unsigned char * mark) {
    int top = 0;
    memset(mark, 0, (size_t)f->n);
    for (int i = 0; i < f->n; i++) if (f->seed[i]) { mark[i] = 1; f->stk[top++] = i; }
    while (top > 0) { int x = f->stk[--top]; for (int e = o[x]; e < o[x + 1]; e++) { int y = a[e]; if (!mark[y]) { mark[y] = 1; f->stk[top++] = y; } } }
}
static int zls_var_span_retains(const IR_graph_t * g, const char * vn, zls_vflow_t * f) {
    int n = g->n, any = 0;
    memset(f->seed, 0, (size_t)n);
    for (int j = 0; j < n; j++) {
        const IR_t * x = g->all[j];
        const char * xs[2];
        int nx = x ? zls_node_var_names(x, xs) : 0;
        for (int q = 0; q < nx; q++) if (xs[q] && !strcmp(xs[q], vn)) { f->seed[j] = 1; any = 1; }
    }
    if (!any) return 0;
    for (int i = 0; i < n; i++) {
        if (!f->exc[i]) continue;
        for (int k = 0; k < g->all[i]->n_operands; k++) { int j = g->all[i]->operands[k] ? zls_reuse_index(&f->nx, g->all[i]->operands[k]) : -1; if (j >= 0 && f->seed[j]) return 1; }
    }
    zls_vflow_close(f, f->fo, f->fa, f->mf);
    zls_vflow_close(f, f->ro, f->ra, f->mb);
    for (int i = 0; i < n; i++) if (f->exc[i] && f->mf[i] && f->mb[i]) return 1;
    return 0;
}
static int zls_var_arm_split(const zls_vflow_t * f) {
    unsigned long long arms = (1ULL << 62) - 1, bef = 1ULL << 62, oth = 1ULL << 63;
    for (int k = 0; k < f->nd; k++) {
        const unsigned long long * m = f->rg + (size_t)k * f->n;
        unsigned long long u = 0;
        for (int j = 0; j < f->n; j++) if (f->seed[j]) u |= m[j] ? m[j] : oth;
        if ((u & arms) && !(u & bef) && __builtin_popcountll(u) >= 2) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_frame_words(IR_graph_t * g, zls_graph_t * r, int root, int base, int k, const char * rb, const char ** nix, int nix_cap, int hf) {
    r->resume_off = -1;
    for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && g->all[i]->op == IR_SUSPEND) {
        r->resume_off = base + k * 16;
        zls_field(root, r->resume_off, 8, ZK_PTR_CODE, 0, "gen-proc resume continuation", (const IR_t *)0);
        zls_field(root, r->resume_off + 8, 8, ZK_RAW, 0, "resume.pad (unused)", (const IR_t *)0);
        k += 1;
        break;
    }
    if (r->resume_off < 0 && g->resumable_callable) {
        r->resume_off = base + k * 16;
        zls_field(root, r->resume_off, 8, ZK_PTR_CODE, 0,
            "resumable-callable blob β continuation (NCB-2/SZ-1: α-init = tail element's β; esi=1 re-entry dispatches jmp [slot] — the graph-level twin of alt.resume)", (const IR_t *)0);
        zls_field(root, r->resume_off + 8, 8, ZK_RAW, 0, "resume.pad (unused)", (const IR_t *)0);
        k += 1;
    }
    r->locals_off = base + k * 16;
    r->zeta_mark_off = -1;
    zls_vflow_t vf;
    if (hf) zls_vflow_build(&vf, g);
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || !rb[i]) continue;
        const char * vns[2];
        int nvn = zls_node_var_names(nd, vns);
        for (int q = 0; q < nvn; q++) {
            const char * vn = vns[q];
            if (!vn || vn[0] == '&' || (is_global(vn) && !graph_has_local(g, vn))) continue;
            const char ** ns = zls_nix_slot(nix, nix_cap, vn);
            if (*ns) continue;
            *ns = vn;
            cv_reserve(&g_lower.ir.zv_v, (uint32_t)sizeof(zls_vslot_t), (uint64_t)g_lower.ir.zv_n + 1, "zv");
            if (hf && !zls_home_local_test(g, vn) && (zls_var_span_retains(g, vn, &vf) || zls_var_arm_split(&vf))) zls_home_local_declare(g, vn, 8);
            if (hf && !zls_home_local_test(g, vn)) { zv[g_lower.ir.zv_n++] = (zls_vslot_t){ vn, -2 }; r->n_vslots++; continue; }
            zv[g_lower.ir.zv_n++] = (zls_vslot_t){ vn, base + k * 16 };
            r->n_vslots++;
            zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "local", (const IR_t *)0);
            k++;
        }
    }
    if (g->caller_frame && g->lnames && g->nlocals > 0) {
        for (int i = 0; i < g->nlocals; i++) {
            const char * vn = g->lnames[i];
            if (!vn || vn[0] == '&') continue;
            const char ** ns = zls_nix_slot(nix, nix_cap, vn);
            if (*ns) continue;
            *ns = vn;
            cv_reserve(&g_lower.ir.zv_v, (uint32_t)sizeof(zls_vslot_t), (uint64_t)g_lower.ir.zv_n + 1, "zv");
            zv[g_lower.ir.zv_n++] = (zls_vslot_t){ vn, base + k * 16 };
            r->n_vslots++;
            zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "declared-local", (const IR_t *)0);
            k++;
        }
    }
    if (g->zframe_graph && !g->cell_frame && g->decl_level == 3) {
        for (int _pdl = 4; _pdl <= 16; _pdl++) {
            const char * nm = zls_pas_display_name(_pdl);
            cv_reserve(&g_lower.ir.zv_v, (uint32_t)sizeof(zls_vslot_t), (uint64_t)g_lower.ir.zv_n + 1, "zv");
            zv[g_lower.ir.zv_n++] = (zls_vslot_t){ nm, base + k * 16 };
            r->n_vslots++;
            zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "pas-display-spill", (const IR_t *)0);
            k++;
        }
    }
    if (hf) zls_vflow_drop(&vf);
    return k;
}
static int zls_dl_place(int root, int base, int k, int dl_w) {
    int dl_base = base + k * 16;
    for (int q = 0; q < dl_w; q++)
        zls_field(root, dl_base + q * 16, 16, ZK_DESCR, 0,
        "call.argv (pooled: every sealed det leaf of this graph marshals here -- live only between one call's alpha and its gamma; the callee returns once and its beta falls to omega; a "
        "straight temp read only by one sealed call's operand j is granted slot j directly and the marshal copies nothing)", (const IR_t *)0);
    return dl_base;
}
void zls_build(IR_graph_t * g) {
    if (!g) return;
    {
        int f = 1;
        for (int i = 0; i < g->n && f; i++) {
            IR_t * x = g->all[i];
            if (!x) continue;
            if (x->op == IR_GOTO_DEFERRED || x->op == IR_GATE || x->op == IR_MATCH_BEGIN || x->op == IR_MATCH_DEFER) f = 0;
            else if ((x->op == IR_DEFINE && ir_define_sr_citizen(x))) {
                long long v = IR_LIT(x).ival;
                if (v == 1 || v == 2) f = 0;
            }
                else if ((ir_kind0(x) == IR_CALL_BUILTIN || ir_kind0(x) == IR_CALL_SNOBOL4 || ir_kind0(x) == IR_CALL) && IR_LIT(x).sval &&
                (!strcmp(IR_LIT(x).sval, "EVAL") || !strcmp(IR_LIT(x).sval, "CODE"))) f = 0;
        }
        g_lower.ir.fcc_gfence = f;
    }
    for (int vi = 0; vi < g->n; vi++) {
        IR_t * a = g->all[vi];
        if (!(a && a->op == IR_ASSIGN && a->n_operands == 1 && a->operands[0])) continue;
        { const char * vn = IR_LIT(a).sval; if (!(vn && is_global(vn) && !graph_has_local(g, vn))) continue; }
        IR_t * r = a->operands[0];
        if ((r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL || r->op == IR_LIT_CHARSET || r->op == IR_LIT_NAME ||
            (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&' &&
            ((is_global(IR_LIT(r).sval) && !graph_has_local(g, IR_LIT(r).sval)) || !strcmp(IR_LIT(r).sval, "write") || !strcmp(IR_LIT(r).sval, "writes")))) && r->γ.node == a) {
            fc_vlit_register(r);
            fc_vread_register(a, 0);
            continue;
        }
        if ((r->op == IR_BINOP || fc_vunop_ok(r) || fc_call_ok(r)) && r->γ.node == a) {
            cv_t _pv = { 0, 0, 0, 0 };
            int pn = 0;
            int _ts = fc_vtree_scan(g, r, &_pv, &pn, 0);
            const IR_t ** post = (const IR_t **)_pv.p;
            {
                if (g_lower.ir.zls_build_dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); g_lower.ir.zls_build_dbg = (e && *e == '1') ? 1 : 0; }
                if (g_lower.ir.zls_build_dbg && _ts && pn > 0) fprintf(stderr, "[FCC] tree ts=%d pn=%d tail_is_r=%d\n", _ts, pn, (post[pn-1] == r) ? 1 : 0);
            }
            if (_ts && post[pn - 1] == r) {
                int ok = 1, L = 0, B = 0;
                for (int i = 0; i + 1 < pn; i++) {
                    const IR_t * gx = post[i]->γ.node;
                    if (gx == post[i + 1]) continue;
                    if (gx && ir_kind0(gx) == IR_DEFINE && gx->γ.node == post[i + 1] && (ir_kind0(post[i + 1]) == IR_CALL || ir_kind0(post[i + 1]) == IR_CALL_PROC_STAGED)) {
                        long long v = IR_LIT(gx).ival;
                        if (!(v == 1 || v == 2 || v == 3)) continue;
                    }
                    ok = 0;
                    break;
                }
                for (int i = 0; i < pn; i++) { if (post[i]->op == IR_BINOP || post[i]->op == IR_UNOP) B++; else L++; }
                {
                    if (g_lower.ir.zls_build_dbg2 < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); g_lower.ir.zls_build_dbg2 = (e && *e == '1') ? 1 : 0; }
                    if (g_lower.ir.zls_build_dbg2 && pn > 1) fprintf(stderr, "[FCC] ok=%d L=%d B=%d p0g=%p p1=%p\n", ok, L, B, (void*)(pn > 1 ? post[0]->γ.node : 0), (void*)(pn > 1 ? post[1] : 0));
                    if (g_lower.ir.zls_build_dbg2 && pn > 1 && post[0]->γ.node)
                        fprintf(stderr, "[FCC] p0=%p p0op=%d p0g_op=%d r=%p rop=%d\n", (void*)post[0], (int)post[0]->op, (int)post[0]->γ.node->op, (void*)r, (int)r->op);
                }
                if (ok) {
                    int d = 0;
                    for (int i = 0; i < pn; i++) {
                        const IR_t * x = post[i];
                        if (x->op == IR_BINOP) {
                            fc_vbinop_register(x);
                            fc_vwpop_register(x, (long)d * 16);
                            d -= 1;
                        } else if (x->op == IR_UNOP) {
                            fc_vbinop_register(x);
                            fc_vwpop_register(x, (long)d * 16);
                        } else if (ir_kind0(x) == IR_CALL || ir_kind0(x) == IR_CALL_PROC_STAGED) {
                            fc_call_register(x);
                            fc_vwpop_register(x, (long)d * 16);
                        } else {
                            fc_vlit_register(x);
                            if (x->op == IR_VAR && d > 0) fc_vwpop_register(x, (long)d * 16);
                            d += 1;
                        }
                    }
                    {
                        if (g_lower.ir.zls_build_dbg3 < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); g_lower.ir.zls_build_dbg3 = (e && *e == '1') ? 1 : 0; }
                        if (g_lower.ir.zls_build_dbg3) fprintf(stderr, "[FCC] REGISTERED pn=%d\n", pn);
                    }
                    fc_vread_register(a, 0);
                }
            }
            ct_drop(_pv.p);
        }
    }
    {
        if (g_lower.ir.zls_build_subj_on < 0) { const char * b = getenv("SCRIP_SUBJ_CELL"); g_lower.ir.zls_build_subj_on = (b && *b == '0') ? 0 : 1; }
        if (g_lower.ir.zls_build_subj_on) for (int vi = 0; vi < g->n; vi++) {
            IR_t * h = g->all[vi];
            if (!(h && h->op == IR_MATCH_BEGIN && h->n_operands > 0 && h->operands[0])) continue;
            {
                if (g_lower.ir.zls_build_dyn_on < 0) { const char * b = getenv("SCRIP_SUBJ_DYN"); g_lower.ir.zls_build_dyn_on = (b && *b == '0') ? 0 : 1; }
                if (!g_lower.ir.zls_build_dyn_on) {
                    int dyn = 0;
                    for (int di = 0; di < g->n && !dyn; di++) { const IR_t * d = g->all[di]; if (d && (d->op == IR_MATCH_DEFER || d->op == IR_MATCH_FENCE1)) dyn = 1; }
                    if (dyn) continue;
                }
            }
            IR_t * r = h->operands[0];
            {
                int nc = 0;
                for (int ci = 0; ci < g->n && nc < 2; ci++) {
                    const IR_t * c = g->all[ci];
                    if (!c || c == h) continue;
                    for (int oi = 0; oi < c->n_operands; oi++) if (c->operands[oi] == r) { nc++; break; }
                }
                if (nc) {
                    if (g_lower.ir.zls_build_dbg4 < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); g_lower.ir.zls_build_dbg4 = (e && *e == '1') ? 1 : 0; }
                    if (g_lower.ir.zls_build_dbg4) fprintf(stderr, "[FCS] refuse SOLE-CONSUMER head=%p subj=%p extra_consumers>=%d\n", (void*)h, (void*)r, nc);
                    continue;
                }
            }
            {
                if (g_lower.ir.zls_build_dbg5 < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); g_lower.ir.zls_build_dbg5 = (e && *e == '1') ? 1 : 0; }
                if (g_lower.ir.zls_build_dbg5)
                    fprintf(stderr, "[FCS] head=%p subj=%p subj_op=%d gamma=%p gamma_is_head=%d sval=%s\n", (void*)h, (void*)r, (int)r->op, (void*)r->γ.node, r->γ.node == h ? 1 : 0,
                    IR_LIT(r).sval ? IR_LIT(r).sval : "(null)");
            }
            {
                int _pb1s_adj = 0;
                {
                    IR_t *_c = r->γ.node;
                    int _hops = 0;
                    while (_c && _c != h && _hops < 128) {
                        if (_c->op == IR_VAR) {
                            IR_t *_d = _c->γ.node;
                            if (_d && _d->op == IR_ASSIGN && IR_LIT(_d).sval && !strncmp(IR_LIT(_d).sval, "PATV$", 5)) { _c = _d->γ.node; _hops += 2; continue; }
                        }
                        if (_c->op == IR_ASSIGN && IR_LIT(_c).sval && !strncmp(IR_LIT(_c).sval, "PATV$", 5)) { _c = _c->γ.node; _hops += 1; continue; }
                        break;
                    }
                    if (_c == h) _pb1s_adj = 1;
                }
                if ((r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL ||
                    (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&' &&
                    ((is_global(IR_LIT(r).sval) && !graph_has_local(g, IR_LIT(r).sval)) || !strcmp(IR_LIT(r).sval, "write") || !strcmp(IR_LIT(r).sval, "writes")))) && _pb1s_adj) {
                    fc_vlit_register(r);
                    fc_subj_register(r);
                    fc_vread_register(h, 0);
                    continue;
                }
            }
            if ((r->op == IR_BINOP || fc_vunop_ok(r)) && r->γ.node == h && !zc_nofc()) {
                cv_t _pv = { 0, 0, 0, 0 };
                int pn = 0;
                int _ts = fc_vtree_scan(g, r, &_pv, &pn, 0);
                const IR_t ** post = (const IR_t **)_pv.p;
                if (_ts && post[pn - 1] == r) {
                    int ok = 1, L = 0, B = 0;
                    for (int i = 0; i + 1 < pn; i++) if (post[i]->γ.node != post[i + 1]) { ok = 0; break; }
                    for (int i = 0; i < pn; i++) { if (post[i]->op == IR_BINOP || post[i]->op == IR_UNOP) B++; else L++; }
                    if (ok) {
                        int d = 0;
                        for (int i = 0; i < pn; i++) {
                            const IR_t * x = post[i];
                            if (x->op == IR_BINOP) {
                                fc_vbinop_register(x);
                                fc_vwpop_register(x, (long)d * 16);
                                d -= 1;
                            } else if (x->op == IR_UNOP) {
                                fc_vbinop_register(x);
                                fc_vwpop_register(x, (long)d * 16);
                            } else {
                                fc_vlit_register(x);
                                if (x->op == IR_VAR && d > 0) fc_vwpop_register(x, (long)d * 16);
                                d += 1;
                            }
                        }
                        fc_vread_register(h, 0);
                    }
                }
                ct_drop(_pv.p);
            }
        }
    }
    zls_graph_t * r = zls_g_find(g);
    if (r && r->first_scope >= 0) return;
    if (!r) {
        cv_reserve(&g_lower.ir.zg_v, (uint32_t)sizeof(zls_graph_t), (uint64_t)g_lower.ir.zg_n + 1, "zg");
        zg[g_lower.ir.zg_n] = (zls_graph_t){ g, (const char *)0, -1, 0, 0, 0, -1, -1, 0, 0, 0, (struct zls_reuse_s *)0, 0, -1, (const IR_t *)0, (const IR_t *)0 };
        r = &zg[g_lower.ir.zg_n];
        g_lower.ir.zg_n++;
        zgh_note(g_lower.ir.zg_n - 1);
    }
    int gi = (int)(r - zg);
    if (!r->name) { char ab[16]; snprintf(ab, sizeof ab, "g%d", gi); r->name = ct_strdup(ab); r->own_name = 1; }
    int root = zls_scope_new(-1, ZSC_FN, r->name);
    r->first_scope = root;
    r->n_scopes = 1;
    int * mfirst = (int *)ct_alloc(sizeof(int) * (size_t)(g_lower.ir.zm_n + 1));
    int * mstart = (int *)ct_alloc(sizeof(int) * (size_t)(g_lower.ir.zm_n + 1));
    int nl = 0;
    for (int mi = 0; mi < g_lower.ir.zm_n; mi++) {
        if (zm[mi].g != g) continue;
        int sc = zls_scope_new(root, ZSC_GROUP, zm[mi].name);
        mfirst[nl] = sc;
        mstart[nl] = zm[mi].start_n;
        nl++;
        r->n_scopes++;
    }
    char rb_s[8192];
    char * rb = (g->n <= (int)sizeof rb_s) ? rb_s : (char *)ct_alloc((size_t)(g->n > 0 ? g->n : 1));
    memset(rb, 0, (size_t)(g->n > 0 ? g->n : 1));
    {
        int hn = 4;
        while (hn < g->n * 2 + 4) hn <<= 1;
        IR_t ** hk = (IR_t **)ct_zalloc((size_t)hn, sizeof(IR_t *));
        int * hv = (int *)ct_alloc((size_t)hn * sizeof(int));
        int * wl = (int *)ct_alloc((size_t)(g->n > 0 ? g->n : 1) * sizeof(int));
        int wn = 0;
        for (int i = 0; i < g->n; i++) {
            IR_t * p = g->all[i];
            if (!p) continue;
            unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1);
            while (hk[h]) h = (h + 1) & (unsigned long)(hn - 1);
            hk[h] = p;
            hv[h] = i;
        }
        if (g->entry) {
            IR_t * p = g->entry;
            unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1);
            while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1);
            if (hk[h]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; }
        } else for (int i = 0; i < g->n; i++) rb[i] = 1;
        if (g->n_alts > 1 && g->alt_entry) for (int _ak = 1; _ak < g->n_alts; _ak++) {
            IR_t * p = g->alt_entry[_ak];
            if (!p) continue;
            unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1);
            while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1);
            if (hk[h] && !rb[hv[h]]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; }
        }
        int dyn = 0;
        for (int i = 0; i < g->n && !dyn; i++)
            if (g->all[i] &&
            (g->all[i]->op == IR_GOTO_DEFERRED || g->all[i]->op == IR_GATE ||
            ((ir_kind0(g->all[i]) == IR_CALL || ir_kind0(g->all[i]) == IR_CALL_BUILTIN || ir_kind0(g->all[i]) == IR_CALL_SNOBOL4) && IR_LIT(g->all[i]).sval &&
            strcmp(IR_LIT(g->all[i]).sval, "CODE") == 0))) dyn = 1;
        if (dyn) { for (int i = 0; i < g->n; i++) if (g->all[i]) rb[i] = 1; wn = 0; }
        for (int i = 0; i < g->n; i++) if (g->all[i] && !rb[i] && zls_is_wiring(ir_kind0(g->all[i]))) { rb[i] = 1; wl[wn++] = i; }
        for (int mi2 = 0; mi2 < nl; mi2++) {
            int sp0 = mstart[mi2];
            int sp1 = (mi2 + 1 < nl) ? mstart[mi2 + 1] : g->n;
            for (int i = sp0; i >= 0 && i < sp1 && i < g->n; i++) if (g->all[i] && !rb[i]) { rb[i] = 1; wl[wn++] = i; }
        }
        while (wn > 0) {
            IR_t * c = g->all[wl[--wn]];
            if (!c) continue;
            for (int j = -2; j < c->n_operands; j++) {
                IR_t * p = (j == -2) ? c->γ.node : (j == -1) ? c->ω.node : c->operands[j];
                if (!p) continue;
                unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1);
                while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1);
                if (hk[h] && !rb[hv[h]]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; }
            }
        }
        ct_drop(hk);
        ct_drop(hv);
        ct_drop(wl);
    }
    int s0 = (g->nparams > 0 || g->resumable_callable) ? 1 : 0;
    for (int i = 0; !s0 && i < g->n; i++) if (g->all[i] && rb[i] && (g->all[i]->op == IR_RETURN || g->all[i]->op == IR_SUSPEND)) s0 = 1;
    int blk = zls_g_block_args(g);
    int base = s0 ? 16 + ((g->nparams > 0 && !blk) ? g->nparams * 16 : 0) : 0;
    if (s0)
        zls_field(root, 0, 16, ZK_DESCR, 0,
        "return value (IR_RETURN stores the graph result DESCR at frame +0 and the graph gamma loads it into rdi:rsi; zeroed by the prologue, a valid null DESCR until then)", (const IR_t *)0);
    int k = 0;
    int nix_cap = 16;
    while (nix_cap < 2 * (g->nparams + g->nlocals + 2 * g->n + 32)) nix_cap *= 2;
    const char ** nix = (const char **)ct_alloc(sizeof(const char *) * (size_t)nix_cap);
    memset(nix, 0, sizeof(const char *) * (size_t)nix_cap);
    r->first_vslot = g_lower.ir.zv_n;
    for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) {
        cv_reserve(&g_lower.ir.zv_v, (uint32_t)sizeof(zls_vslot_t), (uint64_t)g_lower.ir.zv_n + 1, "zv");
        *zls_nix_slot(nix, nix_cap, g->pnames[i]) = g->pnames[i];
        zv[g_lower.ir.zv_n++] = (zls_vslot_t){ g->pnames[i], blk ? -1 : 16 + i * 16 };
        r->n_vslots++;
        if (!blk) zls_field(root, 16 + i * 16, 16, ZK_DESCR, 0, "param", (const IR_t *)0);
    }
    int hf = g->home_frame ? 1 : 0;
    r->hf = hf;
    if (hf) k = zls_frame_words(g, r, root, base, k, rb, nix, nix_cap, 1);
    int cur = 0;
    {
        if (g_lower.ir.zls_build_eon < 0) { const char * e = getenv("SCRIP_SLOT_ELIDE"); g_lower.ir.zls_build_eon = (e && *e == '0') ? 0 : 1; }
        char lv_sbuf[1024];
        char * lv = (g->n <= (int)sizeof lv_sbuf) ? lv_sbuf : (char *)ct_alloc((size_t)g->n);
        memset(lv, 0, (size_t)g->n);
        char uo[g->n > 0 ? g->n : 1];
        memset(uo, 0, sizeof uo);
        if (g_lower.ir.zls_build_eon) zls_mark_value_refs(g, lv, uo);
        int scratch_off = -1;
        zls_reuse_t * rec = (zls_reuse_t *)ct_alloc(sizeof(zls_reuse_t) * (size_t)g->n);
        int nloop = 0;
        zls_reuse_plan(g, rec, (int *)0, &nloop);
        int al_n = g->n_alts > 1 ? g->n_alts : 1, al[al_n + 1];
        int ov = ir_clause_bounds(g, al, al_n) == g->n_alts && g->alt_zhi && g->n_alts > 1 && nl == 0 && al[0] <= al[g->n_alts] && al[g->n_alts] <= g->n && !zls_ov_shares(g);
        int pool_free[64], pool_n = 0, pool_base = -1, dl_base = -1, dl_w = 0, rs_scratch = -1, ov_in = 0;
        for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && rb[i] && zls_reuse_detleaf(nd) && (nl == 0 || i < mstart[0]) && nd->n_operands > dl_w) dl_w = nd->n_operands; }
        {
            int * order = (int *)ct_alloc(sizeof(int) * (size_t)g->n);
            int on = 0;
            for (int i = 0; i < g->n; i++) {
                IR_t * nd = g->all[i];
                if (!nd || !rb[i]) continue;
                if (nl > 0 && i >= mstart[0]) continue;
                if (g_lower.ir.zls_build_eon && lv[i] && rec[i].cls == ZR_CANDIDATE && nd->op != IR_LINE_MARK && !zls_fc_cell(nd) && !fc_vlit_active(nd) && fc_vread_fp(nd) < 0 &&
                    !fc_vbinop_active(nd) && !fc_vdj_active(nd) && !(hf && zls_hf_resident(g, nd))) order[on++] = i;
            }
            {
                zls_pi_t * ow = (zls_pi_t *)ct_alloc(sizeof(zls_pi_t) * (size_t)(on > 0 ? on : 1));
                for (int a = 0; a < on; a++) { ow[a].p = (const IR_t *)(intptr_t)rec[order[a]].w; ow[a].i = order[a]; }
                qsort(ow, (size_t)on, sizeof(zls_pi_t), zls_wi_cmp);
                for (int a = 0; a < on; a++) order[a] = ow[a].i;
                ct_drop(ow);
            }
            zls_ds_t ds;
            zls_ds_build(&ds, g, rb, dl_w);
            for (int a = 0; a < on; a++) {
                int i = order[a];
                int j = zls_direct_slot(g, rec, rb, nl, mstart, i, dl_w, &ds);
                if (j >= 0) { rec[i].direct = j; if (rec[i].last > ds.maxlast[j]) ds.maxlast[j] = rec[i].last; continue; }
                int p = -1;
                for (int q = 0; q < pool_n && p < 0; q++) if (pool_free[q] < rec[i].w) p = q;
                if (p < 0 && pool_n < 64) { p = pool_n++; }
                if (p < 0) continue;
                pool_free[p] = rec[i].last;
                rec[i].pooled = p;
            }
            zls_ds_drop(&ds);
            ct_drop(order);
            if (hf) {
                for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && zls_hf_resident(g, g->all[i]) && ov && i >= al[0] && i < al[g->n_alts]) ov_in = 1;
                for (int i = 0, c2 = 0; i < g->n; i++) {
                    IR_t * nd = g->all[i];
                    if (!nd) continue;
                    while (c2 < nl && i >= mstart[c2]) c2++;
                    if (!rb[i] || !zls_hf_resident(g, nd) || (ov && i >= al[0] && i < al[g->n_alts])) continue;
                    k += zls_grant_at(g, nd, i, c2 > 0 ? mfirst[c2 - 1] : root, root, rec, dl_base, lv, uo, &rs_scratch, base + k * 16);
                }
                if (ov_in) k = zls_overlay(g, base, k, root, rb, rec, dl_base, lv, uo, &rs_scratch, 1);
                for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && !zls_hf_resident(g, g->all[i]) && zls_redisp_node(g->all[i])) {
                    IR_t * nd = g->all[i];
                    for (int j = 0; j < nd->n_operands; j++) zls_field(root, base + (k + j) * 16, 16, ZK_DESCR, 0, "redisp.argv (the dispatcher copies the arguments here, in front of the record)", nd)
                        ;
                    zls_redisp_fields(root, base + (k + nd->n_operands) * 16, nd);
                    zls_redisp_mark(nd, base + (k + nd->n_operands) * 16);
                    k += nd->n_operands + RK_REDISP_REC_SLOTS;
                }
                r->fregion = base + k * 16;
                r->kbase = zls_hf_key_base(g, r->fregion, base);
                k = (r->kbase - base) / 16;
                for (int j = r->first_vslot; j >= 0 && j < r->first_vslot + r->n_vslots && j < g_lower.ir.zv_n; j++) if (zv[j].off == -2) {
                    zv[j].off = base + k * 16;
                    zls_field(root, zv[j].off, 16, ZK_DESCR, 0, "local (a pure key: the planner carves it on the spine at its first use)", (const IR_t *)0);
                    k++;
                }
            }
            if (pool_n > 0) {
                pool_base = base + k * 16;
                for (int q = 0; q < pool_n; q++)
                    zls_field(root, pool_base + q * 16, 16, ZK_DESCR, 0, "result (pooled: straight temps share it by interval colouring on the gamma spine)", (const IR_t *)0);
                k += pool_n;
            }
            if (dl_w > 0 && dl_base < 0) { dl_base = zls_dl_place(root, base, k, dl_w); k += dl_w; }
            r->scratch_pool = -1;
            r->scratch_hit_w = (const IR_t *)0;
            r->scratch_hit_t = (const IR_t *)0;
            if (g_lower.ir.zls_build_eon && nloop == 0 && pool_n > 0) {
                int pc[pool_n], po[pool_n + 1];
                for (int q = 0; q < pool_n; q++) pc[q] = 0;
                for (int t = 0; t < g->n; t++) if (rec[t].pooled >= 0 && rec[t].pooled < pool_n && rec[t].direct < 0) pc[rec[t].pooled]++;
                po[0] = 0;
                for (int q = 0; q < pool_n; q++) po[q + 1] = po[q] + pc[q];
                zls_pi_t * pv = (zls_pi_t *)ct_alloc(sizeof(zls_pi_t) * (size_t)(po[pool_n] > 0 ? po[pool_n] : 1));
                for (int q = 0; q < pool_n; q++) pc[q] = po[q];
                for (int t = 0; t < g->n; t++) if (rec[t].pooled >= 0 && rec[t].pooled < pool_n && rec[t].direct < 0) {
                    pv[pc[rec[t].pooled]].p = (const IR_t *)(intptr_t)(2 * rec[t].w + 1);
                    pv[pc[rec[t].pooled]].i = t;
                    pc[rec[t].pooled]++;
                }
                for (int q = 0; q < pool_n; q++) qsort(pv + po[q], (size_t)(po[q + 1] - po[q]), sizeof(zls_pi_t), zls_wi_cmp);
                for (int q = 0; q < pool_n && r->scratch_pool < 0; q++) {
                    int hit = 0;
                    for (int i = 0; i < g->n && !hit; i++) {
                        const IR_t * d = g->all[i];
                        if (!d || !rb[i] || lv[i] || zls_is_wiring(ir_kind0(d))) continue;
                        if (!zls_reuse_detleaf(d) && !zls_elide_ok(ir_kind0(d))) continue;
                        if (zls_writes_no_result(ir_kind0(d))) continue;
                        if (rec[i].w < 0 || (nl > 0 && i >= mstart[0]) || (!zls_reuse_detleaf(d) && !zls_reuse_straight(ir_kind0(d)))) { hit = 1; r->scratch_hit_w = d; break; }
                        int p = 2 * rec[i].w + 1, lo = po[q], hi = po[q + 1], t = -1;
                        while (lo < hi) { int mid = lo + (hi - lo) / 2; if ((intptr_t)pv[mid].p <= p) lo = mid + 1; else hi = mid; }
                        if (lo > po[q]) t = pv[lo - 1].i;
                        if (t >= 0 && p <= 2 * rec[t].last) { hit = 1; r->scratch_hit_w = d; r->scratch_hit_t = g->all[t]; }
                    }
                    if (!hit) r->scratch_pool = q;
                }
                ct_drop(pv);
            }
            if (r->scratch_pool >= 0) scratch_off = pool_base + r->scratch_pool * 16;
            for (int i = 0; i < g->n; i++) { if (rec[i].direct >= 0) rec[i].pooled = dl_base + rec[i].direct * 16; else if (rec[i].pooled >= 0) rec[i].pooled = pool_base + rec[i].pooled * 16; }
        }
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd) continue;
            while (cur < nl && i >= mstart[cur]) cur++;
            if (!rb[i] || (hf && zls_hf_resident(g, nd))) continue;
            if (ov && i >= al[0] && i < al[g->n_alts]) continue;
            int sc = (cur > 0) ? mfirst[cur - 1] : root;
            k += zls_grant_at(g, nd, i, sc, root, rec, dl_base, lv, uo, &scratch_off, base + k * 16);
        }
        if (ov) {
            k = zls_overlay(g, base, k, root, rb, rec, dl_base, lv, uo, &scratch_off, hf ? 2 : 0);
            if (hf && !ov_in) for (int c = 0; c < g->n_alts; c++) g->alt_zhi[c] = 0;
        } else if (g->alt_zhi) for (int c = 0; c < g->n_alts; c++) g->alt_zhi[c] = 0;
        {
            int nr = 0;
            for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && !zls_is_wiring(ir_kind0(g->all[i]))) nr++;
            if (r->reuse) ct_drop(r->reuse);
            r->reuse = (struct zls_reuse_s *)ct_alloc(sizeof(zls_reuse_t) * (size_t)(nr > 0 ? nr : 1));
            r->n_reuse = 0;
            for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && !zls_is_wiring(ir_kind0(g->all[i]))) r->reuse[r->n_reuse++] = rec[i];
        }
        ct_drop(rec);
        if (lv != lv_sbuf) ct_drop(lv);
    }
    if (!hf) k = zls_frame_words(g, r, root, base, k, rb, nix, nix_cap, 0);
    ct_drop(nix);
    r->nslots = k;
    r->region = base + k * 16;
    if (blk) { int ab = zls_g_block_base(g), j = r->first_vslot; for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) zv[j++].off = ab + i * 16; }
    for (int f = 0; f < g_lower.ir.zf_n; f++) {
        int sid = zf[f].scope_id;
        if (sid < root || sid >= g_lower.ir.zs_n) continue;
        if (zf[f].off < zs[sid].lo_off) zs[sid].lo_off = zf[f].off;
        if (zf[f].off + zf[f].size > zs[sid].hi_off) zs[sid].hi_off = zf[f].off + zf[f].size;
        if (zs[sid].first_field < 0) zs[sid].first_field = f;
        zs[sid].n_fields++;
    }
    zx_settle();
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || !rb[i] || nd->op != IR_MATCH_ARBNO || IR_LIT(nd).ival != 1 || nd->n_operands < 3) continue;
        int i0 = -1, i1 = -1;
        for (int j = 0; j < g->n; j++) { if (g->all[j] == nd->operands[1]) i0 = j; if (g->all[j] == nd->operands[2]) i1 = j; }
        if (i0 < 0 || i1 < 0) { fprintf(stderr, "zls: arbno2 geometry — body bracket operands not found in g->all\n"); abort(); }
        if (i0 > i1) { int t = i0; i0 = i1; i1 = t; }
        int mn = 0x7fffffff, mx = 0;
        int azq[i1 - i0 + 1];
        int anzq = 0;
        for (int j = i0; j <= i1; j++) {
            const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0;
            if (!e) continue;
            if (e->off < mn) mn = e->off;
            if (g->all[j]->op == IR_MATCH_ASSIGN_SAVE) {
                int co = -1;
                for (int f = 0; f < g_lower.ir.zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].kind == ZK_PTR_GC) { co = zf[f].off; break; }
                if (co >= 0 && anzq >= 0) azq[anzq++] = co;
                else anzq = -1;
            }
            for (int f = 0; f < g_lower.ir.zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].off + zf[f].size > mx) mx = zf[f].off + zf[f].size;
        }
        cv_reserve(&g_lower.ir.za_v, (uint32_t)sizeof(zls_ageom_t), (uint64_t)g_lower.ir.za_n + 1, "za");
        if (mn == 0x7fffffff) CV_AT(g_lower.ir.za_v, zls_ageom_t, g_lower.ir.za_n++) = (zls_ageom_t){ nd, 16, 0, (int *)0, 0 };
        else {
            zls_ageom_t a;
            a.nd = nd;
            a.min_off = mn;
            a.span = mx - mn;
            a.nzq = anzq;
            a.zq = anzq > 0 ? (int *)ct_alloc((size_t)anzq * sizeof(int)) : (int *)0;
            for (int q = 0; q < anzq; q++) a.zq[q] = azq[q];
            CV_AT(g_lower.ir.za_v, zls_ageom_t, g_lower.ir.za_n++) = a;
        }
    }
    if (rb != rb_s) ct_drop(rb);
    ct_drop(mfirst);
    ct_drop(mstart);
    zls_fct_finalize(g, 0);
    zls_slot_census(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_fct_finalize(IR_graph_t * g, int late) {
    if (!g) return;
    {
        extern int fc_geom(const IR_t *, long *);
        extern void fc_cond_register(const IR_t *, int);
        for (int c = 0; c < g_lower.ir.fct_n; c++) {
            if (g_lower.ir.fct[c].fin) continue;
            { int in = 0; for (int j = 0; j < g->n; j++) if (g->all[j] == g_lower.ir.fct[c].arbno) { in = 1; break; } if (!in) continue; }
            {
                int hd = 0;
                for (int j = g_lower.ir.fct[c].i0; j < g_lower.ir.fct[c].r1 && j < g->n && !hd; j++) { IR_t * x = g->all[j]; if (x && (x->op == IR_MATCH_DEFER)) hd = 1; }
                if (hd && !late) continue;
            }
            int b0 = g_lower.ir.fct[c].b0, b1 = g_lower.ir.fct[c].b1, i0 = g_lower.ir.fct[c].i0, ia = g_lower.ir.fct[c].ia, r1 = g_lower.ir.fct[c].r1;
            long k1 = 0;
            int bmn = 0x7fffffff, bmx = 0, rmn = 0x7fffffff, rmx = 0;
            for (int j = b0; j <= b1 && j < g->n; j++) {
                const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0;
                if (!e) continue;
                int _bfmx = 0;
                for (int f = 0; f < g_lower.ir.zf_n; f++) if (zf[f].nd == g->all[j]) { if (zf[f].off + zf[f].size > _bfmx) _bfmx = zf[f].off + zf[f].size; if (zf[f].off < bmn) bmn = zf[f].off; }
                if (_bfmx > bmx) bmx = _bfmx;
            }
            for (int j = b1 + 1; j < r1 && j < g->n; j++) {
                const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0;
                if (!e) continue;
                int _rfmx = 0;
                for (int f = 0; f < g_lower.ir.zf_n; f++) if (zf[f].nd == g->all[j]) { if (zf[f].off + zf[f].size > _rfmx) _rfmx = zf[f].off + zf[f].size; if (zf[f].off < rmn) rmn = zf[f].off; }
                if (_rfmx > rmx) rmx = _rfmx;
            }
            int span = (bmn == 0x7fffffff) ? 0 : bmx - bmn;
            int rspan = (rmn == 0x7fffffff) ? 0 : rmx - rmn;
            {
                int _dok = 1, _dfr = 0;
                for (int j = i0; j < r1 && j < g->n && _dok; j++) { IR_t * x = g->all[j]; if (x && (x->op == IR_MATCH_DEFER)) { _dfr = 1; if (fct_defer_susp(x) <= 0) _dok = 0; } }
                g_lower.ir.fct[c].dfr = _dfr;
                if (!_dok) {
                    if (getenv("SCRIP_TAIL_DIAG")) fprintf(stderr, "[TAIL-DIAG] finalize refuse: defer target unregistered/non-uniform\n");
                    g_lower.ir.fct[c].head = 0;
                    g_lower.ir.fct[c].arbno = 0;
                    continue;
                }
            }
            g_lower.ir.fct_pricing = 1;
            int fpl = fct_rsp_range(g, i0, ia);
            int fpb = fct_fp_range(g, b0, b1 + 1);
            int fpr = fct_fp_range(g, b1 + 1, r1);
            int fpr_rsp = fct_rsp_range(g, b1 + 1, r1);
            (void)k1;
            g_lower.ir.fct_pricing = 0;
            g_lower.ir.fct[c].fpl = fpl;
            g_lower.ir.fct[c].fpb = fpb;
            g_lower.ir.fct[c].fpr = fpr;
            g_lower.ir.fct[c].fpr_rsp = fpr_rsp;
            g_lower.ir.fct[c].span = span;
            g_lower.ir.fct[c].rspan = rspan;
            g_lower.ir.fct[c].opsb = (span + rspan + 32 + 16 * g_lower.ir.fct[c].nw + 15) & ~15;
            g_lower.ir.fct[c].fin = 1;
            for (int w = 0; w < g_lower.ir.fct[c].nw; w++) fc_cond_register(CV_AT(g_lower.ir.fct[c].wcd, const IR_t *, w), fpb + span + rspan + 32 + 16 * w);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_arbno_geom(const IR_t * nd, int * min_off, int * span) {
    for (int i = 0; i < g_lower.ir.za_n; i++) if (CV_AT(g_lower.ir.za_v, zls_ageom_t, i).nd == nd) {
        if (min_off) *min_off = CV_AT(g_lower.ir.za_v, zls_ageom_t, i).min_off;
        if (span) *span = CV_AT(g_lower.ir.za_v, zls_ageom_t, i).span;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_arbno_zq(const IR_t * nd, const int ** zq) {
    for (int i = 0; i < g_lower.ir.za_n; i++) if (CV_AT(g_lower.ir.za_v, zls_ageom_t, i).nd == nd) {
        *zq = CV_AT(g_lower.ir.za_v, zls_ageom_t, i).zq;
        return CV_AT(g_lower.ir.za_v, zls_ageom_t, i).nzq;
    }
    *zq = (const int *)0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zc_nofc(void) { if (g_lower.ir.zc_nofc_v < 0) { const char * e = getenv("SCRIP_NOFC"); g_lower.ir.zc_nofc_v = (e && e[0] == '0') ? 0 : 1; } return g_lower.ir.zc_nofc_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_geom(const IR_t * nd, long * k) {
    if (!nd || !fc_cells_on()) return 0;
    {
        if (g_lower.ir.fc_geom_ac < 0) { const char * e = getenv("SCRIP_ALT_CAP"); g_lower.ir.fc_geom_ac = (e && *e == '0') ? 0 : 1; }
        if (fc_arm_member(nd) && !(g_lower.ir.fc_geom_ac && nd->op == IR_MATCH_ASSIGN_SAVE && fc_save_active(nd))) return 0;
    }
    if (nd->op == IR_MATCH_ASSIGN_SAVE && fc_save_active(nd)) { if (k) *k = 16; return 1; }
    if ((nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME || nd->op == IR_VAR) && fc_vlit_active(nd) &&
        (!zc_nofc() || fc_subj_member(nd))) {
        if (k) *k = 16;
        return 1;
    }
    if (nd->op == IR_DISJUNCTION && fc_vdj_active(nd) && IR_LIT(nd).ival > 0 && nd->n_operands > 2 * (int)IR_LIT(nd).ival) { if (k) *k = 32; return 1; }
    if (nd->op == IR_MATCH_ARB) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_SPAN) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_TAB) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_RTAB) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAK) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAKX) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BAL) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_REM) { if (k) *k = 16; return 1; }
    if (nd->op == IR_SCAN_TAB) { if (k) *k = 16; return 1; }
    if (nd->op == IR_SCAN_MOVE) { if (k) *k = 16; return 1; }
    if (nd->op == IR_SCAN_MATCH) { if (k) *k = 16; return 1; }
    return 0;
}
typedef struct { const IR_t * nd; uint32_t gen; uint32_t fl; int vr; int pe; int cf; int hf; int alt; long vw; const IR_t * cs; int hw; int lw; int ro; } fcn_t;
#define FCN_ARM 0x0001u
#define FCN_VDJ 0x0002u
#define FCN_VLIT 0x0004u
#define FCN_SUBJ 0x0008u
#define FCN_VBIN 0x0010u
#define FCN_CALL 0x0020u
#define FCN_SAVE 0x0040u
#define FCN_VR 0x0080u
#define FCN_VW 0x0100u
#define FCN_PE 0x0200u
#define FCN_CC 0x0400u
#define FCN_HD 0x0800u
#define FCN_ALT 0x1000u
#define FCN_HRET 0x2000u
#define FCN_HUNRES 0x4000u
#define FCN_HOME 0x8000u
#define FCN_LHOME 0x10000u
#define FCN_RES 0x20000u
#define FCN_RO 0x40000u
#define FCN_BSK 0x80000u
#define FCN_BSF 0x100000u
#define FCN_BOK 0x200000u
#define FCN_BOV 0x400000u
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fc_reg_hw(const char * tbl, int n) {
    if (g_lower.ir.fc_reg_hw_e == (const char *) 1) g_lower.ir.fc_reg_hw_e = getenv("SCRIP_FC_REG_HIGHWATER");
    if (g_lower.ir.fc_reg_hw_e && *g_lower.ir.fc_reg_hw_e != '0') fprintf(stderr, "FC-REG-HW %-6s %d\n", tbl, n);
}
static fcn_t * fcn_probe(cv_t * v, const IR_t * nd) {
    uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 0xff51afd7ed558ccdull) & (v->len - 1);
    for (;;) { fcn_t * t = &CV_AT(*v, fcn_t, h); if (t->gen != g_lower.ir.fcn_gen || t->nd == nd) return t; h = (h + 1) & (v->len - 1); }
}
static const fcn_t * fcn_find(const IR_t * nd, uint32_t fl) {
    if (!nd || !g_lower.ir.fcn.len) return (const fcn_t *)0;
    { const fcn_t * t = fcn_probe(&g_lower.ir.fcn, nd); return (t->gen == g_lower.ir.fcn_gen && (t->fl & fl)) ? t : (const fcn_t *)0; }
}
static fcn_t * fcn_get(const IR_t * nd) {
    if ((uint64_t)(g_lower.ir.fcn_n + 1) * 2 > g_lower.ir.fcn.len) {
        uint32_t nc = g_lower.ir.fcn.len ? g_lower.ir.fcn.len * 2 : 256;
        cv_t o = g_lower.ir.fcn, n = { 0, 0, 0, 0 };
        n.p = ct_zalloc(nc, sizeof(fcn_t));
        n.len = nc;
        n.cap = nc;
        n.esz = (uint32_t)sizeof(fcn_t);
        g_lower.ir.fcn = n;
        for (uint32_t i = 0; i < o.len; i++) { fcn_t q = CV_AT(o, fcn_t, i); if (q.gen == g_lower.ir.fcn_gen) *fcn_probe(&g_lower.ir.fcn, q.nd) = q; }
    }
    fcn_t * t = fcn_probe(&g_lower.ir.fcn, nd);
    if (t->gen != g_lower.ir.fcn_gen) { memset(t, 0, sizeof *t); t->nd = nd; t->gen = g_lower.ir.fcn_gen; g_lower.ir.fcn_n++; fc_reg_hw("node", (int)g_lower.ir.fcn_n); }
    return t;
}
static void fcn_flag(const IR_t * nd, uint32_t fl) { if (nd) fcn_get(nd)->fl |= fl; }
static const int * fca_arms(const IR_t * nd) { const fcn_t * t = fcn_find(nd, FCN_ALT); return t ? (const int *)g_lower.ir.fca.p + t->alt : (const int *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_arm_member_register(const IR_t * nd) { fcn_flag(nd, FCN_ARM); }
int fc_arm_member(const IR_t * nd) { return fcn_find(nd, FCN_ARM) != (const fcn_t *)0; }
int fc_leaf_cell(const IR_t * nd, long * k) { if (fc_geom(nd, k)) return 1; if (!nd || zdp_seam_tier(nd) != 0 || !zdp_scratch_cell(nd) || !fc_arm_member(nd)) return 0; if (k) *k = 16; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_alt_register(const IR_t * nd, int n, const int * fp, const int * ab, const int * ae) {
    if (!nd || n <= 0 || n > 10) return;
    fcn_t * t = fcn_get(nd);
    if (t->fl & FCN_ALT) return;
    if (g_lower.ir.fca.len + 1 + 3 * (uint32_t)n > g_lower.ir.fca.cap) {
        uint32_t nc = g_lower.ir.fca.cap ? g_lower.ir.fca.cap * 2 : 256;
        while (nc < g_lower.ir.fca.len + 1 + 3 * (uint32_t)n) nc *= 2;
        int * q = (int *)ct_alloc((size_t)nc * sizeof(int));
        if (g_lower.ir.fca.len) memcpy(q, g_lower.ir.fca.p, (size_t)g_lower.ir.fca.len * sizeof(int));
        g_lower.ir.fca.p = q;
        g_lower.ir.fca.cap = nc;
        g_lower.ir.fca.esz = (uint32_t)sizeof(int);
    }
    int * a = (int *)g_lower.ir.fca.p + g_lower.ir.fca.len;
    a[0] = n;
    for (int i = 0; i < n; i++) { a[1 + i] = fp[i]; a[1 + n + i] = ab ? ab[i] : -1; a[1 + 2 * n + i] = ae ? ae[i] : -1; }
    t->alt = (int)g_lower.ir.fca.len;
    t->fl |= FCN_ALT;
    g_lower.ir.fca.len += 1 + 3 * (uint32_t)n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_extent(const IR_t * nd, int * b, int * e) {
    const int * a = fca_arms(nd);
    if (!a) return 0;
    { int n = a[0]; if (a[1 + n] < 0 || a[1 + 2 * n + n - 1] < 0) return 0; if (b) *b = a[1 + n]; if (e) *e = a[1 + 2 * n + n - 1]; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_fpmax(const IR_t * nd) { const int * a = fca_arms(nd); if (!a) return -1; { int m = 0; for (int j = 0; j < a[0]; j++) if (a[1 + j] > m) m = a[1 + j]; return m; } }
int fc_seq_active(const IR_t * nd) { (void)nd; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vdj_register(const IR_t * nd) { fcn_flag(nd, FCN_VDJ); }
int fc_vdj_active(const IR_t * nd) { return fcn_find(nd, FCN_VDJ) != (const fcn_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vlit_register(const IR_t * nd) { fcn_flag(nd, FCN_VLIT); }
void fc_subj_register(const IR_t * nd) { fcn_flag(nd, FCN_SUBJ); }
int fc_subj_member(const IR_t * nd) { return fcn_find(nd, FCN_SUBJ) != (const fcn_t *)0; }
int fc_vlit_active(const IR_t * nd) {
    if (!fc_cells_on()) return 0;
    if (!nd || !(nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME || nd->op == IR_VAR)) return 0;
    return fcn_find(nd, FCN_VLIT) != (const fcn_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vread_register(const IR_t * nd, int fp) { if (!nd || fp < 0) return; { fcn_t * t = fcn_get(nd); if (!(t->fl & FCN_VR)) { t->vr = fp; t->fl |= FCN_VR; } } }
int fc_vread_fp(const IR_t * nd) { const fcn_t * t = fcn_find(nd, FCN_VR); return t ? t->vr : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_call_register(const IR_t * nd) { fcn_flag(nd, FCN_CALL); }
int fc_call_active(const IR_t * nd) { if (!nd || !fc_cells_on()) return 0; return fcn_find(nd, FCN_CALL) != (const fcn_t *)0; }
void fc_vbinop_register(const IR_t * nd) { fcn_flag(nd, FCN_VBIN); }
int fc_vbinop_active(const IR_t * nd) { if (!nd || (nd->op != IR_BINOP && nd->op != IR_UNOP)) return 0; return fcn_find(nd, FCN_VBIN) != (const fcn_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vwpop_register(const IR_t * nd, long w) { if (!nd || w <= 0) return; { fcn_t * t = fcn_get(nd); if (!(t->fl & FCN_VW)) { t->vw = w; t->fl |= FCN_VW; } } }
long fc_vwpop(const IR_t * nd) { if (!fc_cells_on()) return 0; { const fcn_t * t = fcn_find(nd, FCN_VW); return t ? t->vw : 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_save_register(const IR_t * nd) { fcn_flag(nd, FCN_SAVE); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_save_active(const IR_t * nd) { if (!nd || nd->op != IR_MATCH_ASSIGN_SAVE || !fc_cells_on()) return 0; return fcn_find(nd, FCN_SAVE) != (const fcn_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_pair_extent_register(const IR_t * nd, int e) { if (!nd || e <= 0) return; { fcn_t * t = fcn_get(nd); if (!(t->fl & FCN_PE)) { t->pe = e; t->fl |= FCN_PE; } } }
int fc_pair_extent(const IR_t * nd) { const fcn_t * t = fcn_find(nd, FCN_PE); return t ? t->pe : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_cond_register_with_save(const IR_t * nd, const IR_t * save, int fp_inner) {
    if (!nd || fp_inner < 0) return;
    { fcn_t * t = fcn_get(nd); if (!(t->fl & FCN_CC)) { t->cs = save; t->cf = fp_inner; t->fl |= FCN_CC; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_cond_register(const IR_t * nd, int fp_inner) { fc_cond_register_with_save(nd, (const IR_t *)0, fp_inner); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_cond_fp(const IR_t * nd) { if (!fc_cells_on()) return -1; { const fcn_t * t = fcn_find(nd, FCN_CC); if (!t) return -1; if (t->cs && fc_arm_member(t->cs)) return -1; return t->cf; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_head_register(const IR_t * nd, int fp) { if (!nd || fp < 0) return; { fcn_t * t = fcn_get(nd); if (!(t->fl & FCN_HD)) { t->hf = fp; t->fl |= FCN_HD; } } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_head_fp(const IR_t * nd) { if (!fc_cells_on()) return -1; { const fcn_t * t = fcn_find(nd, FCN_HD); return t ? t->hf : -1; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_home_holds_retain(const IR_graph_t * g) {
    for (int i = 0; g && i < g->n; i++) { int o = g->all[i] ? (int)ir_kind0(g->all[i]) : 0; if (o == IR_SUSPEND || o == IR_CREATE || o == IR_ACTIVATE || o == IR_CORET || o == IR_COFAIL) return 1; }
    return 0;
}
static uint32_t zls_home_hash(const char * s) { uint32_t h = 2166136261u; for (; *s; s++) h = (h ^ (unsigned char)*s) * 16777619u; return h; }
static int zls_home_local_retain(const IR_graph_t * g) {
    int ns = 0;
    for (int i = 0; g && i < g->n; i++) {
        const IR_t * x = g->all[i];
        int o = x ? (int)ir_kind0(x) : 0;
        if (o == IR_SUSPEND || o == IR_CREATE || o == IR_ACTIVATE || o == IR_CORET || o == IR_COFAIL || o == IR_CALL_VALUE || o == IR_CALL_GEN || o == IR_PROC_GEN) return 1;
        if (x && x->op == IR_SUCCEED) ns++;
    }
    return ns > 1;
}
static int zls_home_callee(const char * const * nm, const int * gx, int cap, const char * s) {
    uint32_t h = zls_home_hash(s) & (uint32_t)(cap - 1);
    while (nm[h] && strcmp(nm[h], s)) h = (h + 1) & (uint32_t)(cap - 1);
    return nm[h] ? gx[h] : -1;
}
void zls_home_callees_mark(IR_graph_t ** gs, int n, const char ** pn, const int * px, int np) {
    int cap = 16;
    while (cap < 2 * np + 2) cap <<= 1;
    const char * nm[cap];
    int gx[cap];
    unsigned char rt[n > 0 ? n : 1], hs[n > 0 ? n : 1];
    memset(nm, 0, sizeof nm);
    for (int k = 0; k < np; k++) {
        if (!pn[k] || px[k] < 0 || px[k] >= n || !gs[px[k]]) continue;
        uint32_t h = zls_home_hash(pn[k]) & (uint32_t)(cap - 1);
        while (nm[h] && strcmp(nm[h], pn[k])) h = (h + 1) & (uint32_t)(cap - 1);
        nm[h] = pn[k];
        gx[h] = px[k];
    }
    for (int k = 0; k < n; k++) { rt[k] = gs[k] ? (unsigned char)zls_home_local_retain(gs[k]) : 0; hs[k] = gs[k] ? (unsigned char)zls_home_holds_retain(gs[k]) : 0; }
    for (int changed = 1; changed;) {
        changed = 0;
        for (int k = 0; k < n; k++) for (int i = 0; gs[k] && !rt[k] && i < gs[k]->n; i++) {
            const IR_t * x = gs[k]->all[i];
            if (!x || !ir_is_call_kind(ir_kind0(x)) || ir_kind0(x) == IR_CALL_VALUE || !IR_LIT(x).sval) continue;
            int h = zls_home_callee(nm, gx, cap, IR_LIT(x).sval);
            if (h >= 0 && rt[h]) { rt[k] = 1; changed = 1; }
        }
    }
    for (int k = 0; k < n; k++) for (int i = 0; gs[k] && i < gs[k]->n; i++) {
        const IR_t * x = gs[k]->all[i];
        if (!x || !ir_is_call_kind(ir_kind0(x)) || ir_kind0(x) == IR_CALL_VALUE || !IR_LIT(x).sval) continue;
        int h = zls_home_callee(nm, gx, cap, IR_LIT(x).sval);
        if (h < 0) fcn_flag(x, FCN_HUNRES);
        else if (rt[h] && !(gs[h]->cell_frame && !hs[h])) fcn_flag(x, FCN_HRET);
    }
}
int zls_fcn_hret(const IR_t * nd) { return fcn_find(nd, FCN_HRET) ? 1 : 0; }
int zls_fcn_hunres(const IR_t * nd) { return fcn_find(nd, FCN_HUNRES) ? 1 : 0; }
int zls_fcn_bs_get(const IR_t * nd, int which) { uint32_t k = which ? FCN_BOK : FCN_BSK, v = which ? FCN_BOV : FCN_BSF; return fcn_find(nd, k) ? (fcn_find(nd, v) ? 2 : 1) : 0; }
void zls_fcn_bs_set(const IR_t * nd, int which, int val) {
    if (!nd) return;
    uint32_t k = which ? FCN_BOK : FCN_BSK, v = which ? FCN_BOV : FCN_BSF;
    fcn_t * t = fcn_get(nd);
    t->fl |= k;
    if (val == 2) t->fl |= v;
    else t->fl &= ~v;
}
static int zls_home_dyn(const IR_graph_t * g, const IR_t * nd) { extern int planner_home_dyn(const IR_graph_t *, const IR_t *); return planner_home_dyn(g, nd); }
const char * zls_home_why_name(int w) {
    switch (w) {
        case 1:
        return "suspend";
        case 2:
        return "coexpression";
        case 3:
        return "unknown-callee";
        case 4:
        return "generator-callee";
        case 5:
        return "retaining-callee";
        case 6:
        return "unresolved-callee";
        case 7:
        return "deferred-pattern";
        case 8:
        return "dynamic";
        case 9:
        return "uplevel";
        case 10:
        return "pinned-temp";
        case 11:
        return "read-across-excursion";
        case 12:
        return "plan-run-split";
        default:
        return "-";
    }
}
static const IR_t * zls_chase(const IR_t * t) { int g = 0; while (t && (t->op == IR_GOTO || (t->op == IR_SUCCEED && t->γ.node)) && g++ < 4096) t = t->γ.node; return t; }
int zls_frame_resident(const IR_t * nd);
int zls_dj_arm_resumes(const IR_t * nd, int a) { const IR_t * rs = zls_chase(nd->operands[2 * a + 1]); return rs && rs != nd && rs->op != IR_FAIL && rs->op != IR_SUCCEED; }
static int zls_dj_arm_rbp_state(const IR_t * nd, int a) {
    const IR_t * rs = zls_chase(nd->operands[2 * a + 1]), * x = zls_chase(nd->operands[2 * a]);
    if (!rs || rs->op != IR_TO || !ir_range_trailed(rs)) return 0;
    for (int g = 0; x && x != rs && g < 8; g++) {
        int o = (int)x->op;
        if (o != IR_LIT_INTEGER && o != IR_LIT_STRING && o != IR_LIT_REAL && o != IR_VAR && o != IR_LINE_MARK) return 0;
        x = zls_chase(x->γ.node);
    }
    return x == rs && zls_chase(rs->γ.node) == nd;
}
int zls_dj_rbp_arms(const IR_t * nd) {
    int na = (nd && nd->op == IR_DISJUNCTION) ? (int)IR_LIT(nd).ival : 0, r = 0;
    if (na <= 0 || nd->n_operands < 3 * na) return 0;
    for (int a = 0; a < na; a++) if (zls_dj_arm_resumes(nd, a)) { if (!zls_dj_arm_rbp_state(nd, a)) return 0; r = 1; }
    return r;
}
int zls_dj_retains(const IR_t * nd) {
    if (!nd || nd->op != IR_DISJUNCTION) return 0;
    int na = (int)IR_LIT(nd).ival;
    if (na <= 0 || nd->n_operands < 3 * na) return 0;
    for (int a = 0; a < na; a++) if (zls_dj_arm_resumes(nd, a)) return !zls_dj_rbp_arms(nd);
    return 0;
}
int zls_dj_cut_arm(const IR_graph_t * g, const IR_t * nd) {
    if (!g || !nd || nd->op != IR_DISJUNCTION || g->n <= 0) return 0;
    int na = (int)IR_LIT(nd).ival;
    if (na <= 0 || nd->n_operands < 3 * na) return 0;
    zls_nidx_t nx;
    zls_nidx_build(&nx, g);
    int n = g->n, hit = 0;
    unsigned char vis[n];
    int st[2 * n + 4];
    for (int a = 0; a < na && !hit; a++) {
        const IR_t * rs = zls_chase(nd->operands[2 * a + 1]);
        int top = 0, e0 = nd->operands[2 * a] ? zls_reuse_index(&nx, zls_chase(nd->operands[2 * a])) : -1;
        memset(vis, 0, sizeof vis);
        if (e0 >= 0) { st[top++] = e0; vis[e0] = 1; }
        while (top > 0 && !hit) {
            const IR_t * x = g->all[st[--top]];
            if (!x || x == nd || x == rs) continue;
            if (x->op == IR_CUT) { hit = 1; break; }
            for (int k = 0; k < 3; k++) {
                const IR_t * y = k == 0 ? x->γ.node : k == 1 ? x->ω.node : (x->op == IR_GATE_ARM && x->n_operands > 0) ? x->operands[0] : (const IR_t *)0;
                int yi = y ? zls_reuse_index(&nx, zls_chase(y)) : -1;
                if (yi < 0 || vis[yi]) continue;
                vis[yi] = 1;
                st[top++] = yi;
            }
        }
    }
    ct_drop(nx.k);
    ct_drop(nx.v);
    return hit;
}
int zls_frame_word_why(const IR_t * nd) { return zls_dj_retains(nd) ? 1 : (nd && nd->op == IR_TO && ir_range_kind_is(nd, "db")) ? 2 : 0; }
static int zls_home_local_kind(int lo) {
    return lo == IR_VAR || lo == IR_VAR_REF || lo == IR_ASSIGN || lo == IR_ASSIGN_VAR || lo == IR_REV_ASSIGN_VAR || lo == IR_NULLTEST_VAR || lo == IR_FIELD_VAR || lo == IR_UNIFY_FIRST ||
        lo == IR_UNIFY_VALUE;
}
static int zls_hf_excursion(const IR_graph_t * g, const IR_t * x) { return g->home_frame && (x->op == IR_CALL_PROC_STAGED || x->op == IR_TO) && zls_edge_retains(g, x); }
void zls_home_pass(const IR_graph_t * g) {
    if (!g || g->n <= 0) return;
    int n = g->n;
    zls_nidx_t nx;
    zls_nidx_build(&nx, g);
    int sg[n], so[n], st[2 * n + 2], sd[2 * n + 2], sw[2 * n + 2];
    unsigned char dy[n], rn[n], tw[n];
    unsigned int seen[2 * n];
    memset(seen, 0, sizeof seen);
    for (int i = 0; i < n; i++) {
        const IR_t * x = g->all[i];
        sg[i] = (x && x->γ.node) ? zls_reuse_index(&nx, x->γ.node) : -1;
        so[i] = (x && x->ω.node) ? zls_reuse_index(&nx, x->ω.node) : -1;
        dy[i] = x ? (unsigned char)(zls_home_dyn(g, x) ? zls_home_dyn(g, x) : zls_hf_excursion(g, x) ? 5 : (g->home_frame && zls_g_resumable_frame(g) && planner_span_retains(g, i, i)) ? 11 : 0) : 0;
        rn[i] = (x && x->op == IR_CUT) ? 1 : 0;
        tw[i] = 0;
    }
    unsigned int stamp = 0;
    for (int c = 0; c < n; c++) {
        const IR_t * cx = g->all[c];
        if (!cx) continue;
        if (cx->op == IR_UNMARK && (IR_LIT(cx).ival & 32) && cx->n_operands > 0 && planner_bound_spine(g, cx->operands[0])) continue;
        for (int oj = 0; oj < cx->n_operands; oj++) {
            const IR_t * po = cx->operands[oj];
            for (int cg = 0; g->home_frame && po && po->op == IR_CONJUNCTION && po->n_operands > 0 && cg < 16; cg++) po = po->operands[0];
            int p = po ? zls_reuse_index(&nx, po) : -1;
            if (p < 0 || fcn_find(g->all[p], FCN_HOME) || (cx->op == IR_DISJUNCTION && g->cell_frame && !dy[p])) continue;
            int top = 0, hit = 0;
            stamp++;
            if (sg[p] >= 0 && sg[p] != p) { st[top] = sg[p]; sd[top] = dy[p] ? 1 : 0; sw[top] = dy[p]; seen[2 * sg[p] + sd[top]] = stamp; top++; }
            while (top > 0 && !hit) {
                top--;
                int x = st[top], d = sd[top], w = sw[top];
                if (x == c) { if (d) { fcn_t * t = fcn_get(g->all[p]); t->fl |= FCN_HOME; t->hw = w ? w : 8; hit = 1; } continue; }
                if (rn[x] && w != 5 && w != 11) { d = 0; w = 0; } else if (dy[x]) { d = 1; w = dy[x]; }
                const IR_t * xx = g->all[x];
                int na = (g->home_frame && xx && xx->op == IR_DISJUNCTION) ? (int)IR_LIT(xx).ival : 0;
                for (int e = 0; e < 2 + na; e++) {
                    int y = e == 0 ? sg[x] : e == 1 ? so[x] : (2 * (e - 2) < xx->n_operands && xx->operands[2 * (e - 2)]) ? zls_reuse_index(&nx, xx->operands[2 * (e - 2)]) : -1;
                    if (y < 0 || y == p || seen[2 * y + d] == stamp) continue;
                    seen[2 * y + d] = stamp;
                    st[top] = y;
                    sd[top] = d;
                    sw[top] = w;
                    top++;
                }
            }
        }
    }
    {
        int top = 0;
        for (int i = 0; i < n; i++) if (dy[i] && !tw[i] && !rn[i]) {
            tw[i] = dy[i];
            st[top] = i;
            top++;
            while (top > 0) { top--; int x = st[top]; for (int e = 0; e < 2; e++) { int y = e ? so[x] : sg[x]; if (y < 0 || tw[y] || rn[y]) continue; tw[y] = tw[x]; st[top] = y; top++; } }
        }
    }
    for (int i = 0; i < n; i++) { const IR_t * x = g->all[i]; if (x && tw[i] && zls_home_local_kind((int)x->op) && IR_LIT(x).sval) { fcn_t * t = fcn_get(x); t->fl |= FCN_LHOME; t->lw = tw[i]; } }
    for (int c = 0; c < n; c++) {
        const IR_t * cx = g->all[c];
        if (!g->home_frame || !cx || !tw[c]) continue;
        for (int oj = 0; oj < cx->n_operands; oj++) {
            const IR_t * o = cx->operands[oj];
            if (o && o->op == IR_VAR && IR_LIT(o).sval && !fcn_find(o, FCN_LHOME)) { fcn_t * t = fcn_get(o); t->fl |= FCN_LHOME; t->lw = tw[c]; }
        }
    }
    for (int c = 0; c < n; c++) {
        const IR_t * cx = g->all[c];
        if (!g->home_frame || !cx || cx->op != IR_CREATE || cx->n_operands < 1 || !cx->operands[0]) continue;
        int e0 = zls_reuse_index(&nx, cx->operands[0]), qn = 0, q[n];
        unsigned char vis[n];
        if (e0 < 0) continue;
        memset(vis, 0, sizeof vis);
        q[qn++] = e0;
        vis[e0] = 1;
        for (int h = 0; h < qn; h++) {
            const IR_t * x = g->all[q[h]];
            if (!x) continue;
            if (zls_home_local_kind((int)x->op) && IR_LIT(x).sval) zls_home_local_declare(g, IR_LIT(x).sval, 2);
            if (x->op == IR_CORET || x->op == IR_COFAIL) continue;
            for (int k = 0; k < 2 + x->n_operands; k++) {
                const IR_t * y = k == 0 ? x->γ.node : k == 1 ? x->ω.node : x->operands[k - 2];
                int yi = y ? zls_reuse_index(&nx, y) : -1;
                if (yi < 0 || yi == c || vis[yi]) continue;
                vis[yi] = 1;
                q[qn++] = yi;
            }
        }
    }
    ct_drop(nx.k);
    ct_drop(nx.v);
}
void zls_home_local_declare(const IR_graph_t * g, const char * name, int why) {
    for (int i = 0; g && name && i < g->n; i++) {
        const IR_t * x = g->all[i];
        if (x && zls_home_local_kind((int)x->op) && IR_LIT(x).sval && !strcmp(IR_LIT(x).sval, name)) { fcn_t * t = fcn_get(x); t->fl |= FCN_LHOME; t->lw = why; }
    }
}
int zls_home_why(const IR_t * nd) { const fcn_t * t = fcn_find(nd, FCN_HOME); return t ? t->hw : 0; }
int zls_home_member(const IR_t * nd) { return (nd && zls_home_why(nd) && ir_node_produces_value(nd->op)) ? 1 : 0; }
void zls_frame_resident_mark(const IR_t * nd) { fcn_flag(nd, FCN_RES); }
int zls_frame_home(const IR_t * nd) { return (zls_home_member(nd) || zls_frame_word_why(nd) || (nd && nd->op == IR_SCAN_ENTER && zls_home_why(nd))) && zls_off(nd) >= 0 ? 1 : 0; }
int zls_frame_resident(const IR_t * nd) {
    return nd &&
        (zls_home_member(nd) || zls_frame_word_why(nd) || (nd->op == IR_SCAN_ENTER && zls_home_why(nd)) || fcn_find(nd, FCN_RES) || fc_vlit_active(nd) || fc_vread_fp(nd) >= 0 ||
        fc_vbinop_active(nd) || fc_vdj_active(nd) || fc_call_active(nd)) ? 1 : 0;
}
static int zls_edge_retains(const IR_graph_t * g, const IR_t * nd) { extern int planner_edge_retains(const IR_graph_t *, const IR_t *); return planner_edge_retains(g, nd); }
static int zls_hf_resident_cleanup(const IR_graph_t * g, const IR_t * nd) { return zls_g_home_frame(g) && nd->op == IR_CLEANUP ? 1 : 0; }
static int zls_hf_resident(const IR_graph_t * g, const IR_t * nd) {
    return nd &&
        (zls_home_member(nd) || zls_frame_word_why(nd) || (fcn_find(nd, FCN_RES) && !(g->spine_cells && nd->op == IR_DISJUNCTION)) || zls_hf_resident_cleanup(g, nd) ||
        (zls_home_why(nd) && !ir_node_produces_value(nd->op)) ||
        (zls_g_resumable_frame(g) &&
        (zls_home_dyn(g, nd) || zls_edge_retains(g, nd) ||
        (zls_g_home_frame(g) && ((nd->op == IR_BOUND && !planner_bound_spine(g, nd)) || nd->op == IR_GATE || nd->op == IR_TO || (nd->op == IR_DISJUNCTION && zls_dj_cut_arm(g, nd))))))) ? 1 : 0;
}
int zls_g_hf_resident(const IR_graph_t * g, const IR_t * nd) { return zls_g_home_frame(g) && zls_hf_resident(g, nd) ? 1 : 0; }
int zls_home_local_why(const IR_t * nd) { const fcn_t * t = fcn_find(nd, FCN_LHOME); return t ? t->lw : 0; }
int zls_redisp_off(const IR_t * nd) { const fcn_t * t = fcn_find(nd, FCN_RO); return t ? t->ro : -1; }
void zls_redisp_mark(const IR_t * nd, int off) { fcn_t * t = fcn_get(nd); t->ro = off; t->fl |= FCN_RO; }
int zls_home_local_test(const IR_graph_t * g, const char * vn) {
    for (int i = 0; g && vn && i < g->n; i++) {
        const IR_t * x = g->all[i];
        if (x && zls_home_local_kind((int)x->op) && IR_LIT(x).sval && !strcmp(IR_LIT(x).sval, vn) && zls_home_local_why(x)) return zls_home_local_why(x);
    }
    return 0;
}
int zls_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; return e->loff; }
int zls_result_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->off : -1; }
int zls_act_off(const IR_t * nd) {
    const zls_entry_t * e = zx_find(nd);
    if (!e) return -1;
    for (int i = zfc_first(nd, e->scope_id); i >= 0; i = zfc_next(i)) if (zf[i].what && !strncmp(zf[i].what, "callgen.act +0", 14)) return zf[i].off;
    return -1;
}
int zls_argv_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; return e->aoff >= 0 ? e->aoff : e->off + 16; }
int zls_node_bytes(const IR_t * nd) {
    const zls_entry_t * e = zx_find(nd);
    if (!e) return 0;
    int end = e->off;
    if (g_lower.ir.znb.len) { const znb_slot_t * t = znb_probe(&g_lower.ir.znb, nd, e->scope_id); if (t->gen == g_lower.ir.znb_gen && t->end > end) end = t->end; }
    int b = end - e->off;
    b = (b + 15) & ~15;
    return b;
}
int zls_scope_of(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->scope_id : -1; }
int zls_g_nslots(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->nslots : -1; }
int zls_g_region(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->region : -1; }
int zls_g_keys_only(const IR_graph_t * g) { return (g && zls_g_find(g) && !g->zframe_graph && !g->entry_frame && !g->cell_frame && !g->spine_cells && !g->root_graph && !g->caller_frame) ? 1 : 0; }
int zls_g_frame_region(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? (zls_g_keys_only(g) ? 0 : r->hf ? r->fregion : r->region) : -1; }
int zls_g_home_frame(const IR_graph_t * g) { zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0; return r ? r->hf : 0; }
int zls_g_kbase(const IR_graph_t * g) { zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0; return r ? r->kbase : 0; }
int zls_g_value_region(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? (r->hf ? r->fregion : r->region) : -1; }
int zls_key_unbacked(const IR_graph_t * g, int off) { zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0; return (r && (zls_g_keys_only(g) || (r->hf && off >= r->kbase))) ? 1 : 0; }
int zls_retains_why(const IR_graph_t * g, const IR_t * nd) { return (g && nd) ? zls_home_dyn(g, nd) : 0; }
int zls_g_key_class(const IR_graph_t * g, int off) {
    zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0;
    if (!r || r->first_scope < 0 || zls_g_keys_only(g) || off < 0) return 0;
    int region = r->hf ? r->fregion : r->region;
    if (!r->kmap) {
        int hi = region + 16;
        for (int j = r->first_vslot; j >= 0 && j < r->first_vslot + r->n_vslots && j < g_lower.ir.zv_n; j++) if (zv[j].off + 16 > hi) hi = zv[j].off + 16;
        r->kmap_n = (hi + 7) / 8;
        r->kmap = (unsigned char *)ct_zalloc((size_t)(r->kmap_n > 0 ? r->kmap_n : 1), 1);
        for (int f = 0; f < g_lower.ir.zf_n; f++) {
            if (zf[f].scope_id < r->first_scope || zf[f].scope_id >= r->first_scope + r->n_scopes || zf[f].size <= 0 || zf[f].off < 0) continue;
            int c = zf[f].nd ? (zls_frame_resident(zf[f].nd) ? 2 : 3) : 1;
            for (int u = zf[f].off / 8; u < (zf[f].off + zf[f].size + 7) / 8 && u < r->kmap_n; u++) if (!r->kmap[u] || c == 2 || (c == 3 && r->kmap[u] == 1)) r->kmap[u] = (unsigned char)c;
        }
        for (int j = r->first_vslot; j >= 0 && j < r->first_vslot + r->n_vslots && j < g_lower.ir.zv_n; j++) {
            int par = 0;
            for (int q = 0; q < g->nparams && g->pnames && zv[j].name; q++) if (g->pnames[q] && !strcmp(g->pnames[q], zv[j].name)) par = 1;
            int c = (par || zls_home_local_test(g, zv[j].name)) ? 2 : 3;
            for (int u = zv[j].off / 8; u < (zv[j].off + 16) / 8 && u >= 0 && u < r->kmap_n; u++) r->kmap[u] = (unsigned char)c;
        }
        for (int u = region / 8; u < (region + 16) / 8 && u < r->kmap_n; u++) if (!r->kmap[u]) r->kmap[u] = 1;
    }
    return off / 8 < r->kmap_n ? r->kmap[off / 8] : 0;
}
int zls_g_resident_bytes(const IR_graph_t * g) {
    zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0;
    if (!r || r->first_scope < 0 || zls_g_keys_only(g)) return 0;
    zls_g_key_class(g, 0);
    int n = 0, region = r->hf ? r->fregion : r->region;
    for (int u = 0; u < r->kmap_n && u < region / 8; u++) if (r->kmap[u] == 1 || r->kmap[u] == 2) n += 8;
    return n;
}
void zls_g_generator_set(const IR_graph_t * g) { zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0; if (r) r->gen = zls_g_has_suspend(g); }
int zls_g_generator_top(const IR_graph_t * g) {
    zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0;
    return (r && r->gen && g->cell_frame) ? zls_g_frame_bytes(g) + (g->nparams + g->nlocals) * 16 : 0;
}
int zls_frame_off(const IR_graph_t * g, const IR_t * nd) { return zls_off(nd) - zls_g_generator_top(g); }
int zls_g_resumable_frame(const IR_graph_t * g) { if (!g || !g->zframe_graph || g->cell_frame) return 0; return (g->resumable_callable || g->root_graph) ? 1 : 0; }
static int zls_g_pinned_frame(const IR_graph_t * g) { return zls_g_resumable_frame(g); }
int zls_g_det_block(const IR_graph_t * g) { return (g && g->static_calls && g->zframe_graph && !zls_g_resumable_frame(g) && !g->cell_frame && !g->root_graph) ? 1 : 0; }
int zls_g_block_args(const IR_graph_t * g) {
    if (g && g->cell_frame && !g->root_graph) return 1;
    if (!g || !g->zframe_graph || g->cell_frame) return 0;
    if (zls_g_resumable_frame(g)) return 1;
    if (zls_g_det_block(g)) return 1;
    return (g->block_args && !g->root_graph && g->nlocals == 0 && g->nparams <= 12) ? 1 : 0;
}
int zls_g_entry_block(const IR_graph_t * g) { return (zls_g_pinned_frame(g) || (g && g->cell_frame && !g->root_graph)) ? 1 : 0; }
int zls_g_frame_bytes(const IR_graph_t * g) {
    zls_graph_t * r = zls_g_find(g);
    int region = r ? zls_g_frame_region(g) : 0;
    int allowance = zls_g_pinned_frame(g) ? (g->root_graph ? FLAT_FRAME_ALLOWANCE_ROOT : FLAT_FRAME_ALLOWANCE_PINNED) : FLAT_FRAME_ALLOWANCE;
    return (allowance + region + (g ? 8 * g->standing_cells : 0) + 15) & ~15;
}
int zls_g_has_suspend(const IR_graph_t * g) { if (!g) return 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) return 1; return 0; }
int zls_g_block_base(const IR_graph_t * g) {
    if (!zls_g_block_args(g)) return -1;
    if (!g->cell_frame) return zls_g_frame_bytes(g);
    { int ft = zls_g_frame_bytes(g) + (g->nparams + g->nlocals) * 16; return zls_g_has_suspend(g) ? ((ft + 15) & ~15) + 72 : ft; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_forget_graph_nodes(const IR_graph_t * g) {
    if (!g) return;
    for (int i = 0; i < g_lower.ir.ze_n; i++) { if (!ze[i].nd) continue; for (int k = 0; k < g->n; k++) if (g->all[k] == ze[i].nd) { ze[i].nd = (const IR_t *)0; break; } }
    g_lower.ir.zx_n = 0;
    for (int i = 0; i < g_lower.ir.ze_n; i++) if (ze[i].nd) { cv_reserve(&g_lower.ir.zx_v, (uint32_t)sizeof(int), (uint64_t)g_lower.ir.zx_n + 1, "zx"); zx[g_lower.ir.zx_n++] = i; }
    qsort(zx, g_lower.ir.zx_n, sizeof(int), zx_cmp);
    g_lower.ir.zx_sorted = g_lower.ir.zx_n;
    {
        zls_graph_t * r = zls_g_find(g);
        if (r) { if (r->reuse) ct_drop(r->reuse); *r = (zls_graph_t){ g, r->name, -1, 0, 0, 0, -1, -1, 0, 0, 0, (struct zls_reuse_s *)0, 0, -1, (const IR_t *)0, (const IR_t *)0, r->own_name }; }
    }
}
int zls_g_resume(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->resume_off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_resume_by_name(const char *name) { if (!name) return -1; for (int i = 0; i < g_lower.ir.zg_n; i++) if (zg[i].name && strcmp(zg[i].name, name) == 0) return zg[i].resume_off; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_locals(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->locals_off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "gc_frame_map.h"
_Static_assert(GC_LAY_DESCR == ZK_DESCR && GC_LAY_RAW == ZK_RAW && GC_LAY_PTR_GC == ZK_PTR_GC && GC_LAY_PTR_CODE == ZK_PTR_CODE && GC_LAY_PTR_FRAME == ZK_PTR_FRAME &&
    GC_LAY_PTR_FRAME_H == ZK_PTR_FRAME_H,
    "a layout quad's kind IS the zls_field kind: the emitter seals zls_field_t.kind unchanged after the map's four quads and the collector reads it back as GC_LAY_*, so the two enumerations must agr"
    "ee by value (ARCH-GC section 6.2h)");
typedef struct { int off; int size; unsigned char kind; } zls_layq_t;
#define zlq ((zls_layq_t *)g_lower.ir.zlq_v.p)
static int zls_layq_cmp(const void * a, const void * b) {
    const zls_layq_t * x = (const zls_layq_t *)a;
    const zls_layq_t * y = (const zls_layq_t *)b;
    if (x->off != y->off) return x->off < y->off ? -1 : 1;
    if (x->size != y->size) return x->size > y->size ? -1 : 1;
    return (int)x->kind - (int)y->kind;
}
int zls_g_layout_q(const IR_graph_t * g, uint64_t * out, int cap, int * gap_bytes, int * conflicts) {
    zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0;
    int n = 0, m = 0, gaps = 0, conf = 0, expect = 0;
    if (gap_bytes) *gap_bytes = 0;
    if (conflicts) *conflicts = 0;
    if (!r || r->first_scope < 0) return -1;
    if (zls_g_keys_only(g)) return 0;
    cv_reserve(&g_lower.ir.zlq_v, (uint32_t)sizeof(zls_layq_t), (uint64_t)g_lower.ir.zf_n + 1, "zlq");
    for (int f = 0; f < g_lower.ir.zf_n; f++)
        if (zf[f].scope_id >= r->first_scope && zf[f].scope_id < r->first_scope + r->n_scopes && zf[f].size > 0 && zf[f].off >= 0 && !(r->hf && zf[f].off + zf[f].size > r->fregion)) zlq[n++] =
        (zls_layq_t){ zf[f].off, zf[f].size, zf[f].kind };
    qsort(zlq, (size_t)n, sizeof zlq[0], zls_layq_cmp);
    for (int i = 0; i < n; i++) {
        int off = zlq[i].off, end = zlq[i].off + zlq[i].size;
        unsigned kind = zlq[i].kind;
        if (off < expect) { unsigned lk = m > 0 ? GC_LAY_KIND(out[m - 1]) : 255u; if (lk != kind) conf++; if (end <= expect) continue; off = expect; }
        if (off > expect) gaps += off - expect;
        if (m > 0 && GC_LAY_KIND(out[m - 1]) == kind && GC_LAY_OFF(out[m - 1]) + GC_LAY_SIZE(out[m - 1]) == off && GC_LAY_SIZE(out[m - 1]) + (end - off) <= 0xFFF0) out[m - 1] =
            GC_LAY_Q(GC_LAY_OFF(out[m - 1]), kind, GC_LAY_SIZE(out[m - 1]) + (end - off));
        else { if (m >= cap) return cap + 1; out[m++] = GC_LAY_Q(off, kind, end - off); }
        expect = end;
    }
    if (r->region > expect) gaps += r->region - expect;
    if (gap_bytes) *gap_bytes = gaps;
    if (conflicts) *conflicts = conf;
    return m;
}
static int zls_int_cmp(const void * a, const void * b) { int x = *(const int *)a, y = *(const int *)b; return x < y ? -1 : x > y ? 1 : 0; }
static int zls_slot_matches(int f, int kind) {
    if (zf[f].kind != kind || zf[f].size <= 0 || zf[f].off < 0) return 0;
    return kind != ZK_PTR_FRAME || (zf[f].what && !strncmp(zf[f].what, "callgen.act +0", 14));
}
int zls_g_ptr_frame_offs(const IR_graph_t * g, int kind, int * out, int cap) {
    zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0;
    int n = 0;
    if (!r || r->first_scope < 0) return 0;
    for (int f = 0; f < g_lower.ir.zf_n; f++) if (zls_slot_matches(f, kind) && zf[f].scope_id >= r->first_scope && zf[f].scope_id < r->first_scope + r->n_scopes && !zls_key_unbacked(g, zf[f].off)) {
        if (out && n < cap) out[n] = zf[f].off;
        n++;
    }
    return n;
}
int zls_g_release_act_offs(const IR_graph_t * g, const IR_t * scope_nd, int kind, int * out, int cap) {
    int lo = 0, hi = 0, raw = 0, n = 0;
    const IR_construct_t * sc = ir_construct_find(g, (scope_nd && scope_nd->op == IR_CUT) ? IR_CK_CUT : IR_CK_BOUND, scope_nd);
    if (!g || !scope_nd || !sc) return zls_g_ptr_frame_offs(g, kind, out, cap);
    lo = sc->lo;
    hi = sc->hi;
    if (hi > g->n) hi = g->n;
    for (int k = lo; k < hi; k++) if (g->all[k]) raw++;
    {
        int tmp[raw > 0 ? raw : 1];
        for (int k = lo; k < hi; k++) {
            const IR_t * x = g->all[k];
            const zls_entry_t * e;
            if (!x || (x->op != IR_CALL_PROC_STAGED && x->op != IR_CALL_VALUE && x->op != IR_PROC_GEN) || !(e = zx_find(x))) continue;
            for (int f = zfc_first(x, e->scope_id); f >= 0; f = zfc_next(f)) if (zls_slot_matches(f, kind) && !zls_key_unbacked(g, zf[f].off)) { tmp[n++] = zf[f].off; break; }
        }
        if (n > 1) { int w = 1; qsort(tmp, (size_t)n, sizeof tmp[0], zls_int_cmp); for (int i = 1; i < n; i++) if (tmp[i] != tmp[w - 1]) tmp[w++] = tmp[i]; n = w; }
        for (int i = 0; out && i < n && i < cap; i++) out[i] = tmp[i];
    }
    return n;
}
int zls_g_zeta_mark(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->zeta_mark_off : -1; }
int zls_g_vslot_count(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->n_vslots : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * zls_g_vslot_get(const IR_graph_t * g, int i, int * off) {
    zls_graph_t * r = zls_g_find(g);
    if (!r || i < 0 || i >= r->n_vslots) { if (off) *off = -1; return (const char *)0; }
    if (off) *off = zv[r->first_vslot + i].off;
    return zv[r->first_vslot + i].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zk_name(int k) {
    return k == ZK_DESCR ? "DESCR" : k == ZK_RAW ? "RAW" : k == ZK_PTR_GC ? "PTR_GC" : k == ZK_PTR_CODE ? "PTR_CODE" : k == ZK_PTR_FRAME ? "PTR_FRAME" : k == ZK_PTR_FRAME_H ? "PTR_FRAME_H" : "?";
}
static const char * zsc_name(int k) { return k == ZSC_FN ? "FN" : k == ZSC_GROUP ? "GROUP" : k == ZSC_ITER ? "ITER" : k == ZSC_PAT ? "PAT" : k == ZSC_COEXPR ? "COEXPR" : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_op_names_a_string(IR_e op) {
    return op == IR_CALL || op == IR_CALL_BUILTIN || op == IR_CALL_BUILTIN_GEN || op == IR_CALL_ICON || op == IR_CALL_PROC_STAGED || op == IR_CALL_SNOBOL4 || op == IR_CALL_VALUE ||
        op == IR_MATCH_DEFER || op == IR_LIT_STRING || op == IR_VAR || op == IR_VAR_REF;
}
static int zls_reuse_straight(IR_e op) {
    return op == IR_LIT_INTEGER || op == IR_LIT_REAL || op == IR_LIT_STRING || op == IR_LIT_ATOM || op == IR_VAR || op == IR_VAR_REF || op == IR_BINOP || op == IR_CMP_TEST || op == IR_LINE_MARK;
}
static int zls_reuse_detleaf(const IR_t * c) { return c && ir_kind0(c) == IR_CALL && c->seal == IR_SEAL_CALL_DET_LEAF; }
static int zls_wi_cmp(const void * a, const void * b) {
    intptr_t x = (intptr_t)((const zls_pi_t *)a)->p, y = (intptr_t)((const zls_pi_t *)b)->p;
    if (x != y) return x < y ? -1 : 1;
    return ((const zls_pi_t *)a)->i - ((const zls_pi_t *)b)->i;
}
static int zls_pi_cmp(const void * a, const void * b) {
    uintptr_t x = (uintptr_t)((const zls_pi_t *)a)->p, y = (uintptr_t)((const zls_pi_t *)b)->p;
    if (x != y) return x < y ? -1 : 1;
    return ((const zls_pi_t *)a)->i - ((const zls_pi_t *)b)->i;
}
static void zls_ds_build(zls_ds_t * ds, const IR_graph_t * g, const char * rb, int dl_w) {
    int n = g->n;
    zls_pi_t * pi = (zls_pi_t *)ct_alloc(sizeof(zls_pi_t) * (size_t)(n > 0 ? n : 1));
    ds->rdk = (int *)ct_alloc(sizeof(int) * (size_t)(n > 0 ? n : 1));
    ds->rdm = (int *)ct_alloc(sizeof(int) * (size_t)(n > 0 ? n : 1));
    ds->dl_w = dl_w;
    ds->lw = (int **)ct_alloc(sizeof(int *) * (size_t)(dl_w > 0 ? dl_w : 1));
    ds->nlw = (int *)ct_alloc(sizeof(int) * (size_t)(dl_w > 0 ? dl_w : 1));
    ds->maxlast = (int *)ct_alloc(sizeof(int) * (size_t)(dl_w > 0 ? dl_w : 1));
    for (int j = 0; j < dl_w; j++) { ds->lw[j] = (int *)0; ds->nlw[j] = -1; ds->maxlast[j] = -1; }
    for (int i = 0; i < n; i++) { pi[i].p = g->all[i]; pi[i].i = i; ds->rdk[i] = -1; ds->rdm[i] = -1; }
    qsort(pi, (size_t)n, sizeof(zls_pi_t), zls_pi_cmp);
    for (int k = 0; k < n; k++) {
        const IR_t * d = g->all[k];
        if (!d || !rb[k]) continue;
        for (int m = 0; m < d->n_operands; m++) {
            const IR_t * o = d->operands[m];
            int lo = 0, hi = n;
            if (!o) continue;
            while (lo < hi) { int mid = lo + (hi - lo) / 2; if ((uintptr_t)pi[mid].p < (uintptr_t)o) lo = mid + 1; else hi = mid; }
            for (; lo < n && pi[lo].p == o; lo++) if (ds->rdk[pi[lo].i] < 0) { ds->rdk[pi[lo].i] = k; ds->rdm[pi[lo].i] = m; }
        }
    }
    ct_drop(pi);
}
static void zls_ds_leaves(zls_ds_t * ds, const IR_graph_t * g, const zls_reuse_t * rec, const char * rb, int j) {
    int c = 0;
    for (int k = 0; k < g->n; k++) { const IR_t * d = g->all[k]; if (d && rb[k] && zls_reuse_detleaf(d) && d->n_operands > j && rec[k].w >= 0) c++; }
    ds->lw[j] = (int *)ct_alloc(sizeof(int) * (size_t)(c > 0 ? c : 1));
    ds->nlw[j] = 0;
    for (int k = 0; k < g->n; k++) { const IR_t * d = g->all[k]; if (d && rb[k] && zls_reuse_detleaf(d) && d->n_operands > j && rec[k].w >= 0) ds->lw[j][ds->nlw[j]++] = rec[k].w; }
    qsort(ds->lw[j], (size_t)ds->nlw[j], sizeof(int), zls_int_cmp);
}
static int zls_ds_count(const int * v, int n, int a, int b) {
    int lo = 0, hi = n, l2 = 0, h2 = n;
    while (lo < hi) { int mid = lo + (hi - lo) / 2; if (v[mid] < a) lo = mid + 1; else hi = mid; }
    while (l2 < h2) { int mid = l2 + (h2 - l2) / 2; if (v[mid] <= b) l2 = mid + 1; else h2 = mid; }
    return l2 - lo;
}
static void zls_ds_drop(zls_ds_t * ds) {
    for (int j = ds->dl_w - 1; j >= 0; j--) if (ds->lw[j]) ct_drop(ds->lw[j]);
    ct_drop(ds->maxlast);
    ct_drop(ds->nlw);
    ct_drop(ds->lw);
    ct_drop(ds->rdm);
    ct_drop(ds->rdk);
}
static int zls_direct_slot(const IR_graph_t * g, const zls_reuse_t * rec, const char * rb, int nl, const int * mstart, int i, int dl_w, zls_ds_t * ds) {
    if (dl_w <= 0 || rec[i].nread != 1 || rec[i].w < 0 || rec[i].last <= rec[i].w) return -1;
    int rd = ds->rdk[i], j = ds->rdm[i];
    if (rd < 0 || !zls_reuse_detleaf(g->all[rd]) || (nl > 0 && rd >= mstart[0]) || rec[rd].w != rec[i].last || j >= dl_w) return -1;
    if (ds->nlw[j] < 0) zls_ds_leaves(ds, g, rec, rb, j);
    if (zls_ds_count(ds->lw[j], ds->nlw[j], rec[i].w + 1, rec[i].last) > 1) return -1;
    if (ds->maxlast[j] >= rec[i].w + 1) return -1;
    return j;
}
static int zls_reuse_reader_ok(IR_e op) {
    return zls_reuse_straight(op) || op == IR_CALL || op == IR_CALL_PROC_STAGED || op == IR_UNIFY_CONST || op == IR_UNIFY_STRUCT || op == IR_UNIFY_FIRST || op == IR_UNIFY_VALUE;
}
static int zls_reuse_dynamic(const IR_graph_t * g) {
    for (int i = 0; i < g->n; i++) {
        const IR_t * d = g->all[i];
        if (!d) continue;
        if (d->op == IR_GOTO_DEFERRED || d->op == IR_GATE || d->op == IR_MATCH_DEFER || d->op == IR_MATCH_FENCE1) return 1;
        if ((ir_kind0(d) == IR_CALL || ir_kind0(d) == IR_CALL_BUILTIN || ir_kind0(d) == IR_CALL_SNOBOL4) && IR_LIT(d).sval && strcmp(IR_LIT(d).sval, "CODE") == 0) return 1;
    }
    return 0;
}
static int zls_reuse_walk(const zls_nidx_t * m, const IR_t * start, int * pos, int * step, int * loop_lo, int * loop_hi, int * nloop) {
    const IR_t * c = start;
    while (c) {
        int i = zls_reuse_index(m, c);
        if (i < 0) return 1;
        if (pos[i] >= 0) { if (pos[i] < *loop_lo) *loop_lo = pos[i]; if (*step - 1 > *loop_hi) *loop_hi = *step - 1; (*nloop)++; return 0; }
        pos[i] = (*step)++;
        c = c->γ.node;
    }
    return 0;
}
static void zls_reuse_plan(const IR_graph_t * g, zls_reuse_t * rec, int * ncp_out, int * nloop_out) {
    int n = g->n;
    int * pos = (int *)ct_alloc(sizeof(int) * (size_t)n);
    int * cp = (int *)ct_alloc(sizeof(int) * (size_t)n);
    zls_nidx_t nx;
    zls_nidx_build(&nx, g);
    for (int i = 0; i < n; i++) { pos[i] = -1; cp[i] = 0; rec[i] = (zls_reuse_t){ g->all[i], ZR_NONE, -1, -1, -1, (const IR_t *)0, 0, -1, -1, -1, -1 }; }
    int step = 0, loop_lo = 0x7fffffff, loop_hi = -1, nloop = 0, opaque = 0;
    opaque |= zls_reuse_walk(&nx, g->entry, pos, &step, &loop_lo, &loop_hi, &nloop);
    if (g->n_alts > 1 && g->alt_entry) for (int k = 1; k < g->n_alts; k++) if (g->alt_entry[k]) opaque |= zls_reuse_walk(&nx, g->alt_entry[k], pos, &step, &loop_lo, &loop_hi, &nloop);
    int dynamic = zls_reuse_dynamic(g) || opaque;
    int ncp = 0;
    for (int i = 0; i < n; i++) {
        const IR_t * c = g->all[i];
        if (!c) continue;
        const IR_t * t = c->ω.node;
        if (t && !zls_is_wiring(ir_kind0(t))) { int j = zls_reuse_index(&nx, t); if (j >= 0 && !cp[j]) { cp[j] = 1; ncp++; } }
        if (pos[i] >= 0 && !zls_is_wiring(ir_kind0(c)) && !zls_reuse_straight(ir_kind0(c)) && !zls_reuse_detleaf(c) && !cp[i]) { cp[i] = 1; ncp++; }
    }
    int * rd_n = (int *)ct_zalloc((size_t)n, sizeof(int));
    int * rd_max = (int *)ct_alloc(sizeof(int) * (size_t)n);
    char * rd_bad = (char *)ct_zalloc((size_t)n, 1);
    char * rd_bop = (char *)ct_zalloc((size_t)n, 1);
    for (int i = 0; i < n; i++) rd_max[i] = -1;
    int gsz = 1;
    while (gsz < step) gsz <<= 1;
    int * gst = (int *)ct_alloc(sizeof(int) * 2 * (size_t)gsz);
    for (int q = 0; q < 2 * gsz; q++) gst[q] = 0x7fffffff;
    for (int q = 0; q < n; q++) if (cp[q] && pos[q] >= 0 && q < gst[gsz + pos[q]]) gst[gsz + pos[q]] = q;
    for (int q = gsz - 1; q >= 1; q--) gst[q] = gst[2 * q] < gst[2 * q + 1] ? gst[2 * q] : gst[2 * q + 1];
    for (int k = 0; k < n; k++) {
        const IR_t * d = g->all[k];
        if (!d) continue;
        for (int j = 0; j < d->n_operands; j++) {
            int t = d->operands[j] ? zls_reuse_index(&nx, d->operands[j]) : -1;
            if (t < 0) continue;
            rd_n[t]++;
            if (pos[k] < 0) rd_bad[t] = 1;
            else { if (pos[k] > rd_max[t]) rd_max[t] = pos[k]; if (!zls_reuse_reader_ok(ir_kind0(d))) rd_bop[t] = 1; }
        }
    }
    for (int i = 0; i < n; i++) {
        const IR_t * c = g->all[i];
        if (!c || zls_is_wiring(ir_kind0(c))) continue;
        rec[i].w = pos[i];
        rec[i].llo = loop_lo;
        rec[i].lhi = loop_hi;
        if (dynamic) { rec[i].cls = ZR_DYNAMIC; continue; }
        if (pos[i] < 0) { rec[i].cls = ZR_UNPLACED; continue; }
        if (!zls_reuse_straight(ir_kind0(c)) && !zls_reuse_detleaf(c)) { rec[i].cls = ZR_SELF; continue; }
        int ci = zls_reuse_index(&nx, c);
        int last = pos[i] > rd_max[ci] ? pos[i] : rd_max[ci], badreader = rd_bad[ci], badop = rd_bop[ci], nread = rd_n[ci];
        rec[i].last = last;
        rec[i].nread = nread;
        if (badreader) { rec[i].cls = ZR_UNPLACED; continue; }
        int guard = -1;
        {
            int lo = pos[i] + 1 + gsz, hi = last + gsz, best = 0x7fffffff;
            for (; lo < hi; lo >>= 1, hi >>= 1) { if (lo & 1) { if (gst[lo] < best) best = gst[lo]; lo++; } if (hi & 1) { hi--; if (gst[hi] < best) best = gst[hi]; } }
            if (best != 0x7fffffff) guard = best;
        }
        if (guard >= 0) { rec[i].cls = ZR_GUARD; rec[i].gpos = pos[guard]; rec[i].guard = g->all[guard]; continue; }
        if (nloop && pos[i] < loop_lo && last >= loop_lo) { rec[i].cls = ZR_LOOP; continue; }
        if (badop) { rec[i].cls = ZR_READER; continue; }
        rec[i].cls = ZR_CANDIDATE;
    }
    ct_drop(pos);
    ct_drop(cp);
    ct_drop(rd_n);
    ct_drop(rd_max);
    ct_drop(rd_bad);
    ct_drop(rd_bop);
    ct_drop(nx.k);
    ct_drop(nx.v);
    ct_drop(gst);
    if (ncp_out) *ncp_out = ncp;
    if (nloop_out) *nloop_out = nloop;
}
static const zls_reuse_t * zls_reuse_find(const zls_graph_t * r, const IR_t * nd) { for (int i = 0; i < r->n_reuse; i++) if (r->reuse[i].nd == nd) return &r->reuse[i]; return (const zls_reuse_t *)0; }
static void zls_reuse_dump(FILE * fp, const zls_graph_t * r) {
    const IR_graph_t * g = r->g;
    if (!g || g->n <= 0 || !g->all) return;
    int n = g->n;
    int * lo = (int *)ct_alloc(sizeof(int) * (size_t)n);
    int * hi = (int *)ct_alloc(sizeof(int) * (size_t)n);
    for (int i = 0; i < n; i++) { lo[i] = -1; hi[i] = -1; }
    int results = 0, cand = 0, pdet = 0, pself = 0, pguard = 0, punpl = 0, ploop = 0, pdyn = 0, preader = 0, elided = 0, placed = 0, boxes = 0, pooled = 0, direct = 0, ncp = 0, nloop = 0,
        pool_slots = 0;
    {
        int seen[n > 0 ? n : 1];
        int sn = 0;
        for (int i = 0; i < n; i++) {
            const IR_t * c = g->all[i];
            if (!c || zls_is_wiring(ir_kind0(c))) continue;
            const zls_reuse_t * q = zls_reuse_find(r, c);
            if (!q) continue;
            boxes++;
            if (q->w >= 0) placed++;
            if (q->pooled >= 0 && q->direct < 0) { int dup = 0; for (int t = 0; t < sn; t++) if (seen[t] == q->pooled) dup = 1; if (!dup) seen[sn++] = q->pooled; }
            if (q->direct >= 0) direct++;
        }
        pool_slots = sn;
    }
    for (int i = 0; i < n; i++) {
        const IR_t * c = g->all[i];
        if (!c || zls_is_wiring(ir_kind0(c))) continue;
        const zls_entry_t * e = zx_find(c);
        if (!e || e->scope_id < r->first_scope || e->scope_id >= r->first_scope + r->n_scopes) continue;
        const zls_reuse_t * q = zls_reuse_find(r, c);
        if (!q) continue;
        results++;
        const char * on = bb_op_name(ir_kind0(c));
        if (!on) on = "?";
        const char * dsv = IR_LIT(c).sval ? IR_LIT(c).sval : "?";
        int dlt = zls_reuse_detleaf(c);
        char dls[!dlt ? 1 : c->n_operands == 0 ? fmt_len("  det leaf %s: no beta, argv dead at gamma, no argv", dsv) :
            fmt_len("  det leaf %s: no beta, argv dead at gamma, argv@+%d%s", dsv, zls_argv_off(c), e->aoff >= 0 ? " pooled" : " own")];
        dls[0] = 0;
        if (dlt) {
            pdet++;
            if (c->n_operands == 0) snprintf(dls, sizeof dls, "  det leaf %s: no beta, argv dead at gamma, no argv", IR_LIT(c).sval ? IR_LIT(c).sval : "?");
            else snprintf(dls, sizeof dls, "  det leaf %s: no beta, argv dead at gamma, argv@+%d%s", IR_LIT(c).sval ? IR_LIT(c).sval : "?", zls_argv_off(c), e->aoff >= 0 ? " pooled" : " own");
        }
        {
            int shared = 0;
            for (int t = 0; t < g_lower.ir.ze_n; t++) if (ze[t].off == e->off && ze[t].scope_id >= r->first_scope && ze[t].scope_id < r->first_scope + r->n_scopes && ze[t].nd != c) shared = 1;
            if (shared && !e->live) {
                elided++;
                fprintf(fp, ";     reuse +%-5d %-18s w=%-4d ELIDED shared dead-result scratch: its gamma writes here at %d%s\n", e->off, on, q->w, 2 * q->w + 1, dls);
                continue;
            }
        }
        switch (q->cls) {
            case ZR_DYNAMIC:
            pdyn++;
            fprintf(fp, ";     reuse +%-5d %-18s PINNED dynamic entry into this graph%s\n", e->off, on, dls);
            continue;
            case ZR_UNPLACED:
            punpl++;
            fprintf(fp, ";     reuse +%-5d %-18s w=%-4d PINNED unplaced: not on the gamma spine, or a reader is not%s\n", e->off, on, q->w, dls);
            continue;
            case ZR_SELF:
            pself++;
            fprintf(fp, ";     reuse +%-5d %-18s w=%-4d PINNED self: beta-capable box, its result may be re-read on resume\n", e->off, on, q->w);
            continue;
            case ZR_GUARD:
            {
                pguard++;
                const char * gn = q->guard ? bb_op_name(ir_kind0(q->guard)) : "?";
                const char * gs = (q->guard && zls_op_names_a_string(ir_kind0(q->guard)) && IR_LIT(q->guard).sval) ? IR_LIT(q->guard).sval : "";
                fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d PINNED guard: box @%d %s%s%s can recede between the write and the read%s\n", e->off, on, q->w, q->last, q->gpos, gn ? gn : "?",
                    gs[0] ? " " : "", gs, dls);
                continue;
            }
            case ZR_LOOP:
            ploop++;
            fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d PINNED loop: written before a gamma back-edge span [%d..%d] that re-reads it%s\n", e->off, on, q->w, q->last, q->llo, q->lhi, dls);
            continue;
            case ZR_READER:
            preader++;
            fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d PINNED reader: a reader's template is not verified to read operands on alpha only%s\n", e->off, on, q->w, q->last, dls);
            continue;
            default:
            break;
        }
        cand++;
        if (q->pooled >= 0) { if (q->direct < 0) pooled++; } else { lo[i] = q->w; hi[i] = q->last; }
        {
            char ds[64];
            if (q->direct >= 0) snprintf(ds, sizeof ds, " direct: argv block slot %d, the marshal copies nothing", q->direct);
            else snprintf(ds, sizeof ds, "%s", q->pooled >= 0 ? " pooled" : " unpooled");
            fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d CANDIDATE reads=%d%s%s%s\n", e->off, on, q->w, q->last, q->nread, e->live ? "" : " dead-result", ds, dls);
        }
    }
    int packed = 0;
    for (int i = 0; i < n; i++) if (lo[i] >= 0) for (int t = lo[i]; t <= hi[i]; t++) {
        int depth = 0;
        for (int k = 0; k < n; k++) if (lo[k] >= 0 && lo[k] <= t && hi[k] >= t) depth++;
        if (depth > packed) packed = depth;
    }
    { const zls_reuse_t * q0 = (r->n_reuse > 0) ? &r->reuse[0] : (const zls_reuse_t *)0; if (q0 && q0->lhi >= 0) nloop = 1; }
    for (int i = 0; i < r->n_reuse; i++) if (r->reuse[i].cls == ZR_SELF || r->reuse[i].cls == ZR_GUARD) ncp++;
    int predicted = r->region - 16 * ((cand - pooled - direct) - packed > 0 ? (cand - pooled - direct) - packed : 0);
    char scr[200];
    if (r->scratch_pool >= 0) snprintf(scr, sizeof scr, "overlay(pool slot %d: no dead write lands inside a pooled interval)", r->scratch_pool);
    else if (r->scratch_hit_w && r->scratch_hit_t) {
        const zls_reuse_t * qw = zls_reuse_find(r, r->scratch_hit_w), * qt = zls_reuse_find(r, r->scratch_hit_t);
        snprintf(scr, sizeof scr, "own(REFUSED: %s dead write at %d lands inside %s [%d..%d] on every pool slot)", bb_op_name(ir_kind0(r->scratch_hit_w)), qw ? 2 * qw->w + 1 : -1,
            bb_op_name(ir_kind0(r->scratch_hit_t)), qt ? 2 * qt->w + 1 : -1, qt ? 2 * qt->last : -1);
    } else if (r->scratch_hit_w) snprintf(scr, sizeof scr, "own(REFUSED: %s is a dead writer whose position or scope the plan cannot bound)", bb_op_name(ir_kind0(r->scratch_hit_w)));
    else snprintf(scr, sizeof scr, "%s", elided ? (pool_slots ? "own(loop: a back-edge re-runs the dead writers)" : "own(no pool slot)") : "none");
    fprintf(fp,
        ";   reuse '%s' results=%d candidates=%d pooled=%d direct=%d pool_slots=%d pinned=%d (self=%d guard=%d unplaced=%d loop=%d dynamic=%d reader=%d detleaf=%d) elided=%d spine=%d/%d betacapable="
        "%d loops=%d packed_min=%d predicted_region_end=%d scratch=%s\n", r->name ? r->name : "?", results, cand, pooled, direct, pool_slots, pself + pguard + punpl + ploop + pdyn + preader, pself,
        pguard, punpl, ploop, pdyn, preader, pdet, elided, placed, boxes, ncp, nloop, packed, predicted, scr);
    ct_drop(lo);
    ct_drop(hi);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_dump(FILE * fp) {
    fprintf(fp, "; FRAME LAYOUT (per-activation, typed)\n");
    fprintf(fp,
        "; kinds: DESCR = 16B t.p pair (GC traces payload) | RAW = int/cursor/counter (GC skips) | PTR_GC = heap pointer (GC traces+fixes) | PTR_CODE = continuation (GC skips, never relocates)\n");
    fprintf(fp, "; (audit) = kind provisional pending template audit — 2026-07-05 burndown: all shipped grants template-verified, audit=0; any NEW grant lands audit=1 until verified\n");
    for (int i = 0; i < g_lower.ir.zg_n; i++) {
        zls_graph_t * r = &zg[i];
        if (r->first_scope < 0) continue;
        fprintf(fp, "; graph %d '%s' — slots=%d region_end=%d resume=%d vslots=%d scopes=%d\n", i, r->name ? r->name : "?", r->nslots, r->region, r->resume_off, r->n_vslots, r->n_scopes);
        for (int s = r->first_scope; s < r->first_scope + r->n_scopes; s++) {
            if (zs[s].n_fields == 0 && zs[s].klass == ZSC_GROUP) {
                fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d (no frame fields)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent);
                continue;
            }
            fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d [%d..%d)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent, zs[s].n_fields ? zs[s].lo_off : 0,
                zs[s].n_fields ? zs[s].hi_off : 0);
            for (int f = 0; f < g_lower.ir.zf_n; f++) if (zf[f].scope_id == s) {
                const char * on = "-";
                char onb[16];
                if (zf[f].nd) { on = bb_op_name(ir_kind0(zf[f].nd)); if (!on) { snprintf(onb, sizeof onb, "op%d", (int)ir_kind0(zf[f].nd)); on = onb; } }
                fprintf(fp, ";     +%-5d %-3d %-8s %-36s %s%s\n", zf[f].off, zf[f].size, zk_name(zf[f].kind), zf[f].what ? zf[f].what : "", on, zf[f].audit ? "  (audit)" : "");
            }
        }
        for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) fprintf(fp, ";   vslot +%-5d 16  DESCR    %s\n", zv[v].off, zv[v].name ? zv[v].name : "?");
        zls_reuse_dump(fp, r);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_tail_candidate(const IR_t * head, const IR_t * arbno, int i0, int ia, int b0, int b1, int r1) {
    if (!head || !arbno || g_lower.ir.fct_n >= 64) return;
    g_lower.ir.fct[g_lower.ir.fct_n].head = head;
    g_lower.ir.fct[g_lower.ir.fct_n].arbno = arbno;
    g_lower.ir.fct[g_lower.ir.fct_n].i0 = i0;
    g_lower.ir.fct[g_lower.ir.fct_n].ia = ia;
    g_lower.ir.fct[g_lower.ir.fct_n].b0 = b0;
    g_lower.ir.fct[g_lower.ir.fct_n].b1 = b1;
    g_lower.ir.fct[g_lower.ir.fct_n].r1 = r1;
    g_lower.ir.fct[g_lower.ir.fct_n].fpl = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].fpb = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].fpr = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].fpr_rsp = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].span = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].rspan = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].opsb = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].fin = 0;
    g_lower.ir.fct[g_lower.ir.fct_n].nw = 0;
    g_lower.ir.fct_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_tail_wrap(const IR_t * arbno, const IR_t * save, const IR_t * cond) {
    if (!arbno || !save || !cond) return;
    for (int i = g_lower.ir.fct_n - 1; i >= 0; i--) if (g_lower.ir.fct[i].arbno == arbno) {
        cv_reserve(&g_lower.ir.fct[i].wsv, (uint32_t)sizeof(const IR_t *), (uint64_t)g_lower.ir.fct[i].nw + 1, "fct.wsv");
        cv_reserve(&g_lower.ir.fct[i].wcd, (uint32_t)sizeof(const IR_t *), (uint64_t)g_lower.ir.fct[i].nw + 1, "fct.wcd");
        CV_AT(g_lower.ir.fct[i].wsv, const IR_t *, g_lower.ir.fct[i].nw) = save;
        CV_AT(g_lower.ir.fct[i].wcd, const IR_t *, g_lower.ir.fct[i].nw) = cond;
        g_lower.ir.fct[i].nw++;
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_ncap(const IR_t * nd) { for (int i = 0; i < g_lower.ir.fct_n; i++) if (g_lower.ir.fct[i].arbno == nd && g_lower.ir.fct[i].fin) return g_lower.ir.fct[i].nw; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_arbno(const IR_t * nd, int * fpb, int * fpl, int * opsb, int * hdrb) {
    for (int i = 0; i < g_lower.ir.fct_n; i++) if (g_lower.ir.fct[i].arbno == nd && g_lower.ir.fct[i].fin) {
        if (fpb) *fpb = g_lower.ir.fct[i].fpb;
        if (fpl) *fpl = g_lower.ir.fct[i].fpl;
        if (opsb) *opsb = g_lower.ir.fct[i].opsb;
        if (hdrb) *hdrb = g_lower.ir.fct[i].span + g_lower.ir.fct[i].rspan;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_fpr_rsp(const IR_t * nd) { for (int i = 0; i < g_lower.ir.fct_n; i++) if (g_lower.ir.fct[i].arbno == nd && g_lower.ir.fct[i].fin) return g_lower.ir.fct[i].fpr_rsp; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_dfr(const IR_t * nd) { for (int i = 0; i < g_lower.ir.fct_n; i++) if (g_lower.ir.fct[i].arbno == nd && g_lower.ir.fct[i].fin) return g_lower.ir.fct[i].dfr; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_release(const IR_t * head, int * brdisp) {
    for (int i = 0; i < g_lower.ir.fct_n; i++) if (g_lower.ir.fct[i].head == head && g_lower.ir.fct[i].fin) {
        if (brdisp) *brdisp = g_lower.ir.fct[i].fpr + g_lower.ir.fct[i].fpb + g_lower.ir.fct[i].span + g_lower.ir.fct[i].rspan + 16;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_head(const IR_t * head) { for (int i = 0; i < g_lower.ir.fct_n; i++) if (g_lower.ir.fct[i].head == head) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_defer_susp_g(IR_graph_t * g, const IR_t * nd) {
    if (!g || !nd || (nd->op != IR_MATCH_DEFER)) return -1;
    for (int i = 0; i < g_lower.ir.fct_n; i++) if (g_lower.ir.fct[i].fin && g_lower.ir.fct[i].arbno) {
        int in = 0;
        for (int j = 0; j < g->n; j++) if (g->all[j] == g_lower.ir.fct[i].arbno) { in = 1; break; }
        if (!in) continue;
        for (int j = g_lower.ir.fct[i].i0; j < g_lower.ir.fct[i].r1 && j < g->n; j++) if (g->all[j] == nd) return fct_defer_susp(nd);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fl_derive_tier(IR_graph_t * g) {
    if (!g || !g->all) return;
    int statements = 0, matchers = 0, others = 0;
    for (int i = 0; i < g->n; i++) {
        const IR_t * c = g->all[i];
        if (!c) continue;
        {
            int o = (int)ir_kind0(c);
            if (o >= IR_MATCH && o <= IR_MATCH_VALUE) matchers++;
            else if (o != IR_SUCCEED && o != IR_FAIL && o != IR_GOTO && o != IR_LIT_STRING && o != IR_LIT_ATOM && o != IR_LIT_INTEGER && o != IR_LIT_REAL && o != IR_LIT_CHARSET && o != IR_LIT_NAME)
                others++;
        }
        switch ((int)ir_kind0(c)) {
            case IR_STATEMENT:
            case IR_STATEMENT_BEGIN:
            case IR_STATEMENT_END:
            case IR_STMT_MARK:
            case IR_SETEXIT_TEST:
            case IR_DEFINE:
            case IR_GOTO_DEFERRED:
            case IR_CALL_SNOBOL4:
            case IR_KW_SNOBOL4:
            case IR_KW_ASSIGN_SNOBOL4:
            case IR_DTP_ASSIGN:
            statements = 1;
            break;
            case IR_MATCH_BEGIN:
            case IR_MATCH_END:
            case IR_MATCH_LIT:
            case IR_MATCH_LEN:
            case IR_MATCH_ANY:
            case IR_MATCH_NOTANY:
            case IR_MATCH_SPAN:
            case IR_MATCH_BREAK:
            case IR_MATCH_BREAKX:
            case IR_MATCH_TAB:
            case IR_MATCH_RTAB:
            case IR_MATCH_POS:
            case IR_MATCH_RPOS:
            case IR_MATCH_REM:
            case IR_MATCH_ARB:
            case IR_MATCH_BAL:
            case IR_MATCH_ATP:
            case IR_MATCH_LAMBDA:
            case IR_MATCH_RETRY:
            case IR_MATCH_REPLACE:
            case IR_MATCH_ASSIGN_COND:
            case IR_MATCH_ASSIGN_IMM:
            case IR_MATCH_ASSIGN_SAVE:
            case IR_MATCH_SPAN_VAR:
            if (!g->block_args) statements = 1;
            break;
            default:
            break;
        }
    }
    if (g->cell_frame) return;
    if (statements || (matchers > 0 && others == 0)) return;
    g->zframe_graph = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_tables_reset(void) { g_lower.ir.fct_n = 0; g_lower.ir.fcn_gen++; g_lower.ir.fcn_n = 0; g_lower.ir.fca.len = 0; }
typedef struct { const char * name; int fb; int fp; int uni; } zls_pz_t;
#define pz ((zls_pz_t *)g_lower.ir.pz_v.p)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_patzeta_register(const char * name, int frame_bytes, int fp_total, int uniform) {
    if (!name) return;
    for (int i = 0; i < g_lower.ir.pz_n; i++) if (!strcmp(pz[i].name, name)) { pz[i].fb = frame_bytes; pz[i].fp = fp_total; pz[i].uni = uniform; return; }
    cv_reserve(&g_lower.ir.pz_v, (uint32_t)sizeof(zls_pz_t), (uint64_t)g_lower.ir.pz_n + 1, "pz");
    pz[g_lower.ir.pz_n].name = name;
    pz[g_lower.ir.pz_n].fb = frame_bytes;
    pz[g_lower.ir.pz_n].fp = fp_total;
    pz[g_lower.ir.pz_n].uni = uniform;
    g_lower.ir.pz_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_patzeta_frame_reserve(const char * name, int * bytes) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.ir.pz_n; i++) if (!strcmp(pz[i].name, name)) { if (bytes) *bytes = (((FLAT_FRAME_ALLOWANCE + pz[i].fb + 15) & ~15) + pz[i].fp); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_patzeta_lookup(const char * name, int * susp) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.ir.pz_n; i++) if (!strcmp(pz[i].name, name)) { if (susp) *susp = (((FLAT_FRAME_ALLOWANCE + pz[i].fb + 15) & ~15) + pz[i].fp); return pz[i].uni; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_fp_total(IR_graph_t * g) { return g ? fct_fp_range(g, 0, g->n) : 0; }
int zls_node_has_fields(const IR_t * nd) { if (!nd) return 0; for (int f = 0; f < g_lower.ir.zf_n; f++) if (zf[f].nd == nd) return 1; return 0; }
long zw_node_k(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; if (!e) return 0; long b = (long)zls_node_bytes(nd); if (!e->live && b <= 16) return 0; return b; }
static int zw_nid_listed_c(const char * e, int nid) {
    if (!e || !*e) return 0;
    { const char * p = e; while (*p) { long v = strtol(p, (char **)&p, 10); if ((int)v == nid) return 1; while (*p && *p != ',') p++; if (*p) p++; } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long zw_carve_k(const IR_t * nd) {
    extern int bb_node_id(IR_t *);
    long _d, _k;
    int _spine;
    if (g_lower.ir.zw_carve_k_ba < 0) {
        const char * e = getenv("SCRIP_BB_ALLOC");
        g_lower.ir.zw_carve_k_ba = (e && *e == '0') ? 0 : 1;
        g_lower.ir.zw_carve_k_bo = getenv("SCRIP_BB_ONLY");
        g_lower.ir.zw_carve_k_bs = getenv("SCRIP_BB_SKIP");
        { const char * a = getenv("SCRIP_BB_ALLOC_ALL"); g_lower.ir.zw_carve_k_all = (a && *a == '0') ? 0 : 1; }
    }
    if (!g_lower.ir.zw_carve_k_ba || !nd) return 0;
    _spine =
        (nd->op == IR_BINOP || nd->op == IR_ASSIGN || nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_ATOM || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET ||
        nd->op == IR_LIT_NAME || nd->op == IR_VAR || nd->op == IR_CMP_TEST || nd->op == IR_COERCE_NUMERIC || nd->op == IR_IDENT || nd->op == IR_DIFFER);
    if (_spine) return 0;
    if (!g_lower.ir.zw_carve_k_all && ((ir_kind0(nd) == IR_DEFINE && ir_define_sr_citizen(nd)) || ir_norm_call_kind(ir_kind0(nd)) == IR_CALL || ir_kind0(nd) == IR_GOTO_DEFERRED)) return 0;
    if (fc_geom(nd, &_d)) return 0;
    _k = zw_node_k(nd);
    if (_k <= 0) return 0;
    if ((g_lower.ir.zw_carve_k_bo && *g_lower.ir.zw_carve_k_bo) || (g_lower.ir.zw_carve_k_bs && *g_lower.ir.zw_carve_k_bs)) {
        int _nid = bb_node_id((IR_t *)nd);
        if (g_lower.ir.zw_carve_k_bo && *g_lower.ir.zw_carve_k_bo && !zw_nid_listed_c(g_lower.ir.zw_carve_k_bo, _nid)) return 0;
        if (zw_nid_listed_c(g_lower.ir.zw_carve_k_bs, _nid)) return 0;
    }
    return _k;
}
#include <stdio.h>
#include <stdlib.h>
void zop_audit_report(void);
void zop_audit_graph_close(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zop_audit_graph_close(void) {
    extern int zop_audit_seen(void);
    extern void zop_audit_seen_clear(void);
    int m, frames;
    { if (!g_lower.ir.zop_audit_graph_close_reg) { g_lower.ir.zop_audit_graph_close_reg = 1; atexit(zop_audit_report); } }
    m = zop_audit_seen();
    if (m == 0) return;
    zop_audit_seen_clear();
    frames = ((m >> 1) & 1) + ((m >> 3) & 1) + ((m >> 4) & 1);
    g_lower.ir.zop_g_total++;
    if (frames > 1) g_lower.ir.zop_g_mixed++;
    g_lower.ir.zop_hist[(m >> 1) & 15]++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zop_audit_report(void) {
    zop_audit_graph_close();
    if (!getenv("SCRIP_ZOP_AUDIT")) return;
    fprintf(stderr, "[ZOP] graphs=%ld mixed=%ld\n", g_lower.ir.zop_g_total, g_lower.ir.zop_g_mixed);
    {
        int k;
        const char * nm[16] = {"-","isle","cell","isle+cell","rsp","isle+___","cell+___","isle+cell+___","rsp","isle+rsp","cell+rsp","isle+cell+rsp","___+rsp","+","cell+___+rsp","all"};
        for (k = 0; k < 16; k++) if (g_lower.ir.zop_hist[k]) fprintf(stderr, "[ZOP]   %-18s %ld\n", nm[k], g_lower.ir.zop_hist[k]);
    }
}
int zls_g_first_scope(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->first_scope : -1; }
