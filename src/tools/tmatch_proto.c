#include "ast.h"
#include <string.h>
#include <stdarg.h>
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*====================================================================================================================*/
/*====================================================================================================================*/
#if 0
static IR_t * v_unop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * un = nalloc(cx, IR_UNOP); if (!un) return NULL;
    un->sval = e->v.sval; un->ival = (int64_t) e->t;
    IR_t * ea=NULL,*eb=NULL;
    if (!lower2(cx, e->c[0], un, w, &ea, &eb)) return NULL;
    set_succ_fail(un, g, w);
    return ret(un, ao, bo, ea, eb);
}
static IR_t * p_unop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    const tree_t * E;
    if (!tm(e, e->t, 1, &E)) return NULL;
    IR_t * un = nalloc(cx, IR_UNOP); un->sval = e->v.sval; un->ival = (int64_t) e->t;
    IR_t * ea=NULL,*eb=NULL; lower2(cx, E, un, w, &ea, &eb);
    set_succ_fail(un, g, w);
    return ret(un, ao, bo, ea, eb);
}
static IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP); bin->sval=e->v.sval; bin->ival=(int64_t)e->t; bin->dval=tt_is_relational(e->t)?1.0:0.0;
    IR_t * a1=NULL,*b1=NULL,*a2=NULL,*b2=NULL;
    if (!lower2(cx, e->c[0], NULL, w, &a1, &b1)) return NULL;
    if (!lower2(cx, e->c[1], bin, b1, &a2, &b2)) return NULL;
    if (!((IR_t*)0)) { }
    set_succ_fail(bin, g, w);
    return ret(bin, ao, bo, a1, b2);
}
static IR_t * p_binop(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    const tree_t * E1, * E2;
    if (!tm(e, e->t, 2, &E1, &E2)) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP); bin->sval=e->v.sval; bin->ival=(int64_t)e->t; bin->dval=tt_is_relational(e->t)?1.0:0.0;
    IR_t * a1=NULL,*b1=NULL,*a2=NULL,*b2=NULL;
    lower2(cx, E1, NULL, w, &a1, &b1);
    lower2(cx, E2, bin, b1, &a2, &b2);
    IR_t * c1 = a1; (void)c1;
    set_succ_fail(bin, g, w);
    return ret(bin, ao, bo, a1, b2);
}
static int match_every_var_to(const tree_t * e, const tree_t ** var, const tree_t ** lo, const tree_t ** hi) {
    const tree_t * asn, * rhs;
    return tm(e, TT_EVERY, 1, &asn)
        && tm(asn, TT_ASSIGN, 2, var, &rhs)
        && (*var)->t == TT_VAR
        && tm(rhs, TT_TO, 2, lo, hi);
}
static IR_t * p_goal(lcx_t cx, const tree_t * e, IR_t * g, IR_t * w, IR_t ** ao, IR_t ** bo) {
    const tree_t * A, * B;
    if (tm_g(e, TT_FNC, ",", 2, &A, &B)) return lower_conj(cx, A, B, g, w, ao, bo);
    if (tm_g(e, TT_FNC, ";", 2, &A, &B)) return lower_disj(cx, A, B, g, w, ao, bo);
    if (tm_g(e, TT_FNC, "=", 2, &A, &B)) return lower_unify(cx, A, B, g, w, ao, bo);
    return 0;
}
#endif
