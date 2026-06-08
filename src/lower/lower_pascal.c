/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_internal.h"
#include "IR_interp_state.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <gc/gc.h>
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pas_lower(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * pas_iref(IR_t * n, IR_ref_t * α, IR_ref_t * β, IR_t * na, IR_t * nb);
IR_t * pas_wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
int pas_flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap);
int pas_tm(const tree_t * e, tree_e kind, int nargs, ...);
int pas_tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...);
int pas_kind_is_resumable(IR_e t);
IR_t * pas_emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_literal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_unop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
int pas_tt_is_relational(tree_e t);
int pas_tt_to_binop(tree_e t);
IR_t * pas_v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static int pas_to_by_const_step(const tree_t * s, int64_t * out_bits, int * is_real);
IR_t * pas_v_to(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_wire_if(lcx_t cx, const tree_t * e, int else_succeeds, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_graph_t * pas_value_subgraph(lcx_t cx, const tree_t * e);
IR_t * pas_v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_every(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_until(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_repeat(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_loop_break(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_loop_next(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_not(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_value_shared(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_raku_det_call(lcx_t cx, const char * fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_raku_mutate_writeback(lcx_t cx, const char * target, const char * pure_fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * pas_bool_diamond(lcx_t cx, const tree_t * child, IR_t * ω_in, IR_t ** entry_out, IR_t ** as1_out, IR_t ** as0_out);
IR_t * pas_binop_bool(lcx_t cx, const tree_t * e, IR_t * bin, int b1, int b2, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * pas_leaf_node(lcx_t cx, IR_e kind, const char * name, long long iv);
static IR_t * pas_binop_ll(lcx_t cx, int code, int is_rel, IR_t * op0, IR_t * op1, IR_t * γ_in, IR_t * ω_in);
static IR_t * pas_binop_lt(lcx_t cx, int code, int is_rel, IR_t * op0, const tree_t * op1_t, IR_t * γ_in, IR_t * ω_in);
IR_t * pas_v_pascal_for(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_v_pascal_repeat(lcx_t cx, const tree_t * body_t, const tree_t * cond_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_assign_fnc_lhs(lcx_t cx, const tree_t * lhs_t, const tree_t * rhs_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_goto_u(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
IR_t * pas_label_def(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * lower_pas_value(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_t ** α, IR_t ** β);
IR_t * lower_pas(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β);
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pas_nalloc(lcx_t cx, IR_e kind) { return IR_node_alloc(cx.bbg, kind); }
/*--------------------------------------------------------------------------------------------------------------------*/
static void pas_set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in) {
    if (!n) return;
    if (!n->γ.node && γ_in) { n->γ.node = γ_in; memcpy(n->γ.sz, "α", 3); }
    if (!n->ω.node && ω_in) { n->ω.node = ω_in; memcpy(n->ω.sz, "α", 3); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β) {
    if (α_out) *α_out = α;
    if (β_out) *β_out = β;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_iref(IR_t * n, IR_ref_t * α, IR_ref_t * β, IR_t * na, IR_t * nb) {
    if (α) { α->node = na; memcpy(α->sz, "α", 3); }
    if (β) { β->node = nb; memcpy(β->sz, "β", 3); }
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) ω_in;
    fprintf(stderr, "[lower] UNHANDLED role=%d kind=%d\n", (int)cx.role, e ? (int)e->t : -1);
    return pas_ret(NULL, α_out, β_out, NULL, NULL);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_lower(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) { if (α_out) *α_out = γ_in; if (β_out) *β_out = ω_in; return NULL; }
    return lower_pas_value(cx, e, γ_in, ω_in, α_out, β_out);
}
IR_t * pas_wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = pas_nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    for (int i = nkids - 1; i >= 0; i--) {
        if (!kids[i]) return NULL;
        IR_t * γi = (i + 1 < nkids) ? entry[i + 1] : node;
        IR_t * αi = NULL, * βi = NULL;
        IR_t * c = pas_lower(cx, kids[i], γi, ω_in  , &αi, &βi);
        if (!c) return NULL;
        apply[i] = c; entry[i] = αi ? αi : c; resume[i] = βi;
    }
    for (int i = 1; i < nkids; i++) {
        IR_t * tgt = ω_in;
        for (int j = i - 1; j >= 0; j--) {
            if (resume[j] && resume[j] != ω_in) { tgt = resume[j]; break; }
        }
        apply[i]->ω.node = tgt; memcpy(apply[i]->ω.sz, "α", 3);
    }
    if (kind == IR_GCONJ) {
        bb_conj_state_t * zs = (bb_conj_state_t *)GC_MALLOC(sizeof *zs);
        if (zs) {
            zs->goals = (IR_t **)GC_MALLOC((size_t)nkids * sizeof(IR_t *));
            if (zs->goals) { for (int i = 0; i < nkids; i++) zs->goals[i] = apply[i]; zs->ngoals = nkids; IR_LIT(node).ival = (int64_t)(intptr_t)zs; }
        }
    }
    pas_set_succ_fail(node, γ_in, ω_in);
    return pas_ret(node, α_out, β_out, entry[0], resume[nkids - 1]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * pas_wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = pas_nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    IR_t * arm_succ = (kind == IR_DISJ) ? γ_in : node;
    for (int j = nkids - 1; j >= 0; j--) {
        if (!kids[j]) return NULL;
        IR_t * ωj = (j + 1 < nkids) ? entry[j + 1] : ω_in;
        IR_t * αj = NULL, * βj = NULL;
        IR_t * arm = pas_lower(cx, kids[j], arm_succ, ωj, &αj, &βj);
        if (!arm) return NULL;
        if (!arm->γ.node) { arm->γ.node = arm_succ; memcpy(arm->γ.sz, "α", 3); }
        apply[j] = arm; entry[j] = αj ? αj : arm; resume[j] = βj;
    }
    bb_operand_aux_set(cx.bbg, node, apply, nkids);
    pas_set_succ_fail(node, γ_in, ω_in);
    return pas_ret(node, α_out, β_out, entry[0], node  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
int pas_flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap) {
    int n = 0;
    if (!e) return 0;
    if (e->t == kind) {
        for (int i = 0; i < e->n; i++) { int got = pas_flatten_seq(e->c[i], kind, out + n, cap - n); n += got; if (n >= cap) break; }
        return n;
    }
    if (n < cap) out[n++] = e;
    return n;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
int pas_tm(const tree_t * e, tree_e kind, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int pas_tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    if (tag && (!e->v.sval || strcmp(e->v.sval, tag) != 0)) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
int pas_kind_is_resumable(IR_e t) {
    return t == IR_TO || t == IR_TO_BY || t == IR_UPTO || t == IR_ALT || t == IR_BINOP_GEN || t == IR_ITERATE || t == IR_LIMIT || t == IR_PROC_GEN ||
           t == IR_EVERY || t == IR_REPEAT || t == IR_SUSPEND || t == IR_SCAN || t == IR_LIST_BANG || t == IR_KEY_GEN || t == IR_FIND_GEN || t == IR_SEQ_GEN || t == IR_GATHER ||
           t == IR_GEN_SCAN || t == IR_CONJ || t == IR_MAP || t == IR_GREP ||
           t == IR_PAT_LIT || t == IR_PAT_ARB || t == IR_PAT_REM || t == IR_PAT_SPAN || t == IR_PAT_SPAN_VAR || t == IR_PAT_ANY || t == IR_PAT_NOTANY ||
           t == IR_PAT_BREAK || t == IR_PAT_BREAKX || t == IR_PAT_LEN || t == IR_PAT_TAB || t == IR_PAT_RTAB || t == IR_PAT_ARBNO || t == IR_PAT_DEFER ||
           t == IR_PAT_BAL ||
           t == IR_PAT_ASSIGN_COND || t == IR_PAT_ASSIGN_IMM || t == IR_PAT_ATP;
}
IR_t * pas_emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!n) return NULL;
    pas_set_succ_fail(n, γ_in, ω_in);
    IR_t * resume = (!cx.bounded && pas_kind_is_resumable(n->op)) ? n : ω_in;
    return pas_ret(n, α_out, β_out, n, resume);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_literal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_ILIT:    n = pas_nalloc(cx, IR_LIT_I);   if (n) IR_LIT(n).ival = e->v.ival; break;
    case TT_FLIT:    n = pas_nalloc(cx, IR_LIT_F);   if (n) IR_LIT(n).dval = e->v.dval; break;
    case TT_QLIT:    n = pas_nalloc(cx, IR_LIT_S);   if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; break;
    case TT_CSET:    n = pas_nalloc(cx, IR_LIT_S);   if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; break;
    case TT_NUL: case TT_NULL: n = pas_nalloc(cx, IR_LIT_NUL); break;
    case TT_VAR:     n = pas_nalloc(cx, IR_VAR);     if (n) IR_LIT(n).sval = e->v.sval; break;
    case TT_NAME:    n = pas_nalloc(cx, IR_VAR);     if (n) IR_LIT(n).sval = e->v.sval; break;
    case TT_KEYWORD: n = pas_nalloc(cx, IR_KEYWORD); if (n) IR_LIT(n).sval = e->v.sval; break;
    default: return NULL;
    }
    return pas_emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_unop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    if (e->t == TT_ITERATE) {
        IR_t * bang = pas_nalloc(cx, IR_LIST_BANG);
        if (!bang) return NULL;
        IR_t * oα = NULL, * oβ = NULL;
        IR_t * operand = pas_lower(cx, e->c[0], NULL, ω_in, &oα, &oβ);
        if (!operand) return NULL;
        if (!ir_operand_push(bang, oα ? oα : operand)) return NULL;
        pas_set_succ_fail(bang, γ_in, ω_in);
        return pas_ret(bang, α_out, β_out, bang, bang);
    }
    IR_t * un = pas_nalloc(cx, IR_UNOP);
    if (!un) return NULL;
    IR_LIT(un).sval = e->v.sval;
    IR_LIT(un).ival = (int64_t) e->t;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * child = pas_lower(cx, e->c[0], un  , ω_in, &eα, &eβ);
    if (!child) return NULL;
    pas_set_succ_fail(un, γ_in, ω_in);
    return pas_ret(un, α_out, β_out, eα, eβ);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
int pas_tt_is_relational(tree_e t) {
    return t==TT_LT||t==TT_LE||t==TT_GT||t==TT_GE||t==TT_EQ||t==TT_NE
         ||t==TT_LLT||t==TT_LLE||t==TT_LGT||t==TT_LGE||t==TT_LEQ||t==TT_LNE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
int pas_tt_to_binop(tree_e t) {
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
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * pas_v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * bin = pas_nalloc(cx, IR_BINOP);
    if (!bin) return NULL;
    IR_LIT(bin).sval = e->v.sval;
    IR_LIT(bin).ival = (int64_t) pas_tt_to_binop(e->t);
    IR_LIT(bin).dval = pas_tt_is_relational(e->t) ? 1.0 : 0.0;
    IR_t * e1α=NULL, * e1β=NULL, * e2α=NULL, * e2β=NULL;
    IR_t * c1 = pas_lower(cx, e->c[0], NULL  , ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = pas_lower(cx, e->c[1], bin  , e1β  , &e2α, &e2β);
    if (!c2) return NULL;
    if (!c1->γ.node) { c1->γ.node = e2α; memcpy(c1->γ.sz, "α", 3); }
    IR_t * binops[2] = { c1, c2 };
    bb_operand_aux_set(cx.bbg, bin, binops, 2);
    pas_set_succ_fail(bin, γ_in, ω_in);
    return pas_ret(bin, α_out, β_out, e1α, e2β);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int pas_to_by_const_step(const tree_t * s, int64_t * out_bits, int * is_real) {
    if (!s) return 0;
    if (s->t == TT_ILIT) { *out_bits = s->v.ival; *is_real = 0; return 1; }
    if (s->t == TT_FLIT) { double d = s->v.dval; memcpy(out_bits, &d, sizeof(double)); *is_real = 1; return 1; }
    if ((s->t == TT_MNS || s->t == TT_PLS) && s->n >= 1 && s->c[0]) {
        if (!pas_to_by_const_step(s->c[0], out_bits, is_real)) return 0;
        if (s->t == TT_MNS) {
            if (*is_real) { double d; memcpy(&d, out_bits, sizeof(double)); d = -d; memcpy(out_bits, &d, sizeof(double)); }
            else *out_bits = -(*out_bits);
        }
        return 1;
    }
    return 0;
}
IR_t * pas_v_to(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = pas_nalloc(cx, (e->t == TT_TO_BY) ? IR_TO_BY : IR_TO);
    if (!node) return NULL;
    IR_LIT(node).sval = "ag";
    if (e->t == TT_TO_BY && e->n >= 3 && e->c[2]) {
        int64_t bits = 0; int isr = 0;
        if (pas_to_by_const_step(e->c[2], &bits, &isr)) { IR_LIT(node).ival = bits; if (isr) IR_LIT(node).sval = "ar"; }
    }
    IR_t * fα=NULL,*fβ=NULL,*tα=NULL,*tβ=NULL;
    IR_t * lo = pas_lower(cx, e->c[0], NULL  , ω_in, &fα, &fβ);
    if (!lo) return NULL;
    IR_t * hi = pas_lower(cx, e->c[1], node  , fβ  , &tα, &tβ);
    if (!hi) return NULL;
    if (!lo->γ.node) { lo->γ.node = tα; memcpy(lo->γ.sz, "α", 3); }
    ir_operand_push(node, lo);
    ir_operand_push(node, hi);
    pas_set_succ_fail(node, γ_in, ω_in);
    return pas_ret(node, α_out, β_out, fα, node  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_wire_if(lcx_t cx, const tree_t * e, int else_succeeds, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = pas_nalloc(cx, IR_IF);
    if (!node) return NULL;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * c1α=NULL,*c1β=NULL, *c2α=NULL,*c2β=NULL, *c3α=NULL,*c3β=NULL;
    IR_t * thenα = NULL, * elseα = NULL;
    IR_t * c2 = pas_lower(cx, e->c[1], γ_in, ω_in, &c2α, &c2β);
    if (!c2) return NULL;
    thenα = c2α;
    if (e->n >= 3 && e->c[2]) {
        IR_t * c3 = pas_lower(cx, e->c[2], γ_in, ω_in, &c3α, &c3β);
        if (!c3) return NULL;
        elseα = c3α;
    } else {
        elseα = else_succeeds ? γ_in : ω_in;
    }
    IR_t * c1 = pas_lower(cb, e->c[0], thenα  , elseα  , &c1α, &c1β);
    if (!c1) return NULL;
    if (!ir_operand_push(node, c1α)) return NULL;
    pas_set_succ_fail(node, γ_in, ω_in);
    return pas_ret(node, α_out, β_out, c1α, node  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * pas_v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    return pas_wire_if(cx, e, 0, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
lcx_t pas_bounded(lcx_t cx) { cx.bounded = 1; return cx; }
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t pas_with_loop(lcx_t cx, IR_t * lω, IR_t * lnext) { cx.loop_ω = lω; cx.loop_next = lnext; return cx; }
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_graph_t * pas_value_subgraph(lcx_t cx, const tree_t * e) {
    IR_graph_t * blk = IR_alloc(256, cx.lang);
    if (!blk) return NULL;
    IR_t * vfail = IR_node_alloc(blk, IR_FAIL);
    lcx_t vcx = cx; vcx.bbg = blk;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * en = pas_lower(vcx, e, NULL, vfail, &eα, &eβ);
    if (!en) { IR_free(blk); return NULL; }
    (void) eβ;
    blk->entry = eα ? eα : en;
    return blk;
}
IR_t * pas_v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * kids[64];
    int nk = pas_flatten_seq(e, e->t, kids, 64);
    if (nk < 1) return NULL;
    return pas_wire_seq(cx, IR_CONJ, kids, nk, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1) return NULL;
    return pas_wire_alt(cx, IR_ALT, (const tree_t * const *) e->c, e->n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_every(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * ev = pas_nalloc(cx, IR_EVERY);
    if (!ev) return NULL;
    IR_t * g1α=NULL,*g1β=NULL;
    IR_t * gen = pas_lower(cx, e->c[0], NULL  , ev  , &g1α, &g1β);
    if (!gen) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = pas_lower(pas_bounded(cx), e->c[1], g1β  , g1β  , &b2α, &b2β);
        if (!body) return NULL;
        if (!gen->γ.node) { gen->γ.node = b2α; memcpy(gen->γ.sz, "α", 3); }
    } else {
        if (!gen->γ.node) { gen->γ.node = g1β; memcpy(gen->γ.sz, "α", 3); }
    }
    if (!ir_operand_push(ev, g1α)) return NULL;
    pas_set_succ_fail(ev, γ_in, ω_in);
    return pas_ret(ev, α_out, β_out, g1α, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * wh = pas_nalloc(cx, IR_WHILE);
    if (!wh) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = pas_lower(pas_bounded(cx), e->c[0], NULL  , wh  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = pas_lower(pas_with_loop(pas_bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
        if (!body) return NULL;
        if (!cond->γ.node) { cond->γ.node = b2α; memcpy(cond->γ.sz, "α", 3); }
    } else {
        if (!cond->γ.node) { cond->γ.node = c1α; memcpy(cond->γ.sz, "α", 3); }
    }
    if (!ir_operand_push(wh, c1α)) return NULL;
    pas_set_succ_fail(wh, γ_in, ω_in);
    return pas_ret(wh, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_until(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * un = pas_nalloc(cx, IR_UNTIL);
    if (!un) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = pas_lower(pas_bounded(cx), e->c[0], un  , NULL  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = pas_lower(pas_with_loop(pas_bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
        if (!body) return NULL;
        if (!cond->ω.node) { cond->ω.node = b2α; memcpy(cond->ω.sz, "α", 3); }
    } else {
        if (!cond->ω.node) { cond->ω.node = c1α; memcpy(cond->ω.sz, "α", 3); }
    }
    if (!ir_operand_push(un, c1α)) return NULL;
    pas_set_succ_fail(un, γ_in, ω_in);
    return pas_ret(un, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_repeat(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * rp = pas_nalloc(cx, IR_REPEAT);
    if (!rp) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * body = pas_lower(pas_with_loop(pas_bounded(cx), γ_in, rp), e->c[0], rp  , rp  , &eα, &eβ);
    if (!body) return NULL;
    if (!ir_operand_push(rp, eα)) return NULL;
    pas_set_succ_fail(rp, γ_in, ω_in);
    rp->γ.node = eα; memcpy(rp->γ.sz, "α", 3);
    return pas_ret(rp, α_out, β_out, eα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * call = pas_nalloc(cx, IR_CALL); if (!call) return NULL;
    IR_LIT(call).sval = e->c[0]->v.sval;
    int nargs = e->n - 1;
    IR_LIT(call).ival = nargs;
    IR_LIT(call).dval = 3.0;
    if (nargs > 0) {
        IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
        if (!blks) return NULL;
        lcx_t ac = cx; ac.role = ROLE_VALUE;
        for (int i = 0; i < nargs; i++) {
            blks[i] = pas_value_subgraph(ac, e->c[i + 1]);
            if (!blks[i]) { free(blks); return NULL; }
        }
        IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    }
    pas_set_succ_fail(call, γ_in, ω_in);
    IR_t * call_beta = (allow_generator) ? call : ω_in;
    return pas_ret(call, α_out, β_out, call, call_beta);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_loop_break(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void)γ_in;
    IR_t * br = pas_nalloc(cx, IR_BREAK);
    if (!br) return NULL;
    IR_t * tgt = cx.loop_ω ? cx.loop_ω : ω_in;
    pas_set_succ_fail(br, tgt, tgt);
    return pas_ret(br, α_out, β_out, br, tgt);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_loop_next(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void)γ_in; (void)e;
    IR_t * nx = pas_nalloc(cx, IR_NEXT);
    if (!nx) return NULL;
    IR_t * tgt = cx.loop_next ? cx.loop_next : ω_in;
    pas_set_succ_fail(nx, tgt, tgt);
    return pas_ret(nx, α_out, β_out, nx, tgt);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_not(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * nt = pas_nalloc(cx, IR_NOT);
    if (!nt) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * ce = pas_lower(pas_bounded(cx), e->c[0], ω_in  , nt  , &eα, &eβ);
    if (!ce) return NULL;
    bb_operand_aux_set(cx.bbg, nt, &ce, 1);
    pas_set_succ_fail(nt, γ_in, ω_in);
    return pas_ret(nt, α_out, β_out, eα, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * lhs_t = NULL, * rhs_t = NULL;
    if (!pas_tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
    if (!lhs_t || !rhs_t) return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    int lhs_is_var = (lhs_t->t == TT_VAR);
    if (!lhs_is_var) return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * as = pas_nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = lhs_t->v.sval ? lhs_t->v.sval : "";
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = pas_lower(cx, rhs_t, as  , ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    pas_set_succ_fail(as, γ_in, ω_in);
    return pas_ret(as, α_out, β_out, rα, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * pas_value_shared(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    switch (e->t) {
    case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_CSET:
    case TT_NUL:  case TT_NULL: case TT_VAR:  case TT_NAME: case TT_KEYWORD:
        return pas_v_literal(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUCCEED: { IR_t * n = pas_nalloc(cx, IR_SUCCEED); if (!n) return NULL; return pas_emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FAIL:    { IR_t * n = pas_nalloc(cx, IR_FAIL);    if (!n) return NULL; lcx_t bx = cx; bx.bounded = 1; return pas_emit_leaf(bx, n, γ_in, ω_in, α_out, β_out); }
    case TT_MATCH_UNARY:
        return pas_v_unop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL:
    case TT_RANDOM: case TT_CSET_COMPL: case TT_ITERATE: case TT_INTERROGATE:
        return pas_v_unop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
        return pas_v_binop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_TO: case TT_TO_BY:
        return pas_v_to(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_IF:
        return pas_v_if(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SEQ: case TT_SEQ_EXPR:
        return pas_v_conj(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ALTERNATE: case TT_ALT:
        return pas_v_alt(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_EVERY:
        return pas_v_every(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_WHILE:
        return pas_v_while(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_UNTIL:
        return pas_v_until(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_REPEAT:
        return pas_v_repeat(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_NOT:
        return pas_v_not(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_BREAK:
        return pas_v_loop_break(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_NEXT:
        return pas_v_loop_next(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ASSIGN:
        return pas_v_assign(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_RETURN:
    case TT_NRETURN:
        return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUSPEND:
        return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_INITIAL:
        return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FNC: case TT_PROC_FAIL: case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP: case TT_LIMIT: case TT_CASE: {
        if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return pas_wire_det_builtin1(cx, e->c[1], fn, γ_in, ω_in, α_out, β_out);
        }
        return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    case TT_SCAN:
        return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return pas_lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!arg_t) return NULL;
    IR_t * call = pas_nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = fn;
    IR_LIT(call).ival = 1;
    IR_LIT(call).dval = 1.0;
    lcx_t av = cx; av.role = ROLE_VALUE;
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * a = pas_lower(av, arg_t, call  , ω_in, &aα, &aβ);
    if (!a) return NULL;
    pas_set_succ_fail(call, γ_in, ω_in);
    IR_t * call_resume = g_icn_postfix_resume ? aβ : ω_in;
    return pas_ret(call, α_out, β_out, aα, call_resume);
}
IR_t * pas_v_raku_det_call(lcx_t cx, const char * fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!fn || nkids < 0) return NULL;
    IR_t * call = pas_nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = GC_strdup(fn);
    IR_LIT(call).ival = nkids;
    IR_LIT(call).dval = 2.0;
    IR_graph_t ** blks = (IR_graph_t **) calloc((size_t)(nkids > 0 ? nkids : 1), sizeof(IR_graph_t *));
    if (!blks) return NULL;
    lcx_t mv = cx; mv.role = ROLE_VALUE;
    for (int i = 0; i < nkids; i++) {
        if (!kids[i]) { free(blks); return NULL; }
        blks[i] = pas_value_subgraph(mv, kids[i]);
        if (!blks[i]) { free(blks); return NULL; }
    }
    IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    pas_set_succ_fail(call, γ_in, ω_in);
    return pas_ret(call, α_out, β_out, call  , ω_in  );
}

IR_t * pas_v_raku_mutate_writeback(lcx_t cx, const char * target, const char * pure_fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!target || !pure_fn || nkids < 1) return NULL;
    IR_t * as = pas_nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = GC_strdup(target);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * call = pas_v_raku_det_call(cx, pure_fn, kids, nkids, as  , ω_in, &cα, &cβ);
    if (!call) return NULL;
    (void) cβ;
    pas_set_succ_fail(as, γ_in, ω_in);
    return pas_ret(as, α_out, β_out, cα  , ω_in  );
}
/*====================================================================================================================*/
static IR_t * pas_bool_diamond(lcx_t cx, const tree_t * child, IR_t * ω_in, IR_t ** entry_out, IR_t ** as1_out, IR_t ** as0_out) {
    static int g_pas_bool_tmp = 0;
    char * tn = (char *) GC_malloc(16); if (!tn) return NULL;
    snprintf(tn, 16, "__pbt%d", g_pas_bool_tmp++);
    IR_t * lit1 = pas_nalloc(cx, IR_LIT_I); if (!lit1) return NULL; IR_LIT(lit1).ival = 1;
    IR_t * lit0 = pas_nalloc(cx, IR_LIT_I); if (!lit0) return NULL; IR_LIT(lit0).ival = 0;
    IR_t * as1  = pas_nalloc(cx, IR_ASSIGN); if (!as1) return NULL; IR_LIT(as1).sval = tn;
    IR_t * as0  = pas_nalloc(cx, IR_ASSIGN); if (!as0) return NULL; IR_LIT(as0).sval = tn;
    IR_t * rd   = pas_nalloc(cx, IR_VAR);    if (!rd)  return NULL; IR_LIT(rd).sval  = tn;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cn = pas_lower(cb, child, lit1, lit0, &cα, &cβ);
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
    IR_t * c1 = b1 ? rd1 : pas_lower(cx, e->c[0], NULL, ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = b2 ? rd2 : pas_lower(cx, e->c[1], bin, ω_in, &e2α, &e2β);
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
    pas_set_succ_fail(bin, γ_in, ω_in);
    return pas_ret(bin, α_out, β_out, entry, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * pas_leaf_node(lcx_t cx, IR_e kind, const char * name, long long iv) {
    IR_t * n = pas_nalloc(cx, kind); if (!n) return NULL;
    if (kind == IR_VAR) IR_LIT(n).sval = (char *) name; else IR_LIT(n).ival = iv;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_binop_ll(lcx_t cx, int code, int is_rel, IR_t * op0, IR_t * op1, IR_t * γ_in, IR_t * ω_in) {
    if (!op0 || !op1) return NULL;
    IR_t * bin = pas_nalloc(cx, IR_BINOP); if (!bin) return NULL;
    IR_LIT(bin).ival = (int64_t) code; IR_LIT(bin).dval = is_rel ? 1.0 : 0.0;
    pas_set_succ_fail(op0, op1, ω_in); pas_set_succ_fail(op1, bin, ω_in);
    IR_t * ops[2] = { op0, op1 }; bb_operand_aux_set(cx.bbg, bin, ops, 2);
    pas_set_succ_fail(bin, γ_in, ω_in);
    return bin;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_binop_lt(lcx_t cx, int code, int is_rel, IR_t * op0, const tree_t * op1_t, IR_t * γ_in, IR_t * ω_in) {
    if (!op0 || !op1_t) return NULL;
    IR_t * bin = pas_nalloc(cx, IR_BINOP); if (!bin) return NULL;
    IR_LIT(bin).ival = (int64_t) code; IR_LIT(bin).dval = is_rel ? 1.0 : 0.0;
    IR_t * p1α = NULL, * p1β = NULL;
    IR_t * p1 = pas_lower(cx, op1_t, bin, ω_in, &p1α, &p1β); if (!p1) return NULL;
    pas_set_succ_fail(op0, p1α, ω_in);
    IR_t * ops[2] = { op0, p1 }; bb_operand_aux_set(cx.bbg, bin, ops, 2);
    pas_set_succ_fail(bin, γ_in, ω_in);
    return bin;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * pas_v_pascal_for(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e || e->n < 4 || !e->c[0] || e->c[0]->t != TT_VAR || !e->c[0]->v.sval) return NULL;
    int down = (e->v.ival == 1);
    const char * vname = e->c[0]->v.sval;
    const tree_t * from = e->c[1]; const tree_t * to = e->c[2]; const tree_t * body = e->c[3];
    lcx_t cb = pas_bounded(cx);
    IR_t * cond_v = pas_leaf_node(cb, IR_VAR, vname, 0); if (!cond_v) return NULL;
    IR_t * cond = pas_binop_lt(cb, down ? BINOP_GE : BINOP_LE, 1, cond_v, to, NULL, γ_in); if (!cond) return NULL;
    IR_t * cond_entry = cond_v;
    IR_t * incr = pas_nalloc(cb, IR_ASSIGN); if (!incr) return NULL; IR_LIT(incr).sval = (char *) vname;
    IR_t * step_v = pas_leaf_node(cb, IR_VAR, vname, 0); IR_t * step_one = pas_leaf_node(cb, IR_LIT_I, NULL, 1);
    IR_t * step = pas_binop_ll(cb, down ? BINOP_SUB : BINOP_ADD, 0, step_v, step_one, incr, ω_in); if (!step) return NULL;
    pas_set_succ_fail(incr, cond_entry, ω_in);
    IR_t * incr_entry = step_v;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * bnode = pas_lower(cb, body, incr_entry, incr_entry, &bα, &bβ); if (!bnode) return NULL;
    if (!cond->γ.node) { cond->γ.node = bα ? bα : bnode; memcpy(cond->γ.sz, "α", 3); }
    IR_t * init = pas_nalloc(cb, IR_ASSIGN); if (!init) return NULL; IR_LIT(init).sval = (char *) vname;
    IR_t * fα = NULL, * fβ = NULL;
    IR_t * fnode = pas_lower(cb, from, init, ω_in, &fα, &fβ); if (!fnode) return NULL;
    pas_set_succ_fail(init, cond_entry, ω_in);
    return pas_ret(init, α_out, β_out, fα ? fα : fnode, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_v_pascal_repeat(lcx_t cx, const tree_t * body_t, const tree_t * cond_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!body_t || !cond_t) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cond = pas_lower(pas_bounded(cx), cond_t, γ_in, NULL, &cα, &cβ);
    if (!cond) return NULL;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * body = pas_lower(pas_bounded(cx), body_t, cα, ω_in, &bα, &bβ);
    if (!body) return NULL;
    if (!cond->ω.node) { cond->ω.node = bα; memcpy(cond->ω.sz, "α", 3); }
    return pas_ret(body, α_out, β_out, bα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_assign_fnc_lhs(lcx_t cx, const tree_t * lhs_t, const tree_t * rhs_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * as = pas_nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = lhs_t->c[0]->v.sval;
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = pas_lower(cx, rhs_t, as, ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    pas_set_succ_fail(as, γ_in, ω_in);
    return pas_ret(as, α_out, β_out, rα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_goto_u(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in;
    IR_t * landing = bb_label_landing(e->v.sval);
    if (!landing) return NULL;
    IR_t * n = pas_nalloc(cx, IR_SUCCEED);
    if (!n) return NULL;
    n->γ.node = landing; memcpy(n->γ.sz, "α", 3); n->ω.node = ω_in; memcpy(n->ω.sz, "α", 3);
    return pas_ret(n, α_out, β_out, n, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * pas_label_def(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * landing = bb_label_landing(e->v.sval);
    if (!landing) return NULL;
    IR_t * iα = NULL, * iβ = NULL;
    IR_t * inner = pas_lower(cx, e->c[0], γ_in, ω_in, &iα, &iβ);
    if (!inner) return NULL;
    landing->γ.node = iα ? iα : inner; memcpy(landing->γ.sz, "α", 3);
    landing->ω.node = ω_in; memcpy(landing->ω.sz, "α", 3);
    return pas_ret(landing, α_out, β_out, landing, iβ ? iβ : ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * lower_pas_value(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_t ** α, IR_t ** β) {
    switch (e->t) {
    case TT_IF:
        return pas_wire_if(cx, e, 1, γ, ω, α, β);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE: {
        if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
        int b1 = pas_tt_is_relational(e->c[0]->t);
        int b2 = pas_tt_is_relational(e->c[1]->t);
        if (!b1 && !b2) return pas_value_shared(cx, e, γ, ω, α, β);
        IR_t * bin = pas_nalloc(cx, IR_BINOP);
        if (!bin) return NULL;
        IR_LIT(bin).sval = e->v.sval;
        IR_LIT(bin).ival = (int64_t) pas_tt_to_binop(e->t);
        IR_LIT(bin).dval = pas_tt_is_relational(e->t) ? 1.0 : 0.0;
        return pas_binop_bool(cx, e, bin, b1, b2, γ, ω, α, β);
    }
    case TT_GOTO_U:
        if (!e->v.sval) return NULL;
        return pas_goto_u(cx, e, γ, ω, α, β);
    case TT_LABEL_DEF:
        if (e->n < 1 || !e->c[0] || !e->v.sval) return NULL;
        return pas_label_def(cx, e, γ, ω, α, β);
    case TT_REPEAT:
        if (e->n >= 2 && e->c[0] && e->c[1]) return pas_v_pascal_repeat(cx, e->c[0], e->c[1], γ, ω, α, β);
        return pas_value_shared(cx, e, γ, ω, α, β);
    case TT_FNC: case TT_PROC_FAIL: case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP: case TT_LIMIT: case TT_CASE: {
        if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return pas_wire_det_builtin1(cx, e->c[1], fn, γ, ω, α, β);
        }
        if (e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval)
            return pas_v_det_call(cx, e, 0, γ, ω, α, β);
        return pas_lower_unhandled(cx, e, γ, ω, α, β);
    }
    case TT_IDX:
        if (e->n >= 2 && e->c[0] && e->c[1]) { const tree_t * k[2] = { e->c[0], e->c[1] }; return pas_v_raku_det_call(cx, "arr_get", k, 2, γ, ω, α, β); }
        return pas_lower_unhandled(cx, e, γ, ω, α, β);
    case TT_FOR:
        return pas_v_pascal_for(cx, e, γ, ω, α, β);
    case TT_ASSIGN: {
        const tree_t * lhs_t = NULL, * rhs_t = NULL;
        if (!pas_tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
        if (!lhs_t || !rhs_t) return pas_lower_unhandled(cx, e, γ, ω, α, β);
        if (lhs_t->t == TT_IDX && lhs_t->n >= 2 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval && lhs_t->c[1]) {
            const tree_t * k[3] = { lhs_t->c[0], lhs_t->c[1], rhs_t };
            return pas_v_raku_mutate_writeback(cx, lhs_t->c[0]->v.sval, "arr_set_pure", k, 3, γ, ω, α, β);
        }
        if (lhs_t->t == TT_FNC && lhs_t->n >= 2 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval && !strcmp(lhs_t->c[0]->v.sval, "__pas_deref")) {
            const tree_t * k[2] = { lhs_t->c[1], rhs_t };
            return pas_v_raku_det_call(cx, "__pas_deref_set", k, 2, γ, ω, α, β);
        }
        if (lhs_t->t == TT_FNC && lhs_t->n >= 1 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval)
            return pas_assign_fnc_lhs(cx, lhs_t, rhs_t, γ, ω, α, β);
        return pas_value_shared(cx, e, γ, ω, α, β);
    }
    default:
        return pas_value_shared(cx, e, γ, ω, α, β);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_pas(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β) {
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * r = lower_pas_value(cx, e, γ, ω, &aα, &aβ);
    return pas_iref(r, α, β, aα, aβ);
}
