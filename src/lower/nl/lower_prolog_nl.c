#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "ast.h"
#include "IR.h"
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; } lcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γα_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ωβ_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "β", 3); nd->ω.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = IR_node_alloc(cx->g, op); γα_to(nd, γ); ωβ_to(nd, ω); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_builtin_visible(const char * s) { return s && (!strcmp(s, "write") || !strcmp(s, "nl") || !strcmp(s, "format") || !strcmp(s, "aggregate_all")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term(lcx_t * cx, const tree_t * t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_nest(lcx_t * cx, const tree_t * t, int from) {
    if (from == t->n - 1) return term(cx, t->c[from]);
    IR_t * nd = build(cx, IR_STRUCT, NULL, NULL); IR_LIT(nd).sval = t->v.sval; IR_LIT(nd).ival = 2;
    ir_operand_push(nd, term(cx, t->c[from]));
    ir_operand_push(nd, term_nest(cx, t, from + 1));
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term(lcx_t * cx, const tree_t * t) {
    if (!t) return NULL;
    switch (t->t) {
    case TT_QLIT: { IR_t * nd = build(cx, IR_ATOM, NULL, NULL); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, NULL, NULL); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_VAR:  { IR_t * nd = build(cx, IR_LOGICVAR, NULL, NULL); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_MAKELIST: {
        IR_t * prev = build(cx, IR_ATOM, NULL, NULL); IR_LIT(prev).sval = "[]";
        for (int i = t->n - 1; i >= 0; i--) {
            IR_t * e = term(cx, t->c[i]);
            IR_t * c = build(cx, IR_STRUCT, NULL, NULL); IR_LIT(c).sval = "."; IR_LIT(c).ival = 2;
            ir_operand_push(c, e); ir_operand_push(c, prev); prev = c;
        }
        return prev;
    }
    case TT_FNC: {
        if (t->v.sval && (!strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";")) && t->n > 2) return term_nest(cx, t, 0);
        IR_t * nd = build(cx, IR_STRUCT, NULL, NULL); IR_LIT(nd).sval = t->v.sval; IR_LIT(nd).ival = t->n;
        for (int i = 0; i < t->n; i++) ir_operand_push(nd, term(cx, t->c[i]));
        return nd;
    }
    default: { IR_t * nd = build(cx, IR_ATOM, NULL, NULL); IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail) {
    if (!t) return build(cx, IR_SUCCEED, γnext, ωfail);
    switch (t->t) {
    case TT_FNC: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (is_builtin_visible(nm)) {
            IR_t * nd = build(cx, IR_BUILTIN, γnext, ωfail); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = t->n;
            for (int i = 0; i < t->n; i++) ir_operand_push(nd, term(cx, t->c[i]));
            return nd;
        }
        IR_t * nd = build(cx, IR_GOAL, γnext, ωfail); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long)(intptr_t) calloc(1, sizeof(void *) * (t->n ? t->n : 1));
        for (int i = 0; i < t->n; i++) term(cx, t->c[i]);
        return nd;
    }
    case TT_QLIT: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, "true"))  return build(cx, IR_SUCCEED, γnext, ωfail);
        if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return build(cx, IR_FAIL, γnext, ωfail);
        if (is_builtin_visible(nm)) { IR_t * nd = build(cx, IR_BUILTIN, γnext, ωfail); IR_LIT(nd).sval = nm; return nd; }
        IR_t * nd = build(cx, IR_GOAL, γnext, ωfail); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long)(intptr_t) calloc(1, sizeof(void *));
        return nd;
    }
    case TT_UNIFY: {
        IR_t * nd = build(cx, IR_UNIFY, γnext, ωfail);
        ir_operand_push(nd, term(cx, t->c[0]));
        ir_operand_push(nd, term(cx, t->c[1]));
        return nd;
    }
    case TT_CUT: return build(cx, IR_CUT, γnext, ωfail);
    default: return build(cx, IR_SUCCEED, γnext, ωfail);
    }
}
/*====================================================================================================================================================================================================*/
IR_graph_t * lower_prolog(const tree_t * prog) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PL);
    lcx_t cx; cx.g = g;
    const tree_t * clause = NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (d->t == TT_CHOICE && d->v.sval && !strcmp(d->v.sval, "main/0") && d->n > 0) { clause = d->c[0]; break; }
    }
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gconj   = build(&cx, IR_GCONJ, succeed, fail);
    IR_LIT(gconj).ival = (long long)(intptr_t) calloc(1, sizeof(void *));
    if (!clause) { g->entry = gconj; return g; }
    int arity = (int) clause->v.dval;
    int ng = clause->n - arity;
    IR_t ** gn = (IR_t **) calloc((ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t * next = gconj;
    for (int i = clause->n - 1; i >= arity; i--) {
        IR_t * nd = goal(&cx, clause->c[i], next, fail);
        gn[i - arity] = nd;
        next = nd;
    }
    IR_t * last_res = fail;
    for (int i = 0; i < ng; i++) {
        ωβ_to(gn[i], last_res);
        if (gn[i]->op == IR_GOAL) last_res = gn[i];
    }
    g->entry = (ng > 0) ? gn[0] : gconj;
    free(gn);
    return g;
}
