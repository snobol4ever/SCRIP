/*====================================================================================================================================================================================================*/
/* lower.c — UNIFIED AST->IR LOWERER (the new tree root; formerly lower2.c).
 *
 * THIS IS THE LOWERER. The prior 3183-line lower.c (the three-dispatch tangle: lower_expr_threaded /
 * lower_pl_goal / build_node, each its own switch) was DELETED 2026-05-31 — its content is preserved in
 * git history (blob d2d8c8e1; `git show d2d8c8e1`). One root, grown box-by-box, proven via
 * scripts/prove_lower2.sh. The full `make scrip` link stays RED until this tree regrows the program/proc
 * walkers + the remaining role arms (the rest of the codebase still calls the old API in lower.h).
 *
 * THE MODEL (Proebsting, "Simple Translation of Goal-Directed Evaluation"; jcon tran/irgen.icn ir_a_*).
 * Lowering IS an attribute grammar over the AST with FOUR ports per node, named by the four Byrd ports:
 *     α  start    — synthesized (the callee produces it; where control enters this node fresh)
 *     β  resume   — synthesized (the callee produces it; where control re-enters to make the next value)
 *     γ  succeed  — inherited   (the caller supplies it; where to go after producing a value)
 *     ω  fail     — inherited   (the caller supplies it; where to go when no more values)
 * So inherited attrs come IN as two pointers (γ_in, ω_in); synthesized attrs go OUT as two
 * pointers-to-pointers (α_out, β_out). That is THE canonical lowering signature — every box has it:
 *
 *     IR_t * lower_X(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in,
 *                    IR_t **α_out, IR_t **β_out);
 *
 * SCRIP's IR_t carries α/β/γ/ω as POINTER PORTS (not jcon's ir_Goto chunk-lists), so "goto L" in the
 * paper is a pointer assignment here: `child->γ = next` means child.succeed -> next.start.
 *
 * TWO TEMPLATE CLASSES (the "linear-forward-fail" vs "forward-backward-backtrack-abort" distinction;
 * jcon spells it `bounded`):
 *   (1) BOUNDED LEAF — a single-value box. α produces once -> γ; β -> ω (resume immediately fails).
 *       Paper §4.1 literal N. jcon emits the resume chunk only `/bounded` (when NOT bounded). When the
 *       enclosing context is bounded (only one value is ever wanted) the resume port collapses to ω.
 *   (2) RESUMABLE GENERATOR — produces a sequence. α starts; γ per value; β re-enters for the next;
 *       ω on exhaustion. May also abort. Paper §4.4 `to`, §4.3 relational backtracking.
 *
 * WIRING IS LOCAL. Each rule wires 4-12 ports among 2-3 boxes (the node + its children), exactly the
 * port equations in the paper/irgen. Children are lowered with the caller threading their γ/ω, and the
 * forward references (e.g. E1.succeed -> E2.start) are patched after both children exist.
 *
 * ROLE. A tree_e kind has a shared STRUCTURE but its ROLE-CONTEXT picks the rule: VALUE (general expr /
 * statement — Icon/Snocone/Rebus/Raku/SNOBOL value position), PATTERN (SNOBOL pattern element, entered at
 * `subj ? pat`), GOAL (Prolog goal, entered in a clause body). Role is carried in the cursor; the master
 * dispatch branches role -> per-role switch over tree_e.
 */
/*====================================================================================================================================================================================================*/
#define BB_DEFINE_NAMES
#include "lower.h"
#include "../ast/ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <gc/gc.h>
/* GZ-6 mode-2/mode-3 SEAM. The deterministic-builtin CALL's resume port is wired to its argument's resume
   (aβ) so the mode-2 port-walker (bb_exec.c) re-pumps a generator argument — jcon ir_a_Call's
   call.resume -> last-arg.resume. The mode-3 ring->tree adapter (driver/scrip.c icn_ring_to_tree) does not
   yet model that re-pump cycle (Lon's Path-1/Path-2 fork), so for mode-3 the CALL stays deterministic
   (resume -> ω_in), byte-identical to the pre-GZ6 graph. The driver sets this to 1 only for the in-process
   Icon mode-2 (--interp) path before lowering. Default 0 keeps every other path on the old wiring. */
int g_icn_postfix_resume = 0;
/*====================================================================================================================================================================================================*/
/* CURSOR — threaded through the descent. `role` selects the rule family; `bbg` is the graph populated;
 * `bounded` is the inherited single-value flag (jcon's `bounded`): when set, generator boxes may collapse
 * their resume port to ω. `lang` is the source language for the rare language-specific arm.              */
/*====================================================================================================================================================================================================*/
typedef enum { ROLE_VALUE = 0, ROLE_PATTERN = 1, ROLE_GOAL = 2 } lower_role_e;
/* PL_VARS — per-clause frame-size tracker. The Prolog frontend (prolog_lower.c tr_assign_slots /
 * lower_clause) already assigns every clause variable a dense slot index, written into the TT_VAR node's
 * v.ival (the SWI analyseVariables2 / gprolog clause-var numbering: pl-comp.c:874 `index = ci->arity +
 * nvars++`). The lowerer reads that slot directly for IR_LOGICVAR; this tracker records the max slot seen so
 * the driver allocates a per-activation env (g_resolve_env) of exactly count = max_slot + 1 cells.            */
