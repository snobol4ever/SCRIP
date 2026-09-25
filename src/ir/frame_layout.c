#include <string.h>
#include "ct_arena.h"
#include "ct_vec.h"
#include <stdlib.h>
#include "frame_layout.h"
#include "gc_frame_map.h"
#include "ast.h"
extern const char * bb_op_name(IR_e k);
extern int is_global(const char *);
extern int rt_proc_is_registered(const char *);
extern int rt_proc_is_generator(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_callee_is_gen(const IR_t * nd) { const char * fn = IR_LIT(nd).sval; return fn && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn); }
#define FL_MAX_ENTRIES 262144
#define FL_FC_SYNTH    0x7F000
#define FL_MAX_FIELDS  524288
#define FL_MAX_SCOPES  16384
#define FL_MAX_GRAPHS  16384
#define FL_MAX_VSLOTS  16384
#define FL_MAX_MARKS   262144
typedef struct { const IR_t * nd; int scope_id; int off; int loff; int live; int aoff; } zls_entry_t;
typedef struct { int scope_id; int off; int size; unsigned char kind; unsigned char audit; const char * what; const IR_t * nd; } zls_pfield_t;
typedef struct { const char * name; int off; } zls_vslot_t;
typedef struct { const IR_graph_t * g; const char * name; int start_n; const IR_t * anchor; } zls_mark_t;
typedef struct { const IR_graph_t * g; const char * name; int first_scope; int n_scopes; int nslots; int region; int resume_off; int zeta_mark_off; int locals_off; int first_vslot; int n_vslots; struct zls_reuse_s * reuse; int n_reuse; int scratch_pool; const IR_t * scratch_hit_w; const IR_t * scratch_hit_t; } zls_graph_t;
static zls_entry_t  ze[FL_MAX_ENTRIES];  static int ze_n = 0;
static zls_pfield_t zf[FL_MAX_FIELDS];   static int zf_n = 0;
typedef struct { const IR_t * nd; int scope; int end; uint32_t gen; } znb_slot_t;
static cv_t g_znb; static uint32_t g_znb_gen = 1, g_znb_n = 0;
static znb_slot_t * znb_probe(cv_t * v, const IR_t * nd, int scope) {
    uint64_t h = ((((uint64_t)(uintptr_t)nd >> 4) ^ ((uint64_t)(uint32_t)scope * 0x9e3779b97f4a7c15ull)) * 0xff51afd7ed558ccdull) & (v->len - 1);
    for (;;) { znb_slot_t * t = &CV_AT(*v, znb_slot_t, h); if (t->gen != g_znb_gen || (t->nd == nd && t->scope == scope)) return t; h = (h + 1) & (v->len - 1); }
}
static void znb_note(const IR_t * nd, int scope, int end) {
    if ((uint64_t)(g_znb_n + 1) * 2 > g_znb.len) { uint32_t nc = g_znb.len ? g_znb.len * 2 : 256; cv_t o = g_znb, n = { 0, 0, 0, 0 };
        n.p = ct_zalloc(nc, sizeof(znb_slot_t)); n.len = nc; n.cap = nc; n.esz = (uint32_t)sizeof(znb_slot_t); g_znb = n;
        for (uint32_t i = 0; i < o.len; i++) { znb_slot_t q = CV_AT(o, znb_slot_t, i); if (q.gen == g_znb_gen) *znb_probe(&g_znb, q.nd, q.scope) = q; }
        if (o.p) ct_drop(o.p); }
    znb_slot_t * t = znb_probe(&g_znb, nd, scope);
    if (t->gen != g_znb_gen) { t->nd = nd; t->scope = scope; t->end = end; t->gen = g_znb_gen; g_znb_n++; } else if (end > t->end) t->end = end;
}
static zls_scope_t  zs[FL_MAX_SCOPES];   static int zs_n = 0;
static zls_graph_t  zg[FL_MAX_GRAPHS];   static int zg_n = 0;
static zls_vslot_t  zv[FL_MAX_VSLOTS];   static int zv_n = 0;
static zls_mark_t   zm[FL_MAX_MARKS];    static int zm_n = 0;
static zls_entry_t * zx[FL_MAX_ENTRIES]; static int zx_n = 0;
typedef struct zls_reuse_s { const IR_t * nd; int cls; int w; int last; int gpos; const IR_t * guard; int nread; int pooled; int llo; int lhi; int direct; } zls_reuse_t;
#define ZR_CANDIDATE 0
#define ZR_SELF      1
#define ZR_GUARD     2
#define ZR_UNPLACED  3
#define ZR_LOOP      4
#define ZR_DYNAMIC   5
#define ZR_READER    6
#define ZR_DETLEAF   7
#define ZR_NONE      -1
static void zls_reuse_plan(const IR_graph_t * g, zls_reuse_t * rec, int * ncp_out, int * nloop_out);
static int zls_reuse_detleaf(const IR_t * c);
static int zls_reuse_straight(IR_e op);
static int zls_direct_slot(const IR_graph_t * g, const zls_reuse_t * rec, const char * rb, int nl, const int * mstart, int i, int dl_w);
typedef struct { const IR_t * nd; int min_off; int span; int zq[8]; int nzq; } zls_ageom_t;
static zls_ageom_t  za[1024];             static int za_n = 0;
static struct { const IR_t * head; const IR_t * arbno; int i0; int ia; int b0; int b1; int r1; int fpl; int fpb; int fpr; int fpr_rsp; int span; int rspan; int opsb; int fin; int dfr; const IR_t * wsv[4]; const IR_t * wcd[4]; int nw; } fct[64];
static int fct_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_reset(void) { for (int i = 0; i < zg_n; i++) if (zg[i].reuse) { ct_drop(zg[i].reuse); zg[i].reuse = (struct zls_reuse_s *)0; zg[i].n_reuse = 0; } ze_n = 0; zf_n = 0; zs_n = 0; zg_n = 0; zv_n = 0; zm_n = 0; zx_n = 0; za_n = 0; g_znb_gen++; g_znb_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    if (zm_n >= FL_MAX_MARKS) { fprintf(stderr, "zls: mark table overflow (%d)\n", FL_MAX_MARKS); abort(); }
    zm[zm_n++] = (zls_mark_t){ g, name, g->n, (const IR_t *)0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark_anchor(const IR_graph_t * g, const char * name, const IR_t * anchor) {
    if (!g || !name) return;
    if (zm_n >= FL_MAX_MARKS) { fprintf(stderr, "zls: mark table overflow (%d)\n", FL_MAX_MARKS); abort(); }
    zm[zm_n++] = (zls_mark_t){ g, name, g->n, anchor };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_group_count(const IR_graph_t * g) { int c = 0; for (int i = 0; i < zm_n; i++) if (zm[i].g == g) c++; return c; }
const IR_t * zls_g_group_anchor(const IR_graph_t * g, int k) { int c = 0; for (int i = 0; i < zm_n; i++) if (zm[i].g == g) { if (c == k) return zm[i].anchor; c++; } return (const IR_t *)0; }
static zls_graph_t * zls_g_find(const IR_graph_t * g) { for (int i = 0; i < zg_n; i++) if (zg[i].g == g) return &zg[i]; return (zls_graph_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_graph_name(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    zls_graph_t * r = zls_g_find(g);
    if (r) { r->name = name; if (r->first_scope >= 0 && r->first_scope < zs_n) zs[r->first_scope].name = name; return; }
    if (zg_n >= FL_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", FL_MAX_GRAPHS); abort(); }
    zg[zg_n] = (zls_graph_t){ g, name, -1, 0, 0, 0, -1, -1, 0, 0, 0, (struct zls_reuse_s *)0, 0, -1, (const IR_t *)0, (const IR_t *)0 };
    zg_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * zls_graph_name_get(const IR_graph_t * g) { zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0; return r ? r->name : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zx_cmp(const void * a, const void * b) { const zls_entry_t * x = *(zls_entry_t * const *)a; const zls_entry_t * y = *(zls_entry_t * const *)b; return (x->nd > y->nd) - (x->nd < y->nd); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const zls_entry_t * zx_find(const IR_t * nd) {
    int lo = 0, hi = zx_n - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (zx[m]->nd == nd) return zx[m]; if (zx[m]->nd < nd) lo = m + 1; else hi = m - 1; }
    return (const zls_entry_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_field(int scope_id, int off, int size, int kind, int audit, const char * what, const IR_t * nd) {
    if (zf_n >= FL_MAX_FIELDS) { fprintf(stderr, "zls: field table overflow (%d)\n", FL_MAX_FIELDS); abort(); }
    zf[zf_n++] = (zls_pfield_t){ scope_id, off, size, (unsigned char)kind, (unsigned char)audit, what, nd };
    if (nd) znb_note(nd, scope_id, off + size);
}
static int zls_fence_widen(const IR_graph_t * g) { return g && g->zframe_pinned_base && g->zframe_graph; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_locals_shifted(IR_e op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_entry(const IR_t * nd, int scope_id, int off) {
    if (ze_n >= FL_MAX_ENTRIES) { fprintf(stderr, "zls: entry table overflow (%d)\n", FL_MAX_ENTRIES); abort(); }
    ze[ze_n] = (zls_entry_t){ nd, scope_id, off, off + (zls_locals_shifted(nd->op) ? 16 : 0), 1, -1 };
    zx[zx_n++] = &ze[ze_n];
    ze_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant_locals(const IR_graph_t * g, const IR_t * nd, int scope_id, int off) {
    switch (nd->op) {
    case IR_TO: case IR_TO_BY:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "to.I counter", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "to.limit", nd); return 1;
    case IR_MAKE_LIST: {
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "list.elem", nd);
        return 0 + nd->n_operands; }
    case IR_SCAN_ENTER:
        zls_field(scope_id, off, 8, ZK_PTR_GC, 0, "scan.leave out3 sigma (the OUTER subject pointer, held here across the whole inner scan body and reloaded into r13 at leave: a collected-heap pointer the walker must relocate -- ZK_RAW until 2026-09-19 read it as dead at safe points, and the walker-reporter measured it live at every collection of a nested scan, CTO-88)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.leave out3 delta", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "scan.leave out3 Delta", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "scan.pad (unused)", nd); return 2;
    case IR_MATCH_BEGIN:
        zls_field(scope_id, off + 4, 4, ZK_RAW, 0, "head.cursor pad (unused upper half of the quad)", nd); zls_field(scope_id, off, 4, ZK_RAW, 0, "head.cursor (ZB-FC-3d granted: the LIVE anchor lives in HEAD's self-pushed 32B rsp cell at [rsp+0] via the op_fc_wbytes window; this FLAT +0 then holds the RELEASE-stashed match START read by IR_MATCH_REPLACE -- same logical offset, window-disambiguated, so REPLACE's template is unchanged both paths)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "head.zeta_mark (BB-OWNED-zeta statement-scope saved the rsp mark() pointer; ZB-FC-3d granted: cell-resident at [rsp+8])", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "head.zls2_mark (retired ALLOC-port era: saved the rsp mark() cursor; released by head's own omega-choke on failure / IR_MATCH_END on success — the RSP-carve twin of head.zeta_mark, widened to a second quad because the first quad's padding is spent.  ZB-FC-3d granted: cell-resident at [rsp+16] holding the PRE-PUSH rsp, so the S10e unwind releases HEAD's cell and every suspended pattern cell in one mov)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "head.end (SN4-REPL: end cursor stashed by IR_MATCH_END when the statement carries a replacement, read by IR_MATCH_REPLACE; ZB-FC-3d: FLAT on both paths -- post-unwind lifetime)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "head.dcap_mark (REG-6 PEND-PROMOTE: α saves live-r12 pend top = this match's MARK; ω/RELEASE truncate r12 from it — the cell [RT_DCAP_TOP] is now seed-source only, prologue-read, never written mid-match; ZK_RAW — points into the base-pinned dcap island, never GC-moved.  ZB-FC-3d: FLAT on both paths -- RELEASE's post-unwind pump reads it after the match dies)", nd); zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "head.incoming____ (DEAD at REG-2 — ___ is no longer the pend cursor, nothing saves or restores here; slot left ALLOCATED v1 so op_off accounting does not ripple, reclaim is a named follow-up.  Re-tenants at REG-3 as the frame-___ era's saved-outer-___ if the wire-header [+24] route changes)", nd); zls_field(scope_id, off + 48, 8, ZK_RAW, 0, "head.sigma_save (PATCTX, Lon directive 2026-07-29: HEAD saves the OUTER match's Σ=r13 at α before rt_match_enter installs the new subject; BOTH exits restore -- head's own omega-choke on failure, release_pump's tail on success -- and re-sync the C-side Σ/Σlen mirror via rt_match_ctx_restore, so nested matches (deferred *F() evaluation, EVAL/CODE, pump-committed *VAR proc bodies that themselves match) are LIFO-sound by construction.  ZK_RAW deliberately: Σ is register-resident today and registers are GC-invisible regardless; when ZHEAP moves strings THIS slot is exactly where a suspended subject becomes a findable root -- retag interior-PTR_GC in that rung.  KNOWN BYPASS, named not hidden: pat_seal kills (ABORT, FENCE-seal) jump straight to fJ and skip the omega-choke, so they skip this restore the same way they already skip the zls unwind and CAS pop -- the ___ match-frame rung is what closes that class)", nd); zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "head.delta_save (PATCTX: outer δ=r14)", nd); zls_field(scope_id, off + 64, 8, ZK_RAW, 0, "head.Delta_save (PATCTX: outer Δ=r15)", nd); zls_field(scope_id, off + 72, 8, ZK_RAW, 0, "head.capgen_save (PATCTX-2: the OUTER match g_cap_gen id, read at alpha before rt_match_enter draws a fresh id from the monotonic well; both exits restore it through rt_match_ctx_restore -- nest1 autopsy: the inner match stamp invalidated the outer SAVE bracket, pop no-opd, top returned 0, R captured [0,end).  Occupies the former pad quad, so the 5-quad grant and every downstream offset are unchanged)", nd); return 5;
    case IR_MATCH_SPAN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "span.cnt/cur", nd); return 1;
    case IR_MATCH_BAL:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "bal.n/start/depth", nd); return 1;
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "break.cnt/cur", nd); return 1;
    case IR_MATCH_FENCE0:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "fence0.watermark (RESERVED — own-extent whack rung; unread today)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "fence0.pad (unused)", nd); return 1;
    case IR_MATCH_FENCE1:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "fence.watermark (α-saved rsp; σ/φ glue restores)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "fence.pad (unused)", nd); return 1;
    case IR_MATCH_ARB:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "arb.cnt/cur (matched-length +0 4B, saved-start +4 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "arb.zls2 activation block ptr (save-slot-in-frame, retired ALLOC-port era: reuses this node's existing pad, same reuse precedent as IR_MATCH_BEGIN.zeta_mark; block itself is a separate RSP-carve allocation, header +0 chains the previous activation's ptr)", nd); return 1;
    case IR_MATCH_REM:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "match.cursor save", nd); return 1;
    case IR_MATCH_DEFER:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "defer.pad (ZS-2 jmp-entry, Lon s58: the fn/frame cell pair is DELETED — the blob is a jmp-entered new activation that self-allocates on rsp with a 32B wire header, so there is nothing to stash and nothing to guard; quad KEPT at 16B so no later node's offset shifts. s137 OVER-SEAL: when IR_t.seal, quad +0 is REPURPOSED as the fence-demarked sync watermark — α stamps rsp there, the γ/ω glues and β bulk-restore it; +8 stays pad)", nd); return 1;
    case IR_MATCH_VALUE:
        return 0;
    case IR_MATCH_TAB: case IR_MATCH_RTAB:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "tab.cursor save (+0 4B r14d saved at α, restored at β; +4 pad)", nd); return 1;
    case IR_MATCH_ARBNO:
        if (IR_LIT(nd).ival == 1) { zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno.owner quad: entry/yield/i/cap (4x4B; SN4-NARY-ARBNO one-node form)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "arbno.COLLECTION ptr (rt_zcol_push-grown per-iteration elements: 16B header {prev_view, saved_delta} + body slot window; the rsp flavor = linked frame chain + explicit count in the header — Lon ruling 2026-07-12, lands at ZB-ITER under FL_ARBNO_STACK)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "arbno.saved_rsp (alpha saves rsp here; exhaust L(2) restores it)", nd); zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "arbno.pad (unused sixth quad of the 3-slot grant)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "arbno.saved_outer____ (W-1c.2: unconditional — chain-beta always saves MATCH_BEGIN ___ into this slot before repointing ___ as element view; exhaust L(2) always restores it; view-restores at sigma/phi re-derive ___ from rsp unconditionally)", nd); return 3; }
        return 0;
    case IR_MATCH_ASSIGN_SAVE:
        zls_field(scope_id, off, 8, ZK_PTR_GC, 0, "capture.stack ws u32[] ([0]=cap, frames from [1]; box α-push/β-pop)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "capture.stack gen(+8,4B)/sp(+12,4B)", nd); return 1;
    case IR_MATCH_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "alt.entry cursor save (+0 4B r14d; +4 4B dead — was alt_i, killed by ALT-FLAT s202 address dispatch)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_CODE, 0, "alt.resume continuation (ALT-FLAT s202: each arm's sigma stub stores its own resume trampoline address via lea rip; beta is one indirect jmp — the alt_i cmp-chain is dead.  Retenants the old dcap-pad quad)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "alt.pad (unused fourth quad of the 2-slot grant)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_CODE, 0, "alt.next-entry continuation (ALT-FLAT s202: alpha and each entry stub store the NEXT arm's entry-stub address; the fail-advance is delta-restore + one indirect jmp — the entry cmp-chain is dead)", nd); return 2;
    case IR_SCAN_SEQUENCE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scanseq.entry δ save (+16 from box base, 4B r14d) + seq_i live-element index (+20, 4B; α=0, na_s ++, na_f --, β=N); the value DESCR is the box result slot at [base]", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scanseq.pad (unused upper quad of the 1-slot grant; registered CEO-820 so the frame map has no unmapped word)", nd); return 1;
    case IR_SCAN_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scanalt.entry δ save (+16 from box base, 4B r14d) + dcap height (+20, 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scanalt.alt_i live-alternative index (+24, 4B; α=0, na_f ++; β dispatches) (+28 pad)", nd); return 2;
    case IR_SCAN:
        if (nd && nd->n_operands > 2 && !nd->operands[1]) { zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.suspend-leave bank: +0 4B inner-δ, +4 4B the inner subject's carried length, which rt_scan_reenter_live takes back at resume because neither the shared scan_saved stack nor the length cache can (icon-scan-env-value-residue slice 3: per-activation env round-trip across suspension — the shared scan_saved stack cannot carry it, +1 leak per exhaustion-through-scan measured)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "scan.suspend-leave live subject (the subject pointer rt_scan_live_subj banks across the suspension and rt_scan_reenter_live takes back at resume: a collected-heap pointer, interior to its block, the walker relocates it -- banked in the RESULT slot until 2026-09-23 with the delta in its tag word, invisible to the walker, cto CTO-154)", nd); return 1; }
        return 0;
    case IR_SCAN_TAB: case IR_SCAN_MOVE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.r14 data-backtrack save", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.pad (unused)", nd); return 1;
    case IR_BOUND:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "bound.saved rsp (Op_Mark: bounded-expression entry frontier; IR_UNMARK restores it, discarding abandoned retained-suspension FC carves — interp.r Op_Unmark rsp=efp-1)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "bound.saved rsp at entry -- the gamma-fence release frontier (rung 9, CEO-690)", nd); if (!zls_fence_widen(g)) return 1; zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "bound.saved B at entry, banked BEFORE rt_pl_disj_open raises it -- the gamma-fence commit target, because a fence that leaves B at this frame's own H pins the frame it just emptied", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "bound.pad (unused)", nd); return 2;
    case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_MATCH: case IR_SCAN_BAL:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.cursor", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.len/counter", nd); return 1;
    case IR_INITIAL:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "initial.pad (unused low half)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "initial.once flag (0->1)", nd); return 1;
    case IR_ITERATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "iterate.index i (alpha=0, beta inc)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "iterate.pad (unused)", nd); return 1;
    case IR_LIMIT:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "limit.counter", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "limit.resumed flag (+24 from box base; beta sets before resuming the generator, alpha consumes and clears it so a FRESH entry zeroes limit.counter -- see bb_limit.cpp)", nd); return 1;
    case IR_REPALT:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "repalt.yielded flag (clear/yield/test)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "repalt.pad (unused)", nd); return 1;
    case IR_REV_ASSIGN: case IR_REV_ASSIGN_VAR:
        zls_field(scope_id, off, 16, ZK_DESCR, 0, "revasg.saved old value (beta restore; LIVE across suspension — GC must trace)", nd); return 1;
    case IR_REV_SWAP:
        zls_field(scope_id, off, 16, ZK_DESCR, 0, "revswap.saved lhs old (beta restore; LIVE across suspension — GC must trace)", nd); zls_field(scope_id, off + 16, 16, ZK_DESCR, 0, "revswap.saved rhs old (beta restore; LIVE across suspension — GC must trace)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "revswap.delta spill (in-scan r14 round-trip)", nd); zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "revswap.Delta spill (in-scan r15, read-only len)", nd); zls_field(scope_id, off + 48, 8, ZK_RAW, 0, "revswap.lhs cell pointer parked across the rhs NV_PTR_fn lookup (global operand only; a raw interior pointer into the name table, never a heap object -- ZK_RAW so GC does not trace it)", nd); zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "revswap.pad (unused eighth quad of the 4-slot grant; registered CEO-820 so the frame map has no unmapped word)", nd); return 4;
    case IR_KW_ICON: case IR_KW_ICON_GEN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "kw.gen counter", nd); return 1;
    case IR_KW_SNOBOL4:
        return 0;
    case IR_DEREF: case IR_ASSIGN_VAR: case IR_RANDOM: case IR_SWAP_VAR:
        return 0;
    case IR_KW_ASSIGN:
    case IR_KW_ASSIGN_SNOBOL4:
        return 0;
    case IR_CREATE:
        zls_field(scope_id, off,      8, ZK_RAW, 0, "coexpr.handle ctx* (malloc'd non-GC — never trace/relocate)", nd);
        zls_field(scope_id, off + 8,  8, ZK_RAW, 0, "coexpr.handle pad (unwritten)", nd);
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
    case IR_LINE_MARK:
    case IR_STATEMENT:
        return 0;
    case IR_GATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "gate.live arm index (+16 from box base; IR_GATE_ARM banks its own position in this gate's operand list at each arm's success, the box beta dispatches on it over compile-time wired ports). NOT A CODE ADDRESS: the stored resume target this slot used to hold died with the two label ops on Lon's order 2026-09-13 (CEO-693, 'There are no labels'), and the field is ZK_RAW rather than ZK_PTR_CODE for that reason -- a collector that sees a code pointer here is reading a small integer. Granted at rung 5 -- both templates had ALWAYS addressed [op_off+16], which is this field, while the kind fell through to default and was granted only its 16-byte result, so the gate write landed 8 bytes past the grant. Harmless with ONE such box live and a core dump with two, because the second box's region began where the first's gate was still being written", nd);
        zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "gate.pad (unused)", nd);
        return 1;
    case IR_DISJUNCTION:
        if (nd->op == IR_DISJUNCTION && nd->n_operands > 0) { zls_field(scope_id, off, 8, ZK_RAW, 0, "disj.alt_i live-alternative index (+16 from box base; nary self-state, MOVE_LABEL-ERAD: α=0, φ-glue ++, β dispatches; value DESCR = the box result slot at [base], option-B per-arm copy in σ-glue) (+24 pad)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "disj.pad (unused)", nd); return 1; }
        zls_field(scope_id, off, 8, ZK_RAW, 0, "gate.live arm index (operand-less disjunction shares the gate box)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "gate.pad (unused)", nd); return 1;
    case IR_CALL_BUILTIN_GEN:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0, "callgen.resume position (alpha=0, runtime writes next start)", nd);
        zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "callgen.pad (unused)", nd);
        return 1 + nd->n_operands;
    case IR_PROC_GEN: case IR_CALL_VALUE:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0, "callgen.act +0 (ZK_RAW: the spine arm writes 0 at alpha and 1 once its epilogue has run, or 2 for the whole life of a call that opened a NON-RESUMABLE callee -- a plain procedure returns once and releases, so beta fails forward on 2 instead of resuming (ceo 2026-09-19, CVSPINE_t in descr.h); at the gamma landing it holds the callee's RETAINED frame base (rax) or 0 when the callee released -- a machine-stack address, never a heap block; measured from bb_call_proc_staged.cpp)", nd);
        zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_PTR_CODE, 0, "callgen.act +8 (WRITTEN, not a pad: the callee's graph beta at the gamma landing, or the saved rsp on the LCO/forwarding arms; read back on beta to resume the callee -- bb_call_proc_staged.cpp)", nd);
        return 1 + nd->n_operands;
    default:
        if (nd->op == IR_CALL || ir_is_call_kind(nd->op)) {
            if (nd->op == IR_CALL_PROC_STAGED) {
                if (!zls_callee_is_gen(nd)) return 0;
                zls_field(scope_id, off, 8, ZK_RAW, 0, "callgen.act +0 (ZK_RAW: the spine arm writes 0 at alpha and 1 once its epilogue has run, or 2 for the whole life of a call that opened a NON-RESUMABLE callee -- a plain procedure returns once and releases, so beta fails forward on 2 instead of resuming (ceo 2026-09-19, CVSPINE_t in descr.h); at the gamma landing it holds the callee's RETAINED frame base (rax) or 0 when the callee released -- a machine-stack address, never a heap block; measured from bb_call_proc_staged.cpp)", nd);
                zls_field(scope_id, off + 8, 8, ZK_PTR_CODE, 0, "callgen.act +8 (WRITTEN, not a pad: the callee's graph beta at the gamma landing, or the saved rsp on the LCO/forwarding arms; read back on beta to resume the callee -- bb_call_proc_staged.cpp)", nd);
                return 1;
            }
            for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
            { const char * cmn = IR_LIT(nd).sval; if (cmn && (!strcmp(cmn, "tab") || !strcmp(cmn, "move"))) {
                zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0, "scan.saved_delta — ICN-BYNAME-CURSOR-RESTORE: a cursor-mover (tab/move, and =s == tab(match(s))) reached by-name through rt_call_arr has no inline bb_scan_tab body, so it also had no saved-δ slot and its β degenerated to a bare jmp ω — the backtrack never restored &pos. This quad is that slot; bb_call_byname_str writes r14 here at α and reloads it in β, mirroring bb_scan_tab's restore-δ-and-FAIL port. Same extra-quad shape as callgen.act above.", nd);
                zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "scan.saved_delta pad (unused)", nd);
                return 1 + nd->n_operands; } }
            return nd->n_operands;
        }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_is_wiring(IR_e op) { return op == IR_GOTO || op == IR_GATE_ARM || op == IR_GOTO_DEFERRED || op == IR_SUCCEED || op == IR_FAIL || op == IR_RETURN || op == IR_SUSPEND || op == IR_CORET || op == IR_COFAIL || op == IR_CUT || op == IR_MATCH_END || op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_STMT_MARK || op == IR_GLIT || op == IR_GCC || op == IR_GALT; }
static int zls_locals_shifted(IR_e op) { return op == IR_BOUND || op == IR_MATCH_BEGIN || op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARB || op == IR_MATCH_BAL || op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_MATCH_ARBNO || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE || op == IR_MATCH_ASSIGN_SAVE || op == IR_SCAN_ENTER || op == IR_INITIAL; }
int fc_arm_member(const IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_cells_on(void) { return 1; }
int fc_cells_active(void) { return fc_cells_on(); }
static int zls_fc_cell(const IR_t * nd) { if (!fc_cells_on()) return 0; if (!nd) return 0; { extern int fc_arm_member(const IR_t *); if (fc_arm_member(nd)) return 0; } switch (nd->op) { case IR_MATCH_SPAN: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_BREAK: case IR_MATCH_BREAKX: case IR_MATCH_BAL: case IR_MATCH_REM: case IR_MATCH_ARB: return 16; default: return 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant(const IR_graph_t * g, const IR_t * nd, int scope_id, int off) {
    if (zls_is_wiring(nd->op)) return 0;
    zls_entry(nd, scope_id, off);
    zls_field(scope_id, off, 16, ZK_DESCR, 0, "result", nd);
    if (zls_fc_cell(nd)) { ze[ze_n - 1].loff = FL_FC_SYNTH; return 1; }
    return 1 + zls_grant_locals(g, nd, scope_id, off + 16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_elide_ok(IR_e op) { return op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_LEN || op == IR_MATCH_LIT || op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_CMP_TEST || op == IR_ASSIGN; }
static int zls_s4_ok(IR_e op) { return op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_BAL || op == IR_MATCH_ALTERNATE || op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const IR_t ** k; int * v; size_t cap; } zls_nidx_t;
static void zls_nidx_build(zls_nidx_t * m, const IR_graph_t * g) {
    size_t cap = 16; while (cap < (size_t)g->n * 2 + 2) cap <<= 1;
    m->k = (const IR_t **)ct_zalloc(cap, sizeof(*m->k)); m->v = (int *)ct_alloc(sizeof(int) * cap); m->cap = cap;
    for (int i = 0; i < g->n; i++) { const IR_t * nd = g->all[i]; if (!nd) continue; size_t h = ((size_t)(uintptr_t)nd >> 4) & (cap - 1);
        while (m->k[h] && m->k[h] != nd) h = (h + 1) & (cap - 1);
        if (!m->k[h]) { m->k[h] = nd; m->v[h] = i; } }
}
static int zls_reuse_index(const zls_nidx_t * m, const IR_t * nd) { size_t h = ((size_t)(uintptr_t)nd >> 4) & (m->cap - 1); while (m->k[h]) { if (m->k[h] == nd) return m->v[h]; h = (h + 1) & (m->cap - 1); } return -1; }
static void zls_mark_value_refs(const IR_graph_t * g, char * live) {
    zls_nidx_t nx; zls_nidx_build(&nx, g);
    for (int k = 0; k < g->n; k++) { const IR_t * c = g->all[k]; if (!c) continue;
        if (c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_FENCE0 || c->op == IR_MATCH_FENCE1 || c->op == IR_GATE_ARM || c->op == IR_GATE) continue;
        for (int j = 0; j < c->n_operands; j++) { const IR_t * p = c->operands[j]; if (!p) continue; if (j == 0 && (c->op == IR_MATCH_ASSIGN_COND || c->op == IR_MATCH_ASSIGN_IMM)) continue; { int i = zls_reuse_index(&nx, p); if (i >= 0) live[i] = 1; } } }
    ct_drop(nx.k); ct_drop(nx.v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant_elide(const IR_graph_t * g, const IR_t * nd, int scope_id, int off, int live, int * scratch_off) {
    if (zls_is_wiring(nd->op)) return 0;
    if (!live && zls_elide_ok(nd->op)) {
        if (*scratch_off < 0) { *scratch_off = off; zls_entry(nd, scope_id, off); ze[ze_n - 1].live = 0; zls_field(scope_id, off, 16, ZK_DESCR, 0, "result (SLOT-ELIDE shared dead-result scratch — every later dead leaf in this graph aliases here)", nd); return 1; }
        zls_entry(nd, scope_id, *scratch_off); ze[ze_n - 1].live = 0; return 0;
    }
    if (!live && zls_s4_ok(nd->op)) { zls_entry(nd, scope_id, off); ze[ze_n - 1].loff = off; ze[ze_n - 1].live = 0; if (zls_fc_cell(nd)) { ze[ze_n - 1].loff = FL_FC_SYNTH; return 0; } return zls_grant_locals(g, nd, scope_id, off); }
    int ei = ze_n; int n = zls_grant(g, nd, scope_id, off); if (ze_n > ei) ze[ei].live = live; return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_result_live(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->live : 1; }
int zls_node_off(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->off : -0x40000000; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_scope_new(int parent, int klass, const char * name) {
    if (zs_n >= FL_MAX_SCOPES) { fprintf(stderr, "zls: scope table overflow (%d)\n", FL_MAX_SCOPES); abort(); }
    zs[zs_n] = (zls_scope_t){ zs_n, parent, klass, name, -1, 0, 0x7fffffff, 0 };
    return zs_n++;
}
int fc_alt_fpmax(const IR_t *);
int fc_alt_extent(const IR_t *, int *, int *);
int fc_geom(const IR_t *, long *);
int emit_patzeta_lookup(const char *, int *);
static int fct_pricing = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fct_defer_susp(const IR_t * nd) {
    if (!nd || (nd->op != IR_MATCH_DEFER) || nd->seal != 2) return -1;
    const char * pn = 0; for (int j = 0; j < nd->n_operands; j++) { const IR_t * o = nd->operands[j]; if (o && o->op == IR_LIT_STRING && IR_LIT(o).sval && !strncmp(IR_LIT(o).sval, "PAT$", 4)) { pn = IR_LIT(o).sval; break; } }
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
        int op = (int)x->op;
        if (op == IR_ASSIGN || op == IR_GOTO || op == IR_GOTO_DEFERRED || (op == IR_DEFINE && ir_define_sr_citizen(x)) ||
            op == IR_MATCH_BEGIN || op == IR_MATCH_END || op == IR_MATCH_REPLACE ||
            op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_STMT_MARK || op == IR_LINE_MARK ||
            op == IR_MATCH_LIT || op == IR_MATCH_LEN || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY ||
            op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_ASSIGN_COND ||
            op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE || op == IR_MATCH_ALTERNATE ||
            op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_BOUND || op == IR_UNMARK || op == IR_CONJUNCTION ||
            op == IR_CUT || op == IR_GATE_ARM || op == IR_GLIT || op == IR_GCC || op == IR_GALT ||
            op == IR_RETURN || (op == IR_DISJUNCTION && x->n_operands == 0) ||
            (op == IR_MATCH_DEFER && x->pat_static && IR_LIT(x).sval && !strncmp(IR_LIT(x).sval, "PATV$", 5))) continue;
        if (op == IR_MATCH_ALTERNATE) {
            int _b = 0, _e = 0;
            if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > j + 1) j = _e - 1; }
            continue;
        }
        rsp += 16;
    }
    return rsp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fct_fp_range(IR_graph_t * g, int k0, int k1) {
    int fp = 0; long fck = 0;
    for (int j = k0; j < k1 && j < g->n; j++) {
        IR_t * x = g->all[j];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) {
            int _b = 0, _e = 0;
            if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > j + 1) j = _e - 1; }
            continue;
        }
        if ((x->op == IR_MATCH_DEFER) && fct_pricing) { int s = fct_defer_susp(x); if (s > 0) fp += s; continue; }
        if (fc_geom(x, &fck)) fp += (int)fck;
    }
    return fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_slot_census(IR_graph_t * g) {
    static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_SLOT_CENSUS"); on = (e && *e == '1') ? 1 : 0; } if (!on || !g) return;
    static long tg = 0, tl = 0, tn = 0;
    extern int zls_result_off(const IR_t *);
    int G = 0, L = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (!nd || zls_is_wiring(nd->op) || zls_result_off(nd) < 0) continue; G++;
        int ref = 0;
        for (int k = 0; k < g->n && !ref; k++) { IR_t * c = g->all[k]; if (!c || c == nd) continue;
            if (c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_ARBNO || c->op == IR_MATCH_FENCE1 || c->op == IR_SCAN_SEQUENCE || c->op == IR_SCAN_ALTERNATE || c->op == IR_REPALT || c->op == IR_GATE_ARM) continue;
            for (int j = 0; j < c->n_operands; j++) if (c->operands[j] == nd) { ref = 1; break; } }
        if (ref) L++; }
    tg += G; tl += L; tn++;
    fprintf(stderr, "[SLOT-CENSUS] g=%p n=%d result_quads=%d value_refd=%d dead_floor=%d rq_bytes %d -> %d | TOTALS graphs=%ld quads=%ld refd=%ld dead=%ld bytes %ld -> %ld\n",
            (void *)g, g->n, G, L, G - L, G * 16, (L + 1) * 16, tn, tg, tl, tg - tl, tg * 16, (tl + tn) * 16);
}
void zls_fct_finalize(IR_graph_t * g, int late);
void fc_vlit_register(const IR_t *); void fc_vread_register(const IR_t *, int); void fc_vbinop_register(const IR_t *); int is_global(const char *); void fc_vwpop_register(const IR_t *, long); void fc_subj_register(const IR_t *); int zc_nofc(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_vvar_ok(const IR_graph_t * g, const IR_t * r) { const char * vn = IR_LIT(r).sval; return vn && vn[0] != '&' && ((is_global(vn) && !graph_has_local(g, vn)) || !strcmp(vn, "write") || !strcmp(vn, "writes")); }
static int fc_vbinop_ok(long long v) { return v == 0 || v == 1 || v == 2 || v == 3 || v == 4 || v == 11 || v == 18; }
int fc_vbinop_active(const IR_t * nd); int fc_vlit_active(const IR_t * nd); int fc_vread_fp(const IR_t * nd); int fc_vdj_active(const IR_t * nd);
static int fc_vunop_ok(const IR_t * nd) { return nd->op == IR_UNOP && nd->n_operands == 1 && ((int)IR_LIT(nd).ival == TT_MNS || (int)IR_LIT(nd).ival == TT_PLS); }
static int g_fcc_gfence = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_call_ok(const IR_t * nd) { (void)nd; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_vtree_scan(const IR_graph_t * g, const IR_t * nd, const IR_t ** post, int * pn, int cap, int depth) {
    if (!nd || depth > 24 || *pn >= cap) return 0;
    if (nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || (nd->op == IR_VAR && fc_vvar_ok(g, nd))) { post[(*pn)++] = nd; return 1; }
    if (fc_vunop_ok(nd) && fc_vtree_scan(g, nd->operands[0], post, pn, cap, depth + 1) && *pn < cap) { post[(*pn)++] = nd; return 1; }
    if (fc_call_ok(nd) && g_fcc_gfence && nd->operands[0] && (nd->operands[0]->op == IR_LIT_INTEGER || nd->operands[0]->op == IR_LIT_STRING || (nd->operands[0]->op == IR_VAR && fc_vvar_ok(g, nd->operands[0]))) && *pn + 2 <= cap) { post[(*pn)++] = nd->operands[0]; post[(*pn)++] = nd; return 1; }
    if (nd->op == IR_BINOP && nd->n_operands == 2 && fc_vbinop_ok((long long)IR_LIT(nd).ival)
        && fc_vtree_scan(g, nd->operands[0], post, pn, cap, depth + 1) && fc_vtree_scan(g, nd->operands[1], post, pn, cap, depth + 1) && *pn < cap) { post[(*pn)++] = nd; return 1; }
    return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zls_pas_display_name(int lvl) {
    static char names[13][24]; static int init = 0;
    if (!init) { for (int i = 0; i < 13; i++) snprintf(names[i], sizeof names[i], "__pas_display_%d", i + 4); init = 1; }
    return (lvl >= 4 && lvl <= 16) ? names[lvl - 4] : (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_build(IR_graph_t * g) {
    if (!g) return;
    { int f = 1; for (int i = 0; i < g->n && f; i++) { IR_t * x = g->all[i]; if (!x) continue; if (x->op == IR_GOTO_DEFERRED || x->op == IR_GATE || x->op == IR_MATCH_BEGIN || x->op == IR_MATCH_DEFER) f = 0; else if ((x->op == IR_DEFINE && ir_define_sr_citizen(x))) { long long v = IR_LIT(x).ival; if (v == 1 || v == 2) f = 0; } else if ((x->op == IR_CALL_BUILTIN || x->op == IR_CALL_SNOBOL4 || x->op == IR_CALL) && IR_LIT(x).sval && (!strcmp(IR_LIT(x).sval, "EVAL") || !strcmp(IR_LIT(x).sval, "CODE"))) f = 0; } g_fcc_gfence = f; }
    for (int vi = 0; vi < g->n; vi++) { IR_t * a = g->all[vi]; if (!(a && a->op == IR_ASSIGN && a->n_operands == 1 && a->operands[0])) continue;
        { const char * vn = IR_LIT(a).sval; if (!(vn && is_global(vn) && !graph_has_local(g, vn))) continue; }
        IR_t * r = a->operands[0];
        if ((r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL || r->op == IR_LIT_CHARSET || r->op == IR_LIT_NAME
             || (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&' && ((is_global(IR_LIT(r).sval) && !graph_has_local(g, IR_LIT(r).sval)) || !strcmp(IR_LIT(r).sval, "write") || !strcmp(IR_LIT(r).sval, "writes"))))
            && r->γ.node == a) { fc_vlit_register(r); fc_vread_register(a, 0); continue; }
        if ((r->op == IR_BINOP || fc_vunop_ok(r) || fc_call_ok(r)) && r->γ.node == a) {
            const IR_t * post[49]; int pn = 0;
            int _ts = fc_vtree_scan(g, r, post, &pn, 49, 0);
            { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg && _ts && pn > 0) fprintf(stderr, "[FCC] tree ts=%d pn=%d tail_is_r=%d\n", _ts, pn, (post[pn-1] == r) ? 1 : 0); }
            if (_ts && post[pn - 1] == r) {
                int ok = 1, L = 0, B = 0;
                for (int i = 0; i + 1 < pn; i++) { const IR_t * gx = post[i]->γ.node; if (gx == post[i + 1]) continue; if (gx && gx->op == IR_DEFINE && gx->γ.node == post[i + 1] && (post[i + 1]->op == IR_CALL || post[i + 1]->op == IR_CALL_PROC_STAGED)) { long long v = IR_LIT(gx).ival; if (!(v == 1 || v == 2 || v == 3)) continue; } ok = 0; break; }
                for (int i = 0; i < pn; i++) { if (post[i]->op == IR_BINOP || post[i]->op == IR_UNOP) B++; else L++; }
                { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg && pn > 1) fprintf(stderr, "[FCC] ok=%d L=%d B=%d p0g=%p p1=%p\n", ok, L, B, (void*)(pn > 1 ? post[0]->γ.node : 0), (void*)(pn > 1 ? post[1] : 0)); if (dbg && pn > 1 && post[0]->γ.node) fprintf(stderr, "[FCC] p0=%p p0op=%d p0g_op=%d r=%p rop=%d\n", (void*)post[0], (int)post[0]->op, (int)post[0]->γ.node->op, (void*)r, (int)r->op); }
                if (ok) {
                    int d = 0;
                    for (int i = 0; i < pn; i++) { const IR_t * x = post[i];
                        if (x->op == IR_BINOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); d -= 1; }
                        else if (x->op == IR_UNOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); }
                        else if (x->op == IR_CALL || x->op == IR_CALL_PROC_STAGED) { fc_call_register(x); fc_vwpop_register(x, (long)d * 16); }
                        else { fc_vlit_register(x); if (x->op == IR_VAR && d > 0) fc_vwpop_register(x, (long)d * 16); d += 1; } }
                    { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) fprintf(stderr, "[FCC] REGISTERED pn=%d\n", pn); }
                    fc_vread_register(a, 0); } } } }
    { static int subj_on = -1; if (subj_on < 0) { const char * b = getenv("SCRIP_SUBJ_CELL"); subj_on = (b && *b == '0') ? 0 : 1; }
      if (subj_on) for (int vi = 0; vi < g->n; vi++) { IR_t * h = g->all[vi]; if (!(h && h->op == IR_MATCH_BEGIN && h->n_operands > 0 && h->operands[0])) continue;
        { static int dyn_on = -1; if (dyn_on < 0) { const char * b = getenv("SCRIP_SUBJ_DYN"); dyn_on = (b && *b == '0') ? 0 : 1; }
          if (!dyn_on) { int dyn = 0; for (int di = 0; di < g->n && !dyn; di++) { const IR_t * d = g->all[di]; if (d && (d->op == IR_MATCH_DEFER || d->op == IR_MATCH_FENCE1)) dyn = 1; } if (dyn) continue; } }
        IR_t * r = h->operands[0];
        { int nc = 0; for (int ci = 0; ci < g->n && nc < 2; ci++) { const IR_t * c = g->all[ci]; if (!c || c == h) continue; for (int oi = 0; oi < c->n_operands; oi++) if (c->operands[oi] == r) { nc++; break; } } if (nc) { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) fprintf(stderr, "[FCS] refuse SOLE-CONSUMER head=%p subj=%p extra_consumers>=%d\n", (void*)h, (void*)r, nc); continue; } }
        { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) fprintf(stderr, "[FCS] head=%p subj=%p subj_op=%d gamma=%p gamma_is_head=%d sval=%s\n", (void*)h, (void*)r, (int)r->op, (void*)r->γ.node, r->γ.node == h ? 1 : 0, IR_LIT(r).sval ? IR_LIT(r).sval : "(null)"); }
        { int _pb1s_adj = 0; { IR_t *_c = r->γ.node; int _hops = 0;
              while (_c && _c != h && _hops < 128) { if (_c->op == IR_VAR) { IR_t *_d = _c->γ.node; if (_d && _d->op == IR_ASSIGN && IR_LIT(_d).sval && !strncmp(IR_LIT(_d).sval, "PATV$", 5)) { _c = _d->γ.node; _hops += 2; continue; } }
                  if (_c->op == IR_ASSIGN && IR_LIT(_c).sval && !strncmp(IR_LIT(_c).sval, "PATV$", 5)) { _c = _c->γ.node; _hops += 1; continue; }
                  break; }
              if (_c == h) _pb1s_adj = 1; }
        if ((r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL
             || (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&' && ((is_global(IR_LIT(r).sval) && !graph_has_local(g, IR_LIT(r).sval)) || !strcmp(IR_LIT(r).sval, "write") || !strcmp(IR_LIT(r).sval, "writes"))))
            && _pb1s_adj) { fc_vlit_register(r); fc_subj_register(r); fc_vread_register(h, 0); continue; } }
        if ((r->op == IR_BINOP || fc_vunop_ok(r)) && r->γ.node == h && !zc_nofc()) {
            const IR_t * post[49]; int pn = 0;
            int _ts = fc_vtree_scan(g, r, post, &pn, 49, 0);
            if (_ts && post[pn - 1] == r) {
                int ok = 1, L = 0, B = 0;
                for (int i = 0; i + 1 < pn; i++) if (post[i]->γ.node != post[i + 1]) { ok = 0; break; }
                for (int i = 0; i < pn; i++) { if (post[i]->op == IR_BINOP || post[i]->op == IR_UNOP) B++; else L++; }
                if (ok) {
                    int d = 0;
                    for (int i = 0; i < pn; i++) { const IR_t * x = post[i];
                        if (x->op == IR_BINOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); d -= 1; }
                        else if (x->op == IR_UNOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); }
                        else { fc_vlit_register(x); if (x->op == IR_VAR && d > 0) fc_vwpop_register(x, (long)d * 16); d += 1; } }
                    fc_vread_register(h, 0); } } } } }
    zls_graph_t * r = zls_g_find(g);
    if (r && r->first_scope >= 0) return;
    if (!r) {
        if (zg_n >= FL_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", FL_MAX_GRAPHS); abort(); }
        zg[zg_n] = (zls_graph_t){ g, (const char *)0, -1, 0, 0, 0, -1, -1, 0, 0, 0, (struct zls_reuse_s *)0, 0, -1, (const IR_t *)0, (const IR_t *)0 }; r = &zg[zg_n]; zg_n++;
    }
    static char anon[FL_MAX_GRAPHS][8]; int gi = (int)(r - zg);
    if (!r->name) { snprintf(anon[gi], sizeof anon[gi], "g%d", gi); r->name = anon[gi]; }
    int root = zls_scope_new(-1, ZSC_FN, r->name);
    r->first_scope = root; r->n_scopes = 1;
    int mfirst[FL_MAX_SCOPES]; int mstart[FL_MAX_SCOPES]; int nl = 0;
    for (int mi = 0; mi < zm_n; mi++) {
        if (zm[mi].g != g) continue;
        int sc = zls_scope_new(root, ZSC_GROUP, zm[mi].name);
        mfirst[nl] = sc; mstart[nl] = zm[mi].start_n; nl++;
        r->n_scopes++;
    }
    char rb_s[8192]; char * rb = (g->n <= (int)sizeof rb_s) ? rb_s : (char *)ct_alloc((size_t)(g->n > 0 ? g->n : 1)); memset(rb, 0, (size_t)(g->n > 0 ? g->n : 1));
    { int hn = 4; while (hn < g->n * 2 + 4) hn <<= 1; IR_t ** hk = (IR_t **)ct_zalloc((size_t)hn, sizeof(IR_t *)); int * hv = (int *)ct_alloc((size_t)hn * sizeof(int)); int * wl = (int *)ct_alloc((size_t)(g->n > 0 ? g->n : 1) * sizeof(int)); int wn = 0;
      for (int i = 0; i < g->n; i++) { IR_t * p = g->all[i]; if (!p) continue; unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h]) h = (h + 1) & (unsigned long)(hn - 1); hk[h] = p; hv[h] = i; }
      if (g->entry) { IR_t * p = g->entry; unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1); if (hk[h]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; } }
      else for (int i = 0; i < g->n; i++) rb[i] = 1;
      if (g->n_alts > 1 && g->alt_entry) for (int _ak = 1; _ak < g->n_alts; _ak++) { IR_t * p = g->alt_entry[_ak]; if (!p) continue;
          unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1);
          if (hk[h] && !rb[hv[h]]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; } }
      int dyn = 0;
      for (int i = 0; i < g->n && !dyn; i++) if (g->all[i] && (g->all[i]->op == IR_GOTO_DEFERRED || g->all[i]->op == IR_GATE || ((g->all[i]->op == IR_CALL || g->all[i]->op == IR_CALL_BUILTIN || g->all[i]->op == IR_CALL_SNOBOL4) && IR_LIT(g->all[i]).sval && strcmp(IR_LIT(g->all[i]).sval, "CODE") == 0))) dyn = 1;
      if (dyn) { for (int i = 0; i < g->n; i++) if (g->all[i]) rb[i] = 1; wn = 0; }
      for (int i = 0; i < g->n; i++) if (g->all[i] && !rb[i] && zls_is_wiring(g->all[i]->op)) { rb[i] = 1; wl[wn++] = i; }
      for (int mi2 = 0; mi2 < nl; mi2++) { int sp0 = mstart[mi2]; int sp1 = (mi2 + 1 < nl) ? mstart[mi2 + 1] : g->n; for (int i = sp0; i >= 0 && i < sp1 && i < g->n; i++) if (g->all[i] && !rb[i]) { rb[i] = 1; wl[wn++] = i; } }
      while (wn > 0) { IR_t * c = g->all[wl[--wn]]; if (!c) continue; for (int j = -2; j < c->n_operands; j++) { IR_t * p = (j == -2) ? c->γ.node : (j == -1) ? c->ω.node : c->operands[j]; if (!p) continue; unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1); if (hk[h] && !rb[hv[h]]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; } } }
      ct_drop(hk); ct_drop(hv); ct_drop(wl); }
    int s0 = (g->nparams > 0 || g->resumable_callable) ? 1 : 0;
    for (int i = 0; !s0 && i < g->n; i++) if (g->all[i] && rb[i] && (g->all[i]->op == IR_RETURN || g->all[i]->op == IR_SUSPEND)) s0 = 1;
    int base = s0 ? 16 + (g->nparams > 0 ? g->nparams * 16 : 0) : 0;
    if (s0) zls_field(root, 0, 16, ZK_DESCR, 0, "return value (IR_RETURN stores the graph result DESCR at frame +0 and the graph gamma loads it into rdi:rsi; zeroed by the prologue, a valid null DESCR until then)", (const IR_t *)0);
    int k = 0;
    r->first_vslot = zv_n;
    for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) {
        if (zv_n >= FL_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", FL_MAX_VSLOTS); abort(); }
        zv[zv_n++] = (zls_vslot_t){ g->pnames[i], 16 + i * 16 }; r->n_vslots++;
        zls_field(root, 16 + i * 16, 16, ZK_DESCR, 0, "param", (const IR_t *)0);
    }
    int cur = 0;
    { static int eon = -1; if (eon < 0) { const char * e = getenv("SCRIP_SLOT_ELIDE"); eon = (e && *e == '0') ? 0 : 1; }
      char lv_sbuf[1024]; char * lv = (g->n <= (int)sizeof lv_sbuf) ? lv_sbuf : (char *)ct_alloc((size_t)g->n);
      memset(lv, 0, (size_t)g->n);
      if (eon) zls_mark_value_refs(g, lv);
      int scratch_off = -1;
      zls_reuse_t * rec = (zls_reuse_t *)ct_alloc(sizeof(zls_reuse_t) * (size_t)g->n);
      int nloop = 0; zls_reuse_plan(g, rec, (int *)0, &nloop);
      int pool_free[64], pool_n = 0, pool_base = -1, dl_base = -1, dl_w = 0;
      for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && rb[i] && zls_reuse_detleaf(nd) && (nl == 0 || i < mstart[0]) && nd->n_operands > dl_w) dl_w = nd->n_operands; }
      { int * order = (int *)ct_alloc(sizeof(int) * (size_t)g->n); int on = 0;
        for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (!nd || !rb[i]) continue; if (nl > 0 && i >= mstart[0]) continue;
            if (eon && lv[i] && rec[i].cls == ZR_CANDIDATE && nd->op != IR_LINE_MARK && !zls_fc_cell(nd) && !fc_vlit_active(nd) && fc_vread_fp(nd) < 0 && !fc_vbinop_active(nd) && !fc_vdj_active(nd)) order[on++] = i; }
        for (int a = 1; a < on; a++) { int v = order[a]; int b = a - 1; while (b >= 0 && rec[order[b]].w > rec[v].w) { order[b + 1] = order[b]; b--; } order[b + 1] = v; }
        for (int a = 0; a < on; a++) { int i = order[a]; int j = zls_direct_slot(g, rec, rb, nl, mstart, i, dl_w); if (j >= 0) { rec[i].direct = j; continue; }
            int p = -1; for (int q = 0; q < pool_n && p < 0; q++) if (pool_free[q] < rec[i].w) p = q;
            if (p < 0 && pool_n < 64) { p = pool_n++; }
            if (p < 0) continue;
            pool_free[p] = rec[i].last; rec[i].pooled = p; }
        ct_drop(order);
        if (pool_n > 0) { pool_base = base + k * 16; for (int q = 0; q < pool_n; q++) zls_field(root, pool_base + q * 16, 16, ZK_DESCR, 0, "result (pooled: straight temps share it by interval colouring on the gamma spine)", (const IR_t *)0); k += pool_n; }
        if (dl_w > 0) { dl_base = base + k * 16; for (int q = 0; q < dl_w; q++) zls_field(root, dl_base + q * 16, 16, ZK_DESCR, 0, "call.argv (pooled: every sealed det leaf of this graph marshals here -- live only between one call's alpha and its gamma; the callee returns once and its beta falls to omega; a straight temp read only by one sealed call's operand j is granted slot j directly and the marshal copies nothing)", (const IR_t *)0); k += dl_w; }
        r->scratch_pool = -1; r->scratch_hit_w = (const IR_t *)0; r->scratch_hit_t = (const IR_t *)0;
        for (int q = 0; eon && nloop == 0 && q < pool_n && r->scratch_pool < 0; q++) { int hit = 0;
            for (int i = 0; i < g->n && !hit; i++) { const IR_t * d = g->all[i]; if (!d || !rb[i] || lv[i] || zls_is_wiring(d->op)) continue; if (!zls_reuse_detleaf(d) && !zls_elide_ok(d->op)) continue;
                if (rec[i].w < 0 || (nl > 0 && i >= mstart[0]) || (!zls_reuse_detleaf(d) && !zls_reuse_straight(d->op))) { hit = 1; r->scratch_hit_w = d; break; }
                int p = 2 * rec[i].w + 1;
                for (int t = 0; t < g->n; t++) if (rec[t].pooled == q && rec[t].direct < 0 && 2 * rec[t].w + 1 <= p && p <= 2 * rec[t].last) { hit = 1; r->scratch_hit_w = d; r->scratch_hit_t = g->all[t]; break; } }
            if (!hit) r->scratch_pool = q; }
        if (r->scratch_pool >= 0) scratch_off = pool_base + r->scratch_pool * 16;
        for (int i = 0; i < g->n; i++) { if (rec[i].direct >= 0) rec[i].pooled = dl_base + rec[i].direct * 16; else if (rec[i].pooled >= 0) rec[i].pooled = pool_base + rec[i].pooled * 16; } }
      for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        while (cur < nl && i >= mstart[cur]) cur++;
        if (!rb[i]) continue;
        int sc = (cur > 0) ? mfirst[cur - 1] : root;
        if (rec[i].pooled >= 0) { zls_entry(nd, sc, rec[i].pooled); if (dl_base >= 0 && zls_reuse_detleaf(nd)) ze[ze_n - 1].aoff = dl_base; continue; }
        if (dl_base >= 0 && zls_reuse_detleaf(nd) && sc == root) {
            if (eon && !lv[i]) {
                if (scratch_off < 0) { scratch_off = base + k * 16; zls_entry(nd, sc, scratch_off); ze[ze_n - 1].live = 0; ze[ze_n - 1].aoff = dl_base; zls_field(sc, scratch_off, 16, ZK_DESCR, 0, "result (SLOT-ELIDE shared dead-result scratch — every later dead leaf in this graph aliases here)", nd); k += 1; continue; }
                zls_entry(nd, sc, scratch_off); ze[ze_n - 1].live = 0; ze[ze_n - 1].aoff = dl_base; continue; }
            zls_entry(nd, sc, base + k * 16); ze[ze_n - 1].live = eon ? lv[i] : 1; ze[ze_n - 1].aoff = dl_base; zls_field(sc, base + k * 16, 16, ZK_DESCR, 0, "result", nd); k += 1; continue; }
        k += eon ? zls_grant_elide(g, nd, sc, base + k * 16, lv[i], &scratch_off) : zls_grant(g, nd, sc, base + k * 16);
      }
      { int nr = 0; for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && !zls_is_wiring(g->all[i]->op)) nr++;
        if (r->reuse) ct_drop(r->reuse);
        r->reuse = (struct zls_reuse_s *)ct_alloc(sizeof(zls_reuse_t) * (size_t)(nr > 0 ? nr : 1)); r->n_reuse = 0;
        for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && !zls_is_wiring(g->all[i]->op)) r->reuse[r->n_reuse++] = rec[i]; }
      ct_drop(rec);
      if (lv != lv_sbuf) ct_drop(lv);
    }
    r->resume_off = -1;
    for (int i = 0; i < g->n; i++) if (g->all[i] && rb[i] && g->all[i]->op == IR_SUSPEND) {
        r->resume_off = base + k * 16;
        zls_field(root, r->resume_off, 8, ZK_PTR_CODE, 0, "gen-proc resume continuation", (const IR_t *)0);
        zls_field(root, r->resume_off + 8, 8, ZK_RAW, 0, "resume.pad (unused)", (const IR_t *)0);
        k += 1; break;
    }
    if (r->resume_off < 0 && g->resumable_callable) {
        r->resume_off = base + k * 16;
        zls_field(root, r->resume_off, 8, ZK_PTR_CODE, 0, "resumable-callable blob β continuation (NCB-2/SZ-1: α-init = tail element's β; esi=1 re-entry dispatches jmp [slot] — the graph-level twin of alt.resume)", (const IR_t *)0);
        zls_field(root, r->resume_off + 8, 8, ZK_RAW, 0, "resume.pad (unused)", (const IR_t *)0);
        k += 1;
    }
    r->locals_off = base + k * 16;
    r->zeta_mark_off = -1;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || !rb[i]) continue;
        const char * vns[2]; int nvn = 0;
        if (nd->op == IR_ASSIGN) vns[nvn++] = IR_LIT(nd).sval;
        else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) vns[nvn++] = IR_LIT(nd->operands[1]).sval;
        else if (nd->op == IR_REV_SWAP) { vns[nvn++] = IR_LIT(nd).sval; if (nd->n_operands > 0 && nd->operands[0]) vns[nvn++] = IR_LIT(nd->operands[0]).sval; }
        else if (nd->op == IR_VAR || nd->op == IR_VAR_REF) vns[nvn++] = IR_LIT(nd).sval;
        for (int q = 0; q < nvn; q++) {
            const char * vn = vns[q];
            if (!vn || vn[0] == '&' || (is_global(vn) && !graph_has_local(g, vn))) continue;
            int have = 0; for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) if (zv[v].name && strcmp(zv[v].name, vn) == 0) { have = 1; break; }
            if (have) continue;
            if (zv_n >= FL_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", FL_MAX_VSLOTS); abort(); }
            zv[zv_n++] = (zls_vslot_t){ vn, base + k * 16 }; r->n_vslots++;
            zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "local", (const IR_t *)0);
            k++;
        }
    }
    if (g->caller_frame && g->lnames && g->nlocals > 0) {
        for (int i = 0; i < g->nlocals; i++) {
            const char * vn = g->lnames[i];
            if (!vn || vn[0] == '&') continue;
            int have = 0; for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) if (zv[v].name && strcmp(zv[v].name, vn) == 0) { have = 1; break; }
            if (have) continue;
            if (zv_n >= FL_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", FL_MAX_VSLOTS); abort(); }
            zv[zv_n++] = (zls_vslot_t){ vn, base + k * 16 }; r->n_vslots++;
            zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "declared-local", (const IR_t *)0);
            k++;
        }
    }
    if (g->zframe_graph && !g->icn_cells_graph && g->decl_level == 3) {
        for (int _pdl = 4; _pdl <= 16; _pdl++) {
            const char * nm = zls_pas_display_name(_pdl);
            if (zv_n >= FL_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", FL_MAX_VSLOTS); abort(); }
            zv[zv_n++] = (zls_vslot_t){ nm, base + k * 16 }; r->n_vslots++;
            zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "pas-display-spill", (const IR_t *)0);
            k++;
        }
    }
    r->nslots = k;
    r->region = base + k * 16;
    for (int f = 0; f < zf_n; f++) {
        int sid = zf[f].scope_id;
        if (sid < root || sid >= zs_n) continue;
        if (zf[f].off < zs[sid].lo_off) zs[sid].lo_off = zf[f].off;
        if (zf[f].off + zf[f].size > zs[sid].hi_off) zs[sid].hi_off = zf[f].off + zf[f].size;
        if (zs[sid].first_field < 0) zs[sid].first_field = f;
        zs[sid].n_fields++;
    }
    qsort(zx, zx_n, sizeof(zls_entry_t *), zx_cmp);
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || !rb[i] || nd->op != IR_MATCH_ARBNO || IR_LIT(nd).ival != 1 || nd->n_operands < 3) continue;
        int i0 = -1, i1 = -1;
        for (int j = 0; j < g->n; j++) { if (g->all[j] == nd->operands[1]) i0 = j; if (g->all[j] == nd->operands[2]) i1 = j; }
        if (i0 < 0 || i1 < 0) { fprintf(stderr, "zls: arbno2 geometry — body bracket operands not found in g->all\n"); abort(); }
        if (i0 > i1) { int t = i0; i0 = i1; i1 = t; }
        int mn = 0x7fffffff, mx = 0; int azq[8]; int anzq = 0;
        for (int j = i0; j <= i1; j++) {
            const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0;
            if (!e) continue;
            if (e->off < mn) mn = e->off;
            if (g->all[j]->op == IR_MATCH_ASSIGN_SAVE) { int co = -1; for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].kind == ZK_PTR_GC) { co = zf[f].off; break; } if (co >= 0) { if (anzq < 8) azq[anzq++] = co; else anzq = 9; } else anzq = 9; }
            for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].off + zf[f].size > mx) mx = zf[f].off + zf[f].size;
        }
        if (za_n >= (int)(sizeof za / sizeof *za)) { fprintf(stderr, "zls: arbno2 geometry table overflow (%d)\n", (int)(sizeof za / sizeof *za)); abort(); }
        if (mn == 0x7fffffff) za[za_n++] = (zls_ageom_t){ nd, 16, 0, {0}, 0 };
        else                  { zls_ageom_t a; a.nd = nd; a.min_off = mn; a.span = mx - mn; a.nzq = anzq > 8 ? 9 : anzq; for (int q = 0; q < (anzq > 8 ? 0 : anzq); q++) a.zq[q] = azq[q]; za[za_n++] = a; }
    }
    if (rb != rb_s) ct_drop(rb);
    zls_fct_finalize(g, 0);
    zls_slot_census(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_fct_finalize(IR_graph_t * g, int late) {
    if (!g) return;
    { extern int fc_geom(const IR_t *, long *); extern void fc_cond_register(const IR_t *, int);
      for (int c = 0; c < fct_n; c++) {
        if (fct[c].fin) continue;
        { int in = 0; for (int j = 0; j < g->n; j++) if (g->all[j] == fct[c].arbno) { in = 1; break; } if (!in) continue; }
        { int hd = 0; for (int j = fct[c].i0; j < fct[c].r1 && j < g->n && !hd; j++) { IR_t * x = g->all[j]; if (x && (x->op == IR_MATCH_DEFER)) hd = 1; }
          if (hd && !late) continue; }
        int b0 = fct[c].b0, b1 = fct[c].b1, i0 = fct[c].i0, ia = fct[c].ia, r1 = fct[c].r1; long k1 = 0;
        int bmn = 0x7fffffff, bmx = 0, rmn = 0x7fffffff, rmx = 0;
        for (int j = b0; j <= b1 && j < g->n; j++) { const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0; if (!e) continue; int _bfmx = 0; for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j]) { if (zf[f].off + zf[f].size > _bfmx) _bfmx = zf[f].off + zf[f].size; if (zf[f].off < bmn) bmn = zf[f].off; } if (_bfmx > bmx) bmx = _bfmx; }
        for (int j = b1 + 1; j < r1 && j < g->n; j++) { const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0; if (!e) continue; int _rfmx = 0; for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j]) { if (zf[f].off + zf[f].size > _rfmx) _rfmx = zf[f].off + zf[f].size; if (zf[f].off < rmn) rmn = zf[f].off; } if (_rfmx > rmx) rmx = _rfmx; }
        int span = (bmn == 0x7fffffff) ? 0 : bmx - bmn;
        int rspan = (rmn == 0x7fffffff) ? 0 : rmx - rmn;
        { int _dok = 1, _dfr = 0;
          for (int j = i0; j < r1 && j < g->n && _dok; j++) { IR_t * x = g->all[j]; if (x && (x->op == IR_MATCH_DEFER)) { _dfr = 1; if (fct_defer_susp(x) <= 0) _dok = 0; } }
          fct[c].dfr = _dfr;
          if (!_dok) { if (getenv("SCRIP_TAIL_DIAG")) fprintf(stderr, "[TAIL-DIAG] finalize refuse: defer target unregistered/non-uniform\n");
                       fct[c].head = 0; fct[c].arbno = 0; continue; } }
        fct_pricing = 1;
        int fpl = fct_rsp_range(g, i0, ia);
        int fpb = fct_fp_range(g, b0, b1 + 1);
        int fpr = fct_fp_range(g, b1 + 1, r1);
        int fpr_rsp = fct_rsp_range(g, b1 + 1, r1);
        (void)k1;
        fct_pricing = 0;
        fct[c].fpl = fpl; fct[c].fpb = fpb; fct[c].fpr = fpr; fct[c].fpr_rsp = fpr_rsp; fct[c].span = span; fct[c].rspan = rspan; fct[c].opsb = (span + rspan + 32 + 16 * fct[c].nw + 15) & ~15; fct[c].fin = 1;
        for (int w = 0; w < fct[c].nw; w++) fc_cond_register(fct[c].wcd[w], fpb + span + rspan + 32 + 16 * w);
      }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_arbno_geom(const IR_t * nd, int * min_off, int * span) {
    for (int i = 0; i < za_n; i++) if (za[i].nd == nd) { if (min_off) *min_off = za[i].min_off; if (span) *span = za[i].span; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_arbno_zq(const IR_t * nd, int * zq, int max) {
    for (int i = 0; i < za_n; i++) if (za[i].nd == nd) { if (za[i].nzq > 8) return 9; int n = za[i].nzq > max ? max : za[i].nzq; for (int q = 0; q < n; q++) zq[q] = za[i].zq[q]; return n; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zc_nofc(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_NOFC"); v = (e && e[0] == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_geom(const IR_t * nd, long * k) {
    if (!nd || !fc_cells_on()) return 0;
    { static int _ac = -1; if (_ac < 0) { const char * e = getenv("SCRIP_ALT_CAP"); _ac = (e && *e == '0') ? 0 : 1; }
      if (fc_arm_member(nd) && !(_ac && nd->op == IR_MATCH_ASSIGN_SAVE && fc_save_active(nd))) return 0; }
    if (nd->op == IR_MATCH_ASSIGN_SAVE && fc_save_active(nd)) { if (k) *k = 16; return 1; }
    if ((nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME || nd->op == IR_VAR) && fc_vlit_active(nd) && (!zc_nofc() || fc_subj_member(nd))) { if (k) *k = 16; return 1; }
    if (nd->op == IR_DISJUNCTION && fc_vdj_active(nd) && IR_LIT(nd).ival > 0 && nd->n_operands > 2 * (int)IR_LIT(nd).ival) { if (k) *k = 32; return 1; }
    if (nd->op == IR_MATCH_ARB)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_SPAN)   { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_TAB)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_RTAB)   { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAK)  { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAKX) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BAL)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_REM)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_SCAN_TAB)     { if (k) *k = 16; return 1; }
    if (nd->op == IR_SCAN_MOVE)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_SCAN_MATCH)   { if (k) *k = 16; return 1; }
    return 0;
}
typedef struct { const IR_t * nd; uint32_t gen; uint32_t fl; int vr; int pe; int cf; int hf; int alt; long vw; const IR_t * cs; } fcn_t;
#define FCN_ARM  0x0001u
#define FCN_VDJ  0x0002u
#define FCN_VLIT 0x0004u
#define FCN_SUBJ 0x0008u
#define FCN_VBIN 0x0010u
#define FCN_CALL 0x0020u
#define FCN_SAVE 0x0040u
#define FCN_VR   0x0080u
#define FCN_VW   0x0100u
#define FCN_PE   0x0200u
#define FCN_CC   0x0400u
#define FCN_HD   0x0800u
#define FCN_ALT  0x1000u
static cv_t g_fcn; static uint32_t g_fcn_gen = 1, g_fcn_n = 0;
static cv_t g_fca;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fc_reg_hw(const char * tbl, int n) { static const char * e = (const char *) 1; if (e == (const char *) 1) e = getenv("SCRIP_FC_REG_HIGHWATER");
    if (e && *e != '0') fprintf(stderr, "FC-REG-HW %-6s %d\n", tbl, n); }
