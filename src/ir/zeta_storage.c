#include <string.h>
#include <stdlib.h>
#include "zeta_storage.h"
#include "zeta_choices.h"
#include "ast.h"
extern const char * bb_op_name(IR_e k);
extern int is_global(const char *);
extern int rt_proc_is_registered(const char *);
extern int rt_proc_is_generator(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_callee_is_gen(const IR_t * nd) { const char * fn = IR_LIT(nd).sval; return fn && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn); }
#define ZLS_MAX_ENTRIES 65536
#define ZLS_FC_SYNTH    0x7F000
#define ZLS_MAX_FIELDS  131072
#define ZLS_MAX_SCOPES  4096
#define ZLS_MAX_GRAPHS  4096
#define ZLS_MAX_VSLOTS  4096
#define ZLS_MAX_MARKS   65536
typedef struct { const IR_t * nd; int scope_id; int off; int loff; int live; } zls_entry_t;
typedef struct { int scope_id; int off; int size; unsigned char kind; unsigned char audit; const char * what; const IR_t * nd; } zls_pfield_t;
typedef struct { const char * name; int off; } zls_vslot_t;
typedef struct { const IR_graph_t * g; const char * name; int start_n; const IR_t * anchor; } zls_mark_t;
typedef struct { const IR_graph_t * g; const char * name; int first_scope; int n_scopes; int nslots; int region; int resume_off; int zeta_mark_off; int locals_off; int first_vslot; int n_vslots; int pl_trail_mark_off; } zls_graph_t;
static zls_entry_t  ze[ZLS_MAX_ENTRIES];  static int ze_n = 0;
static zls_pfield_t zf[ZLS_MAX_FIELDS];   static int zf_n = 0;
static zls_scope_t  zs[ZLS_MAX_SCOPES];   static int zs_n = 0;
static zls_graph_t  zg[ZLS_MAX_GRAPHS];   static int zg_n = 0;
static zls_vslot_t  zv[ZLS_MAX_VSLOTS];   static int zv_n = 0;
static zls_mark_t   zm[ZLS_MAX_MARKS];    static int zm_n = 0;
static zls_entry_t * zx[ZLS_MAX_ENTRIES]; static int zx_n = 0;
typedef struct { const IR_t * nd; int min_off; int span; int zq[8]; int nzq; } zls_ageom_t;
static zls_ageom_t  za[1024];             static int za_n = 0;
static struct { const IR_t * head; const IR_t * arbno; int i0; int ia; int b0; int b1; int r1; int fpl; int fpb; int fpr; int fpr_rsp; int span; int rspan; int opsb; int fin; int dfr; const IR_t * wsv[4]; const IR_t * wcd[4]; int nw; } fct[64];
static int fct_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_reset(void) { ze_n = 0; zf_n = 0; zs_n = 0; zg_n = 0; zv_n = 0; zm_n = 0; zx_n = 0; za_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    if (zm_n >= ZLS_MAX_MARKS) { fprintf(stderr, "zls: mark table overflow (%d)\n", ZLS_MAX_MARKS); abort(); }
    zm[zm_n++] = (zls_mark_t){ g, name, g->n, (const IR_t *)0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark_anchor(const IR_graph_t * g, const char * name, const IR_t * anchor) {
    if (!g || !name) return;
    if (zm_n >= ZLS_MAX_MARKS) { fprintf(stderr, "zls: mark table overflow (%d)\n", ZLS_MAX_MARKS); abort(); }
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
    if (zg_n >= ZLS_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", ZLS_MAX_GRAPHS); abort(); }
    zg[zg_n] = (zls_graph_t){ g, name, -1, 0, 0, 0, -1, -1, 0, 0, 0, -1 };
    zg_n++;
}
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
    if (zf_n >= ZLS_MAX_FIELDS) { fprintf(stderr, "zls: field table overflow (%d)\n", ZLS_MAX_FIELDS); abort(); }
    zf[zf_n++] = (zls_pfield_t){ scope_id, off, size, (unsigned char)kind, (unsigned char)audit, what, nd };
}
static int zls_locals_shifted(IR_e op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_entry(const IR_t * nd, int scope_id, int off) {
    if (ze_n >= ZLS_MAX_ENTRIES) { fprintf(stderr, "zls: entry table overflow (%d)\n", ZLS_MAX_ENTRIES); abort(); }
    ze[ze_n] = (zls_entry_t){ nd, scope_id, off, off + (zls_locals_shifted(nd->op) ? 16 : 0), 1 };
    zx[zx_n++] = &ze[ze_n];
    ze_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant_locals(const IR_t * nd, int scope_id, int off) {
    switch (nd->op) {
    case IR_TO: case IR_TO_BY:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "to.I counter", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "to.limit", nd); return 1;
    case IR_MAKE_LIST: {
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off * j, 16, ZK_DESCR, 0, "list.elem", nd);
        return 0 + nd->n_operands; }
    case IR_SCAN_ENTER:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.leave out3 sigma (transient reg out-area; dead at safe points)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.leave out3 delta", nd); zls_field(scope_id, off + 16, 8, ZK_RAW, 0, "scan.leave out3 Delta", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "scan.pad (unused)", nd); return 2;
    case IR_MATCH_BEGIN:
        zls_field(scope_id, off, 4, ZK_RAW, 0, "head.cursor (ZB-FC-3d granted: the LIVE anchor lives in HEAD's self-pushed 32B rsp cell at [rsp+0] via the op_fc_wbytes window; this FLAT +0 then holds the RELEASE-stashed match START read by IR_MATCH_REPLACE -- same logical offset, window-disambiguated, so REPLACE's template is unchanged both paths)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "head.zeta_mark (BB-OWNED-zeta statement-scope saved rt_zls_mark() pointer; ZB-FC-3d granted: cell-resident at [rsp+8])", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "head.zls2_mark (ZC_PORT_ALLOC only: saved rt_zls2_mark() cursor; released by head's own omega-choke on failure / IR_MATCH_END on success — the ZLS2 twin of head.zeta_mark, widened to a second quad because the first quad's padding is spent.  ZB-FC-3d granted: cell-resident at [rsp+16] holding the PRE-PUSH rsp, so the S10e unwind releases HEAD's cell and every suspended pattern cell in one mov)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "head.end (SN4-REPL: end cursor stashed by IR_MATCH_END when the statement carries a replacement, read by IR_MATCH_REPLACE; ZB-FC-3d: FLAT on both paths -- post-unwind lifetime)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "head.dcap_mark (REG-6 PEND-PROMOTE: α saves live-r12 pend top = this match's MARK; ω/RELEASE truncate r12 from it — the cell [RT_DCAP_TOP] is now seed-source only, prologue-read, never written mid-match; ZK_RAW — points into the base-pinned dcap island, never GC-moved.  ZB-FC-3d: FLAT on both paths -- RELEASE's post-unwind pump reads it after the match dies)", nd); zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "head.incoming____ (DEAD at REG-2 — ___ is no longer the pend cursor, nothing saves or restores here; slot left ALLOCATED v1 so op_off accounting does not ripple, reclaim is a named follow-up.  Re-tenants at REG-3 as the frame-___ era's saved-outer-___ if the wire-header [+24] route changes)", nd); zls_field(scope_id, off + 48, 8, ZK_RAW, 0, "head.sigma_save (PATCTX, Lon directive 2026-07-29: HEAD saves the OUTER match's Σ=r13 at α before rt_match_enter installs the new subject; BOTH exits restore -- head's own omega-choke on failure, release_pump's tail on success -- and re-sync the C-side Σ/Σlen mirror via rt_match_ctx_restore, so nested matches (deferred *F() evaluation, EVAL/CODE, pump-committed *VAR proc bodies that themselves match) are LIFO-sound by construction.  ZK_RAW deliberately: Σ is register-resident today and registers are GC-invisible regardless; when ZHEAP moves strings THIS slot is exactly where a suspended subject becomes a findable root -- retag interior-PTR_GC in that rung.  KNOWN BYPASS, named not hidden: pat_seal kills (ABORT, FENCE-seal) jump straight to fJ and skip the omega-choke, so they skip this restore the same way they already skip the zls unwind and CAS pop -- the ___ match-frame rung is what closes that class)", nd); zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "head.delta_save (PATCTX: outer δ=r14)", nd); zls_field(scope_id, off + 64, 8, ZK_RAW, 0, "head.Delta_save (PATCTX: outer Δ=r15)", nd); zls_field(scope_id, off + 72, 8, ZK_RAW, 0, "head.capgen_save (PATCTX-2: the OUTER match g_cap_gen id, read at alpha before rt_match_enter draws a fresh id from the monotonic well; both exits restore it through rt_match_ctx_restore -- nest1 autopsy: the inner match stamp invalidated the outer SAVE bracket, pop no-opd, top returned 0, R captured [0,end).  Occupies the former pad quad, so the 5-quad grant and every downstream offset are unchanged)", nd); return 5;
    case IR_MATCH_SPAN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "span.cnt/cur", nd); return 1;
    case IR_MATCH_BAL:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "bal.n/start/depth", nd); return 1;
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "break.cnt/cur", nd); return 1;
    case IR_MATCH_FENCE0:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "fence0.watermark (RESERVED — own-extent whack rung; unread today)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "fence0.pad (unused)", nd); return 1;
    case IR_MATCH_FENCE1:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "fence.watermark (α-saved rsp under FORTH / zls2 cursor under ALLOC; σ/φ glue restores)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "fence.pad (unused)", nd); return 1;
    case IR_MATCH_ARB:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "arb.cnt/cur (matched-length +0 4B, saved-start +4 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "arb.zls2 activation block ptr (save-slot-in-frame, ZC_PORT_ALLOC only: reuses this node's existing pad, same reuse precedent as IR_MATCH_BEGIN.zeta_mark; block itself is a separate ZLS2 allocation, header +0 chains the previous activation's ptr)", nd); return 1;
    case IR_MATCH_REM:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "match.cursor save", nd); return 1;
    case IR_MATCH_DEFER:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "defer.pad (ZS-2 jmp-entry, Lon s58: the fn/frame cell pair is DELETED — the blob is a jmp-entered new activation that self-allocates on rsp with a 32B wire header, so there is nothing to stash and nothing to guard; quad KEPT at 16B so no later node's offset shifts. s137 OVER-SEAL: when IR_t.seal, quad +0 is REPURPOSED as the fence-demarked sync watermark — α stamps rsp there, the γ/ω glues and β bulk-restore it; +8 stays pad)", nd); return 1;
    case IR_MATCH_VALUE:
        return 0;
    case IR_MATCH_TAB: case IR_MATCH_RTAB:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "tab.cursor save (+0 4B r14d saved at α, restored at β; +4 pad)", nd); return 1;
    case IR_MATCH_ARBNO:
        if (IR_LIT(nd).ival == 1) { zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno.owner quad: entry/yield/i/cap (4x4B; SN4-NARY-ARBNO one-node form)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "arbno.COLLECTION ptr (rt_zcol_push-grown per-iteration elements: 16B header {prev_view, saved_delta} + body slot window; the rsp flavor = linked frame chain + explicit count in the header — Lon ruling 2026-07-12, lands at ZB-ITER under ZLS_ARBNO_STACK)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "arbno.saved_rsp (alpha saves rsp here; exhaust L(2) restores it)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "arbno.saved_outer____ (W-1c.2: unconditional — chain-beta always saves MATCH_BEGIN ___ into this slot before repointing ___ as element view; exhaust L(2) always restores it; view-restores at sigma/phi re-derive ___ from rsp unconditionally)", nd); return 3; }
        return 0;
    case IR_MATCH_ASSIGN_SAVE:
        zls_field(scope_id, off, 8, ZK_PTR_GC, 0, "capture.stack ws u32[] ([0]=cap, frames from [1]; box α-push/β-pop)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "capture.stack gen(+8,4B)/sp(+12,4B)", nd); return 1;
    case IR_MATCH_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "alt.entry cursor save (+0 4B r14d; +4 4B dead — was alt_i, killed by ALT-FLAT s202 address dispatch)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_CODE, 0, "alt.resume continuation (ALT-FLAT s202: each arm's sigma stub stores its own resume trampoline address via lea rip; beta is one indirect jmp — the alt_i cmp-chain is dead.  Retenants the old dcap-pad quad)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_CODE, 0, "alt.next-entry continuation (ALT-FLAT s202: alpha and each entry stub store the NEXT arm's entry-stub address; the fail-advance is delta-restore + one indirect jmp — the entry cmp-chain is dead)", nd); return 2;
    case IR_SCAN_SEQUENCE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scanseq.entry δ save (+16 from box base, 4B r14d) + seq_i live-element index (+20, 4B; α=0, na_s ++, na_f --, β=N); the value DESCR is the box result slot at [base]", nd); return 1;
    case IR_SCAN_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scanalt.entry δ save (+16 from box base, 4B r14d) + dcap height (+20, 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scanalt.alt_i live-alternative index (+24, 4B; α=0, na_f ++; β dispatches) (+28 pad)", nd); return 2;
    case IR_SCAN:
        return 0;
    case IR_SCAN_TAB: case IR_SCAN_MOVE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.r14 data-backtrack save", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.pad (unused)", nd); return 1;
    case IR_BOUND:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "bound.saved rsp (Op_Mark: bounded-expression entry frontier; IR_UNMARK restores it, discarding abandoned retained-suspension FC carves — interp.r Op_Unmark rsp=efp-1)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "bound.pad (unused)", nd); return 1;
    case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_MATCH: case IR_SCAN_BAL:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.cursor", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.len/counter", nd); return 1;
    case IR_INITIAL:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "initial.pad (unused low half)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "initial.once flag (0->1)", nd); return 1;
    case IR_ITERATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "iterate.index i (alpha=0, beta inc)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "iterate.pad (unused)", nd); return 1;
    case IR_LIMIT:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "limit.counter", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "limit.pad (unused)", nd); return 1;
    case IR_REPALT:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "repalt.yielded flag (clear/yield/test)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "repalt.pad (unused)", nd); return 1;
    case IR_REV_ASSIGN: case IR_REV_ASSIGN_VAR:
        zls_field(scope_id, off, 16, ZK_DESCR, 0, "revasg.saved old value (beta restore; LIVE across suspension — GC must trace)", nd); return 1;
    case IR_REV_SWAP:
        zls_field(scope_id, off, 16, ZK_DESCR, 0, "revswap.saved lhs old (beta restore; LIVE across suspension — GC must trace)", nd); zls_field(scope_id, off + 16, 16, ZK_DESCR, 0, "revswap.saved rhs old (beta restore; LIVE across suspension — GC must trace)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "revswap.delta spill (in-scan r14 round-trip)", nd); zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "revswap.Delta spill (in-scan r15, read-only len)", nd); return 3;
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
    case IR_STATEMENT:
        return 0;
    case IR_DISJUNCTION:
        if (nd->op == IR_DISJUNCTION && nd->n_operands > 0) { zls_field(scope_id, off, 8, ZK_RAW, 0, "disj.alt_i live-alternative index (+16 from box base; nary self-state, MOVE_LABEL-ERAD: α=0, φ-glue ++, β dispatches; value DESCR = the box result slot at [base], option-B per-arm copy in σ-glue) (+24 pad)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "disj.pad (unused)", nd); return 1; }
        zls_field(scope_id, off, 8, ZK_PTR_CODE, 0, "gate.stored resume target", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "gate.pad (unused)", nd); return 1;
    case IR_CALL_BUILTIN_GEN:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0, "callgen.resume position (alpha=0, runtime writes next start)", nd);
        zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "callgen.pad (unused)", nd);
        return 1 + nd->n_operands;
    case IR_PROC_GEN: case IR_CALL_VALUE:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_PTR_GC, 0, "callgen.act — GENP-SPINE s92: the spine arm's epilogue-once flag (0/1, α-zeroed); was the pthread model's ZLS2 activation handle, which legacy non-RSP configs still write via rt_proc_call_gen_h's hout", nd);
        zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "callgen.act pad (unused)", nd);
        return 2 + nd->n_operands;
    default:
        if (nd->op == IR_CALL || ir_is_call_kind(nd->op)) {
            for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
            if (nd->op == IR_CALL_PROC_STAGED && zls_callee_is_gen(nd)) {
                zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_PTR_GC, 0, "callgen.act — GENP-SPINE s92: the spine arm's epilogue-once flag (0/1, α-zeroed); was the pthread model's ZLS2 activation handle, which legacy non-RSP configs still write via rt_proc_call_gen_h's hout.  Offset repaired off*(1+n) → off + 16*(1+n), the emitting arms' exact formula", nd);
                zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "callgen.act pad (unused)", nd);
                return 2 + nd->n_operands;
            }
            { const char * cmn = IR_LIT(nd).sval; if (cmn && (!strcmp(cmn, "tab") || !strcmp(cmn, "move"))) {
                zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0, "scan.saved_delta — ICN-BYNAME-CURSOR-RESTORE: a cursor-mover (tab/move, and =s == tab(match(s))) reached by-name through rt_call_arr has no inline bb_scan_tab body, so it also had no saved-δ slot and its β degenerated to a bare jmp ω — the backtrack never restored &pos. This quad is that slot; bb_call_byname_str writes r14 here at α and reloads it in β, mirroring bb_scan_tab's restore-δ-and-FAIL port. Same extra-quad shape as callgen.act above.", nd);
                zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "scan.saved_delta pad (unused)", nd);
                return 2 + nd->n_operands; } }
            return 1 + nd->n_operands;
        }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_is_wiring(IR_e op) { return op == IR_GOTO || op == IR_MOVE_LABEL || op == IR_GOTO_DEFERRED || op == IR_SUCCEED || op == IR_FAIL || op == IR_RETURN || op == IR_SUSPEND || op == IR_CORET || op == IR_COFAIL || op == IR_CUT || op == IR_MATCH_END || op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_GLIT || op == IR_GCC || op == IR_GALT; }
