#include <string.h>
#include "ast.h"
#include "IR.h"
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; } pcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "β", 3); nd->ω.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(pcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = IR_node_alloc(cx->g, op); γ_to(nd, γ); ω_to(nd, ω); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_binop(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    IR_t * op = build(cx, IR_BINOP, γ, ω);
    IR_t * le = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω);
    IR_t * re = lower(cx, (t->n > 1) ? t->c[1] : NULL, op, ω);
    γ_to(le, re); return le;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * c0 = (t->n > 0) ? t->c[0] : NULL; IR_t * nd = build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = (c0 && c0->v.sval) ? c0->v.sval : NULL;
    IR_LIT(nd).ival = (t->n > 0) ? t->n - 1 : 0; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_assign(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * lhs = (t->n > 0) ? t->c[0] : NULL; const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
    IR_t * nd = build(cx, IR_ASSIGN, γ, ω);
    IR_LIT(nd).sval = (lhs && lhs->t == TT_VAR) ? lhs->v.sval : (lhs && lhs->t == TT_FNC && lhs->n > 0 && lhs->c[0]) ? lhs->c[0]->v.sval : NULL;
    IR_t * rentry = lower(cx, rhs, nd, ω); return rentry ? rentry : nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_block(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * st[512]; int k = 0;
    for (int i = 0; i < t->n && k < 512; i++) {
        const tree_t * s = t->c[i];
        if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        if (s && s->t == TT_PROC_DECL) continue; if (s) st[k++] = s; }
    if (k == 0) return build(cx, IR_SUCCEED, γ, ω); IR_t * succ = γ; IR_t * entry = NULL;
    for (int i = k - 1; i >= 0; i--) { entry = lower(cx, st[i], succ, ω); succ = entry; }
    return entry;
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    if (!t) return build(cx, IR_SUCCEED, γ, ω);
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, γ, ω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_VAR: { IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW: case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return lower_binop(cx, t, γ, ω);
    case TT_ASSIGN: return lower_assign(cx, t, γ, ω);
    case TT_FNC: return lower_call(cx, t, γ, ω);
    case TT_PROGRAM: return lower_block(cx, t, γ, ω);
    case TT_SEQ_EXPR: return lower_block(cx, t, γ, ω);
    case TT_STMT: { const tree_t * sub = stmt_subj(t); return sub ? lower(cx, sub, γ, ω) : build(cx, IR_SUCCEED, γ, ω); }
    default: return build(cx, IR_SUCCEED, γ, ω);
    }
}
/*====================================================================================================================================================================================================*/
int lower_pascal_enum(const tree_t * prog, const tree_t ** out, int max) {
    int n = 0; if (!prog) return 0;
    for (int i = 0; i < prog->n && n < max; i++) {
        const tree_t * s = prog->c[i];
        if (s && s->t == TT_STMT) s = stmt_subj(s);
        if (s && s->t == TT_PROC_DECL) out[n++] = s; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_pascal_proc(const tree_t * prog, const tree_t * pd) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PAS); pcx_t cx; cx.g = g;
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_t * fail = IR_node_alloc(g, IR_FAIL);
    const tree_t * body = (pd && pd->n > 2) ? pd->c[2] : NULL;
    int is_func = (pd && pd->n > 3 && pd->c[3] && pd->c[3]->t == TT_VAR); IR_t * top = succ;
    if (is_func) { IR_t * ret = build(&cx, IR_RETURN, succ, succ); IR_t * rv = build(&cx, IR_VAR, NULL, NULL);
        IR_LIT(rv).sval = pd->v.sval; ir_operand_push(ret, rv); top = ret; }
    IR_t * entry = lower(&cx, body, top, fail); g->entry = entry ? entry : succ; return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_pascal(const tree_t * prog) {
    const tree_t * procs[256]; int np = lower_pascal_enum(prog, procs, 256);
    if (np > 0) return lower_pascal_proc(prog, procs[np - 1]);
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PAS); pcx_t cx; cx.g = g;
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_node_alloc(g, IR_FAIL);
    g->entry = succ; return g;
}
