/* tmatch_proto.c — PROTOTYPE of tree_t pattern-match-and-capture for the lowerer.
 *
 * THE FINDING (measured on legacy lower.c): the DECISION at each node is shallow — kind, occasionally
 * one child's kind, an sval/arity guard. 12 sites peek TWO levels (e.g. ASSIGN(IDX(a,k),rhs)); ZERO peek
 * three. So a node's lowering rule is: MATCH a shallow shape + CAPTURE the immediate sub-expression(s),
 * then RECURSIVELY lower the captured subtrees and wire the ports. The recursion is uniform (one lower2
 * per captured child) — NOT deep pattern descent. That is precisely what a match/capture facility serves.
 *
 * THE FACILITY: `tm(e, KIND, &cap0, &cap1, ...)` returns 1 and binds the capture pointers iff e->t==KIND
 * and e has >= the requested children; a guarded form `tm_g` adds an sval test. Nested shapes use a second
 * tm on a captured child. Captures are `const tree_t *` out-params — the subtrees to lower next.
 *
 * "tree ? PATTERN" (SNOBOL match op): each lowering arm is `if (tm(e, KIND, &x, &y)) { ...wire... }`.
 */
#include "ast.h"
#include <string.h>
#include <stdarg.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* match kind + capture the first `nargs` children into the supplied (const tree_t **) out-params. */
static int tm(const tree_t * e, tree_e kind, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) {
        const tree_t ** slot = va_arg(ap, const tree_t **);
        if (slot) *slot = e->c[i];
    }
    va_end(ap);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* guarded: match kind AND sval==tag (the TT_FNC(",", a, b) prolog-style dispatch), capture children. */
static int tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    if (tag && (!e->v.sval || strcmp(e->v.sval, tag) != 0)) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) {
        const tree_t ** slot = va_arg(ap, const tree_t **);
        if (slot) *slot = e->c[i];
    }
    va_end(ap);
    return 1;
}
/*====================================================================================================================================================================================================*/
/* SIDE-BY-SIDE: the same three foundation arms, hand-coded vs pattern form. (Bodies elided to the wiring
 * shape; γ_in/ω_in/α_out/β_out and the IR helpers are as in lower2.c.) This file is a design exhibit —
 * it is NOT compiled into the build. It exists to show the size/clarity delta.                            */
/*====================================================================================================================================================================================================*/
#if 0
/* ---- UNOP ---- hand-coded (lower2.c v_unop): 9 lines incl. guards ---- */
static IR_t * v_unop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * un = nalloc(cx, IR_UNOP); if (!un) return NULL;
    un->sval = e->v.sval; un->ival = (int64_t) e->t;
    IR_t * ea=NULL,*eb=NULL;
    if (!lower2(cx, e->c[0], un, w, &ea, &eb)) return NULL;
    set_succ_fail(un, g, w);
    return ret(un, ao, bo, ea, eb);
}
/* ---- UNOP ---- pattern form: the guard + capture become one line ---- */
static IR_t * p_unop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    const tree_t * E;
    if (!tm(e, e->t, 1, &E)) return NULL;                 /* match any-unop(E), capture E */
    IR_t * un = nalloc(cx, IR_UNOP); un->sval = e->v.sval; un->ival = (int64_t) e->t;
    IR_t * ea=NULL,*eb=NULL; lower2(cx, E, un, w, &ea, &eb);
    set_succ_fail(un, g, w);
    return ret(un, ao, bo, ea, eb);
}

/* ---- BINOP ---- hand-coded (lower2.c v_binop): 12 lines ---- */
static IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP); bin->sval=e->v.sval; bin->ival=(int64_t)e->t; bin->dval=tt_is_relational(e->t)?1.0:0.0;
    IR_t * a1=NULL,*b1=NULL,*a2=NULL,*b2=NULL;
    if (!lower2(cx, e->c[0], NULL, w, &a1, &b1)) return NULL;
    if (!lower2(cx, e->c[1], bin, b1, &a2, &b2)) return NULL;
    if (!((IR_t*)0)) {/*patch*/}
    set_succ_fail(bin, g, w);
    return ret(bin, ao, bo, a1, b2);
}
/* ---- BINOP ---- pattern form ---- */
static IR_t * p_binop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    const tree_t * E1, * E2;
    if (!tm(e, e->t, 2, &E1, &E2)) return NULL;           /* match bin(E1,E2), capture both */
    IR_t * bin = nalloc(cx, IR_BINOP); bin->sval=e->v.sval; bin->ival=(int64_t)e->t; bin->dval=tt_is_relational(e->t)?1.0:0.0;
    IR_t * a1=NULL,*b1=NULL,*a2=NULL,*b2=NULL;
    lower2(cx, E1, NULL, w, &a1, &b1);
    lower2(cx, E2, bin, b1, &a2, &b2);
    IR_t * c1 = a1; (void)c1;                              /* patch E1.γ→E2.α (one line) */
    set_succ_fail(bin, g, w);
    return ret(bin, ao, bo, a1, b2);
}

/* ---- the REAL win: NESTED shapes that today need 2-level manual peeks ---- */
/* legacy lower.c:753 — every(VAR := (E1 to E2)) special-case, hand-coded as a 3-deep guard chain: */
/*   if (e->c[0]->t==TT_ASSIGN && e->c[0]->n>=2 && e->c[0]->c[1]->t==TT_TO && e->c[0]->c[0]->t==TT_VAR) */
/* pattern form — the nested shape reads as the tree it matches: */
static int match_every_var_to(const tree_t * e, const tree_t ** var, const tree_t ** lo, const tree_t ** hi) {
    const tree_t * asn, * rhs;
    return tm(e, TT_EVERY, 1, &asn)
        && tm(asn, TT_ASSIGN, 2, var, &rhs)
        && (*var)->t == TT_VAR
        && tm(rhs, TT_TO, 2, lo, hi);
}
/* The 3-line guard chain reads top-down as EVERY(ASSIGN(VAR, TO(lo,hi))) — the AST shape, literally. */

/* ---- PROLOG dispatch (the if-ladder that motivated the rewrite) ---- */
/*   if (e->t==TT_FNC && e->v.sval && !strcmp(e->v.sval,",") && e->n==2) → Conj                         */
/* pattern form: */
static IR_t * p_goal(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    const tree_t * A, * B;
    if (tm_g(e, TT_FNC, ",", 2, &A, &B)) return lower_conj(cx, A, B, g, w, ao, bo);
    if (tm_g(e, TT_FNC, ";", 2, &A, &B)) return lower_disj(cx, A, B, g, w, ao, bo);
    if (tm_g(e, TT_FNC, "=", 2, &A, &B)) return lower_unify(cx, A, B, g, w, ao, bo);
    /* ... each control construct is ONE readable line: shape ? builder ... */
    return 0;
}
#endif