static int zls_locals_shifted(IR_e op) { return op == IR_MATCH_BEGIN || op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARB || op == IR_MATCH_BAL || op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_MATCH_ARBNO || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE || op == IR_MATCH_ASSIGN_SAVE || op == IR_SCAN_ENTER || op == IR_INITIAL; }
int fc_arm_member(const IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_cells_on(void) { return 1; }
int fc_cells_active(void) { return fc_cells_on(); }
static int zls_fc_cell(const IR_t * nd) { if (!fc_cells_on()) return 0; if (!nd) return 0; { extern int fc_arm_member(const IR_t *); if (fc_arm_member(nd)) return 0; } switch (nd->op) { case IR_MATCH_SPAN: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_BREAK: case IR_MATCH_BREAKX: case IR_MATCH_BAL: case IR_MATCH_REM: case IR_MATCH_ARB: return 16; default: return 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant(const IR_t * nd, int scope_id, int off) {
    if (zls_is_wiring(nd->op)) return 0;
    zls_entry(nd, scope_id, off);
    zls_field(scope_id, off, 16, ZK_DESCR, 0, "result", nd);
    if (zls_fc_cell(nd)) { ze[ze_n - 1].loff = ZLS_FC_SYNTH; return 1; }
    return 1 + zls_grant_locals(nd, scope_id, off + 16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_elide_ok(IR_e op) { return op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_LEN || op == IR_MATCH_LIT || op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_CMP_TEST || op == IR_ASSIGN; }
static int zls_s4_ok(IR_e op) { return op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_BAL || op == IR_MATCH_ALTERNATE || op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE || op == IR_CALL_PROLOG; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_mark_value_refs(const IR_graph_t * g, char * live) {
    for (int k = 0; k < g->n; k++) { const IR_t * c = g->all[k]; if (!c) continue;
        if (c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_FENCE0 || c->op == IR_MATCH_FENCE1 || c->op == IR_MOVE_LABEL) continue;
        for (int j = 0; j < c->n_operands; j++) { const IR_t * p = c->operands[j]; if (!p) continue; if (j == 0 && (c->op == IR_MATCH_ASSIGN_COND || c->op == IR_MATCH_ASSIGN_IMM)) continue; for (int i = 0; i < g->n; i++) if (g->all[i] == p) { live[i] = 1; break; } } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant_elide(const IR_t * nd, int scope_id, int off, int live, int * scratch_off) {
    if (zls_is_wiring(nd->op)) return 0;
    if (!live && zls_elide_ok(nd->op)) {
        if (*scratch_off < 0) { *scratch_off = off; zls_entry(nd, scope_id, off); ze[ze_n - 1].live = 0; zls_field(scope_id, off, 16, ZK_DESCR, 0, "result (SLOT-ELIDE shared dead-result scratch — every later dead leaf in this graph aliases here)", nd); return 1; }
        zls_entry(nd, scope_id, *scratch_off); ze[ze_n - 1].live = 0; return 0;
    }
    if (!live && zls_s4_ok(nd->op)) { zls_entry(nd, scope_id, off); ze[ze_n - 1].loff = off; ze[ze_n - 1].live = 0; if (zls_fc_cell(nd)) { ze[ze_n - 1].loff = ZLS_FC_SYNTH; return 0; } return zls_grant_locals(nd, scope_id, off); }
    int ei = ze_n; int n = zls_grant(nd, scope_id, off); if (ze_n > ei) ze[ei].live = live; return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_result_live(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->live : 1; }
int zls_node_off(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->off : -0x40000000; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_scope_new(int parent, int klass, const char * name) {
    if (zs_n >= ZLS_MAX_SCOPES) { fprintf(stderr, "zls: scope table overflow (%d)\n", ZLS_MAX_SCOPES); abort(); }
    zs[zs_n] = (zls_scope_t){ zs_n, parent, klass, name, -1, 0, 0x7fffffff, 0 };
    return zs_n++;
}
int fc_alt_fpmax(const IR_t *);
int fc_alt_extent(const IR_t *, int *, int *);
int fc_alt_n(const IR_t *);
int fc_alt_arm_range(const IR_t *, int, int *, int *);
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
            op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END ||
            op == IR_MATCH_LIT || op == IR_MATCH_LEN || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY ||
            op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_ASSIGN_COND ||
            op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE || op == IR_MATCH_ALTERNATE ||
            op == IR_MATCH_FENCE0 || op == IR_MATCH_FENCE1 || op == IR_BOUND || op == IR_UNMARK || op == IR_CONJUNCTION ||
            op == IR_CUT || op == IR_MOVE_LABEL || op == IR_GLIT || op == IR_GCC || op == IR_GALT ||
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
            if (c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_ARBNO || c->op == IR_MATCH_FENCE1 || c->op == IR_SCAN_SEQUENCE || c->op == IR_SCAN_ALTERNATE || c->op == IR_REPALT || c->op == IR_MOVE_LABEL) continue;
            for (int j = 0; j < c->n_operands; j++) if (c->operands[j] == nd) { ref = 1; break; } }
        if (ref) L++; }
    tg += G; tl += L; tn++;
    fprintf(stderr, "[SLOT-CENSUS] g=%p n=%d result_quads=%d value_refd=%d dead_floor=%d rq_bytes %d -> %d | TOTALS graphs=%ld quads=%ld refd=%ld dead=%ld bytes %ld -> %ld\n",
            (void *)g, g->n, G, L, G - L, G * 16, (L + 1) * 16, tn, tg, tl, tg - tl, tg * 16, (tl + tn) * 16);
}
void zls_fct_finalize(IR_graph_t * g, int late);
void fc_vlit_register(const IR_t *); void fc_vread_register(const IR_t *, int); void fc_vbinop_register(const IR_t *); int fc_vcap(int, int, int, int); int is_global(const char *); void fc_vwpop_register(const IR_t *, long); void fc_subj_register(const IR_t *); int zc_nofc(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_vvar_ok(const IR_graph_t * g, const IR_t * r) { const char * vn = IR_LIT(r).sval; return vn && vn[0] != '&' && ((is_global(vn) && !graph_has_local(g, vn)) || !strcmp(vn, "write") || !strcmp(vn, "writes")); }
static int fc_vbinop_ok(long long v) { return v == 0 || v == 1 || v == 2 || v == 3 || v == 4 || v == 11 || v == 18; }
static int fc_vunop_ok(const IR_t * nd) { return nd->op == IR_UNOP && nd->n_operands == 1 && ((int)IR_LIT(nd).ival == TT_MNS || (int)IR_LIT(nd).ival == TT_PLS); }
static int g_fcc_gfence = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_call_ok(const IR_t * nd) { static int on = -1; if (on < 0) { const char * a = getenv("SCRIP_STMT_FRAME"); const char * b = getenv("SCRIP_CALL2BB"); on = (a && *a == '1' && b && *b == '1') ? 1 : 0; if (on) { const char * c = getenv("SCRIP_CALL2BB_FC"); if (!(c && *c == '1')) on = 0; } } if (!on || !nd) return 0; { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) { const char * fn = IR_LIT(nd).sval; extern int rt_proc_is_registered(const char *); extern int rt_builtin_is_known(const char *); fprintf(stderr, "[FCC] op=%d nops=%d sval=%s reg=%d blt=%d\n", (int)nd->op, nd->n_operands, fn ? fn : "(null)", (fn && fn[0]) ? rt_proc_is_registered(fn) : -1, (fn && fn[0]) ? rt_builtin_is_known(fn) : -1); { extern int rt_proc_is_generator(const char *); if (fn && fn[0]) fprintf(stderr, "[FCC] gen=%d\n", rt_proc_is_generator(fn)); } } } if (!(nd->op == IR_CALL || nd->op == IR_CALL_PROC_STAGED)) return 0; if (nd->n_operands != 1 || !nd->operands[0]) return 0; { const char * fn = IR_LIT(nd).sval; extern int rt_proc_is_registered(const char *); extern int rt_proc_is_generator(const char *); extern int rt_builtin_is_known(const char *); if (!fn || !fn[0] || !strcmp(fn, "CODE") || rt_builtin_is_known(fn) || !rt_proc_is_registered(fn) || rt_proc_is_generator(fn)) return 0; } return 1; }
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
void zls_build(IR_graph_t * g) {
    if (!g) return;
    { int f = 1; for (int i = 0; i < g->n && f; i++) { IR_t * x = g->all[i]; if (!x) continue; if (x->op == IR_GOTO_DEFERRED || x->op == IR_INDIRECT_GOTO || x->op == IR_MATCH_BEGIN || x->op == IR_MATCH_DEFER) f = 0; else if ((x->op == IR_DEFINE && ir_define_sr_citizen(x))) { long long v = IR_LIT(x).ival; if (v == 1 || v == 2) f = 0; } else if ((x->op == IR_CALL_BUILTIN || x->op == IR_CALL_SNOBOL4 || x->op == IR_CALL) && IR_LIT(x).sval && (!strcmp(IR_LIT(x).sval, "EVAL") || !strcmp(IR_LIT(x).sval, "CODE"))) f = 0; } g_fcc_gfence = f; }
    for (int vi = 0; vi < g->n; vi++) { IR_t * a = g->all[vi]; if (!(a && a->op == IR_ASSIGN && a->n_operands == 1 && a->operands[0])) continue;
        { const char * vn = IR_LIT(a).sval; if (!(vn && is_global(vn) && !graph_has_local(g, vn))) continue; }
        IR_t * r = a->operands[0];
        if ((r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL || r->op == IR_LIT_CHARSET || r->op == IR_LIT_NAME
             || (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&' && ((is_global(IR_LIT(r).sval) && !graph_has_local(g, IR_LIT(r).sval)) || !strcmp(IR_LIT(r).sval, "write") || !strcmp(IR_LIT(r).sval, "writes"))))
            && r->γ.node == a && fc_vcap(1, 1, 0, 0)) { fc_vlit_register(r); fc_vread_register(a, 0); continue; }
        if ((r->op == IR_BINOP || fc_vunop_ok(r) || fc_call_ok(r)) && r->γ.node == a) {
            const IR_t * post[49]; int pn = 0;
            int _ts = fc_vtree_scan(g, r, post, &pn, 49, 0);
            { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg && _ts && pn > 0) fprintf(stderr, "[FCC] tree ts=%d pn=%d tail_is_r=%d\n", _ts, pn, (post[pn-1] == r) ? 1 : 0); }
            if (_ts && post[pn - 1] == r) {
                int ok = 1, L = 0, B = 0;
                for (int i = 0; i + 1 < pn; i++) { const IR_t * gx = post[i]->γ.node; if (gx == post[i + 1]) continue; if (gx && gx->op == IR_DEFINE && gx->γ.node == post[i + 1] && (post[i + 1]->op == IR_CALL || post[i + 1]->op == IR_CALL_PROC_STAGED)) { long long v = IR_LIT(gx).ival; if (!(v == 1 || v == 2 || v == 3)) continue; } ok = 0; break; }
                for (int i = 0; i < pn; i++) { if (post[i]->op == IR_BINOP || post[i]->op == IR_UNOP) B++; else L++; }
                { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg && pn > 1) fprintf(stderr, "[FCC] ok=%d cap=%d p0g=%p p1=%p\n", ok, fc_vcap(L, 1, B, pn), (void*)(pn > 1 ? post[0]->γ.node : 0), (void*)(pn > 1 ? post[1] : 0)); if (dbg && pn > 1 && post[0]->γ.node) fprintf(stderr, "[FCC] p0=%p p0op=%d p0g_op=%d r=%p rop=%d\n", (void*)post[0], (int)post[0]->op, (int)post[0]->γ.node->op, (void*)r, (int)r->op); }
                if (ok && fc_vcap(L, 1, B, pn)) {
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
            && _pb1s_adj && fc_vcap(1, 1, 0, 0)) { fc_vlit_register(r); fc_subj_register(r); fc_vread_register(h, 0); continue; } }
        if ((r->op == IR_BINOP || fc_vunop_ok(r)) && r->γ.node == h && !zc_nofc()) {
            const IR_t * post[49]; int pn = 0;
            int _ts = fc_vtree_scan(g, r, post, &pn, 49, 0);
            if (_ts && post[pn - 1] == r) {
                int ok = 1, L = 0, B = 0;
                for (int i = 0; i + 1 < pn; i++) if (post[i]->γ.node != post[i + 1]) { ok = 0; break; }
                for (int i = 0; i < pn; i++) { if (post[i]->op == IR_BINOP || post[i]->op == IR_UNOP) B++; else L++; }
                if (ok && fc_vcap(L, 1, B, pn)) {
                    int d = 0;
                    for (int i = 0; i < pn; i++) { const IR_t * x = post[i];
                        if (x->op == IR_BINOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); d -= 1; }
                        else if (x->op == IR_UNOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); }
                        else { fc_vlit_register(x); if (x->op == IR_VAR && d > 0) fc_vwpop_register(x, (long)d * 16); d += 1; } }
                    fc_vread_register(h, 0); } } } } }
    zls_graph_t * r = zls_g_find(g);
    if (r && r->first_scope >= 0) return;
    if (!r) {
        if (zg_n >= ZLS_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", ZLS_MAX_GRAPHS); abort(); }
        zg[zg_n] = (zls_graph_t){ g, (const char *)0, -1, 0, 0, 0, -1, -1, 0, 0, 0, -1 }; r = &zg[zg_n]; zg_n++;
    }
    static char anon[ZLS_MAX_GRAPHS][8]; int gi = (int)(r - zg);
    if (!r->name) { snprintf(anon[gi], sizeof anon[gi], "g%d", gi); r->name = anon[gi]; }
    int root = zls_scope_new(-1, ZSC_FN, r->name);
    r->first_scope = root; r->n_scopes = 1;
    int mfirst[ZLS_MAX_SCOPES]; int mstart[ZLS_MAX_SCOPES]; int nl = 0;
    for (int mi = 0; mi < zm_n; mi++) {
        if (zm[mi].g != g) continue;
        int sc = zls_scope_new(root, ZSC_GROUP, zm[mi].name);
        mfirst[nl] = sc; mstart[nl] = zm[mi].start_n; nl++;
        r->n_scopes++;
    }
    char rb_s[8192]; char * rb = (g->n <= (int)sizeof rb_s) ? rb_s : (char *)malloc((size_t)(g->n > 0 ? g->n : 1)); memset(rb, 0, (size_t)(g->n > 0 ? g->n : 1));
    { int hn = 4; while (hn < g->n * 2 + 4) hn <<= 1; IR_t ** hk = (IR_t **)calloc((size_t)hn, sizeof(IR_t *)); int * hv = (int *)malloc((size_t)hn * sizeof(int)); int * wl = (int *)malloc((size_t)(g->n > 0 ? g->n : 1) * sizeof(int)); int wn = 0;
      for (int i = 0; i < g->n; i++) { IR_t * p = g->all[i]; if (!p) continue; unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h]) h = (h + 1) & (unsigned long)(hn - 1); hk[h] = p; hv[h] = i; }
      if (g->entry) { IR_t * p = g->entry; unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1); if (hk[h]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; } }
      else for (int i = 0; i < g->n; i++) rb[i] = 1;
      int dyn = 0;
      for (int i = 0; i < g->n && !dyn; i++) if (g->all[i] && (g->all[i]->op == IR_GOTO_DEFERRED || g->all[i]->op == IR_INDIRECT_GOTO || ((g->all[i]->op == IR_CALL || g->all[i]->op == IR_CALL_BUILTIN || g->all[i]->op == IR_CALL_SNOBOL4) && IR_LIT(g->all[i]).sval && strcmp(IR_LIT(g->all[i]).sval, "CODE") == 0))) dyn = 1;
      if (dyn) { for (int i = 0; i < g->n; i++) if (g->all[i]) rb[i] = 1; wn = 0; }
      for (int i = 0; i < g->n; i++) if (g->all[i] && !rb[i] && zls_is_wiring(g->all[i]->op)) { rb[i] = 1; wl[wn++] = i; }
      for (int mi2 = 0; mi2 < nl; mi2++) { int sp0 = mstart[mi2]; int sp1 = (mi2 + 1 < nl) ? mstart[mi2 + 1] : g->n; for (int i = sp0; i >= 0 && i < sp1 && i < g->n; i++) if (g->all[i] && !rb[i]) { rb[i] = 1; wl[wn++] = i; } }
      while (wn > 0) { IR_t * c = g->all[wl[--wn]]; if (!c) continue; for (int j = -2; j < c->n_operands; j++) { IR_t * p = (j == -2) ? c->γ.node : (j == -1) ? c->ω.node : c->operands[j]; if (!p) continue; unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1); if (hk[h] && !rb[hv[h]]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; } } }
      free(hk); free(hv); free(wl); }
    int s0 = (g->nparams > 0 || g->resumable_callable) ? 1 : 0;
    for (int i = 0; !s0 && i < g->n; i++) if (g->all[i] && rb[i] && (g->all[i]->op == IR_RETURN || g->all[i]->op == IR_SUSPEND)) s0 = 1;
    int base = s0 ? 16 + (g->nparams > 0 ? g->nparams * 16 : 0) : 0;
    int k = 0;
    r->first_vslot = zv_n;
    for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) {
        if (zv_n >= ZLS_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", ZLS_MAX_VSLOTS); abort(); }
        zv[zv_n++] = (zls_vslot_t){ g->pnames[i], 16 + i * 16 }; r->n_vslots++;
        zls_field(root, 16 + i * 16, 16, ZK_DESCR, 0, "param", (const IR_t *)0);
    }
    int cur = 0;
    { static int eon = -1; if (eon < 0) { const char * e = getenv("SCRIP_SLOT_ELIDE"); eon = (e && *e == '0') ? 0 : 1; { const char * sf = getenv("SCRIP_STMT_FRAME"); const char * xo = getenv("SCRIP_STF_ELIDE_OFF"); if (sf && *sf == '1' && xo && *xo == '1') eon = 0; } }
      char lv_sbuf[1024]; char * lv = (g->n <= (int)sizeof lv_sbuf) ? lv_sbuf : (char *)malloc((size_t)g->n);
      memset(lv, 0, (size_t)g->n);
      if (eon) zls_mark_value_refs(g, lv);
      int scratch_off = -1;
      for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        while (cur < nl && i >= mstart[cur]) cur++;
        if (!rb[i]) continue;
        int sc = (cur > 0) ? mfirst[cur - 1] : root;
        k += eon ? zls_grant_elide(nd, sc, base + k * 16, lv[i], &scratch_off) : zls_grant(nd, sc, base + k * 16);
      }
      if (lv != lv_sbuf) free(lv);
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
    if (rt_zc_frame_live() != ZC_FRAME_RSP) {
        r->zeta_mark_off = base + k * 16;
        zls_field(root, r->zeta_mark_off, 8, ZK_RAW, 0, "graph-scope zeta mark (rt_zls_mark snapshot, prologue-stashed, epilogue-released)", (const IR_t *)0);
        zls_field(root, r->zeta_mark_off + 8, 8, ZK_RAW, 0, "zeta_mark.pad (unused)", (const IR_t *)0);
        k += 1;
    } else r->zeta_mark_off = -1;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd || !rb[i]) continue;
        const char * vn = (const char *)0;
        if (nd->op == IR_ASSIGN) vn = IR_LIT(nd).sval;
        else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) vn = IR_LIT(nd->operands[1]).sval;
        else if (nd->op == IR_VAR || nd->op == IR_VAR_REF) vn = IR_LIT(nd).sval;
        if (!vn || vn[0] == '&' || (is_global(vn) && !graph_has_local(g, vn))) continue;
        int have = 0; for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) if (zv[v].name && strcmp(zv[v].name, vn) == 0) { have = 1; break; }
        if (have) continue;
        if (zv_n >= ZLS_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", ZLS_MAX_VSLOTS); abort(); }
        zv[zv_n++] = (zls_vslot_t){ vn, base + k * 16 }; r->n_vslots++;
        zls_field(root, base + k * 16, 16, ZK_DESCR, 0, "local", (const IR_t *)0);
        k++;
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
    if (rb != rb_s) free(rb);
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
int zls2_geom(const IR_t * nd, int base_off, int * slot_off, long * k) {
    if (!nd || base_off < 0) return 0;
    if (nd->op == IR_MATCH_ARBNO) {
        long ph = IR_LIT(nd).ival;
        int ops = ph == 0 ? ZLS2_BUMP : ph == 2 ? ZLS2_RELEASE : 0;
        if (!ops) return 0;
        if (slot_off) *slot_off = base_off + 16;
        if (k) *k = 32;
        return ops;
    }
    return 0;
}
int fc_alt_fpmax(const IR_t * nd);
int fc_save_active(const IR_t * nd);
int fc_vlit_active(const IR_t * nd);
int fc_vdj_active(const IR_t * nd);
int fc_arm_member(const IR_t * nd);
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
static struct { const IR_t * nd; int n; int fp[16]; int ab[16]; int ae[16]; } fca[256];
static int fca_n = 0;
static const IR_t * fcm[1024];
static int fcm_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_arm_member_register(const IR_t * nd) { if (nd && fcm_n < 1024) fcm[fcm_n++] = nd; }
int fc_arm_member(const IR_t * nd) { for (int i = 0; i < fcm_n; i++) if (fcm[i] == nd) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_alt_register(const IR_t * nd, int n, const int * fp, const int * ab, const int * ae) {
    if (!nd || n <= 0 || n > 10 || fca_n >= 256) return;
    fca[fca_n].nd = nd; fca[fca_n].n = n;
    for (int i = 0; i < n && i < 16; i++) { fca[fca_n].fp[i] = fp[i]; fca[fca_n].ab[i] = ab ? ab[i] : -1; fca[fca_n].ae[i] = ae ? ae[i] : -1; }
    fca_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_n(const IR_t * nd) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) return fca[i].n;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_extent(const IR_t * nd, int * b, int * e) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) { if (fca[i].ab[0] < 0 || fca[i].ae[fca[i].n - 1] < 0) return 0; if (b) *b = fca[i].ab[0]; if (e) *e = fca[i].ae[fca[i].n - 1]; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_arm_range(const IR_t * nd, int j, int * b, int * e) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) { if (j < 0 || j >= fca[i].n || fca[i].ab[j] < 0) return 0; if (b) *b = fca[i].ab[j]; if (e) *e = fca[i].ae[j]; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_fpmax(const IR_t * nd) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) { int m = 0; for (int j = 0; j < fca[i].n; j++) if (fca[i].fp[j] > m) m = fca[i].fp[j]; return m; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_alt_fp(const IR_t * nd, int j) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) return (j >= 0 && j < fca[i].n) ? fca[i].fp[j] : 0;
    return 0;
}
static const IR_t * fcs[512];
static int fcs_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_seq_register(const IR_t * nd) { if (!nd || fcs_n >= 512) return; fcs[fcs_n++] = nd; }
void fc_seq_unregister(const IR_t * nd) { for (int i = 0; i < fcs_n; i++) if (fcs[i] == nd) { fcs[i] = fcs[--fcs_n]; return; } }
static const IR_t * fcab[512]; static int fcab_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_arbno_member_register(const IR_t * nd) { if (nd && fcab_n < 512) fcab[fcab_n++] = nd; }
int fc_arbno_member(const IR_t * nd) { for (int i = 0; i < fcab_n; i++) if (fcab[i] == nd) return 1; return 0; }
int fc_seq_active(const IR_t * nd) { (void)nd; return 0; }
static const IR_t * fvl[2048]; static int fvl_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Value-disjunctions whose CONSUMERS read the arm result from the ζ-spine, declared by whoever lowered them. The
   structural shape (3N operands past 2N port pairs, ival>0) is NOT sufficient to grant a flat cell: it is true of
   every frontend that lowers a value-carrying disjunction onto this one shared host, and a grant to a host whose
   consumers still address the FRAME re-routes FRQ() to the spine for the producer alone -- the producer/consumer
   split. The grant is therefore keyed on the consuming regime, which only the lowerer knows, and is registered the
   same way fc_vlit/fc_save already are. Every IR_DISJUNCTION stays eligible, so this is a behavioral condition and
   not a per-op admission list. */
