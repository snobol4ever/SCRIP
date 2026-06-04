/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_internal.h"
#include "IR_interp_state.h"
#include <string.h>
#include <gc/gc.h>
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * g_term(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * g_builtin(lcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*====================================================================================================================*/
/*====================================================================================================================*/
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
    uni->α = lα;
    uni->β = rα;
    set_succ_fail(uni, γ_in, ω_in);
    return ret(uni, α_out, β_out, uni, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
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
    switch (e->t) {
    case TT_VAR: case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_NAME:
        return g_term(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return lower2(tv, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
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
    return ret(cmp, α_out, β_out, cmp, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_term_compare(lcx_t cx, const tree_t * l_t, const tree_t * r_t, const char * op_str, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!l_t || !r_t) return NULL;
    IR_t * cmp = nalloc(cx, IR_BUILTIN);
    if (!cmp) return NULL;
    cmp->sval = op_str; cmp->ival = 2;
    IR_t * lα = NULL, * lβ = NULL;
    IR_t * l = g_term(cx, l_t, NULL, NULL, &lα, &lβ);
    if (!l) return NULL;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_term(cx, r_t, NULL, NULL, &rα, &rβ);
    if (!r) return NULL;
    (void) lβ; (void) rβ;
    cmp->α = lα; cmp->β = rα;
    set_succ_fail(cmp, γ_in, ω_in);
    return ret(cmp, α_out, β_out, cmp, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
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
    return ret(bb, α_out, β_out, bb, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
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
            tail = nalloc(cx, IR_ATOM); if (!tail) return NULL; tail->sval = "[]";
        }
        if (nelem == 0) {
            return emit_leaf(cx, tail, γ_in, ω_in, α_out, β_out);
        }
        IR_t * suffix = tail;
        for (int i = nelem - 1; i >= 0; i--) {
            IR_t * hα = NULL, * hβ = NULL;
            IR_t * h = g_term(cx, e->c[i], NULL, NULL, &hα, &hβ);
            if (!h) return NULL; (void) hβ;
            IR_t * cell = nalloc(cx, IR_STRUCT); if (!cell) return NULL;
            cell->sval = "."; cell->ival = 2;
            cell->α = hα;
            hα->γ = suffix;
            suffix = cell;
        }
        set_succ_fail(suffix, γ_in, ω_in);
        return ret(suffix, α_out, β_out, suffix, ω_in);
    }
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
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
    return ret(bb, α_out, β_out, bb, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_goal(lcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    int ar = e ? e->n : 0;
    IR_t * nd = nalloc(cx, IR_GOAL); if (!nd) return NULL;
    bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc);
    if (!zc) return NULL;
    zc->callee = fn; zc->arity = ar; zc->nargs = ar; zc->cs = NULL;
    nd->sval = fn;
    zc->args = ar > 0 ? (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *)) : NULL;
    for (int ai = 0; ai < ar; ai++) {
        if (!e->c[ai]) { zc->args[ai] = NULL; continue; }
        IR_t * aaα = NULL, * aaβ = NULL;
        g_term(cx, e->c[ai], NULL, NULL, &aaα, &aaβ);
        zc->args[ai] = aaα;
    }
    nd->ival = (int64_t)(intptr_t)zc;
    set_succ_fail(nd, γ_in, ω_in);
    return ret(nd, α_out, β_out, nd, nd  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_ite(lcx_t cx, const tree_t * cond, const tree_t * then_, const tree_t * else_, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!cond || !then_) return NULL;
    IR_t * bα = NULL, * bβ = NULL, * b = NULL;
    if (else_) { b = lower_goal(cx, else_, γ_in, ω_in, &bα, &bβ); if (!b) return NULL; }
    else       { b = nalloc(cx, IR_FAIL); if (!b) return NULL; b = emit_leaf(cx, b, γ_in, ω_in, &bα, &bβ); if (!b) return NULL; }
    IR_t * tα = NULL, * tβ = NULL;
    IR_t * t = lower_goal(cx, then_, γ_in, ω_in, &tα, &tβ); if (!t) return NULL; (void) tβ;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = lower_goal(cx, cond, tα  , bα  , &cα, &cβ); if (!c) return NULL; (void) cβ;
    IR_t * ite = nalloc(cx, IR_ITE); if (!ite) return NULL;
    ite->α = cα;
    bb_ite_state_t * zi = (bb_ite_state_t *)GC_MALLOC(sizeof *zi);
    if (zi) { zi->cond = cα; zi->then_ = tα; zi->else_ = bα; zi->then_root = t; zi->else_root = b; zi->cond_root = c; ite->ival = (int64_t)(intptr_t)zi; }
    set_succ_fail(ite, γ_in, ω_in);
    return ret(ite, α_out, β_out, ite, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_neg_goal(lcx_t cx, const tree_t * goal_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!goal_t) return NULL;
    IR_t * suc = nalloc(cx, IR_SUCCEED); if (!suc) return NULL;
    IR_t * bα = NULL, * bβ = NULL; if (!emit_leaf(cx, suc, γ_in, ω_in, &bα, &bβ)) return NULL;
    IR_t * fal = nalloc(cx, IR_FAIL);  if (!fal) return NULL;
    IR_t * tα = NULL, * tβ = NULL; if (!emit_leaf(cx, fal, γ_in, ω_in, &tα, &tβ)) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = lower_goal(cx, goal_t, tα, bα, &cα, &cβ); if (!c) return NULL; (void) cβ;
    IR_t * ite = nalloc(cx, IR_ITE); if (!ite) return NULL;
    ite->α = cα;
    bb_ite_state_t * zi = (bb_ite_state_t *)GC_MALLOC(sizeof *zi);
    if (zi) { zi->cond = cα; zi->then_ = tα; zi->else_ = bα; zi->then_root = fal; zi->else_root = suc; zi->cond_root = c; ite->ival = (int64_t)(intptr_t)zi; }
    set_succ_fail(ite, γ_in, ω_in);
    return ret(ite, α_out, β_out, ite, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_not_unify(lcx_t cx, const tree_t * A, const tree_t * B, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!A || !B) return NULL;
    IR_t * suc = nalloc(cx, IR_SUCCEED); if (!suc) return NULL;
    IR_t * bα = NULL, * bβ = NULL; if (!emit_leaf(cx, suc, γ_in, ω_in, &bα, &bβ)) return NULL;
    IR_t * fal = nalloc(cx, IR_FAIL);  if (!fal) return NULL;
    IR_t * tα = NULL, * tβ = NULL; if (!emit_leaf(cx, fal, γ_in, ω_in, &tα, &tβ)) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = g_unify(cx, A, B, tα, bα, &cα, &cβ); if (!c) return NULL; (void) cβ;
    IR_t * ite = nalloc(cx, IR_ITE); if (!ite) return NULL;
    ite->α = cα;
    bb_ite_state_t * zi = (bb_ite_state_t *)GC_MALLOC(sizeof *zi);
    if (zi) { zi->cond = cα; zi->then_ = tα; zi->else_ = bα; zi->then_root = fal; zi->else_root = suc; zi->cond_root = c; ite->ival = (int64_t)(intptr_t)zi; }
    set_succ_fail(ite, γ_in, ω_in);
    return ret(ite, α_out, β_out, ite, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_catch(lcx_t cx, const tree_t * goal_t, const tree_t * catcher_t, const tree_t * rec_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!goal_t || !catcher_t || !rec_t) return NULL;
    IR_t * bb = nalloc(cx, IR_CATCH); if (!bb) return NULL;
    bb_catch_state_t * zc = (bb_catch_state_t *)GC_MALLOC(sizeof *zc); if (!zc) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * c = g_term(cx, catcher_t, NULL, NULL, &cα, &cβ); if (!c) return NULL; (void) cβ;
    zc->catcher = cα;
    IR_graph_t * gcfg = IR_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
    lcx_t gx = cx; gx.bbg = gcfg;
    IR_t * gα = NULL, * gβ = NULL;
    IR_t * g = lower_goal(gx, goal_t, NULL, NULL, &gα, &gβ); if (!g) return NULL; (void) gβ;
    gcfg->entry = gα ? gα : g;
    zc->goal_g = gcfg;
    IR_graph_t * rcfg = IR_alloc(128, IR_LANG_PL); if (!rcfg) return NULL;
    lcx_t rx = cx; rx.bbg = rcfg;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = lower_goal(rx, rec_t, NULL, NULL, &rα, &rβ); if (!r) return NULL; (void) rβ;
    rcfg->entry = rα ? rα : r;
    zc->rec_g = rcfg;
    bb->ival = (int64_t)(intptr_t)zc;
    bb->α = cα;
    set_succ_fail(bb, γ_in, ω_in);
    return ret(bb, α_out, β_out, bb, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_findall(lcx_t cx, const tree_t * tmpl_t, const tree_t * goal_t, const tree_t * result_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!tmpl_t || !goal_t || !result_t) return NULL;
    IR_t * bb = nalloc(cx, IR_BUILTIN); if (!bb) return NULL;
    bb->sval = "findall"; bb->ival = 0;
    bb_findall_state_t * fs = (bb_findall_state_t *)GC_MALLOC(sizeof *fs); if (!fs) return NULL;
    IR_t * tα = NULL, * tβ = NULL;
    IR_t * t = g_term(cx, tmpl_t, NULL, NULL, &tα, &tβ); if (!t) return NULL; (void) tβ; fs->tmpl = tα;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_term(cx, result_t, NULL, NULL, &rα, &rβ); if (!r) return NULL; (void) rβ; fs->result = rα;
    IR_graph_t * gcfg = IR_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
    lcx_t gx = cx; gx.bbg = gcfg;
    IR_t * gα = NULL, * gβ = NULL;
    IR_t * g = lower_goal(gx, goal_t, NULL, NULL, &gα, &gβ); if (!g) return NULL; (void) gβ;
    gcfg->entry = gα ? gα : g;
    fs->gcfg = gcfg;
    fs->goal_node = gα ? gα : g;
    bb->ival = (int64_t)(intptr_t)fs;
    set_succ_fail(bb, γ_in, ω_in);
    return ret(bb, α_out, β_out, bb, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * g_phrase(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * rs   = (e->n >= 1) ? e->c[0] : NULL;
    const tree_t * inp  = (e->n >= 2) ? e->c[1] : NULL;
    const tree_t * rest = (e->n >= 3) ? e->c[2] : NULL;
    if (!rs || !inp) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const char * callee = NULL; int orig_ar = 0; tree_t * const * orig_args = NULL;
    if (rs->t == TT_QLIT || rs->t == TT_NAME) { callee = rs->v.sval; orig_ar = 0; }
    else if (rs->t == TT_FNC)                  { callee = rs->v.sval; orig_ar = rs->n; orig_args = rs->c; }
    else return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    if (!callee) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    int ar = orig_ar + 2;
    IR_t * nd = nalloc(cx, IR_GOAL); if (!nd) return NULL;
    bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc); if (!zc) return NULL;
    zc->callee = callee; zc->arity = ar; zc->nargs = ar; zc->cs = NULL;
    nd->sval = callee;
    zc->args = (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *));
    for (int ai = 0; ai < orig_ar; ai++) {
        IR_t * aaα = NULL, * aaβ = NULL;
        if (orig_args[ai]) g_term(cx, orig_args[ai], NULL, NULL, &aaα, &aaβ);
        zc->args[ai] = aaα;
    }
    { IR_t * aaα = NULL, * aaβ = NULL; g_term(cx, inp, NULL, NULL, &aaα, &aaβ); zc->args[orig_ar] = aaα; }
    if (rest) { IR_t * aaα = NULL, * aaβ = NULL; g_term(cx, rest, NULL, NULL, &aaα, &aaβ); zc->args[orig_ar + 1] = aaα; }
    else      { IR_t * nil = nalloc(cx, IR_ATOM); if (!nil) return NULL; nil->sval = "[]"; zc->args[orig_ar + 1] = nil; }
    nd->ival = (int64_t)(intptr_t)zc;
    set_succ_fail(nd, γ_in, ω_in);
    return ret(nd, α_out, β_out, nd, nd  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_goal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    switch (e->t) {
    case TT_CUT: return emit_leaf(cx, nalloc(cx, IR_CUT), γ_in, ω_in, α_out, β_out);
    case TT_QLIT: {
        const char * fn = e->v.sval;
        if (fn && (!strcmp(fn,"true")||!strcmp(fn,"otherwise"))) return emit_leaf(cx, nalloc(cx, IR_SUCCEED), γ_in, ω_in, α_out, β_out);
        if (fn && (!strcmp(fn,"fail")||!strcmp(fn,"false")))     return emit_leaf(cx, nalloc(cx, IR_FAIL), γ_in, ω_in, α_out, β_out);
        if (fn && !strcmp(fn,"nl")) return g_builtin(cx, "nl", NULL, γ_in, ω_in, α_out, β_out);
        if (fn) return g_goal(cx, fn, e, γ_in, ω_in, α_out, β_out);
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    case TT_UNIFY: {
        const tree_t * l = NULL, * r = NULL;
        if (!tm(e, TT_UNIFY, 2, &l, &r)) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        return g_unify(cx, l, r, γ_in, ω_in, α_out, β_out);
    }
    case TT_IF: {
        const tree_t * cond  = (e->n >= 1) ? e->c[0] : NULL;
        const tree_t * then_ = (e->n >= 2) ? e->c[1] : NULL;
        const tree_t * else_ = (e->n >= 3) ? e->c[2] : NULL;
        if (!cond || !then_) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        return g_ite(cx, cond, then_, else_, γ_in, ω_in, α_out, β_out);
    }
    case TT_VAR:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_PROGRAM: {
        if (e->n == 0) return emit_leaf(cx, nalloc(cx, IR_SUCCEED), γ_in, ω_in, α_out, β_out);
        if (e->n == 1) return lower_goal(cx, e->c[0], γ_in, ω_in, α_out, β_out);
        if (e->n > 64) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        return wire_seq(cx, IR_GCONJ, (const tree_t * const *)e->c, e->n, γ_in, ω_in, α_out, β_out);
    }
    case TT_FNC: {
        const tree_t * A = NULL, * B = NULL, * arg = NULL;
        if (tm_g(e, TT_FNC, "write",   1, &arg)) return g_builtin(cx, "write",   e, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "writeln", 1, &arg)) return g_builtin(cx, "writeln", e, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "print",   1, &arg)) return g_builtin(cx, "print",   e, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, ",", 2, &A, &B)) {
            const tree_t * spine[64]; int sn = 0;
            const tree_t * cur = e;
            while (cur && cur->t == TT_FNC && cur->v.sval && !strcmp(cur->v.sval, ",") && cur->n == 2 && sn < 63) {
                spine[sn++] = cur->c[0]; cur = cur->c[1];
            }
            if (cur && sn < 64) spine[sn++] = cur;
            return wire_seq(cx, IR_GCONJ, spine, sn, γ_in, ω_in, α_out, β_out);
        }
        if (tm_g(e, TT_FNC, ";", 2, &A, &B)) {
            const tree_t * spine[64]; int sn = 0;
            const tree_t * cur = e;
            while (cur && cur->t == TT_FNC && cur->v.sval && !strcmp(cur->v.sval, ";") && cur->n == 2 && sn < 63) {
                spine[sn++] = cur->c[0]; cur = cur->c[1];
            }
            if (cur && sn < 64) spine[sn++] = cur;
            return wire_alt(cx, IR_DISJ, spine, sn, γ_in, ω_in, α_out, β_out);
        }
        if (tm_g(e, TT_FNC, "=",   2, &A, &B)) return g_unify(cx, A, B, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "\\=", 2, &A, &B)) return g_not_unify(cx, A, B, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "\\+", 1, &arg)) return g_neg_goal(cx, e->c[0], γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "not", 1, &arg)) return g_neg_goal(cx, e->c[0], γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "<",   2, &A, &B)) return g_compare(cx, A, B, "<",   γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, ">",   2, &A, &B)) return g_compare(cx, A, B, ">",   γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "=<",  2, &A, &B)) return g_compare(cx, A, B, "=<",  γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, ">=",  2, &A, &B)) return g_compare(cx, A, B, ">=",  γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "=:=", 2, &A, &B)) return g_compare(cx, A, B, "=:=", γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "=\\=",2, &A, &B)) return g_compare(cx, A, B, "=\\=",γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "is",  2, &A, &B)) return g_is(cx, A, B, γ_in, ω_in, α_out, β_out);
        if (e->t == TT_FNC && e->v.sval && !strcmp(e->v.sval, "catch") && e->n == 3)
            return g_catch(cx, e->c[0], e->c[1], e->c[2], γ_in, ω_in, α_out, β_out);
        if (e->t == TT_FNC && e->v.sval && !strcmp(e->v.sval, "findall") && e->n == 3)
            return g_findall(cx, e->c[0], e->c[1], e->c[2], γ_in, ω_in, α_out, β_out);
        if (e->t == TT_FNC && e->v.sval && !strcmp(e->v.sval, "phrase") && (e->n == 2 || e->n == 3))
            return g_phrase(cx, e, γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "==",  2, &A, &B)) return g_term_compare(cx, A, B, "==",  γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "\\==",2, &A, &B)) return g_term_compare(cx, A, B, "\\==",γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "@<",  2, &A, &B)) return g_term_compare(cx, A, B, "@<",  γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "@>",  2, &A, &B)) return g_term_compare(cx, A, B, "@>",  γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "@=<", 2, &A, &B)) return g_term_compare(cx, A, B, "@=<", γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "@>=", 2, &A, &B)) return g_term_compare(cx, A, B, "@>=", γ_in, ω_in, α_out, β_out);
        if (tm_g(e, TT_FNC, "succ", 2, &A, &B)) return g_term_compare(cx, A, B, "succ", γ_in, ω_in, α_out, β_out);
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
        { const char * fn = e->v.sval; int ar = e->n;
          if (fn && ar >= 0) {
              IR_t * nd = nalloc(cx, IR_GOAL); if (!nd) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
              bb_goal_state_t * zc = (bb_goal_state_t *)GC_MALLOC(sizeof *zc);
              if (!zc) return NULL;
              zc->callee = fn; zc->arity = ar; zc->nargs = ar; zc->cs = NULL;
              nd->sval = fn;
              zc->args = ar > 0 ? (IR_t **)GC_MALLOC((size_t)ar * sizeof(IR_t *)) : NULL;
              for (int ai = 0; ai < ar; ai++) {
                  if (!e->c[ai]) { zc->args[ai] = NULL; continue; }
                  IR_t * aaα = NULL, * aaβ = NULL;
                  g_term(cx, e->c[ai], NULL, NULL, &aaα, &aaβ);
                  zc->args[ai] = aaα;
              }
              nd->ival = (int64_t)(intptr_t)zc;
              set_succ_fail(nd, γ_in, ω_in);
              return ret(nd, α_out, β_out, nd, nd  );
          }
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_goal_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_GOAL, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * g_head_unify(lcx_t cx, int slot, const tree_t * head_arg, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!head_arg) return NULL;
    IR_t * uni = nalloc(cx, IR_UNIFY); if (!uni) return NULL;
    IR_t * lv = nalloc(cx, IR_LOGICVAR); if (!lv) return NULL;
    lv->ival = slot; lv->sval = NULL;
    if (cx.pl_vars && slot + 1 > cx.pl_vars->count) cx.pl_vars->count = slot + 1;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * r = g_term(cx, head_arg, NULL, NULL, &rα, &rβ);
    if (!r) return NULL; (void) rβ;
    uni->α = lv;
    uni->β = rα;
    set_succ_fail(uni, γ_in, ω_in);
    return ret(uni, α_out, β_out, uni, ω_in  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_clause_body_entry(IR_graph_t * bbg, const tree_t * clause, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    int arity = (int) clause->v.dval;
    if (arity < 0) arity = 0;
    int nbody = clause->n - arity;
    if (nbody < 0) nbody = 0;
    pl_vars_t pv; pv.count = 0;
    lcx_t cx = { bbg, ROLE_GOAL, 0, 0 };
    cx.pl_vars = &pv;
    if (arity == 0 && nbody == 0) {
        IR_t * s = nalloc(cx, IR_SUCCEED);
        if (bbg) { bbg->nslots = 0; bbg->body_root = s; }
        return emit_leaf(cx, s, γ_in, ω_in, α_out, β_out);
    }
    int total = arity + nbody;
    if (total > 128) return NULL;
    IR_t * entry[128]; IR_t * resume[128]; IR_t * apply[128];
    IR_t * node = nalloc(cx, IR_GCONJ); if (!node) return NULL;
    for (int idx = total - 1; idx >= 0; idx--) {
        IR_t * γi = (idx + 1 < total) ? entry[idx + 1] : node;
        IR_t * eα = NULL, * eβ = NULL; IR_t * top = NULL;
        if (idx < arity) top = g_head_unify(cx, idx, clause->c[idx], γi, ω_in, &eα, &eβ);
        else             top = lower2(cx, clause->c[idx], γi, ω_in, &eα, &eβ);
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
            if (zs->goals) { for (int i = 0; i < total; i++) zs->goals[i] = apply[i]; zs->ngoals = total; node->ival = (int64_t)(intptr_t)zs; }
        }
    }
    set_succ_fail(node, γ_in, ω_in);
    if (bbg) { bbg->nslots = pv.count; bbg->body_root = node; }
    return ret(node, α_out, β_out, entry[0], resume[total - 1]);
}

