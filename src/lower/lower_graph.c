#include "lower_graph.h"
#include "BB.h"
#include "core.h"
#include "coerce.h"
#include "ast.h"
#include "../frontend/icon/icon_lex.h"
#include "../runtime/interp/gen.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
IR_t *lower_new_Intlit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_I);
    if (!bb) return NULL;
    bb->ival = e->v.ival;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Reallit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_F);
    if (!bb) return NULL;
    bb->dval = e->v.dval;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Stringlit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_S);
    if (!bb) return NULL;
    bb->sval = e->v.sval ? e->v.sval : "";
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Csetlit(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_LIT_S);
    if (!bb) return NULL;
    bb->sval = e->v.sval ? e->v.sval : "";
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Global(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_SUCCEED);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int is_suspendable(struct tree_t *e);
extern IR_t *lower_expr_node(IR_graph_t *bbg, struct tree_t *e);
IR_t *lower_new_Binop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    if (e->t == TT_LCONCAT) {
        IR_t *bb = IR_node_alloc(bbg, IR_LCONCAT);
        if (!bb) return NULL;
        bb->α = lhs; bb->β = rhs;
        return bb;
    }
    BinopKind op = BINOP_ADD;
    int is_relop  = 0;
    int is_strrel = 0;
    switch (e->t) {
    case TT_ADD: op = BINOP_ADD;    break;
    case TT_SUB: op = BINOP_SUB;    break;
    case TT_MUL: op = BINOP_MUL;    break;
    case TT_DIV: op = BINOP_DIV;    break;
    case TT_MOD: op = BINOP_MOD;    break;
    case TT_POW: op = BINOP_POW;    break;
    case TT_LT:  op = BINOP_LT; is_relop = 1; break;
    case TT_LE:  op = BINOP_LE; is_relop = 1; break;
    case TT_GT:  op = BINOP_GT; is_relop = 1; break;
    case TT_GE:  op = BINOP_GE; is_relop = 1; break;
    case TT_EQ:  op = BINOP_EQ; is_relop = 1; break;
    case TT_NE:  op = BINOP_NE; is_relop = 1; break;
    case TT_CAT: op = BINOP_CONCAT; break;
    case TT_LLT: op = BINOP_SLT; is_strrel = 1; break;
    case TT_LLE: op = BINOP_SLE; is_strrel = 1; break;
    case TT_LGT: op = BINOP_SGT; is_strrel = 1; break;
    case TT_LGE: op = BINOP_SGE; is_strrel = 1; break;
    case TT_LEQ: op = BINOP_SEQ; is_strrel = 1; break;
    case TT_LNE: op = BINOP_SNE; is_strrel = 1; break;
    default: return NULL;
    }
    int is_gen = is_strrel ? 0 : (is_suspendable(e->c[0]) || is_suspendable(e->c[1]));
    IR_t *bb = IR_node_alloc(bbg, is_gen ? IR_BINOP_GEN : IR_BINOP);
    if (!bb) return NULL;
    bb->α     = lhs;
    bb->β     = rhs;
    bb->ival  = (int64_t)op;
    bb->state = is_strrel ? 1 : is_relop;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Binop_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    BinopKind op = BINOP_ADD;
    int is_relop  = 0;
    int is_strrel = 0;
    switch (e->t) {
    case TT_ADD: op = BINOP_ADD;    break;
    case TT_SUB: op = BINOP_SUB;    break;
    case TT_MUL: op = BINOP_MUL;    break;
    case TT_DIV: op = BINOP_DIV;    break;
    case TT_MOD: op = BINOP_MOD;    break;
    case TT_POW: op = BINOP_POW;    break;
    case TT_LT:  op = BINOP_LT; is_relop = 1; break;
    case TT_LE:  op = BINOP_LE; is_relop = 1; break;
    case TT_GT:  op = BINOP_GT; is_relop = 1; break;
    case TT_GE:  op = BINOP_GE; is_relop = 1; break;
    case TT_EQ:  op = BINOP_EQ; is_relop = 1; break;
    case TT_NE:  op = BINOP_NE; is_relop = 1; break;
    case TT_CAT: op = BINOP_CONCAT; break;
    case TT_LLT: op = BINOP_SLT; is_strrel = 1; break;
    case TT_LLE: op = BINOP_SLE; is_strrel = 1; break;
    case TT_LGT: op = BINOP_SGT; is_strrel = 1; break;
    case TT_LGE: op = BINOP_SGE; is_strrel = 1; break;
    case TT_LEQ: op = BINOP_SEQ; is_strrel = 1; break;
    case TT_LNE: op = BINOP_SNE; is_strrel = 1; break;
    default: return NULL;
    }
    int is_gen = is_strrel ? 0 : (is_suspendable(e->c[0]) || is_suspendable(e->c[1]));
    if (is_gen) return NULL;
    IR_t *l_αo = NULL, *l_βo = NULL;
    IR_t *lhs = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &l_αo, &l_βo, 0);
    if (!lhs) return NULL;
    IR_t *lhs_entry = l_αo ? l_αo : lhs;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *rhs = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &r_αo, &r_βo, 0);
    if (!rhs) return NULL;
    IR_t *rhs_entry = r_αo ? r_αo : rhs;
    IR_t *bb = IR_node_alloc(bbg, IR_BINOP);
    if (!bb) return NULL;
    bb->ival  = (int64_t)op;
    bb->state = is_strrel ? 1 : is_relop;
    if (!lhs->γ) lhs->γ = rhs_entry;
    if (!lhs->ω) lhs->ω = ω_in;
    if (!rhs->γ) rhs->γ = bb;
    if (!rhs->ω) rhs->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = lhs_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Lconcat_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *l_αo = NULL, *l_βo = NULL;
    IR_t *lhs = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &l_αo, &l_βo, 0);
    if (!lhs) return NULL;
    IR_t *lhs_entry = l_αo ? l_αo : lhs;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *rhs = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &r_αo, &r_βo, 0);
    if (!rhs) return NULL;
    IR_t *rhs_entry = r_αo ? r_αo : rhs;
    IR_t *bb = IR_node_alloc(bbg, IR_LCONCAT);
    if (!bb) return NULL;
    if (!lhs->γ) lhs->γ = rhs_entry;
    if (!lhs->ω) lhs->ω = ω_in;
    if (!rhs->γ) rhs->γ = bb;
    if (!rhs->ω) rhs->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = lhs_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Sectionop_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 3 || !e->c[0] || !e->c[1] || !e->c[2]) return NULL;
    IR_t *b_αo = NULL, *b_βo = NULL;
    IR_t *base = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &b_αo, &b_βo, 0);
    if (!base) return NULL;
    IR_t *base_entry = b_αo ? b_αo : base;
    IR_t *i1_αo = NULL, *i1_βo = NULL;
    IR_t *i1 = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &i1_αo, &i1_βo, 0);
    if (!i1) return NULL;
    IR_t *i1_entry = i1_αo ? i1_αo : i1;
    IR_t *i2_αo = NULL, *i2_βo = NULL;
    IR_t *i2 = lower_expr_threaded_b(bbg, e->c[2], NULL, ω_in, &i2_αo, &i2_βo, 0);
    if (!i2) return NULL;
    IR_t *i2_entry = i2_αo ? i2_αo : i2;
    IR_t *bb = IR_node_alloc(bbg, IR_SECTION);
    if (!bb) return NULL;
    bb->ival = (e->t == TT_SECTION) ? 0 : (e->t == TT_SECTION_PLUS) ? 1 : 2;
    if (!base->γ) base->γ = i1_entry;
    if (!base->ω) base->ω = ω_in;
    if (!i1->γ) i1->γ = i2_entry;
    if (!i1->ω) i1->ω = ω_in;
    if (!i2->γ) i2->γ = bb;
    if (!i2->ω) i2->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = base_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Idx_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *b_αo = NULL, *b_βo = NULL;
    IR_t *base = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &b_αo, &b_βo, 0);
    if (!base) return NULL;
    IR_t *base_entry = b_αo ? b_αo : base;
    IR_t *x_αo = NULL, *x_βo = NULL;
    IR_t *idx = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &x_αo, &x_βo, 0);
    if (!idx) return NULL;
    IR_t *idx_entry = x_αo ? x_αo : idx;
    IR_t *bb = IR_node_alloc(bbg, IR_IDX);
    if (!bb) return NULL;
    if (!base->γ) base->γ = idx_entry;
    if (!base->ω) base->ω = ω_in;
    if (!idx->γ) idx->γ = bb;
    if (!idx->ω) idx->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = base_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Idx_set_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->c[0]->t != TT_IDX) return NULL;
    if (e->c[0]->n < 2 || !e->c[0]->c[0] || !e->c[0]->c[1]) return NULL;
    IR_t *b_αo = NULL, *b_βo = NULL;
    IR_t *base = lower_expr_threaded_b(bbg, e->c[0]->c[0], NULL, ω_in, &b_αo, &b_βo, 0);
    if (!base) return NULL;
    IR_t *base_entry = b_αo ? b_αo : base;
    IR_t *x_αo = NULL, *x_βo = NULL;
    IR_t *idx = lower_expr_threaded_b(bbg, e->c[0]->c[1], NULL, ω_in, &x_αo, &x_βo, 0);
    if (!idx) return NULL;
    IR_t *idx_entry = x_αo ? x_αo : idx;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *rhs = lower_expr_threaded_b(bbg, e->c[1], NULL, ω_in, &r_αo, &r_βo, 0);
    if (!rhs) return NULL;
    IR_t *rhs_entry = r_αo ? r_αo : rhs;
    IR_t *bb = IR_node_alloc(bbg, IR_IDX_SET);
    if (!bb) return NULL;
    if (!base->γ) base->γ = idx_entry;
    if (!base->ω) base->ω = ω_in;
    if (!idx->γ) idx->γ = rhs_entry;
    if (!idx->ω) idx->ω = ω_in;
    if (!rhs->γ) rhs->γ = bb;
    if (!rhs->ω) rhs->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = base_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_If(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *cond = lower_expr_node(bbg, e->c[0]);
    if (!cond) return NULL;
    IR_t *then_nd = NULL;
    IR_t *else_nd = NULL;
    if (e->n >= 2 && e->c[1]) {
        then_nd = lower_expr_node(bbg, e->c[1]);
        if (!then_nd) return NULL;
    }
    if (e->n >= 3 && e->c[2]) {
        else_nd = lower_expr_node(bbg, e->c[2]);
        if (!else_nd) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_IF);
    if (!bb) return NULL;
    bb->α = cond;
    if (then_nd) bb->β = then_nd;
    if (else_nd) bb->ω = else_nd;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_If_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_IF);
    if (!bb) return NULL;
    IR_t *cond_αo = NULL, *cond_βo = NULL;
    IR_t *cond = lower_expr_threaded_b(bbg, e->c[0], bb, bb, &cond_αo, &cond_βo, 0);
    if (!cond) return NULL;
    IR_t *cond_entry = cond_αo ? cond_αo : cond;
    if (!cond->γ) cond->γ = bb;
    if (!cond->ω) cond->ω = bb;
    IR_t *then_entry = NULL;
    IR_t *else_entry = NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t *αo = NULL, *βo = NULL;
        IR_t *then_apply = lower_expr_threaded_b(bbg, e->c[1], γ_in, ω_in, &αo, &βo, 0);
        if (!then_apply) return NULL;
        then_entry = αo ? αo : then_apply;
        if (!then_apply->γ && γ_in) then_apply->γ = γ_in;
        if (!then_apply->ω && ω_in) then_apply->ω = ω_in;
    }
    if (e->n >= 3 && e->c[2]) {
        IR_t *αo = NULL, *βo = NULL;
        IR_t *else_apply = lower_expr_threaded_b(bbg, e->c[2], γ_in, ω_in, &αo, &βo, 0);
        if (!else_apply) return NULL;
        else_entry = αo ? αo : else_apply;
        if (!else_apply->γ && γ_in) else_apply->γ = γ_in;
        if (!else_apply->ω && ω_in) else_apply->ω = ω_in;
    }
    bb->γ = then_entry ? then_entry : γ_in;
    bb->ω = else_entry ? else_entry : γ_in;
    if (α_out) *α_out = cond_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int fold_signed_lit(struct tree_t *n, int64_t *iv, double *dv, int *is_real);
