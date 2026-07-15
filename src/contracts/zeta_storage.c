#include <string.h>
#include <stdlib.h>
#include "zeta_storage.h"
#include "zeta_choices.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern const char * bb_op_name(IR_e k);
extern int is_global(const char *);
extern int rt_proc_is_registered(const char *);
extern int rt_proc_is_generator(const char *);
static int zls_callee_is_gen(const IR_t * nd) { const char * fn = IR_LIT(nd).sval; return fn && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZLS_MAX_ENTRIES 65536
#define ZLS_MAX_FIELDS  131072
#define ZLS_MAX_SCOPES  4096
#define ZLS_MAX_GRAPHS  256
#define ZLS_MAX_VSLOTS  4096
#define ZLS_MAX_MARKS   8192
typedef struct { const IR_t * nd; int scope_id; int off; } zls_entry_t;
typedef struct { int scope_id; int off; int size; unsigned char kind; unsigned char audit; const char * what; const IR_t * nd; } zls_pfield_t;
typedef struct { const char * name; int off; } zls_vslot_t;
typedef struct { const IR_graph_t * g; const char * name; int start_n; } zls_mark_t;
typedef struct { const IR_graph_t * g; const char * name; int first_scope; int n_scopes; int nslots; int region; int resume_off; int zeta_mark_off; int first_vslot; int n_vslots; } zls_graph_t;
static zls_entry_t  ze[ZLS_MAX_ENTRIES];  static int ze_n = 0;
static zls_pfield_t zf[ZLS_MAX_FIELDS];   static int zf_n = 0;
static zls_scope_t  zs[ZLS_MAX_SCOPES];   static int zs_n = 0;
static zls_graph_t  zg[ZLS_MAX_GRAPHS];   static int zg_n = 0;
static zls_vslot_t  zv[ZLS_MAX_VSLOTS];   static int zv_n = 0;
static zls_mark_t   zm[ZLS_MAX_MARKS];    static int zm_n = 0;
static zls_entry_t * zx[ZLS_MAX_ENTRIES]; static int zx_n = 0;
typedef struct { const IR_t * nd; int min_off; int span; } zls_ageom_t;
static zls_ageom_t  za[1024];             static int za_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_reset(void) { ze_n = 0; zf_n = 0; zs_n = 0; zg_n = 0; zv_n = 0; zm_n = 0; zx_n = 0; za_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_group_mark(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    if (zm_n >= ZLS_MAX_MARKS) { fprintf(stderr, "zls: mark table overflow (%d)\n", ZLS_MAX_MARKS); abort(); }
    zm[zm_n++] = (zls_mark_t){ g, name, g->n };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static zls_graph_t * zls_g_find(const IR_graph_t * g) { for (int i = 0; i < zg_n; i++) if (zg[i].g == g) return &zg[i]; return (zls_graph_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_graph_name(const IR_graph_t * g, const char * name) {
    if (!g || !name) return;
    zls_graph_t * r = zls_g_find(g);
    if (r) { r->name = name; if (r->first_scope >= 0 && r->first_scope < zs_n) zs[r->first_scope].name = name; return; }
    if (zg_n >= ZLS_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", ZLS_MAX_GRAPHS); abort(); }
    zg[zg_n] = (zls_graph_t){ g, name, -1, 0, 0, 0, -1, -1, 0 };
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
static void zls_entry(const IR_t * nd, int scope_id, int off) {
    if (ze_n >= ZLS_MAX_ENTRIES) { fprintf(stderr, "zls: entry table overflow (%d)\n", ZLS_MAX_ENTRIES); abort(); }
    ze[ze_n] = (zls_entry_t){ nd, scope_id, off };
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
    case IR_MATCH_HEAD:
        /* BB-OWNED-ζ statement-scope pivot (this session): the existing 16B grant only ever used its first
         * 4 bytes (head.cursor, an int32 counter read/written as FR(op_off) by bb_match_head.cpp) -- 12 bytes
         * of padding sat unused.  Reusing 8 of those bytes (off+8) for the saved rt_zls_mark() pointer needs
         * NO change to the slot count (still "return 1", still one 16B quad) and therefore cannot shift any
         * later node's offset -- the safest possible way to add a field.  ZK_PTR_GC to match the precedent
         * set by IR_MATCH_ARBNO's own COLLECTION pointer (arbno2.COLLECTION ptr below): both are pointers
         * into the SAME zeta arena, whose address-range rooting (rt_zls_alloc's GC_add_roots widening,
         * zeta_alloc.c) already covers them regardless of tag, but the tag is recorded honestly for
         * whichever future consumer reads it, not left as an untagged raw word next to a tagged sibling. */
        zls_field(scope_id, off, 4, ZK_RAW, 0, "head.cursor (ZB-FC-3d granted: the LIVE anchor lives in HEAD's self-pushed 32B rsp cell at [rsp+0] via the op_fc_wbytes window; this FLAT +0 then holds the RELEASE-stashed match START read by IR_MATCH_REPLACE -- same logical offset, window-disambiguated, so REPLACE's template is unchanged both paths)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "head.zeta_mark (BB-OWNED-zeta statement-scope saved rt_zls_mark() pointer; ZB-FC-3d granted: cell-resident at [rsp+8])", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "head.zls2_mark (ZC_PORT_ALLOC only: saved rt_zls2_mark() cursor; released by head's own omega-choke on failure / IR_MATCH_RELEASE on success — the ZLS2 twin of head.zeta_mark, widened to a second quad because the first quad's padding is spent.  ZB-FC-3d granted: cell-resident at [rsp+16] holding the PRE-PUSH rsp, so the S10e unwind releases HEAD's cell and every suspended pattern cell in one mov)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "head.end (SN4-REPL: end cursor stashed by IR_MATCH_RELEASE when the statement carries a replacement, read by IR_MATCH_REPLACE; ZB-FC-3d: FLAT on both paths -- post-unwind lifetime)", nd); zls_field(scope_id, off + 32, 8, ZK_RAW, 0, "head.dcap_mark (rbp-dcap: α saves the mirror-loaded rbp cursor = this match's pend MARK; ω/RELEASE restore rbp and g_dcap_top from it; ZK_RAW — points into the register-anchored dcap island, never GC-moved.  ZB-FC-3d: FLAT on both paths -- RELEASE's post-unwind pump reads it after the cell dies)", nd); zls_field(scope_id, off + 40, 8, ZK_RAW, 0, "head.incoming_rbp (rbp-dcap: the C caller's callee-saved rbp, clobbered by α's mirror load and restored at BOTH exits — this pair is what lets graphs stay rbp-free while match spans own the register.  ZB-FC-3d: FLAT on both paths, same post-unwind argument as dcap_mark)", nd); return 3;
    case IR_MATCH_SPAN:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "span.cnt/cur", nd); return 1;
    case IR_MATCH_BAL:
        /* SN4-BAL (s34): n / entry-δ / paren-depth.  The SPAN shape, not the ARB shape: BAL is a pure in-frame
         * generator that pushes no ζ activation, so it takes NO ZLS2 grant (zls2_geom returns 0 for it). */
        zls_field(scope_id, off, 16, ZK_RAW, 0, "bal.n/start/depth", nd); return 1;
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "break.cnt/cur", nd); return 1;
    case IR_MATCH_ARB:
        /* ZLS2 second consumer (Claude Sonnet 5, 2026-07-08) -- the "natural sibling" GOAL-SNOBOL4-BB.md names
         * for extending BB-OWNED-zeta past ARBNO. ARB's existing 16B grant already carried 8B of unused pad
         * (bytes 0-3 = matched-length counter, 4-7 = saved start position, 8-15 = pad) -- the SAME shape
         * IR_MATCH_HEAD's zeta_mark reuse found above, so the ZLS2 activation-block save-slot fits in the
         * EXISTING quad with zero widening: no later node's offset shifts, the safest possible way to add the
         * field (mirrors head.zeta_mark, not ARBNO's wider 2-quad case -- ARB only ever used 8B of its 16B and
         * already had exactly 8B of pad to spend). */
        zls_field(scope_id, off, 8, ZK_RAW, 0, "arb.cnt/cur (matched-length +0 4B, saved-start +4 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_PTR_GC, 0, "arb.zls2 activation block ptr (save-slot-in-frame, ZC_PORT_ALLOC only: reuses this node's existing pad, same reuse precedent as IR_MATCH_HEAD.zeta_mark; block itself is a separate ZLS2 allocation, header +0 chains the previous activation's ptr)", nd); return 1;
    case IR_MATCH_REM:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "match.cursor save", nd); return 1;
    case IR_MATCH_DEFER:
        zls_field(scope_id, off, 16, ZK_RAW, 0, "defer.pad (ZS-2 jmp-entry, Lon s58: the fn/frame cell pair is DELETED — the blob is a jmp-entered new activation that self-allocates on rsp with a 32B wire header, so there is nothing to stash and nothing to guard; quad KEPT at 16B so no later node's offset shifts)", nd); return 1;
    case IR_MATCH_TAB: case IR_MATCH_RTAB:
        /* UNIFORM-BETA WIRING (Claude, this session, per Lon "EVERY BB must be wired properly"): TAB/RTAB
         * OVERWRITE r14d (mov, not add) — the only match primitives whose cursor effect is unrecoverable by
         * recomputation, so their β-restore needs a saved copy.  Same 16B "cursor save" shape as REM above
         * (α: mov FR(off), r14d; β: mov r14d, FR(off)); POS deliberately EXCLUDED — it mutates nothing and
         * is the canonical pure box (needs no RW data, touches no ζ). */
        zls_field(scope_id, off, 16, ZK_RAW, 0, "tab.cursor save (+0 4B r14d saved at α, restored at β; +4 pad)", nd); return 1;
    case IR_MATCH_ARBNO:
        if (IR_LIT(nd).ival == 1) { zls_field(scope_id, off, 16, ZK_RAW, 0, "arbno.owner quad: entry/yield/i/cap (4x4B; SN4-NARY-ARBNO one-node form)", nd); zls_field(scope_id, off + 16, 8, ZK_PTR_GC, 0, "arbno.COLLECTION ptr (rt_zcol_push-grown per-iteration elements: 16B header {prev_view, saved_delta} + body slot window; the rsp flavor = linked frame chain + explicit count in the header — Lon ruling 2026-07-12, lands at ZB-ITER under ZLS_ARBNO_STACK)", nd); zls_field(scope_id, off + 24, 8, ZK_RAW, 0, "arbno.pad (unused)", nd); return 2; }
        return 0;
    case IR_MATCH_ASSIGN_SAVE:
        zls_field(scope_id, off, 8, ZK_PTR_GC, 0, "capture.stack GC_MALLOC_ATOMIC u32[] ([0]=cap, frames from [1]; box α-push/β-pop)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "capture.stack gen(+8,4B)/sp(+12,4B)", nd); return 1;
    case IR_MATCH_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "alt.entry cursor save (+0 4B r14d) + alt_i live-alternative index (+4 4B; α=0, fail-glue ++; β dispatches on it)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "alt.pad (unused — was the dcap height save for the deleted rt_dcap_height/restore_to pair; rbp-dcap s46 proved ALTERNATE needs NO pend-stack state at all: generator LIFO already restores rbp before any alternative switch is visible, MEASURED watermark-exact in both flavors with the save/restore removed.  Quad KEPT at 16B: shrinking it would shift every later node's offset for zero gain)", nd); return 1;
    case IR_MATCH_SEQUENCE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "seq.entry cursor save (+0 4B r14d; SU-C result anchor, no reload — elements undo their own δ) + seq_i live-element index (+4 4B; α=0, ns_s ++, ns_f --, β=N; flat-frame array flavor of the two-flavor design — rsp flavor = linked frame chain, lands with ZB-ITER/ZB-OWN)", nd); return 1;
    case IR_SCAN_SEQUENCE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scanseq.entry δ save (+16 from box base, 4B r14d) + seq_i live-element index (+20, 4B; α=0, na_s ++, na_f --, β=N) — same wiring as IR_MATCH_SEQUENCE; the value DESCR is the box result slot at [base]", nd); return 1;
    case IR_SCAN_ALTERNATE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scanalt.entry δ save (+16 from box base, 4B r14d) + dcap height (+20, 4B)", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scanalt.alt_i live-alternative index (+24, 4B; α=0, na_f ++; β dispatches) (+28 pad)", nd); return 2;
    case IR_SCAN:
        return 0;
    case IR_SCAN_TAB: case IR_SCAN_MOVE:
        zls_field(scope_id, off, 8, ZK_RAW, 0, "scan.r14 data-backtrack save", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "scan.pad (unused)", nd); return 1;
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
        zls_field(scope_id, off + 56, 8, ZK_RAW, 0, "coexpr.marshal rbp", nd);
        return 4;
    case IR_ASSIGN:
        return 0;
    case IR_INDIRECT_GOTO: case IR_DISJUNCTION:
        zls_field(scope_id, off, 8, ZK_PTR_CODE, 0, "gate.stored resume target", nd); zls_field(scope_id, off + 8, 8, ZK_RAW, 0, "gate.pad (unused)", nd); return 1;
    case IR_CALL_BUILTIN_GEN:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off * (1 + nd->n_operands), 8, ZK_RAW, 0, "callgen.resume position (alpha=0, runtime writes next start)", nd);
        zls_field(scope_id, off * (1 + nd->n_operands) + 8, 8, ZK_RAW, 0, "callgen.pad (unused)", nd);
        return 1 + nd->n_operands;
    case IR_PROC_GEN: case IR_CALL_VALUE:
        for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off * j, 16, ZK_DESCR, 0, "call.argv", nd);
        zls_field(scope_id, off * (1 + nd->n_operands), 8, ZK_PTR_GC, 0, "callgen.act ZLS2 activation handle (box-owned: alpha writes via rt_proc_call_gen_h's hout, beta resumes rt_proc_resume_frame(handle) — replaces the deleted global g_gen_act stack)", nd);
        zls_field(scope_id, off * (1 + nd->n_operands) + 8, 8, ZK_RAW, 0, "callgen.act pad (unused)", nd);
        return -1 + nd->n_operands;
    default:
        if (nd->op == IR_CALL || ir_is_call_kind(nd->op)) {
            for (int j = 0; j < nd->n_operands; j++) zls_field(scope_id, off * j, 16, ZK_DESCR, 0, "call.argv", nd);
            if (nd->op == IR_CALL_PROC_STAGED && zls_callee_is_gen(nd)) {
                zls_field(scope_id, off * (1 + nd->n_operands), 8, ZK_PTR_GC, 0, "callgen.act ZLS2 activation handle (box-owned: alpha writes via rt_proc_call_gen_h's hout, beta resumes rt_proc_resume_frame(handle) — replaces the deleted global g_gen_act stack)", nd);
                zls_field(scope_id, off * (1 + nd->n_operands) + 8, 8, ZK_RAW, 0, "callgen.act pad (unused)", nd);
                return 2 + nd->n_operands;
            }
            return 1 + nd->n_operands;
        }
        if (ir_node_produces_value(nd->op)) { return 1; }
        return 0;
    }
}
static int zls_is_wiring(IR_e op) { return op == IR_GOTO || op == IR_MOVE_LABEL || op == IR_GOTO_DEFERRED || op == IR_SUCCEED || op == IR_FAIL || op == IR_RETURN || op == IR_SUSPEND || op == IR_CORET || op == IR_COFAIL || op == IR_CUT || op == IR_MATCH_RELEASE; }
static int zls_locals_shifted(IR_e op) { return op == IR_MATCH_HEAD || op == IR_MATCH_ALTERNATE || op == IR_MATCH_SEQUENCE || op == IR_MATCH_ARB || op == IR_MATCH_BAL || op == IR_MATCH_ARBNO || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_REM || op == IR_MATCH_DEFER || op == IR_MATCH_ASSIGN_SAVE || op == IR_SCAN_ENTER || op == IR_INITIAL; }
static int zls_grant(const IR_t * nd, int scope_id, int off) {
    if (zls_is_wiring(nd->op)) return 0;
    zls_entry(nd, scope_id, off);
    zls_field(scope_id, off, 16, ZK_DESCR, 0, "result", nd);
    return 1 + zls_grant_locals(nd, scope_id, off + 16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zls_scope_new(int parent, int klass, const char * name) {
    if (zs_n >= ZLS_MAX_SCOPES) { fprintf(stderr, "zls: scope table overflow (%d)\n", ZLS_MAX_SCOPES); abort(); }
    zs[zs_n] = (zls_scope_t){ zs_n, parent, klass, name, -1, 0, 0x7fffffff, 0 };
    return zs_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zls_build(IR_graph_t * g) {
    if (!g) return;
    zls_graph_t * r = zls_g_find(g);
    if (r && r->first_scope >= 0) return;
    if (!r) {
        if (zg_n >= ZLS_MAX_GRAPHS) { fprintf(stderr, "zls: graph table overflow (%d)\n", ZLS_MAX_GRAPHS); abort(); }
        zg[zg_n] = (zls_graph_t){ g, (const char *)0, -1, 0, 0, 0, -1, -1, 0 }; r = &zg[zg_n]; zg_n++;
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
    int base = 16 + (g->nparams > 0 ? g->nparams * 16 : 0);
    int k = 0;
    r->first_vslot = zv_n;
    for (int i = 0; i < g->nparams && g->pnames; i++) if (g->pnames[i]) {
        if (zv_n >= ZLS_MAX_VSLOTS) { fprintf(stderr, "zls: vslot table overflow (%d)\n", ZLS_MAX_VSLOTS); abort(); }
        zv[zv_n++] = (zls_vslot_t){ g->pnames[i], 16 + i * 16 }; r->n_vslots++;
        zls_field(root, 16 + i * 16, 16, ZK_DESCR, 0, "param", (const IR_t *)0);
    }
    int cur = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        while (cur < nl && i >= mstart[cur]) cur++;
        int sc = (cur > 0) ? mfirst[cur - 1] : root;
        k += zls_grant(nd, sc, base + k * 16);
    }
    r->resume_off = -1;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) {
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
    r->zeta_mark_off = base + k * 16;
    zls_field(root, r->zeta_mark_off, 8, ZK_RAW, 0, "graph-scope zeta mark (rt_zls_mark snapshot, prologue-stashed, epilogue-released)", (const IR_t *)0);
    zls_field(root, r->zeta_mark_off + 8, 8, ZK_RAW, 0, "zeta_mark.pad (unused)", (const IR_t *)0);
    k += 1;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
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
        if (!nd || nd->op != IR_MATCH_ARBNO || IR_LIT(nd).ival != 1 || nd->n_operands < 3) continue;
        int i0 = -1, i1 = -1;
        for (int j = 0; j < g->n; j++) { if (g->all[j] == nd->operands[1]) i0 = j; if (g->all[j] == nd->operands[2]) i1 = j; }
        if (i0 < 0 || i1 < 0) { fprintf(stderr, "zls: arbno2 geometry — body bracket operands not found in g->all\n"); abort(); }
        if (i0 > i1) { int t = i0; i0 = i1; i1 = t; }
        int mn = 0x7fffffff, mx = 0;
        for (int j = i0; j <= i1; j++) {
            const zls_entry_t * e = g->all[j] ? zx_find(g->all[j]) : (const zls_entry_t *)0;
            if (!e) continue;
            if (e->off < mn) mn = e->off;
            for (int f = 0; f < zf_n; f++) if (zf[f].nd == g->all[j] && zf[f].off + zf[f].size > mx) mx = zf[f].off + zf[f].size;
        }
        if (za_n >= (int)(sizeof za / sizeof *za)) { fprintf(stderr, "zls: arbno2 geometry table overflow (%d)\n", (int)(sizeof za / sizeof *za)); abort(); }
        if (mn == 0x7fffffff) za[za_n++] = (zls_ageom_t){ nd, 16, 0 };
        else                  za[za_n++] = (zls_ageom_t){ nd, mn, mx - mn };
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_arbno_geom(const IR_t * nd, int * min_off, int * span) {
    for (int i = 0; i < za_n; i++) if (za[i].nd == nd) { if (min_off) *min_off = za[i].min_off; if (span) *span = za[i].span; return 1; }
    return 0;
}
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
int fc_geom(const IR_t * nd, long * k) {
    if (!nd) return 0;
    if (nd->op == IR_MATCH_ASSIGN_SAVE && fc_save_active(nd)) { if (k) *k = 16; return 1; }   /* ZB-FC-3c: delta at cell+0; ungranted SAVE stays zero-cell = the flat rt_cap array path */
    if (nd->op == IR_MATCH_ARB)    { if (k) *k = 16; return 1; }   /* ZB-FC-4 (Lon s50 S14): the 8-byte counter+saved-cursor cell, ex-zls2, now a clean fixed FORTH cell */
    if (nd->op == IR_MATCH_SPAN)   { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_TAB)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_RTAB)   { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAK)  { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BREAKX) { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_BAL)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_REM)    { if (k) *k = 16; return 1; }
    if (nd->op == IR_MATCH_ALTERNATE && fc_alt_fpmax(nd) >= 0) { if (k) *k = 16; return 1; }
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
static struct { const IR_t * nd; int n; int fp[16]; } fca[256];
static int fca_n = 0;
void fc_alt_register(const IR_t * nd, int n, const int * fp) {
    if (!nd || n <= 0 || n > 10 || fca_n >= 256) return;   /* N>10 exceeds the 3N+2 <= XA_BB_EMIT_PAIR_MAX(32) stub budget -- silent decline */
    fca[fca_n].nd = nd; fca[fca_n].n = n;
    for (int i = 0; i < n && i < 16; i++) fca[fca_n].fp[i] = fp[i];
    fca_n++;
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
int fc_seq_active(const IR_t * nd) {
    if (!nd || nd->op != IR_MATCH_SEQUENCE) return 0;
    for (int i = 0; i < fcs_n; i++) if (fcs[i] == nd) return 1;
    return 0;
}
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
static const IR_t * fcv[256];
static int fcv_n = 0;
void fc_save_register(const IR_t * nd) { if (!nd || fcv_n >= 256) return; fcv[fcv_n++] = nd; }
int fc_save_active(const IR_t * nd) {
    if (!nd || nd->op != IR_MATCH_ASSIGN_SAVE) return 0;
    for (int i = 0; i < fcv_n; i++) if (fcv[i] == nd) return 1;
    return 0;
}
static struct { const IR_t * nd; int fp; } fcc[256];
static int fcc_n = 0;
void fc_cond_register(const IR_t * nd, int fp_inner) {
    if (!nd || fp_inner < 0 || fcc_n >= 256) return;
    fcc[fcc_n].nd = nd; fcc[fcc_n].fp = fp_inner; fcc_n++;
}
int fc_cond_fp(const IR_t * nd) {
    for (int i = 0; i < fcc_n; i++) if (fcc[i].nd == nd) return fcc[i].fp;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_head_* -- RUNG ZB-FC-3d (ARCH-ZETA S13 Tier C, HEAD/RELEASE/REPLACE; ruling of record: the D4 splice-survivor fork resolved by PARTITION, s49).  HEAD is NOT hook-shaped (fc_geom stays 0 BY LAW so
 * no enclosing sum ever counts a self-releasing cell): the template pushes its own 32-byte cell after alpha holding ONLY the match-span-lifetime fields (anchor@+0, zls mark@+8, rsp mark@+16 = the
 * PRE-PUSH rsp, so the existing S10e statement UNWIND releases the cell and every suspended pattern cell in one mov), addressed through the op_fc_wbytes WINDOW (rebase without the hook).  The four
 * fields with POST-UNWIND lifetime stay FLAT on the existing offsets -- dcap mark@+32 / saved rbp@+40 (RELEASE's post-unwind pump + HEAD's own omega read them) and the splice pair for REPLACE (end@+24
 * written by RELEASE pre-unwind as today; START copied by RELEASE pre-unwind from the cell into flat +0, which the granted HEAD no longer writes -- REPLACE's reads are UNCHANGED both paths).  RELEASE
 * reads the cell CROSS-BOX at [rsp + fp(pattern) + k] via op_fc_disp (the 3c mechanism verbatim); staticity of fp holds because the v1 fence is ALT-FREE (the linear range sum over-counts alternation --
 * only one padded arm is live at yield -- so statements containing ALTERNATE decline wholesale; the per-ALT 16+fpmax lift is a named follow-on).  Eligibility = the 3c walk verbatim over the PATTERN
 * range only; either-direction failure declines and the whole statement keeps the flat path byte-verbatim (degrade never die).  Side table keyed by the HEAD node ptr (the fcc precedent). */
static struct { const IR_t * nd; int fp; } fch[256];
static int fch_n = 0;
void fc_head_register(const IR_t * nd, int fp) { if (!nd || fp < 0 || fch_n >= 256) return; fch[fch_n].nd = nd; fch[fch_n].fp = fp; fch_n++; }
int fc_head_fp(const IR_t * nd) {
    for (int i = 0; i < fch_n; i++) if (fch[i].nd == nd) return fch[i].fp;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* fc_leaf_* -- R12-ERAD s65 (ZC_FRAME_RSP flat-displacement registrar).  Under rsp-as-frame the flat frame sits ABOVE the pushed FORTH cells, so every non-window FR/FRQ inside a granted match window
 * must add the box's static depth D = 32 (HEAD's self-cell) + prefix (granted cells suspended before this box on the LINEAR spine, S10c: every passed box is gamma-suspended) + own cell.  LOWER fills
 * this in the SAME fc_head walk that computes fp_stmt (allocation order = flow order for linear spines -- SEQ lowers elements left-to-right); the v1 fence declines ALTERNATE statements wholesale so
 * the prefix is exact by the same argument as fp.  Consumed by FR/FRQ via g_emit.op_flat_disp (dispatch-delivered, default 0); R12/RBP builds never read it.  Side table keyed by node ptr (fch style). */
static struct { const IR_t * nd; int d; } fcl[1024];
static int fcl_n = 0;
void fc_leaf_register(const IR_t * nd, int d) { if (!nd || d < 0 || fcl_n >= 1024) return; fcl[fcl_n].nd = nd; fcl[fcl_n].d = d; fcl_n++; }
int fc_leaf_disp(const IR_t * nd) {
    for (int i = 0; i < fcl_n; i++) if (fcl[i].nd == nd) return fcl[i].d;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zls_off(const IR_t * nd) { const zls_entry_t * e = zx_find(nd); if (!e) return -1; return e->off + (zls_locals_shifted(nd->op) ? 16 : 0); }
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
    static const char * alloc_names[]    = { "BUMP_INFINITE", "BUMP_LIFO", "MALLOC", "GC" };
    static const char * col_names[]      = { "MALLOC", "ARENA", "GC" };
    static const char * selfload_names[] = { "OFF", "PLANE_CELL", "ASSERT", "STATIC" };
    static const char * init_names[]     = { "ZERO", "NONE", "CLONE" };
    fprintf(fp, "; ZETA LOCAL STORAGE (ZLS layout, ZB-2) — choices: ALLOC=%s COLLECTION=%s SELFLOAD=%s INIT=%s POISON=%s TELEM=%s OVF=%s ARENA_MB=%d PROMOTE=%s\n",
        alloc_names[ZC_ALLOC], col_names[ZC_COLLECTION], selfload_names[ZC_SELFLOAD], init_names[ZC_INIT],
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