static fcn_t * fcn_probe(cv_t * v, const IR_t * nd) {
    uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 0xff51afd7ed558ccdull) & (v->len - 1);
    for (;;) { fcn_t * t = &CV_AT(*v, fcn_t, h); if (t->gen != g_fcn_gen || t->nd == nd) return t; h = (h + 1) & (v->len - 1); }
}
static const fcn_t * fcn_find(const IR_t * nd, uint32_t fl) { if (!nd || !g_fcn.len) return (const fcn_t *)0; { const fcn_t * t = fcn_probe(&g_fcn, nd); return (t->gen == g_fcn_gen && (t->fl & fl)) ? t : (const fcn_t *)0; } }
static fcn_t * fcn_get(const IR_t * nd) {
    if ((uint64_t)(g_fcn_n + 1) * 2 > g_fcn.len) { uint32_t nc = g_fcn.len ? g_fcn.len * 2 : 256; cv_t o = g_fcn, n = { 0, 0, 0, 0 };
        n.p = ct_zalloc(nc, sizeof(fcn_t)); n.len = nc; n.cap = nc; n.esz = (uint32_t)sizeof(fcn_t); g_fcn = n;
        for (uint32_t i = 0; i < o.len; i++) { fcn_t q = CV_AT(o, fcn_t, i); if (q.gen == g_fcn_gen) *fcn_probe(&g_fcn, q.nd) = q; } }
    fcn_t * t = fcn_probe(&g_fcn, nd);
    if (t->gen != g_fcn_gen) { memset(t, 0, sizeof *t); t->nd = nd; t->gen = g_fcn_gen; g_fcn_n++; fc_reg_hw("node", (int)g_fcn_n); }
    return t;
}
static void fcn_flag(const IR_t * nd, uint32_t fl) { if (nd) fcn_get(nd)->fl |= fl; }
static const int * fca_arms(const IR_t * nd) { const fcn_t * t = fcn_find(nd, FCN_ALT); return t ? (const int *)g_fca.p + t->alt : (const int *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_arm_member_register(const IR_t * nd) { fcn_flag(nd, FCN_ARM); }
int fc_arm_member(const IR_t * nd) { return fcn_find(nd, FCN_ARM) != (const fcn_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_alt_register(const IR_t * nd, int n, const int * fp, const int * ab, const int * ae) {
    if (!nd || n <= 0 || n > 10) return;
    fcn_t * t = fcn_get(nd);
    if (t->fl & FCN_ALT) return;
    if (g_fca.len + 1 + 3 * (uint32_t)n > g_fca.cap) { uint32_t nc = g_fca.cap ? g_fca.cap * 2 : 256; while (nc < g_fca.len + 1 + 3 * (uint32_t)n) nc *= 2;
        int * q = (int *)ct_alloc((size_t)nc * sizeof(int)); if (g_fca.len) memcpy(q, g_fca.p, (size_t)g_fca.len * sizeof(int)); g_fca.p = q; g_fca.cap = nc; g_fca.esz = (uint32_t)sizeof(int); }
    int * a = (int *)g_fca.p + g_fca.len;
    a[0] = n;
    for (int i = 0; i < n; i++) { a[1 + i] = fp[i]; a[1 + n + i] = ab ? ab[i] : -1; a[1 + 2 * n + i] = ae ? ae[i] : -1; }
    t->alt = (int)g_fca.len; t->fl |= FCN_ALT; g_fca.len += 1 + 3 * (uint32_t)n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_extent(const IR_t * nd, int * b, int * e) {
    const int * a = fca_arms(nd); if (!a) return 0;
    { int n = a[0]; if (a[1 + n] < 0 || a[1 + 2 * n + n - 1] < 0) return 0; if (b) *b = a[1 + n]; if (e) *e = a[1 + 2 * n + n - 1]; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_fpmax(const IR_t * nd) {
    const int * a = fca_arms(nd); if (!a) return -1;
    { int m = 0; for (int j = 0; j < a[0]; j++) if (a[1 + j] > m) m = a[1 + j]; return m; }
}
int fc_seq_active(const IR_t * nd) { (void)nd; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vdj_register(const IR_t * nd) { fcn_flag(nd, FCN_VDJ); }
int fc_vdj_active(const IR_t * nd) { return fcn_find(nd, FCN_VDJ) != (const fcn_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vlit_register(const IR_t * nd) { fcn_flag(nd, FCN_VLIT); }
void fc_subj_register(const IR_t * nd) { fcn_flag(nd, FCN_SUBJ); }
int fc_subj_member(const IR_t * nd) { return fcn_find(nd, FCN_SUBJ) != (const fcn_t *)0; }
int fc_vlit_active(const IR_t * nd) { if (!fc_cells_on()) return 0; if (!nd || !(nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME || nd->op == IR_VAR)) return 0; return fcn_find(nd, FCN_VLIT) != (const fcn_t *)0; }
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
int fc_save_active(const IR_t * nd) {
    if (!nd || nd->op != IR_MATCH_ASSIGN_SAVE || !fc_cells_on()) return 0;
    return fcn_find(nd, FCN_SAVE) != (const fcn_t *)0;
}
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
int fc_cond_fp(const IR_t * nd) {
    if (!fc_cells_on()) return -1;
    { const fcn_t * t = fcn_find(nd, FCN_CC); if (!t) return -1; if (t->cs && fc_arm_member(t->cs)) return -1; return t->cf; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_head_register(const IR_t * nd, int fp) { if (!nd || fp < 0) return; { fcn_t * t = fcn_get(nd); if (!(t->fl & FCN_HD)) { t->hf = fp; t->fl |= FCN_HD; } } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_head_fp(const IR_t * nd) {
    if (!fc_cells_on()) return -1;
    { const fcn_t * t = fcn_find(nd, FCN_HD); return t ? t->hf : -1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; return e->loff; }
int zls_result_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->off : -1; }
int zls_act_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; for (int i = 0; i < zf_n; i++) if (zf[i].nd == nd && zf[i].scope_id == e->scope_id && zf[i].what && !strncmp(zf[i].what, "callgen.act +0", 14)) return zf[i].off; return -1; }
int zls_argv_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; return e->aoff >= 0 ? e->aoff : e->off + 16; }
int zls_node_bytes(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return 0;
    int end = e->off; if (g_znb.len) { const znb_slot_t * t = znb_probe(&g_znb, nd, e->scope_id); if (t->gen == g_znb_gen && t->end > end) end = t->end; }
    int b = end - e->off; b = (b + 15) & ~15; return b; }
int zls_scope_of(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->scope_id : -1; }
int zls_g_nslots(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->nslots : -1; }
int zls_g_region(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->region : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_forget_graph_nodes(const IR_graph_t * g) {
    if (!g) return;
    for (int i = 0; i < ze_n; i++) { if (!ze[i].nd) continue;
        for (int k = 0; k < g->n; k++) if (g->all[k] == ze[i].nd) { ze[i].nd = (const IR_t *)0; break; } }
    zx_n = 0;
    for (int i = 0; i < ze_n; i++) if (ze[i].nd) zx[zx_n++] = &ze[i];
    qsort(zx, zx_n, sizeof(zls_entry_t *), zx_cmp);
    { zls_graph_t * r = zls_g_find(g); if (r) { if (r->reuse) ct_drop(r->reuse); *r = (zls_graph_t){ g, r->name, -1, 0, 0, 0, -1, -1, 0, 0, 0, (struct zls_reuse_s *)0, 0, -1, (const IR_t *)0, (const IR_t *)0 }; } }
}
int zls_g_resume(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->resume_off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_resume_by_name(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < zg_n; i++)
        if (zg[i].name && strcmp(zg[i].name, name) == 0)
            return zg[i].resume_off;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_locals(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->locals_off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "gc_frame_map.h"
_Static_assert(GC_LAY_DESCR == ZK_DESCR && GC_LAY_RAW == ZK_RAW && GC_LAY_PTR_GC == ZK_PTR_GC && GC_LAY_PTR_CODE == ZK_PTR_CODE, "a layout quad's kind IS the zls_field kind: the emitter seals zls_field_t.kind unchanged after the map's four quads and the collector reads it back as GC_LAY_*, so the two enumerations must agree by value (ARCH-GC section 6.2h)");
typedef struct { int off; int size; unsigned char kind; } zls_layq_t;
static zls_layq_t zlq[FL_MAX_FIELDS];
static int zls_layq_cmp(const void * a, const void * b) { const zls_layq_t * x = (const zls_layq_t *)a; const zls_layq_t * y = (const zls_layq_t *)b; if (x->off != y->off) return x->off < y->off ? -1 : 1; if (x->size != y->size) return x->size > y->size ? -1 : 1; return (int)x->kind - (int)y->kind; }
int zls_g_layout_q(const IR_graph_t * g, uint64_t * out, int cap, int * gap_bytes, int * conflicts) {
    zls_graph_t * r = g ? zls_g_find(g) : (zls_graph_t *)0; int n = 0, m = 0, gaps = 0, conf = 0, expect = 0;
    if (gap_bytes) *gap_bytes = 0;
    if (conflicts) *conflicts = 0;
    if (!r || r->first_scope < 0) return -1;
    for (int f = 0; f < zf_n; f++) if (zf[f].scope_id >= r->first_scope && zf[f].scope_id < r->first_scope + r->n_scopes && zf[f].size > 0 && zf[f].off >= 0) zlq[n++] = (zls_layq_t){ zf[f].off, zf[f].size, zf[f].kind };
    qsort(zlq, (size_t)n, sizeof zlq[0], zls_layq_cmp);
    for (int i = 0; i < n; i++) {
        int off = zlq[i].off, end = zlq[i].off + zlq[i].size; unsigned kind = zlq[i].kind;
        if (off < expect) { unsigned lk = m > 0 ? GC_LAY_KIND(out[m - 1]) : 255u; if (lk != kind) conf++; if (end <= expect) continue; off = expect; }
        if (off > expect) gaps += off - expect;
        if (m > 0 && GC_LAY_KIND(out[m - 1]) == kind && GC_LAY_OFF(out[m - 1]) + GC_LAY_SIZE(out[m - 1]) == off && GC_LAY_SIZE(out[m - 1]) + (end - off) <= 0xFFF0) out[m - 1] = GC_LAY_Q(GC_LAY_OFF(out[m - 1]), kind, GC_LAY_SIZE(out[m - 1]) + (end - off));
        else { if (m >= cap) return cap + 1; out[m++] = GC_LAY_Q(off, kind, end - off); }
        expect = end;
    }
    if (r->region > expect) gaps += r->region - expect;
    if (gap_bytes) *gap_bytes = gaps;
    if (conflicts) *conflicts = conf;
    return m;
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
static const char * zk_name(int k) { return k == ZK_DESCR ? "DESCR" : k == ZK_RAW ? "RAW" : k == ZK_PTR_GC ? "PTR_GC" : k == ZK_PTR_CODE ? "PTR_CODE" : "?"; }
static const char * zsc_name(int k) { return k == ZSC_FN ? "FN" : k == ZSC_GROUP ? "GROUP" : k == ZSC_ITER ? "ITER" : k == ZSC_PAT ? "PAT" : k == ZSC_COEXPR ? "COEXPR" : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_op_names_a_string(IR_e op) { return op == IR_CALL || op == IR_CALL_BUILTIN || op == IR_CALL_BUILTIN_GEN || op == IR_CALL_ICON || op == IR_CALL_PROC_STAGED || op == IR_CALL_SNOBOL4 || op == IR_CALL_VALUE || op == IR_MATCH_DEFER || op == IR_LIT_STRING || op == IR_VAR || op == IR_VAR_REF; }
static int zls_reuse_straight(IR_e op) { return op == IR_LIT_INTEGER || op == IR_LIT_REAL || op == IR_LIT_STRING || op == IR_VAR || op == IR_VAR_REF || op == IR_BINOP || op == IR_CMP_TEST || op == IR_LINE_MARK; }
static int zls_reuse_detleaf(const IR_t * c) { return c && c->op == IR_CALL && c->seal == IR_SEAL_CALL_DET_LEAF; }
static int zls_direct_slot(const IR_graph_t * g, const zls_reuse_t * rec, const char * rb, int nl, const int * mstart, int i, int dl_w) {
    const IR_t * c = g->all[i];
    if (dl_w <= 0 || rec[i].nread != 1 || rec[i].w < 0 || rec[i].last <= rec[i].w) return -1;
    int rd = -1, j = -1;
    for (int k = 0; k < g->n && rd < 0; k++) { const IR_t * d = g->all[k]; if (!d || !rb[k]) continue; for (int m = 0; m < d->n_operands; m++) if (d->operands[m] == c) { rd = k; j = m; break; } }
    if (rd < 0 || !zls_reuse_detleaf(g->all[rd]) || (nl > 0 && rd >= mstart[0]) || rec[rd].w != rec[i].last || j >= dl_w) return -1;
    int lo = 2 * rec[i].w + 1, hi = 2 * rec[i].last;
    for (int k = 0; k < g->n; k++) { const IR_t * d = g->all[k]; if (!d || !rb[k] || k == i) continue;
        if (k != rd && zls_reuse_detleaf(d) && d->n_operands > j && rec[k].w >= 0 && 2 * rec[k].w >= lo && 2 * rec[k].w <= hi) return -1;
        if (rec[k].direct == j && 2 * rec[k].w + 1 <= hi && 2 * rec[k].last >= lo) return -1; }
    return j;
}
static int zls_reuse_reader_ok(IR_e op) { return zls_reuse_straight(op) || op == IR_CALL || op == IR_CALL_PROC_STAGED; }
static int zls_reuse_dynamic(const IR_graph_t * g) {
    for (int i = 0; i < g->n; i++) { const IR_t * d = g->all[i]; if (!d) continue;
        if (d->op == IR_GOTO_DEFERRED || d->op == IR_GATE || d->op == IR_MATCH_DEFER || d->op == IR_MATCH_FENCE1) return 1;
        if ((d->op == IR_CALL || d->op == IR_CALL_BUILTIN || d->op == IR_CALL_SNOBOL4) && IR_LIT(d).sval && strcmp(IR_LIT(d).sval, "CODE") == 0) return 1; }
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
    int * pos = (int *)ct_alloc(sizeof(int) * (size_t)n); int * cp = (int *)ct_alloc(sizeof(int) * (size_t)n); zls_nidx_t nx; zls_nidx_build(&nx, g);
    for (int i = 0; i < n; i++) { pos[i] = -1; cp[i] = 0; rec[i] = (zls_reuse_t){ g->all[i], ZR_NONE, -1, -1, -1, (const IR_t *)0, 0, -1, -1, -1, -1 }; }
    int step = 0, loop_lo = 0x7fffffff, loop_hi = -1, nloop = 0, opaque = 0;
    opaque |= zls_reuse_walk(&nx, g->entry, pos, &step, &loop_lo, &loop_hi, &nloop);
    if (g->n_alts > 1 && g->alt_entry) for (int k = 1; k < g->n_alts; k++) if (g->alt_entry[k]) opaque |= zls_reuse_walk(&nx, g->alt_entry[k], pos, &step, &loop_lo, &loop_hi, &nloop);
    int dynamic = zls_reuse_dynamic(g) || opaque;
    int ncp = 0;
    for (int i = 0; i < n; i++) { const IR_t * c = g->all[i]; if (!c) continue;
        const IR_t * t = c->ω.node; if (t && !zls_is_wiring(t->op)) { int j = zls_reuse_index(&nx, t); if (j >= 0 && !cp[j]) { cp[j] = 1; ncp++; } }
        if (pos[i] >= 0 && !zls_is_wiring(c->op) && !zls_reuse_straight(c->op) && !zls_reuse_detleaf(c) && !cp[i]) { cp[i] = 1; ncp++; } }
    int * rd_n = (int *)ct_zalloc((size_t)n, sizeof(int)); int * rd_max = (int *)ct_alloc(sizeof(int) * (size_t)n); char * rd_bad = (char *)ct_zalloc((size_t)n, 1); char * rd_bop = (char *)ct_zalloc((size_t)n, 1);
    for (int i = 0; i < n; i++) rd_max[i] = -1;
    int gsz = 1; while (gsz < step) gsz <<= 1; int * gst = (int *)ct_alloc(sizeof(int) * 2 * (size_t)gsz);
    for (int q = 0; q < 2 * gsz; q++) gst[q] = 0x7fffffff;
    for (int q = 0; q < n; q++) if (cp[q] && pos[q] >= 0 && q < gst[gsz + pos[q]]) gst[gsz + pos[q]] = q;
    for (int q = gsz - 1; q >= 1; q--) gst[q] = gst[2 * q] < gst[2 * q + 1] ? gst[2 * q] : gst[2 * q + 1];
    for (int k = 0; k < n; k++) { const IR_t * d = g->all[k]; if (!d) continue;
        for (int j = 0; j < d->n_operands; j++) { int t = d->operands[j] ? zls_reuse_index(&nx, d->operands[j]) : -1; if (t < 0) continue;
            rd_n[t]++; if (pos[k] < 0) rd_bad[t] = 1; else { if (pos[k] > rd_max[t]) rd_max[t] = pos[k]; if (!zls_reuse_reader_ok(d->op)) rd_bop[t] = 1; } } }
    for (int i = 0; i < n; i++) {
        const IR_t * c = g->all[i]; if (!c || zls_is_wiring(c->op)) continue;
        rec[i].w = pos[i]; rec[i].llo = loop_lo; rec[i].lhi = loop_hi;
        if (dynamic) { rec[i].cls = ZR_DYNAMIC; continue; }
        if (pos[i] < 0) { rec[i].cls = ZR_UNPLACED; continue; }
        if (!zls_reuse_straight(c->op) && !zls_reuse_detleaf(c)) { rec[i].cls = ZR_SELF; continue; }
        int ci = zls_reuse_index(&nx, c);
        int last = pos[i] > rd_max[ci] ? pos[i] : rd_max[ci], badreader = rd_bad[ci], badop = rd_bop[ci], nread = rd_n[ci];
        rec[i].last = last; rec[i].nread = nread;
        if (badreader) { rec[i].cls = ZR_UNPLACED; continue; }
        int guard = -1;
        { int lo = pos[i] + 1 + gsz, hi = last + gsz, best = 0x7fffffff;
          for (; lo < hi; lo >>= 1, hi >>= 1) { if (lo & 1) { if (gst[lo] < best) best = gst[lo]; lo++; } if (hi & 1) { hi--; if (gst[hi] < best) best = gst[hi]; } }
          if (best != 0x7fffffff) guard = best; }
        if (guard >= 0) { rec[i].cls = ZR_GUARD; rec[i].gpos = pos[guard]; rec[i].guard = g->all[guard]; continue; }
        if (nloop && pos[i] < loop_lo && last >= loop_lo) { rec[i].cls = ZR_LOOP; continue; }
        if (badop) { rec[i].cls = ZR_READER; continue; }
        rec[i].cls = ZR_CANDIDATE;
    }
    ct_drop(pos); ct_drop(cp); ct_drop(rd_n); ct_drop(rd_max); ct_drop(rd_bad); ct_drop(rd_bop); ct_drop(nx.k); ct_drop(nx.v); ct_drop(gst);
    if (ncp_out) *ncp_out = ncp; if (nloop_out) *nloop_out = nloop;
}
static const zls_reuse_t * zls_reuse_find(const zls_graph_t * r, const IR_t * nd) { for (int i = 0; i < r->n_reuse; i++) if (r->reuse[i].nd == nd) return &r->reuse[i]; return (const zls_reuse_t *)0; }
static void zls_reuse_dump(FILE * fp, const zls_graph_t * r) {
    const IR_graph_t * g = r->g;
    if (!g || g->n <= 0 || !g->all) return;
    int n = g->n;
    int * lo = (int *)ct_alloc(sizeof(int) * (size_t)n); int * hi = (int *)ct_alloc(sizeof(int) * (size_t)n);
    for (int i = 0; i < n; i++) { lo[i] = -1; hi[i] = -1; }
    int results = 0, cand = 0, pdet = 0, pself = 0, pguard = 0, punpl = 0, ploop = 0, pdyn = 0, preader = 0, elided = 0, placed = 0, boxes = 0, pooled = 0, direct = 0, ncp = 0, nloop = 0, pool_slots = 0;
    { int seen[64]; int sn = 0;
      for (int i = 0; i < n; i++) {
        const IR_t * c = g->all[i]; if (!c || zls_is_wiring(c->op)) continue;
        const zls_reuse_t * q = zls_reuse_find(r, c); if (!q) continue;
        boxes++; if (q->w >= 0) placed++;
        if (q->pooled >= 0 && q->direct < 0) { int dup = 0; for (int t = 0; t < sn; t++) if (seen[t] == q->pooled) dup = 1; if (!dup && sn < 64) seen[sn++] = q->pooled; }
        if (q->direct >= 0) direct++;
      }
      pool_slots = sn; }
    for (int i = 0; i < n; i++) {
        const IR_t * c = g->all[i]; if (!c || zls_is_wiring(c->op)) continue;
        const zls_entry_t * e = zx_find(c); if (!e || e->scope_id < r->first_scope || e->scope_id >= r->first_scope + r->n_scopes) continue;
        const zls_reuse_t * q = zls_reuse_find(r, c); if (!q) continue;
        results++;
        const char * on = bb_op_name(c->op); if (!on) on = "?";
        char dls[160]; dls[0] = 0; if (zls_reuse_detleaf(c)) { pdet++; if (c->n_operands == 0) snprintf(dls, sizeof dls, "  det leaf %s: no beta, argv dead at gamma, no argv", IR_LIT(c).sval ? IR_LIT(c).sval : "?"); else snprintf(dls, sizeof dls, "  det leaf %s: no beta, argv dead at gamma, argv@+%d%s", IR_LIT(c).sval ? IR_LIT(c).sval : "?", zls_argv_off(c), e->aoff >= 0 ? " pooled" : " own"); }
        { int shared = 0; for (int t = 0; t < ze_n; t++) if (ze[t].off == e->off && ze[t].scope_id >= r->first_scope && ze[t].scope_id < r->first_scope + r->n_scopes && ze[t].nd != c) shared = 1;
          if (shared && !e->live) { elided++; fprintf(fp, ";     reuse +%-5d %-18s w=%-4d ELIDED shared dead-result scratch: its gamma writes here at %d%s\n", e->off, on, q->w, 2 * q->w + 1, dls); continue; } }
        switch (q->cls) {
        case ZR_DYNAMIC: pdyn++; fprintf(fp, ";     reuse +%-5d %-18s PINNED dynamic entry into this graph%s\n", e->off, on, dls); continue;
        case ZR_UNPLACED: punpl++; fprintf(fp, ";     reuse +%-5d %-18s w=%-4d PINNED unplaced: not on the gamma spine, or a reader is not%s\n", e->off, on, q->w, dls); continue;
        case ZR_SELF: pself++; fprintf(fp, ";     reuse +%-5d %-18s w=%-4d PINNED self: beta-capable box, its result may be re-read on resume\n", e->off, on, q->w); continue;
        case ZR_GUARD: { pguard++; const char * gn = q->guard ? bb_op_name(q->guard->op) : "?"; const char * gs = (q->guard && zls_op_names_a_string(q->guard->op) && IR_LIT(q->guard).sval) ? IR_LIT(q->guard).sval : "";
            fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d PINNED guard: box @%d %s%s%s can recede between the write and the read%s\n", e->off, on, q->w, q->last, q->gpos, gn ? gn : "?", gs[0] ? " " : "", gs, dls); continue; }
        case ZR_LOOP: ploop++; fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d PINNED loop: written before a gamma back-edge span [%d..%d] that re-reads it%s\n", e->off, on, q->w, q->last, q->llo, q->lhi, dls); continue;
        case ZR_READER: preader++; fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d PINNED reader: a reader's template is not verified to read operands on alpha only%s\n", e->off, on, q->w, q->last, dls); continue;
        default: break; }
        cand++; if (q->pooled >= 0) { if (q->direct < 0) pooled++; } else { lo[i] = q->w; hi[i] = q->last; }
        { char ds[64]; if (q->direct >= 0) snprintf(ds, sizeof ds, " direct: argv block slot %d, the marshal copies nothing", q->direct); else snprintf(ds, sizeof ds, "%s", q->pooled >= 0 ? " pooled" : " unpooled");
          fprintf(fp, ";     reuse +%-5d %-18s w=%-4d r=%-4d CANDIDATE reads=%d%s%s%s\n", e->off, on, q->w, q->last, q->nread, e->live ? "" : " dead-result", ds, dls); }
    }
    int packed = 0;
    for (int i = 0; i < n; i++) if (lo[i] >= 0) for (int t = lo[i]; t <= hi[i]; t++) { int depth = 0; for (int k = 0; k < n; k++) if (lo[k] >= 0 && lo[k] <= t && hi[k] >= t) depth++; if (depth > packed) packed = depth; }
    { const zls_reuse_t * q0 = (r->n_reuse > 0) ? &r->reuse[0] : (const zls_reuse_t *)0; if (q0 && q0->lhi >= 0) nloop = 1; }
    for (int i = 0; i < r->n_reuse; i++) if (r->reuse[i].cls == ZR_SELF || r->reuse[i].cls == ZR_GUARD) ncp++;
    int predicted = r->region - 16 * ((cand - pooled - direct) - packed > 0 ? (cand - pooled - direct) - packed : 0);
    char scr[200]; if (r->scratch_pool >= 0) snprintf(scr, sizeof scr, "overlay(pool slot %d: no dead write lands inside a pooled interval)", r->scratch_pool);
    else if (r->scratch_hit_w && r->scratch_hit_t) { const zls_reuse_t * qw = zls_reuse_find(r, r->scratch_hit_w), * qt = zls_reuse_find(r, r->scratch_hit_t); snprintf(scr, sizeof scr, "own(REFUSED: %s dead write at %d lands inside %s [%d..%d] on every pool slot)", bb_op_name(r->scratch_hit_w->op), qw ? 2 * qw->w + 1 : -1, bb_op_name(r->scratch_hit_t->op), qt ? 2 * qt->w + 1 : -1, qt ? 2 * qt->last : -1); }
    else if (r->scratch_hit_w) snprintf(scr, sizeof scr, "own(REFUSED: %s is a dead writer whose position or scope the plan cannot bound)", bb_op_name(r->scratch_hit_w->op));
    else snprintf(scr, sizeof scr, "%s", elided ? (pool_slots ? "own(loop: a back-edge re-runs the dead writers)" : "own(no pool slot)") : "none");
    fprintf(fp, ";   reuse '%s' results=%d candidates=%d pooled=%d direct=%d pool_slots=%d pinned=%d (self=%d guard=%d unplaced=%d loop=%d dynamic=%d reader=%d detleaf=%d) elided=%d spine=%d/%d betacapable=%d loops=%d packed_min=%d predicted_region_end=%d scratch=%s\n",
            r->name ? r->name : "?", results, cand, pooled, direct, pool_slots, pself + pguard + punpl + ploop + pdyn + preader, pself, pguard, punpl, ploop, pdyn, preader, pdet, elided, placed, boxes, ncp, nloop, packed, predicted, scr);
    ct_drop(lo); ct_drop(hi);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_dump(FILE * fp) {
    static int plant = -1; if (plant < 0) { const char * e = getenv("SCRIP_TEST_PLANT_ZLS_HOLE"); plant = (e && *e == '1') ? 1 : 0; }
                fprintf(fp, "; FRAME LAYOUT (per-activation, typed)\n");
    fprintf(fp, "; kinds: DESCR = 16B t.p pair (GC traces payload) | RAW = int/cursor/counter (GC skips) | PTR_GC = heap pointer (GC traces+fixes) | PTR_CODE = continuation (GC skips, never relocates)\n");
    fprintf(fp, "; (audit) = kind provisional pending template audit — 2026-07-05 burndown: all shipped grants template-verified, audit=0; any NEW grant lands audit=1 until verified\n");
    for (int i = 0; i < zg_n; i++) {
        zls_graph_t * r = &zg[i]; int planted = 0;
        if (r->first_scope < 0) continue;
        fprintf(fp, "; graph %d '%s' — slots=%d region_end=%d resume=%d vslots=%d scopes=%d\n", i, r->name ? r->name : "?", r->nslots, r->region, r->resume_off, r->n_vslots, r->n_scopes);
        for (int s = r->first_scope; s < r->first_scope + r->n_scopes; s++) {
            if (zs[s].n_fields == 0 && zs[s].klass == ZSC_GROUP) { fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d (no frame fields)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent); continue; }
            fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d [%d..%d)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent, zs[s].n_fields ? zs[s].lo_off : 0, zs[s].n_fields ? zs[s].hi_off : 0);
            for (int f = 0; f < zf_n; f++) if (zf[f].scope_id == s) {
                if (plant && !planted) { planted = 1; continue; }
                const char * on = "-"; char onb[16];
                if (zf[f].nd) { on = bb_op_name(zf[f].nd->op); if (!on) { snprintf(onb, sizeof onb, "op%d", (int)zf[f].nd->op); on = onb; } }
                fprintf(fp, ";     +%-5d %-3d %-8s %-36s %s%s\n", zf[f].off, zf[f].size, zk_name(zf[f].kind), zf[f].what ? zf[f].what : "", on, zf[f].audit ? "  (audit)" : "");
            }
        }
        for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) fprintf(fp, ";   vslot +%-5d 16  DESCR    %s\n", zv[v].off, zv[v].name ? zv[v].name : "?");        zls_reuse_dump(fp, r);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_tail_candidate(const IR_t * head, const IR_t * arbno, int i0, int ia, int b0, int b1, int r1) {
    if (!head || !arbno || fct_n >= 64) return;
    fct[fct_n].head = head; fct[fct_n].arbno = arbno; fct[fct_n].i0 = i0; fct[fct_n].ia = ia; fct[fct_n].b0 = b0; fct[fct_n].b1 = b1; fct[fct_n].r1 = r1;
    fct[fct_n].fpl = 0; fct[fct_n].fpb = 0; fct[fct_n].fpr = 0; fct[fct_n].fpr_rsp = 0; fct[fct_n].span = 0; fct[fct_n].rspan = 0; fct[fct_n].opsb = 0; fct[fct_n].fin = 0; fct[fct_n].nw = 0; fct_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_tail_wrap(const IR_t * arbno, const IR_t * save, const IR_t * cond) {
    if (!arbno || !save || !cond) return;
    for (int i = fct_n - 1; i >= 0; i--) if (fct[i].arbno == arbno) { if (fct[i].nw < 4) { fct[i].wsv[fct[i].nw] = save; fct[i].wcd[fct[i].nw] = cond; fct[i].nw++; } return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_ncap(const IR_t * nd) {
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) return fct[i].nw;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_arbno(const IR_t * nd, int * fpb, int * fpl, int * opsb, int * hdrb) {
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) { if (fpb) *fpb = fct[i].fpb; if (fpl) *fpl = fct[i].fpl; if (opsb) *opsb = fct[i].opsb; if (hdrb) *hdrb = fct[i].span + fct[i].rspan; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_fpr_rsp(const IR_t * nd) {
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) return fct[i].fpr_rsp;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_dfr(const IR_t * nd) {
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) return fct[i].dfr;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_release(const IR_t * head, int * brdisp) {
    for (int i = 0; i < fct_n; i++) if (fct[i].head == head && fct[i].fin) { if (brdisp) *brdisp = fct[i].fpr + fct[i].fpb + fct[i].span + fct[i].rspan + 16; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_head(const IR_t * head) {
    for (int i = 0; i < fct_n; i++) if (fct[i].head == head) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_tail_defer_susp_g(IR_graph_t * g, const IR_t * nd) {
    if (!g || !nd || (nd->op != IR_MATCH_DEFER)) return -1;
    for (int i = 0; i < fct_n; i++) if (fct[i].fin && fct[i].arbno) {
        int in = 0; for (int j = 0; j < g->n; j++) if (g->all[j] == fct[i].arbno) { in = 1; break; }
        if (!in) continue;
        for (int j = fct[i].i0; j < fct[i].r1 && j < g->n; j++) if (g->all[j] == nd) return fct_defer_susp(nd);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fl_derive_tier(IR_graph_t * g) {
    if (!g || !g->all) return;
    int window = 0, callee = 0, statements = 0, resumed = 0, matchers = 0, others = 0;
    for (int i = 0; i < g->n; i++) { const IR_t * c = g->all[i]; if (!c) continue;
        { int o = (int)c->op; if (o >= IR_MATCH && o <= IR_MATCH_VALUE) matchers++; else if (o != IR_SUCCEED && o != IR_FAIL && o != IR_GOTO && o != IR_LIT_STRING && o != IR_LIT_INTEGER && o != IR_LIT_REAL && o != IR_LIT_CHARSET && o != IR_LIT_NAME) others++; }
        switch ((int)c->op) {
            case IR_STATEMENT: case IR_STATEMENT_BEGIN: case IR_STATEMENT_END: case IR_STMT_MARK: case IR_DEFINE: case IR_GOTO_DEFERRED: case IR_CALL_SNOBOL4: case IR_KW_SNOBOL4: case IR_KW_ASSIGN_SNOBOL4: case IR_DTP_ASSIGN: statements = 1; break;
            case IR_MATCH_BEGIN: case IR_MATCH_END: case IR_MATCH_LIT: case IR_MATCH_LEN: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_SPAN: case IR_MATCH_BREAK: case IR_MATCH_BREAKX: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_REM: case IR_MATCH_ARB: case IR_MATCH_BAL: case IR_MATCH_ATP: case IR_MATCH_LAMBDA: case IR_MATCH_RETRY: case IR_MATCH_REPLACE: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_IMM: case IR_MATCH_ASSIGN_SAVE: case IR_MATCH_SPAN_VAR: statements = 1; break;
            case IR_PATTERN_ALT: case IR_PATTERN_CAT: case IR_PATTERN_CAPTURE: case IR_PATTERN_DEFER: window = 1; break;
            case IR_SUSPEND: case IR_PROC_GEN: case IR_CREATE: case IR_ACTIVATE: case IR_CORET: case IR_COFAIL: resumed = 1; window = 1; break;
            case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_SEQUENCE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_MANY: case IR_SCAN_ANY: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE: case IR_SCAN_TAB: case IR_SCAN_POS:
            case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_ITERATE: case IR_DISJUNCTION: case IR_GALT: case IR_CUT:
            case IR_CALL_BUILTIN_GEN: case IR_KW_ICON_GEN: case IR_CALL_VALUE: case IR_REV_ASSIGN: case IR_REV_ASSIGN_VAR: case IR_REV_SWAP:
            case IR_MATCH_FENCE1: case IR_MATCH_FENCE0: case IR_MATCH_ABORT: case IR_MATCH_ARBNO: case IR_MATCH_CALLOUT: case IR_MATCH_VALUE: case IR_MATCH_DEFER: case IR_MATCH_ALTERNATE: window = 1; break;
            case IR_CALL: case IR_CALL_ICON: case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_GATE: case IR_MATCH: callee = 1; break;
            default: break; } }
    if (g->icn_cells_graph) { g->zframe_pinned_base = (window || callee) ? 1 : 0; return; }
    if (statements || (matchers > 0 && others == 0)) return;
    if (g->root_graph) { g->zframe_graph = 1; g->zframe_pinned_base = 1; return; }
    int host = (window || callee) ? 1 : 0;
    g->zframe_pinned_base = (resumed || g->resumable_callable) ? 1 : 0; g->zframe_graph = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_tables_reset(void) {
    fct_n = 0; g_fcn_gen++; g_fcn_n = 0; g_fca.len = 0;
}
int fc_frameless_fpr_rsp(const IR_t * nd) { if (!nd) return 0; { long _fk = 0; return !fc_geom(nd, &_fk); } }
static struct { const char * name; int fb; int fp; int uni; } pz[512];
static int pz_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_patzeta_register(const char * name, int frame_bytes, int fp_total, int uniform) {
    if (!name || pz_n >= (int)(sizeof pz / sizeof *pz)) return;
    for (int i = 0; i < pz_n; i++) if (!strcmp(pz[i].name, name)) { pz[i].fb = frame_bytes; pz[i].fp = fp_total; pz[i].uni = uniform; return; }
    pz[pz_n].name = name; pz[pz_n].fb = frame_bytes; pz[pz_n].fp = fp_total; pz[pz_n].uni = uniform; pz_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_patzeta_frame_reserve(const char * name, int * bytes) {
    if (!name) return 0;
    for (int i = 0; i < pz_n; i++) if (!strcmp(pz[i].name, name)) { if (bytes) *bytes = (((FLAT_FRAME_ALLOWANCE + pz[i].fb + 15) & ~15) + pz[i].fp); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_patzeta_lookup(const char * name, int * susp) {
    if (!name) return 0;
    for (int i = 0; i < pz_n; i++) if (!strcmp(pz[i].name, name)) { if (susp) *susp = (((FLAT_FRAME_ALLOWANCE + pz[i].fb + 15) & ~15) + pz[i].fp); return pz[i].uni; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_fp_total(IR_graph_t * g) { return g ? fct_fp_range(g, 0, g->n) : 0; }
int zls_node_has_fields(const IR_t * nd) { if (!nd) return 0; for (int f = 0; f < zf_n; f++) if (zf[f].nd == nd) return 1; return 0; }
long zw_node_k(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; if (!e) return 0; long b = (long)zls_node_bytes(nd); if (!e->live && b <= 16) return 0; return b; }
static int zw_nid_listed_c(const char * e, int nid) { if (!e || !*e) return 0; { const char * p = e; while (*p) { long v = strtol(p, (char **)&p, 10); if ((int)v == nid) return 1; while (*p && *p != ',') p++; if (*p) p++; } } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long zw_carve_k(const IR_t * nd) {
    static int _ba = -1, _all = -1; static const char * _bo; static const char * _bs;
    extern int bb_node_id(IR_t *); long _d, _k; int _spine;
    if (_ba < 0) { const char * e = getenv("SCRIP_BB_ALLOC"); _ba = (e && *e == '0') ? 0 : 1; _bo = getenv("SCRIP_BB_ONLY"); _bs = getenv("SCRIP_BB_SKIP"); { const char * a = getenv("SCRIP_BB_ALLOC_ALL"); _all = (a && *a == '0') ? 0 : 1; }    }
    if (!_ba || !nd) return 0;
    _spine = (nd->op == IR_BINOP || nd->op == IR_ASSIGN || nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME || nd->op == IR_VAR || nd->op == IR_CMP_TEST || nd->op == IR_COERCE_NUMERIC || nd->op == IR_IDENT || nd->op == IR_DIFFER);
    if (_spine) return 0;
    if (!_all && ((nd->op == IR_DEFINE && ir_define_sr_citizen(nd)) || ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_GOTO_DEFERRED || nd->op == IR_GLIT || nd->op == IR_GCC || nd->op == IR_GALT)) return 0;
    if (fc_geom(nd, &_d)) return 0;
    _k = zw_node_k(nd); if (_k <= 0) return 0;
    if ((_bo && *_bo) || (_bs && *_bs)) { int _nid = bb_node_id((IR_t *)nd); if (_bo && *_bo && !zw_nid_listed_c(_bo, _nid)) return 0; if (zw_nid_listed_c(_bs, _nid)) return 0; }
    return _k;
}
#include <stdio.h>
#include <stdlib.h>
static long zop_g_total = 0, zop_g_mixed = 0, zop_hist[16];
void zop_audit_report(void);
void zop_audit_graph_close(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zop_audit_graph_close(void) {
    extern int zop_audit_seen(void); extern void zop_audit_seen_clear(void); int m, frames;
    { static int reg = 0; if (!reg) { reg = 1; atexit(zop_audit_report); } }
    m = zop_audit_seen(); if (m == 0) return; zop_audit_seen_clear();
    frames = ((m >> 1) & 1) + ((m >> 3) & 1) + ((m >> 4) & 1);
    zop_g_total++; if (frames > 1) zop_g_mixed++; zop_hist[(m >> 1) & 15]++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zop_audit_report(void) {
    zop_audit_graph_close();
    if (!getenv("SCRIP_ZOP_AUDIT")) return;
    fprintf(stderr, "[ZOP] graphs=%ld mixed=%ld\n", zop_g_total, zop_g_mixed);
    { int k; const char * nm[16] = {"-","isle","cell","isle+cell","rsp","isle+___","cell+___","isle+cell+___","rsp","isle+rsp","cell+rsp","isle+cell+rsp","___+rsp","+","cell+___+rsp","all"};
      for (k = 0; k < 16; k++) if (zop_hist[k]) fprintf(stderr, "[ZOP]   %-18s %ld\n", nm[k], zop_hist[k]); }
}
int zls_g_first_scope(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->first_scope : -1; }