/* ⛔ SILENT TRUNCATION IN A CORRECTNESS PATH IS A SIZE-DEPENDENT MISCOMPILE (hq_P s271). Every fc_*_register below
   is a fixed-cap table that used to `return` at the cap without a word. For the optimization tables that merely cost
   a missed cell; for fvdj it silently withholds a grant the consumer still expects from the spine, reintroducing the
   producer/consumer split as a bug that appears ONLY in large programs and shows on nothing in our corpus, because
   our programs are small. It is reported LOUDLY, once per table, and never gated on an env var -- a diagnostic you
   have to opt into is exactly the instrument that cannot express its own failure. SCRIP_FC_REG_HIGHWATER=1 prints
   the peak occupancy of every table so the caps can be sized from measurement rather than from guessing. */
static void fc_reg_full(const char * tbl, int cap) { static const char * seen[16]; static int seen_n = 0;
    for (int i = 0; i < seen_n; i++) if (seen[i] == tbl) return;
    if (seen_n < 16) seen[seen_n++] = tbl;
    fprintf(stderr, "⛔ fc_%s_register: TABLE FULL at cap %d -- further registrations are being DROPPED. In a correctness-gating table (fvdj) this withholds a flat cell the consumer still reads from the spine, i.e. a miscompile that only appears in programs this large. Raise the cap in src/contracts/zeta_storage.c.\n", tbl, cap); }
