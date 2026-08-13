#include <string.h>
#include <stdlib.h>
#include "zeta_storage.h"
#include "zeta_choices.h"
#include "ast.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern const char * bb_op_name(IR_e k);
extern int is_global(const char *);
extern int rt_proc_is_registered(const char *);
extern int rt_proc_is_generator(const char *);
static int zls_callee_is_gen(const IR_t * nd) { const char * fn = IR_LIT(nd).sval; return fn && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZLS_MAX_ENTRIES 65536
#define ZLS_FC_SYNTH    0x7F000
#define ZLS_MAX_FIELDS  131072
#define ZLS_MAX_SCOPES  4096
#define ZLS_MAX_GRAPHS  4096
#define ZLS_MAX_VSLOTS  4096
#define ZLS_MAX_MARKS   65536   /* SN4 (2026-07-22): was 8192. Each entry-in-main DEFINE (labelled-range-in-main idiom) re-lowers the full statement array to build its own correctly-framed graph, re-marking every main label; beauty (~163 labels x ~39 such DEFINEs) blows 8192 mid-lowering. Sharing main's graph would cut the marks but hands a called DEFINE main's oversized frame (SIGBUS at scale) — so per-DEFINE graphs stay and the table grows instead (~24B/entry). */
typedef struct { const IR_t * nd; int scope_id; int off; int loff; int live; } zls_entry_t;
typedef struct { int scope_id; int off; int size; unsigned char kind; unsigned char audit; const char * what; const IR_t * nd; } zls_pfield_t;
typedef struct { const char * name; int off; } zls_vslot_t;
typedef struct { const IR_graph_t * g; const char * name; int start_n; const IR_t * anchor; } zls_mark_t;   /* anchor: SN4-FLAT-PROC (s176) -- the label's statement anchor by pointer, orphan-proof emission root (see zls_group_mark_anchor) */
typedef struct { const IR_graph_t * g; const char * name; int first_scope; int n_scopes; int nslots; int region; int resume_off; int zeta_mark_off; int locals_off; int first_vslot; int n_vslots; int pl_trail_mark_off; } zls_graph_t;   /* PL-FR-4: pl_trail_mark_off is the frame slot of the first $trail_mark call's result (set in zls_assign_flat_prolog by scanning for the first IR_CALL_BUILTIN_PROLOG with sval==$trail_mark; -1 if absent). Baked by bcps_spine_gen_arm via zls_g_pl_trail_mark_by_name() to know where to restore the trail mark in a fresh callee zframe during β-resume. */
static zls_entry_t  ze[ZLS_MAX_ENTRIES];  static int ze_n = 0;
static zls_pfield_t zf[ZLS_MAX_FIELDS];   static int zf_n = 0;
static zls_scope_t  zs[ZLS_MAX_SCOPES];   static int zs_n = 0;
static zls_graph_t  zg[ZLS_MAX_GRAPHS];   static int zg_n = 0;
static zls_vslot_t  zv[ZLS_MAX_VSLOTS];   static int zv_n = 0;
static zls_mark_t   zm[ZLS_MAX_MARKS];    static int zm_n = 0;
static zls_entry_t * zx[ZLS_MAX_ENTRIES]; static int zx_n = 0;
typedef struct { const IR_t * nd; int min_off; int span; int zq[8]; int nzq; } zls_ageom_t;   /* zq/nzq (s141 ARBNO-NOFILL): ζ offsets of body IR_MATCH_ASSIGN_SAVE cells — the implicit-zero citizens (rt_cap head cells, empty==zero BY DESIGN, s139 class) the chain-β must still zero per element */
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
/* SN4-FLAT-PROC (s176): the anchor-carrying mark.  The label's statement anchor is captured BY POINTER at lower time because the optimizer's GOTO-fold orphans anchors with no static in-edge (all[] slot
 * nulled, node object alive, registry/edges still point through it) -- exactly the runtime-enterable chain heads.  The emitter seeds these as chain-BFS roots (codegen_flat_chain_body), giving mode 4
 * the same enterable set mode 3 reaches lazily through the runtime label registry; γ-ordered entry keeps every fallthrough assumption intact. */
void zls_group_mark_anchor(const IR_graph_t * g, const char * name, const IR_t * anchor) {
    if (!g || !name) return;
    if (zm_n >= ZLS_MAX_MARKS) { fprintf(stderr, "zls: mark table overflow (%d)\n", ZLS_MAX_MARKS); abort(); }
    zm[zm_n++] = (zls_mark_t){ g, name, g->n, anchor };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_group_count(const IR_graph_t * g) { int c = 0; for (int i = 0; i < zm_n; i++) if (zm[i].g == g) c++; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const IR_t * zls_g_group_anchor(const IR_graph_t * g, int k) { int c = 0; for (int i = 0; i < zm_n; i++) if (zm[i].g == g) { if (c == k) return zm[i].anchor; c++; } return (const IR_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_locals_shifted(IR_e op);
static void zls_entry(const IR_t * nd, int scope_id, int off) {
    if (ze_n >= ZLS_MAX_ENTRIES) { fprintf(stderr, "zls: entry table overflow (%d)\n", ZLS_MAX_ENTRIES); abort(); }
    ze[ze_n] = (zls_entry_t){ nd, scope_id, off, off + (zls_locals_shifted(nd->op) ? 16 : 0), 1 };   /* ZB-VAL-8b: live DEFAULTS to 1 -- every entry site that is not the elide path (which stamps the measured value below) stays conservatively READ, so the use predicate can only ever REMOVE work that was proven dead, never assume deadness it did not measure */
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
        /* BB-OWNED-ζ statement-scope pivot (this session): the existing 16B grant only ever used its first
         * 4 bytes (head.cursor, an int32 counter read/written as FR(op_off) by bb_match_begin.cpp) -- 12 bytes
         * of padding sat unused.  Reusing 8 of those bytes (off+8) for the saved rt_zls_mark() pointer needs
         * NO change to the slot count (still "return 1", still one 16B quad) and therefore cannot shift any
         * later node's offset -- the safest possible way to add a field.  ZK_PTR_GC to match the precedent
         * set by IR_MATCH_ARBNO's own COLLECTION pointer (arbno2.COLLECTION ptr below): both are pointers
         * into the SAME zeta arena, whose address-range rooting (rt_zls_alloc's GC_add_roots widening,
         * zeta_alloc.c) already covers them regardless of tag, but the tag is recorded honestly for
         * whichever future consumer reads it, not left as an untagged raw word next to a tagged sibling. */
        zls_field(scope_id, off, 4, ZK_RAW, 0, "head.cursor (ZB-FC-3d granted: the LIVE anchor lives in HEAD's self-pushed 32B rsp cell at [rsp+0] via the op_fc_wbytes window; this FLAT +0 then holds the RELEASE-stashed match START read by IR_MATCH_REPLACE -- same logical offset, window-disambiguated, so REPLACE's template is unchanged both paths)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "head.zeta_mark (BB-OWNED-zeta statement-scope saved rt_zls_mark() pointer; ZB-FC-3d granted: cell-resident at [rsp+8])", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "head.zls2_mark (ZC_PORT_ALLOC only: saved rt_zls2_mark() cursor; released by head's own omega-choke on failure / IR_MATCH_END on success — the ZLS2 twin of head.zeta_mark, widened to a second quad because the first quad's padding is spent.  ZB-FC-3d granted: cell-resident at [rsp+16] holding the PRE-PUSH rsp, so the S10e unwind releases HEAD's cell and every suspended pattern cell in one mov)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "head.end (SN4-REPL: end cursor stashed by IR_MATCH_END when the statement carries a replacement, read by IR_MATCH_REPLACE; ZB-FC-3d: FLAT on both paths -- post-unwind lifetime)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "head.dcap_mark (REG-6 PEND-PROMOTE: α saves live-r12 pend top = this match's MARK; ω/RELEASE truncate r12 from it — the cell [RT_DCAP_TOP] is now seed-source only, prologue-read, never written mid-match; ZK_RAW — points into the base-pinned dcap island, never GC-moved.  ZB-FC-3d: FLAT on both paths -- RELEASE's post-unwind pump reads it after the match dies)", nd); zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "head.incoming____ (DEAD at REG-2 — ___ is no longer the pend cursor, nothing saves or restores here; slot left ALLOCATED v1 so op_off accounting does not ripple, reclaim is a named follow-up.  Re-tenants at REG-3 as the frame-___ era's saved-outer-___ if the wire-header [+24] route changes)", nd); zls_field(scope_id, off + 48, 8, ZK_RAW, 0, "head.sigma_save (PATCTX, Lon directive 2026-07-29: HEAD saves the OUTER match's Σ=r13 at α before rt_match_enter installs the new subject; BOTH exits restore -- head's own omega-choke on failure, release_pump's tail on success -- and re-sync the C-side Σ/Σlen mirror via rt_match_ctx_restore, so nested matches (deferred *F() evaluation, EVAL/CODE, pump-committed *VAR proc bodies that themselves match) are LIFO-sound by construction.  ZK_RAW deliberately: Σ is register-resident today and registers are GC-invisible regardless; when ZHEAP moves strings THIS slot is exactly where a suspended subject becomes a findable root -- retag interior-PTR_GC in that rung.  KNOWN BYPASS, named not hidden: pat_seal kills (ABORT, FENCE-seal) jump straight to fJ and skip the omega-choke, so they skip this restore the same way they already skip the zls unwind and CAS pop -- the ___ match-frame rung is what closes that class)", nd); zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "head.delta_save (PATCTX: outer δ=r14)", nd); zls_field(scope_id, off + 64, 8, ZK_RAW, 0, "head.Delta_save (PATCTX: outer Δ=r15)", nd); zls_field(scope_id, off + 72, 8, ZK_RAW, 0, "head.capgen_save (PATCTX-2: the OUTER match g_cap_gen id, read at alpha before rt_match_enter draws a fresh id from the monotonic well; both exits restore it through rt_match_ctx_restore -- nest1 autopsy: the inner match stamp invalidated the outer SAVE bracket, pop no-opd, top returned 0, R captured [0,end).  Occupies the former pad quad, so the 5-quad grant and every downstream offset are unchanged)", nd); return 5;
    case IR_MATCH_SPAN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "span.cnt/cur", nd); return 1;
    case IR_MATCH_BAL:
        /* SN4-BAL (s34): n / entry-δ / paren-depth.  The SPAN shape, not the ARB shape: BAL is a pure in-frame
         * generator that pushes no ζ activation, so it takes NO ZLS2 grant (zls2_geom returns 0 for it). */
        zls_field(scope_id, off, 16, ZK_RAW, 0, "bal.n/start/depth", nd); return 1;
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "break.cnt/cur", nd); return 1;
    case IR_MATCH_FENCE1:
        /* SYNC-POINT ζ RELEASE (Lon ruling s132; sync point 2 = FENCE1 = FENCE(P) success exit.  s137 over-seal: interior FENCE0 now shares this op as an operand-free ival=0 sync box — it takes this
         * quad grant uniformly but its template arm never reads it, the whack target being the ___ activation floor; first-position FENCE0 stays node-free).  One quad: +0 holds
         * the α-recorded watermark — the raw rsp under the FORTH/cstack port (the fenced span's pre-push
         * frontier; ZK_RAW: a machine-stack address, never a GC-scanned arena pointer), the ZLS2 cursor under
         * the ALLOC/INLINE/OWNED ports (the head.zls2_mark caveat applies: arena-cursor tag honesty deferred
         * with the same rationale).  The na_s seal-success glue restores to it, bulk-whacking every ζ cell P
         * retained (its alternatives are invisible on backup — manual ln 4716 — so no β will ever look for
         * them); na_f restores too (defensive exactness: leftward exhaust arrives at mark depth by LIFO, the
         * mov is then the identity).  NO fc_geom registration BY DESIGN: the slot must stay [___+off]
         * (depth-immune) because the σ glue reads it at the DYNAMIC post-P depth. */
        zls_field(scope_id, off, 8, ZK_RAW, 0, "fence.watermark (α-saved rsp under FORTH / zls2 cursor under ALLOC; σ/φ glue restores)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "fence.pad (unused)", nd); return 1;
    case IR_MATCH_ARB:
        /* ZLS2 second consumer (Claude Sonnet 5, 2026-07-08) -- the "natural sibling" GOAL-SNOBOL4-BB.md names
         * for extending BB-OWNED-zeta past ARBNO. ARB's existing 16B grant already carried 8B of unused pad
         * (bytes 0-3 = matched-length counter, 4-7 = saved start position, 8-15 = pad) -- the SAME shape
         * IR_MATCH_BEGIN's zeta_mark reuse found above, so the ZLS2 activation-block save-slot fits in the
         * EXISTING quad with zero widening: no later node's offset shifts, the safest possible way to add the
         * field (mirrors head.zeta_mark, not ARBNO's wider 2-quad case -- ARB only ever used 8B of its 16B and
         * already had exactly 8B of pad to spend). */
        zls_field(scope_id, off, 8, ZK_RAW, 0, "arb.cnt/cur (matched-length +0 4B, saved-start +4 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "arb.zls2 activation block ptr (save-slot-in-frame, ZC_PORT_ALLOC only: reuses this node's existing pad, same reuse precedent as IR_MATCH_BEGIN.zeta_mark; block itself is a separate ZLS2 allocation, header +0 chains the previous activation's ptr)", nd); return 1;
    case IR_MATCH_REM:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "match.cursor save", nd); return 1;
    case IR_MATCH_DEFER:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "defer.pad (ZS-2 jmp-entry, Lon s58: the fn/frame cell pair is DELETED — the blob is a jmp-entered new activation that self-allocates on rsp with a 32B wire header, so there is nothing to stash and nothing to guard; quad KEPT at 16B so no later node's offset shifts. s137 OVER-SEAL: when IR_t.seal, quad +0 is REPURPOSED as the fence-demarked sync watermark — α stamps rsp there, the γ/ω glues and β bulk-restore it; +8 stays pad)", nd); return 1;
    case IR_MATCH_VALUE:
        /* ZB-FC-VALUE (s186): RETIRED — this quad was never read or written by anything.  The grant predates the
         * measurement: bb_match_value.cpp addresses only FR(op_a_slot) (operand[0]'s slot) and FR(op_scan_head_off)
         * (a cross-box scan field), the emit arm sets op_off = -1 outright, and the resume continuation is already
         * pushed on rsp (x86_sub rsp,8 + push rax / add rsp,16) — the field's own comment conceded "nothing to stash
         * here" and kept 16B purely so later offsets would not shift.  s185 made that reason obsolete by deriving
         * every node extent from zls_result_off() + zls_node_bytes() instead of hardcoded arithmetic, so the quad can
         * go.  A box owning no private RW state has a ZERO cell as its FORTH form (the ZERO-LAW mechanism, same as
         * LEN/ANY/NOTANY/POS) — this closes the SNOBOL4 IR_MATCH_* family, last unconverted kind. */
        return 0;
    case IR_MATCH_TAB: case IR_MATCH_RTAB:
        /* UNIFORM-BETA WIRING (Claude, this session, per Lon "EVERY BB must be wired properly"): TAB/RTAB
         * OVERWRITE r14d (mov, not add) — the only match primitives whose cursor effect is unrecoverable by
         * recomputation, so their β-restore needs a saved copy.  Same 16B "cursor save" shape as REM above
         * (α: mov FR(off), r14d; β: mov r14d, FR(off)); POS deliberately EXCLUDED — it mutates nothing and
         * is the canonical pure box (needs no RW data, touches no ζ). */
        zls_field(scope_id, off, 16, ZK_RAW, 0, "tab.cursor save (+0 4B r14d saved at α, restored at β; +4 pad)", nd); return 1;
    case IR_MATCH_ARBNO:
        if (IR_LIT(nd).ival == 1) { zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno.owner quad: entry/yield/i/cap (4x4B; SN4-NARY-ARBNO one-node form)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "arbno.COLLECTION ptr (rt_zcol_push-grown per-iteration elements: 16B header {prev_view, saved_delta} + body slot window; the rsp flavor = linked frame chain + explicit count in the header — Lon ruling 2026-07-12, lands at ZB-ITER under ZLS_ARBNO_STACK)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "arbno.saved_rsp (alpha saves rsp here; exhaust L(2) restores it)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "arbno.saved_outer____ (W-1c.2: unconditional — chain-beta always saves MATCH_BEGIN ___ into this slot before repointing ___ as element view; exhaust L(2) always restores it; view-restores at sigma/phi re-derive ___ from rsp unconditionally)", nd); return 3; }   /* W-1c.2: SCRIP_U2 gate removed.  The template always writes slot+32 (save ___ at alpha, restore at exhaust).  Symmetry law: grant=3, template reads +32 on every chain arm invocation. */
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
    case IR_KEYWORD_ICON: case IR_KEYWORD_ICON_GEN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "kw.gen counter", nd); return 1;
    case IR_KEYWORD_SNOBOL4:
        return 0;
    case IR_DEREF: case IR_ASSIGN_VAR: case IR_RANDOM: case IR_SWAP_VAR:
        return 0;
    case IR_KEYWORD_ASSIGN:
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
    case IR_STATEMENT:   /* ZW-5 (O-1 LIGHTING, s23q): IR_STATEMENT is a K=0 pure-whack trailer with no result cell, no locals, and no zls extent.  Without this arm, lc_build's new node receives a phantom 16-byte result slot in the flat layout, shifting every subsequent node's zls offset by +16 and inflating the UCLAIM span by 16 -- the extra 16 bytes corrupt the old____ field in the match frame and cause a post-match segv.  Same class as IR_ASSIGN (the other K=0 sink). */
        return 0;
    case IR_DISJUNCTION:   /* IR_INDIRECT_GOTO retired slice 3 (zero producers) */
        if (nd->op == IR_DISJUNCTION && nd->n_operands > 0) { zls_field(scope_id, off, 8, ZK_RAW, 0, "disj.alt_i live-alternative index (+16 from box base; nary self-state, MOVE_LABEL-ERAD: α=0, φ-glue ++, β dispatches; value DESCR = the box result slot at [base], option-B per-arm copy in σ-glue) (+24 pad)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "disj.pad (unused)", nd); return 1; }
        zls_field(scope_id, off, 8, ZK_PTR_CODE, 0, "gate.stored resume target", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "gate.pad (unused)", nd); return 1;
    /* ZLS-CALL-BASE (MEASURED, not assumed): all three call arms below spoke NODE-BASE while zls_grant_locals is handed the LOCALS base (zls_grant calls it with off+16), so every registered
     * call field sat one quad high; two arms also multiplied the base by the arg index (off*j -- the s92 note below names that typo but repaired only the PROC_GEN/CALL_VALUE arm's SHAPE,
     * keeping its node-base formula, so that arm stayed +16 high).  Emitter truth (bb_call.cpp, plain and by-name-gen arms alike): argbase = resoff+16, arg j at argbase+16*j, extra quad
     * (callgen.act / scan.saved_delta) at resoff+16*(1+nargs); with off = resoff+16 those are off+16*j and off+16*nargs -- one truth.  BEFORE: REPLACE(S,'lo','LO') registered argv at
     * +0/+48/+96 -- a phantom DESCR landing on ANOTHER node's result quad at +0, and a hi_off one quad past the node -- against the emitter's +48/+64/+80; AFTER: exact.  Blast radius was
     * the FIELD MAP, never addressing (the emitter computes argv addresses itself): what was corrupted is scope lo_off/hi_off, zls_node_bytes (-> g_emit.op_own_ci, emit.cpp) and the GC
     * kind map.  Gate: mode-3 crosscheck 314/1 with the fail SET identical to a rebuilt pre-change baseline (not merely an equal count), mode-4 14/14 call-heavy with DIVERGE=0. */
    case IR_CALL_BUILTIN_GEN:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off + 16 * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off + 16 * nd->n_operands, 8, ZK_RAW, 0, "callgen.resume position (alpha=0, runtime writes next start)", nd);
        zls_field(scope_id, off + 16 * nd->n_operands + 8, 8, ZK_RAW, 0, "callgen.pad (unused)", nd);
        return 1 + nd->n_operands;
    case IR_PROC_GEN: case IR_CALL_VALUE:
        /* GENP-SPINE s92 GRANT REPAIR (pre-existing, exposed by t_poison's 0-operand `return`-generator): this arm returned -1 + n_operands — a 0-operand IR_PROC_GEN moved the cursor BACKWARD one unit, so
         * the graph-scope resume/zeta_mark slots landed ON the call's own result DESCR and every value delivery smashed the anchor ([___+zeta_mark+8]) that the ret-epilogue's anchor_leave restores into
         * rsp (rc=139 at graph exit, s91-reproducible).  The unit count is now 2 + n (result + argv + act), matching the staged-gen sibling below; the field offsets drop the off*j multiplication typo for
         * off + 16*(1+j) / off + 16*(1+n) — the act address now equals the emitting arms' formula (off + 16*(1+nargs)) exactly, one truth. */
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
        return 0;   /* SN4-FRAME-DIET (s174): the default-arm phantom is DELETED -- every armless value producer (LIT/VAR/binop class) burned result+16 with no registered field and no reader; audit: every template consuming [op_off+16] (bb_scan_*, bb_match_begin/arbno/release/replace, bb_disjunction, bb_call_value, bb_create, bb_limit, bb_repalt, bb_rev_swap, bb_to*, bb_move_label, bb_indirect_goto, bb_keyword_icon, bb_gen_scan, bb_make_list) owns an explicit arm above, so no default-class kind reads past its result quad */
    }
}
static int zls_is_wiring(IR_e op) { return op == IR_GOTO || op == IR_MOVE_LABEL || op == IR_GOTO_DEFERRED || op == IR_SUCCEED || op == IR_FAIL || op == IR_RETURN || op == IR_SUSPEND || op == IR_CORET || op == IR_COFAIL || op == IR_CUT || op == IR_MATCH_END || op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_GLIT || op == IR_GCC || op == IR_GALT; }   /* RK-GRAM-3d: grammar scanner nodes (GLIT/GCC/GALT) are scanner-register-only with no result cell and no zls extent -- same class as IR_STATEMENT.  Without this they receive a phantom 16B result slot each, inflating UCLAIM span K. */   /* s30: IR_STATEMENT_BEGIN/END join the wiring class (same law as IR_STATEMENT at O-1): both are K=0 pure-relay/whack boxes with no result cell and no local fields; without this they receive a phantom 16-byte result slot each from zls_entry(), inflating the UCLAIM span K by 32 (2x16) and corrupting the match frame's [___+88] old____ restore slot (MEASURED: 058_pat_fence_keyword segfaults with ___/rsp=0x0 at main_gamma; ZW5=0 cures). */   /* IR_STATEMENT (ZW-5, O-1 LIGHTING, s23q): the statement-terminal TRAILER is pure control -- K=0, no result cell, no value produced, no zls extent.  Adding it here (before zls_entry/zls_field fire) prevents the phantom 16-byte result slot that was shifting every subsequent node's offset by +16 and inflating the UCLAIM span.  The `case IR_ASSIGN/IR_STATEMENT: return 0` in zls_grant was too late: zls_entry and the result zls_field both fire unconditionally before the switch, so returning 0 from the switch only prevented k from incrementing -- the node was already registered with a 16-byte field, making zls_node_bytes return 16 and the span include it. */
static int zls_locals_shifted(IR_e op) { return op == IR_MATCH_BEGIN || op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARB || op == IR_MATCH_BAL || op == IR_MATCH_FENCE1 || op == IR_MATCH_ARBNO || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE || op == IR_MATCH_ASSIGN_SAVE || op == IR_SCAN_ENTER || op == IR_INITIAL; }   /* SEQ-ERAD SE-5: IR_MATCH_SEQUENCE removed — zero locals, no front-quad shift */
int fc_arm_member(const IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_cells_on -- Z4-6 (GOAL-ZETA-FOUR): THE ONE PORT OPINION for the whole fixed-cell family.  The s206 root cause (FINDING-2026-07-28 ZHEAP) was PORT-BLIND GRANT vs PORT-GATED CONSUMPTION -- fc_geom
 * granted on every port while x86_fc_on/hit were FORTH-only, so under any non-FORTH port one box got contradictory answers (geometry said carve, consumers said flat, zls_fc_cell netted out the flat
 * quad the box then needed, fc_vwpop counted omega pops for cells never pushed).  Z4-0/Z4-6 measured the consequence: the three pattern probes SEGV under frame-rsp while arith/fib run.  This predicate
 * is consulted at the QUERY SURFACE of every family member below (fc_geom, fc_save_active, fc_vlit_active, fc_cond_fp, fc_head_fp, fc_vwpop, fc_seq_active, fc_leaf_disp, zls_fc_cell), so LOWER's
 * consumers, the emitter's grants, the layout's quad net-out, and the template predicates all read ONE answer regardless of what the side tables registered.  ALLOWLIST {FORTH, HEAP} on purpose:
 * FORTH is the live cell protocol (default -- this gate is a no-op there, byte-identity preserved); HEAP keeps today's in-flight granted-but-broken state UNCHANGED so Z4-8's pre/post SEGV proof
 * stands (Z4-8 refines the HEAP arm here, at the one authority, per its own rung text).  Every other port (CSTACK/PLAIN/INSTRUMENTED/ALLOC/INLINE/OWNED) is ZERO-CELL = the flat frame-slot regime
 * those ports were built for.  Port is process-global and set before any lowering (CLI/env in m3, the bake in m4), so both call phases read the same stable answer -- none of the registration-order
 * hazard zls_fc_cell's own comment documents for the side tables. */
static int fc_cells_on(void) { extern int rt_zeta_port_mode(void); int m = rt_zeta_port_mode(); return m == ZC_PORT_FORTH || m == ZC_PORT_HEAP; }
int fc_cells_active(void) { return fc_cells_on(); }   /* Z4-6 PUBLIC ACCESSOR (RBX census, this session): the one sanctioned external read of the private ONE-PORT-OPINION above. */
static int zls_fc_cell(const IR_t * nd) { if (!fc_cells_on()) return 0; if (!nd) return 0; { extern int fc_arm_member(const IR_t *); if (fc_arm_member(nd)) return 0; } switch (nd->op) { case IR_MATCH_SPAN: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_BREAK: case IR_MATCH_BREAKX: case IR_MATCH_BAL: case IR_MATCH_REM: case IR_MATCH_ARB: return 16; default: return 0; } }   /* PAT$N REGION NET-OUT (s191, Lon directive "99.999% of allocation are now inside the BB's"): the UNCONDITIONAL-cell slice of fc_geom -- these eight kinds each self-push a fixed 16B rsp cell at alpha (x86_asm.h ~1765 arms sub rsp,op_fc_bytes) AND were each granted exactly ONE 16B locals quad in the bulk proc-entry carve, which the fc_hit window rebase then makes UNREACHABLE: FR(off) for off inside [op_fc_base, +16) emits [rsp + off-base], never the flat slot.  Pure double-count, netted out here.  Restricted to the UNCONDITIONAL arms ON PURPOSE: fc_geom's conditional arms (fc_save_active / fc_vlit_active / fc_alt_fpmax) read side tables populated during LOWER, so asking them at zls_build time is registration-order-dependent and could answer 0 here but 16 at emit -- that disagreement would silently misplace a cell.  All eight verified 1:1 (one locals quad, 16B cell) against zls_grant_locals, and all eight are in zls_locals_shifted so the netted quad is the LOCALS quad, never the cross-box-read result quad. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant(const IR_t * nd, int scope_id, int off) {
    if (zls_is_wiring(nd->op)) return 0;
    zls_entry(nd, scope_id, off);
    zls_field(scope_id, off, 16, ZK_DESCR, 0, "result", nd);
    if (zls_fc_cell(nd)) { ze[ze_n - 1].loff = ZLS_FC_SYNTH; return 1; }   /* NET-OUT: locals are cell-resident; loff becomes the SYNTHETIC window base so fc_hit still rebases every own-cell ref to [rsp + off-base] (the base value is arithmetically irrelevant -- it only ever appears as a difference).  Deliberately far past any real region so an ESCAPING ref (a cross-box reader of a netted quad, which the zls_s4_ok audit says does not exist) emits an obviously-wrong [rsp + 520192+] that greps out of the .s and faults loudly, instead of silently aliasing the neighbouring node's live slot. */
    return 1 + zls_grant_locals(nd, scope_id, off + 16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SLOT-ELIDE S1 (Lon ruling s133: "some BB's will have a result and some will not determined by NEED").  SCRATCH-ALIAS, degrade-never-die: a node whose result no one reads gets NO quad of its own —
 * its zx entry aliases ONE shared per-graph scratch quad (the first dead node's own slot becomes it), so templates that self-write results write harmlessly and layout math never sees a hole; frame Σ
 * shrinks by (dead−1)×16 and the prologue stosb with it (s132: the match phase is STORE-BOUND, 63% stores).  TWO safety fences, both whitelists so drift cannot create a wrong answer: (1) only kinds
 * POSITIVELY KNOWN to own zero locals may elide (zls_elide_ok — the default-grant leaf class; a kind with locals keeps its result so the locals@+16 layout law is untouched); (2) liveness marks a node
 * on ANY operand reference EXCEPT from the four SNOBOL4 constructs whose operands are pure entry/resume wiring (ALT/SEQ/ARBNO/FENCE1 + MOVE_LABEL's label operand); DISJUNCTION/HEAD/REPALT/the Icon
 * scan twins stay in the reader class because their operand lists mix wiring with real slot reads (emit.cpp op_parts/op_sa/op_off) — conservative toward LIVE.  SCRIP_SLOT_ELIDE=0 is the kill-switch. */
static int zls_elide_ok(IR_e op) { return op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_LEN || op == IR_MATCH_LIT || op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_CMP_TEST || op == IR_ASSIGN; }   /* ZB-VAL-8b: IR_CMP_TEST joins the zero-locals class -- AUDITED, not assumed: it appears in NO arm of zls_grant_locals (owns no locals, so the locals@+16 layout law is untouched), its template bb_cmp_test.cpp reads only FRQ(op_sa)/FRQ(op_sb)/FRQ(op_off) with no cross-box reader of its front quad, and its control result travels in EAX via test/jcc -- never through the cell.  The cell holds the SPITBOL predicate value (manual p.33: "when any of these functions succeed, they produce a null string value"), which is REAL when read (OUTPUT = LT(A,B) 'yes' feeds it to str_concat_d) and DEAD when the statement consumes only the S/F branches -- exactly the per-instance discrimination zls_mark_value_refs measures */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SLOT-ELIDE S4a (Lon directive s138: "reduce the RSP adjustments to ZERO for BB's whose result is NOT USED and have just the local BB memory if any").  The locals-shifted SNOBOL4 match family's front
 * 16B "result" quad is runtime-DEAD BY CONSTRUCTION: every runtime accessor (drive_value_slot -> nd_slot -> zls_off, bb_prepare's scratch, the fc window bases, sealed-DEFER's watermark repoint) reads
 * the LOCALS-shifted offset; zls_result_off's only consumers are the census and the --dump pretty-printer.  So a DEAD shifted node needs NO front quad at all -- its locals land AT the entry offset
 * (loff = off) and the node shrinks by one quad.  LIVE shifted nodes keep the byte-identical legacy layout (result quad + locals@+16), so whatever conservative-live reader exists sees no change.
 * WHITELIST = kinds positively audited to hold ALL runtime state in their zls_grant_locals fields with no cross-box reader of the front quad: SPAN/BREAK/BREAKX/TAB/RTAB/REM/BAL (fc-cell matchers),
 * ALTERNATE/SEQUENCE (entry-cursor+index quads), FENCE1 (watermark quad), DEFER/VALUE (pad quads; sealed DEFER's watermark IS the pad quad via zls_off).  EXCLUDED: HEAD (RELEASE/REPLACE cross-box flat
 * reads), ARBNO (body-window geometry + COLLECTION), ARB (zls2 save-slot), ASSIGN_SAVE (COND cross-reads), SCAN_* (Icon scans use the front quad as the value DESCR -- "the value DESCR is the box
 * result slot at [base]"), INITIAL.  Same SCRIP_SLOT_ELIDE=0 kill-switch reverts to zls_grant wholesale. */
static int zls_s4_ok(IR_e op) { return op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_BAL || op == IR_MATCH_ALTERNATE || op == IR_MATCH_FENCE1 || op == IR_MATCH_DEFER || op == IR_MATCH_VALUE || op == IR_CALL_BUILTIN_PROLOG; }   /* SEQ-ERAD SE-5: IR_MATCH_SEQUENCE removed — zls_grant_locals returns 0, no dead front quad to elide */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zls_mark_value_refs(const IR_graph_t * g, char * live) {
    for (int k = 0; k < g->n; k++) { const IR_t * c = g->all[k]; if (!c) continue;
        if (c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_FENCE1 || c->op == IR_MOVE_LABEL) continue;   /* ARBNO deliberately NOT here: operands[2] geometry bracket is a REAL slot read (s133 crosscheck caught it — 075/164/167/W04 arbno family).  COND/IMM operands[0] excluded PER-INDEX below (s21x-p, the roman LEN ghost-cell finding): lower_snobol4.c ~1327/~1355 pushes "[0] inner entry" -- the backtrack-in WIRING edge -- and "[1] SAVE" -- the ZB-FC-3c cross-box slot read.  [0] is control, [1] is value; a blanket kind exclusion would kill the SAVE liveness the s4 audit protects, so the skip is (kind, j==0), not (kind). */
        for (int j = 0; j < c->n_operands; j++) { const IR_t * p = c->operands[j]; if (!p) continue; if (j == 0 && (c->op == IR_MATCH_ASSIGN_COND || c->op == IR_MATCH_ASSIGN_IMM)) continue; for (int i = 0; i < g->n; i++) if (g->all[i] == p) { live[i] = 1; break; } } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_grant_elide(const IR_t * nd, int scope_id, int off, int live, int * scratch_off) {
    if (zls_is_wiring(nd->op)) return 0;
    if (!live && zls_elide_ok(nd->op)) {
        if (*scratch_off < 0) { *scratch_off = off; zls_entry(nd, scope_id, off); ze[ze_n - 1].live = 0; zls_field(scope_id, off, 16, ZK_DESCR, 0, "result (SLOT-ELIDE shared dead-result scratch — every later dead leaf in this graph aliases here)", nd); return 1; }
        zls_entry(nd, scope_id, *scratch_off); ze[ze_n - 1].live = 0; return 0;
    }
    if (!live && zls_s4_ok(nd->op)) { zls_entry(nd, scope_id, off); ze[ze_n - 1].loff = off; ze[ze_n - 1].live = 0; if (zls_fc_cell(nd)) { ze[ze_n - 1].loff = ZLS_FC_SYNTH; return 0; } return zls_grant_locals(nd, scope_id, off); }   /* PAT$N REGION NET-OUT (s191): S4a already dropped the dead front quad, so a netted-out DEAD matcher now consumes ZERO flat quads -- the whole node lives in its own rsp cell.  This is the arm that drives an all-converted graph's region to 0 and lets the DROP-PROLOGUE rung delete its sub rsp entirely. */
    int ei = ze_n; int n = zls_grant(nd, scope_id, off); if (ze_n > ei) ze[ei].live = live; return n;   /* zls_grant_locals adds FIELDS, never entries, so the node's own entry is exactly ze[ei] */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-VAL-8b USE PREDICATE (Lon directive s182: "allocate its RESULT value, IF it has one and if it is used").  The measured answer to "is THIS INSTANCE's result cell ever read as a VALUE", which the IR
 * itself cannot express: IR.h carries no n_uses/use_count/consumed field and ir_node_produces_value() is OPCODE-keyed (it answers "can this KIND produce a value", never "is this instance's value read").
 * zls_mark_value_refs already computes exactly this fact -- an operands[] reference is a VALUE use, a gamma/omega wire is a CONTROL use and grants nothing -- but until now it died as a local array inside
 * zls_build after steering layout alone.  Persisting it on the entry lets the EMITTER ask, so a box whose result no one reads can also skip WRITING it.  Conservative by construction: unknown node -> 1. */
int zls_result_live(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->live : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_node_off(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; return e ? e->off : -0x40000000; }   /* UCLAIM (wholesale flip, Lon directive): the node's flat-plan base offset, the owner-table key for the ONE execution-order offset resolver in emit.cpp -- sentinel -0x40000000 = no plan entry, no claim, resolver never consults it. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_scope_new(int parent, int klass, const char * name) {
    if (zs_n >= ZLS_MAX_SCOPES) { fprintf(stderr, "zls: scope table overflow (%d)\n", ZLS_MAX_SCOPES); abort(); }
    zs[zs_n] = (zls_scope_t){ zs_n, parent, klass, name, -1, 0, 0x7fffffff, 0 };
    return zs_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fct_fp_range / fct_leaf_range -- R12-EXIT-1 L1 (the s69 ALT-in-body lift): the FINALIZE-PASS twins of lower_snobol4.c's fc_walk_range/fc_leaf_walk ALT arms, verbatim shape.  A GRANTED ALTERNATE
 * contributes 16 (own cell) + fpmax (the S10d padded arm -- only one arm is live at yield) with its arm allocation extent SKIPPED on the spine (the naive fc_geom range sum counts every arm's leaves =
 * the exact over-count the pre-lift fc_tail_walk decline existed to avoid; fc_geom(granted ALT)=16, so the ALT case MUST run before the fc_geom catch).  Leaf displacements: the ALT itself registers
 * at pfx+16+bias (window refs bypass, uniformity only); each ARM restarts at pfx+16 (arms are alternatives on top of the ALT's cell, not concatenation -- recursion covers nested granted ALTs, which
 * fc_alt_register admits at statement level); nodes AFTER the ALT continue at pfx+16+fpmax (the pad stubs' uniform yield depth).  bias carries the region formula (0 left-flat / -bmn body /
 * fpb+span-rmn right); skip = the ARBNO node itself (registers nothing; its tail arm speaks raw [rsp+const]).  An ungranted ALT cannot reach a finalized candidate (fc_tail_walk declined the
 * statement), so the fc_alt guard here is belt-and-braces. */
int fc_alt_fpmax(const IR_t *);
int fc_alt_extent(const IR_t *, int *, int *);
int fc_alt_n(const IR_t *);
int fc_alt_arm_range(const IR_t *, int, int *, int *);
int fc_geom(const IR_t *, long *);
int emit_patzeta_lookup(const char *, int *);
static int fct_pricing = 0;   /* PS-3 s153: 1 ONLY inside the fct ARBNO finalize below -- the defer-as-leaf SUSP pricing must never leak into the other fct_fp_range consumer (zls_g_fp_total, which
                               * feeds emit_patzeta_register's own fp term: pricing there would make a registered fp depend on registration ORDER via the mid-computation lookups -- a silent
                               * mode-divergence mine.  Context-gated, the pricing is confined to finalize, where the registry is complete for every reachable target by loop order). */
static int fct_defer_susp(const IR_t * nd) {   /* the licensed defer's compile-time retention: SUSP = align16(32+fb)+fp+16 via the emit_patzeta registry keyed by the node's PAT$ operand literal;
                                                * -1 = not licensed here (no PAT$ operand / unregistered / non-uniform target) -- the caller declines the candidate wholesale. */
    if (!nd || (nd->op != IR_MATCH_DEFER) || nd->seal != 2) return -1;
    const char * pn = 0; for (int j = 0; j < nd->n_operands; j++) { const IR_t * o = nd->operands[j]; if (o && o->op == IR_LIT_STRING && IR_LIT(o).sval && !strncmp(IR_LIT(o).sval, "PAT$", 4)) { pn = IR_LIT(o).sval; break; } }
    int susp = 0;
    if (!pn || !emit_patzeta_lookup(pn, &susp) || susp <= 0) return -1;
    return susp;
}
static int fct_rsp_range(IR_graph_t * g, int k0, int k1) {
    /* M-2 BUG-5 FIX: fpl = actual rsp-push bytes of left-spine nodes (ARBNO arrival depth from MATCH_BEGIN).
     * fct_fp_range uses fc_geom which returns 0 for flat-allocated nodes (LIT_INTEGER, etc.) that DO push rsp.
     * Must mirror zd_k from emit.cpp (THE ONE K AUTHORITY) — K=0 for the listed kinds, K=16 for all others.
     * This function is ONLY called for fpl (left-spine); fpb/fpr keep fct_fp_range (they drive slot offsets). */
    int rsp = 0;
    for (int j = k0; j < k1 && j < g->n; j++) {
        IR_t * x = g->all[j];
        if (!x) continue;
        int op = (int)x->op;
        if (op == IR_ASSIGN || op == IR_GOTO || op == IR_GOTO_DEFERRED || op == IR_SAVE_RESTORE ||
            op == IR_MATCH_BEGIN || op == IR_MATCH_END || op == IR_MATCH_REPLACE ||
            op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END ||
            op == IR_MATCH_LIT || op == IR_MATCH_LEN || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY ||
            op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_ASSIGN_COND ||
            op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE || op == IR_MATCH_ALTERNATE ||
            op == IR_MATCH_FENCE1 || op == IR_BOUND || op == IR_UNMARK || op == IR_CONJUNCTION ||
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
static int fct_fp_range(IR_graph_t * g, int k0, int k1) {
    int fp = 0; long fck = 0;
    for (int j = k0; j < k1 && j < g->n; j++) {
        IR_t * x = g->all[j];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) {
            int _b = 0, _e = 0;
            if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > j + 1) j = _e - 1; }   /* ALT-FLAT s202: zero-cell ALT + flat arms contribute 0; extent still skipped off the spine */
            continue;
        }
        if ((x->op == IR_MATCH_DEFER) && fct_pricing) { int s = fct_defer_susp(x); if (s > 0) fp += s; continue; }   /* PS-3 s153: the licensed defer is a leaf of size SUSP (finalize pre-scan proved s>0 for every defer in range) */
        if (fc_geom(x, &fck)) fp += (int)fck;
    }
    return fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SLOT-ELIDE S0 CENSUS (Lon ruling s133: unused result quads are bug #1).  SCRIP_SLOT_CENSUS=1 prints, per graph, how many result quads exist vs how many are ever referenced as a VALUE operand —
 * operand references FROM the pure-wiring construct kinds (ALT/SEQ/ARBNO/FENCE1/scan twins/REPALT/MOVE_LABEL: their operands are entry/resume NODES, not value reads) do not count.  DISJUNCTION and
 * MATCH_BEGIN are deliberately left in the value-reader class (their operand lists mix wiring with real reads), so the dead count is a conservative FLOOR on the S1 win.  O(n²) walk — census only. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_fct_finalize(IR_graph_t * g, int late);
void fc_vlit_register(const IR_t *); void fc_vread_register(const IR_t *, int); void fc_vbinop_register(const IR_t *); int fc_vcap(int, int, int, int); int is_global(const char *); void fc_vwpop_register(const IR_t *, long); void fc_subj_register(const IR_t *); int zc_nofc(void);
static int fc_vvar_ok(const IR_graph_t * g, const IR_t * r) { const char * vn = IR_LIT(r).sval; return vn && vn[0] != '&' && ((is_global(vn) && !graph_has_local(g, vn)) || !strcmp(vn, "write") || !strcmp(vn, "writes")); }   /* ZB-VAL-3/5: MIRRORS the IR_VAR walk routing -- bb_var_global only */
static int fc_vbinop_ok(long long v) { return v == 0 || v == 1 || v == 2 || v == 3 || v == 4 || v == 11 || v == 18; }   /* ZB-VAL-4/6a widened s21x-e (Lon ACROSS-THE-BOARD): ADD/SUB/MUL/CONCAT + DIV(3)/MOD(4)/POW(18).  The prior DIV/MOD decline ("the lean fc arm has no idiv fault path") is STALE vs the collapsed one-per-op-call arm (bb_binop_arith rtop dispatch): non-fast ops take the generic rt_num_arith call whose DT_FAIL lands x86_omega -- the fault path IS the arm's default, measured on 026/027 (the original regime casualties) this session.  Relops stay out (ZB-VAL-7's own zero-or-one-result shape) */
static int fc_vunop_ok(const IR_t * nd) { return nd->op == IR_UNOP && nd->n_operands == 1 && ((int)IR_LIT(nd).ival == TT_MNS || (int)IR_LIT(nd).ival == TT_PLS); }   /* ZB-VAL-6b: SNOBOL4's ONLY arithmetic unaries (manual p.181 -- unary * is DEFER, not size; ~ ? @ $ . & are pattern/name operators with their own shapes).  Both route to the bb_unop rt_num_neg/rt_num_pos tail, which is a single infallible call: the operand cell IS the result cell, so the box nets ZERO and emits no rsp instruction */
static int g_fcc_gfence = 0;   /* CALL2BB 3b v1 graph fence: call registration confined to STATIC NON-BODY graphs (no RETURN/FRETURN floaters = not a DEFINE body, no deferred/indirect transfer, no EVAL/CODE dynamics, no pattern statements: deferred *F() re-entry under an armed bracket is the 140/141 casualty -- patterns join at SUBJECT-CELL rung (a)) -- the s21x-i gate-ON regression set {088, 1010 recursion; 140/141 EVAL} is exactly the excluded class, named follow-ups */
static int fc_call_ok(const IR_t * nd) { static int on = -1; if (on < 0) { const char * a = getenv("SCRIP_STMT_FRAME"); const char * b = getenv("SCRIP_CALL2BB"); on = (a && *a == '1' && b && *b == '1') ? 1 : 0; if (on) { const char * c = getenv("SCRIP_CALL2BB_FC"); if (!(c && *c == '1')) on = 0; } } if (!on || !nd) return 0; { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) { const char * fn = IR_LIT(nd).sval; extern int rt_proc_is_registered(const char *); extern int rt_builtin_is_known(const char *); fprintf(stderr, "[FCC] op=%d nops=%d sval=%s reg=%d blt=%d\n", (int)nd->op, nd->n_operands, fn ? fn : "(null)", (fn && fn[0]) ? rt_proc_is_registered(fn) : -1, (fn && fn[0]) ? rt_builtin_is_known(fn) : -1); { extern int rt_proc_is_generator(const char *); if (fn && fn[0]) fprintf(stderr, "[FCC] gen=%d\n", rt_proc_is_generator(fn)); } } } if (!(nd->op == IR_CALL || nd->op == IR_CALL_PROC_STAGED)) return 0; if (nd->n_operands != 1 || !nd->operands[0]) return 0; { const char * fn = IR_LIT(nd).sval; extern int rt_proc_is_registered(const char *); extern int rt_proc_is_generator(const char *); extern int rt_builtin_is_known(const char *); if (!fn || !fn[0] || !strcmp(fn, "CODE") || rt_builtin_is_known(fn) || !rt_proc_is_registered(fn) || rt_proc_is_generator(fn)) return 0; } return 1; }   /* CALL2BB 3b v1 (s21x-i): ONE-ARG registered non-generator user-proc call as a value-spine member -- gated on BOTH envs so gate-off registration is ZERO (byte-identity); mirrors bb_call_route_classify's line-752 staged route (sval + registered + !generator, builtins excluded) so planner and emitter agree; registry-empty-at-plan fails SAFE (under-register -> decline -> flat path, never wrong code).  N>=2 (arg-order inversion vs the affine window), generator callees (second L(2) protocol), and the dc pointer-arg arm are the named follow-ups. */
static int fc_vtree_scan(const IR_graph_t * g, const IR_t * nd, const IR_t ** post, int * pn, int cap, int depth) {
    if (!nd || depth > 24 || *pn >= cap) return 0;
    if (nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || (nd->op == IR_VAR && fc_vvar_ok(g, nd))) { post[(*pn)++] = nd; return 1; }   /* ZB-VAL-6a: STRING leaves join INTEGER inside trees -- concat's natural leaf; the cell is a type-blind 16B DESCR either way (bb_lit_scalar is one FRQ-relative body for all four scalar arms, so the fc rebase already serves it -- ZB-VAL-2 proved this at the pair) */
    if (fc_vunop_ok(nd) && fc_vtree_scan(g, nd->operands[0], post, pn, cap, depth + 1) && *pn < cap) { post[(*pn)++] = nd; return 1; }
    if (fc_call_ok(nd) && g_fcc_gfence && nd->operands[0] && (nd->operands[0]->op == IR_LIT_INTEGER || nd->operands[0]->op == IR_LIT_STRING || (nd->operands[0]->op == IR_VAR && fc_vvar_ok(g, nd->operands[0]))) && *pn + 2 <= cap) { post[(*pn)++] = nd->operands[0]; post[(*pn)++] = nd; return 1; }   /* CALL2BB 3b v1: the call takes ONE DIRECT-LEAF arg (lit or registrable global var -- no subtree: binop-arg trees are the 088/1010 recursion casualty class) inside a fenced static graph; unop-shaped on the spine, registers into fvcl below */
    if (nd->op == IR_BINOP && nd->n_operands == 2 && fc_vbinop_ok((long long)IR_LIT(nd).ival)
        && fc_vtree_scan(g, nd->operands[0], post, pn, cap, depth + 1) && fc_vtree_scan(g, nd->operands[1], post, pn, cap, depth + 1) && *pn < cap) { post[(*pn)++] = nd; return 1; }
    return 0; }   /* ZB-VAL-4/5: post-order collect + validate -- int-lit and global-var leaves, ADD/SUB/MUL internals, depth-capped (24 x 16B = 384B max rsp excursion) */
void zls_build(IR_graph_t * g) {
    if (!g) return;
    { int f = 1; for (int i = 0; i < g->n && f; i++) { IR_t * x = g->all[i]; if (!x) continue; if (x->op == IR_GOTO_DEFERRED || x->op == IR_INDIRECT_GOTO || x->op == IR_MATCH_BEGIN || x->op == IR_MATCH_DEFER) f = 0; else if (x->op == IR_SAVE_RESTORE) { long long v = IR_LIT(x).ival; if (v == 1 || v == 2) f = 0; } else if ((x->op == IR_CALL_BUILTIN || x->op == IR_CALL_BUILTIN_SNOBOL4 || x->op == IR_CALL) && IR_LIT(x).sval && (!strcmp(IR_LIT(x).sval, "EVAL") || !strcmp(IR_LIT(x).sval, "CODE"))) f = 0; } g_fcc_gfence = f; }
    for (int vi = 0; vi < g->n; vi++) { IR_t * a = g->all[vi]; if (!(a && a->op == IR_ASSIGN && a->n_operands == 1 && a->operands[0])) continue;   /* ZB-VAL-0/1: POST-OPTIMIZER value-spine scan -- lower-time pointer registration dies to node rebuild/fold; gamma-adjacency IS the fence */
        { const char * vn = IR_LIT(a).sval; if (!(vn && is_global(vn) && !graph_has_local(g, vn))) continue; }   /* ZB-VAL-1: consumer MUST route to bb_assign_global (the only vfc release arm) -- mirrors the walk's routing predicate; a local-assign consumer would strand the carve (closes the ZB-VAL-0 latent) */
        IR_t * r = a->operands[0];
        if ((r->op == IR_LIT_INTEGER || r->op == IR_LIT_STRING || r->op == IR_LIT_REAL || r->op == IR_LIT_CHARSET
             || (r->op == IR_VAR && IR_LIT(r).sval && IR_LIT(r).sval[0] != '&' && ((is_global(IR_LIT(r).sval) && !graph_has_local(g, IR_LIT(r).sval)) || !strcmp(IR_LIT(r).sval, "write") || !strcmp(IR_LIT(r).sval, "writes"))))
            && r->γ.node == a && fc_vcap(1, 1, 0, 0)) { fc_vlit_register(r); fc_vread_register(a, 0); continue; }   /* ZB-VAL-0/2/3 pair: scalar lit OR global-routed var -> assign, disp 0 exact -- the cell is a type-blind 16B DESCR; bb_lit_scalar and bb_var_global both write FRQ(op_off)-relative so the fc_hit rebase serves them identically; the var gate MIRRORS the IR_VAR walk routing (bb_var_global only -- '&' keywords and locals stay flat) so registration <=> the one template with hook-served exits */
        if ((r->op == IR_BINOP || fc_vunop_ok(r) || fc_call_ok(r)) && r->γ.node == a) {   /* ZB-VAL-4/5/6a/6b: ARBITRARY tree over int/string-lit and GLOBAL-VAR leaves -- post-order emission makes every binop's operands the TOP TWO cells, so the [rsp+24]/[rsp+8] + net add rsp,16 geometry is SHAPE-INVARIANT; a unop's single operand is the TOP cell and its net is ZERO.  The fc binop arm carries the FULL type structure (fast/overload/generic) because a var cell holds whatever the global holds.  FALLIBLE boxes (vars: NV DT_FAIL edge; binops: rt_num_arith DT_FAIL) get a REGISTERED TOTAL POP from the depth simulation below -- their omega must release EVERY cell live in the statement, not just their own (own 16B rides the fc hook; the remainder rides op_wpop, the BP-9 summed-pop mechanism). */
            const IR_t * post[49]; int pn = 0;
            int _ts = fc_vtree_scan(g, r, post, &pn, 49, 0);
            { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg && _ts && pn > 0) fprintf(stderr, "[FCC] tree ts=%d pn=%d tail_is_r=%d\n", _ts, pn, (post[pn-1] == r) ? 1 : 0); }
            if (_ts && post[pn - 1] == r) {
                int ok = 1, L = 0, B = 0;
                for (int i = 0; i + 1 < pn; i++) { const IR_t * gx = post[i]->γ.node; if (gx == post[i + 1]) continue; if (gx && gx->op == IR_SAVE_RESTORE && gx->γ.node == post[i + 1] && (post[i + 1]->op == IR_CALL || post[i + 1]->op == IR_CALL_PROC_STAGED)) { long long v = IR_LIT(gx).ival; if (!(v == 1 || v == 2 || v == 3)) continue; } ok = 0; break; }   /* CALL2BB 3b: the role-0 sr0 prefix (slice 2) is threaded ON THE GAMMA SPINE between the last arg producer and its call (measured: LIT.gamma -> IR_SAVE_RESTORE(role 0) -> CALL) -- it is the call cluster's own protocol box, rsp-balanced across the committed edge by the c2 landings' restore+release, so the adjacency fence treats exactly this shape as transparent; the predicate (gamma-chained + role-0 by UNION-TAG + next-is-the-registered-call-kind) mirrors the emit-side sr0 pairing at the dispatch (nd->gamma keying) verbatim, keeping planner and emitter in structural agreement */
                for (int i = 0; i < pn; i++) { if (post[i]->op == IR_BINOP || post[i]->op == IR_UNOP) B++; else L++; }   /* ZB-VAL-6b: unops share the fvb operator registry, so they must be counted against ITS cap, not the leaf cap -- a miscount here is the exact shape of the ZB-VAL-0 partial-quartet latent */
                { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg && pn > 1) fprintf(stderr, "[FCC] ok=%d cap=%d p0g=%p p1=%p\n", ok, fc_vcap(L, 1, B, pn), (void*)(pn > 1 ? post[0]->γ.node : 0), (void*)(pn > 1 ? post[1] : 0)); if (dbg && pn > 1 && post[0]->γ.node) fprintf(stderr, "[FCC] p0=%p p0op=%d p0g_op=%d r=%p rop=%d\n", (void*)post[0], (int)post[0]->op, (int)post[0]->γ.node->op, (void*)r, (int)r->op); }
                if (ok && fc_vcap(L, 1, B, pn)) {
                    int d = 0;
                    for (int i = 0; i < pn; i++) { const IR_t * x = post[i];
                        if (x->op == IR_BINOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); d -= 1; }
                        else if (x->op == IR_UNOP) { fc_vbinop_register(x); fc_vwpop_register(x, (long)d * 16); }
                        else if (x->op == IR_CALL || x->op == IR_CALL_PROC_STAGED) { fc_call_register(x); fc_vwpop_register(x, (long)d * 16); }   /* CALL2BB 3b: the call is unop-shaped in the sim -- consumes its 1 arg cell, produces its 1 result cell IN PLACE (net-zero rsp at L(2)), so d is unchanged; at its post-L(2) omega live depth = d cells (unders + own result), all released by wpop exactly like the binop's total-pop contract */
                        else { fc_vlit_register(x); if (x->op == IR_VAR && d > 0) fc_vwpop_register(x, (long)d * 16); d += 1; } }
                    { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) fprintf(stderr, "[FCC] REGISTERED pn=%d\n", pn); }
                    fc_vread_register(a, 0); } } } }   /* the simulation: d = cells live BEFORE the node's alpha.  A leaf's own cell pops via the fc hook, so its wpop = d*16 (the cells UNDER it); a binop carves nothing at alpha, so its wpop = d*16 covers ALL live cells including its two operands.  Registration stays capacity-atomic and all-or-nothing: any invalid member (DIV/MOD, local var, '&' keyword, broken chain) declines the WHOLE tree to the flat path */
    { static int subj_on = -1; if (subj_on < 0) { const char * b = getenv("SCRIP_SUBJ_CELL"); subj_on = (b && *b == '0') ? 0 : 1; }   /* SUBJECT-CELL rung (b), s22y DEFAULT-ON (killswitch SCRIP_SUBJ_CELL=0, the zc_nofc polarity): proven strictly-better-by-set at flip time -- crosscheck 318, m3 220->233 m4 217->229, ZERO broken either mode, decline set carrying the dynamic-box/FENCE1 graphs flat-verbatim.  STMT_FRAME conjunct DROPPED same session: it rode the s21x era when STF was default-ON; one env, one pair. */   /* SUBJECT-CELL rung (a) (Lon s21x directive "NO FRAME RELATIVE addressing for operands" -- the s21x-e measured frontier: LIT_STRING 157 / VAR 85 / LIT_INTEGER 29 first-blockers are subject-position scalars): register the subject producer chain with IR_MATCH_BEGIN as the reading consumer, MIRRORING the IR_ASSIGN loop above member for member -- same leaves, same tree scan, same gamma-adjacency fence, same d-simulation, same all-or-nothing capacity discipline.  Two-env gate = the fc_call_ok precedent: gate-off registration is ZERO, so the default path is byte-identical.  The head POPS the subject DESCR from TOS at the very top of alpha (bb_match_begin subjc arm) BEFORE its own 32B window carve, so fc_leaf_walk's D=32+prefix math for every downstream pattern box is untouched.  fc_call arms cannot appear in subject trees: fc_vtree_scan's call arm requires g_fcc_gfence, which the first zls_build loop zeroes for every IR_MATCH_BEGIN-bearing graph. */
      if (subj_on) for (int vi = 0; vi < g->n; vi++) { IR_t * h = g->all[vi]; if (!(h && h->op == IR_MATCH_BEGIN && h->n_operands > 0 && h->operands[0])) continue;
        { static int dyn_on = -1; if (dyn_on < 0) { const char * b = getenv("SCRIP_SUBJ_DYN"); dyn_on = (b && *b == '0') ? 0 : 1; }
          if (!dyn_on) { int dyn = 0; for (int di = 0; di < g->n && !dyn; di++) { const IR_t * d = g->all[di]; if (d && (d->op == IR_MATCH_DEFER || d->op == IR_MATCH_FENCE1)) dyn = 1; } if (dyn) continue; } }   /* SUBJECT-CELL rung (b) DECLINE RETIRED (s22z, killswitch SCRIP_SUBJ_DYN=0 restores it for A/B): the s22y decline held DEFER/PATREF/FENCE1 graphs flat-verbatim because arming them exposed the PAT$ blob's dead protocol -- CARVE-KILL had deleted the jmp-entry seed and left the blob's γ/ω on CLASS O exit-glue, so the s22y casualties (117/142 silent-exit, 061/107 m3 spin) were BLOB defects wearing subject-arming's clothes, and every "passing" declined fence program was passing VACUOUSLY (fail-expected refs indistinguishable from garbage-subject failure -- 061's own ref is the f-branch).  With BLOB-GRANT + CLASS D landed (emit.cpp s22z) the blob self-allocates, adopts wires, suspends/unwinds correctly, so the decline's protective function is spent: dyn graphs now take the same producer-grant + head-TOS-pop delivery as everything else. */
        IR_t * r = h->operands[0];
        { int nc = 0; for (int ci = 0; ci < g->n && nc < 2; ci++) { const IR_t * c = g->all[ci]; if (!c || c == h) continue; for (int oi = 0; oi < c->n_operands; oi++) if (c->operands[oi] == r) { nc++; break; } } if (nc) { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) fprintf(stderr, "[FCS] decline SOLE-CONSUMER head=%p subj=%p extra_consumers>=%d\n", (void*)h, (void*)r, nc); continue; } }   /* SOLE-CONSUMER FENCE (v1, the 062/063/cross/wordcount casualty class measured this session): the head POPS the cell, so any OTHER operand-holder of subjval -- the replacement SPLICE above all (sno_lower_match pushes subjval into it) -- still reads the FLAT slot the registered producer no longer writes.  Pure dataflow, no kind naming: registration requires the head to be subjval's ONLY operand consumer; replacement statements decline wholesale (their subject is an lvalue with post-match readers -- the rung's named follow-up), everything they read stays flat-verbatim.  Degrade never die. */
        { static int dbg = -1; if (dbg < 0) { const char * e = getenv("SCRIP_FCC_DEBUG"); dbg = (e && *e == '1') ? 1 : 0; } if (dbg) fprintf(stderr, "[FCS] head=%p subj=%p subj_op=%d gamma=%p gamma_is_head=%d sval=%s\n", (void*)h, (void*)r, (int)r->op, (void*)r->γ.node, r->γ.node == h ? 1 : 0, IR_LIT(r).sval ? IR_LIT(r).sval : "(null)"); }
        /* PB-1s SNAPSHOT (Lon 5-stage 2026-08-07): the pre-chain inserts VAR(live)→ASSIGN(PATV$) between the subject producer and MATCH_BEGIN.  Chase through them so fc_subj_register fires and op_subj_cell=1 (TOS-pop delivery) is used.  Without this: op_subj_cell=0 → MATCH_BEGIN reads FRQ(op_sa)=[new____+off] but the subject wrote [old____+off] before push ___ established the frame → Σ='' → every snapshot match fails. */
        { int _pb1s_adj = 0; { IR_t *_c = r->γ.node; int _hops = 0;   /* PB-1s N-REF (X07/V8 root cause, 2026-08-07): the chase handled exactly ONE VAR→ASSIGN(PATV$) pair, so any pattern with ≥2 plain refs declined the grant and fell to the pinned FRQ(op_sa) read against a flat rsp-relative producer store (off by the prologue 8 — rt_match_enter(lo=subject POINTER, hi=0), the shim-measured garbage-subject class).  Walk ANY number of pairs: the pre-chain between subject and head is strictly alternating VAR (arg fetch) → ASSIGN(PATV$k) by construction (drain at lower_snobol4.c OPERAND-EDGE HOIST), one pair per plain ref, npre ≤ 64. */
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
                for (int i = 0; i + 1 < pn; i++) if (post[i]->γ.node != post[i + 1]) { ok = 0; break; }   /* strict gamma chain -- no sr0-transparency arm needed: calls cannot register here (gfence 0 in match graphs), so the transparent shape never occurs */
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
      for (int i = 0; i < g->n && !dyn; i++) if (g->all[i] && (g->all[i]->op == IR_GOTO_DEFERRED || g->all[i]->op == IR_INDIRECT_GOTO || ((g->all[i]->op == IR_CALL || g->all[i]->op == IR_CALL_BUILTIN || g->all[i]->op == IR_CALL_BUILTIN_SNOBOL4) && IR_LIT(g->all[i]).sval && strcmp(IR_LIT(g->all[i]).sval, "CODE") == 0))) dyn = 1;
      if (dyn) { for (int i = 0; i < g->n; i++) if (g->all[i]) rb[i] = 1; wn = 0; }   /* SN4-FRAME-DIET (s174): DYNAMIC-TRANSFER graphs (indirect goto :($X), :<C>, CODE) can enter ANY label chain at runtime -- the optimizer's GOTO-fold leaves those chain heads with no static in-edge (measured: 214 reach=17/57), and m3 emits chains lazily from the label registry on first entry, so the walk under-marks exactly the enterable set.  Full grants for this class (the pre-diet layout); the diet applies to the static majority */
      for (int i = 0; i < g->n; i++) if (g->all[i] && !rb[i] && zls_is_wiring(g->all[i]->op)) { rb[i] = 1; wl[wn++] = i; }   /* SN4-FRAME-DIET (s174) ROOT REPAIR: the emitter is CHAIN-BFS -- every statement anchor (IR_GOTO) and deferred/indirect transfer is an emission root, so label-reachable chains with no static γ/ω in-edge (indirect goto :($X), CODE label transfers -- crosscheck 214/215/216/1020/1021, the drive_value_slot FATAL) must be granted too.  Wiring ops take no ticks themselves; rooting them marks their subtrees.  The registry-only NRETURN landing has no wiring in-edge and is not itself wiring, so it stays ungranted */
      for (int mi2 = 0; mi2 < nl; mi2++) { int sp0 = mstart[mi2]; int sp1 = (mi2 + 1 < nl) ? mstart[mi2 + 1] : g->n; for (int i = sp0; i >= 0 && i < sp1 && i < g->n; i++) if (g->all[i] && !rb[i]) { rb[i] = 1; wl[wn++] = i; } }   /* SN4-FLAT-PROC (s176) LABEL-SPAN ROOTS: every LABELED statement is runtime-enterable (DEFINE stubs and rt_goto_transfer resolve entries through the label registry at CALL time), and the optimizer's GOTO-fold deletes anchors with no static in-edge -- exactly the flat function bodies -- so the wiring repair above finds nothing to root and the body span goes ungranted (the m2 drive_value_slot FATAL, IR_COERCE_STRING).  The group marks the lowerer records per label (zls_group_mark at sno_build_graph) ARE the span watermarks: root every node of every labeled span.  Statically-referenced labels keep surviving anchors, so their spans were already reached and this adds nothing; the diet loss is confined to labeled statements that are dead even at runtime */
      while (wn > 0) { IR_t * c = g->all[wl[--wn]]; if (!c) continue; for (int j = -2; j < c->n_operands; j++) { IR_t * p = (j == -2) ? c->γ.node : (j == -1) ? c->ω.node : c->operands[j]; if (!p) continue; unsigned long h = (((unsigned long)(uintptr_t)p) >> 4) & (unsigned long)(hn - 1); while (hk[h] && hk[h] != p) h = (h + 1) & (unsigned long)(hn - 1); if (hk[h] && !rb[hv[h]]) { rb[hv[h]] = 1; wl[wn++] = hv[h]; } } }
      free(hk); free(hv); free(wl); }   /* SN4-FRAME-DIET (s174): ZLS grants only REACHABLE nodes -- entry-rooted walk over γ/ω/operands.  Unreachable clusters (e.g. the synthetic NRETURN landing, lower_snobol4.c:1816) held 5 ticks = 80B in EVERY SNOBOL4 graph while the emitter BFS never emits them, so no emitted instruction can reference a skipped slot; operands are walked so a reachable consumer's producer is always granted even off the chain spine */
    int s0 = (g->nparams > 0 || g->resumable_callable) ? 1 : 0;
    for (int i = 0; !s0 && i < g->n; i++) if (g->all[i] && rb[i] && (g->all[i]->op == IR_RETURN || g->all[i]->op == IR_SUSPEND)) s0 = 1;
    int base = s0 ? 16 + (g->nparams > 0 ? g->nparams * 16 : 0) : 0;   /* SN4-FRAME-DIET (s174): slot0 [0,16) is the result/FAILDESCR cell of the gen/resumable/RETURN protocols (xa_flat gen γ reads [___+0..15]; rt.c lex epilogue reads [fb+0]) and the params anchor (+16 binding is baked into xa_flat/rt_frame_bind) -- graphs with none of those carried a 16B cell nothing reads, so they get base 0 */
    int k = 0;
    r->first_vslot = zv_n;
    for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) {
        if (zv_n >= ZLS_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", ZLS_MAX_VSLOTS); abort(); }
        zv[zv_n++] = (zls_vslot_t){ g->pnames[i], 16 + i * 16 }; r->n_vslots++;
        zls_field(root, 16 + i * 16, 16, ZK_DESCR, 0, "param", (const IR_t *)0);
    }
    int cur = 0;
    { static int eon = -1; if (eon < 0) { const char * e = getenv("SCRIP_SLOT_ELIDE"); eon = (e && *e == '0') ? 0 : 1; { const char * sf = getenv("SCRIP_STMT_FRAME"); const char * xo = getenv("SCRIP_STF_ELIDE_OFF"); if (sf && *sf == '1' && xo && *xo == '1') eon = 0; } }   /* s21x-r ZLEAK-1 -- THE STF ELIDE-OFF WAS A PROCESS-SCOPE FLAG DRIVING A GRAPH-SCOPE REGIME, AND IT WAS THE ENTIRE MEASURED DAMAGE OF SCRIP_STMT_FRAME=1.  zls_build runs PER GRAPH (g) but `eon` is a static computed once from the env, so s21x-e's forced elide-off applied to ALL 316 programs while the emit-time verdict (emit.cpp:2536 g_emit.flat_stmt_frame) armed only 31 -- the 285 DECLINED graphs got zls_grant instead of zls_grant_elide plus a skipped zls_mark_value_refs, i.e. plan-side damage with no emit-side regime to justify it (ALL-OR-NOTHING PER GRAPH violated from the planner side; the s21x-o "two authorities that could not see each other" class, one level out: the ELIDE authority is process-scoped, the REGIME authority is graph-scoped).  MEASURED s21x-r: armed set and m4 regression set are DISJOINT (31 vs 41, intersection 0, all 31 armed PASS) -> the defect was never in the armed path the s21x-q cursor sent the next session to bisect.  Restoring elide under the regime returns m4 to 264/50/2 with the fail set IDENTICAL to baseline and DIV back to 3, while the regime STILL fires on all 31 (023_arith_add still differs 43 lines) -- so the s21x-e rationale (hole-free accumulated-offset traversal) is NOT load-bearing at HEAD and the scaffold is retired to a killswitch.  SCRIP_STF_ELIDE_OFF=1 restores it for anyone reproducing s21x-e. */
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
    r->locals_off = base + k * 16;   /* ALIGN-INV-3c: the boundary where named locals begin (== the old mark position) -- recorded unconditionally so the seed suffix survives the mark reservation going away */
    if (rt_zc_frame_live() != ZC_FRAME_RSP) {
        r->zeta_mark_off = base + k * 16;
        zls_field(root, r->zeta_mark_off, 8, ZK_RAW, 0, "graph-scope zeta mark (rt_zls_mark snapshot, prologue-stashed, epilogue-released)", (const IR_t *)0);
        zls_field(root, r->zeta_mark_off + 8, 8, ZK_RAW, 0, "zeta_mark.pad (unused)", (const IR_t *)0);
        k += 1;
    } else r->zeta_mark_off = -1;   /* ALIGN-INV-3c: under RSP both halves are dead (mark_call/release_to return empty; anchor writer+readers removed) -- slot unreserved, region reclaimed */
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
    /* SN4-NARY-ARBNO COLLECTION geometry: the one node brackets its body subgraph with operands[1]=first
     * (the resume node, first-allocated) and operands[2]=last body node BY ALLOCATION (the optimizer never
     * reorders/compacts g->all, so the index window is exact and its grants are CONTIGUOUS — offsets are
     * handed out in node order).  Element layout = 16B header {prev_view, saved_delta} + the window's slot
     * range; the emitter repoints the view to elem+16-min_off so body boxes' frame-relative slots land
     * per-iteration (ARCH-ZETA-LOCAL-STORAGE.md section 5f). */
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
            if (g->all[j]->op == IR_MATCH_ASSIGN_SAVE) { int co = -1; for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].kind == ZK_PTR_GC) { co = zf[f].off; break; } if (co >= 0) { if (anzq < 8) azq[anzq++] = co; else anzq = 9; } else anzq = 9; }   /* s141 ARBNO-NOFILL: the implicit-zero citizen is the cap BUF QUAD — the node's registered ZK_PTR_GC field (grant base + 16, zls_locals_shifted) — NOT the grant base; gen/sp self-heal via the g_cap_gen stamp (pattern_match.c:744).  No-field or >8-capture → nzq=9 sentinel → eager blanket. */
            for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].off + zf[f].size > mx) mx = zf[f].off + zf[f].size;
        }
        if (za_n >= (int)(sizeof za / sizeof *za)) { fprintf(stderr, "zls: arbno2 geometry table overflow (%d)\n", (int)(sizeof za / sizeof *za)); abort(); }
        if (mn == 0x7fffffff) za[za_n++] = (zls_ageom_t){ nd, 16, 0, {0}, 0 };
        else                  { zls_ageom_t a; a.nd = nd; a.min_off = mn; a.span = mx - mn; a.nzq = anzq > 8 ? 9 : anzq; for (int q = 0; q < (anzq > 8 ? 0 : anzq); q++) a.zq[q] = azq[q]; za[za_n++] = a; }
    }
    if (rb != rb_s) free(rb);
    /* R12-EXIT-1 CARRY-THE-TAIL finalize (see the fc_tail_* block below): LOWER registered structural candidates; only HERE do zls offsets exist, so windows, footprints, op_sb, and every range leaf's
     * fcl displacement land in this pass.  Left leaves take the flat formula (32 + prefix + own, the fc_leaf_walk math verbatim); body leaves rebase into the element ([rsp + off + d] = elem + off - bmn
     * with rsp = elem - prefix - own, so d = prefix + own - bmn, routinely NEGATIVE -- the fcl relax above); right leaves add fp_body + span (their window sits after the body window and rsp is deeper by
     * the suspended body cells).  The ARBNO node itself registers nothing (its tail arm speaks raw [rsp+const], no FR).
     * PS-3 s153 TWO-PASS: zls_build runs at drive_slots_all time, BEFORE any proc emits -- the emit_patzeta registry is EMPTY then, so a DEFER-bearing candidate cannot price its SUSP leaves yet.
     * The early pass (late=0) finalizes defer-free candidates exactly as ever and leaves defer-bearing ones PENDING (fin=0, pointers intact); emit_chain calls the late pass (late=1) at each graph's
     * emission entry, where every earlier-emitted PAT$ is registered, and the pending candidates resolve (price at SUSP) or decline wholesale (chain arm; head/arbno nulled + SEQ unregistered). */
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
          if (hd && !late) continue; }   /* pending until the registry is fed */
        int b0 = fct[c].b0, b1 = fct[c].b1, i0 = fct[c].i0, ia = fct[c].ia, r1 = fct[c].r1; long k1 = 0;
        int bmn = 0x7fffffff, bmx = 0, rmn = 0x7fffffff, rmx = 0;
        for (int j = b0; j <= b1 && j < g->n; j++) { const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0; if (!e) continue; int _bfmx = 0; for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j]) { if (zf[f].off + zf[f].size > _bfmx) _bfmx = zf[f].off + zf[f].size; if (zf[f].off < bmn) bmn = zf[f].off; } if (_bfmx > bmx) bmx = _bfmx; }   /* ⭐ N02-FIX: gate bmn on pfield presence (same criterion as bmx) -- a node with a zls_entry but zero pfields (K=0 ZLS-granted scanner like RPOS) has e->off=0 which pollutes bmn with a spurious 0, blowing span to rmx-0=full-frame-size.  Use the field-level min/max consistently. */
        for (int j = b1 + 1; j < r1 && j < g->n; j++) { const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0; if (!e) continue; int _rfmx = 0; for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j]) { if (zf[f].off + zf[f].size > _rfmx) _rfmx = zf[f].off + zf[f].size; if (zf[f].off < rmn) rmn = zf[f].off; } if (_rfmx > rmx) rmx = _rfmx; }   /* ⭐ N02-FIX: same gate for rspan -- RPOS(0) in right-spine has e->off=0 but zero pfields; the old `if (e->off < rmn) rmn=e->off` fired unconditionally, setting rmn=0 and inflating rspan to rmx-0=240 instead of the correct 16 (one right-spine node with actual fields). */
        int span = (bmn == 0x7fffffff) ? 0 : bmx - bmn;
        int rspan = (rmn == 0x7fffffff) ? 0 : rmx - rmn;
        /* PS-3 s153 DEFER-AS-KNOWN-FOOTPRINT-LEAF pre-scan: fc_tail_walk admitted seal==2 prologue-bound defers STRUCTURALLY at lower; only HERE is the emit_patzeta registry complete (driver loop
         * registers each PAT$ before the next proc emits, all before main), so the SUSP license is confirmable only now.  EVERY defer in the three ranges must resolve to a registered UNIFORM target
         * (fct_defer_susp > 0) or the WHOLE candidate declines to the chain arm -- head+arbno NULLED so fc_tail_head (fin-blind, consulted by IR_MATCH_BEGIN before this ARBNO emits) and every other
         * fct consumer miss it coherently: a half-tail statement (head self-pushing, arbno chaining) is the mismatch this wholesale null exists to make impossible. */
        { int _dok = 1, _dfr = 0;
          for (int j = i0; j < r1 && j < g->n && _dok; j++) { IR_t * x = g->all[j]; if (x && (x->op == IR_MATCH_DEFER)) { _dfr = 1; if (fct_defer_susp(x) <= 0) _dok = 0; } }
          fct[c].dfr = _dfr;
          if (!_dok) { if (getenv("SCRIP_TAIL_DIAG")) fprintf(stderr, "[TAIL-DIAG] finalize decline: defer target unregistered/non-uniform\n");
                       fct[c].head = 0; fct[c].arbno = 0; continue; } }   /* SEQ-ERAD SE-5/SE-6: fc_seq_unregister loop deleted, no IR_MATCH_SEQUENCE nodes exist */
        /* L1 ALT-in-body lift: footprints + leaf displacements via the ALT-aware helpers (fct_fp_range/fct_leaf_range above) -- a granted ALT enters each range as 16+fpmax with its arm extent skipped,
         * its arm leaves registering per-arm at pfx+16 with the same region bias.  ALT-free ranges reduce to the exact prior linear walks (same fc_geom sums, same formulas: left = 32+prefix+own flat,
         * body = prefix+own-bmn, right = fpb+prefix+own+span-rmn). */
        fct_pricing = 1;
        int fpl = fct_rsp_range(g, i0, ia);   /* M-2 BUG-5: fpl = actual rsp-push depth (zd_k sum), not fc_geom sum -- flat-allocated left-spine nodes (LIT_INTEGER etc.) push rsp but have fc_geom=0 */
        int fpb = fct_fp_range(g, b0, b1 + 1);
        int fpr = fct_fp_range(g, b1 + 1, r1);
        int fpr_rsp = fct_rsp_range(g, b1 + 1, r1);   /* ⭐ N02-FIX: actual rsp-push depth of right-spine nodes -- the same BUG-5 lesson applied to the right-spine: LIT_INTEGER (arg to RPOS/POS) pushes K=16 rsp cells that fct_fp_range/fc_geom does not see; when right-spine nodes fail, their cells stay and beta arrives displaced; beta must pop fpr_rsp before reading element slots. */
        (void)k1;
        fct_pricing = 0;
        fct[c].fpl = fpl; fct[c].fpb = fpb; fct[c].fpr = fpr; fct[c].fpr_rsp = fpr_rsp; fct[c].span = span; fct[c].rspan = rspan; fct[c].opsb = (span + rspan + 32 + 16 * fct[c].nw + 15) & ~15; fct[c].fin = 1;
        for (int w = 0; w < fct[c].nw; w++) fc_cond_register(fct[c].wcd[w], fpb + span + rspan + 32 + 16 * w);   /* WRAP-CAPTURE: COND reads its element slot at the uniform yield depth (elem - fpb) */
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
}   /* s141 ARBNO-NOFILL sibling accessor: body ASSIGN_SAVE ζ offsets (implicit-zero citizens); returns 9 on >8-capture overflow — caller must keep the eager blanket fill */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* zls2_geom — the ZLS2 per-activation frame-protocol geometry authority (Lon directive, 2026-07-08 session:
 * "templates instrumented through x86's α/β/γ/ω with code injection of stack frame bump, restore on
 * backtrack, and release on exit").  This file already OWNS the save-slot layout (the zls_field grants above
 * name "arb.zls2" at +8 and "arbno.zls2" at +16), so the per-node protocol geometry lives here too — ONE
 * authority, queried at the emit dispatch point (the zls_arbno_geom precedent) and promoted into g_emit for
 * the central port hook in x86_asm.h.  Inputs: nd = the node; base_off = the node's ALREADY-RESOLVED frame
 * base (x86_scratch_off for ARB, op_off for ARBNO — owner resolution via operand[0] for phases 1/2 happens
 * at dispatch, exactly as the templates read it today).  Outputs: *slot_off = absolute frame offset of the
 * activation-block save slot; *k = ZLS2 block size (the former template-private ARB_ZLS2_K/ARBNO_ZLS2_K
 * constants, relocated here — block layouts documented at the grants above and in the templates).  Returns
 * the ZLS2_* ops bitmask (zeta_choices.h): which injections the port hook fires for this node.  The grant IS
 * the ω-death classifier: ZLS2_RELEASE is awarded ONLY to roles whose jmp-ω sites are STATICALLY all
 * activation-death (construct-role knowledge — never the op_omega_is_death chain-window classifier, recorded
 * broken in bb_match_arbno.cpp's L(9) note).  ⛔ RULING (Lon, 2026-07-08 s7): ONLY α AND ω PARTICIPATE IN
 * MEMORY ALLOCATION.  Under pure LIFO the ω that lands on a β has already put the cursor right (its own
 * RELEASE fired at death), so a β restore is always a no-op — exactly what the ALLOC-flavor traces observed
 * ("silent no-op restores under clean LIFO").  ZLS2_RESTORE is therefore never granted; frames of roles whose
 * own ω is an alias (ARBNO ph0) die at ph2's release / the statement backstop.  Per-role mapping:
 *   IR_MATCH_ARB        BUMP|RELEASE          α pushes; its single exhaust ω is always this activation's
 *                                             death → release.
 *   IR_MATCH_ARBNO ph0  BUMP                  α pushes.  Its β-arm jmp ω is the BODY-ENTRY ALIAS → never
 *                                             release here; ph2's release (same slot, chained) frees it.
 *   IR_MATCH_ARBNO ph1  0                     forward-flow landing; its jmp ω aliases into ph2 → no ops.
 *   IR_MATCH_ARBNO ph2  RELEASE               its single jmp ω is the construct's outer-fail true death →
 *                                             unchain + release ph0's frame (and stray body frames below).
 *   ARBNO v2 (ph3/4/5)  0                     rt_zcol COLLECTION machinery, not ZLS2 — untouched.
 * Returns 0 (no participation) for every other node kind — the hook stays inert exactly where it is today. */
int zls2_geom(const IR_t * nd, int base_off, int * slot_off, long * k) {
    if (!nd || base_off < 0) return 0;
    /* IR_MATCH_ARB moved to the fc_geom fixed-cell path (RUNG ZB-FC-4, Lon s50 ALL-STACK ruling ARCH-ZETA S14): ARB is a fixed 8-byte generator (counter+saved-cursor), never a linked activation, so it takes the clean sub-rsp/add-rsp fixed cell like SPAN/BAL, NOT the zls2 linked-block BUMP|RELEASE grant.  Its zls2 grant is DELETED here; fc_geom returns k=16 for it. */
    if (nd->op == IR_MATCH_ARBNO) {
        long ph = IR_LIT(nd).ival;
        int ops = ph == 0 ? ZLS2_BUMP : ph == 2 ? ZLS2_RELEASE : 0;
        if (!ops) return 0;   /* ph1 / v2: outputs UNWRITTEN — bytes>0 with ops==0 is the DORMANT direct-sub arm's key (sub ZR,K!) and must never be reachable from this path */
        if (slot_off) *slot_off = base_off + 16;
        if (k) *k = 32;
        return ops;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_geom -- the ZC_PORT_FORTH per-box FIXED-CELL geometry authority (RUNG ZB-FC-0, Lon 2026-07-12; design
 * ARCH-ZETA S10 + zeta_choices.h ZC_PORT_FORTH).  Same authority pattern as zls2_geom above: queried at the
 * emit dispatch point, promoted into g_emit (op_fc_bytes/op_fc_base), consumed by the central port hook and
 * the FR/FRQ in-range translation in x86_asm.h.  Returns 1 and *k = the box's fixed cell size (16-multiple
 * BY LAW, S10a item 6) iff the kind participates; 0 = inert, the hook and translation stay dormant exactly
 * where they are today.  v1 kind set: IR_MATCH_SPAN only (keystone looping box -- two private 4-byte fields,
 * scratch+0 counter and scratch+4 beta-undo delta, one 16-byte cell; no zls2 grant so no arm collision; all
 * conditional omegas route through the x86_jcc synth).  Widening is per-kind with the S10c port-invariant
 * check (own rsp motion must not straddle an FR access; SPAN's +-8 strchr dance does not).  WIDENED (rung
 * ZB-FC-1, 2026-07-12): TAB/RTAB each own ONE 4-byte local (entry-delta save at scratch+0, written at alpha
 * before any read, read at beta) with ZERO internal rsp motion and conditional omegas already synth-routed --
 * one 16-byte cell each; the window is the SHIFTED locals base (zls_off = result+16), so the result slot and
 * operand slots stay flat-frame outside it.  MEASURED VERDICT, NOT A REFUSAL -- LEN/ANY/NOTANY own NO zeta
 * locals (state is pure register arithmetic; beta-undo re-reads the OPERAND slot or immediate): under the
 * S13 fixed-size law a box with no locals has a ZERO cell, i.e. no rsp motion, so returning 0 IS their
 * grant; they join when SU-C gives them a result field to home (the S13 candidate list assumed all five own
 * scratch -- the per-kind check says three do not).  BREAK/BREAKX (same widening): BREAK one 4-byte counter,
 * BREAKX counter+entry-delta-save (generator-kind; the cell suspends across gamma/beta cycling per S10c and
 * LIFO keeps every beta entry at the frontier); both strchr dances verified non-straddling like SPAN's.
 * POS/RPOS stay ungranted: the canonical PURE boxes, zero scratch.  ARB/ARBNO carry zls2 grants -- excluded
 * from the v1 fixed-cell set by design (the heap-flavor gamma/omega overloads are ZB-ACT-3's entry). */
int fc_alt_fpmax(const IR_t * nd);
int fc_save_active(const IR_t * nd);
int fc_vlit_active(const IR_t * nd);
int fc_arm_member(const IR_t * nd);
int zc_nofc(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_NOFC"); v = (e && e[0] == '0') ? 0 : 1; } return v; }   /* ⭐⭐⭐ NOFC-DEFAULT-ON (s22r, Lon directive "climb the ladder to NON-POPPING FORTH-style RSP ZETA stack").  KILLSWITCH IS NOW SCRIP_NOFC=0.  ⛔ THE s22l AND s22l-B NUMBERS THAT DEFERRED THIS FLIP ARE BOTH STALE AND THE REASON IS s22n: they were measured while xa_flat_prologue still carved a whole-graph frame, so suppressing the ZW-1 universal carve took the match family OFF its self-allocated zls extent and back ONTO a frame that still existed -- worth +32/+33 programs and costing 2.  At HEAD that frame is DELETED, so neither half of that trade survives its premise.  RE-MEASURED at HEAD, crosscheck 317, one binary, setarch -R: default m3 199/118 m4 186/130/1 DIV 13 -> NOFC m3 204/113 m4 188/128/1 DIV 16, FIXING 5 in m3 (063/064/065_pat_fence_fn_*, 116_pat_arbno_of_fence_inline, 156_pat_cap_alt_abandon_pop) and 2 in m4, and BREAKING ZERO IN EITHER MODE by set diff.  The 2-program regression set s22l-B named (143_pat_regex_quantified_class, 164_pat_arbno_nested) is GONE -- both fail in the default regime too, so they are no longer NOFC's debt.  ⭐⭐ AND THE THIRD REGIME IS THE FINDING: SCRIP_NOFC=1 + SCRIP_NOFC_CARVE=1 is IDENTICAL BY SET to default in BOTH modes, which proves the VALUE-SPINE half of this switch (the fc_geom vlit grant suppression below) is a VERIFIED NO-OP at HEAD -- the value spine is already fully ZD-armed, so there are no unarmed value nodes left for it to suppress.  100% of NOFC's delta is the ZW-1 carve suppression at emit.cpp:820.  That RETIRES s22l-B's stated live question ("which node kinds' carve helps and which hurts") for the whole-graph carve: at HEAD it helps none and hurts five, because the fallback it used to buy no longer exists. */   /* ⭐⭐ NOFC-SYM: THE ONE AUTHORITY for the FC killswitch, and the reason it now lives here instead of in three templates.  MEASURED (s22l): SCRIP_NOFC was ASYMMETRIC -- it flipped only the CONSUMER's read (bb_assign_global's vfc / bb_binop_arith's vfcb / bb_binop_concat_slot's vfcc, each carrying its OWN copy of the getenv, the "spelled three times" shape the s22k ZD-K law was written about) while the PRODUCER kept its fc_geom grant and kept carving its cell.  A DECLINED statement therefore had its producer write [rsp+0] and its consumer read the flat slot -- nobody's write -- which is why func_call printed `result: 0` and why the loop-resident programs leaked 16B per statement per iteration until the stack died.  THE FIX IS SYMMETRY, NOT A NEW MECHANISM: under NOFC a node that is not ZD-armed takes NO grant and NO carve, so it is fully flat, exactly the pre-zeta world the carve corpse still serves; a node that IS ZD-armed is untouched here because the ZD staging sets op_fc_bytes/op_zdepth itself at the emit choke and the dispatch's fc_geom arms are all guarded by !op_zres.  The two regimes then partition every node with no third state, which is what makes SCRIP_NOFC=1 an HONEST INSTRUMENT for "the FC arm and its five pops are deleted" rather than a self-inflicted break. */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_geom(const IR_t * nd, long * k) {
    if (!nd || !fc_cells_on()) return 0;   /* Z4-6: one port opinion (fc_cells_on above) */
    if (fc_arm_member(nd)) return 0;   /* ALT-FLAT (s202): arm residents are flat -- zero cell, zero rsp motion; their zls quads are kept (zls_fc_cell twin guard) */
    if (nd->op == IR_MATCH_ASSIGN_SAVE && fc_save_active(nd)) { if (k) *k = 16; return 1; }   /* ZB-FC-3c: delta at cell+0; ungranted SAVE stays zero-cell = the flat rt_cap array path */
    if ((nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_VAR) && fc_vlit_active(nd) && (!zc_nofc() || fc_subj_member(nd))) { if (k) *k = 16; return 1; }   /* s22y: fc_subj_member exemption -- the subject producer's cell IS the delivery mechanism (head pops it at alpha top), not a legacy alternative the spine replaces; suppressing it while fc_vread_fp arms the head was the s22s producer/consumer split */   /* NOFC-SYM narrowing (s22l, MEASURED both ways): the killswitch belongs on THIS LINE ONLY, not at the head of fc_geom.  A blanket `if (zc_nofc()) return 0` fixed the entire DEFINE/EVAL break set (func_call, func_call_overhead, indirect_dispatch, eval_fixed all went green -- the very programs the ZD-7 rung was scoped to fix by porting a 590-line template) and then broke FIVE pattern programs that had been passing: mixed_workload, pattern_bt, pattern_bt_deep, roman, string_pattern.  THE REASON IS A CATEGORY ERROR THE NAME "FC" INVITES: the ZB-FC-3c/ZB-FC-4 cells below (MATCH_ARB/SPAN/TAB/RTAB/BREAK/BREAKX/BAL/REM, the SCAN_* Icon twins, the granted ASSIGN_SAVE) are the match family's REAL AND ONLY STORAGE -- their templates are written against the cell, it is not a legacy alternative to anything -- whereas THIS line is the ZB-VAL-0/2/3 value-spine grant, the producer half of the popping vfc/vfcb/vfcc read that the non-popping zeta spine is meant to REPLACE.  Only the latter is the "FC arm" the ladder is retiring, so only the latter answers to the killswitch. */   /* ZB-VAL-0/2/3 (s177/s178): registered statement-level value producer (scalar lit or global var read) feeding a plain assign; ungranted producers stay flat */
    if (nd->op == IR_MATCH_ARB)    { if (k) *k = 16; return 1; }   /* ZB-FC-4 (Lon s50 S14): the 8-byte counter+saved-cursor cell, ex-zls2, now a clean fixed FORTH cell */
    if (nd->op == IR_MATCH_SPAN)   { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_TAB)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_RTAB)   { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAK)  { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAKX) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BAL)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_REM)    { if (k) *k = 16; return 1; }
    /* IR_MATCH_ALTERNATE: NO cell (ALT-FLAT s202).  The ALT's own state (delta + resume/next continuation ptrs) lives in its flat zls quads; arms are flat (fc_arm_member); the box moves rsp nowhere. */
    if (nd->op == IR_SCAN_TAB)     { if (k) *k = 16; return 1; }   /* ZB-ICN-FC-1 (Icon, first FORTH cell): tab's single saved-cursor scratch at op_off+16 (fscan.r: oldpos saved at alpha, restored at beta then fail); result DESCR stays flat at op_off+0, read cross-box */
    if (nd->op == IR_SCAN_MOVE)    { if (k) *k = 16; return 1; }   /* ZB-ICN-FC-2 (Icon): move is tab's twin — same saved-cursor scratch at op_off+16 (fscan.r move: oldpos saved at alpha, restored at beta then fail), identical layout/shape */
    if (nd->op == IR_SCAN_MATCH)   { if (k) *k = 16; return 1; }   /* ZB-ICN-FC-3 (Icon): match is function{0,1} single-yield (fstranl.r) — needle {ptr,len} scratch at op_off+16/+24 is TRANSIENT within alpha->gamma (written by rt_scan_needle, read by the memcmp, never read at beta); result stays flat at op_off+0. Single-yield = clean alpha-push/omega-pop even inside a scan (unlike the {*} generators) */
    /* NOT-YET (Icon generator scans UPTO/FIND/MATCH/BAL): a function{*} scan cursor is durable across gamma/beta cycling, but when the box runs INSIDE a scan environment (s ? upto(c)) the outer every's re-pump threads control back through IR_GEN_SCAN's enter/leave beta WITHOUT preserving the box's own alpha-push/omega-pop LIFO — a fixed rsp cell double-accounts and corrupts rsp (measured: rung06 scan-subject upto -> empty under FORTH, was 6).  These stay flat-frame until the scan re-pump story is on the spine (the Icon analogue of SNOBOL4 Tier-C footprint). Standalone upto(c,s) re-pumps fine; the hazard is scan-subject only.  (FIND/BAL are the other {*} generators; MATCH is {0,1} and IS granted above.)
       ZERO-CELL BY LAW (Icon ANY/MANY/POS): function{0,1} position tests that own NO box-private RW scratch — any/pos are pure register compares, many's forward-scan loop runs to completion within alpha->gamma using eax only (never re-pumps); all three touch only the result DESCR at op_off+0/+8 (no zls_layout case -> result-only default grant).  Under the fixed-size law a box with no locals has a ZERO cell = no rsp motion, so fc_geom returning 0 IS their FORTH form (the SNOBOL4 LEN/ANY/NOTANY/POS precedent).  Nothing to convert. */
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_alt_* -- RUNG ZB-FC-3a (ARCH-ZETA S13 wholesale map, Tier C first slice): the LINEAR-ARM ALTERNATE
 * registrar.  LOWER computes each arm's EXACT static footprint (16 x granted leaves in the arm's allocation
 * range; the S10d pad-to-max law needs EXACT fp -- any error misaligns ALT.beta's own-cell read) and
 * registers here iff EVERY arm is linear (range contains only granted leaves / zero-cell leaves / SEQ /
 * captures / wiring -- nested ALTERNATE, ARBNO, ARB, DEFER, anything unknown DECLINES: outer stays flat,
 * the pre-rung path, degrade never die).  Side table keyed by node pointer (the zls_entry precedent; PEERS
 * RULE forbids IR_t fields, and operand-appending would corrupt flat_drive's N = n_operands/2).  Node-ptr
 * staleness exposure identical to the zls tables (fresh process per compile; EVAL graphs mint fresh nodes). */
static struct { const IR_t * nd; int n; int fp[16]; int ab[16]; int ae[16]; } fca[256];
static int fca_n = 0;
/* fc_arm_member -- ALT-FLAT (s202, Lon design: "fixed offsets all the way down"): every node INSIDE a granted ALT's arm ranges is registered here at fc_alt_register time, and fc_geom/zls_fc_cell
 * both decline it -- arm residents keep their FLAT zls quads and push NO rsp cell, so every arm's rsp footprint is 0, every arm yields at the ALT's own frontier, and the S10d pad-to-max law is
 * structurally unnecessary (deleted).  ORDER IS LOAD-BEARING: fc_walk_range's per-arm admission walk (lower_snobol4.c ~1504) runs BEFORE registration, so admission fp math is computed on the
 * pre-decline answers; every LATER reader (statement walk, fc_leaf_walk, fct twins, zls_build via drive_slots_all, emit staging) sees the decline.  Same node-ptr keying/staleness envelope as fca. */
static const IR_t * fcm[1024];
static int fcm_n = 0;
void fc_arm_member_register(const IR_t * nd) { if (nd && fcm_n < 1024) fcm[fcm_n++] = nd; }
int fc_arm_member(const IR_t * nd) { for (int i = 0; i < fcm_n; i++) if (fcm[i] == nd) return 1; return 0; }
void fc_alt_register(const IR_t * nd, int n, const int * fp, const int * ab, const int * ae) {
    if (!nd || n <= 0 || n > 10 || fca_n >= 256) return;   /* N>10 exceeds the 3N+2 <= XA_BB_EMIT_PAIR_MAX(32) stub budget -- silent decline */
    fca[fca_n].nd = nd; fca[fca_n].n = n;
    for (int i = 0; i < n && i < 16; i++) { fca[fca_n].fp[i] = fp[i]; fca[fca_n].ab[i] = ab ? ab[i] : -1; fca[fca_n].ae[i] = ae ? ae[i] : -1; }
    fca_n++;
}
int fc_alt_n(const IR_t * nd) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) return fca[i].n;
    return -1;
}
int fc_alt_extent(const IR_t * nd, int * b, int * e) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) { if (fca[i].ab[0] < 0 || fca[i].ae[fca[i].n - 1] < 0) return 0; if (b) *b = fca[i].ab[0]; if (e) *e = fca[i].ae[fca[i].n - 1]; return 1; }
    return 0;
}
int fc_alt_arm_range(const IR_t * nd, int j, int * b, int * e) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) { if (j < 0 || j >= fca[i].n || fca[i].ab[j] < 0) return 0; if (b) *b = fca[i].ab[j]; if (e) *e = fca[i].ae[j]; return 1; }
    return 0;
}
int fc_alt_fpmax(const IR_t * nd) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) { int m = 0; for (int j = 0; j < fca[i].n; j++) if (fca[i].fp[j] > m) m = fca[i].fp[j]; return m; }
    return -1;
}
int fc_alt_fp(const IR_t * nd, int j) {
    for (int i = 0; i < fca_n; i++) if (fca[i].nd == nd) return (j >= 0 && j < fca[i].n) ? fca[i].fp[j] : 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_seq_* -- RUNG ZB-FC-3b (ARCH-ZETA S13 Tier C, SEQUENCE).  DERIVED FROM THE LIVE TREE, AND THE DERIVATION
 * OVERTURNED THE WRITTEN PLAN: S13 prescribed pad-to-max sigma stubs + a per-i un-pad in a shared glue (ALT's
 * shape).  SEQ needs NEITHER, and owns NO CELL.  Let F = SEQ's frontier and S_i = sum of fp(elem 0..i).  At
 * elem i's gamma rsp = F - S_i, which is EXACTLY where elem i+1's alpha expects it (it pushes below); at elem
 * i's omega its whole subtree has popped so rsp = F - S_{i-1}, which is EXACTLY elem i-1's yield frontier =
 * where its beta expects it (S10c port invariant).  Every sigma/phi transition is therefore ALREADY at the
 * correct depth -- zero adjustment -- and every dispatch target is STATICALLY known from the SOURCE element.
 * seq_i (the runtime index the shared glue exists to carry) is therefore DEAD: the LIFO stack position IS the
 * sequence position.  This is the s32 two-flavor ruling landing on its feet ("seq_i and the head link are the
 * same datum in two representations") -- the stack flavor's representation turns out to be NO DATUM AT ALL.
 * The saved entry-delta is dead too (SEQ never restores it; the retreat goes to elem i-1's BETA, which does
 * its own undo -- verified: no cross-box reader of a SEQUENCE slot exists in emitter/templates/runtime).
 * Consequences: zero pairs added (no 4N+2<=32 crunch, so no N limit, unlike ALT's N<=10); the s21 TRANSIT
 * guarantee stays structural (elem i's omega still lands elem i-1's BETA, wired statically instead of
 * dispatched); and a zero-cell SEQ contributes 0 to footprint whether or not it converts, so ALT's pad-to-max
 * arithmetic is untouched either way -- the two conversions are INDEPENDENT.  The ANS-FORTH asymmetry, exact:
 * ALTERNATION needs a runtime flag (ALT keeps alt_i); SEQUENCING does not.  ELIGIBILITY (LOWER-side, the ALT
 * linear whitelist verbatim): every element must respect the S10c port invariant -- granted leaves, zero-cell
 * leaves, nested SEQ, captures, wiring.  Nested ALTERNATE/ARBNO/ARB/DEFER/ABORT/unknown DECLINE the whole SEQ
 * (it stays flat, the pre-rung path -- degrade never die); widen once ARB/DEFER carry clean fixed cells. */
static const IR_t * fcs[512];
static int fcs_n = 0;
void fc_seq_register(const IR_t * nd) { if (!nd || fcs_n >= 512) return; fcs[fcs_n++] = nd; }
void fc_seq_unregister(const IR_t * nd) { for (int i = 0; i < fcs_n; i++) if (fcs[i] == nd) { fcs[i] = fcs[--fcs_n]; return; } }
static const IR_t * fcab[512]; static int fcab_n = 0;
void fc_arbno_member_register(const IR_t * nd) { if (nd && fcab_n < 512) fcab[fcab_n++] = nd; }   /* SEQ-CELL fence (s21x-l): registered at LOWER where sno_in_arbno is live (lower_snobol4.c ~1146) -- an ARBNO-body SEQ's 16B cell interleaves with iteration frames whose extent arithmetic never counted it (the 066/164/165 m4 falsification, same disease as the s202 defer-window depth class), so ARBNO-extent residents stay flat, degrade never die.  Same node-ptr keying/staleness envelope as fc_arm_member. */
int fc_arbno_member(const IR_t * nd) { for (int i = 0; i < fcab_n; i++) if (fcab[i] == nd) return 1; return 0; }   /* PS-3 s153: the tail candidate's pat_entry SEQ converts at lower ASSUMING the tail lands; a finalize-decline (defer target unregistered/non-uniform) must revert it or the SEQ's static re-points assume element depths the chain arm never establishes.  Swap-remove; consulted (fc_seq_active) only at emit, after the layout pass where the decline runs. */
int fc_seq_active(const IR_t * nd) { (void)nd; return 0; }   /* SEQ-ERAD SE-5/SE-6: IR_MATCH_SEQUENCE deleted; always inactive */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_save_* / fc_cond_* -- RUNG ZB-FC-3c (ARCH-ZETA S13 Tier C, CAPTURES; landing plan of record
 * FINDING-2026-07-13-CLAUDE-SN4-ZB-FC-3C-COND-CROSS-BOX-READ.md, which superseded the s44 plan).  TWO
 * registrars because the capture is TWO boxes: ASSIGN_SAVE owns the 16-byte cell (delta at cell+0, the
 * software rt_cap array retired on the granted path), and ASSIGN_COND/IMM -- different IR nodes with NO cell
 * of their own -- read SAVE's delta through a CROSS-BOX rsp displacement.  By the S10c law (cells pop at
 * omega, not gamma) the entire inner subtree is still suspended at COND's alpha, so SAVE's cell sits at
 * [rsp + fp(inner)], a STATIC displacement -- exactly ALTERNATE's own-cell-across-arm-footprint mechanism
 * (bb_match_alternate rspd(op_fc_fpmax+4)), reused not invented.  fp(inner) is LOWER's range sum over the
 * inner allocation range (the ALT arm loop verbatim), exact ONLY for fc-LINEAR spines; eligibility is
 * TWO-DIRECTIONAL (the ZB-FC-3b lesson, in the mirror): an ARBNO/ALT/ARB/DEFER *inside* the inner breaks the
 * static displacement just as a capture *inside* an ARBNO body (sno_in_arbno) breaks the port invariant --
 * either direction DECLINES both registrations and the capture keeps the flat rt_cap array path verbatim
 * (degrade never die).  Side tables keyed by node ptr (the fc_alt precedent; PEERS RULE forbids IR_t fields).
 * COND/IMM stay ZERO-CELL BY LAW -- granting them a cell would silently inflate every enclosing ALT's
 * pad-to-max fpmax (the fc_geom catch at lower_snobol4.c's arm sum picks up granted SAVEs automatically and
 * must pick up nothing else).  IMM is the identical topology at op_phase 2 -- one mechanism, two phases. */
static const IR_t * fvl[256]; static int fvl_n = 0;
void fc_vlit_register(const IR_t * nd) { if (!nd || fvl_n >= 256) return; fvl[fvl_n++] = nd; }
static const IR_t * fvs[64]; static int fvs_n = 0;   /* SUBJECT-CELL rung (b), s22y: membership of the SUBJECT producer class within fvl -- the one fvl population whose consumer (IR_MATCH_BEGIN, ZD's 247-strong first-blocker) the non-popping spine cannot serve, so its grant is exempt from the zc_nofc retirement below */
void fc_subj_register(const IR_t * nd) { if (!nd || fvs_n >= 64) return; fvs[fvs_n++] = nd; }
int fc_subj_member(const IR_t * nd) { if (!nd) return 0; for (int i = 0; i < fvs_n; i++) if (fvs[i] == nd) return 1; return 0; }
int fc_vlit_active(const IR_t * nd) { if (!fc_cells_on()) return 0; if (!nd || !(nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_VAR)) return 0; for (int i = 0; i < fvl_n; i++) if (fvl[i] == nd) return 1; return 0; }
static struct { const IR_t * nd; int fp; } fvr[256]; static int fvr_n = 0;
void fc_vread_register(const IR_t * nd, int fp) { if (!nd || fp < 0 || fvr_n >= 256) return; fvr[fvr_n].nd = nd; fvr[fvr_n].fp = fp; fvr_n++; }   /* ZB-VAL-0: consumer-side displacement, the fc_cond_register shape */
int fc_vread_fp(const IR_t * nd) { for (int i = 0; i < fvr_n; i++) if (fvr[i].nd == nd) return fvr[i].fp; return -1; }
static const IR_t * fvb[256]; static int fvb_n = 0;
static const IR_t * fvcl[64]; static int fvcl_n = 0;   /* CALL2BB 3b: value-spine CALL registrations (node-ptr keyed, same staleness envelope as fvl/fvb) */
void fc_call_register(const IR_t * nd) { if (!nd || fvcl_n >= 64) return; fvcl[fvcl_n++] = nd; }
int fc_call_active(const IR_t * nd) { if (!nd || !fc_cells_on()) return 0; for (int i = 0; i < fvcl_n; i++) if (fvcl[i] == nd) return 1; return 0; }   /* CALL2BB 3b: queried at the staged-call dispatch preamble (op_fc_wbytes window grant) and at the sr0 role-0 dispatch (arg-window grant into FRQB's existing fc arm) */
void fc_vbinop_register(const IR_t * nd) { if (!nd || fvb_n >= 256) return; fvb[fvb_n++] = nd; }   /* ZB-VAL-1/6b: value-spine OPERATOR (binop or unop) -- a binop's two operand cells are the top two and it nets add rsp,16; a unop's ONE operand cell IS its result cell and it nets ZERO (release 16 + carve 16 cancel, so no rsp instruction is emitted at all) */
int fc_vbinop_active(const IR_t * nd) { if (!nd || (nd->op != IR_BINOP && nd->op != IR_UNOP)) return 0; for (int i = 0; i < fvb_n; i++) if (fvb[i] == nd) return 1; return 0; }
static struct { const IR_t * nd; long w; } fvw[512]; static int fvw_n = 0;
void fc_vwpop_register(const IR_t * nd, long w) { if (!nd || w <= 0 || fvw_n >= 512) return; fvw[fvw_n].nd = nd; fvw[fvw_n].w = w; fvw_n++; }   /* ZB-VAL-5: fallible box's EXTRA omega pop (cells under it); own cell rides the fc hook */
long fc_vwpop(const IR_t * nd) { if (!fc_cells_on()) return 0; for (int i = 0; i < fvw_n; i++) if (fvw[i].nd == nd) return fvw[i].w; return 0; }
int fc_vcap(int nl, int nr, int nb, int nw) { return fvl_n + nl <= 256 && fvr_n + nr <= 256 && fvb_n + nb <= 256 && fvw_n + nw <= 512; }   /* ZB-VAL-1/5: registration is all-or-nothing per statement -- a dropped member would strand a carve or under-pop an omega */
static const IR_t * fcv[256];
static int fcv_n = 0;
void fc_save_register(const IR_t * nd) { if (!nd || fcv_n >= 256) return; fcv[fcv_n++] = nd; }
int fc_save_active(const IR_t * nd) {
    if (!nd || nd->op != IR_MATCH_ASSIGN_SAVE || !fc_cells_on()) return 0;   /* Z4-6: one port opinion */
    for (int i = 0; i < fcv_n; i++) if (fcv[i] == nd) return 1;
    return 0;
}
static struct { const IR_t * nd; int e; } fpe[256]; static int fpe_n = 0;   /* FLATDISP-LEAF-ORDER (this session, the rt_dcap_pump segv root): capture pairs allocate [COND, SAVE, inner...] but FLOW save->inner->cond, so fc_leaf_walk's running prefix registered COND BEFORE SAVE's 16 accumulated -- COND then FR-read the SAVE slot 16 short of the true depth (039: [rsp+32] over head's rsp-snapshot cell, low-32 = the 0xffff9b20 gdb saw; SAVE's own write at [rsp+48] was right).  Latent since the walk landed; EXPOSED by s193's HEAD-leaves-deep-list (___-seeded FR was depth-immune, the wrong D was never consulted).  The lowerer records each pair's inner allocation END here at build time; fc_leaf_walk recurses [k+1, E) first and registers COND at the RESULT pfx -- the S10c suspended depth. */
void fc_pair_extent_register(const IR_t * nd, int e) { if (!nd || e <= 0 || fpe_n >= 256) return; for (int i = 0; i < fpe_n; i++) if (fpe[i].nd == nd) return; fpe[fpe_n].nd = nd; fpe[fpe_n].e = e; fpe_n++; }
int fc_pair_extent(const IR_t * nd) { for (int i = 0; i < fpe_n; i++) if (fpe[i].nd == nd) return fpe[i].e; return -1; }
static struct { const IR_t * nd; const IR_t * save; int fp; } fcc[256];
static int fcc_n = 0;
void fc_cond_register(const IR_t * nd, int fp_inner) {
    if (!nd || fp_inner < 0 || fcc_n >= 256) return;
    fcc[fcc_n].nd = nd; fcc[fcc_n].save = NULL; fcc[fcc_n].fp = fp_inner; fcc_n++;
}
void fc_cond_register_with_save(const IR_t * nd, const IR_t * save, int fp_inner) {
    if (!nd || fp_inner < 0 || fcc_n >= 256) return;
    fcc[fcc_n].nd = nd; fcc[fcc_n].save = save; fcc[fcc_n].fp = fp_inner; fcc_n++;
}
int fc_cond_fp(const IR_t * nd) {
    if (!fc_cells_on()) return -1;   /* Z4-6: one port opinion */
    for (int i = 0; i < fcc_n; i++) if (fcc[i].nd == nd) {
        if (fcc[i].save && fc_arm_member(fcc[i].save)) return -1;   /* ALT-FLAT coherence: SAVE inside ALT arm gets no rsp cell (fc_arm_member=1, fc_geom=0); COND must also decline to rt_cap_top path */
        return fcc[i].fp;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_head_* -- RUNG ZB-FC-3d (ARCH-ZETA S13 Tier C, HEAD/RELEASE/REPLACE; ruling of record: the D4 splice-survivor fork resolved by PARTITION, s49).  HEAD is NOT hook-shaped (fc_geom stays 0 BY LAW so
 * no enclosing sum ever counts a self-releasing cell): the template pushes its own 32-byte cell after alpha holding ONLY the match-span-lifetime fields (anchor@+0, zls mark@+8, rsp mark@+16 = the
 * PRE-PUSH rsp, so the existing S10e statement UNWIND releases the cell and every suspended pattern cell in one mov), addressed through the op_fc_wbytes WINDOW (rebase without the hook).  The four
 * fields with POST-UNWIND lifetime stay FLAT on the existing offsets -- dcap mark@+32 / saved ___@+40 (RELEASE's post-unwind pump + HEAD's own omega read them) and the splice pair for REPLACE (end@+24
 * written by RELEASE pre-unwind as today; START copied by RELEASE pre-unwind from the cell into flat +0, which the granted HEAD no longer writes -- REPLACE's reads are UNCHANGED both paths).  RELEASE
 * reads the cell CROSS-BOX at [rsp + fp(pattern) + k] via op_fc_disp (the 3c mechanism verbatim); staticity of fp holds because the v1 fence is ALT-FREE (the linear range sum over-counts alternation --
 * only one padded arm is live at yield -- so statements containing ALTERNATE decline wholesale; the per-ALT 16+fpmax lift is a named follow-on).  Eligibility = the 3c walk verbatim over the PATTERN
 * range only; either-direction failure declines and the whole statement keeps the flat path byte-verbatim (degrade never die).  Side table keyed by the HEAD node ptr (the fcc precedent). */
static struct { const IR_t * nd; int fp; } fch[256];
static int fch_n = 0;
void fc_head_register(const IR_t * nd, int fp) { if (!nd || fp < 0 || fch_n >= 256) return; fch[fch_n].nd = nd; fch[fch_n].fp = fp; fch_n++; }
int fc_head_fp(const IR_t * nd) {
    if (!fc_cells_on()) return -1;   /* Z4-6: one port opinion */
    for (int i = 0; i < fch_n; i++) if (fch[i].nd == nd) return fch[i].fp;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; return e->loff; }
int zls_result_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->off : -1; }
int zls_node_bytes(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return 0; int end = e->off; for (int i = 0; i < zf_n; i++) if (zf[i].nd == nd && zf[i].scope_id == e->scope_id && zf[i].off + zf[i].size > end) end = zf[i].off + zf[i].size; int b = end - e->off; return (b + 15) & ~15; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_scope_of(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); return e ? e->scope_id : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_nslots(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->nslots : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_region(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->region : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_resume(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->resume_off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ICN-FR-4 ZFRAME GENERATOR RESUME — emit-time lookup for the callee's resume-slot byte offset by PROC NAME.
 * bb_call_proc_staged knows the callee name (_.op_sval) but not the callee's IR_graph_t (cross-graph).
 * This avoids adding a resume_slot field to rt_proc_t (which has a baked sizeof==128 static-assert) or to
 * rt_pcall_t (sizeof==64, shl-6 stride baked in rtx_call.S).  zls_graph_t.name is the canonical proc name set
 * at zls_g_register_scope; the scan is done ONCE at emit time, result baked as an immediate in the template.
 * Returns -1 when name is NULL, not found, or the graph has no resume slot (non-generator). */
int zls_g_resume_by_name(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < zg_n; i++)
        if (zg[i].name && strcmp(zg[i].name, name) == 0)
            return zg[i].resume_off;
    return -1;
}
int zls_g_icn_zframe_gen_by_name(const char *name) { if (!name) return 0; for (int i = 0; i < zg_n; i++) if (zg[i].name && strcmp(zg[i].name, name) == 0) return zg[i].g ? zg[i].g->icn_zframe_gen : 0; return 0; }   /* ICN-FR-5 (77aa7119): callee icn_zframe_gen lookup by name -- referenced in bb_call_proc_staged.cpp:566; defined here to fix the undefined-reference build error after M-3 merge. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-FR-4 TRAIL MARK SLOT — setter called by ir_drive_slot_assign after zls_build runs (so zls_off(nd) is valid); lookup used by bcps_spine_gen_arm in bb_call_proc_staged.cpp to know the callee's trail-mark frame offset for β-resume frame population. */
void zls_g_set_pl_trail_mark(const IR_graph_t *g, int off) { if (!g) return; for (int i = 0; i < zg_n; i++) if (zg[i].g == g) { zg[i].pl_trail_mark_off = off; return; } }
int zls_g_pl_zf_trail_mark_off_by_name(const char *name) { if (!name) return 0; for (int i = 0; i < zg_n; i++) if (zg[i].name && strcmp(zg[i].name, name) == 0) return zg[i].pl_trail_mark_off > 0 ? zg[i].pl_trail_mark_off : 0; return 0; }   /* PL-FR-4 s12: callee trail-mark slot lookup by name; mirrors zls_g_icn_zframe_gen_by_name pattern; returns 0 (unset) if absent. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_g_pl_trail_mark_by_name(const char *name) { if (!name) return -1; for (int i = 0; i < zg_n; i++) if (zg[i].name && strcmp(zg[i].name, name) == 0) return zg[i].pl_trail_mark_off; return -1; }   /* PL-FR-4: callee trail-mark frame slot offset by name; -1 = not a Prolog zframe pred or no $trail_mark node. */
int zls_g_locals(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->locals_off : -1; }
int zls_g_zeta_mark(const IR_graph_t * g) { zls_graph_t * r = zls_g_find(g); return r ? r->zeta_mark_off : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_tail_* -- RUNG R12-EXIT-1 CARRY-THE-TAIL (Lon directive s68, the static-size proof: "the operator that is the argument has a pre-determined size -- you know the size").  A statement whose only
 * grant-blocker is ONE spine ARBNO converts to the sliding-rsp element scheme: alpha AND beta both push a fixed-size element (op_sb = align16(body window span + right-spine window span + 16B header
 * {entry-cursor, yield-cursor, elem0-flag} + 16B bracket copy {patstk mark, rsp mark} -- the HEAD-cell essentials carried forward so RELEASE unwinds from the TOP element at static depth, never a
 * dynamic count in an address)); alpha's push is op_sb + fp_body (the phantom body pad) so EVERY yield sits at elem - fp_body uniformly (S10c: body cells stay suspended at gamma) and the fail-glue's
 * `add rsp, op_sb` pop lands EXACTLY on the previous element's yield frontier -- LIFO + known size = arithmetic, never indirection.  LOWER registers the structural CANDIDATE (index ranges); this
 * file's layout pass FINALIZES geometry (windows need zls offsets, which exist only here) and registers every range leaf's fcl displacement with the element-region formula.  Declines degrade to the
 * anchored window verbatim (never die).  Residues (rulings of record, not this rung's): nested ARBNO -> ZB-ITER; DEFER windows -> R12-EXIT-2; REPLACE statements, ALTERNATE anywhere, runtime-arg
 * primitives (pre-chain flat operand slots), and captures allocated left of the ARBNO all decline v1. */
void fc_tail_candidate(const IR_t * head, const IR_t * arbno, int i0, int ia, int b0, int b1, int r1) {
    if (!head || !arbno || fct_n >= 64) return;
    fct[fct_n].head = head; fct[fct_n].arbno = arbno; fct[fct_n].i0 = i0; fct[fct_n].ia = ia; fct[fct_n].b0 = b0; fct[fct_n].b1 = b1; fct[fct_n].r1 = r1;
    fct[fct_n].fpl = 0; fct[fct_n].fpb = 0; fct[fct_n].fpr = 0; fct[fct_n].fpr_rsp = 0; fct[fct_n].span = 0; fct[fct_n].rspan = 0; fct[fct_n].opsb = 0; fct[fct_n].fin = 0; fct[fct_n].nw = 0; fct_n++;
}
/* fc_tail_wrap / fc_tail_ncap -- R12-EXIT-1 WRAP-CAPTURE LIFT (Lon ruling this session, the static-size proof applied to the wrap shape ARBNO(body).V: the SAVE cell is just one more predetermined 16
 * in the element sum).  LOWER's candidacy appends the wrap pairs INNERMOST-FIRST (j=0 = the pair whose COND wraps the ARBNO node itself); flow order therefore pushes the flat SAVE cells outermost-
 * first, so at ARBNO.alpha the innermost cell sits at [flat + 0] and pair j at [flat + 16j] -- the alpha copy source [rsp + KA + 16j] with KA = op_sb + fp_body.  The element gains one 16-byte slot per
 * pair directly above the bracket ([HDRB + 32 + 16j]); FINALIZE widens opsb by 16*nw and registers each COND's cross-box read at the UNIFORM YIELD DEPTH displacement fpb + span + rspan + 32 + 16j --
 * the same fcc mechanism as the linear-inner grant, so bb_match_capture's cfc() arm consumes it with ZERO template change.  The pend entry COND records is BY-VALUE on the ___-dcap stack (F2), so the
 * slot only needs to be readable AT COND.alpha -- copy-forward through elements (the bracket's own mechanism) is sufficient, and the flat cell stays live for the exhaust path's SAVE.omega pop. */
void fc_tail_wrap(const IR_t * arbno, const IR_t * save, const IR_t * cond) {
    if (!arbno || !save || !cond) return;
    for (int i = fct_n - 1; i >= 0; i--) if (fct[i].arbno == arbno) { if (fct[i].nw < 4) { fct[i].wsv[fct[i].nw] = save; fct[i].wcd[fct[i].nw] = cond; fct[i].nw++; } return; }
}
int fc_tail_ncap(const IR_t * nd) {
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) return fct[i].nw;
    return 0;
}
int fc_tail_arbno(const IR_t * nd, int * fpb, int * fpl, int * opsb, int * hdrb) {
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) { if (fpb) *fpb = fct[i].fpb; if (fpl) *fpl = fct[i].fpl; if (opsb) *opsb = fct[i].opsb; if (hdrb) *hdrb = fct[i].span + fct[i].rspan; return 1; }
    return 0;
}
int fc_tail_fpr_rsp(const IR_t * nd) {   /* ⭐ N02-FIX: actual rsp-push depth of right-spine -- BUG-5 law applied to right-spine: LIT_INTEGER (RPOS/POS arg) pushes K=16 rsp cells, fpr from fct_fp_range misses them; beta must pop this before reading element slots. */
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) return fct[i].fpr_rsp;
    return 0;
}
int fc_tail_dfr(const IR_t * nd) {   /* PS-3 s153: this finalized candidate carries priced DEFER leaves -> the tail α must ZERO the phantom FPB pad [0,FPB) so the zero-guarded defer β's discriminator
                                      * is deterministic (the pad is otherwise DIRTY STACK -- prior C-call frames -- and granted-leaf βs never cared about content, but the defer β is content-sensitive:
                                      * a garbage nonzero word reads as a γ-record and jmp's wild).  Defer-free candidates keep the unzeroed pad byte-verbatim. */
    for (int i = 0; i < fct_n; i++) if (fct[i].arbno == nd && fct[i].fin) return fct[i].dfr;
    return 0;
}
int fc_tail_release(const IR_t * head, int * brdisp) {
    for (int i = 0; i < fct_n; i++) if (fct[i].head == head && fct[i].fin) { if (brdisp) *brdisp = fct[i].fpr + fct[i].fpb + fct[i].span + fct[i].rspan + 16; return 1; }
    return 0;
}
int fc_tail_head(const IR_t * head) {
    for (int i = 0; i < fct_n; i++) if (fct[i].head == head) return 1;
    return 0;
}
int fc_tail_defer_susp_g(IR_graph_t * g, const IR_t * nd) {   /* PS-3 s153: is nd a PRICED defer leaf inside a FINALIZED tail candidate of graph g?  Returns its SUSP for the zero-guarded β (the
                                             * ε-resume convention: the phantom FPB pad is zeros, and every body box's β must be zero-safe -- granted leaves read a zero cell and fail benignly, but
                                             * the defer's raw `jmp [rsp+0]` on the pad is a jump through NULL, the t3 rip=0 crash.  The guarded β tests the record and, on zero, pops its own SUSP
                                             * share and ω-transits: the exhausted-leaf behavior, consuming the pad exactly as granted leaves consume theirs).  -1 everywhere else -- flat defers,
                                             * DT-arm defers, declined/pending candidates all keep the original β byte-verbatim (default md5 identity). */
    if (!g || !nd || (nd->op != IR_MATCH_DEFER)) return -1;
    for (int i = 0; i < fct_n; i++) if (fct[i].fin && fct[i].arbno) {
        int in = 0; for (int j = 0; j < g->n; j++) if (g->all[j] == fct[i].arbno) { in = 1; break; }
        if (!in) continue;
        for (int j = fct[i].i0; j < fct[i].r1 && j < g->n; j++) if (g->all[j] == nd) return fct_defer_susp(nd);
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_tables_reset -- s67 (the 140/test_case wild-jump fix; s86 U4 sweep deleted the fcanc/fcah tables, two remain).  The fc side tables are keyed by RAW NODE POINTER and were only ever fed by the MAIN
 * lowering; a RUNTIME compile (EVAL chain, CODE fragment, rt pattern tree) lowers fresh IR whose malloc'd nodes can land on a FREED prior graph's addresses (eval_build_chain ends in IR_free_dyn) -- historically a stale fcanc hit emitted ONE r12-viewed box
 * inside an otherwise rsp-viewed self-allocated blob, whose [r12+disp] writes land ABOVE the shim anchor, straight over the C caller's frame (measured: EVAL_fn's return address = 0x21; the blob at
 * +0xa5 wrote 0x148(%r12) while its own allocation was sub 0x1b0,%rsp).  A stale fcl hit is the same disease with a silent wrong-displacement payload.  Every runtime-compile entry resets the fc
 * tables before lowering: emission consults them only for the graph lowered SINCE the reset, and all pre-reset graphs are already emitted (mode-3 emits main wholesale before run).  The zls tables
 * share the pointer-keying and are NOT reset here (bb_compile_pat_tree's zls_reset is the existing precedent; the zls lifecycle question routes to GC-W-1's frame-map design). */
void fc_tables_reset(void) { fct_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_frameless_fpr_rsp -- OS-2·SLICE-ARBNO-FPR (s32): the ONE ADDRESS-MODE AUTHORITY for "this node's value was pushed to rsp by the ZD universal arm in FORTH mode, with no ZLS frame slot".  Returns 1 iff ZC_PORT_FORTH AND fc_geom(nd)==0 (flat-allocated: no ZLS slot, so the 16B cell lives at the rsp frontier, not in a named FRQ slot).  Callers gate on g_zd_arm separately (per-run condition: "was the ZD arm actually armed this compile?") because g_zd_arm is emit.cpp-private staging state, not an address-mode property.  The canonical use case: a POS/RPOS constant-arg path (op_sa=-1) NEVER reads ZOPQ from its LIT_INT operand; in FORTH+ZD the LIT_INT still received `sub rsp,16` (universal arm zd_wl_kind:1907, fc_vlit_active=0 → fc_geom=0); without op_wpop+=16 the failure path orphans that cell, displacing any rsp-resident ARBNO header ([rsp+0/4] entry/yield delta) that follows on the right spine.  N21 witness: two-sibling ARBNO POS(0) ARBNO('a') $ OUTPUT ARBNO('b') $ OUTPUT RPOS(0) on 'aabb' → =F, oracle =S. */
int fc_frameless_fpr_rsp(const IR_t * nd) { extern int rt_zeta_port_mode(void); if (!nd || rt_zeta_port_mode() != ZC_PORT_FORTH) return 0; { long _fk = 0; return !fc_geom(nd, &_fk); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* emit_patzeta_* -- PS-3 (s152, Lon directive: "implement ZETA size calculation for DT_P type DESCR_t").  The compile-time ζ size of an ACTIVATED DT_P is its blob's suspension footprint: entry carve
 * align16(32 + frame_bytes) PLUS the interior FORTH port cells suspended at γ (S10c law -- measured in t1.s: SPAN's 16 stays carved on the hit path) PLUS the 16B γ-frontier record {res-addr, saved
 * ___} the jmp-entry epilogue pushes (proc_PAT$_γ: push ___; push res).  frame_bytes alone (the PS-1b registry payload) is therefore NOT the ζ size -- the fp term is the fct_fp_range sum over the
 * whole graph (ALT arms enter as 16+fpmax, the S10d pad making the yield depth uniform).  UNIFORM means the footprint is the SAME every activation: region known AND no interior ARBNO (its retained
 * elements vary per activation -- a variable-count carve zstatic never sees, the s152 strengthening) AND no interior DEFER/VALUE (the PS-1b zstatic predicate).  Name-keyed, fed by the driver proc
 * loops in BOTH modes BEFORE main emission (MODE34-IDENTICAL), append-only across runtime compiles (names are unique per process; never reset -- the fc_tables_reset lifecycle note applies). */
static struct { const char * name; int fb; int fp; int uni; } pz[512];
static int pz_n = 0;
void emit_patzeta_register(const char * name, int frame_bytes, int fp_total, int uniform) {
    if (!name || pz_n >= (int)(sizeof pz / sizeof *pz)) return;
    for (int i = 0; i < pz_n; i++) if (!strcmp(pz[i].name, name)) { pz[i].fb = frame_bytes; pz[i].fp = fp_total; pz[i].uni = uniform; return; }
    pz[pz_n].name = name; pz[pz_n].fb = frame_bytes; pz[pz_n].fp = fp_total; pz[pz_n].uni = uniform; pz_n++;
}
int emit_patzeta_lookup(const char * name, int * susp) {
    if (!name) return 0;
    for (int i = 0; i < pz_n; i++) if (!strcmp(pz[i].name, name)) { if (susp) *susp = (((32 + pz[i].fb + 15) & ~15) + pz[i].fp + 16); return pz[i].uni; }
    return 0;
}
int zls_g_fp_total(IR_graph_t * g) { return g ? fct_fp_range(g, 0, g->n) : 0; }
int zls_node_has_fields(const IR_t * nd) { if (!nd) return 0; for (int f = 0; f < zf_n; f++) if (zf[f].nd == nd) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZW-1 UNIVERSAL K AUTHORITY (Lon directive s21x-m "turn on the per-BB allocation across the board, then crawl the rungs"): the ONE per-node cell-size answer for the universal alpha-sub/omega-add
 * regime (SCRIP_BB_ALLOC).  K = the node's whole zls extent (front result quad + granted locals, ceil16) -- EXCEPT a dead-result node with no locals, which gets ZERO (the zero-cell law: "RESULT iff
 * it has one and it is used; LOCALS iff any"; no zls entry -> no storage need -> 0).  v0 is CARVE-ONLY BY DESIGN: the consumer sets op_fc_bytes WITHOUT op_fc_base, so x86_fc_hit never fires, every
 * FR/FRQ spelling stays byte-identical, and the ONLY new machine effect is the rsp motion at the alpha/omega hook -- one variable isolated, the crosscheck fail-set becomes the rung ladder.  Window
 * migration (front quad + locals riding the cell) is the crawl's second axis, per family, against the RESULT-IS-THE-CELL and SUSPENDED-CELL laws. */
long zw_node_k(const IR_t * nd) { const zls_entry_t * e = nd ? zx_find(nd) : (const zls_entry_t *)0; if (!e) return 0; long b = (long)zls_node_bytes(nd); if (!e->live && b <= 16) return 0; return b; }
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* CARVE-ERAD step 1 -- THE ONE CARVE AUTHORITY (Lon "complete ZETA CELLS on the RSP FORTH-style stack; remove the whole graph carve").  zw_carve_k answers ONE question -- how many bytes does this node
 * actually carve on rsp at its own alpha -- and it is the ONLY place that question is answered.  Provenance: the predicate was written INLINE at emit.cpp's walk_bb_node_inner choke, where it decided
 * op_fc_bytes; the lower-side prefix walk (fc_leaf_walk, the op_flat_disp registrar) accumulated fc_geom cells ONLY and therefore could not see a single universal carve.  THAT SPLIT IS THE DISPLACEMENT:
 * a spine producer carves K at alpha and holds it live for its consumer (RESULT-IS-THE-CELL), while a consumer still addressing through the static authority compensates by a prefix computed as if the
 * carve did not exist, and reads K bytes too low -- the emit.cpp:802 negative result ("arming a spine producer that FEEDS a match deepens rsp under a consumer whose compensation cannot see it"), which
 * is the arming frontier being a property of the READERS, not of the armed box.  With one authority the allocator and the address side cannot hold two opinions, which is the same ONE-K discipline
 * zw_node_k already established for the size.  Port/regime/bisect conjuncts all live here so the prefix advances by EXACTLY the bytes the alpha hook will spend, node for node. */
static int zw_nid_listed_c(const char * e, int nid) { if (!e || !*e) return 0; { const char * p = e; while (*p) { long v = strtol(p, (char **)&p, 10); if ((int)v == nid) return 1; while (*p && *p != ',') p++; if (*p) p++; } } return 0; }
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long zw_carve_k(const IR_t * nd) {
    static int _ba = -1, _all = -1; static const char * _bo; static const char * _bs;
    extern int rt_zeta_port_mode(void); extern int bb_node_id(IR_t *); long _d, _k; int _spine;
    if (_ba < 0) { const char * e = getenv("SCRIP_BB_ALLOC"); _ba = (e && *e == '0') ? 0 : 1; _bo = getenv("SCRIP_BB_ONLY"); _bs = getenv("SCRIP_BB_SKIP"); { const char * a = getenv("SCRIP_BB_ALLOC_ALL"); _all = (a && *a == '0') ? 0 : 1; }   /* UNIVERSAL CARVE default-ON (Lon s12): the five-kind exclusion list dies unless =0 opt-out */ }
    if (!_ba || !nd) return 0;
    _spine = (nd->op == IR_BINOP || nd->op == IR_ASSIGN || nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_VAR || nd->op == IR_CMP_TEST || nd->op == IR_COERCE_NUMERIC);
    if (_spine || rt_zeta_port_mode() != ZC_PORT_FORTH) return 0;
    if (!_all && (nd->op == IR_SAVE_RESTORE || ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_GOTO_DEFERRED || nd->op == IR_GLIT || nd->op == IR_GCC || nd->op == IR_GALT)) return 0;   /* RK-GRAM-3d: grammar-box nodes use [___+N] frame slots exclusively; never participate in RSP FORTH-spine carve */
    if (fc_geom(nd, &_d)) return 0;
    _k = zw_node_k(nd); if (_k <= 0) return 0;
    { int _nid = bb_node_id((IR_t *)nd); if (_bo && *_bo && !zw_nid_listed_c(_bo, _nid)) return 0; if (zw_nid_listed_c(_bs, _nid)) return 0; }
    return _k;
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZOP-1 MIX AUDIT (Lon s21x-n "parameterize the access to operands via the FOUR modes").  Tallies, over a whole compile, how many emitted graphs resolved their operand addresses through MORE THAN ONE
 * whole-graph regime.  The regimes are x86_zop's named arms: 1 = r12 island frame, 3 = pinned ___ whole-graph frame, 4 = rsp whole-graph frame + op_flat_disp compensation.  Arm 2 (the box's own per-BB
 * rsp cell) is deliberately NOT counted as a regime -- it is a WINDOW into whichever regime the graph is in, and composing one window with one frame is the design, not the defect.  Two or more of {1,3,4}
 * on a single graph is the s188 mixed shape zeta_choices.h declares must be UNREPRESENTABLE; it stayed silently representable for as long as the deciding booleans lived in three separate accessors, and
 * "silently" is the operative word -- x86_fc_hit's fallback is documented as returning a WRONG-BASE address without crashing, so a mixed graph produced plausible code and a wrong answer.  Reported under
 * SCRIP_ZOP_AUDIT=1; the gate script asserts mixed == 0. */
#include <stdio.h>
#include <stdlib.h>
static long zop_g_total = 0, zop_g_mixed = 0, zop_hist[16];
void zop_audit_report(void);
void zop_audit_graph_close(void);
void zop_audit_graph_close(void) {
    extern int zop_audit_seen(void); extern void zop_audit_seen_clear(void); int m, frames;
    { static int reg = 0; if (!reg) { reg = 1; atexit(zop_audit_report); } }   /* ZOP-1: registered UNCONDITIONALLY on first call, never inside the m!=0 guard -- the guarded form made the exit report itself conditional on some graph having already been tallied, so a program whose only graph was still open at exit registered nothing and reported nothing. */
    m = zop_audit_seen(); if (m == 0) return; zop_audit_seen_clear();
    frames = ((m >> 1) & 1) + ((m >> 3) & 1) + ((m >> 4) & 1);
    zop_g_total++; if (frames > 1) zop_g_mixed++; zop_hist[(m >> 1) & 15]++;
}
void zop_audit_report(void) {
    zop_audit_graph_close();   /* ZOP-1 FINAL-GRAPH FIX: close fires at the START of each new graph, so the LAST graph of every compile was never tallied -- and a single-graph program tallied NOTHING AT ALL, which is how the first run of this instrument reported 38 graphs across 120 programs and a clean mixed=0.  A zero from an instrument that never ran is not a measurement; closing here is what makes the number mean anything. */
    if (!getenv("SCRIP_ZOP_AUDIT")) return;
    fprintf(stderr, "[ZOP] graphs=%ld mixed=%ld\n", zop_g_total, zop_g_mixed);
    { int k; const char * nm[16] = {"-","isle","cell","isle+cell","rsp","isle+___","cell+___","isle+cell+___","rsp","isle+rsp","cell+rsp","isle+cell+rsp","___+rsp","+","cell+___+rsp","all"};
      for (k = 0; k < 16; k++) if (zop_hist[k]) fprintf(stderr, "[ZOP]   %-18s %ld\n", nm[k], zop_hist[k]); }
}
