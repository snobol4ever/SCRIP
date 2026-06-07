/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower.h"
#include "lower_internal.h"
#include "IR_interp_state.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <gc/gc.h>
/*====================================================================================================================*/
/*====================================================================================================================*/
typedef enum { PL_ROLE_VALUE = 0, PL_ROLE_GOAL = 2 } pl_role_e;
typedef struct {
    IR_graph_t * bbg;
    pl_role_e    role;
    int          bounded;
    int          lang;
    IR_t       * loop_ω;
    IR_t       * loop_next;
    pl_vars_t  * pl_vars;
} plcx_t;
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pl_lower_goal(plcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_nalloc(plcx_t cx, IR_e kind) { return IR_node_alloc(cx.bbg, kind); }
/*--------------------------------------------------------------------------------------------------------------------*/
static void pl_set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in) {
    if (!n) return;
    if (!n->γ && γ_in) n->γ = γ_in;
    if (!n->ω && ω_in) n->ω = ω_in;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β) {
    if (α_out) *α_out = α;
    if (β_out) *β_out = β;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_emit_leaf(plcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) cx;
    if (!n) return NULL;
    pl_set_succ_fail(n, γ_in, ω_in);
    return pl_ret(n, α_out, β_out, n, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_unhandled(plcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) cx; (void) γ_in; (void) ω_in;
    fprintf(stderr, "[lower_prolog] UNHANDLED kind=%d\n", e ? (int)e->t : -1);
    return pl_ret(NULL, α_out, β_out, NULL, NULL);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_tm(const tree_t * e, tree_e kind, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    if (tag && (!e->v.sval || strcmp(e->v.sval, tag) != 0)) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pl_wire_seq(plcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1 || nkids > 64) return NULL;
    IR_t * node = pl_nalloc(cx, kind); if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    for (int i = nkids - 1; i >= 0; i--) {
        if (!kids[i]) return NULL;
        IR_t * γi = (i + 1 < nkids) ? entry[i + 1] : node;
        IR_t * αi = NULL, * βi = NULL;
        IR_t * c = pl_lower_goal(cx, kids[i], γi, ω_in, &αi, &βi);
        if (!c) return NULL;
        apply[i] = c; entry[i] = αi ? αi : c; resume[i] = βi;
    }
    for (int i = 1; i < nkids; i++) {
        IR_t * tgt = ω_in;
        for (int j = i - 1; j >= 0; j--) {
            if (resume[j] && resume[j] != ω_in) { tgt = resume[j]; break; }
        }
        apply[i]->ω = tgt;
    }
    if (kind == IR_GCONJ) {
        bb_conj_state_t * zs = (bb_conj_state_t *)GC_MALLOC(sizeof *zs);
        if (zs) {
            zs->goals = (IR_t **)GC_MALLOC((size_t)nkids * sizeof(IR_t *));
            if (zs->goals) { for (int i = 0; i < nkids; i++) zs->goals[i] = apply[i]; zs->ngoals = nkids; IR_LIT(node).ival = (int64_t)(intptr_t)zs; }
        }
    }
    pl_set_succ_fail(node, γ_in, ω_in);
    return pl_ret(node, α_out, β_out, entry[0], resume[nkids - 1]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_wire_alt(plcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1 || nkids > 64) return NULL;
    IR_t * node = pl_nalloc(cx, kind); if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    IR_t * arm_succ = (kind == IR_DISJ) ? γ_in : node;
    for (int j = nkids - 1; j >= 0; j--) {
        if (!kids[j]) return NULL;
        IR_t * ωj = (j + 1 < nkids) ? entry[j + 1] : ω_in;
        IR_t * αj = NULL, * βj = NULL;
        IR_t * arm = pl_lower_goal(cx, kids[j], arm_succ, ωj, &αj, &βj);
        if (!arm) return NULL;
        if (!arm->γ) arm->γ = arm_succ;
        apply[j] = arm; entry[j] = αj ? αj : arm; resume[j] = βj;
    }
    bb_operand_aux_set(cx.bbg, node, apply, nkids);
    pl_set_succ_fail(node, γ_in, ω_in);
    return pl_ret(node, α_out, β_out, entry[0], node);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_term(plcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * g_unify(plcx_t cx, const tree_t * l_t, const tree_t * r_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!l_t || !r_t) return NULL;
    IR_t * uni = pl_nalloc(cx, IR_UNIFY); if (!uni) return NULL;
    IR_t * lα = NULL, * lβ = NULL, * rα = NULL, * rβ = NULL;
    IR_t * l = g_term(cx, l_t, NULL, NULL, &lα, &lβ); if (!l) return NULL;
    IR_t * r = g_term(cx, r_t, NULL, NULL, &rα, &rβ); if (!r) return NULL;
    (void) lβ; (void) rβ;
    if (!ir_operand_push(uni, lα)) return NULL;
    if (!ir_operand_push(uni, rα)) return NULL;
    pl_set_succ_fail(uni, γ_in, ω_in);
    return pl_ret(uni, α_out, β_out, uni, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_arith_expr(plcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) return NULL;
    if (e->t == TT_FNC) {
        const char * op = e->v.sval;
        if (!op) return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        int ar = e->n;
        IR_t * nd = pl_nalloc(cx, IR_ARITH); if (!nd) return NULL;
        IR_LIT(nd).sval = op; IR_LIT(nd).ival = ar;
        if (ar >= 1 && e->c[0]) {
            IR_t * aα = NULL, * aβ = NULL;
            IR_t * a = g_arith_expr(cx, e->c[0], NULL, ω_in, &aα, &aβ);
            if (!a) return NULL; (void) aβ;
            if (!ir_operand_push(nd, aα)) return NULL;
        }
        if (ar >= 2 && e->c[1]) {
            IR_t * bα = NULL, * bβ = NULL;
            IR_t * b = g_arith_expr(cx, e->c[1], NULL, ω_in, &bα, &bβ);
            if (!b) return NULL; (void) bβ;
            if (!ir_operand_push(nd, bα)) return NULL;
        }
        pl_set_succ_fail(nd, γ_in, ω_in);
        return pl_ret(nd, α_out, β_out, nd, ω_in);
    }
    switch (e->t) {
    case TT_VAR: case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_NAME:
        return g_term(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return pl_lower_goal(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_compare(plcx_t cx, const tree_t * l_t, const tree_t * r_t, const char * op_str, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!l_t || !r_t) return NULL;
    IR_t * cmp = pl_nalloc(cx, IR_BUILTIN); if (!cmp) return NULL;
    IR_LIT(cmp).sval = op_str; IR_LIT(cmp).ival = 2;
    IR_t * lα = NULL, * lβ = NULL;
    IR_t * l = g_arith_expr(cx, l_t, NULL, ω_in, &lα, &lβ); if (!l) return NULL;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_arith_expr(cx, r_t, NULL, ω_in, &rα, &rβ); if (!r) return NULL;
    (void) lβ; (void) rβ;
    if (!ir_operand_push(cmp, lα) || !ir_operand_push(cmp, rα)) return NULL;
    pl_set_succ_fail(cmp, γ_in, ω_in);
    return pl_ret(cmp, α_out, β_out, cmp, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_term_compare(plcx_t cx, const tree_t * l_t, const tree_t * r_t, const char * op_str, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!l_t || !r_t) return NULL;
    IR_t * cmp = pl_nalloc(cx, IR_BUILTIN); if (!cmp) return NULL;
    IR_LIT(cmp).sval = op_str; IR_LIT(cmp).ival = 2;
    IR_t * lα = NULL, * lβ = NULL;
    IR_t * l = g_term(cx, l_t, NULL, NULL, &lα, &lβ); if (!l) return NULL;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_term(cx, r_t, NULL, NULL, &rα, &rβ); if (!r) return NULL;
    (void) lβ; (void) rβ;
    if (!ir_operand_push(cmp, lα) || !ir_operand_push(cmp, rα)) return NULL;
    pl_set_succ_fail(cmp, γ_in, ω_in);
    return pl_ret(cmp, α_out, β_out, cmp, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_is(plcx_t cx, const tree_t * lhs_t, const tree_t * rhs_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!lhs_t || !rhs_t) return NULL;
    IR_t * bb = pl_nalloc(cx, IR_BUILTIN); if (!bb) return NULL;
    IR_LIT(bb).sval = "is"; IR_LIT(bb).ival = 2;
    IR_t * laα = NULL, * laβ = NULL;
    IR_t * l = g_term(cx, lhs_t, NULL, ω_in, &laα, &laβ); if (!l) return NULL;
    IR_t * raα = NULL, * raβ = NULL;
    IR_t * r = g_arith_expr(cx, rhs_t, NULL, ω_in, &raα, &raβ); if (!r) return NULL;
    (void) laβ; (void) raβ;
    if (!ir_operand_push(bb, laα) || !ir_operand_push(bb, raα)) return NULL;
    pl_set_succ_fail(bb, γ_in, ω_in);
    return pl_ret(bb, α_out, β_out, bb, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_term(plcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) return NULL;
    switch (e->t) {
    case TT_ILIT: { IR_t * n = pl_nalloc(cx, IR_LIT_I); if (!n) return NULL; IR_LIT(n).ival = e->v.ival; return pl_emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FLIT: { IR_t * n = pl_nalloc(cx, IR_LIT_F); if (!n) return NULL; IR_LIT(n).dval = e->v.dval; return pl_emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_QLIT: case TT_NAME: { IR_t * n = pl_nalloc(cx, IR_ATOM); if (!n) return NULL; IR_LIT(n).sval = e->v.sval ? e->v.sval : "[]"; return pl_emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_VAR: { IR_t * n = pl_nalloc(cx, IR_LOGICVAR); if (!n) return NULL; int slot = (int)e->v.ival; IR_LIT(n).ival = slot; IR_LIT(n).sval = NULL; if (cx.pl_vars && slot + 1 > cx.pl_vars->count) cx.pl_vars->count = slot + 1; return pl_emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FNC: {
        IR_t * st = pl_nalloc(cx, IR_STRUCT); if (!st) return NULL;
        IR_LIT(st).sval = e->v.sval ? e->v.sval : "[]"; IR_LIT(st).ival = e->n;
        for (int i = 0; i < e->n; i++) {
            IR_t * cα = NULL, * cβ = NULL;
            IR_t * c = g_term(cx, e->c[i], NULL, NULL, &cα, &cβ); if (!c) return NULL;
            if (!ir_operand_push(st, cα)) return NULL;
        }
        pl_set_succ_fail(st, γ_in, ω_in);
        return pl_ret(st, α_out, β_out, st, ω_in);
    }
    case TT_MAKELIST: {
        int improper = (e->v.ival == 1);
        int nelem = e->n - (improper ? 1 : 0);
        if (nelem < 0) nelem = 0;
        IR_t * tail = NULL;
        if (improper) {
            IR_t * tα = NULL, * tβ = NULL;
            tail = g_term(cx, e->c[e->n - 1], NULL, NULL, &tα, &tβ);
            if (!tail) return NULL; (void) tβ; tail = tα;
        } else {
            tail = pl_nalloc(cx, IR_ATOM); if (!tail) return NULL; IR_LIT(tail).sval = "[]";
        }
        if (nelem == 0) return pl_emit_leaf(cx, tail, γ_in, ω_in, α_out, β_out);
        IR_t * suffix = tail;
        for (int i = nelem - 1; i >= 0; i--) {
            IR_t * hα = NULL, * hβ = NULL;
            IR_t * h = g_term(cx, e->c[i], NULL, NULL, &hα, &hβ); if (!h) return NULL; (void) hβ;
            IR_t * cell = pl_nalloc(cx, IR_STRUCT); if (!cell) return NULL;
            IR_LIT(cell).sval = "."; IR_LIT(cell).ival = 2;
            if (!ir_operand_push(cell, hα) || !ir_operand_push(cell, suffix)) return NULL; suffix = cell;
        }
        pl_set_succ_fail(suffix, γ_in, ω_in);
        return pl_ret(suffix, α_out, β_out, suffix, ω_in);
    }
    default:
        return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_builtin(plcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * bb = pl_nalloc(cx, IR_BUILTIN); if (!bb) return NULL;
    IR_LIT(bb).sval = fn; IR_LIT(bb).ival = e ? e->n : 0;
    if (e) for (int i = 0; i < e->n; i++) {
        IR_t * aα = NULL, * aβ = NULL;
        IR_t * a = g_term(cx, e->c[i], NULL, NULL, &aα, &aβ); if (!a) return NULL;
        if (!ir_operand_push(bb, aα)) return NULL;
    }
    pl_set_succ_fail(bb, γ_in, ω_in);
    return pl_ret(bb, α_out, β_out, bb, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_goal(plcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    int ar = e ? e->n : 0;
    IR_t * nd = pl_nalloc(cx, IR_GOAL); if (!nd) return NULL;
    bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc); if (!zc) return NULL;
    zc->callee = fn; zc->arity = ar; zc->nargs = ar; zc->cs = NULL; IR_LIT(nd).sval = fn;
    zc->args = ar > 0 ? (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *)) : NULL;
    for (int ai = 0; ai < ar; ai++) {
        if (!e->c[ai]) { zc->args[ai] = NULL; continue; }
        IR_t * aaα = NULL, * aaβ = NULL;
        g_term(cx, e->c[ai], NULL, NULL, &aaα, &aaβ);
        zc->args[ai] = aaα;
    }
    IR_LIT(nd).ival = (int64_t)(intptr_t)zc;
    pl_set_succ_fail(nd, γ_in, ω_in);
    return pl_ret(nd, α_out, β_out, nd, nd);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_ite(plcx_t cx, const tree_t * cond, const tree_t * then_, const tree_t * else_, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!cond || !then_) return NULL;
    IR_t * bα = NULL, * bβ = NULL, * b = NULL;
    if (else_) { b = pl_lower_goal(cx, else_, γ_in, ω_in, &bα, &bβ); if (!b) return NULL; }
    else { b = pl_nalloc(cx, IR_FAIL); if (!b) return NULL; b = pl_emit_leaf(cx, b, γ_in, ω_in, &bα, &bβ); if (!b) return NULL; }
    IR_t * tα = NULL, * tβ = NULL;
    IR_t * t = pl_lower_goal(cx, then_, γ_in, ω_in, &tα, &tβ); if (!t) return NULL; (void) tβ;
    bb_ite_state_t * zi = (bb_ite_state_t *)GC_MALLOC(sizeof *zi); if (!zi) return NULL;
    IR_t * cm = pl_nalloc(cx, IR_ITE_COMMIT); if (!cm) return NULL;
    IR_LIT(cm).ival = (int64_t)(intptr_t)zi; pl_set_succ_fail(cm, tα, ω_in);
    IR_t * gv = pl_nalloc(cx, IR_ITE_GATE); if (!gv) return NULL;
    IR_LIT(gv).ival = (int64_t)(intptr_t)zi; pl_set_succ_fail(gv, bα, ω_in);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = pl_lower_goal(cx, cond, cm, gv, &cα, &cβ); if (!c) return NULL; (void) cβ;
    IR_t * ite = pl_nalloc(cx, IR_ITE); if (!ite) return NULL;
    if (!ir_operand_push(ite, cα)) return NULL;
    zi->cond = cα; zi->then_ = tα; zi->else_ = bα; zi->then_root = t; zi->else_root = b; zi->cond_root = c; zi->cp_mark = NULL; zi->committed = 0; IR_LIT(ite).ival = (int64_t)(intptr_t)zi;
    pl_set_succ_fail(ite, γ_in, ω_in);
    return pl_ret(ite, α_out, β_out, ite, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_neg_goal(plcx_t cx, const tree_t * goal_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!goal_t) return NULL;
    IR_t * suc = pl_nalloc(cx, IR_SUCCEED); if (!suc) return NULL;
    IR_t * bα = NULL, * bβ = NULL; if (!pl_emit_leaf(cx, suc, γ_in, ω_in, &bα, &bβ)) return NULL;
    IR_t * fal = pl_nalloc(cx, IR_FAIL); if (!fal) return NULL;
    IR_t * tα = NULL, * tβ = NULL; if (!pl_emit_leaf(cx, fal, γ_in, ω_in, &tα, &tβ)) return NULL;
    bb_ite_state_t * zi = (bb_ite_state_t *)GC_MALLOC(sizeof *zi); if (!zi) return NULL;
    IR_t * cm = pl_nalloc(cx, IR_ITE_COMMIT); if (!cm) return NULL;
    IR_LIT(cm).ival = (int64_t)(intptr_t)zi; pl_set_succ_fail(cm, tα, ω_in);
    IR_t * gv = pl_nalloc(cx, IR_ITE_GATE); if (!gv) return NULL;
    IR_LIT(gv).ival = (int64_t)(intptr_t)zi; pl_set_succ_fail(gv, bα, ω_in);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = pl_lower_goal(cx, goal_t, cm, gv, &cα, &cβ); if (!c) return NULL; (void) cβ;
    IR_t * ite = pl_nalloc(cx, IR_ITE); if (!ite) return NULL;
    if (!ir_operand_push(ite, cα)) return NULL;
    zi->cond = cα; zi->then_ = tα; zi->else_ = bα; zi->then_root = fal; zi->else_root = suc; zi->cond_root = c; zi->cp_mark = NULL; zi->committed = 0; IR_LIT(ite).ival = (int64_t)(intptr_t)zi;
    pl_set_succ_fail(ite, γ_in, ω_in);
    return pl_ret(ite, α_out, β_out, ite, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_not_unify(plcx_t cx, const tree_t * A, const tree_t * B, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!A || !B) return NULL;
    IR_t * suc = pl_nalloc(cx, IR_SUCCEED); if (!suc) return NULL;
    IR_t * bα = NULL, * bβ = NULL; if (!pl_emit_leaf(cx, suc, γ_in, ω_in, &bα, &bβ)) return NULL;
    IR_t * fal = pl_nalloc(cx, IR_FAIL); if (!fal) return NULL;
    IR_t * tα = NULL, * tβ = NULL; if (!pl_emit_leaf(cx, fal, γ_in, ω_in, &tα, &tβ)) return NULL;
    bb_ite_state_t * zi = (bb_ite_state_t *)GC_MALLOC(sizeof *zi); if (!zi) return NULL;
    IR_t * cm = pl_nalloc(cx, IR_ITE_COMMIT); if (!cm) return NULL;
    IR_LIT(cm).ival = (int64_t)(intptr_t)zi; pl_set_succ_fail(cm, tα, ω_in);
    IR_t * gv = pl_nalloc(cx, IR_ITE_GATE); if (!gv) return NULL;
    IR_LIT(gv).ival = (int64_t)(intptr_t)zi; pl_set_succ_fail(gv, bα, ω_in);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = g_unify(cx, A, B, cm, gv, &cα, &cβ); if (!c) return NULL; (void) cβ;
    IR_t * ite = pl_nalloc(cx, IR_ITE); if (!ite) return NULL;
    if (!ir_operand_push(ite, cα)) return NULL;
    zi->cond = cα; zi->then_ = tα; zi->else_ = bα; zi->then_root = fal; zi->else_root = suc; zi->cond_root = c; zi->cp_mark = NULL; zi->committed = 0; IR_LIT(ite).ival = (int64_t)(intptr_t)zi;
    pl_set_succ_fail(ite, γ_in, ω_in);
    return pl_ret(ite, α_out, β_out, ite, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_catch(plcx_t cx, const tree_t * goal_t, const tree_t * catcher_t, const tree_t * rec_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!goal_t || !catcher_t || !rec_t) return NULL;
    IR_t * bb = pl_nalloc(cx, IR_CATCH); if (!bb) return NULL;
    bb_catch_state_t * zc = (bb_catch_state_t *)GC_MALLOC(sizeof *zc); if (!zc) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = g_term(cx, catcher_t, NULL, NULL, &cα, &cβ); if (!c) return NULL; (void) cβ;
    zc->catcher = cα;
    IR_graph_t * gcfg = IR_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
    plcx_t gx = cx; gx.bbg = gcfg;
    IR_t * gα = NULL, * gβ = NULL;
    IR_t * g = pl_lower_goal(gx, goal_t, NULL, NULL, &gα, &gβ); if (!g) return NULL; (void) gβ;
    gcfg->entry = gα ? gα : g;
    zc->goal_g = gcfg;
    IR_graph_t * rcfg = IR_alloc(128, IR_LANG_PL); if (!rcfg) return NULL;
    plcx_t rx = cx; rx.bbg = rcfg;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = pl_lower_goal(rx, rec_t, NULL, NULL, &rα, &rβ); if (!r) return NULL; (void) rβ;
    rcfg->entry = rα ? rα : r;
    zc->rec_g = rcfg;
    IR_LIT(bb).ival = (int64_t)(intptr_t)zc; bb->α = cα;
    pl_set_succ_fail(bb, γ_in, ω_in);
    return pl_ret(bb, α_out, β_out, bb, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_findall(plcx_t cx, const tree_t * tmpl_t, const tree_t * goal_t, const tree_t * result_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!tmpl_t || !goal_t || !result_t) return NULL;
    IR_t * bb = pl_nalloc(cx, IR_BUILTIN); if (!bb) return NULL;
    IR_LIT(bb).sval = "findall"; IR_LIT(bb).ival = 0;
    bb_findall_state_t * fs = (bb_findall_state_t *)GC_MALLOC(sizeof *fs); if (!fs) return NULL;
    IR_t * tα = NULL, * tβ = NULL;
    IR_t * t = g_term(cx, tmpl_t, NULL, NULL, &tα, &tβ); if (!t) return NULL; (void) tβ; fs->tmpl = tα;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_term(cx, result_t, NULL, NULL, &rα, &rβ); if (!r) return NULL; (void) rβ; fs->result = rα;
    IR_graph_t * gcfg = IR_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
    plcx_t gx = cx; gx.bbg = gcfg;
    IR_t * gα = NULL, * gβ = NULL;
    IR_t * g = pl_lower_goal(gx, goal_t, NULL, NULL, &gα, &gβ); if (!g) return NULL; (void) gβ;
    gcfg->entry = gα ? gα : g;
    fs->gcfg = gcfg; fs->goal_node = gα ? gα : g;
    IR_LIT(bb).ival = (int64_t)(intptr_t)fs;
    pl_set_succ_fail(bb, γ_in, ω_in);
    return pl_ret(bb, α_out, β_out, bb, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_phrase(plcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * rs   = (e->n >= 1) ? e->c[0] : NULL;
    const tree_t * inp  = (e->n >= 2) ? e->c[1] : NULL;
    const tree_t * rest = (e->n >= 3) ? e->c[2] : NULL;
    if (!rs || !inp) return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const char * callee = NULL; int orig_ar = 0; tree_t * const * orig_args = NULL;
    if      (rs->t == TT_QLIT || rs->t == TT_NAME) { callee = rs->v.sval; orig_ar = 0; }
    else if (rs->t == TT_FNC)                       { callee = rs->v.sval; orig_ar = rs->n; orig_args = rs->c; }
    else return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    if (!callee) return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    int ar = orig_ar + 2;
    IR_t * nd = pl_nalloc(cx, IR_GOAL); if (!nd) return NULL;
    bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc); if (!zc) return NULL;
    zc->callee = callee; zc->arity = ar; zc->nargs = ar; zc->cs = NULL; IR_LIT(nd).sval = callee;
    zc->args = (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *));
    for (int ai = 0; ai < orig_ar; ai++) {
        IR_t * aaα = NULL, * aaβ = NULL;
        if (orig_args[ai]) g_term(cx, orig_args[ai], NULL, NULL, &aaα, &aaβ);
        zc->args[ai] = aaα;
    }
    { IR_t * aaα = NULL, * aaβ = NULL; g_term(cx, inp, NULL, NULL, &aaα, &aaβ); zc->args[orig_ar] = aaα; }
    if (rest) { IR_t * aaα = NULL, * aaβ = NULL; g_term(cx, rest, NULL, NULL, &aaα, &aaβ); zc->args[orig_ar + 1] = aaα; }
    else { IR_t * nil = pl_nalloc(cx, IR_ATOM); if (!nil) return NULL; IR_LIT(nil).sval = "[]"; zc->args[orig_ar + 1] = nil; }
    IR_LIT(nd).ival = (int64_t)(intptr_t)zc;
    pl_set_succ_fail(nd, γ_in, ω_in);
    return pl_ret(nd, α_out, β_out, nd, nd);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pl_lower_goal(plcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) return NULL;
    switch (e->t) {
    case TT_CUT: return pl_emit_leaf(cx, pl_nalloc(cx, IR_CUT), γ_in, ω_in, α_out, β_out);
    case TT_QLIT: {
        const char * fn = e->v.sval;
        if (fn && (!strcmp(fn,"true")||!strcmp(fn,"otherwise"))) return pl_emit_leaf(cx, pl_nalloc(cx, IR_SUCCEED), γ_in, ω_in, α_out, β_out);
        if (fn && (!strcmp(fn,"fail")||!strcmp(fn,"false")))     return pl_emit_leaf(cx, pl_nalloc(cx, IR_FAIL), γ_in, ω_in, α_out, β_out);
        if (fn && !strcmp(fn,"nl")) return g_builtin(cx, "nl", NULL, γ_in, ω_in, α_out, β_out);
        if (fn) return g_goal(cx, fn, e, γ_in, ω_in, α_out, β_out);
        return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    case TT_UNIFY: {
        const tree_t * l = NULL, * r = NULL;
        if (!pl_tm(e, TT_UNIFY, 2, &l, &r)) return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        return g_unify(cx, l, r, γ_in, ω_in, α_out, β_out);
    }
    case TT_IF: {
        const tree_t * cond  = (e->n >= 1) ? e->c[0] : NULL;
        const tree_t * then_ = (e->n >= 2) ? e->c[1] : NULL;
        const tree_t * else_ = (e->n >= 3) ? e->c[2] : NULL;
        if (!cond || !then_) return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        return g_ite(cx, cond, then_, else_, γ_in, ω_in, α_out, β_out);
    }
    case TT_VAR:
        return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_PROGRAM: {
        if (e->n == 0) return pl_emit_leaf(cx, pl_nalloc(cx, IR_SUCCEED), γ_in, ω_in, α_out, β_out);
        if (e->n == 1) return pl_lower_goal(cx, e->c[0], γ_in, ω_in, α_out, β_out);
        if (e->n > 64) return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        return pl_wire_seq(cx, IR_GCONJ, (const tree_t * const *)e->c, e->n, γ_in, ω_in, α_out, β_out);
    }
    case TT_FNC: {
        const tree_t * A = NULL, * B = NULL, * arg = NULL;
        if (pl_tm_g(e, TT_FNC, "write",   1, &arg)) return g_builtin(cx, "write",   e, γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "writeln", 1, &arg)) return g_builtin(cx, "writeln", e, γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "print",   1, &arg)) return g_builtin(cx, "print",   e, γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, ",", 2, &A, &B)) {
            const tree_t * spine[64]; int sn = 0;
            const tree_t * cur = e;
            while (cur && cur->t == TT_FNC && cur->v.sval && !strcmp(cur->v.sval, ",") && cur->n == 2 && sn < 63) {
                spine[sn++] = cur->c[0]; cur = cur->c[1];
            }
            if (cur && sn < 64) spine[sn++] = cur;
            return pl_wire_seq(cx, IR_GCONJ, spine, sn, γ_in, ω_in, α_out, β_out);
        }
        if (pl_tm_g(e, TT_FNC, ";", 2, &A, &B)) {
            const tree_t * spine[64]; int sn = 0;
            const tree_t * cur = e;
            while (cur && cur->t == TT_FNC && cur->v.sval && !strcmp(cur->v.sval, ";") && cur->n == 2 && sn < 63) {
                spine[sn++] = cur->c[0]; cur = cur->c[1];
            }
            if (cur && sn < 64) spine[sn++] = cur;
            return pl_wire_alt(cx, IR_DISJ, spine, sn, γ_in, ω_in, α_out, β_out);
        }
        if (pl_tm_g(e, TT_FNC, "=",    2, &A, &B)) return g_unify(cx, A, B, γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "\\=",  2, &A, &B)) return g_not_unify(cx, A, B, γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "\\+",  1, &arg))   return g_neg_goal(cx, e->c[0], γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "not",  1, &arg))   return g_neg_goal(cx, e->c[0], γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "<",    2, &A, &B)) return g_compare(cx, A, B, "<",    γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, ">",    2, &A, &B)) return g_compare(cx, A, B, ">",    γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "=<",   2, &A, &B)) return g_compare(cx, A, B, "=<",   γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, ">=",   2, &A, &B)) return g_compare(cx, A, B, ">=",   γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "=:=",  2, &A, &B)) return g_compare(cx, A, B, "=:=",  γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "=\\=", 2, &A, &B)) return g_compare(cx, A, B, "=\\=", γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "is",   2, &A, &B)) return g_is(cx, A, B, γ_in, ω_in, α_out, β_out);
        if (e->v.sval && !strcmp(e->v.sval, "catch")   && e->n == 3) return g_catch(cx, e->c[0], e->c[1], e->c[2], γ_in, ω_in, α_out, β_out);
        if (e->v.sval && !strcmp(e->v.sval, "findall") && e->n == 3) return g_findall(cx, e->c[0], e->c[1], e->c[2], γ_in, ω_in, α_out, β_out);
        if (e->v.sval && !strcmp(e->v.sval, "phrase")  && (e->n == 2 || e->n == 3)) return g_phrase(cx, e, γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "==",   2, &A, &B)) return g_term_compare(cx, A, B, "==",   γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "\\==", 2, &A, &B)) return g_term_compare(cx, A, B, "\\==", γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "@<",   2, &A, &B)) return g_term_compare(cx, A, B, "@<",   γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "@>",   2, &A, &B)) return g_term_compare(cx, A, B, "@>",   γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "@=<",  2, &A, &B)) return g_term_compare(cx, A, B, "@=<",  γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "@>=",  2, &A, &B)) return g_term_compare(cx, A, B, "@>=",  γ_in, ω_in, α_out, β_out);
        if (pl_tm_g(e, TT_FNC, "succ", 2, &A, &B)) return g_term_compare(cx, A, B, "succ", γ_in, ω_in, α_out, β_out);
        {
            static const struct { const char * name; int arity; } det_builtins[] = {
                {"var",1},{"nonvar",1},{"atom",1},{"atomic",1},{"number",1},{"integer",1},
                {"float",1},{"compound",1},{"callable",1},{"is_list",1},{"ground",1},
                {"functor",3},{"arg",3},{"=..",2},
                {"atom_length",2},{"atom_concat",3},{"atom_chars",2},{"atom_codes",2},
                {"upcase_atom",2},{"downcase_atom",2},{"char_type",2},
                {"atom_string",2},{"atom_number",2},{"number_string",2},{"string_to_atom",2},
                {"string_concat",3},{"string_length",2},{"string_chars",2},{"string_codes",2},
                {"string_upper",2},{"string_lower",2},{"term_to_atom",2},{"term_string",2},
                {"atomic_list_concat",2},{"atomic_list_concat",3},{"concat_atom",2},{"concat_atom",3},
                {"sort",2},{"msort",2},
                {"format",1},{"format",2},{"numbervars",3},
                {"writeq",1},{"write_canonical",1},
                {"copy_term",2},{"plus",3},
                {"nb_setval",2},{"nb_getval",2},{"aggregate_all",3},
                {"retract",1},{"retractall",1},{"abolish",1},
                {"assertz",1},{"asserta",1},{"assert",1},
                {"throw",1},
            };
            const char * fn = e->v.sval; int ar = e->n;
            if (fn) for (size_t bi = 0; bi < sizeof det_builtins / sizeof det_builtins[0]; bi++) {
                if (ar == det_builtins[bi].arity && !strcmp(fn, det_builtins[bi].name))
                    return g_builtin(cx, det_builtins[bi].name, e, γ_in, ω_in, α_out, β_out);
            }
        }
        {
            const char * fn = e->v.sval; int ar = e->n;
            if (fn && ar >= 0) {
                IR_t * nd = pl_nalloc(cx, IR_GOAL); if (!nd) return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
                bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc); if (!zc) return NULL;
                zc->callee = fn; zc->arity = ar; zc->nargs = ar; zc->cs = NULL; IR_LIT(nd).sval = fn;
                zc->args = ar > 0 ? (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *)) : NULL;
                for (int ai = 0; ai < ar; ai++) {
                    if (!e->c[ai]) { zc->args[ai] = NULL; continue; }
                    IR_t * aaα = NULL, * aaβ = NULL;
                    g_term(cx, e->c[ai], NULL, NULL, &aaα, &aaβ);
                    zc->args[ai] = aaα;
                }
                IR_LIT(nd).ival = (int64_t)(intptr_t)zc;
                pl_set_succ_fail(nd, γ_in, ω_in);
                return pl_ret(nd, α_out, β_out, nd, nd);
            }
        }
        return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    default:
        return pl_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_goal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    plcx_t pcx;
    pcx.bbg = cx.bbg; pcx.role = PL_ROLE_GOAL; pcx.bounded = cx.bounded;
    pcx.lang = cx.lang; pcx.loop_ω = cx.loop_ω; pcx.loop_next = cx.loop_next; pcx.pl_vars = cx.pl_vars;
    return pl_lower_goal(pcx, e, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_goal_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    plcx_t cx; cx.bbg = bbg; cx.role = PL_ROLE_GOAL; cx.bounded = 0; cx.lang = bbg ? bbg->lang : 0; cx.loop_ω = NULL; cx.loop_next = NULL; cx.pl_vars = NULL;
    return pl_lower_goal(cx, e, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * g_head_unify(plcx_t cx, int slot, const tree_t * head_arg, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!head_arg) return NULL;
    IR_t * uni = pl_nalloc(cx, IR_UNIFY); if (!uni) return NULL;
    IR_t * lv = pl_nalloc(cx, IR_LOGICVAR); if (!lv) return NULL;
    IR_LIT(lv).ival = slot; IR_LIT(lv).sval = NULL;
    if (cx.pl_vars && slot + 1 > cx.pl_vars->count) cx.pl_vars->count = slot + 1;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_term(cx, head_arg, NULL, NULL, &rα, &rβ); if (!r) return NULL; (void) rβ;
    if (!ir_operand_push(uni, lv)) return NULL;
    if (!ir_operand_push(uni, rα)) return NULL;
    pl_set_succ_fail(uni, γ_in, ω_in);
    return pl_ret(uni, α_out, β_out, uni, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_clause_body_entry(IR_graph_t * bbg, const tree_t * clause, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    int arity = (int)clause->v.dval; if (arity < 0) arity = 0;
    int nbody = clause->n - arity; if (nbody < 0) nbody = 0;
    pl_vars_t pv; pv.count = 0;
    plcx_t cx; cx.bbg = bbg; cx.role = PL_ROLE_GOAL; cx.bounded = 0; cx.lang = 0; cx.loop_ω = NULL; cx.loop_next = NULL; cx.pl_vars = &pv;
    if (arity == 0 && nbody == 0) {
        IR_t * s = pl_nalloc(cx, IR_SUCCEED);
        if (bbg) { bbg->nslots = 0; bbg->body_root = s; }
        return pl_emit_leaf(cx, s, γ_in, ω_in, α_out, β_out);
    }
    int total = arity + nbody;
    if (total > 128) return NULL;
    IR_t * entry[128]; IR_t * resume[128]; IR_t * apply[128];
    IR_t * node = pl_nalloc(cx, IR_GCONJ); if (!node) return NULL;
    for (int idx = total - 1; idx >= 0; idx--) {
        IR_t * γi = (idx + 1 < total) ? entry[idx + 1] : node;
        IR_t * eα = NULL, * eβ = NULL; IR_t * top = NULL;
        if (idx < arity) top = g_head_unify(cx, idx, clause->c[idx], γi, ω_in, &eα, &eβ);
        else             top = pl_lower_goal(cx, clause->c[idx], γi, ω_in, &eα, &eβ);
        if (!top || !eα) return NULL;
        apply[idx] = top; entry[idx] = eα; resume[idx] = eβ;
    }
    for (int i = 1; i < total; i++) {
        IR_t * tgt = ω_in;
        for (int j = i - 1; j >= 0; j--) {
            if (resume[j] && resume[j] != ω_in) { tgt = resume[j]; break; }
        }
        apply[i]->ω = tgt;
    }
    {
        bb_conj_state_t * zs = (bb_conj_state_t *)GC_MALLOC(sizeof *zs);
        if (zs) {
            zs->goals = (IR_t **)GC_MALLOC((size_t)total * sizeof(IR_t *));
            if (zs->goals) { for (int i = 0; i < total; i++) zs->goals[i] = apply[i]; zs->ngoals = total; IR_LIT(node).ival = (int64_t)(intptr_t)zs; }
        }
    }
    pl_set_succ_fail(node, γ_in, ω_in);
    if (bbg) { bbg->nslots = pv.count; bbg->body_root = node; }
    return pl_ret(node, α_out, β_out, entry[0], resume[total - 1]);
}