typedef struct { int count; } pl_vars_t;
typedef struct {
    IR_graph_t * bbg;
    lower_role_e role;
    int          bounded;
    int          lang;
    IR_t       * loop_ω;          /* enclosing loop's exit (BREAK jumps here); NULL = no loop */
    IR_t       * loop_next;       /* enclosing loop's re-entry (NEXT jumps here); NULL = no loop */
    pl_vars_t  * pl_vars;         /* per-clause var->slot table (GOAL role); NULL outside a clause body */
} lcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower2(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * g_term(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * g_builtin(lcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*====================================================================================================================================================================================================*/
/* PORT PRIMITIVES — the only place α/β/γ/ω are assigned in bulk. `nalloc` allocates a node of a kind.
 * `set_succ_fail` fills the two inherited ports iff still unset (the "/x := y" default-only idiom from
 * irgen, so a child's already-threaded port is never clobbered). `ret` writes the two synthesized ports
 * back to the caller. These three encode the whole threading discipline.                                 */
/*====================================================================================================================================================================================================*/
static IR_t * nalloc(lcx_t cx, IR_e kind) { return IR_node_alloc(cx.bbg, kind); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in) {
    if (!n) return;
    if (!n->γ && γ_in) n->γ = γ_in;
    if (!n->ω && ω_in) n->ω = ω_in;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β) {
    if (α_out) *α_out = α;
    if (β_out) *β_out = β;
    return n;
}
/*====================================================================================================================================================================================================*/
/* SHARED COMBINATOR SCAFFOLDING — the two control shapes every role reuses. This is the "sharing" the
 * three concurrent language sessions ride: SNOBOL4 CAT (P1 P2), Icon conjunction (e1 & e2), and Prolog
 * conjunction (g1 , g2) are the SAME four-port sequence; SNOBOL4 ALT (P1 | P2), Icon alternation, and
 * Prolog disjunction (g1 ; g2) are the SAME four-port fail-chain. Each role's arm allocates its own node
 * KIND and hands it to these helpers; the wiring (the port equations from Proebsting / jcon ir_a_Alt) is
 * written ONCE here. Adding a role's call is free (additive); changing a helper's signature is lockstep.  */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* wire_seq — n-ary left-to-right sequence with backtracking, wrapped by a `kind` node. Children flatten
 * (any nested same-tree-kind SEQ/CAT is collapsed by the caller before calling, or handled n-ary here).
 *   child[i].γ -> child[i+1].α        (succeed -> next element's start)
 *   child[i+1].ω -> child[i].β         (next element fails -> retry the preceding element = backtrack)
 *   child[last].γ -> node              (whole sequence produced a value -> the wrapper node, then γ_in)
 *   child[0].ω -> ω_in                 (first element exhausted -> the sequence fails)
 *   node.α = child[0].α ; node.β = child[last].β   (re-enter the last element to make the next value)    */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    for (int i = nkids - 1; i >= 0; i--) {
        if (!kids[i]) return NULL;
        IR_t * γi = (i + 1 < nkids) ? entry[i + 1] : node;   /* last element's success -> the wrapper node */
        IR_t * αi = NULL, * βi = NULL;
        IR_t * c = lower2(cx, kids[i], γi, ω_in /*provisional; preceding-element retry patched below*/, &αi, &βi);
        if (!c) return NULL;
        apply[i] = c; entry[i] = αi ? αi : c; resume[i] = βi;
    }
    for (int i = 1; i < nkids; i++) {                          /* child[i].fail -> nearest RESUMABLE predecessor's resume */
        IR_t * tgt = ω_in;                                     /* a bounded element has resume == ω_in (no 2nd solution): skip it and keep */
        for (int j = i - 1; j >= 0; j--) {                     /* walking back; stop at the first resumable element (resume[j] != ω_in, i.e. a */
            if (resume[j] && resume[j] != ω_in) { tgt = resume[j]; break; }  /* node-self redo). If none, the whole sequence fails to ω_in. */
        }
        apply[i]->ω = tgt;
    }
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, entry[0], resume[nkids - 1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* wire_alt — n-ary alternation, wrapped by a `kind` node (jcon ir_a_Alt). Each arm's success funnels to
 * the node; the fail-chain threads arm[i].ω -> arm[i+1].α (try the next alternative), the last arm's fail
 * -> ω_in. The node is its own resume; on resume the IR_ALT collector re-pumps the active arm (if it is a
 * generator) or fail-chains to the next arm (single-shot). The ordered arm VALUE-nodes live in the
 * operand_aux sidecar (PEERS rule, no fields added to IR_t): operand_aux[i] is arm[i]'s apply node, from
 * which the executor reads arm[i].value and follows arm[i].ω to the next alternative.                      */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    /* Lower arms RIGHT-TO-LEFT (mirrors wire_seq) so each arm's fail/exhaustion continuation IS the next
     * arm's entry — already lowered — threaded through that arm's own deepest-fail wiring (wire_seq's
     * child[0].ω / emit_leaf's leaf.ω). The last arm fails to ω_in. The previous left-to-right patch of
     * apply[j]->ω only reached SINGLE-element arms: a conjunction arm's first element kept ω_in (NULL), so a
     * generator-then-fail left arm (`(G, fail ; Else)`) terminated the whole graph instead of trying Else. */
    for (int j = nkids - 1; j >= 0; j--) {
        if (!kids[j]) return NULL;
        IR_t * ωj = (j + 1 < nkids) ? entry[j + 1] : ω_in;   /* arm[j] exhausted -> next arm's entry ; last -> ω_in */
        IR_t * αj = NULL, * βj = NULL;
        IR_t * arm = lower2(cx, kids[j], node /*arm.γ -> node*/, ωj, &αj, &βj);
        if (!arm) return NULL;
        if (!arm->γ) arm->γ = node;
        apply[j] = arm; entry[j] = αj ? αj : arm; resume[j] = βj;
    }
    bb_operand_aux_set(cx.bbg, node, apply, nkids);
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, entry[0], node /* node is its own resume; collector dispatches to active arm */);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* flatten_seq — collapse a right/left-nested chain of `kind` tree nodes into a flat kids[] array (concat
 * and conjunction are associative, so `a (b c)` and `(a b) c` flatten to the same 3-element sequence).
 * Returns the count; writes up to `cap` leaf pointers. Used by SNOBOL CAT and Prolog `,` so a deep parse
 * tree lowers as one flat sequence (the flat fail-chain is what lets a middle generator backtrack).        */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap) {
    int n = 0;
    if (!e) return 0;
    if (e->t == kind) {
        for (int i = 0; i < e->n; i++) { int got = flatten_seq(e->c[i], kind, out + n, cap - n); n += got; if (n >= cap) break; }
        return n;
    }
    if (n < cap) out[n++] = e;
    return n;
}
/*====================================================================================================================================================================================================*/
/* TREE-PATTERN MATCH-AND-COLLECT (the shared little library; tmatch_proto.c was the design exhibit).
 * A lowering arm is "if the AST node looks like SHAPE, bind its parts and wire them." `tm` tests a node's
 * SHALLOW shape (kind + arity) and CAPTURES the first nargs children into (const tree_t **) out-params;
 * `tm_g` adds an sval-tag guard (the FNC("write",…) / FNC(",",a,b) dispatch). Captures are the subtrees to
 * lower next (capture DEFERS — it binds, it does not lower). Nested shapes chain: a second tm on a capture.
 * This is the AST-side analog of SNOBOL `subj ? pat`; it is the match primitive every concurrent language
 * arm uses, so the match-and-collect reads identically across SNOBOL4 / Icon / Prolog arms.                */
/*====================================================================================================================================================================================================*/
static int tm(const tree_t * e, tree_e kind, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    if (tag && (!e->v.sval || strcmp(e->v.sval, tag) != 0)) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*====================================================================================================================================================================================================*/
/* TEMPLATE CLASS (1) — BOUNDED LEAF. Paper §4.1: a single-value box. The node is its own α and (when
 * resumable & unbounded) its own β; bounded or non-resumable kinds collapse β to ω (resume -> fail).
 * `kind_is_resumable` (shared, in lower.c) classifies which IR kinds can produce a second value.          */
/*====================================================================================================================================================================================================*/
int kind_is_resumable(IR_e t) {
    return t == IR_TO || t == IR_TO_BY || t == IR_UPTO || t == IR_ALT || t == IR_BINOP_GEN || t == IR_ITERATE || t == IR_LIMIT || t == IR_PROC_GEN ||
           t == IR_EVERY || t == IR_REPEAT || t == IR_SUSPEND || t == IR_SCAN || t == IR_LIST_BANG || t == IR_KEY_GEN || t == IR_FIND_GEN || t == IR_SEQ_GEN ||
           t == IR_GEN_SCAN || t == IR_CONJ ||
           /* SNOBOL4 PATTERN generators — bb->β=self (retry to backtrack/shrink/grow): */
           t == IR_PAT_LIT || t == IR_PAT_ARB || t == IR_PAT_REM || t == IR_PAT_SPAN || t == IR_PAT_ANY || t == IR_PAT_NOTANY ||
           t == IR_PAT_BREAK || t == IR_PAT_LEN || t == IR_PAT_TAB || t == IR_PAT_ARBNO || t == IR_PAT_DEFER ||
           t == IR_PAT_BAL ||
           t == IR_PAT_ASSIGN_COND || t == IR_PAT_ASSIGN_IMM || t == IR_PAT_ATP;
           /* SINGLE-SHOT pattern nodes (POS, RPOS, FENCE, ABORT) get β=ω_in via the bounded path. */
}
static IR_t * emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!n) return NULL;
    set_succ_fail(n, γ_in, ω_in);
    IR_t * resume = (!cx.bounded && kind_is_resumable(n->t)) ? n : ω_in;
    return ret(n, α_out, β_out, n, resume);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE LEAVES — scalar literals & references. Each is a bounded leaf (one value, resume -> fail),
 * matching ir_a_Intlit/Reallit/Stringlit/Csetlit. Carries the literal payload in sval/ival/dval.          */
/*====================================================================================================================================================================================================*/
static IR_t * v_literal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_ILIT:    n = nalloc(cx, IR_LIT_I);   if (n) n->ival = e->v.ival; break;
    case TT_FLIT:    n = nalloc(cx, IR_LIT_F);   if (n) n->dval = e->v.dval; break;
    case TT_QLIT:    n = nalloc(cx, IR_LIT_S);   if (n) n->sval = e->v.sval ? e->v.sval : ""; break;
    case TT_CSET:    n = nalloc(cx, IR_LIT_S);   if (n) n->sval = e->v.sval ? e->v.sval : ""; break;
    case TT_NUL: case TT_NULL: n = nalloc(cx, IR_LIT_NUL); break;
    case TT_VAR:     n = nalloc(cx, IR_VAR);     if (n) n->sval = e->v.sval; break;
    case TT_NAME:    n = nalloc(cx, IR_VAR);     if (n) n->sval = e->v.sval; break;
    case TT_KEYWORD: n = nalloc(cx, IR_KEYWORD); if (n) n->sval = e->v.sval; break;
    default: return NULL;
    }
    return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — UNARY (paper §4.2 uminus). Start/resume delegate to the subexpression; on each child value
 * compute & succeed, on child failure fail. Ports:
 *   un.α = E.α ; un.β = E.β ; E.γ = (un computes, then -> un.γ) ; E.ω = un.ω
 * We realize "E.γ = un computes then succeed" by making the unop node sit on E's success edge: E.γ = un,
 * and un.γ = γ_in. un is NOT itself resumable — it inherits E's resumability via E.β being un.β.          */
/*====================================================================================================================================================================================================*/
static IR_t * v_unop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * un = nalloc(cx, IR_UNOP);
    if (!un) return NULL;
    un->sval = e->v.sval;
    un->ival = (int64_t) e->t;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * child = lower2(cx, e->c[0], un /* E.γ -> un */, ω_in, &eα, &eβ);
    if (!child) return NULL;
    set_succ_fail(un, γ_in, ω_in);
    return ret(un, α_out, β_out, eα, eβ);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — BINARY ARITHMETIC / CONCAT (paper §4.3 plus) and RELATIONAL (paper §4.3 LessThan).
 *   bin.α = E1.α ; bin.β = E2.β
 *   E1.γ = E2.α        (each E1 value (re)starts E2)
 *   E1.ω = bin.ω       (E1 exhausted -> bin fails)
 *   E2.ω = E1.β        (E2 exhausted -> resume E1)
 *   E2.γ = bin         (E2 value -> bin computes result -> bin.γ ; a relop additionally loops to E2.β
 *                       when the comparison is unsatisfied — that extra edge lives in the exec/emit of
 *                       IR_BINOP for a relational op, set via the relational flag below)
 * The forward reference E1.γ -> E2.α is patched after E2 is lowered.                                       */
/*====================================================================================================================================================================================================*/
static int tt_is_relational(tree_e t) {
    return t==TT_LT||t==TT_LE||t==TT_GT||t==TT_GE||t==TT_EQ||t==TT_NE
         ||t==TT_LLT||t==TT_LLE||t==TT_LGT||t==TT_LGE||t==TT_LEQ||t==TT_LNE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* tt_to_binop — map an arithmetic/relational/concat tree kind to its BinopKind (what the IR_BINOP exec arm
 * passes to binop_apply). The IR carries the BinopKind in ival, NOT the raw tree_e — they are different
 * enumerations (TT_ADD=13 vs BINOP_ADD=0), so storing the tree kind made binop_apply compute the wrong op. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int tt_to_binop(tree_e t) {
    switch (t) {
    case TT_ADD: return BINOP_ADD; case TT_SUB: return BINOP_SUB; case TT_MUL: return BINOP_MUL;
    case TT_DIV: return BINOP_DIV; case TT_MOD: return BINOP_MOD; case TT_POW: return BINOP_POW;
    case TT_LT:  return BINOP_LT;  case TT_LE:  return BINOP_LE;  case TT_GT:  return BINOP_GT;
    case TT_GE:  return BINOP_GE;  case TT_EQ:  return BINOP_EQ;  case TT_NE:  return BINOP_NE;
    case TT_CAT: case TT_LCONCAT: return BINOP_CONCAT;
    case TT_LLT: return BINOP_SLT; case TT_LLE: return BINOP_SLE; case TT_LGT: return BINOP_SGT;
    case TT_LGE: return BINOP_SGE; case TT_LEQ: return BINOP_SEQ; case TT_LNE: return BINOP_SNE;
    default:     return BINOP_ADD;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP);
    if (!bin) return NULL;
    bin->sval = e->v.sval;
    bin->ival = (int64_t) tt_to_binop(e->t);        /* BinopKind, NOT the raw tree_e */
    bin->dval = tt_is_relational(e->t) ? 1.0 : 0.0;
    IR_t * e1α=NULL, * e1β=NULL, * e2α=NULL, * e2β=NULL;
    IR_t * c1 = lower2(cx, e->c[0], NULL /*E1.γ patched below*/, ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = lower2(cx, e->c[1], bin /*E2.γ -> bin*/, e1β /*E2.ω -> E1.β*/, &e2α, &e2β);
    if (!c2) return NULL;
    if (!c1->γ) c1->γ = e2α;            /* patch E1.succeed -> E2.start */
    IR_t * binops[2] = { c1, c2 };
    bb_operand_aux_set(cx.bbg, bin, binops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return ret(bin, α_out, β_out, e1α, e2β);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — `to` / `to by` GENERATOR (paper §4.4; jcon ir_a_ToBy). The IR_TO node holds the loop
 * counter; α starts E1(from), the from/to children thread through it, and the node's β re-pumps the
 * counter. Mirrors the proven lower_new_ToBy_ag wiring:
 *   from.γ = to ; from.ω = ω_in ; to.γ = node ; to.ω = ω_in ; node.β = node (resumable).                  */
/*====================================================================================================================================================================================================*/
static IR_t * v_to(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = nalloc(cx, (e->t == TT_TO_BY) ? IR_TO_BY : IR_TO);
    if (!node) return NULL;
    node->sval = "ag";
    IR_t * fα=NULL,*fβ=NULL,*tα=NULL,*tβ=NULL;
    /* from-child: fail propagates outward (from.failure -> to.failure == ω_in); succeed patched below. */
    IR_t * lo = lower2(cx, e->c[0], NULL /*from.γ patched*/, ω_in, &fα, &fβ);
    if (!lo) return NULL;
    /* to-child: succeed -> node (compute & emit); FAIL -> from.RESUME (ir_a_ToBy: to.failure->from.resume). */
    IR_t * hi = lower2(cx, e->c[1], node /*to.γ -> node*/, fβ /*to.ω -> from.resume*/, &tα, &tβ);
    if (!hi) return NULL;
    if (!lo->γ) lo->γ = tα;             /* from.succeed -> to.start */
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, fα, node /* node is its own resume */);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — `if E1 then E2 else E3` (paper §4.5; jcon ir_a_If). The ONLY foundation box that wires a
 * port at RUN TIME: if's resume must go to E2.resume or E3.resume depending on which branch E1 selected.
 * The IR_IF node carries that runtime gate; E1.γ selects E2, E1.ω selects E3, and the node's β dispatches
 * through the gate. E1 is evaluated bounded (exactly once — only its success/failure matters).            */
/*====================================================================================================================================================================================================*/
static IR_t * v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = nalloc(cx, IR_IF);
    if (!node) return NULL;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * c1α=NULL,*c1β=NULL, *c2α=NULL,*c2β=NULL, *c3α=NULL,*c3β=NULL;
    IR_t * thenα = NULL, * elseα = NULL;
    IR_t * c2 = lower2(cx, e->c[1], γ_in, ω_in, &c2α, &c2β);   /* E2.γ/ω = if.γ/ω */
    if (!c2) return NULL;
    thenα = c2α;
    if (e->n >= 3 && e->c[2]) {
        IR_t * c3 = lower2(cx, e->c[2], γ_in, ω_in, &c3α, &c3β); /* E3.γ/ω = if.γ/ω */
        if (!c3) return NULL;
        elseα = c3α;
    } else {
        elseα = ω_in;                                            /* no else: E1-fail -> if.fail */
    }
    IR_t * c1 = lower2(cb, e->c[0], thenα /*E1.succ -> then.start*/, elseα /*E1.fail -> else.start*/, &c1α, &c1β);
    if (!c1) return NULL;
    node->α = c1α;
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, c1α, node /* node.β dispatches via runtime gate */);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static lcx_t bounded(lcx_t cx) { cx.bounded = 1; return cx; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static lcx_t with_loop(lcx_t cx, IR_t * lω, IR_t * lnext) { cx.loop_ω = lω; cx.loop_next = lnext; return cx; }
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — n-ary SEQUENCE. The per-language semantics live INSIDE this one case (FACT RULE):              */
/*   Icon/Rebus `E1 & E2` (jcon ir_conjunction): IR_CONJ forwards the LAST operand's value (`e1 & e2` -> e2).   */
/*   SNOBOL4 whitespace CONCATENATION `E1 E2 ...` (SPITBOL Manual ch.3): the value is the operands' strings      */
/*     appended left-to-right. Lowered as a LEFT-ASSOCIATIVE BINARY IR_SEQ chain — `((E1 E2) E3) ...`. Each       */
/*     binary IR_SEQ node lowers its TWO operands into their OWN isolated IR_graph_t sub-graphs (each with its    */
/*     own IR_SUCCEED/IR_FAIL sentinels — the same isolation v_scan uses for its pattern and ARBNO uses for its   */
/*     body), and stores the two sub-graph pointers on `counter` (left) and `ival` (right). The IR_SEQ exec arm   */
/*     runs each sub-graph via bb_exec_once and concatenates the two results with binop_apply(BINOP_CONCAT). This  */
/*     is robust for operands of ANY internal node count — bb_exec_once drives the whole operand chain and        */
/*     returns its final value, so there is no AG-ring positional dependency (the positional peek that IR_BINOP   */
/*     uses mis-counts a multi-node operand's intermediate pushes). Coercion via binop_apply matches the SNOBOL4  */
/*     concatenation operator (which has no symbol — see ch.3).                                                   */
/*   SNOBOL4 value-context operands do not backtrack (that is pattern context), so the node is BOUNDED: computed  */
/*     once, resume(β-port) -> ω_in. Marker: dval=1.0 (Icon/Rebus IR_SEQ never sets it). The node is its own α    */
/*     (the driver visits it; it drives its operand sub-graphs on demand).                                        */
/*====================================================================================================================================================================================================*/
static IR_graph_t * lower_value_subgraph(lcx_t cx, const tree_t * e) {
    IR_graph_t * blk = IR_alloc(256, IR_LANG_SNO);
    if (!blk) return NULL;
    IR_t * vfail = IR_node_alloc(blk, IR_FAIL);
    lcx_t vcx = cx; vcx.bbg = blk;
    IR_t * eα = NULL, * eβ = NULL;
    /* γ = NULL: the operand's value-producing node is TERMINAL. bb_exec_once returns the value of the node it
       halts on (next==NULL), so leaving γ unset makes that node the value node — exactly what we want (an
       IR_SUCCEED terminator would instead overwrite the value with NULVCL). ω -> IR_FAIL (operand failed). */
    IR_t * en = lower2(vcx, e, NULL, vfail, &eα, &eβ);
    if (!en) { IR_free(blk); return NULL; }
    (void) eβ;
    blk->entry = eα ? eα : en;
    return blk;
}
static IR_t * v_seq_concat_pair(lcx_t cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * node = nalloc(cx, IR_SEQ);
    if (!node) return NULL;
    node->dval = 1.0;       /* SNO-concat marker for the IR_SEQ exec arm */
    IR_graph_t * lblk = lower_value_subgraph(cx, lhs);
    if (!lblk) return NULL;
    IR_graph_t * rblk = lower_value_subgraph(cx, rhs);
    if (!rblk) { IR_free(lblk); return NULL; }
    node->counter = (int64_t)(intptr_t) lblk;     /* left operand sub-graph (preserved across bb_reset) */
    node->ival    = (int64_t)(intptr_t) rblk;     /* right operand sub-graph */
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, node /* node is the chain entry */, ω_in /* bounded: resume -> fail */);
}
static IR_t * v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * kids[64];
    int nk = flatten_seq(e, e->t, kids, 64);     /* associative: flatten nested same-kind SEQ into one operand list */
    if (nk < 1) return NULL;
    if (cx.lang != IR_LANG_SNO) return wire_seq(cx, IR_CONJ, kids, nk, γ_in, ω_in, α_out, β_out);
    if (nk == 1) return lower2(cx, kids[0], γ_in, ω_in, α_out, β_out);   /* a lone operand: no concat node needed */
    /* SNOBOL4: build the left-associative binary IR_SEQ chain over the flattened operands by synthesizing nested
       TT_SEQ pairs (kids[0..i] folded left), then lowering the top pair once via v_seq_concat_pair. Synthesizing
       nested TT_SEQ keeps lowering uniform (each pair is two real subtrees) and matches the parser's own
       left-nesting; the whole chain's α = the top IR_SEQ node (which drives its operands on demand). */
    tree_t * left = (tree_t *) kids[0];
    for (int i = 1; i < nk; i++) {
        tree_t * pair = ast_node_new(TT_SEQ);
        if (!pair) return NULL;
        ast_push(pair, left);
        ast_push(pair, (tree_t *) kids[i]);
        left = pair;
    }
    return v_seq_concat_pair(cx, left->c[0], left->c[1], γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — ALTERNATION `E1 | E2 | ... | Ek` (jcon ir_a_Alt; the SIBLING-backtrack box, runtime-gated
 * like `if`). Each arm's success flows to the alt node (which records the active arm's resume in its gate
 * then -> alt.γ); arms are fail-chained (Ei.ω -> E(i+1).α; last -> alt.ω); alt.β re-dispatches via the gate
 * to the active arm's resume. The ordered arm resume ports live in operand_aux for the executor's gate.    */
/*====================================================================================================================================================================================================*/
static IR_t * v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1) return NULL;
    return wire_alt(cx, IR_ALT, (const tree_t * const *) e->c, e->n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — `every E1 [do E2]` (jcon ir_a_Every). Drives E1 as a generator, runs the (bounded) body
 * once per E1 value, and on body success OR failure resumes E1. every yields no value and fails when E1 is
 * exhausted: every.α = E1.α ; E1.γ = body.α ; E1.ω = every.ω ; body.γ = body.ω = E1.β. No body: E1.γ=E1.β.  */
/*====================================================================================================================================================================================================*/
static IR_t * v_every(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * ev = nalloc(cx, IR_EVERY);
    if (!ev) return NULL;
    IR_t * g1α=NULL,*g1β=NULL;
    IR_t * gen = lower2(cx, e->c[0], NULL /*E1.γ patched*/, ev /*E1.ω -> every.fail*/, &g1α, &g1β);
    if (!gen) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower2(bounded(cx), e->c[1], g1β /*body.γ -> E1.resume*/, g1β /*body.ω -> E1.resume*/, &b2α, &b2β);
        if (!body) return NULL;
        if (!gen->γ) gen->γ = b2α;       /* E1.succeed -> body.start */
    } else {
        if (!gen->γ) gen->γ = g1β;       /* no body: E1.succeed -> E1.resume (drain) */
    }
    ev->α = g1α;
    set_succ_fail(ev, γ_in, ω_in);
    return ret(ev, α_out, β_out, g1α, ω_in /* every never generates: resume -> fail */);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — `while E1 [do E2]` (jcon ir_a_While). Condition + body both bounded; each iteration
 * re-evaluates the condition FRESH (not resume): while.α = E1.α ; E1.γ = body.α ; E1.ω = while.ω ;
 * body.γ = body.ω = E1.α. while yields no value; fails when the condition fails.                          */
/*====================================================================================================================================================================================================*/
static IR_t * v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * wh = nalloc(cx, IR_WHILE);
    if (!wh) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = lower2(bounded(cx), e->c[0], NULL /*E1.γ patched*/, wh /*E1.ω -> while.fail*/, &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower2(bounded(cx), e->c[1], c1α /*body.γ -> cond.start*/, c1α /*body.ω -> cond.start*/, &b2α, &b2β);
        if (!body) return NULL;
        if (!cond->γ) cond->γ = b2α;     /* E1.succeed -> body.start */
    } else {
        if (!cond->γ) cond->γ = c1α;     /* no body: loop re-evaluating the condition */
    }
    wh->α = c1α;
    set_succ_fail(wh, γ_in, ω_in);
    return ret(wh, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — `until E1 [do E2]` (jcon ir_a_Until). Mirror of while with the condition sense flipped:
 *   until.α = E1.α ; E1.γ = until.ω (cond true -> until fails) ; E1.ω = body.α ; body.γ = body.ω = E1.α.   */
/*====================================================================================================================================================================================================*/
static IR_t * v_until(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * un = nalloc(cx, IR_UNTIL);
    if (!un) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = lower2(bounded(cx), e->c[0], ω_in /*E1.succeed -> until.fail*/, un /*E1.fail -> until node -> body/loop*/, &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower2(bounded(cx), e->c[1], c1α /*body.γ -> cond.start*/, c1α /*body.ω -> cond.start*/, &b2α, &b2β);
        if (!body) return NULL;
        un->α = b2α;                     /* until node forwards E1-fail to body.start */
    } else {
        un->α = c1α;                     /* no body: forward to cond restart */
    }
    set_succ_fail(un, γ_in, ω_in);
    return ret(un, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — `repeat E` (jcon ir_a_Repeat). Unconditional infinite loop: repeat.α = E.α ;
 * E.γ = E.ω = E.α (every outcome restarts E). Never fails on its own.                                      */
/*====================================================================================================================================================================================================*/
static IR_t * v_repeat(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * rp = nalloc(cx, IR_REPEAT);
    if (!rp) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * body = lower2(bounded(cx), e->c[0], rp /*E.succeed -> repeat (restart)*/, rp /*E.fail -> repeat (restart)*/, &eα, &eβ);
    if (!body) return NULL;
    rp->α = eα;                          /* repeat node re-enters E.start */
    set_succ_fail(rp, γ_in, ω_in);
    return ret(rp, α_out, β_out, eα, ω_in);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — `not E` (jcon ir_a_Not). Succeeds (yielding &null) exactly when E fails, else fails:
 *   not.α = E.α ; E.γ = not.ω (E succeeds -> not fails) ; E.ω = not (E fails -> not produces null, succeeds);
 *   not.γ = γ_in ; not.β = not.ω (at most one value). E lowered bounded.                                    */
/*====================================================================================================================================================================================================*/
static IR_t * v_not(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * nt = nalloc(cx, IR_NOT);
    if (!nt) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * ce = lower2(bounded(cx), e->c[0], ω_in /*E.succeed -> not.fail*/, nt /*E.fail -> not (succeed null)*/, &eα, &eβ);
    if (!ce) return NULL;
    bb_operand_aux_set(cx.bbg, nt, &ce, 1);  /* operand ref via sidecar (PEERS rule) — NOT the β port */
    set_succ_fail(nt, γ_in, ω_in);
    return ret(nt, α_out, β_out, eα, ω_in /* resume -> fail */);
}
/*====================================================================================================================================================================================================*/
/* VALUE ROLE — ASSIGN (the SHARED-TABLE demo arm; SNOBOL4 `OUTPUT = expr`, Icon/Rebus `v := expr`).
 * Shape via tm: ASSIGN(VAR, rhs). Four-port: evaluate rhs (threaded), and on each rhs value STORE to the
 * target, then succeed. Bounded for the plain-VAR SNOBOL4/Rebus case (resume -> fail); Icon's resumable
 * `:=` is a future cx.lang arm under this same case (FACT RULE: language variation lives INSIDE the case).
 * The target var name is carried in as->sval; EXEC recognizes a reserved name (SNOBOL4 `OUTPUT`) and prints
 * — that recognition is an EXEC concern, NOT a lowering fork. Complex lhs (FIELD/IDX) = a later arm.
 *   rhs.γ -> as ; rhs.ω -> ω_in ; as.γ -> γ_in ; as.ω -> ω_in ; as.α = rhs.α ; as.β = ω_in.                */
/*====================================================================================================================================================================================================*/
static IR_t * v_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * lhs_t = NULL, * rhs_t = NULL;
    if (!tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
    if (!lhs_t || lhs_t->t != TT_VAR || !rhs_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    as->sval = lhs_t->v.sval ? lhs_t->v.sval : "";
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower2(cx, rhs_t, as /*rhs.γ -> as (store & succeed)*/, ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    set_succ_fail(as, γ_in, ω_in);
    return ret(as, α_out, β_out, rα, ω_in /* bounded: resume -> fail */);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* v_scan — SNOBOL4 pattern-match statement SUBJECT ? PATTERN (whitespace and ? forms both parse to TT_SCAN;
 * c[0]=subject value-expr, c[1]=pattern; an optional c[2]=replacement makes it SUBJECT ? PATTERN = REPLACEMENT,
 * synthesized by the program walker from a STMT carrying :eq+:repl over a TT_SCAN subject). SPITBOL Manual ch.6:
 * the match succeeds if PATTERN is found in SUBJECT (unanchored unless &ANCHOR nonzero), else fails; the value
 * of the construct is the matched substring. For the plain form the subject is lowered VALUE-role with its
 * gamma -> the IR_SCAN node, so the subject string is on the AG ring when IR_SCAN executes. For the replacement
 * form the subject MUST be a variable ("a variable must be the subject of replacement"); its name rides on sval
 * (fetched by name at exec — robust regardless of replacement complexity), ival=1 marks replacement mode, and
 * the replacement value-expr is threaded (replacement.gamma -> IR_SCAN) so the replacement value is on the AG
 * ring. The pattern is always lowered PATTERN-role into ITS OWN sub-graph (own IR_SUCCEED/IR_FAIL sentinels),
 * the same isolation ARBNO uses; the sub-graph pointer rides on counter (preserved across bb_reset like ARBNO)
 * and the IR_SCAN exec arm drives it with anchored start-iteration + deferred-capture commit + the splice.
 * IR_SCAN is bounded (one match per statement attempt): resume -> ω_in. Icon scanning (s ? expr) is L2-F. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * v_scan(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (cx.lang == IR_LANG_ICN) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const tree_t * subj_t = NULL, * pat_t = NULL;
    if (!tm(e, TT_SCAN, 2, &subj_t, &pat_t) || !subj_t || !pat_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const tree_t * repl_t = (e->n >= 3) ? e->c[2] : NULL;
    if (repl_t && subj_t->t != TT_VAR) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * sc = nalloc(cx, IR_SCAN);
    if (!sc) return NULL;
    IR_graph_t * pat_blk = IR_alloc(256, IR_LANG_SNO);
    if (!pat_blk) return NULL;
    IR_t * psucc = IR_node_alloc(pat_blk, IR_SUCCEED);
    IR_t * pfail = IR_node_alloc(pat_blk, IR_FAIL);
    IR_t * pα = NULL, * pβ = NULL;
    lcx_t pcx = { pat_blk, ROLE_PATTERN, 0, IR_LANG_SNO, NULL, NULL };
    IR_t * pat_entry = lower2(pcx, pat_t, psucc, pfail, &pα, &pβ);
    if (!pat_entry) { IR_free(pat_blk); return NULL; }
    (void) pβ;
    pat_blk->entry = pα ? pα : pat_entry;
    sc->counter = (int64_t)(intptr_t)pat_blk;
    if (repl_t) {
        sc->sval = subj_t->v.sval ? subj_t->v.sval : "";
        sc->ival = 1;
        IR_t * rα = NULL, * rβ = NULL;
        IR_t * repln = lower2(cx, repl_t, sc, ω_in, &rα, &rβ);
        if (!repln) { IR_free(pat_blk); return NULL; }
        (void) rβ;
        set_succ_fail(sc, γ_in, ω_in);
        return ret(sc, α_out, β_out, rα ? rα : sc, ω_in);
    }
    IR_t * sα = NULL, * sβ = NULL;
    IR_t * subj = lower2(cx, subj_t, sc, ω_in, &sα, &sβ);
    if (!subj) { IR_free(pat_blk); return NULL; }
    (void) sβ;
    set_succ_fail(sc, γ_in, ω_in);
    return ret(sc, α_out, β_out, sα ? sα : sc, ω_in);
}
/*====================================================================================================================================================================================================*/
/* VALUE ROLE — master per-kind switch. Foundation boxes are wired; the rest route to lower_unhandled
 * (loud, never silent). Each TODO is one box to add onto the foundation, in the canonical signature.      */
/*====================================================================================================================================================================================================*/
static IR_t * lower_value(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    switch (e->t) {
    /* foundation: literals & refs */
    case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_CSET:
    case TT_NUL:  case TT_NULL: case TT_VAR:  case TT_NAME: case TT_KEYWORD:
        return v_literal(cx, e, γ_in, ω_in, α_out, β_out);
    /* foundation: unary */
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL:
    case TT_RANDOM: case TT_MATCH_UNARY: case TT_CSET_COMPL: case TT_ITERATE: case TT_INTERROGATE:
        return v_unop(cx, e, γ_in, ω_in, α_out, β_out);
    /* foundation: binary arithmetic / relational / concat */
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
        return v_binop(cx, e, γ_in, ω_in, α_out, β_out);
    /* foundation: to-generator */
    case TT_TO: case TT_TO_BY:
        return v_to(cx, e, γ_in, ω_in, α_out, β_out);
    /* foundation: conditional */
    case TT_IF:
        return v_if(cx, e, γ_in, ω_in, α_out, β_out);
    /* L2-A: combinators */
    case TT_SEQ: case TT_SEQ_EXPR:
        return v_conj(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ALTERNATE: case TT_ALT:
        return v_alt(cx, e, γ_in, ω_in, α_out, β_out);
    /* L2-B: loops (core) */
    case TT_EVERY:
        return v_every(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_WHILE:
        return v_while(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_UNTIL:
        return v_until(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_REPEAT:
        return v_repeat(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_NOT:
        return v_not(cx, e, γ_in, ω_in, α_out, β_out);
    /* SHARED TABLE — first cross-language value arm (SNOBOL4 OUTPUT=, Icon/Rebus :=) */
    case TT_ASSIGN:
        return v_assign(cx, e, γ_in, ω_in, α_out, β_out);

    /* --- extension surface (each = one box onto the foundation, canonical signature) --- */
    case TT_LIMIT:      /* jcon ir_a_Limitation */
    case TT_CASE:       /* jcon ir_a_Case */
    case TT_RETURN:     /* jcon ir_a_Return */
    case TT_NRETURN:
    case TT_SUSPEND:    /* jcon ir_a_Suspend */
    case TT_PROC_FAIL:  /* jcon ir_a_Fail */
    case TT_LOOP_BREAK: /* jcon ir_a_Break */
    case TT_LOOP_NEXT:  /* jcon ir_a_Next */
    case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP:
    /* SHARED — Icon `write(x)`/`writes(x)` deterministic output builtin (1-arg), via wire_det_builtin1 ->
       IR_CALL. NOTE the per-language TT_FNC shape (FACT RULE: variation lives inside the case): Icon carries
       the callee as child c[0] (a TT_VAR) with args c[1..] and routes write through the SHARED IR_CALL (Icon
       `write` adds a newline at EXEC via try_call_builtin_by_name; `writes` does not). Prolog carries the
       functor in sval and emits a Prolog-OWNED IR_BUILTIN (g_builtin) instead — pl_write, NO auto-newline —
       so the two languages' write semantics never collide. Multi-arg write + general call = a later L2-E arm. */
    case TT_FNC: {
        /* SNOBOL4 function/builtin CALL (FACT RULE: the per-language TT_FNC SHAPE lives inside this one case).
           SNOBOL4 — like Prolog — carries the callee NAME in e->v.sval with args as c[0..n-1] (Icon instead
           carries the callee as child c[0] and routes write through the Icon arm below, so the two never
           overlap). Lowers to a four-port IR_CALL dispatched BY NAME at exec: a user DEFINE'd function (in the
           proc_table) runs through the SNOBOL4 global save/restore frame (IR_CALL exec arm, dval==2.0), and
           any other name falls to try_call_builtin_by_name (DEFINE, SIZE, REPLACE, ...). Each argument is
           lowered into its OWN isolated value sub-graph (the v_scan / IR_SEQ-concat idiom — robust for an
           operand of any internal node count, no AG-ring positional dependency); the sub-graph pointer array
           rides on `counter` (preserved across bb_reset, like IR_SCAN/ARBNO). SPITBOL Manual ch.4 & ch.8: a
           call evaluates its arguments, transfers to the function (dummy args take the actual values; locals
           and the function-named result variable are saved then nulled), and the value of the call is the
           function-named variable on RETURN, or failure on FRETURN. */
        if (cx.lang == IR_LANG_SNO) {
            IR_t * call = nalloc(cx, IR_CALL); if (!call) return NULL;
            call->sval = e->v.sval ? e->v.sval : "";
            call->ival = e->n;
            call->dval = 2.0;                                      /* SNOBOL4 subgraph-arg call marker */
            if (e->n > 0) {
                IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) e->n, sizeof(IR_graph_t *));
                if (!blks) return NULL;
                for (int i = 0; i < e->n; i++) {
                    blks[i] = lower_value_subgraph(cx, e->c[i]);
                    if (!blks[i]) { free(blks); return NULL; }
                }
                call->counter = (int64_t)(intptr_t) blks;          /* array of arg value sub-graphs */
            }
            set_succ_fail(call, γ_in, ω_in);
            return ret(call, α_out, β_out, call /* call node is the chain entry */, ω_in /* bounded */);
        }
        if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return wire_det_builtin1(cx, e->c[1], fn, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);   /* multi-arg write / general call = L2-E */
    }
    /* SNOBOL4 pattern-match statement SUBJECT ? PATTERN (LOWER2-EXEC). Icon scanning (TT_SMATCH / Icon ?) stays
       in the unhandled group below (L2-F). The per-language split lives inside v_scan (FACT RULE) via cx.lang. */
    case TT_SCAN:
        return v_scan(cx, e, γ_in, ω_in, α_out, β_out);

    case TT_FIELD:      /* jcon ir_a_Field */
    case TT_IDX:
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS:  /* jcon ir_a_Sectionop */
    case TT_INDIRECT: case TT_IDENTICAL:
    case TT_SMATCH:     /* subj ? pat — flips cx.role = ROLE_PATTERN */
    case TT_CSET_UNION: case TT_CSET_DIFF: case TT_CSET_INTER:
    case TT_MAKELIST: case TT_VLIST: case TT_RECORD: case TT_NEW: case TT_SORT:
    case TT_MAP: case TT_GREP: case TT_GATHER:
    case TT_HASH_GET: case TT_HASH_SET: case TT_HASH_DELETE: case TT_HASH_EXISTS:
    case TT_ARR_GET: case TT_ARR_SET:
    case TT_PRINT: case TT_PRINT_FH: case TT_SAY: case TT_SAY_FH:
    case TT_GLOBAL: case TT_LOCAL: case TT_STATIC_DECL: case TT_DECL: case TT_INITIAL: case TT_OPSYN:
    case TT_GOTO_U: case TT_GOTO_S: case TT_GOTO_F:
    case TT_TRY: case TT_DIE: case TT_UNLESS: case TT_DO_WHILE: case TT_FOR: case TT_FOR_RANGE:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================================================================================================*/
/* PATTERN ROLE — SNOBOL pattern elements. Every pattern node is four-port: γ = continue-match,
 * ω = fail-match, α = first attempt, β = backtrack-retry. Leaves use emit_leaf; the structural builders
 * (CAT chain, ALT fail-chain, FENCE, ARBNO, captures) wire children with the same canonical signature.
 * Foundation: the leaf primitives. Extension: the combinators + remaining primitives.                     */
/*====================================================================================================================================================================================================*/
/* cset_try_fold — charset-expression constant folder. STUB (NULL = not foldable) until the PATTERN role (L2-P) lands; the real folder plus its cset_fold_* helper tail return then. No hello path uses it. */
char * cset_try_fold(const tree_t * t) { (void) t; return NULL; }
static int pat_cset_arg(const tree_t * arg, const char ** sval_out, double * varflag_out) {
    if (!arg) return 0;
    if (arg->t == TT_QLIT) { *sval_out = arg->v.sval ? arg->v.sval : ""; *varflag_out = 0.0; return 1; }
    if (arg->t == TT_VAR)  { *sval_out = arg->v.sval ? arg->v.sval : ""; *varflag_out = 1.0; return 1; }
    { char * cs = cset_try_fold(arg); if (!cs) return 0; *sval_out = cs; *varflag_out = 0.0; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_pattern(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_QLIT: n = nalloc(cx, IR_PAT_LIT); if (n) n->sval = e->v.sval ? e->v.sval : ""; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    case TT_ARB:  return emit_leaf(cx, nalloc(cx, IR_PAT_ARB), γ_in, ω_in, α_out, β_out);
    case TT_REM:  return emit_leaf(cx, nalloc(cx, IR_PAT_REM), γ_in, ω_in, α_out, β_out);
    /* BAL — matches the shortest non-null parenthesis-balanced substring; grows on retry (generator,
       β=self), like ARB but constrained to balance. SPITBOL ch.18 "BAL matches any non-null string
       balanced w.r.t. parentheses; a string without parens is balanced; matches the shortest possible". */
    case TT_BAL:  return emit_leaf(cx, nalloc(cx, IR_PAT_BAL), γ_in, ω_in, α_out, β_out);
    case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX: {
        if (e->n < 1 || !e->c[0]) return NULL;
        const char * sv = NULL; double vf = 0.0;
        if (!pat_cset_arg(e->c[0], &sv, &vf)) return NULL;
        IR_e k = (e->t==TT_SPAN)?IR_PAT_SPAN : (e->t==TT_ANY)?IR_PAT_ANY
               : (e->t==TT_NOTANY)?IR_PAT_NOTANY : IR_PAT_BREAK;
        n = nalloc(cx, k); if (!n) return NULL;
        n->sval = sv;
        if (e->t==TT_SPAN) n->ival = (vf!=0.0)?1:0;
        else               n->dval = vf;
        if (e->t==TT_BREAKX) n->ival = 1; else if (e->t==TT_BREAK) n->ival = 0;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* CAT — pattern subsequent `P1 P2` (SPITBOL ch.6 "P2 is subsequent to P1"). SAME shape as Icon
       conjunction / Prolog `,`: P1.γ -> P2.α, P2.ω -> P1.β (backtrack to grow the preceding element). */
    case TT_SEQ: case TT_CAT: {
        const tree_t * kids[64];
        int nk = flatten_seq(e, e->t, kids, 64);
        if (nk < 1) return NULL;
        if (nk == 1) return lower2(cx, kids[0], γ_in, ω_in, α_out, β_out);
        return wire_seq(cx, IR_PAT_CAT, kids, nk, γ_in, ω_in, α_out, β_out);
    }
    /* ALT — pattern alternation `P1 | P2` (SPITBOL ch.6). SAME fail-chain as Icon alternation /
       Prolog `;`: arm.γ -> alt, arm[i].ω -> arm[i+1].α, last arm fail -> ω_in. */
    case TT_ALT:
        if (e->n < 1) return NULL;
        return wire_alt(cx, IR_PAT_ALT, (const tree_t * const *) e->c, e->n, γ_in, ω_in, α_out, β_out);
    /* LEN(n) — match exactly n chars. n is TT_ILIT (→ ival) or TT_VAR (→ sval+dval=1.0). Generator
       (retry undoes the advance): α=β=self. SPITBOL ch.6 "LEN(I) matches exactly I characters". */
    case TT_LEN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_LEN); if (!n) return NULL;
        if (e->c[0]->t == TT_VAR) { n->sval = e->c[0]->v.sval ? e->c[0]->v.sval : ""; n->dval = 1.0; }
        else { n->ival = e->c[0]->v.ival; n->dval = 0.0; }
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* POS(I) — cursor-position test from left (single-shot: never alters cursor, just checks it).
       RPOS(I) — cursor-position test from right (Σlen - I). Neither consumes chars; β=ω_in (bounded).
       SPITBOL ch.6 "POS(I) succeeds iff cursor == I; RPOS(I) succeeds iff cursor == N-I". */
    case TT_POS: case TT_RPOS: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_POS); if (!n) return NULL;
        int is_rpos = (e->t == TT_RPOS);
        if (e->c[0]->t == TT_VAR) {
            n->sval = e->c[0]->v.sval ? e->c[0]->v.sval : "";
            n->dval = is_rpos ? 1.0 : 2.0;   /* 1.0=RPOS-var, 2.0=POS-var (matches oracle dval check) */
        } else {
            n->ival = e->c[0]->v.ival;
            n->sval = is_rpos ? "r" : NULL;   /* "r" triggers from-end in oracle */
            n->dval = 0.0;
        }
        /* Single-shot: cursor check never retries — treat as bounded so β=ω_in. */
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    /* TAB(I) — match chars from cursor to absolute position I (RTAB: from right). Generator (retry
       undoes the TAB-advance): α=β=self. SPITBOL ch.6 "TAB(I) matches up to cursor I". */
    case TT_TAB: case TT_RTAB: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_TAB); if (!n) return NULL;
        int is_rtab = (e->t == TT_RTAB);
        if (e->c[0]->t == TT_VAR) {
            n->sval = e->c[0]->v.sval ? e->c[0]->v.sval : "";
            n->dval = is_rtab ? 1.0 : 2.0;
        } else {
            n->ival = e->c[0]->v.ival;
            n->sval = is_rtab ? "r" : NULL;
            n->dval = 0.0;
        }
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* FENCE — commits the match at this point; if backtracking reaches it, the whole match fails.
       FENCE(pat) variant: pat is lowered first, FENCE follows. Single-shot: β=ω_in (bounded).
       SPITBOL ch.9 "FENCE matches null; on backtrack, fails the entire match". */
    case TT_FENCE: {
        n = nalloc(cx, IR_PAT_FENCE); if (!n) return NULL;
        if (e->n > 0 && e->c[0]) {
            /* FENCE(inner) — lower inner pattern, FENCE is its successor */
            IR_t * fα = NULL, * fβ = NULL;
            lcx_t bx = cx; bx.bounded = 1;
            set_succ_fail(n, γ_in, ω_in);
            IR_t * inner = lower2(cx, e->c[0], n, ω_in, &fα, &fβ);
            if (!inner) return NULL;
            return ret(n, α_out, β_out, fα, ω_in /* FENCE is single-shot: β=ω_in */);
        }
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    /* ABORT — immediately fails the entire pattern match (no alternatives tried).
       SPITBOL ch.9 "ABORT causes immediate failure of the entire pattern match". */
    case TT_ABORT: {
        n = nalloc(cx, IR_PAT_ABORT); if (!n) return NULL;
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    /* FAIL — forces the pattern matcher to backtrack and seek alternatives.
       SPITBOL ch.9 "FAIL signals failure of this portion, causing backtrack". */
    case TT_FAIL: {
        n = nalloc(cx, IR_FAIL); if (!n) return NULL;
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    /* SUCCEED — always succeeds; if backtracking reaches it, reverses direction.
       SPITBOL ch.9 "SUCCEED matches null string, always succeeds". */
    case TT_SUCCEED: {
        n = nalloc(cx, IR_SUCCEED); if (!n) return NULL;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* ARBNO(pat) — zero or more occurrences of pat (shy: starts empty, grows on retry).
       Inner pat is lowered into its own sub-graph referenced via bb_arbno_state_t.
       SPITBOL ch.9 "ARBNO(PAT) behaves like ('' | PAT | PAT PAT | …)". */
    case TT_ARBNO: {
        if (e->n < 1 || !e->c[0]) return NULL;
        int inner_cap = 64;
        IR_graph_t * inner_blk = IR_alloc(inner_cap, IR_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * isucc = IR_node_alloc(inner_blk, IR_SUCCEED);
        IR_t * ifail = IR_node_alloc(inner_blk, IR_FAIL);
        IR_t * iα = NULL, * iβ = NULL;
        lcx_t icx = { inner_blk, ROLE_PATTERN, 0, 0 };
        IR_t * inner_entry = lower2(icx, e->c[0], isucc, ifail, &iα, &iβ);
        if (!inner_entry) { IR_free(inner_blk); return NULL; }
        inner_blk->entry = iα;
        n = nalloc(cx, IR_PAT_ARBNO); if (!n) { IR_free(inner_blk); return NULL; }
        int stack_cap = 64;
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC((size_t)stack_cap * sizeof(int));
        az->cap = stack_cap;
        az->saved_delta = 0;
        n->counter = (int64_t)(intptr_t)az;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* Conditional capture `pat . var` (deferred: commits once on full match success).
       TT_CAPT_COND_ASGN: c[0]=inner pattern, c[1]=TT_VAR (varname). Generator (retry re-enters inner). */
    case TT_CAPT_COND_ASGN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_ASSIGN_COND); if (!n) return NULL;
        n->sval = (e->n > 1 && e->c[1] && e->c[1]->v.sval) ? e->c[1]->v.sval : NULL;
        set_succ_fail(n, γ_in, ω_in);
        IR_t * iα = NULL, * iβ = NULL;
        IR_t * inner = lower2(cx, e->c[0], n, ω_in, &iα, &iβ);
        if (!inner) return NULL;
        n->α = iα;
        return ret(n, α_out, β_out, n, iβ ? iβ : ω_in);
    }
    /* Immediate capture `pat $ var` — same topology as COND but fires assignment immediately.
       TT_CAPT_IMMED_ASGN: c[0]=inner pattern, c[1]=TT_VAR (varname). */
    case TT_CAPT_IMMED_ASGN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_ASSIGN_IMM); if (!n) return NULL;
        n->sval = (e->n > 1 && e->c[1] && e->c[1]->v.sval) ? e->c[1]->v.sval : NULL;
        set_succ_fail(n, γ_in, ω_in);
        IR_t * iα = NULL, * iβ = NULL;
        IR_t * inner = lower2(cx, e->c[0], n, ω_in, &iα, &iβ);
        if (!inner) return NULL;
        n->α = iα;
        return ret(n, α_out, β_out, n, iβ ? iβ : ω_in);
    }
    /* Cursor capture `@var` — records current cursor position in var; zero-width.
       TT_CAPT_CURSOR: c[0]=TT_VAR (varname). Generator (retry re-records). */
    case TT_CAPT_CURSOR: {
        if (e->n < 1 || !e->c[0] || !e->c[0]->v.sval) return NULL;
        n = nalloc(cx, IR_PAT_ATP); if (!n) return NULL;
        n->sval = e->c[0]->v.sval;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* Deferred `*var` — var holds a pattern (DT_P) or string; resolved at match time.
       TT_DEFER: c[0]=TT_VAR (varname); ival=1 = apply indirect deref. Generator. */
    case TT_DEFER: {
        if (e->n < 1 || !e->c[0] || !e->c[0]->v.sval) return NULL;
        n = nalloc(cx, IR_PAT_DEFER); if (!n) return NULL;
        n->sval = e->c[0]->v.sval;
        n->ival = 1;    /* indirect deref flag (oracle bb_exec.c IR_PAT_DEFER state-0 ival check) */
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* TT_VAR in pattern context. Bare pattern-primitive keywords (ARB/REM/BAL/FAIL/SUCCEED/FENCE/ABORT) lex as
       plain identifiers — pat_prim_kind is consulted only for the parenthesized T_FUNCTION form (snobol4.y) — so
       a bare keyword arrives here as TT_VAR. SPITBOL predefines these as pattern values; lower each to its
       primitive leaf (case-sensitive per RULES). A non-keyword name is a real variable reference, resolved as a
       string-literal match at runtime via IR_PAT_DEFER ival=0. (Reassigning one of these names to a different
       pattern is the DEFER-runtime edge case, deferred to Track B.) */
    case TT_VAR: {
        if (!e->v.sval) return NULL;
        const char * nm = e->v.sval;
        IR_e pk = (IR_e) 0; int is_prim = 1; int bnd = 0;
        if      (!strcmp(nm, "ARB"))     pk = IR_PAT_ARB;
        else if (!strcmp(nm, "REM"))     pk = IR_PAT_REM;
        else if (!strcmp(nm, "BAL"))     pk = IR_PAT_BAL;
        else if (!strcmp(nm, "ABORT"))   { pk = IR_PAT_ABORT; bnd = 1; }
        else if (!strcmp(nm, "FAIL"))    { pk = IR_FAIL;      bnd = 1; }
        else if (!strcmp(nm, "SUCCEED")) pk = IR_SUCCEED;
        else if (!strcmp(nm, "FENCE"))   { pk = IR_PAT_FENCE; bnd = 1; }
        else is_prim = 0;
        if (is_prim) {
            n = nalloc(cx, pk); if (!n) return NULL;
            lcx_t bx = cx; if (bnd) bx.bounded = 1;
            return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
        }
        n = nalloc(cx, IR_PAT_DEFER); if (!n) return NULL;
        n->sval = nm;
        n->ival = 0;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    /* extension: FNC(SPAN/ANY/etc.) calls, BAL (future). */
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================================================================================================*/
/* SHARED — deterministic 1-arg builtin call (write/writeln/print). Used by BOTH the Prolog GOAL role and
 * the Icon VALUE role (Icon `write(x)` and Prolog `write(x)` are the same deterministic output builtin —
 * another sharing seam). The arg is lowered VALUE-role and threaded (arg.γ -> call) so it is produced
 * first, pushing its value onto the AG ring; `dval=1.0` (is_deep) tells the IR_CALL exec arm to read the
 * arg from the ring (ag_ring_peek) rather than the legacy bb->α arg-chain. Deterministic: succeed once,
 * resume -> fail. `try_call_builtin_by_name` performs the actual write.
 *   arg.γ -> call ; arg.ω -> ω_in ; call.γ -> γ_in ; call.ω -> ω_in ; α = arg.α ; β = ω_in.               */
/*====================================================================================================================================================================================================*/
static IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!arg_t) return NULL;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    call->sval = fn;
    call->ival = 1;
    call->dval = 1.0;                               /* is_deep: EXEC reads the arg from the AG ring */
    lcx_t av = cx; av.role = ROLE_VALUE;            /* the builtin's argument is a term/value */
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * a = lower2(av, arg_t, call /*arg.γ -> call*/, ω_in, &aα, &aβ);
    if (!a) return NULL;
    set_succ_fail(call, γ_in, ω_in);
    IR_t * call_resume = g_icn_postfix_resume ? aβ : ω_in;
    return ret(call, α_out, β_out, aα, call_resume);
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — `=/2` unification (Prolog). A deterministic-at-most-once goal: bind LHS≈RHS, succeed once,
 * then resume -> fail. Both sides are TERMS (lowered VALUE-role so they materialize as descriptors the
 * unifier consumes). The two operand refs go in the operand_aux sidecar (PEERS rule); EXEC performs the
 * actual unify + trail. Topology: lhs.γ -> rhs.α ; rhs.γ -> unify ; either operand exhausts -> ω_in.       */
/*====================================================================================================================================================================================================*/
static IR_t * g_unify(lcx_t cx, const tree_t * l_t, const tree_t * r_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!l_t || !r_t) return NULL;
    IR_t * uni = nalloc(cx, IR_UNIFY);
    if (!uni) return NULL;
    IR_t * lα = NULL, * lβ = NULL, * rα = NULL, * rβ = NULL;
    IR_t * l = g_term(cx, l_t, NULL, NULL, &lα, &lβ);
    if (!l) return NULL;
    IR_t * r = g_term(cx, r_t, NULL, NULL, &rα, &rβ);
    if (!r) return NULL;
    (void) lβ; (void) rβ;
    uni->α = lα;                                    /* lhs term-tree (read by resolve_node_to_term) */
    uni->β = rα;                                    /* rhs term-tree (read by resolve_node_to_term) */
    set_succ_fail(uni, γ_in, ω_in);
    return ret(uni, α_out, β_out, uni, ω_in /* unify is semidet: resume -> fail */);
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — Prolog arith expression in a VALUE position (rhs of `is/2`, both sides of `</2` etc.).
 * The Prolog tree-builder (lower_clause_from_tree) emits arithmetic ops as TT_FNC(op_name, ...)
 * rather than TT_ADD/TT_SUB/etc., since those kinds only appear from lower_clause (the PlClause path).
 * We emit IR_ARITH(sval=op, ival=arity, α=left, β=right) whose resolve_arith_eval traversal handles:
 * IR_ARITH (recursion), IR_LIT_I/F (constant), IR_LOGICVAR (slot lookup), and default (bb_exec_node).
 * For TT_ADD/TT_SUB/etc. (from lower_clause path) and TT_ILIT/TT_FLIT/TT_VAR we fall through to
 * lower2(VALUE) which already has the correct arms (v_binop, emit_leaf paths). Prolog-ONLY path —
 * FACT RULE clean (only called from g_compare and g_is; no peer language arm touched).               */
/*====================================================================================================================================================================================================*/
static IR_t * g_arith_expr(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * g_arith_expr(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) return NULL;
    lcx_t tv = cx; tv.role = ROLE_VALUE;
    if (e->t == TT_FNC) {
        const char * op = e->v.sval;
        if (!op) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        int ar = e->n;
        IR_t * nd = nalloc(cx, IR_ARITH); if (!nd) return NULL;
        nd->sval = op; nd->ival = ar;
        if (ar >= 1 && e->c[0]) {
            IR_t * aα = NULL, * aβ = NULL;
            IR_t * a = g_arith_expr(cx, e->c[0], NULL, ω_in, &aα, &aβ);
            if (!a) return NULL; (void) aβ;
            nd->α = aα;
        }
        if (ar >= 2 && e->c[1]) {
            IR_t * bα = NULL, * bβ = NULL;
            IR_t * b = g_arith_expr(cx, e->c[1], NULL, ω_in, &bα, &bβ);
            if (!b) return NULL; (void) bβ;
            nd->β = bα;
        }
        set_succ_fail(nd, γ_in, ω_in);
        return ret(nd, α_out, β_out, nd, ω_in);
    }
    /* A Prolog arithmetic leaf operand is a TERM, not an Icon/SNOBOL value: numbers (IR_LIT_I/IR_LIT_F),
     * a variable (IR_LOGICVAR — a frame SLOT, read by resolve_arith_eval from g_resolve_env; NOT the IR_VAR
     * named-variable kind, which resolve_arith_eval cannot read), or a constant atom pi/e (IR_ATOM). g_term
     * emits exactly those kinds, so a bound variable inside arith (e.g. X in `Y is X*2`) resolves. The
     * TT_ADD/TT_SUB/... PlClause-path kinds never appear as leaves in the live lower_clause_from_tree TT_FNC
     * tree; route the recognized term leaves through g_term and keep lower2(VALUE) for any unrecognized leaf. */
    switch (e->t) {
    case TT_VAR: case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_NAME:
        return g_term(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return lower2(tv, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — arithmetic comparison goal (`</2 >/2 =</2 >=/2 =:=/2 =\=/2`). Both args are arithmetic
 * EXPRESSIONS (via g_arith_expr to handle both TT_FNC-op and TT_ADD styles). Emits IR_BUILTIN(sval=op_str)
 * with LHS on bb->α and RHS on bb->β so the bb_exec.c IR_BUILTIN arm (resolve_arith_eval both sides) fires.
 * Previously emitted IR_ARITH with ival=BinopKind — wrong: resolve_arith_eval reads sval as op name.        */
/*====================================================================================================================================================================================================*/
static IR_t * g_compare(lcx_t cx, const tree_t * l_t, const tree_t * r_t, const char * op_str, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!l_t || !r_t) return NULL;
    IR_t * cmp = nalloc(cx, IR_BUILTIN);
    if (!cmp) return NULL;
    cmp->sval = op_str; cmp->ival = 2;
    IR_t * lα = NULL, * lβ = NULL;
    IR_t * l = g_arith_expr(cx, l_t, NULL, ω_in, &lα, &lβ);
    if (!l) return NULL;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_arith_expr(cx, r_t, NULL, ω_in, &rα, &rβ);
    if (!r) return NULL;
    (void) lβ; (void) rβ;
    cmp->α = lα; cmp->β = rα;
    set_succ_fail(cmp, γ_in, ω_in);
    return ret(cmp, α_out, β_out, cmp, ω_in /* semidet: resume -> fail */);
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — `is/2` arithmetic evaluation. LHS is a Prolog TERM (variable or number); RHS is an arith
 * EXPRESSION (via g_arith_expr). Emits IR_BUILTIN(sval="is") with LHS on bb->α (resolve_node_to_term)
 * and RHS on bb->β (resolve_arith_eval). The bb_exec.c IR_BUILTIN "is" arm matches exactly.             */
/*====================================================================================================================================================================================================*/
static IR_t * g_is(lcx_t cx, const tree_t * lhs_t, const tree_t * rhs_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!lhs_t || !rhs_t) return NULL;
    IR_t * bb = nalloc(cx, IR_BUILTIN);
    if (!bb) return NULL;
    bb->sval = "is"; bb->ival = 2;
    IR_t * laα = NULL, * laβ = NULL;
    IR_t * l = g_term(cx, lhs_t, NULL, ω_in, &laα, &laβ);
    if (!l) return NULL;
    IR_t * raα = NULL, * raβ = NULL;
    IR_t * r = g_arith_expr(cx, rhs_t, NULL, ω_in, &raα, &raβ);
    if (!r) return NULL;
    (void) laβ; (void) raβ;
    bb->α = laα; bb->β = raα;
    set_succ_fail(bb, γ_in, ω_in);
    return ret(bb, α_out, β_out, bb, ω_in /* semidet: resume -> fail */);
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — Prolog TERM in argument position. A term is DATA, not a goal: it lowers to the node kinds the
 * Prolog EXEC (bb_exec.c resolve_node_to_term) materializes into a Term* — IR_ATOM (atom/functor name),
 * IR_LOGICVAR (variable, slot in ival), IR_LIT_I/IR_LIT_F (number), IR_STRUCT (compound functor=sval, args on
 * the α-γ chain). This is the new-tree successor to the deleted lower_pl_term (blob d2d8c8e1). A term box is a
 * BOUNDED LEAF: α produces the term once -> γ; β -> ω. Children of a compound chain on prev->γ = next.α.        */
/*====================================================================================================================================================================================================*/
static IR_t * g_term(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) return NULL;
    switch (e->t) {
    case TT_ILIT: { IR_t * n = nalloc(cx, IR_LIT_I); if (!n) return NULL; n->ival = e->v.ival; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FLIT: { IR_t * n = nalloc(cx, IR_LIT_F); if (!n) return NULL; n->dval = e->v.dval; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_QLIT: case TT_NAME: { IR_t * n = nalloc(cx, IR_ATOM); if (!n) return NULL; n->sval = e->v.sval ? e->v.sval : "[]"; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_VAR:  { IR_t * n = nalloc(cx, IR_LOGICVAR); if (!n) return NULL; int slot = (int) e->v.ival; n->ival = slot; n->sval = NULL; if (cx.pl_vars && slot + 1 > cx.pl_vars->count) cx.pl_vars->count = slot + 1; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FNC: {
        IR_t * st = nalloc(cx, IR_STRUCT); if (!st) return NULL;
        st->sval = e->v.sval ? e->v.sval : "[]"; st->ival = e->n;
        IR_t * prev = NULL, * α0 = NULL;
        for (int i = 0; i < e->n; i++) {
            IR_t * cα = NULL, * cβ = NULL;
            IR_t * c = g_term(cx, e->c[i], NULL, NULL, &cα, &cβ);
            if (!c) return NULL;
            if (i == 0) { st->α = cα; α0 = cα; } else prev->γ = cα;
            prev = cα;
        }
        (void) α0;
        set_succ_fail(st, γ_in, ω_in);
        return ret(st, α_out, β_out, st, ω_in);
    }
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — deterministic Prolog builtin (write/writeln/print/nl, and the broader CHAIN family). Emits an
 * IR_BUILTIN node whose Prolog-correct execution lives in bb_exec.c's IR_BUILTIN case (pl_write — NO auto
 * newline; nl = putchar). This is DISTINCT from the SHARED wire_det_builtin1/IR_CALL path (which carries Icon
 * write semantics: arg via the AG ring + a trailing newline). The GOAL role is Prolog-exclusive, so emitting a
 * Prolog-owned kind here is FACT-RULE clean (language variation inside the role; no peer arm touched). Args
 * lower as TERMS (g_term) chained on bb->α (prev->γ = next.α); sval=fn, ival=arity. nl is a bare leaf (0 args).
 * Topology: BOUNDED — α=self, γ=γ_in, ω=ω_in, resume(β) -> ω_in. Successor to deleted lower_pl_new_Builtin.     */
/*====================================================================================================================================================================================================*/
static IR_t * g_builtin(lcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * bb = nalloc(cx, IR_BUILTIN); if (!bb) return NULL;
    bb->sval = fn; bb->ival = e ? e->n : 0;
    IR_t * prev = NULL;
    if (e) for (int i = 0; i < e->n; i++) {
        IR_t * aα = NULL, * aβ = NULL;
        IR_t * a = g_term(cx, e->c[i], NULL, NULL, &aα, &aβ);
        if (!a) return NULL;
        if (i == 0) bb->α = aα; else prev->γ = aα;
        prev = aα;
    }
    set_succ_fail(bb, γ_in, ω_in);
    return ret(bb, α_out, β_out, bb, ω_in /* deterministic: resume -> fail */);
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — user-predicate call (bare atom `foo` arity 0, or compound `foo(a,b)`). Emits IR_GOAL with a
 * bb_goal_state_t sidecar (callee name, arity, arg term-trees). IR_GOAL exec (bb_exec.c:3317) looks up
 * callee/arity via resolve_bb_lookup, allocates a per-activation env, binds args, runs the callee body graph,
 * restores on fail, retries via bb->state. β=self so the conjunction's backtrack re-enters for more solutions.
 * Args lower as g_term term-trees (read by resolve_node_to_term). FACT-RULE clean: Prolog-only IR kind.       */
/*====================================================================================================================================================================================================*/
static IR_t * g_goal(lcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    int ar = e ? e->n : 0;
    IR_t * nd = nalloc(cx, IR_GOAL); if (!nd) return NULL;
    bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc);
    if (!zc) return NULL;
    zc->callee = fn; zc->arity = ar; zc->nargs = ar; zc->cs = NULL;
    zc->args = ar > 0 ? (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *)) : NULL;
    for (int ai = 0; ai < ar; ai++) {
        if (!e->c[ai]) { zc->args[ai] = NULL; continue; }
        IR_t * aaα = NULL, * aaβ = NULL;
        g_term(cx, e->c[ai], NULL, NULL, &aaα, &aaβ);
        zc->args[ai] = aaα;
    }
    nd->ival = (int64_t)(intptr_t)zc;
    set_succ_fail(nd, γ_in, ω_in);
    return ret(nd, α_out, β_out, nd, nd /* β=self: exec re-enters on retry with bb->state advanced */);
}
/*====================================================================================================================================================================================================*/
/* GOAL ROLE — Prolog goals. Kind selects the arm; the sval/arity guards live INSIDE the TT_FNC arm
 * (they pick the control construct/builtin, not the kind). Foundation: cut, true/fail leaves. Extension:
 * Conj/Alt/Ite/Unify/Compare/Call/Builtin/phrase/catch/findall.                                           */
/*====================================================================================================================================================================================================*/
static IR_t * lower_goal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    switch (e->t) {
    case TT_CUT: return emit_leaf(cx, nalloc(cx, IR_CUT), γ_in, ω_in, α_out, β_out);
    case TT_QLIT: {
        const char * fn = e->v.sval;
        if (fn && (!strcmp(fn,"true")||!strcmp(fn,"otherwise"))) return emit_leaf(cx, nalloc(cx, IR_SUCCEED), γ_in, ω_in, α_out, β_out);
        if (fn && (!strcmp(fn,"fail")||!strcmp(fn,"false")))     return emit_leaf(cx, nalloc(cx, IR_FAIL), γ_in, ω_in, α_out, β_out);
        if (fn && !strcmp(fn,"nl")) return g_builtin(cx, "nl", NULL, γ_in, ω_in, α_out, β_out);
        if (fn) return g_goal(cx, fn, e, γ_in, ω_in, α_out, β_out);   /* bare-atom user-pred call (arity 0) */
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    case TT_UNIFY: {
        const tree_t * l = NULL, * r = NULL;
        if (!tm(e, TT_UNIFY, 2, &l, &r)) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        return g_unify(cx, l, r, γ_in, ω_in, α_out, β_out);
    }
    case TT_IF: case TT_VAR:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FNC: {
        const tree_t * A = NULL, * B = NULL, * arg = NULL;
        /* Prolog write-family — Prolog-correct IR_BUILTIN (pl_write, NO auto-newline), NOT the shared
           wire_det_builtin1/IR_CALL Icon path (which appends a newline). g_builtin takes the whole FNC so its
           e->n/e->c[] supply arity+args. (void)arg keeps the tm_g detector available for the user-call arm.) */
        if (tm_g(e, TT_FNC, "write",   1, &arg)) return g_builtin(cx, "write",   e, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "writeln", 1, &arg)) return g_builtin(cx, "writeln", e, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "print",   1, &arg)) return g_builtin(cx, "print",   e, γ_in, ω_in, α_out, β_out);
        /* conjunction `,/2` — SAME four-port sequence as Icon `&` / SNOBOL CAT (wire_seq, IR_GCONJ kind).
           flatten_seq matches by tree-KIND only and every Prolog operator is TT_FNC, so the `,`-spine is
           collected by walking right-nested `,`-tagged FNCs explicitly (sval guard distinguishes from `;`). */
        if (tm_g(e, TT_FNC, ",", 2, &A, &B)) {
            const tree_t * spine[64]; int sn = 0;
            const tree_t * cur = e;
            while (cur && cur->t == TT_FNC && cur->v.sval && !strcmp(cur->v.sval, ",") && cur->n == 2 && sn < 63) {
                spine[sn++] = cur->c[0]; cur = cur->c[1];
            }
            if (cur && sn < 64) spine[sn++] = cur;
            return wire_seq(cx, IR_GCONJ, spine, sn, γ_in, ω_in, α_out, β_out);
        }
        /* disjunction `;/2` — SAME fail-chain as Icon alternation / SNOBOL ALT (wire_alt, IR_DISJ kind). */
        if (tm_g(e, TT_FNC, ";", 2, &A, &B)) {
            const tree_t * spine[64]; int sn = 0;
            const tree_t * cur = e;
            while (cur && cur->t == TT_FNC && cur->v.sval && !strcmp(cur->v.sval, ";") && cur->n == 2 && sn < 63) {
                spine[sn++] = cur->c[0]; cur = cur->c[1];
            }
            if (cur && sn < 64) spine[sn++] = cur;
            return wire_alt(cx, IR_DISJ, spine, sn, γ_in, ω_in, α_out, β_out);
        }
        /* unification as an operator FNC `=/2`. */
        if (tm_g(e, TT_FNC, "=", 2, &A, &B)) return g_unify(cx, A, B, γ_in, ω_in, α_out, β_out);
        /* arithmetic comparison goals — op string passed directly to IR_BUILTIN sval (exec reads sval, not ival). */
        if (tm_g(e, TT_FNC, "<",   2, &A, &B)) return g_compare(cx, A, B, "<",   γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, ">",   2, &A, &B)) return g_compare(cx, A, B, ">",   γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "=<",  2, &A, &B)) return g_compare(cx, A, B, "=<",  γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, ">=",  2, &A, &B)) return g_compare(cx, A, B, ">=",  γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "=:=", 2, &A, &B)) return g_compare(cx, A, B, "=:=", γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "=\\=",2, &A, &B)) return g_compare(cx, A, B, "=\\=",γ_in, ω_in, α_out, β_out);
        /* is/2 — arith evaluation: LHS via g_term (->bb->α for resolve_node_to_term), RHS VALUE role (->bb->β for resolve_arith_eval). */
        if (tm_g(e, TT_FNC, "is",  2, &A, &B)) return g_is(cx, A, B, γ_in, ω_in, α_out, β_out);
        /* user-predicate call: TT_FNC(name, arg0..argN-1) → IR_GOAL with bb_goal_state_t sidecar.
           IR_GOAL exec (bb_exec.c:3317) looks up callee/arity in resolve_bb_lookup, allocates per-activation
           env, runs the body graph, restores on fail, retries via bb->state. FACT-RULE clean: Prolog-only IR kind. */
        { const char * fn = e->v.sval; int ar = e->n;
          if (fn && ar >= 0) {
              IR_t * nd = nalloc(cx, IR_GOAL); if (!nd) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
              bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc);
              if (!zc) return NULL;
              zc->callee = fn; zc->arity = ar; zc->nargs = ar; zc->cs = NULL;
              zc->args = ar > 0 ? (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *)) : NULL;
              for (int ai = 0; ai < ar; ai++) {
                  if (!e->c[ai]) { zc->args[ai] = NULL; continue; }
                  IR_t * aaα = NULL, * aaβ = NULL;
                  g_term(cx, e->c[ai], NULL, NULL, &aaα, &aaβ);
                  zc->args[ai] = aaα;
              }
              nd->ival = (int64_t)(intptr_t)zc;
              set_succ_fail(nd, γ_in, ω_in);
              return ret(nd, α_out, β_out, nd, nd /* β=self: exec re-enters on retry with bb->state advanced */);
          }
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================================================================================================*/
/* MASTER DISPATCH — the single funnel. Branch on role, then per-role switch over tree_e.                  */
/*====================================================================================================================================================================================================*/
static IR_t * lower2(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) { return ret(NULL, α_out, β_out, γ_in, ω_in); }
    switch (cx.role) {
    case ROLE_PATTERN: return lower_pattern(cx, e, γ_in, ω_in, α_out, β_out);
    case ROLE_GOAL:    return lower_goal(cx, e, γ_in, ω_in, α_out, β_out);
    case ROLE_VALUE:
    default:           return lower_value(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================================================================================================*/
/* UNHANDLED — loud single chokepoint for not-yet-added boxes. NOT a silent fallthrough.                   */
/*====================================================================================================================================================================================================*/
static IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) ω_in;
    fprintf(stderr, "[lower2] UNHANDLED role=%d kind=%d\n", (int)cx.role, e ? (int)e->t : -1);
    return ret(NULL, α_out, β_out, NULL, NULL);
}
/*====================================================================================================================================================================================================*/
/* PUBLIC ROLE ENTRIES — the only externally-visible surface of lower2. Each seeds the cursor with a role
 * (γ_in/ω_in are the program-level succeed/fail sentinels) and funnels into lower2. The proof harness and
 * (eventually) the driver reach the per-role switches ONLY through these three; lower2 itself stays static. */
/*====================================================================================================================================================================================================*/
IR_t * lower2_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_VALUE, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_pattern_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_goal_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_GOAL, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* lower2_clause_body_entry — lower one Prolog clause (TT_CLAUSE) into a four-port GOAL graph. The clause
 * carries arity in v.dval; its first `arity` children are head-arg term patterns, the rest are body goals.
 * PLG-3 adds HEAD UNIFICATION: the IR_GOAL caller binds callee env slot i to the caller's i-th argument
 * (term_new_var(i)), so each head arg position i must unify with LOGICVAR(i). For a head VAR at slot i this
 * is a trivial self-unify (frontend assigns head var position i -> slot i); for a head atom/number/compound
 * it binds slot i to that term. We emit, in order: g_head_unify(0..arity-1) then the body goals, all wired
 * as ONE IR_GCONJ. A 0-arg, 0-body bare fact succeeds immediately. γ_in/ω_in = clause succ/fail sentinels.   */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* g_head_unify — emit IR_UNIFY(LOGICVAR(slot), head_arg_term). LHS is a synthetic logicvar referencing the
 * callee env slot the IR_GOAL arm bound; RHS is the head-arg tree lowered as a term. Semidet (resume->ω).   */
static IR_t * g_head_unify(lcx_t cx, int slot, const tree_t * head_arg, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!head_arg) return NULL;
    IR_t * uni = nalloc(cx, IR_UNIFY); if (!uni) return NULL;
    IR_t * lv = nalloc(cx, IR_LOGICVAR); if (!lv) return NULL;
    lv->ival = slot; lv->sval = NULL;
    if (cx.pl_vars && slot + 1 > cx.pl_vars->count) cx.pl_vars->count = slot + 1;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_term(cx, head_arg, NULL, NULL, &rα, &rβ);
    if (!r) return NULL; (void) rβ;
    uni->α = lv;                                    /* lhs = callee env slot (resolve_node_to_term reads it) */
    uni->β = rα;                                    /* rhs = head-arg term-tree */
    set_succ_fail(uni, γ_in, ω_in);
    return ret(uni, α_out, β_out, uni, ω_in /* semidet */);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_clause_body_entry(IR_graph_t * bbg, const tree_t * clause, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    int arity = (int) clause->v.dval;
    if (arity < 0) arity = 0;
    int nbody = clause->n - arity;
    if (nbody < 0) nbody = 0;
    pl_vars_t pv; pv.count = 0;
    lcx_t cx = { bbg, ROLE_GOAL, 0, 0 };
    cx.pl_vars = &pv;
    /* count head args that need a real unify goal: a head VAR at its own positional slot is a self-unify we
       still emit (harmless, keeps slot count correct); atoms/numbers/compounds bind. We emit one per arg. */
    if (arity == 0 && nbody == 0) {                 /* bare 0-arg fact: succeed once */
        IR_t * s = nalloc(cx, IR_SUCCEED);
        if (bbg) bbg->nslots = 0;
        return emit_leaf(cx, s, γ_in, ω_in, α_out, β_out);
    }
    /* Build the head-unify + body goal spine. We can't synthesize tree nodes for head unifies, so we wire
       the IR directly: lower each piece NULL/NULL (unthreaded), collect their α nodes, then fail-chain them
       like wire_seq does for IR_GCONJ. To reuse wire_seq's exact topology we instead lower head-unifies and
       body goals into a single GCONJ by hand-threading: simplest correct form is a left-to-right γ-chain
       with each element's ω -> previous element's β (the conjunction backtrack), matching wire_seq.        */
    int total = arity + nbody;
    if (total > 128) return NULL;
    IR_t * entry[128]; IR_t * resume[128]; IR_t * apply[128];
    IR_t * node = nalloc(cx, IR_GCONJ); if (!node) return NULL;
    /* Lower each element right-to-left (last element's γ -> the wrapper node; mirrors wire_seq). Head-unify
       elements (idx < arity) and body-goal elements (idx >= arity) share the same fail-chain threading. */
    for (int idx = total - 1; idx >= 0; idx--) {
        IR_t * γi = (idx + 1 < total) ? entry[idx + 1] : node;
        IR_t * eα = NULL, * eβ = NULL; IR_t * top = NULL;
        if (idx < arity) top = g_head_unify(cx, idx, clause->c[idx], γi, ω_in, &eα, &eβ);
        else             top = lower2(cx, clause->c[idx], γi, ω_in, &eα, &eβ);
        if (!top || !eα) return NULL;
        apply[idx] = top; entry[idx] = eα; resume[idx] = eβ;
    }
    for (int i = 1; i < total; i++) {                          /* element i fails -> retry nearest RESUMABLE predecessor (skip bounded ones, */
        IR_t * tgt = ω_in;                                     /* whose resume == ω_in). resume[j] for a resumable element (IR_GOAL/IR_CHOICE) */
        for (int j = i - 1; j >= 0; j--) {                     /* is the node itself (redo on re-entry); a bounded head-unify/body goal returns */
            if (resume[j] && resume[j] != ω_in) { tgt = resume[j]; break; }  /* resume == ω_in. Walk back; if none resumable, the clause body fails to ω_in. */
        }
        apply[i]->ω = tgt;
    }
    set_succ_fail(node, γ_in, ω_in);
    if (bbg) bbg->nslots = pv.count;
    return ret(node, α_out, β_out, entry[0], resume[total - 1]);
}