IR_t *lower_new_ToBy(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->t == TT_TO) {
        IR_t *bb = IR_node_alloc(bbg, IR_TO);
        if (!bb) return NULL;
        int lo_lit = (e->c[0]->t == TT_ILIT);
        int hi_lit = (e->c[1]->t == TT_ILIT);
        if (lo_lit && hi_lit) {
            bb->α = NULL;
            bb->β = NULL;
            bb->ival = e->c[0]->v.ival;
            int64_t to_i = e->c[1]->v.ival;
            memcpy(&bb->dval, &to_i, 8);
        } else {
            IR_t *lo = lower_expr_node(bbg, e->c[0]);
            IR_t *hi = lower_expr_node(bbg, e->c[1]);
            if (!lo || !hi) return NULL;
            bb->α = lo;
            bb->β = hi;
        }
        return bb;
    }
    struct tree_t *lo_n = e->c[0];
    struct tree_t *hi_n = e->c[1];
    struct tree_t *by_n = (e->n >= 3 && e->c[2]) ? e->c[2] : NULL;
    int64_t step_i = 1; double step_r = 1.0; int step_is_real = 0;
    if (by_n) (void)fold_signed_lit(by_n, &step_i, &step_r, &step_is_real);
    int is_real = (lo_n->t == TT_FLIT || hi_n->t == TT_FLIT || step_is_real);
    IR_t *lo = lower_expr_node(bbg, lo_n);
    IR_t *hi = lower_expr_node(bbg, hi_n);
    if (!lo || !hi) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_TO_BY);
    if (!bb) return NULL;
    bb->α = lo;
    bb->β = hi;
    if (is_real) {
        double by_r = step_is_real ? step_r : (double)step_i;
        int64_t bi; memcpy(&bi, &by_r, 8);
        bb->ival = bi;
        bb->sval = "r";
    } else {
        bb->ival = step_i;
        bb->sval = "i";
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_ToBy_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    IR_t *bb = lower_new_ToBy(bbg, e);
    if (!bb) return NULL;
    if (bb->α && bb->β) {
        IR_t *lo = bb->α;
        IR_t *hi = bb->β;
        bb->α = NULL;
        bb->β = NULL;
        if (e->t == TT_TO) { bb->sval = "ag"; }
        else { bb->sval = (bb->sval && bb->sval[0] == 'r') ? "ar" : "ai"; }
        if (!lo->γ) lo->γ = hi;
        if (!lo->ω) lo->ω = ω_in;
        if (!hi->γ) hi->γ = bb;
        if (!hi->ω) hi->ω = ω_in;
        if (!bb->γ && γ_in) bb->γ = γ_in;
        if (!bb->ω && ω_in) bb->ω = ω_in;
        if (α_out) *α_out = lo;
        if (β_out) *β_out = bb;
        return bb;
    }
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern IR_t *lower_expr_threaded_b(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out, int bounded);
IR_t *lower_new_Every(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *gen = NULL;
    IR_t *gen_chain_entry = NULL;
    if (e->c[0]->t == TT_TO || e->c[0]->t == TT_TO_BY) {
        IR_t *αo = NULL, *βo = NULL;
        gen = lower_expr_threaded_b(bbg, e->c[0], NULL, NULL, &αo, &βo, 1);
        gen_chain_entry = αo ? αo : gen;
    } else {
        gen = lower_expr_node(bbg, e->c[0]);
        gen_chain_entry = gen;
    }
    if (!gen) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_EVERY);
    if (!bb) return NULL;
    bb->α = gen_chain_entry;
    bb->β = body;
    if (gen->α == NULL && gen->β == NULL) {
        if (body) {
            gen->γ = body;
            body->γ = gen;
            body->ω = gen;
        } else {
            gen->γ = gen;
        }
        gen->ω = bb;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lic_gen_kind_raw(IR_e k) {
    return k == IR_ALT || k == IR_BINOP_GEN || k == IR_ITERATE || k == IR_LIMIT ||
           k == IR_PROC_GEN || k == IR_LIST_BANG || k == IR_KEY_GEN || k == IR_FIND_GEN || k == IR_SEQ_GEN;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lic_is_gen_node(IR_t *e) {
    if (!e) return 0;
    if (e->t == IR_ASSIGN) return lic_is_gen_node(e->β);
    return lic_gen_kind_raw(e->t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lic_body_bears_gen(IR_t *e, int depth) {
    if (!e || depth > 64) return 0;
    if (lic_gen_kind_raw(e->t)) return 1;
    if (e->t == IR_ASSIGN) return lic_body_bears_gen(e->β, depth + 1);
    for (IR_t *a = e->α; a; a = a->γ) if (lic_body_bears_gen(a, depth + 1)) return 1;
    if (e->β && lic_body_bears_gen(e->β, depth + 1)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Every_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *gen = NULL;
    IR_t *gen_chain_entry = NULL;
    if (e->c[0]->t == TT_ASSIGN && e->c[0]->n >= 2 && e->c[0]->c[1] &&
        e->c[0]->c[1]->t == TT_TO &&
        e->c[0]->c[0] && e->c[0]->c[0]->t == TT_VAR) {
        IR_t *αo = NULL, *βo = NULL;
        IR_t *togen = lower_new_ToBy_ag(bbg, e->c[0]->c[1], NULL, NULL, &αo, &βo);
        if (togen && togen->α == NULL && togen->β == NULL) {
            IR_t *lhs = lower_expr_node(bbg, e->c[0]->c[0]);
            if (!lhs || lhs->t != IR_VAR || !lhs->sval) return NULL;
            IR_t *store = IR_node_alloc(bbg, IR_ASSIGN);
            if (!store) return NULL;
            store->α   = lhs;
            store->β   = togen;
            store->ival = 1;
            IR_t *do_body = NULL;
            if (e->n >= 2 && e->c[1]) { do_body = lower_expr_node(bbg, e->c[1]); if (!do_body) return NULL; }
            IR_t *bb = IR_node_alloc(bbg, IR_EVERY);
            if (!bb) return NULL;
            gen_chain_entry = αo ? αo : togen;
            bb->α = gen_chain_entry;
            bb->β = store;
            togen->γ = store;
            if (do_body) { store->γ = do_body; do_body->γ = togen; do_body->ω = togen; } else { store->γ = togen; }
            togen->ω = bb;
            bb->ival = 1;
            if (!bb->γ && γ_in) bb->γ = γ_in;
            if (!bb->ω && ω_in) bb->ω = ω_in;
            if (α_out) *α_out = bb;
            if (β_out) *β_out = ω_in;
            return bb;
        }
    }
    if (e->c[0]->t == TT_TO || e->c[0]->t == TT_TO_BY) {
        IR_t *αo = NULL, *βo = NULL;
        gen = lower_new_ToBy_ag(bbg, e->c[0], NULL, NULL, &αo, &βo);
        gen_chain_entry = αo ? αo : gen;
    } else {
        gen = lower_expr_node(bbg, e->c[0]);
        gen_chain_entry = gen;
    }
    if (!gen) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) { body = lower_expr_node(bbg, e->c[1]); if (!body) return NULL; }
    IR_t *bb = IR_node_alloc(bbg, IR_EVERY);
    if (!bb) return NULL;
    bb->α = gen_chain_entry;
    bb->β = body;
    if (gen->α == NULL && gen->β == NULL) {
        if (body) { gen->γ = body; body->γ = gen; body->ω = gen; } else { gen->γ = gen; }
        gen->ω = bb;
        bb->ival = 1;
    } else if (lic_is_gen_node(gen) && body && body->t == IR_SEQ_EXPR) {
        gen->γ = bb; gen->ω = bb;
        body->γ = bb; body->ω = bb;
        bb->ival = 3;
    } else if (lic_is_gen_node(gen) && !(body && lic_body_bears_gen(body, 0))) {
        if (body) { gen->γ = body; body->γ = gen; body->ω = gen; } else { gen->γ = gen; }
        gen->ω = bb;
        bb->ival = 2;
    }
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Compound(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_ProcBody(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Call(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0] || e->c[0]->t != TT_VAR || !e->c[0]->v.sval) return NULL;
    int nargs = e->n - 1;
    if (nargs == 1 && strcmp(e->c[0]->v.sval, "key") == 0 && e->c[1]) {
        IR_t *tbl = lower_expr_node(bbg, e->c[1]);
        if (!tbl) return NULL;
        IR_t *bb = IR_node_alloc(bbg, IR_KEY_GEN);
        if (!bb) return NULL;
        bb->α = tbl;
        return bb;
    }
    if ((nargs == 2 || nargs == 3 || nargs == 4)
        && strcmp(e->c[0]->v.sval, "find") == 0
        && e->c[1] && e->c[2]) {
        IR_t **args2 = calloc((size_t)nargs, sizeof(IR_t *));
        if (!args2) return NULL;
        for (int j = 0; j < nargs; j++) {
            args2[j] = lower_expr_node(bbg, e->c[1+j]);
            if (!args2[j]) { free(args2); return NULL; }
        }
        IR_t *bb = IR_node_alloc(bbg, IR_FIND_GEN);
        if (!bb) { free(args2); return NULL; }
        if (nargs >= 1) bb->α = args2[0];
        if (nargs >= 2) bb->β = args2[1];
        if (nargs >= 3) bb->γ = args2[2];
        if (nargs >= 4) bb->ω = args2[3];
        free(args2);
        bb->ival = nargs;
        return bb;
    }
    if ((nargs == 0 || nargs == 1 || nargs == 2)
        && strcmp(e->c[0]->v.sval, "seq") == 0) {
        IR_t **args2 = NULL;
        if (nargs > 0) {
            args2 = calloc((size_t)nargs, sizeof(IR_t *));
            if (!args2) return NULL;
            for (int j = 0; j < nargs; j++) {
                args2[j] = lower_expr_node(bbg, e->c[1+j]);
                if (!args2[j]) { free(args2); return NULL; }
            }
        }
        IR_t *bb = IR_node_alloc(bbg, IR_SEQ_GEN);
        if (!bb) { free(args2); return NULL; }
        if (args2 && nargs >= 1) bb->α = args2[0];
        if (args2 && nargs >= 2) bb->β = args2[1];
        if (args2) free(args2);
        bb->ival = nargs;
        return bb;
    }
    IR_t **args = NULL;
    if (nargs > 0) {
        args = calloc((size_t)nargs, sizeof(IR_t *));
        if (!args) return NULL;
        for (int j = 0; j < nargs; j++) {
            args[j] = lower_expr_node(bbg, e->c[1+j]);
            if (!args[j]) { free(args); return NULL; }
        }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_CALL);
    if (!bb) { if (args) free(args); return NULL; }
    bb->sval = e->c[0]->v.sval;
    bb->ival = nargs;
    if (args && nargs >= 1) {
        bb->α = args[0];
        for (int j = 1; j < nargs; j++) args[j-1]->γ = args[j];
    }
    if (args) free(args);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Field(IR_graph_t *bbg, struct tree_t *e) {
    const char *fname = FIELD_NAME(e);
    if (!fname || e->n < 1 || !e->c[0]) return NULL;
    IR_t *obj = lower_expr_node(bbg, e->c[0]);
    if (!obj) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_FIELD_GET);
    if (!bb) return NULL;
    bb->sval = fname;
    bb->α = obj;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Sectionop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 3 || !e->c[0] || !e->c[1] || !e->c[2]) return NULL;
    IR_t *base = lower_expr_node(bbg, e->c[0]);
    if (!base) return NULL;
    IR_t *i1 = lower_expr_node(bbg, e->c[1]);
    if (!i1) return NULL;
    IR_t *i2 = lower_expr_node(bbg, e->c[2]);
    if (!i2) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_SECTION);
    if (!bb) return NULL;
    bb->α = base;
    bb->β = i1;
    i1->γ = i2;
    bb->ival = (e->t == TT_SECTION) ? 0 : (e->t == TT_SECTION_PLUS) ? 1 : 2;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Alt(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_ALT);
    if (!bb) return NULL;
    IR_t *prev = NULL;
    for (int j = 0; j < e->n; j++) {
        if (!e->c[j]) return NULL;
        IR_t *arm = lower_expr_node(bbg, e->c[j]);
        if (!arm) return NULL;
        arm->γ = bb;
        if (j == 0) bb->α = arm;
        else prev->ω = arm;
        prev = arm;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Alt_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 1) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_ALT);
    if (!bb) return NULL;
    IR_t *arm_apply[64];
    IR_t *arm_entry[64];
    int n_arms = e->n;
    if (n_arms > 64) return NULL;
    for (int j = 0; j < n_arms; j++) {
        if (!e->c[j]) return NULL;
        IR_t *αo = NULL, *βo = NULL;
        IR_t *arm = lower_expr_threaded_b(bbg, e->c[j], bb, ω_in, &αo, &βo, 0);
        if (!arm) return NULL;
        if (!arm->γ) arm->γ = bb;
        arm_apply[j] = arm;
        arm_entry[j] = αo ? αo : arm;
    }
    for (int j = 0; j < n_arms; j++) {
        if (j + 1 < n_arms) { if (!arm_apply[j]->ω) arm_apply[j]->ω = arm_entry[j + 1]; }
        else                { if (!arm_apply[j]->ω) arm_apply[j]->ω = ω_in; }
    }
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = arm_entry[0];
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Conjunction(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *e1 = lower_expr_node(bbg, e->c[0]);
    if (!e1) return NULL;
    IR_t *e2 = lower_expr_node(bbg, e->c[1]);
    if (!e2) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_CONJ);
    if (!bb) return NULL;
    bb->α = e1;
    bb->β = e2;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Conjunction_ag(IR_graph_t *bbg, struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_CONJ);
    if (!bb) return NULL;
    IR_t *l_αo = NULL, *l_βo = NULL;
    IR_t *left = lower_expr_threaded_b(bbg, e->c[0], NULL, ω_in, &l_αo, &l_βo, 0);
    if (!left) return NULL;
    IR_t *left_entry = l_αo ? l_αo : left;
    IR_t *r_αo = NULL, *r_βo = NULL;
    IR_t *right = lower_expr_threaded_b(bbg, e->c[1], bb, ω_in, &r_αo, &r_βo, 0);
    if (!right) return NULL;
    IR_t *right_entry = r_αo ? r_αo : right;
    if (!left->γ)  left->γ  = right_entry;
    if (!left->ω)  left->ω  = ω_in;
    if (!right->γ) right->γ = bb;
    if (!right->ω) right->ω = ω_in;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = left_entry;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Not(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *inner = lower_expr_node(bbg, e->c[0]);
    if (!inner) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_NOT);
    if (!bb) return NULL;
    bb->α = inner;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_While(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *cond = lower_expr_node(bbg, e->c[0]);
    if (!cond) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_WHILE);
    if (!bb) return NULL;
    bb->α = cond;
    bb->β = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Until(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *cond = lower_expr_node(bbg, e->c[0]);
    if (!cond) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_UNTIL);
    if (!bb) return NULL;
    bb->α = cond;
    bb->β = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Repeat(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *body = lower_expr_node(bbg, e->c[0]);
    if (!body) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_REPEAT);
    if (!bb) return NULL;
    bb->α = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Limitation(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *gen = lower_expr_node(bbg, e->c[0]);
    if (!gen) return NULL;
    IR_t *lim = lower_expr_node(bbg, e->c[1]);
    if (!lim) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_LIMIT);
    if (!bb) return NULL;
    bb->α = gen;
    bb->β = lim;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Scan(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t *subj = lower_expr_node(bbg, e->c[0]);
    if (!subj) return NULL;
    IR_t *body = NULL;
    if (e->n >= 2 && e->c[1]) {
        body = lower_expr_node(bbg, e->c[1]);
        if (!body) return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_GEN_SCAN);
    if (!bb) return NULL;
    bb->α = subj;
    if (body) bb->β = body;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Case(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t **children = calloc((size_t)e->n, sizeof(IR_t *));
    if (!children) return NULL;
    for (int j = 0; j < e->n; j++) {
        if (!e->c[j]) { free(children); return NULL; }
        children[j] = lower_expr_node(bbg, e->c[j]);
        if (!children[j]) { free(children); return NULL; }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_CASE);
    if (!bb) { free(children); return NULL; }
    if (e->n > 0) bb->α = children[0];
    for (int j = 1; j < e->n; j++) children[j-1]->γ = children[j];
    free(children);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Return(IR_graph_t *bbg, struct tree_t *e) {
    IR_t *bb = IR_node_alloc(bbg, IR_RETURN);
    if (!bb) return NULL;
    if (e->n >= 1 && e->c[0]) {
        IR_t *retval = lower_expr_node(bbg, e->c[0]);
        if (!retval) return NULL;
        bb->α = retval;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Suspend(IR_graph_t *bbg, struct tree_t *e) {
    if (bbg->lang != IR_LANG_RKU) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_SUSPEND);
    if (!bb) return NULL;
    if (e->n >= 1 && e->c[0]) {
        IR_t *val = lower_expr_node(bbg, e->c[0]);
        if (!val) return NULL;
        bb->α = val;
    }
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Break(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_BREAK);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Next(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_NEXT);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Initial(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) {
        IR_t *bb = IR_node_alloc(bbg, IR_SUCCEED);
        return bb;
    }
    IR_t *body = lower_expr_node(bbg, e->c[0]);
    if (!body) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_INITIAL);
    if (!bb) return NULL;
    bb->α = body;
    bb->ival = 0;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Unop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1 || !e->c[0]) {
        if (e->t == TT_NULL) return IR_node_alloc(bbg, IR_LIT_NUL);
        return NULL;
    }
    IR_t *inner = lower_expr_node(bbg, e->c[0]);
    if (!inner) return NULL;
    IR_e kind;
    switch (e->t) {
    case TT_MNS:         kind = IR_NEG;         break;
    case TT_PLS:         kind = IR_POS;         break;
    case TT_SIZE:        kind = IR_SIZE;        break;
    case TT_NULL:        kind = IR_NULL_TEST;   break;
    case TT_NONNULL:     kind = IR_NONNULL;     break;
    case TT_RANDOM:      kind = IR_RANDOM;      break;
    case TT_CSET_COMPL:  kind = IR_CSET_COMPL;  break;
    case TT_ITERATE:     kind = IR_LIST_BANG;   break;
    case TT_MATCH_UNARY: {
        IR_t *bb = IR_node_alloc(bbg, IR_CALL);
        if (!bb) return NULL;
        bb->sval = "match";
        bb->α = inner;
        return bb;
    }
    default: return NULL;
    }
    IR_t *bb = IR_node_alloc(bbg, kind);
    if (!bb) return NULL;
    bb->α = inner;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_AugOp(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    BinopKind op = BINOP_ADD;
    switch ((AugOp_e)e->v.ival) {
    case AUGOP_ADD:    op = BINOP_ADD;    break;
    case AUGOP_SUB:    op = BINOP_SUB;    break;
    case AUGOP_MUL:    op = BINOP_MUL;    break;
    case AUGOP_DIV:    op = BINOP_DIV;    break;
    case AUGOP_MOD:    op = BINOP_MOD;    break;
    case AUGOP_POW:    op = BINOP_POW;    break;
    case AUGOP_CONCAT: op = BINOP_CONCAT; break;
    case AUGOP_EQ:     op = BINOP_EQ;     break;
    case AUGOP_LT:     op = BINOP_LT;     break;
    case AUGOP_LE:     op = BINOP_LE;     break;
    case AUGOP_GT:     op = BINOP_GT;     break;
    case AUGOP_GE:     op = BINOP_GE;     break;
    case AUGOP_NE:     op = BINOP_NE;     break;
    default:           return NULL;
    }
    IR_t *binop = IR_node_alloc(bbg, IR_BINOP);
    if (!binop) return NULL;
    binop->α = lhs;
    binop->β = rhs;
    binop->ival = (int64_t)op;
    IR_t *asgn = IR_node_alloc(bbg, IR_ASSIGN);
    if (!asgn) return NULL;
    IR_t *lhs2 = lower_expr_node(bbg, e->c[0]);
    if (!lhs2) return NULL;
    asgn->α = lhs2;
    asgn->β = binop;
    return asgn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Create(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Mutual(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Key(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Invocable(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Link(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_RepAlt(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_CoexpList(IR_graph_t *bbg, struct tree_t *e) {
    (void)bbg; (void)e;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Var(IR_graph_t *bbg, struct tree_t *e) {
    if (!e->v.sval) return NULL;
    if (e->v.sval[0] == '&') {
        IR_t *bb = IR_node_alloc(bbg, IR_KEYWORD);
        if (!bb) return NULL;
        bb->sval = e->v.sval;
        return bb;
    }
    IR_t *bb = IR_node_alloc(bbg, IR_VAR);
    if (!bb) return NULL;
    bb->sval = e->v.sval;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Keyword(IR_graph_t *bbg, struct tree_t *e) {
    if (!e->v.sval) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_KEYWORD);
    if (!bb) return NULL;
    bb->sval = e->v.sval;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Assign(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->c[0]->t == TT_FIELD) {
        const char *fname = FIELD_NAME(e->c[0]);
        if (!fname) return NULL;
        IR_t *obj = lower_expr_node(bbg, e->c[0]->c[0]);
        if (!obj) return NULL;
        IR_t *rhs = lower_expr_node(bbg, e->c[1]);
        if (!rhs) return NULL;
        IR_t *bb = IR_node_alloc(bbg, IR_FIELD_SET);
        if (!bb) return NULL;
        bb->sval = fname;
        bb->α = obj;
        bb->β = rhs;
        return bb;
    }
    if (e->c[0]->t == TT_IDX) {
        if (e->c[0]->n < 2 || !e->c[0]->c[0] || !e->c[0]->c[1]) return NULL;
        IR_t *base = lower_expr_node(bbg, e->c[0]->c[0]);
        if (!base) return NULL;
        IR_t *idx  = lower_expr_node(bbg, e->c[0]->c[1]);
        if (!idx) return NULL;
        IR_t *rhs  = lower_expr_node(bbg, e->c[1]);
        if (!rhs) return NULL;
        IR_t *bb = IR_node_alloc(bbg, IR_IDX_SET);
        if (!bb) return NULL;
        bb->α = base;
        bb->β = idx;
        idx->γ = rhs;
        return bb;
    }
    if (e->c[0]->t != TT_VAR) return NULL;
    if (e->c[0]->v.sval && e->c[0]->v.sval[0] == '&') return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_ASSIGN);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Swap(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    if (e->c[0]->t != TT_VAR || e->c[1]->t != TT_VAR) return NULL;
    if (!e->c[0]->v.sval || !e->c[1]->v.sval) return NULL;
    if (e->c[0]->v.sval[0] == '&' || e->c[1]->v.sval[0] == '&') return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_SWAP);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_SeqExpr(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 1) return NULL;
    IR_t **stmts = calloc((size_t)e->n, sizeof(IR_t *));
    if (!stmts) return NULL;
    for (int j = 0; j < e->n; j++) {
        if (!e->c[j]) { free(stmts); return NULL; }
        stmts[j] = lower_expr_node(bbg, e->c[j]);
        if (!stmts[j]) { free(stmts); return NULL; }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_SEQ_EXPR);
    if (!bb) { free(stmts); return NULL; }
    if (e->n > 0) bb->α = stmts[0];
    for (int j = 1; j < e->n; j++) stmts[j-1]->γ = stmts[j];
    bb->ival = e->n;
    free(stmts);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Decl(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_SUCCEED);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Idx(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *base = lower_expr_node(bbg, e->c[0]);
    if (!base) return NULL;
    IR_t *idx  = lower_expr_node(bbg, e->c[1]);
    if (!idx) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_IDX);
    if (!bb) return NULL;
    bb->α = base;
    bb->β = idx;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Identical(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_IDENTICAL);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_ProcFail(IR_graph_t *bbg, struct tree_t *e) {
    (void)e;
    return IR_node_alloc(bbg, IR_FAIL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_CsetBinop(IR_graph_t *bbg, struct tree_t *e) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t *lhs = lower_expr_node(bbg, e->c[0]);
    if (!lhs) return NULL;
    IR_t *rhs = lower_expr_node(bbg, e->c[1]);
    if (!rhs) return NULL;
    IR_e kind = (e->t == TT_CSET_UNION) ? IR_CSET_UNION
                 : (e->t == TT_CSET_DIFF)  ? IR_CSET_DIFF
                                           : IR_CSET_INTER;
    IR_t *bb = IR_node_alloc(bbg, kind);
    if (!bb) return NULL;
    bb->α = lhs;
    bb->β = rhs;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Record(IR_graph_t *bbg, struct tree_t *e) {
    if (!e->v.sval) return NULL;
    char spec[512]; int pos = 0;
    pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", e->v.sval);
    for (int i = 0; i < e->n && pos < (int)sizeof(spec)-2; i++) {
        if (i > 0) spec[pos++] = ',';
        const char *fn2 = (e->c[i] && e->c[i]->v.sval) ? e->c[i]->v.sval : "";
        pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
    }
    if (pos < (int)sizeof(spec)-1) spec[pos++] = ')';
    spec[pos] = '\0';
    IR_t *bb = IR_node_alloc(bbg, IR_RECORD_DEF);
    if (!bb) return NULL;
    bb->sval = GC_strdup(spec);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_new_Makelist(IR_graph_t *bbg, struct tree_t *e) {
    int n = e->n;
    IR_t **args = (n > 0) ? calloc((size_t)n, sizeof(IR_t *)) : NULL;
    for (int j = 0; j < n; j++) {
        if (!e->c[j]) { if (args) free(args); return NULL; }
        args[j] = lower_expr_node(bbg, e->c[j]);
        if (!args[j]) { free(args); return NULL; }
    }
    IR_t *bb = IR_node_alloc(bbg, IR_CALL);
    if (!bb) { if (args) free(args); return NULL; }
    bb->sval = "MAKELIST";
    bb->ival = n;
    if (args && n >= 1) {
        bb->α = args[0];
        for (int j = 0; j + 1 < n; j++) args[j]->γ = args[j + 1];
    }
    if (args) free(args);
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int is_suspendable(tree_t *e);
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail) {
    *rel_fail = 0;
    if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
    int either_real = (IS_REAL_fn(lv) || IS_REAL_fn(rv));
    double ld = IS_REAL_fn(lv) ? lv.r : (double)(IS_INT_fn(lv) ? lv.i : 0);
    double rd = IS_REAL_fn(rv) ? rv.r : (double)(IS_INT_fn(rv) ? rv.i : 0);
    long   li = IS_INT_fn(lv) ? lv.i : (long)lv.r;
    long   ri = IS_INT_fn(rv) ? rv.i : (long)rv.r;
    DESCR_t real_result;
    switch (op) {
        case BINOP_ADD: if (either_real) { real_result.v=DT_R; real_result.r=ld+rd; return real_result; } return INTVAL(li + ri);
        case BINOP_SUB: if (either_real) { real_result.v=DT_R; real_result.r=ld-rd; return real_result; } return INTVAL(li - ri);
        case BINOP_MUL: if (either_real) { real_result.v=DT_R; real_result.r=ld*rd; return real_result; } return INTVAL(li * ri);
        case BINOP_DIV: if (either_real) { if (rd == 0.0) return FAILDESCR; real_result.v=DT_R; real_result.r=ld/rd; return real_result; } return ri ? INTVAL(li / ri) : FAILDESCR;
        case BINOP_MOD: return ri ? INTVAL(li % ri) : FAILDESCR;
        case BINOP_POW: { double base = either_real ? ld : (double)li; double exp2 = either_real ? rd : (double)ri; double r = pow(base, exp2); real_result.v = DT_R; real_result.r = r; return real_result; }
        case BINOP_LT: *rel_fail = !(either_real ? ld <  rd : li <  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_LE: *rel_fail = !(either_real ? ld <= rd : li <= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GT: *rel_fail = !(either_real ? ld >  rd : li >  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GE: *rel_fail = !(either_real ? ld >= rd : li >= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_EQ: *rel_fail = !(either_real ? ld == rd : li == ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_NE: *rel_fail = !(either_real ? ld != rd : li != ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_CONCAT: {
            DESCR_t ls_d; ls_d = descr_to_str_icn(lv);
            DESCR_t rs_d; rs_d = descr_to_str_icn(rv);
            if (IS_FAIL_fn(ls_d) || IS_FAIL_fn(rs_d)) return FAILDESCR;
            const char *ls = ls_d.s ? ls_d.s : "";
            const char *rs = rs_d.s ? rs_d.s : "";
            size_t ll = ls_d.slen > 0 ? (size_t)ls_d.slen : strlen(ls);
            size_t rl = rs_d.slen > 0 ? (size_t)rs_d.slen : strlen(rs);
            char *buf = GC_malloc(ll + rl + 1);
            memcpy(buf, ls, ll); memcpy(buf + ll, rs, rl); buf[ll + rl] = '\0';
            { DESCR_t r2; r2.v = DT_S; r2.slen = (int)(ll + rl); r2.s = buf; return r2; }
        }
        case BINOP_SLT: case BINOP_SLE: case BINOP_SGT:
        case BINOP_SGE: case BINOP_SEQ: case BINOP_SNE: {
            DESCR_t ls_d = descr_to_str_icn(lv);
            DESCR_t rs_d = descr_to_str_icn(rv);
            const char *ls = (!IS_FAIL_fn(ls_d) && ls_d.s) ? ls_d.s : "";
            const char *rs = (!IS_FAIL_fn(rs_d) && rs_d.s) ? rs_d.s : "";
            int cmp = strcmp(ls, rs);
            int ok;
            switch (op) {
            case BINOP_SLT: ok = (cmp <  0); break;
            case BINOP_SLE: ok = (cmp <= 0); break;
            case BINOP_SGT: ok = (cmp >  0); break;
            case BINOP_SGE: ok = (cmp >= 0); break;
            case BINOP_SEQ: ok = (cmp == 0); break;
            case BINOP_SNE: ok = (cmp != 0); break;
            default:            ok = 0;           break;
            }
            *rel_fail = !ok;
            return ok ? rv : FAILDESCR;
        }
        default: return FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fold_signed_lit(tree_t *n, int64_t *iv, double *dv, int *is_real) {
    if (!n) return 0;
    double sign = 1.0;
    while (n && (n->t == TT_MNS || n->t == TT_PLS)) {
        if (n->t == TT_MNS) sign = -sign;
        if (n->n < 1 || !n->c[0]) return 0;
        n = n->c[0];
    }
    if (!n) return 0;
    if (n->t == TT_ILIT) {
        if (sign < 0.0) { *iv = -n->v.ival; } else { *iv = n->v.ival; }
        *is_real = 0;
        return 1;
    }
    if (n->t == TT_FLIT) {
        *dv = sign * n->v.dval;
        *is_real = 1;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_upto(const char *cset, const char *hay) {
    if (!cset || !hay) return NULL;
    IR_graph_t *bbg = IR_alloc(4, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_UPTO);
    if (!bb) return NULL;
    bb->sval    = cset;
    bb->counter = 0;
    bb->α = bb;
    bb->β = bb;
    bb->γ = NULL;
    bb->ω = NULL;
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_proc_gen(GeneratorState *gs) {
    if (!gs) return NULL;
    IR_graph_t *bbg = IR_alloc(4, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_PROC_GEN);
    if (!bb) return NULL;
    bb->counter = (int64_t)(uintptr_t)gs;
    bb->α      = bb;
    bb->β      = bb;
    bb->γ      = NULL;
    bb->ω      = NULL;
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_expr_node(IR_graph_t *bbg, tree_t *e) {
    if (!bbg || !e) return NULL;
    switch (e->t) {
    case TT_ILIT:           return lower_new_Intlit(bbg, e);
    case TT_FLIT:           return lower_new_Reallit(bbg, e);
    case TT_QLIT:           return lower_new_Stringlit(bbg, e);
    case TT_CSET:           return lower_new_Csetlit(bbg, e);
    case TT_VAR:            return lower_new_Var(bbg, e);
    case TT_KEYWORD:        return lower_new_Keyword(bbg, e);
    case TT_SCAN:           return lower_new_Scan(bbg, e);
    case TT_ASSIGN:         return lower_new_Assign(bbg, e);
    case TT_SWAP:           return lower_new_Swap(bbg, e);
    case TT_FNC:            return lower_new_Call(bbg, e);
    case TT_IF:             return lower_new_If(bbg, e);
    case TT_TO:             return lower_new_ToBy(bbg, e);
    case TT_TO_BY:          return lower_new_ToBy(bbg, e);
    case TT_EVERY:          return lower_new_Every(bbg, e);
    case TT_WHILE:          return lower_new_While(bbg, e);
    case TT_UNTIL:          return lower_new_Until(bbg, e);
    case TT_SEQ_EXPR:       return lower_new_SeqExpr(bbg, e);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
                            return lower_new_Binop(bbg, e);
    case TT_GLOBAL:         return lower_new_Global(bbg, e);
    case TT_LOCAL:          return lower_new_Decl(bbg, e);
    case TT_STATIC_DECL:    return lower_new_Decl(bbg, e);
    case TT_INITIAL:        return lower_new_Initial(bbg, e);
    case TT_RETURN:         return lower_new_Return(bbg, e);
    case TT_SUSPEND:        return lower_new_Suspend(bbg, e);
    case TT_SEQ:            return lower_new_Conjunction(bbg, e);
    case TT_SIZE:           return lower_new_Unop(bbg, e);
    case TT_IDX:            return lower_new_Idx(bbg, e);
    case TT_SECTION:        return lower_new_Sectionop(bbg, e);
    case TT_SECTION_PLUS:   return lower_new_Sectionop(bbg, e);
    case TT_SECTION_MINUS:  return lower_new_Sectionop(bbg, e);
    case TT_CASE:           return lower_new_Case(bbg, e);
    case TT_NOT:            return lower_new_Not(bbg, e);
    case TT_REPEAT:         return lower_new_Repeat(bbg, e);
    case TT_ALTERNATE:      return lower_new_Alt(bbg, e);
    case TT_LIMIT:          return lower_new_Limitation(bbg, e);
    case TT_AUGOP:          return lower_new_AugOp(bbg, e);
    case TT_LOOP_BREAK:     return lower_new_Break(bbg, e);
    case TT_LOOP_NEXT:      return lower_new_Next(bbg, e);
    case TT_PROC_FAIL:      return lower_new_ProcFail(bbg, e);
    case TT_IDENTICAL:      return lower_new_Identical(bbg, e);
    case TT_NONNULL:        return lower_new_Unop(bbg, e);
    case TT_NULL:           return lower_new_Unop(bbg, e);
    case TT_RANDOM:         return lower_new_Unop(bbg, e);
    case TT_MATCH_UNARY:    return lower_new_Unop(bbg, e);
    case TT_MNS:            return lower_new_Unop(bbg, e);
    case TT_PLS:            return lower_new_Unop(bbg, e);
    case TT_CSET_COMPL:     return lower_new_Unop(bbg, e);
    case TT_CSET_UNION:     return lower_new_CsetBinop(bbg, e);
    case TT_CSET_DIFF:      return lower_new_CsetBinop(bbg, e);
    case TT_CSET_INTER:     return lower_new_CsetBinop(bbg, e);
    case TT_FIELD:          return lower_new_Field(bbg, e);
    case TT_RECORD:         return lower_new_Record(bbg, e);
    case TT_MAKELIST:       return lower_new_Makelist(bbg, e);
    case TT_ITERATE:        return lower_new_Unop(bbg, e);
    default:                return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_proc_body(tree_t *proc) {
    if (!proc) return NULL;
    tree_t **body_arr = NULL;
    int body_off = 0;
    int n_stmts = 0;
    if (proc->t == TT_PROC_DECL) {
        tree_t *body_node = (proc->n >= 3) ? proc->c[2] : NULL;
        if (!body_node) return NULL;
        body_arr = body_node->c;
        n_stmts = body_node->n;
    } else if (proc->t == TT_SUB_DECL) {
        int nparams = (int)proc->v.ival;
        body_off = nparams + 1;
        if (proc->n <= body_off) return NULL;
        body_arr = proc->c;
        n_stmts = proc->n - body_off;
    } else {
        return NULL;
    }
    if (n_stmts <= 0 || !body_arr) return NULL;
    int _lang_tag = (proc->t == TT_SUB_DECL) ? IR_LANG_RKU : IR_LANG_ICN;
    IR_graph_t *bbg = IR_alloc(4096, _lang_tag);
    if (!bbg) return NULL;
    IR_t **stmt_nodes = calloc((size_t)n_stmts, sizeof(IR_t *));
    if (!stmt_nodes) { IR_free(bbg); return NULL; }
    IR_t **stmt_entries = calloc((size_t)n_stmts, sizeof(IR_t *));
    if (!stmt_entries) { free(stmt_nodes); IR_free(bbg); return NULL; }
    int built = 0;
    IR_t *fail_term = IR_node_alloc(bbg, IR_FAIL);
    if (!fail_term) { free(stmt_nodes); free(stmt_entries); IR_free(bbg); return NULL; }
    IR_t *succ = fail_term;
    for (int i = n_stmts - 1; i >= 0; i--) {
        tree_t *st = body_arr[body_off + i];
        if (!st) continue;
        IR_t *aα = NULL, *aβ = NULL;
        IR_t *bb = lower_expr_threaded_b(bbg, st, succ, succ, &aα, &aβ, 1);
        if (!bb) { free(stmt_nodes); free(stmt_entries); IR_free(bbg); return NULL; }
        stmt_nodes[i]   = bb;
        stmt_entries[i] = aα ? aα : bb;
        succ = stmt_entries[i];
        built++;
    }
    IR_t *seq = IR_node_alloc(bbg, IR_SEQ);
    if (!seq) { free(stmt_nodes); free(stmt_entries); IR_free(bbg); return NULL; }
    { IR_t *head = NULL; for (int i = 0; i < n_stmts; i++) if (stmt_entries[i]) { head = stmt_entries[i]; break; } seq->α = head; }
    seq->ival = built;
    {
        IR_t *prev = NULL;
        for (int i = 0; i < n_stmts; i++) {
            IR_t *cur_entry = stmt_entries[i];
            if (!cur_entry) continue;
            int prev_terminal_kind = prev && (prev->t == IR_RETURN || prev->t == IR_FAIL ||
                                              prev->t == IR_BREAK  || prev->t == IR_NEXT);
            if (prev && !prev->ω && !prev_terminal_kind) prev->ω = cur_entry;
            prev = stmt_nodes[i];
        }
        if (prev) {
            int prev_terminal = (prev->t == IR_RETURN || prev->t == IR_FAIL ||
                                 prev->t == IR_BREAK  || prev->t == IR_NEXT);
            if (!prev->γ && !prev_terminal) prev->γ = fail_term;
            if (!prev->ω && !prev_terminal) prev->ω = fail_term;
        }
    }
    free(stmt_nodes);
    free(stmt_entries);
    bbg->entry = seq;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_expr_top(struct tree_t *e) {
    if (!e) return NULL;
    IR_graph_t *bbg = IR_alloc(256, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = lower_expr_node(bbg, e);
    if (!bb) { IR_free(bbg); return NULL; }
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kind_is_resumable(IR_e t) {
    return t == IR_TO || t == IR_TO_BY || t == IR_UPTO || t == IR_ALT ||
           t == IR_BINOP_GEN || t == IR_ITERATE || t == IR_LIMIT || t == IR_PROC_GEN ||
           t == IR_EVERY || t == IR_REPEAT || t == IR_SUSPEND || t == IR_SCAN ||
           t == IR_LIST_BANG || t == IR_KEY_GEN || t == IR_FIND_GEN || t == IR_SEQ_GEN ||
           t == IR_GEN_SCAN || t == IR_CONJ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *gen_leaf(IR_t *bb, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out, int bounded) {
    if (!bb) return NULL;
    if (!bb->γ && γ_in) bb->γ = γ_in;
    if (!bb->ω && ω_in) bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = (!bounded && kind_is_resumable(bb->t)) ? bb : ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int tree_is_leaf(const tree_t *e) {
    if (!e) return 0;
    switch (e->t) {
    case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_CSET:
    case TT_VAR:  case TT_KEYWORD:
    case TT_LOOP_BREAK: case TT_LOOP_NEXT: case TT_PROC_FAIL:
        return 1;
    default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_expr_threaded_b(IR_graph_t *bbg, tree_t *e,
                                 IR_t *γ_in, IR_t *ω_in,
                                 IR_t **α_out, IR_t **β_out, int bounded) {
    if (!bbg || !e) return NULL;
    int is_leaf = tree_is_leaf(e);
    if (e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_DIV ||
        e->t == TT_MOD || e->t == TT_POW ||
        e->t == TT_LT  || e->t == TT_LE  || e->t == TT_GT  || e->t == TT_GE  ||
        e->t == TT_EQ  || e->t == TT_NE  || e->t == TT_CAT ||
        e->t == TT_LLT || e->t == TT_LLE || e->t == TT_LGT ||
        e->t == TT_LGE || e->t == TT_LEQ || e->t == TT_LNE) {
        if (e->n >= 2 && e->c[0] && e->c[1]) {
            IR_t *ag = lower_new_Binop_ag(bbg, e, γ_in, ω_in, α_out, β_out);
            if (ag) return ag;
        }
    }
    if (e->t == TT_LCONCAT && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_Lconcat_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if ((e->t == TT_SECTION || e->t == TT_SECTION_PLUS || e->t == TT_SECTION_MINUS) &&
        e->n >= 3 && e->c[0] && e->c[1] && e->c[2]) {
        IR_t *ag = lower_new_Sectionop_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_Idx_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_ASSIGN && e->n >= 2 && e->c[0] && e->c[1] && e->c[0]->t == TT_IDX) {
        IR_t *ag = lower_new_Idx_set_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_IF && e->n >= 1 && e->c[0]) {
        IR_t *ag = lower_new_If_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_SEQ && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_Conjunction_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_ALTERNATE && e->n >= 1) {
        IR_t *ag = lower_new_Alt_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if (e->t == TT_EVERY && e->n >= 1 && e->c[0]) {
        IR_t *ag = lower_new_Every_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    if ((e->t == TT_TO || e->t == TT_TO_BY) && e->n >= 2 && e->c[0] && e->c[1]) {
        IR_t *ag = lower_new_ToBy_ag(bbg, e, γ_in, ω_in, α_out, β_out);
        if (ag) return ag;
    }
    IR_t *bb = lower_expr_node(bbg, e);
    if (!bb) return NULL;
    if (e->t == TT_ASSIGN && e->n >= 2 && e->c[0] && e->c[1] && e->c[0]->t == TT_VAR &&
        e->c[0]->v.sval && e->c[0]->v.sval[0] != '&' &&
        bb->t == IR_ASSIGN && bb->α && bb->β) {
        IR_t *rhs = bb->β;
        if (!rhs->γ) rhs->γ = bb;
        if (!rhs->ω) rhs->ω = ω_in;
        bb->ival = 1;
        if (!bb->γ && γ_in) bb->γ = γ_in;
        if (!bb->ω && ω_in) bb->ω = ω_in;
        if (α_out) *α_out = rhs;
        if (β_out) *β_out = ω_in;
        return bb;
    }
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval &&
        bb->t == IR_CALL && (int)bb->ival >= 0) {
        int nargs = e->n - 1;
        int any_gen = 0;
        for (int j = 0; j < nargs; j++) {
            if (e->c[1 + j] && is_suspendable(e->c[1 + j])) { any_gen = 1; break; }
        }
        if (!any_gen) {
            if (nargs > 0 && bb->α) {
                IR_t *ax = bb->α;
                for (int j = 0; j < nargs && ax; j++) {
                    if (!ax->ω) ax->ω = ω_in;
                    IR_t *next_ax = ax->γ;
                    if (j == nargs - 1) {
                        if (!ax->γ || ax->γ == bb) ax->γ = bb;
                        next_ax = NULL;
                    }
                    ax = next_ax;
                }
            }
            bb->dval = 1.0;
            if (!bb->γ && γ_in) bb->γ = γ_in;
            if (!bb->ω && ω_in) bb->ω = ω_in;
            if (α_out) *α_out = (nargs > 0 && bb->α) ? bb->α : bb;
            if (β_out) *β_out = ω_in;
            return bb;
        }
    }
    return gen_leaf(bb, γ_in, ω_in, α_out, β_out, is_leaf ? 1 : bounded);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_expr_threaded(IR_graph_t *bbg, tree_t *e,
                               IR_t *γ_in, IR_t *ω_in,
                               IR_t **α_out, IR_t **β_out) {
    return lower_expr_threaded_b(bbg, e, γ_in, ω_in, α_out, β_out, 0);
}
