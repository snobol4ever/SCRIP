/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_internal.h"
#include "IR_interp_state.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc/gc.h>
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pas_bool_diamond(lcx_t cx, const tree_t * child, IR_t * ω_in, IR_t ** entry_out, IR_t ** as1_out, IR_t ** as0_out) {
    static int g_pas_bool_tmp = 0;
    char * tn = (char *) GC_malloc(16); if (!tn) return NULL;
    snprintf(tn, 16, "__pbt%d", g_pas_bool_tmp++);
    IR_t * lit1 = nalloc(cx, IR_LIT_I); if (!lit1) return NULL; IR_LIT(lit1).ival = 1;
    IR_t * lit0 = nalloc(cx, IR_LIT_I); if (!lit0) return NULL; IR_LIT(lit0).ival = 0;
    IR_t * as1  = nalloc(cx, IR_ASSIGN); if (!as1) return NULL; IR_LIT(as1).sval = tn;
    IR_t * as0  = nalloc(cx, IR_ASSIGN); if (!as0) return NULL; IR_LIT(as0).sval = tn;
    IR_t * rd   = nalloc(cx, IR_VAR);    if (!rd)  return NULL; IR_LIT(rd).sval  = tn;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cn = lower_program(cb, child, lit1, lit0, &cα, &cβ);
    if (!cn) return NULL;
    (void) cβ;
    lit1->γ.node = as1; memcpy(lit1->γ.sz, "α", 3); lit0->γ.node = as0; memcpy(lit0->γ.sz, "α", 3);
    as1->ω.node = ω_in; memcpy(as1->ω.sz, "α", 3); as0->ω.node = ω_in; memcpy(as0->ω.sz, "α", 3);
    if (entry_out) *entry_out = cα ? cα : cn;
    if (as1_out) *as1_out = as1;
    if (as0_out) *as0_out = as0;
    return rd;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * pas_binop_bool(lcx_t cx, const tree_t * e, IR_t * bin, int b1, int b2, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * d1e=NULL,*d1a1=NULL,*d1a0=NULL,*rd1=NULL, * d2e=NULL,*d2a1=NULL,*d2a0=NULL,*rd2=NULL;
    if (b1) { rd1 = pas_bool_diamond(cx, e->c[0], ω_in, &d1e, &d1a1, &d1a0); if (!rd1) return NULL; }
    if (b2) { rd2 = pas_bool_diamond(cx, e->c[1], ω_in, &d2e, &d2a1, &d2a0); if (!rd2) return NULL; }
    IR_t * e1α=NULL,*e1β=NULL,*e2α=NULL,*e2β=NULL;
    IR_t * c1 = b1 ? rd1 : lower_program(cx, e->c[0], NULL, ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = b2 ? rd2 : lower_program(cx, e->c[1], bin, ω_in, &e2α, &e2β);
    if (!c2) return NULL;
    IR_t * x1entry = b1 ? rd1 : e1α;
    IR_t * x2entry = b2 ? rd2 : e2α;
    if (b2) { rd2->γ.node = bin; memcpy(rd2->γ.sz, "α", 3); }
    if (b1) { rd1->γ.node = x2entry; memcpy(rd1->γ.sz, "α", 3); } else if (!c1->γ.node) { c1->γ.node = x2entry; memcpy(c1->γ.sz, "α", 3); }
    IR_t * entry = x1entry;
    if (b2) { d2a1->γ.node = entry; memcpy(d2a1->γ.sz, "α", 3); d2a0->γ.node = entry; memcpy(d2a0->γ.sz, "α", 3); entry = d2e; }
    if (b1) { d1a1->γ.node = entry; memcpy(d1a1->γ.sz, "α", 3); d1a0->γ.node = entry; memcpy(d1a0->γ.sz, "α", 3); entry = d1e; }
    IR_t * binops[2] = { c1, c2 };
    bb_operand_aux_set(cx.bbg, bin, binops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return ret(bin, α_out, β_out, entry, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pas_leaf_node(lcx_t cx, IR_e kind, const char * name, long long iv) {
    IR_t * n = nalloc(cx, kind); if (!n) return NULL;
    if (kind == IR_VAR) IR_LIT(n).sval = (char *) name; else IR_LIT(n).ival = iv;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_binop_ll(lcx_t cx, int code, int is_rel, IR_t * op0, IR_t * op1, IR_t * γ_in, IR_t * ω_in) {
    if (!op0 || !op1) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP); if (!bin) return NULL;
    IR_LIT(bin).ival = (int64_t) code; IR_LIT(bin).dval = is_rel ? 1.0 : 0.0;
    set_succ_fail(op0, op1, ω_in); set_succ_fail(op1, bin, ω_in);
    IR_t * ops[2] = { op0, op1 }; bb_operand_aux_set(cx.bbg, bin, ops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return bin;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_binop_lt(lcx_t cx, int code, int is_rel, IR_t * op0, const tree_t * op1_t, IR_t * γ_in, IR_t * ω_in) {
    if (!op0 || !op1_t) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP); if (!bin) return NULL;
    IR_LIT(bin).ival = (int64_t) code; IR_LIT(bin).dval = is_rel ? 1.0 : 0.0;
    IR_t * p1α = NULL, * p1β = NULL;
    IR_t * p1 = lower_program(cx, op1_t, bin, ω_in, &p1α, &p1β); if (!p1) return NULL;
    set_succ_fail(op0, p1α, ω_in);
    IR_t * ops[2] = { op0, p1 }; bb_operand_aux_set(cx.bbg, bin, ops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return bin;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * v_pascal_for(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e || e->n < 4 || !e->c[0] || e->c[0]->t != TT_VAR || !e->c[0]->v.sval) return NULL;
    int down = (e->v.ival == 1);
    const char * vname = e->c[0]->v.sval;
    const tree_t * from = e->c[1]; const tree_t * to = e->c[2]; const tree_t * body = e->c[3];
    lcx_t cb = bounded(cx);
    IR_t * cond_v = pas_leaf_node(cb, IR_VAR, vname, 0); if (!cond_v) return NULL;
    IR_t * cond = pas_binop_lt(cb, down ? BINOP_GE : BINOP_LE, 1, cond_v, to, NULL, γ_in); if (!cond) return NULL;
    IR_t * cond_entry = cond_v;
    IR_t * incr = nalloc(cb, IR_ASSIGN); if (!incr) return NULL; IR_LIT(incr).sval = (char *) vname;
    IR_t * step_v = pas_leaf_node(cb, IR_VAR, vname, 0); IR_t * step_one = pas_leaf_node(cb, IR_LIT_I, NULL, 1);
    IR_t * step = pas_binop_ll(cb, down ? BINOP_SUB : BINOP_ADD, 0, step_v, step_one, incr, ω_in); if (!step) return NULL;
    set_succ_fail(incr, cond_entry, ω_in);
    IR_t * incr_entry = step_v;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * bnode = lower_program(cb, body, incr_entry, incr_entry, &bα, &bβ); if (!bnode) return NULL;
    if (!cond->γ.node) { cond->γ.node = bα ? bα : bnode; memcpy(cond->γ.sz, "α", 3); }
    IR_t * init = nalloc(cb, IR_ASSIGN); if (!init) return NULL; IR_LIT(init).sval = (char *) vname;
    IR_t * fα = NULL, * fβ = NULL;
    IR_t * fnode = lower_program(cb, from, init, ω_in, &fα, &fβ); if (!fnode) return NULL;
    set_succ_fail(init, cond_entry, ω_in);
    return ret(init, α_out, β_out, fα ? fα : fnode, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_pascal_repeat(lcx_t cx, const tree_t * body_t, const tree_t * cond_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!body_t || !cond_t) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cond = lower_program(bounded(cx), cond_t, γ_in, NULL, &cα, &cβ);
    if (!cond) return NULL;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * body = lower_program(bounded(cx), body_t, cα, ω_in, &bα, &bβ);
    if (!body) return NULL;
    if (!cond->ω.node) { cond->ω.node = bα; memcpy(cond->ω.sz, "α", 3); }
    return ret(body, α_out, β_out, bα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_assign_fnc_lhs(lcx_t cx, const tree_t * lhs_t, const tree_t * rhs_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = lhs_t->c[0]->v.sval;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower_program(cx, rhs_t, as, ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    set_succ_fail(as, γ_in, ω_in);
    return ret(as, α_out, β_out, rα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_goto_u(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in;
    IR_t * landing = bb_label_landing(e->v.sval);
    if (!landing) return NULL;
    IR_t * n = nalloc(cx, IR_SUCCEED);
    if (!n) return NULL;
    n->γ.node = landing; memcpy(n->γ.sz, "α", 3); n->ω.node = ω_in; memcpy(n->ω.sz, "α", 3);
    return ret(n, α_out, β_out, n, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_label_def(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * landing = bb_label_landing(e->v.sval);
    if (!landing) return NULL;
    IR_t * iα = NULL, * iβ = NULL;
    IR_t * inner = lower_program(cx, e->c[0], γ_in, ω_in, &iα, &iβ);
    if (!inner) return NULL;
    landing->γ.node = iα ? iα : inner; memcpy(landing->γ.sz, "α", 3);
    landing->ω.node = ω_in; memcpy(landing->ω.sz, "α", 3);
    return ret(landing, α_out, β_out, landing, iβ ? iβ : ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * lower_pas_value(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_t ** α, IR_t ** β) {
    switch (e->t) {
    case TT_IF:
        return wire_if(cx, e, 1, γ, ω, α, β);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE: {
        if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
        int b1 = tt_is_relational(e->c[0]->t);
        int b2 = tt_is_relational(e->c[1]->t);
        if (!b1 && !b2) return lower_value_shared(cx, e, γ, ω, α, β);
        IR_t * bin = nalloc(cx, IR_BINOP);
        if (!bin) return NULL;
        IR_LIT(bin).sval = e->v.sval;
        IR_LIT(bin).ival = (int64_t) tt_to_binop(e->t);
        IR_LIT(bin).dval = tt_is_relational(e->t) ? 1.0 : 0.0;
        return pas_binop_bool(cx, e, bin, b1, b2, γ, ω, α, β);
    }
    case TT_GOTO_U:
        if (!e->v.sval) return NULL;
        return pas_goto_u(cx, e, γ, ω, α, β);
    case TT_LABEL_DEF:
        if (e->n < 1 || !e->c[0] || !e->v.sval) return NULL;
        return pas_label_def(cx, e, γ, ω, α, β);
    case TT_REPEAT:
        if (e->n >= 2 && e->c[0] && e->c[1]) return v_pascal_repeat(cx, e->c[0], e->c[1], γ, ω, α, β);
        return lower_value_shared(cx, e, γ, ω, α, β);
    case TT_FNC: case TT_PROC_FAIL: case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP: case TT_LIMIT: case TT_CASE: {
        if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return wire_det_builtin1(cx, e->c[1], fn, γ, ω, α, β);
        }
        if (e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval)
            return v_det_call(cx, e, 0, γ, ω, α, β);
        return lower_unhandled(cx, e, γ, ω, α, β);
    }
    case TT_IDX:
        if (e->n >= 2 && e->c[0] && e->c[1]) { const tree_t * k[2] = { e->c[0], e->c[1] }; return v_raku_det_call(cx, "arr_get", k, 2, γ, ω, α, β); }
        return lower_unhandled(cx, e, γ, ω, α, β);
    case TT_FOR:
        return v_pascal_for(cx, e, γ, ω, α, β);
    case TT_ASSIGN: {
        const tree_t * lhs_t = NULL, * rhs_t = NULL;
        if (!tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
        if (!lhs_t || !rhs_t) return lower_unhandled(cx, e, γ, ω, α, β);
        if (lhs_t->t == TT_IDX && lhs_t->n >= 2 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval && lhs_t->c[1]) {
            const tree_t * k[3] = { lhs_t->c[0], lhs_t->c[1], rhs_t };
            return v_raku_mutate_writeback(cx, lhs_t->c[0]->v.sval, "arr_set_pure", k, 3, γ, ω, α, β);
        }
        if (lhs_t->t == TT_FNC && lhs_t->n >= 2 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval && !strcmp(lhs_t->c[0]->v.sval, "__pas_deref")) {
            const tree_t * k[2] = { lhs_t->c[1], rhs_t };
            return v_raku_det_call(cx, "__pas_deref_set", k, 2, γ, ω, α, β);
        }
        if (lhs_t->t == TT_FNC && lhs_t->n >= 1 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval)
            return pas_assign_fnc_lhs(cx, lhs_t, rhs_t, γ, ω, α, β);
        return lower_value_shared(cx, e, γ, ω, α, β);
    }
    default:
        return lower_value_shared(cx, e, γ, ω, α, β);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_pas(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β) {
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * r = lower_pas_value(cx, e, γ, ω, &aα, &aβ);
    return iref(r, α, β, aα, aβ);
}
