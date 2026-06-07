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
    IR_t * lit1 = nalloc(cx, IR_LIT_I); if (!lit1) return NULL; lit1->ival = 1;
    IR_t * lit0 = nalloc(cx, IR_LIT_I); if (!lit0) return NULL; lit0->ival = 0;
    IR_t * as1  = nalloc(cx, IR_ASSIGN); if (!as1) return NULL; as1->sval = tn;
    IR_t * as0  = nalloc(cx, IR_ASSIGN); if (!as0) return NULL; as0->sval = tn;
    IR_t * rd   = nalloc(cx, IR_VAR);    if (!rd)  return NULL; rd->sval  = tn;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cn = lower(cb, child, lit1, lit0, &cα, &cβ);
    if (!cn) return NULL;
    (void) cβ;
    lit1->γ = as1; lit0->γ = as0;
    as1->ω = ω_in; as0->ω = ω_in;
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
    IR_t * c1 = b1 ? rd1 : lower(cx, e->c[0], NULL, ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = b2 ? rd2 : lower(cx, e->c[1], bin, ω_in, &e2α, &e2β);
    if (!c2) return NULL;
    IR_t * x1entry = b1 ? rd1 : e1α;
    IR_t * x2entry = b2 ? rd2 : e2α;
    if (b2) rd2->γ = bin;
    if (b1) rd1->γ = x2entry; else if (!c1->γ) c1->γ = x2entry;
    IR_t * entry = x1entry;
    if (b2) { d2a1->γ = entry; d2a0->γ = entry; entry = d2e; }
    if (b1) { d1a1->γ = entry; d1a0->γ = entry; entry = d1e; }
    IR_t * binops[2] = { c1, c2 };
    bb_operand_aux_set(cx.bbg, bin, binops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return ret(bin, α_out, β_out, entry, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pas_leaf_node(lcx_t cx, IR_e kind, const char * name, long long iv) {
    IR_t * n = nalloc(cx, kind); if (!n) return NULL;
    if (kind == IR_VAR) n->sval = (char *) name; else n->ival = iv;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_binop_ll(lcx_t cx, int code, int is_rel, IR_t * op0, IR_t * op1, IR_t * γ_in, IR_t * ω_in) {
    if (!op0 || !op1) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP); if (!bin) return NULL;
    bin->ival = (int64_t) code; bin->dval = is_rel ? 1.0 : 0.0;
    set_succ_fail(op0, op1, ω_in); set_succ_fail(op1, bin, ω_in);
    IR_t * ops[2] = { op0, op1 }; bb_operand_aux_set(cx.bbg, bin, ops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return bin;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_binop_lt(lcx_t cx, int code, int is_rel, IR_t * op0, const tree_t * op1_t, IR_t * γ_in, IR_t * ω_in) {
    if (!op0 || !op1_t) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP); if (!bin) return NULL;
    bin->ival = (int64_t) code; bin->dval = is_rel ? 1.0 : 0.0;
    IR_t * p1α = NULL, * p1β = NULL;
    IR_t * p1 = lower(cx, op1_t, bin, ω_in, &p1α, &p1β); if (!p1) return NULL;
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
    IR_t * incr = nalloc(cb, IR_ASSIGN); if (!incr) return NULL; incr->sval = (char *) vname;
    IR_t * step_v = pas_leaf_node(cb, IR_VAR, vname, 0); IR_t * step_one = pas_leaf_node(cb, IR_LIT_I, NULL, 1);
    IR_t * step = pas_binop_ll(cb, down ? BINOP_SUB : BINOP_ADD, 0, step_v, step_one, incr, ω_in); if (!step) return NULL;
    set_succ_fail(incr, cond_entry, ω_in);
    IR_t * incr_entry = step_v;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * bnode = lower(cb, body, incr_entry, incr_entry, &bα, &bβ); if (!bnode) return NULL;
    if (!cond->γ) cond->γ = bα ? bα : bnode;
    IR_t * init = nalloc(cb, IR_ASSIGN); if (!init) return NULL; init->sval = (char *) vname;
    IR_t * fα = NULL, * fβ = NULL;
    IR_t * fnode = lower(cb, from, init, ω_in, &fα, &fβ); if (!fnode) return NULL;
    set_succ_fail(init, cond_entry, ω_in);
    return ret(init, α_out, β_out, fα ? fα : fnode, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_pascal_repeat(lcx_t cx, const tree_t * body_t, const tree_t * cond_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!body_t || !cond_t) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cond = lower(bounded(cx), cond_t, γ_in, NULL, &cα, &cβ);
    if (!cond) return NULL;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * body = lower(bounded(cx), body_t, cα, ω_in, &bα, &bβ);
    if (!body) return NULL;
    if (!cond->ω) cond->ω = bα;
    return ret(body, α_out, β_out, bα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_assign_fnc_lhs(lcx_t cx, const tree_t * lhs_t, const tree_t * rhs_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    as->sval = lhs_t->c[0]->v.sval;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower(cx, rhs_t, as, ω_in, &rα, &rβ);
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
    n->γ = landing; n->ω = ω_in;
    return ret(n, α_out, β_out, n, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_label_def(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * landing = bb_label_landing(e->v.sval);
    if (!landing) return NULL;
    IR_t * iα = NULL, * iβ = NULL;
    IR_t * inner = lower(cx, e->c[0], γ_in, ω_in, &iα, &iβ);
    if (!inner) return NULL;
    landing->γ = iα ? iα : inner;
    landing->ω = ω_in;
    return ret(landing, α_out, β_out, landing, iβ ? iβ : ω_in);
}