static void fc_reg_hw(const char * tbl, int n) { static const char * e = (const char *) 1; if (e == (const char *) 1) e = getenv("SCRIP_FC_REG_HIGHWATER");
    if (e && *e != '0') fprintf(stderr, "FC-REG-HW %-6s %d\n", tbl, n); }
static const IR_t * fvdj[256]; static int fvdj_n = 0;
void fc_vdj_register(const IR_t * nd) { if (!nd) return; if (fvdj_n >= 256) { fc_reg_full("vdj", 256); return; } fvdj[fvdj_n++] = nd; fc_reg_hw("vdj", fvdj_n); }
int fc_vdj_active(const IR_t * nd) { if (!nd) return 0; for (int i = 0; i < fvdj_n; i++) if (fvdj[i] == nd) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vlit_register(const IR_t * nd) { if (!nd) return; if (fvl_n >= 2048) { fc_reg_full("vlit", 2048); return; } fvl[fvl_n++] = nd; fc_reg_hw("vlit", fvl_n); }
static const IR_t * fvs[64]; static int fvs_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_subj_register(const IR_t * nd) { if (!nd) return; if (fvs_n >= 64) { fc_reg_full("subj", 64); return; } fvs[fvs_n++] = nd; fc_reg_hw("subj", fvs_n); }
int fc_subj_member(const IR_t * nd) { if (!nd) return 0; for (int i = 0; i < fvs_n; i++) if (fvs[i] == nd) return 1; return 0; }
int fc_vlit_active(const IR_t * nd) { if (!fc_cells_on()) return 0; if (!nd || !(nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME || nd->op == IR_VAR)) return 0; for (int i = 0; i < fvl_n; i++) if (fvl[i] == nd) return 1; return 0; }
static struct { const IR_t * nd; int fp; } fvr[1024]; static int fvr_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vread_register(const IR_t * nd, int fp) { if (!nd || fp < 0) return; if (fvr_n >= 1024) { fc_reg_full("vread", 1024); return; } fvr[fvr_n].nd = nd; fvr[fvr_n].fp = fp; fvr_n++; fc_reg_hw("vread", fvr_n); }
int fc_vread_fp(const IR_t * nd) { for (int i = 0; i < fvr_n; i++) if (fvr[i].nd == nd) return fvr[i].fp; return -1; }
static const IR_t * fvb[512]; static int fvb_n = 0;
static const IR_t * fvcl[64]; static int fvcl_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_call_register(const IR_t * nd) { if (!nd || fvcl_n >= 64) return; fvcl[fvcl_n++] = nd; }
int fc_call_active(const IR_t * nd) { if (!nd || !fc_cells_on()) return 0; for (int i = 0; i < fvcl_n; i++) if (fvcl[i] == nd) return 1; return 0; }
void fc_vbinop_register(const IR_t * nd) { if (!nd) return; if (fvb_n >= 512) { fc_reg_full("vbinop", 512); return; } fvb[fvb_n++] = nd; fc_reg_hw("vbinop", fvb_n); }
int fc_vbinop_active(const IR_t * nd) { if (!nd || (nd->op != IR_BINOP && nd->op != IR_UNOP)) return 0; for (int i = 0; i < fvb_n; i++) if (fvb[i] == nd) return 1; return 0; }
static struct { const IR_t * nd; long w; } fvw[512]; static int fvw_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_vwpop_register(const IR_t * nd, long w) { if (!nd || w <= 0) return; if (fvw_n >= 512) { fc_reg_full("vwpop", 512); return; } fvw[fvw_n].nd = nd; fvw[fvw_n].w = w; fvw_n++; fc_reg_hw("vwpop", fvw_n); }
long fc_vwpop(const IR_t * nd) { if (!fc_cells_on()) return 0; for (int i = 0; i < fvw_n; i++) if (fvw[i].nd == nd) return fvw[i].w; return 0; }
int fc_vcap(int nl, int nr, int nb, int nw) { return fvl_n + nl <= 2048 && fvr_n + nr <= 1024 && fvb_n + nb <= 512 && fvw_n + nw <= 512; }
static const IR_t * fcv[256];
static int fcv_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_save_register(const IR_t * nd) { if (!nd) return; if (fcv_n >= 256) { fc_reg_full("save", 256); return; } fcv[fcv_n++] = nd; fc_reg_hw("save", fcv_n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_save_active(const IR_t * nd) {
    if (!nd || nd->op != IR_MATCH_ASSIGN_SAVE || !fc_cells_on()) return 0;
    for (int i = 0; i < fcv_n; i++) if (fcv[i] == nd) return 1;
    return 0;
}
static struct { const IR_t * nd; int e; } fpe[256]; static int fpe_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_pair_extent_register(const IR_t * nd, int e) { if (!nd || e <= 0 || fpe_n >= 256) return; for (int i = 0; i < fpe_n; i++) if (fpe[i].nd == nd) return; fpe[fpe_n].nd = nd; fpe[fpe_n].e = e; fpe_n++; }
int fc_pair_extent(const IR_t * nd) { for (int i = 0; i < fpe_n; i++) if (fpe[i].nd == nd) return fpe[i].e; return -1; }
static struct { const IR_t * nd; const IR_t * save; int fp; } fcc[256];
static int fcc_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_cond_register(const IR_t * nd, int fp_inner) {
    if (!nd || fp_inner < 0 || fcc_n >= 256) return;
    fcc[fcc_n].nd = nd; fcc[fcc_n].save = NULL; fcc[fcc_n].fp = fp_inner; fcc_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_cond_register_with_save(const IR_t * nd, const IR_t * save, int fp_inner) {
    if (!nd || fp_inner < 0 || fcc_n >= 256) return;
    fcc[fcc_n].nd = nd; fcc[fcc_n].save = save; fcc[fcc_n].fp = fp_inner; fcc_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_cond_fp(const IR_t * nd) {
    if (!fc_cells_on()) return -1;
    for (int i = 0; i < fcc_n; i++) if (fcc[i].nd == nd) {
        if (fcc[i].save && fc_arm_member(fcc[i].save)) return -1;
        return fcc[i].fp;
    }
    return -1;
}
static struct { const IR_t * nd; int fp; } fch[256];
static int fch_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fc_head_register(const IR_t * nd, int fp) { if (!nd || fp < 0 || fch_n >= 256) return; fch[fch_n].nd = nd; fch[fch_n].fp = fp; fch_n++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_head_fp(const IR_t * nd) {
    if (!fc_cells_on()) return -1;
    for (int i = 0; i < fch_n; i++) if (fch[i].nd == nd) return fch[i].fp;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; return e->loff; }
int zls_result_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->off : -1; }
int zls_node_bytes(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return 0; int end = e->off; for (int i = 0; i < zf_n; i++) if (zf[i].nd == nd && zf[i].scope_id == e->scope_id && zf[i].off + zf[i].size > end) end = zf[i].off + zf[i].size; int b = end - e->off; return (b + 15) & ~15; }
int zls_scope_of(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->scope_id : -1; }
int zls_g_nslots(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->nslots : -1; }
int zls_g_region(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->region : -1; }
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
int zls_g_icn_zframe_gen_by_name(const char *name) { if (!name) return 0; for (int i = 0; i < zg_n; i++) if (zg[i].name && strcmp(zg[i].name, name) == 0) return zg[i].g ? zg[i].g->icn_zframe_gen : 0; return 0; }
void zls_g_set_pl_trail_mark(const IR_graph_t *g, int off) { if (!g) return; for (int i = 0; i < zg_n; i++) if (zg[i].g == g) { zg[i].pl_trail_mark_off = off; return; } }
int zls_g_pl_zf_trail_mark_off_by_name(const char *name) { if (!name) return 0; for (int i = 0; i < zg_n; i++) if (zg[i].name && strcmp(zg[i].name, name) == 0) return zg[i].pl_trail_mark_off > 0 ? zg[i].pl_trail_mark_off : 0; return 0; }
int zls_g_pl_trail_mark_by_name(const char *name) { if (!name) return -1; for (int i = 0; i < zg_n; i++) if (zg[i].name && strcmp(zg[i].name, name) == 0) return zg[i].pl_trail_mark_off; return -1; }
int zls_g_locals(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->locals_off : -1; }
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
void zls_dump(FILE * fp) {
    static const char * col_names[]      = { "MALLOC", "ARENA", "GC" };
    static const char * selfload_names[] = { "OFF", "PLANE_CELL", "ASSERT", "STATIC" };
    static const char * init_names[]     = { "ZERO", "NONE", "CLONE" };
    fprintf(fp, "; ZETA LOCAL STORAGE (ZLS layout, ZB-2) — choices: COLLECTION=%s SELFLOAD=%s INIT=%s POISON=%s TELEM=%s OVF=%s ARENA_MB=%d PROMOTE=%s\n",
        col_names[ZC_COLLECTION], selfload_names[ZC_SELFLOAD], init_names[ZC_INIT],
        ZC_POISON == ZC_POISON_FILL ? "FILL" : "OFF", ZC_TELEMETRY == ZC_TELEM_ON ? "ON" : "OFF", ZC_OVERFLOW == ZC_OVF_BOMB ? "BOMB" : "GUARD", (int)ZC_ARENA_MB, ZC_PROMOTE == ZC_PROMOTE_GATE ? "GATE" : "ON");
    fprintf(fp, "; kinds: DESCR = 16B t.p pair (GC traces payload) | RAW = int/cursor/counter (GC skips) | PTR_GC = heap pointer (GC traces+fixes) | PTR_CODE = continuation (GC skips, never relocates)\n");
    fprintf(fp, "; (audit) = kind provisional pending template audit — 2026-07-05 burndown: all shipped grants template-verified, audit=0; any NEW grant lands audit=1 until verified\n");
    for (int i = 0; i < zg_n; i++) {
        zls_graph_t * r = &zg[i];
        if (r->first_scope < 0) continue;
        fprintf(fp, "; graph %d '%s' — slots=%d region_end=%d resume=%d vslots=%d scopes=%d\n", i, r->name ? r->name : "?", r->nslots, r->region, r->resume_off, r->n_vslots, r->n_scopes);
        for (int s = r->first_scope; s < r->first_scope + r->n_scopes; s++) {
            if (zs[s].n_fields == 0 && zs[s].klass == ZSC_GROUP) { fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d (no ZLS fields)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent); continue; }
            fprintf(fp, ";   scope %-3d %-6s %-24s parent=%-3d [%d..%d)\n", zs[s].id, zsc_name(zs[s].klass), zs[s].name ? zs[s].name : "?", zs[s].parent, zs[s].n_fields ? zs[s].lo_off : 0, zs[s].n_fields ? zs[s].hi_off : 0);
            for (int f = 0; f < zf_n; f++) if (zf[f].scope_id == s) {
                const char * on = "-"; char onb[16];
                if (zf[f].nd) { on = bb_op_name(zf[f].nd->op); if (!on) { snprintf(onb, sizeof onb, "op%d", (int)zf[f].nd->op); on = onb; } }
                fprintf(fp, ";     +%-5d %-3d %-8s %-36s %s%s\n", zf[f].off, zf[f].size, zk_name(zf[f].kind), zf[f].what ? zf[f].what : "", on, zf[f].audit ? "  (audit)" : "");
            }
        }
        for (int v = r->first_vslot; v < r->first_vslot + r->n_vslots; v++) fprintf(fp, ";   vslot +%-5d 16  DESCR    %s\n", zv[v].off, zv[v].name ? zv[v].name : "?");
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
void fc_tables_reset(void) { fct_n = 0; }
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
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐ N-2 ITEM 2 STEP 1 (hq_P s277): "IS THIS CALLEE'S FRAME SIZE KNOWN YET?" -- AND IT ANSWERS *NOT YET* DISTINGUISHABLY, WHICH IS THE ENTIRE POINT.  ceo RULED that a graph hosting a suspend-surviving call promotes to an RBP activation frame and a DIRECT call reserves the callee's compile-time-known frame bytes inside the host's own carve.  That needs a name -> frame-bytes lookup at HOST-emit time, and the registry for it ALREADY EXISTS: pz[] above is populated by emit_patzeta_register() for every proc, from all three driver arms.  ⛔ SO THIS ADDS NO GLOBAL STATE -- it is an accessor over state that is already there (the s272 precedent: extend what exists, never mint a parallel array; a parallel array is named explicitly in the NO-NEW-GLOBALS law).  ⛔⭐ WHY NOT emit_patzeta_lookup(): IT CANNOT SAY "NOT REGISTERED".  It returns pz[i].uni on a hit and 0 on a miss, so a genuine uniform==0 proc and an ABSENT proc are the same answer.  For the reservation that ambiguity is not cosmetic, it is the bug: an unregistered callee would read as a 0-byte reservation, i.e. a host carve SILENTLY TOO SMALL -- exactly the silent overflow ceo refused worst-case reservation to avoid, arriving through a different door.  ⭐ THE HAZARD IS REAL AND MEASURED, NOT THEORETICAL: procs are emitted before main (every driver proc loop `continue`s on main), so a main-host always finds its callee registered -- but a host that is ITSELF A PROC calling a generator declared LATER in the same loop is a FORWARD REFERENCE and the callee is genuinely not registered yet.  Measured in the bench_correct corpus this rung scores: geddump has 6 such callsites and tgrlink 2.  So the miss case is the common case there, and it MUST be loud.  Returns 1 and fills *bytes on a hit; returns 0 and leaves *bytes untouched on a miss -- callers must branch on the RETURN, never on the value. */
int emit_patzeta_frame_reserve(const char * name, int * bytes) {
    if (!name) return 0;
    for (int i = 0; i < pz_n; i++) if (!strcmp(pz[i].name, name)) { if (bytes) *bytes = (((32 + pz[i].fb + 15) & ~15) + pz[i].fp + 16); return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_patzeta_lookup(const char * name, int * susp) {
    if (!name) return 0;
    for (int i = 0; i < pz_n; i++) if (!strcmp(pz[i].name, name)) { if (susp) *susp = (((32 + pz[i].fb + 15) & ~15) + pz[i].fp + 16); return pz[i].uni; }
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
