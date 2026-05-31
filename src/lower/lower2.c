/*====================================================================================================================================================================================================*/
/* lower2.c — UNIFIED AST->IR LOWERER (ground-zero foundation).
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
#include <gc/gc.h>
/*====================================================================================================================================================================================================*/
/* CURSOR — threaded through the descent. `role` selects the rule family; `bbg` is the graph populated;
 * `bounded` is the inherited single-value flag (jcon's `bounded`): when set, generator boxes may collapse
 * their resume port to ω. `lang` is the source language for the rare language-specific arm.              */
/*====================================================================================================================================================================================================*/
typedef enum { ROLE_VALUE = 0, ROLE_PATTERN = 1, ROLE_GOAL = 2 } lower_role_e;
typedef struct {
    IR_graph_t * bbg;
    lower_role_e role;
    int          bounded;
    int          lang;
} lcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower2(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
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
/* TEMPLATE CLASS (1) — BOUNDED LEAF. Paper §4.1: a single-value box. The node is its own α and (when
 * resumable & unbounded) its own β; bounded or non-resumable kinds collapse β to ω (resume -> fail).
 * `kind_is_resumable` (shared, in lower.c) classifies which IR kinds can produce a second value.          */
/*====================================================================================================================================================================================================*/
extern int kind_is_resumable(IR_e t);
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
static IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP);
    if (!bin) return NULL;
    bin->sval = e->v.sval;
    bin->ival = (int64_t) e->t;
    bin->dval = tt_is_relational(e->t) ? 1.0 : 0.0;
    IR_t * e1α=NULL, * e1β=NULL, * e2α=NULL, * e2β=NULL;
    IR_t * c1 = lower2(cx, e->c[0], NULL /*E1.γ patched below*/, ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = lower2(cx, e->c[1], bin /*E2.γ -> bin*/, e1β /*E2.ω -> E1.β*/, &e2α, &e2β);
    if (!c2) return NULL;
    if (!c1->γ) c1->γ = e2α;            /* patch E1.succeed -> E2.start */
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
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — CONJUNCTION `E1 & E2` (jcon ir_conjunction). Identical to plus minus the value compute:
 *   conj.α = E1.α ; conj.β = E2.β ; E1.γ = E2.α ; E1.ω = conj.ω ; E2.γ = conj ; E2.ω = E1.β.
 * The IR_CONJ node forwards E2's value (Icon `e1 & e2` yields e2), sitting on E2's success edge.          */
/*====================================================================================================================================================================================================*/
static IR_t * v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * conj = nalloc(cx, IR_CONJ);
    if (!conj) return NULL;
    IR_t * e1α=NULL,*e1β=NULL,*e2α=NULL,*e2β=NULL;
    IR_t * c1 = lower2(cx, e->c[0], NULL /*E1.γ patched below*/, ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = lower2(cx, e->c[1], conj /*E2.γ -> conj*/, e1β /*E2.ω -> E1.β*/, &e2α, &e2β);
    if (!c2) return NULL;
    if (!c1->γ) c1->γ = e2α;            /* E1.succeed -> E2.start */
    set_succ_fail(conj, γ_in, ω_in);
    return ret(conj, α_out, β_out, e1α, e2β);
}
/*====================================================================================================================================================================================================*/
/* VALUE-ROLE — ALTERNATION `E1 | E2 | ... | Ek` (jcon ir_a_Alt; the SIBLING-backtrack box, runtime-gated
 * like `if`). Each arm's success flows to the alt node (which records the active arm's resume in its gate
 * then -> alt.γ); arms are fail-chained (Ei.ω -> E(i+1).α; last -> alt.ω); alt.β re-dispatches via the gate
 * to the active arm's resume. The ordered arm resume ports live in operand_aux for the executor's gate.    */
/*====================================================================================================================================================================================================*/
static IR_t * v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1) return NULL;
    IR_t * alt = nalloc(cx, IR_ALT);
    if (!alt) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    int k = e->n;
    if (k > 64) return NULL;
    for (int j = 0; j < k; j++) {
        if (!e->c[j]) return NULL;
        IR_t * αj=NULL,*βj=NULL;
        IR_t * arm = lower2(cx, e->c[j], alt /*arm.γ -> alt*/, NULL /*arm.ω chained below*/, &αj, &βj);
        if (!arm) return NULL;
        if (!arm->γ) arm->γ = alt;
        apply[j] = arm; entry[j] = αj ? αj : arm; resume[j] = βj;
    }
    for (int j = 0; j < k; j++) {
        IR_t * next = (j + 1 < k) ? entry[j + 1] : ω_in;   /* Ei.fail -> E(i+1).start ; last -> alt.fail */
        if (!apply[j]->ω) apply[j]->ω = next;
    }
    bb_operand_aux_set(cx.bbg, alt, resume, k);
    set_succ_fail(alt, γ_in, ω_in);
    return ret(alt, α_out, β_out, entry[0], alt /* alt is its own resume; gate dispatches to active arm */);
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
    nt->β = ce;                          /* operand ref to the negated value-node */
    set_succ_fail(nt, γ_in, ω_in);
    return ret(nt, α_out, β_out, eα, ω_in /* resume -> fail */);
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
    case TT_ALTERNATE:
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

    /* --- extension surface (each = one box onto the foundation, canonical signature) --- */
    case TT_LIMIT:      /* jcon ir_a_Limitation */
    case TT_CASE:       /* jcon ir_a_Case */
    case TT_RETURN:     /* jcon ir_a_Return */
    case TT_NRETURN:
    case TT_SUSPEND:    /* jcon ir_a_Suspend */
    case TT_PROC_FAIL:  /* jcon ir_a_Fail */
    case TT_LOOP_BREAK: /* jcon ir_a_Break */
    case TT_LOOP_NEXT:  /* jcon ir_a_Next */
    case TT_ASSIGN: case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP:
    case TT_FNC:        /* jcon ir_a_Call (+ SNOBOL builtin folds when role flips) */
    case TT_METHCALL:
    case TT_FIELD:      /* jcon ir_a_Field */
    case TT_IDX:
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS:  /* jcon ir_a_Sectionop */
    case TT_INDIRECT: case TT_IDENTICAL:
    case TT_SCAN:       /* jcon ir_a_Scan */
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
extern char * cset_try_fold(const tree_t * t);
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
    /* extension: LEN POS RPOS TAB RTAB FENCE ARBNO CAT/SEQ ALT
       CAPT_COND_ASGN CAPT_IMMED_ASGN CAPT_CURSOR DEFER VAR(*var) BAL, and FNC(SPAN/ANY/.../ARBNO). */
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
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
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);   /* nl builtin; bare-atom Call */
    }
    case TT_UNIFY:
    case TT_GT: case TT_LT: case TT_GE: case TT_LE: case TT_EQ: case TT_NE:
    case TT_IF: case TT_VAR: case TT_FNC:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
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
    lcx_t cx = { bbg, ROLE_VALUE, 0, 0 };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_pattern_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, 0 };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_goal_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_GOAL, 0, 0 };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
