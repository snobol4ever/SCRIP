/*====================================================================================================================*/
/*====================================================================================================================*/
#define BB_DEFINE_NAMES
#include "lower.h"
#include "IR_interp_state.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <gc/gc.h>
int g_icn_postfix_resume = 0;
/*====================================================================================================================*/
/*====================================================================================================================*/
typedef enum { ROLE_VALUE = 0, ROLE_PATTERN = 1, ROLE_GOAL = 2 } lower_role_e;
typedef struct { int count; } pl_vars_t;
typedef struct {
    IR_graph_t * bbg;
    lower_role_e role;
    int          bounded;
    int          lang;
    IR_t       * loop_ω;
    IR_t       * loop_next;
    pl_vars_t  * pl_vars;
} lcx_t;
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower2(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_raku_for(lcx_t cx, const tree_t * range_t, const char * var, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_pascal_for(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_pascal_repeat(lcx_t cx, const tree_t * body_t, const tree_t * cond_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_raku_gather(lcx_t cx, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * v_raku_map_grep(lcx_t cx, int is_grep, const tree_t * closure_t, const tree_t * src_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * g_term(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * g_builtin(lcx_t cx, const char * fn, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static IR_t * lower_goal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * nalloc(lcx_t cx, IR_e kind) { return IR_node_alloc(cx.bbg, kind); }
/*--------------------------------------------------------------------------------------------------------------------*/
static void set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in) {
    if (!n) return;
    if (!n->γ && γ_in) n->γ = γ_in;
    if (!n->ω && ω_in) n->ω = ω_in;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β) {
    if (α_out) *α_out = α;
    if (β_out) *β_out = β;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int icn_proc_is_generator(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++)
        if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, name) == 0)
            return g_stage2.proc_table[i].is_generator;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int icn_is_global(const char * name) { return name ? is_global(name) : 0; }
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    for (int i = nkids - 1; i >= 0; i--) {
        if (!kids[i]) return NULL;
        IR_t * γi = (i + 1 < nkids) ? entry[i + 1] : node;
        IR_t * αi = NULL, * βi = NULL;
        IR_t * c = lower2(cx, kids[i], γi, ω_in  , &αi, &βi);
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
            if (zs->goals) { for (int i = 0; i < nkids; i++) zs->goals[i] = apply[i]; zs->ngoals = nkids; node->ival = (int64_t)(intptr_t)zs; }
        }
    }
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, entry[0], resume[nkids - 1]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    IR_t * arm_succ = (kind == IR_DISJ) ? γ_in : node;
    for (int j = nkids - 1; j >= 0; j--) {
        if (!kids[j]) return NULL;
        IR_t * ωj = (j + 1 < nkids) ? entry[j + 1] : ω_in;
        IR_t * αj = NULL, * βj = NULL;
        IR_t * arm = lower2(cx, kids[j], arm_succ, ωj, &αj, &βj);
        if (!arm) return NULL;
        if (!arm->γ) arm->γ = arm_succ;
        apply[j] = arm; entry[j] = αj ? αj : arm; resume[j] = βj;
    }
    bb_operand_aux_set(cx.bbg, node, apply, nkids);
    set_succ_fail(node, γ_in, ω_in);
    if (kind == IR_DISJ) node->α = entry[0];
    return ret(node, α_out, β_out, entry[0], node  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap) {
    int n = 0;
    if (!e) return 0;
    if (e->t == kind) {
        for (int i = 0; i < e->n; i++) { int got = flatten_seq(e->c[i], kind, out + n, cap - n); n += got; if (n >= cap) break; }
        return n;
    }
    if (n < cap) out[n++] = e;
    return n;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int tm(const tree_t * e, tree_e kind, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    if (tag && (!e->v.sval || strcmp(e->v.sval, tag) != 0)) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
int kind_is_resumable(IR_e t) {
    return t == IR_TO || t == IR_TO_BY || t == IR_UPTO || t == IR_ALT || t == IR_BINOP_GEN || t == IR_ITERATE || t == IR_LIMIT || t == IR_PROC_GEN ||
           t == IR_EVERY || t == IR_REPEAT || t == IR_SUSPEND || t == IR_SCAN || t == IR_LIST_BANG || t == IR_KEY_GEN || t == IR_FIND_GEN || t == IR_SEQ_GEN || t == IR_GATHER ||
           t == IR_GEN_SCAN || t == IR_CONJ || t == IR_MAP || t == IR_GREP ||
           t == IR_PAT_LIT || t == IR_PAT_ARB || t == IR_PAT_REM || t == IR_PAT_SPAN || t == IR_PAT_ANY || t == IR_PAT_NOTANY ||
           t == IR_PAT_BREAK || t == IR_PAT_LEN || t == IR_PAT_TAB || t == IR_PAT_ARBNO || t == IR_PAT_DEFER ||
           t == IR_PAT_BAL ||
           t == IR_PAT_ASSIGN_COND || t == IR_PAT_ASSIGN_IMM || t == IR_PAT_ATP;
}
static IR_t * emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!n) return NULL;
    set_succ_fail(n, γ_in, ω_in);
    IR_t * resume = (!cx.bounded && kind_is_resumable(n->t)) ? n : ω_in;
    return ret(n, α_out, β_out, n, resume);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_literal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_ILIT:    n = nalloc(cx, IR_LIT_I);   if (n) n->ival = e->v.ival; break;
    case TT_FLIT:    n = nalloc(cx, IR_LIT_F);   if (n) n->dval = e->v.dval; break;
    case TT_QLIT:    n = nalloc(cx, IR_LIT_S);   if (n) n->sval = e->v.sval ? e->v.sval : ""; break;
    case TT_CSET:    n = nalloc(cx, IR_LIT_S);   if (n) n->sval = e->v.sval ? e->v.sval : ""; break;
    case TT_NUL: case TT_NULL: n = nalloc(cx, IR_LIT_NUL); break;
    case TT_VAR:     n = nalloc(cx, IR_VAR);     if (n) n->sval = e->v.sval; break;
    case TT_NAME:    n = nalloc(cx, IR_VAR);     if (n) n->sval = e->v.sval; break;
    case TT_KEYWORD: n = nalloc(cx, IR_KEYWORD); if (n) n->sval = e->v.sval; break;
    default: return NULL;
    }
    if (n && n->t == IR_VAR && cx.lang == IR_LANG_ICN && icn_is_global(n->sval)) n->state = 1;
    return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_unop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    if (e->t == TT_ITERATE) {
        IR_t * bang = nalloc(cx, IR_LIST_BANG);
        if (!bang) return NULL;
        IR_t * oα = NULL, * oβ = NULL;
        IR_t * operand = lower2(cx, e->c[0], NULL, ω_in, &oα, &oβ);
        if (!operand) return NULL;
        bang->α = oα ? oα : operand;
        set_succ_fail(bang, γ_in, ω_in);
        return ret(bang, α_out, β_out, bang, bang);
    }
    IR_t * un = nalloc(cx, IR_UNOP);
    if (!un) return NULL;
    un->sval = e->v.sval;
    un->ival = (int64_t) e->t;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * child = lower2(cx, e->c[0], un  , ω_in, &eα, &eβ);
    if (!child) return NULL;
    set_succ_fail(un, γ_in, ω_in);
    return ret(un, α_out, β_out, eα, eβ);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int tt_is_relational(tree_e t) {
    return t==TT_LT||t==TT_LE||t==TT_GT||t==TT_GE||t==TT_EQ||t==TT_NE
         ||t==TT_LLT||t==TT_LLE||t==TT_LGT||t==TT_LGE||t==TT_LEQ||t==TT_LNE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int tt_to_binop(tree_e t) {
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
static IR_t * v_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * bin = nalloc(cx, IR_BINOP);
    if (!bin) return NULL;
    bin->sval = e->v.sval;
    bin->ival = (int64_t) tt_to_binop(e->t);
    bin->dval = tt_is_relational(e->t) ? 1.0 : 0.0;
    IR_t * e1α=NULL, * e1β=NULL, * e2α=NULL, * e2β=NULL;
    IR_t * c1 = lower2(cx, e->c[0], NULL  , ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = lower2(cx, e->c[1], bin  , e1β  , &e2α, &e2β);
    if (!c2) return NULL;
    if (!c1->γ) c1->γ = e2α;
    IR_t * binops[2] = { c1, c2 };
    bb_operand_aux_set(cx.bbg, bin, binops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return ret(bin, α_out, β_out, e1α, e2β);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int to_by_const_step(const tree_t * s, int64_t * out_bits, int * is_real) {
    if (!s) return 0;
    if (s->t == TT_ILIT) { *out_bits = s->v.ival; *is_real = 0; return 1; }
    if (s->t == TT_FLIT) { double d = s->v.dval; memcpy(out_bits, &d, sizeof(double)); *is_real = 1; return 1; }
    if ((s->t == TT_MNS || s->t == TT_PLS) && s->n >= 1 && s->c[0]) {
        if (!to_by_const_step(s->c[0], out_bits, is_real)) return 0;
        if (s->t == TT_MNS) {
            if (*is_real) { double d; memcpy(&d, out_bits, sizeof(double)); d = -d; memcpy(out_bits, &d, sizeof(double)); }
            else *out_bits = -(*out_bits);
        }
        return 1;
    }
    return 0;
}
static IR_t * v_to(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = nalloc(cx, (e->t == TT_TO_BY) ? IR_TO_BY : IR_TO);
    if (!node) return NULL;
    node->sval = "ag";
    if (e->t == TT_TO_BY && e->n >= 3 && e->c[2]) {
        int64_t bits = 0; int isr = 0;
        if (to_by_const_step(e->c[2], &bits, &isr)) { node->ival = bits; if (isr) node->sval = "ar"; }
    }
    IR_t * fα=NULL,*fβ=NULL,*tα=NULL,*tβ=NULL;
    IR_t * lo = lower2(cx, e->c[0], NULL  , ω_in, &fα, &fβ);
    if (!lo) return NULL;
    IR_t * hi = lower2(cx, e->c[1], node  , fβ  , &tα, &tβ);
    if (!hi) return NULL;
    if (!lo->γ) lo->γ = tα;
    IR_t * bounds[2] = { lo, hi };
    bb_operand_aux_set(cx.bbg, node, bounds, 2);
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, fα, node  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = nalloc(cx, IR_IF);
    if (!node) return NULL;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * c1α=NULL,*c1β=NULL, *c2α=NULL,*c2β=NULL, *c3α=NULL,*c3β=NULL;
    IR_t * thenα = NULL, * elseα = NULL;
    IR_t * c2 = lower2(cx, e->c[1], γ_in, ω_in, &c2α, &c2β);
    if (!c2) return NULL;
    thenα = c2α;
    if (e->n >= 3 && e->c[2]) {
        IR_t * c3 = lower2(cx, e->c[2], γ_in, ω_in, &c3α, &c3β);
        if (!c3) return NULL;
        elseα = c3α;
    } else {
        switch (cx.lang) {
        case IR_LANG_RKU:
        case IR_LANG_PAS: elseα = γ_in; break;
        default:          elseα = ω_in; break;
        }
    }
    IR_t * c1 = lower2(cb, e->c[0], thenα  , elseα  , &c1α, &c1β);
    if (!c1) return NULL;
    node->α = c1α;
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, c1α, node  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t bounded(lcx_t cx) { cx.bounded = 1; return cx; }
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t with_loop(lcx_t cx, IR_t * lω, IR_t * lnext) { cx.loop_ω = lω; cx.loop_next = lnext; return cx; }
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_graph_t * lower_value_subgraph(lcx_t cx, const tree_t * e) {
    IR_graph_t * blk = IR_alloc(256, cx.lang);
    if (!blk) return NULL;
    IR_t * vfail = IR_node_alloc(blk, IR_FAIL);
    lcx_t vcx = cx; vcx.bbg = blk;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * en = lower2(vcx, e, NULL, vfail, &eα, &eβ);
    if (!en) { IR_free(blk); return NULL; }
    (void) eβ;
    blk->entry = eα ? eα : en;
    return blk;
}
static IR_t * v_seq_concat_pair(lcx_t cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * node = nalloc(cx, IR_SEQ);
    if (!node) return NULL;
    node->dval = 1.0;
    IR_graph_t * lblk = lower_value_subgraph(cx, lhs);
    if (!lblk) return NULL;
    IR_graph_t * rblk = lower_value_subgraph(cx, rhs);
    if (!rblk) { IR_free(lblk); return NULL; }
    node->counter = (int64_t)(intptr_t) lblk;
    node->ival    = (int64_t)(intptr_t) rblk;
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, node  , ω_in  );
}
static IR_t * v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * kids[64];
    int nk = flatten_seq(e, e->t, kids, 64);
    if (nk < 1) return NULL;
    if (cx.lang != IR_LANG_SNO) return wire_seq(cx, IR_CONJ, kids, nk, γ_in, ω_in, α_out, β_out);
    if (nk == 1) return lower2(cx, kids[0], γ_in, ω_in, α_out, β_out);
    tree_t * left = (tree_t *) kids[0];
    for (int i = 1; i < nk; i++) {
        tree_t * pair = ast_node_new(TT_SEQ);
        if (!pair) return NULL;
        ast_push(pair, left);
        ast_push(pair, (tree_t *) kids[i]);
        left = pair;
    }
    return v_seq_concat_pair(cx, left->c[0], left->c[1], γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_alt(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1) return NULL;
    return wire_alt(cx, IR_ALT, (const tree_t * const *) e->c, e->n, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_every(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * ev = nalloc(cx, IR_EVERY);
    if (!ev) return NULL;
    IR_t * g1α=NULL,*g1β=NULL;
    IR_t * gen = lower2(cx, e->c[0], NULL  , ev  , &g1α, &g1β);
    if (!gen) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower2(bounded(cx), e->c[1], g1β  , g1β  , &b2α, &b2β);
        if (!body) return NULL;
        if (!gen->γ) gen->γ = b2α;
    } else {
        if (!gen->γ) gen->γ = g1β;
    }
    ev->α = g1α;
    set_succ_fail(ev, γ_in, ω_in);
    return ret(ev, α_out, β_out, g1α, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_raku_for(lcx_t cx, const tree_t * range_t, const char * var, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!range_t || !var) return NULL;
    IR_t * bind = nalloc(cx, IR_ASSIGN);
    if (!bind) return NULL;
    bind->sval = var;
    IR_t * gα = NULL, * gβ = NULL;
    IR_t * gen = lower2(cx, range_t, bind  , γ_in  , &gα, &gβ);
    if (!gen) return NULL;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * body = body_t ? lower2(bounded(cx), body_t, gβ  , gβ  , &bα, &bβ) : NULL;
    if (body_t && !body) return NULL;
    set_succ_fail(bind, body_t ? bα : gβ  , ω_in  );
    return ret(gen, α_out, β_out, gα  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_raku_gather(lcx_t cx, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!body_t) return NULL;
    int n = 0;
    if (body_t->t == TT_SEQ_EXPR) {
        for (int i = 0; i < body_t->n; i++) if (body_t->c[i] && body_t->c[i]->t == TT_SUSPEND) n++;
    } else if (body_t->t == TT_SUSPEND) {
        n = 1;
    }
    IR_t * g = nalloc(cx, IR_GATHER);
    if (!g) return NULL;
    g->ival = n;
    if (n > 0) {
        IR_graph_t ** subs = (IR_graph_t **) calloc((size_t) n, sizeof(IR_graph_t *));
        if (!subs) return NULL;
        int k = 0;
        if (body_t->t == TT_SEQ_EXPR) {
            for (int i = 0; i < body_t->n; i++) {
                const tree_t * s = body_t->c[i];
                if (!s || s->t != TT_SUSPEND) continue;
                const tree_t * payload = (s->n >= 1) ? s->c[0] : NULL;
                if (!payload) { free(subs); return NULL; }
                subs[k] = lower_value_subgraph(cx, payload);
                if (!subs[k]) { free(subs); return NULL; }
                k++;
            }
        } else {
            const tree_t * payload = (body_t->n >= 1) ? body_t->c[0] : NULL;
            if (!payload) { free(subs); return NULL; }
            subs[0] = lower_value_subgraph(cx, payload);
            if (!subs[0]) { free(subs); return NULL; }
        }
        g->counter = (int64_t)(intptr_t) subs;
    }
    set_succ_fail(g, γ_in, ω_in);
    return ret(g, α_out, β_out, g  , g  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_raku_map_grep(lcx_t cx, int is_grep, const tree_t * closure_t, const tree_t * src_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!closure_t || !src_t) return NULL;
    IR_t * mg = nalloc(cx, is_grep ? IR_GREP : IR_MAP);
    if (!mg) return NULL;
    IR_graph_t * src_sg = lower_value_subgraph(cx, src_t);
    if (!src_sg) return NULL;
    IR_graph_t * body_sg = lower_value_subgraph(cx, closure_t);
    if (!body_sg) { IR_free(src_sg); return NULL; }
    mg->counter = (int64_t)(intptr_t) src_sg;
    mg->ival    = (int64_t)(intptr_t) body_sg;
    mg->state   = 0;
    set_succ_fail(mg, γ_in, ω_in);
    return ret(mg, α_out, β_out, mg  , mg  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_raku_det_call(lcx_t cx, const char * fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!fn || nkids < 0) return NULL;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    call->sval = GC_strdup(fn);
    call->ival = nkids;
    call->dval = 2.0;
    IR_graph_t ** blks = (IR_graph_t **) calloc((size_t)(nkids > 0 ? nkids : 1), sizeof(IR_graph_t *));
    if (!blks) return NULL;
    lcx_t mv = cx; mv.role = ROLE_VALUE;
    for (int i = 0; i < nkids; i++) {
        if (!kids[i]) { free(blks); return NULL; }
        blks[i] = lower_value_subgraph(mv, kids[i]);
        if (!blks[i]) { free(blks); return NULL; }
    }
    call->counter = (int64_t)(intptr_t) blks;
    set_succ_fail(call, γ_in, ω_in);
    return ret(call, α_out, β_out, call  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_raku_mutate_writeback(lcx_t cx, const char * target, const char * pure_fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!target || !pure_fn || nkids < 1) return NULL;
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    as->sval = GC_strdup(target);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * call = v_raku_det_call(cx, pure_fn, kids, nkids, as  , ω_in, &cα, &cβ);
    if (!call) return NULL;
    (void) cβ;
    set_succ_fail(as, γ_in, ω_in);
    return ret(as, α_out, β_out, cα  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_raku_pop(lcx_t cx, const char * dst, const tree_t * arr, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!dst || !arr || arr->t != TT_VAR || !arr->v.sval) return NULL;
    const tree_t * k[1] = { arr };
    IR_t * o2α = NULL, * o2β = NULL;
    IR_t * op2 = v_raku_mutate_writeback(cx, arr->v.sval, "arr_init", k, 1, γ_in  , ω_in, &o2α, &o2β);
    if (!op2) return NULL;
    (void) o2β;
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    as->sval = GC_strdup(dst);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * call = v_raku_det_call(cx, "arr_last", k, 1, as  , ω_in, &cα, &cβ);
    if (!call) return NULL;
    (void) cβ;
    set_succ_fail(as, o2α  , ω_in);
    return ret(as, α_out, β_out, cα  , ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * wh = nalloc(cx, IR_WHILE);
    if (!wh) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = lower2(bounded(cx), e->c[0], NULL  , wh  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower2(with_loop(bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
        if (!body) return NULL;
        if (!cond->γ) cond->γ = b2α;
    } else {
        if (!cond->γ) cond->γ = c1α;
    }
    wh->α = c1α;
    set_succ_fail(wh, γ_in, ω_in);
    return ret(wh, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_until(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * un = nalloc(cx, IR_UNTIL);
    if (!un) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = lower2(bounded(cx), e->c[0], un  , NULL  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower2(with_loop(bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
        if (!body) return NULL;
        if (!cond->ω) cond->ω = b2α;
    } else {
        if (!cond->ω) cond->ω = c1α;
    }
    un->α = c1α;
    set_succ_fail(un, γ_in, ω_in);
    return ret(un, α_out, β_out, c1α, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_repeat(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * rp = nalloc(cx, IR_REPEAT);
    if (!rp) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * body = lower2(with_loop(bounded(cx), γ_in, rp), e->c[0], rp  , rp  , &eα, &eβ);
    if (!body) return NULL;
    rp->α = eα;
    set_succ_fail(rp, γ_in, ω_in);
    rp->γ = eα;
    return ret(rp, α_out, β_out, eα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * call = nalloc(cx, IR_CALL); if (!call) return NULL;
    call->sval = e->c[0]->v.sval;
    int nargs = e->n - 1;
    call->ival = nargs;
    call->dval = 3.0;
    if (nargs > 0) {
        IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
        if (!blks) return NULL;
        lcx_t ac = cx; ac.role = ROLE_VALUE;
        for (int i = 0; i < nargs; i++) {
            blks[i] = lower_value_subgraph(ac, e->c[i + 1]);
            if (!blks[i]) { free(blks); return NULL; }
        }
        call->counter = (int64_t)(intptr_t) blks;
    }
    set_succ_fail(call, γ_in, ω_in);
    IR_t * call_beta = (allow_generator && icn_proc_is_generator(call->sval)) ? call : ω_in;
    return ret(call, α_out, β_out, call, call_beta);
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
    IR_t * p1 = lower2(cx, op1_t, bin, ω_in, &p1α, &p1β); if (!p1) return NULL;
    set_succ_fail(op0, p1α, ω_in);
    IR_t * ops[2] = { op0, p1 }; bb_operand_aux_set(cx.bbg, bin, ops, 2);
    set_succ_fail(bin, γ_in, ω_in);
    return bin;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * v_pascal_for(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
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
    IR_t * bnode = lower2(cb, body, incr_entry, incr_entry, &bα, &bβ); if (!bnode) return NULL;
    if (!cond->γ) cond->γ = bα ? bα : bnode;
    IR_t * init = nalloc(cb, IR_ASSIGN); if (!init) return NULL; init->sval = (char *) vname;
    IR_t * fα = NULL, * fβ = NULL;
    IR_t * fnode = lower2(cb, from, init, ω_in, &fα, &fβ); if (!fnode) return NULL;
    set_succ_fail(init, cond_entry, ω_in);
    return ret(init, α_out, β_out, fα ? fα : fnode, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_pascal_repeat(lcx_t cx, const tree_t * body_t, const tree_t * cond_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!body_t || !cond_t) return NULL;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cond = lower2(bounded(cx), cond_t, γ_in, NULL, &cα, &cβ);
    if (!cond) return NULL;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * body = lower2(bounded(cx), body_t, cα, ω_in, &bα, &bβ);
    if (!body) return NULL;
    if (!cond->ω) cond->ω = bα;
    return ret(body, α_out, β_out, bα, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_loop_break(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void)γ_in;
    IR_t * br = nalloc(cx, IR_BREAK);
    if (!br) return NULL;
    IR_t * tgt = cx.loop_ω ? cx.loop_ω : ω_in;
    set_succ_fail(br, tgt, tgt);
    return ret(br, α_out, β_out, br, tgt);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_loop_next(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void)γ_in; (void)e;
    IR_t * nx = nalloc(cx, IR_NEXT);
    if (!nx) return NULL;
    IR_t * tgt = cx.loop_next ? cx.loop_next : ω_in;
    set_succ_fail(nx, tgt, tgt);
    return ret(nx, α_out, β_out, nx, tgt);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_not(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * nt = nalloc(cx, IR_NOT);
    if (!nt) return NULL;
    IR_t * eα=NULL,*eβ=NULL;
    IR_t * ce = lower2(bounded(cx), e->c[0], ω_in  , nt  , &eα, &eβ);
    if (!ce) return NULL;
    bb_operand_aux_set(cx.bbg, nt, &ce, 1);
    set_succ_fail(nt, γ_in, ω_in);
    return ret(nt, α_out, β_out, eα, ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * lhs_t = NULL, * rhs_t = NULL;
    if (!tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
    if (!lhs_t || !rhs_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    if (cx.lang == IR_LANG_PAS && lhs_t->t == TT_IDX && lhs_t->n >= 2 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval && lhs_t->c[1]) {
        const tree_t * k[3] = { lhs_t->c[0], lhs_t->c[1], rhs_t };
        return v_raku_mutate_writeback(cx, lhs_t->c[0]->v.sval, "arr_set_pure", k, 3, γ_in, ω_in, α_out, β_out);
    }
    if (cx.lang == IR_LANG_PAS && lhs_t->t == TT_FNC && lhs_t->n >= 1 && lhs_t->c[0] && lhs_t->c[0]->t == TT_VAR && lhs_t->c[0]->v.sval) {
        IR_t * as = nalloc(cx, IR_ASSIGN);
        if (!as) return NULL;
        as->sval = lhs_t->c[0]->v.sval;
        IR_t * rα = NULL, * rβ = NULL;
        IR_t * rhs = lower2(cx, rhs_t, as, ω_in, &rα, &rβ);
        if (!rhs) return NULL;
        (void) rβ;
        set_succ_fail(as, γ_in, ω_in);
        return ret(as, α_out, β_out, rα, ω_in);
    }
    int lhs_is_var = (lhs_t->t == TT_VAR);
    int lhs_is_kw  = (cx.lang == IR_LANG_SNO && lhs_t->t == TT_KEYWORD);
    if (!lhs_is_var && !lhs_is_kw) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    if (cx.lang == IR_LANG_RKU && lhs_is_var && lhs_t->v.sval && rhs_t->t == TT_FNC
        && rhs_t->n >= 2 && rhs_t->c[0] && rhs_t->c[0]->t == TT_VAR && rhs_t->c[0]->v.sval
        && !strcmp(rhs_t->c[0]->v.sval, "pop") && rhs_t->c[1] && rhs_t->c[1]->t == TT_VAR) {
        return v_raku_pop(cx, lhs_t->v.sval, rhs_t->c[1], γ_in, ω_in, α_out, β_out);
    }
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    as->sval = lhs_t->v.sval ? lhs_t->v.sval : "";
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower2(cx, rhs_t, as  , ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    set_succ_fail(as, γ_in, ω_in);
    IR_t * resume = (cx.lang == IR_LANG_ICN && !cx.bounded && rβ && rβ != ω_in) ? rβ : ω_in;
    return ret(as, α_out, β_out, rα, resume);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * v_scan(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (cx.lang == IR_LANG_ICN) {
        const tree_t * isubj_t = NULL, * ibody_t = NULL;
        if (!tm(e, TT_SCAN, 2, &isubj_t, &ibody_t) || !isubj_t || !ibody_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
        IR_t * gs = nalloc(cx, IR_GEN_SCAN);
        if (!gs) return NULL;
        IR_graph_t * subj_sg = lower_value_subgraph(cx, isubj_t);
        if (!subj_sg) return NULL;
        IR_graph_t * body_sg = lower_value_subgraph(cx, ibody_t);
        if (!body_sg) return NULL;
        gs->counter = (int64_t)(intptr_t) subj_sg;
        gs->ival    = (int64_t)(intptr_t) body_sg;
        gs->dval    = 1.0;
        set_succ_fail(gs, γ_in, ω_in);
        return ret(gs, α_out, β_out, gs, ω_in);
    }
    const tree_t * subj_t = NULL, * pat_t = NULL;
    if (!tm(e, TT_SCAN, 2, &subj_t, &pat_t) || !subj_t || !pat_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const tree_t * repl_t = (e->n >= 3) ? e->c[2] : NULL;
    if (repl_t && subj_t->t != TT_VAR) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * sc = nalloc(cx, IR_SCAN);
    if (!sc) return NULL;
    IR_graph_t * pat_blk = IR_alloc(256, IR_LANG_SNO);
    if (!pat_blk) return NULL;
    IR_t * psucc = IR_node_alloc(pat_blk, IR_SUCCEED);
    IR_t * pfail = IR_node_alloc(pat_blk, IR_FAIL);
    IR_t * pα = NULL, * pβ = NULL;
    lcx_t pcx = { pat_blk, ROLE_PATTERN, 0, IR_LANG_SNO, NULL, NULL };
    IR_t * pat_entry = lower2(pcx, pat_t, psucc, pfail, &pα, &pβ);
    if (!pat_entry) { IR_free(pat_blk); return NULL; }
    (void) pβ;
    pat_blk->entry = pα ? pα : pat_entry;
    sc->counter = (int64_t)(intptr_t)pat_blk;
    if (repl_t) {
        sc->sval = subj_t->v.sval ? subj_t->v.sval : "";
        sc->ival = 1;
        IR_graph_t * subj_blk = lower_value_subgraph(cx, subj_t);
        if (!subj_blk) { IR_free(pat_blk); return NULL; }
        IR_graph_t * repl_blk = lower_value_subgraph(cx, repl_t);
        if (!repl_blk) { IR_free(pat_blk); return NULL; }
        IR_t * scan_aux[2]; scan_aux[0] = (IR_t *)(void *)subj_blk; scan_aux[1] = (IR_t *)(void *)repl_blk;
        bb_operand_aux_set(cx.bbg, sc, scan_aux, 2);
        IR_t * rα = NULL, * rβ = NULL;
        IR_t * repln = lower2(cx, repl_t, sc, ω_in, &rα, &rβ);
        if (!repln) { IR_free(pat_blk); return NULL; }
        (void) rβ;
        set_succ_fail(sc, γ_in, ω_in);
        return ret(sc, α_out, β_out, rα ? rα : sc, ω_in);
    }
    IR_graph_t * subj_blk = lower_value_subgraph(cx, subj_t);
    if (!subj_blk) { IR_free(pat_blk); return NULL; }
    IR_t * scan_aux[1]; scan_aux[0] = (IR_t *)(void *)subj_blk;
    bb_operand_aux_set(cx.bbg, sc, scan_aux, 1);
    IR_t * sα = NULL, * sβ = NULL;
    IR_t * subj = lower2(cx, subj_t, sc, ω_in, &sα, &sβ);
    if (!subj) { IR_free(pat_blk); return NULL; }
    (void) sβ;
    set_succ_fail(sc, γ_in, ω_in);
    return ret(sc, α_out, β_out, sα ? sα : sc, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * lower_value(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    switch (e->t) {
    case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_CSET:
    case TT_NUL:  case TT_NULL: case TT_VAR:  case TT_NAME: case TT_KEYWORD:
        return v_literal(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUCCEED: { IR_t * n = nalloc(cx, IR_SUCCEED); if (!n) return NULL; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FAIL:    { IR_t * n = nalloc(cx, IR_FAIL);    if (!n) return NULL; lcx_t bx = cx; bx.bounded = 1; return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out); }
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL:
    case TT_RANDOM: case TT_MATCH_UNARY: case TT_CSET_COMPL: case TT_ITERATE: case TT_INTERROGATE:
        return v_unop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
    case TT_CAT: case TT_LCONCAT:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
        return v_binop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_TO: case TT_TO_BY:
        return v_to(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_IF:
        return v_if(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SEQ: case TT_SEQ_EXPR:
        return v_conj(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ALTERNATE: case TT_ALT:
        return v_alt(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_EVERY:
        if (cx.lang == IR_LANG_RKU && e->n >= 1 && e->c[0] && e->c[0]->t == TT_ITERATE
            && e->c[0]->n >= 1 && e->c[0]->c[0]
            && (e->c[0]->c[0]->t == TT_TO || e->c[0]->c[0]->t == TT_TO_BY || e->c[0]->c[0]->t == TT_GATHER
                || e->c[0]->c[0]->t == TT_MAP || e->c[0]->c[0]->t == TT_GREP)) {
            const char * v = (e->c[0]->v.sval && e->c[0]->v.sval[0]) ? e->c[0]->v.sval : "_";
            return v_raku_for(cx, e->c[0]->c[0], v, (e->n >= 2 ? e->c[1] : NULL), γ_in, ω_in, α_out, β_out);
        }
        return v_every(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_WHILE:
        return v_while(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_UNTIL:
        return v_until(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_REPEAT:
        if (cx.lang == IR_LANG_PAS && e->n >= 2 && e->c[0] && e->c[1])
            return v_pascal_repeat(cx, e->c[0], e->c[1], γ_in, ω_in, α_out, β_out);
        return v_repeat(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_NOT:
        return v_not(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_BREAK:
        if (cx.lang == IR_LANG_ICN) return v_loop_break(cx, e, γ_in, ω_in, α_out, β_out);
        break;
    case TT_LOOP_NEXT:
        if (cx.lang == IR_LANG_ICN) return v_loop_next(cx, e, γ_in, ω_in, α_out, β_out);
        break;
    case TT_ASSIGN:
        return v_assign(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_RETURN:
    case TT_NRETURN:
        if (cx.lang == IR_LANG_ICN) {
            IR_t * rn = nalloc(cx, IR_RETURN); if (!rn) return NULL;
            rn->dval = 0.0;
            IR_t * vα = NULL, * vβ = NULL;
            if (e->n >= 1 && e->c[0]) {
                lcx_t vc = cx; vc.role = ROLE_VALUE;
                IR_t * v = lower2(vc, e->c[0], rn  , ω_in, &vα, &vβ);
                if (!v) return NULL;
            }
            set_succ_fail(rn, γ_in, ω_in);
            return ret(rn, α_out, β_out, vα ? vα : rn, ω_in  );
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUSPEND:
        if (cx.lang == IR_LANG_ICN) {
            IR_t * sn = nalloc(cx, IR_SUSPEND); if (!sn) return NULL;
            sn->dval = 1.0;
            if (e->n >= 1 && e->c[0]) {
                IR_graph_t * eblk = lower_value_subgraph(cx, e->c[0]);
                if (!eblk) return NULL;
                sn->counter = (int64_t)(intptr_t) eblk;
            }
            if (e->n >= 2 && e->c[1]) {
                IR_graph_t * bblk = lower_value_subgraph(cx, e->c[1]);
                if (!bblk) return NULL;
                sn->ival = (int64_t)(intptr_t) bblk;
            }
            set_succ_fail(sn, γ_in, ω_in);
            return ret(sn, α_out, β_out, sn  , ω_in);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LIMIT:
    case TT_CASE:
    case TT_PROC_FAIL:
    case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP:
    case TT_FNC: {
        if (cx.lang == IR_LANG_SNO) {
            IR_t * call = nalloc(cx, IR_CALL); if (!call) return NULL;
            call->sval = e->v.sval ? e->v.sval : "";
            call->ival = e->n;
            call->dval = 2.0;
            if (e->n > 0) {
                IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) e->n, sizeof(IR_graph_t *));
                if (!blks) return NULL;
                for (int i = 0; i < e->n; i++) {
                    blks[i] = lower_value_subgraph(cx, e->c[i]);
                    if (!blks[i]) { free(blks); return NULL; }
                }
                call->counter = (int64_t)(intptr_t) blks;
            }
            set_succ_fail(call, γ_in, ω_in);
            return ret(call, α_out, β_out, call  , ω_in  );
        }
        if (cx.lang == IR_LANG_RKU && e->v.sval && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR) {
            const char * flav = e->v.sval;
            if (!strcmp(flav, "any") || !strcmp(flav, "all") || !strcmp(flav, "one") || !strcmp(flav, "none")) {
                char jfn[32];
                snprintf(jfn, sizeof jfn, "__rk_jct_%s", flav);
                int nmembers = e->n - 1;
                IR_t * call = nalloc(cx, IR_CALL);
                if (!call) return NULL;
                call->sval = GC_strdup(jfn);
                call->ival = nmembers;
                call->dval = 2.0;
                IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nmembers, sizeof(IR_graph_t *));
                if (!blks) return NULL;
                lcx_t mv = cx; mv.role = ROLE_VALUE;
                for (int i = 1; i <= nmembers; i++) {
                    blks[i - 1] = lower_value_subgraph(mv, e->c[i]);
                    if (!blks[i - 1]) { free(blks); return NULL; }
                }
                call->counter = (int64_t)(intptr_t) blks;
                set_succ_fail(call, γ_in, ω_in);
                return ret(call, α_out, β_out, call  , ω_in  );
            }
        }
        if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return wire_det_builtin1(cx, e->c[1], fn, γ_in, ω_in, α_out, β_out);
        }
        if (e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            switch (cx.lang) {
            case IR_LANG_ICN: return v_det_call(cx, e, 1, γ_in, ω_in, α_out, β_out);
            case IR_LANG_PAS: return v_det_call(cx, e, 0, γ_in, ω_in, α_out, β_out);
            default: break;
            }
        }
        if (cx.lang == IR_LANG_RKU && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            {
                static const struct { const char * name; const char * pure; int minargs; } RK_MUT[] = {
                    { "push", "push_pure", 3 }, { "hash_set", "hash_set_pure", 4 },
                    { "hash_delete", "hash_delete_pure", 3 }, { "arr_set", "arr_set_pure", 4 }, { NULL, NULL, 0 } };
                for (int i = 0; RK_MUT[i].name; i++) {
                    if (strcmp(fn, RK_MUT[i].name)) continue;
                    if (e->n < RK_MUT[i].minargs || !e->c[1] || e->c[1]->t != TT_VAR || !e->c[1]->v.sval) break;
                    int nk = e->n - 1;
                    const tree_t * kids[16];
                    if (nk > 16) nk = 16;
                    for (int j = 0; j < nk; j++) kids[j] = e->c[j + 1];
                    return v_raku_mutate_writeback(cx, e->c[1]->v.sval, RK_MUT[i].pure, kids, nk, γ_in, ω_in, α_out, β_out);
                }
            }
            static const char * const RK_PURE[] = {
                "__rk_arr", "elems", "reverse", "sort", "array_sort", "arr_get",
                "hash_get", "hash_exists", "hash_keys", "hash_values", "hash_pairs",
                "join", "sum", "unique", "head", "tail", "chars", "length",
                "lc", "uc", "trim", "substr", "index", "rindex", NULL };
            int is_pure = 0;
            for (int i = 0; RK_PURE[i]; i++) if (!strcmp(fn, RK_PURE[i])) { is_pure = 1; break; }
            if (is_pure) {
                int nk = e->n - 1;
                const tree_t * kids[16];
                if (nk > 16) nk = 16;
                for (int i = 0; i < nk; i++) kids[i] = e->c[i + 1];
                return v_raku_det_call(cx, fn, kids, nk, γ_in, ω_in, α_out, β_out);
            }
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    case TT_SAY:
    case TT_PRINT:
        if (cx.lang == IR_LANG_RKU && e->n >= 1 && e->c[0]) {
            const char * fn = (e->t == TT_SAY) ? "write" : "writes";
            return wire_det_builtin1(cx, e->c[0], fn, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_GATHER:
        if (cx.lang == IR_LANG_RKU && e->n >= 1 && e->c[0])
            return v_raku_gather(cx, e->c[0], γ_in, ω_in, α_out, β_out);
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_MAP:
    case TT_GREP:
        if (cx.lang == IR_LANG_RKU && e->n >= 2 && e->c[0] && e->c[1])
            return v_raku_map_grep(cx, (e->t == TT_GREP), e->c[0], e->c[1], γ_in, ω_in, α_out, β_out);
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_HASH_GET:
        if (cx.lang == IR_LANG_RKU && e->n >= 2 && e->c[0] && e->c[1]) {
            const tree_t * k[2] = { e->c[0], e->c[1] };
            return v_raku_det_call(cx, "hash_get", k, 2, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_HASH_EXISTS:
        if (cx.lang == IR_LANG_RKU && e->n >= 2 && e->c[0] && e->c[1]) {
            const tree_t * k[2] = { e->c[0], e->c[1] };
            return v_raku_det_call(cx, "hash_exists", k, 2, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ARR_GET:
        if (cx.lang == IR_LANG_RKU && e->n >= 2 && e->c[0] && e->c[1]) {
            const tree_t * k[2] = { e->c[0], e->c[1] };
            return v_raku_det_call(cx, "arr_get", k, 2, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SORT:
        if (cx.lang == IR_LANG_RKU && e->n >= 1 && e->c[0]) {
            const tree_t * k[1] = { e->c[0] };
            return v_raku_det_call(cx, "array_sort", k, 1, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_HASH_SET:
        if (cx.lang == IR_LANG_RKU && e->n >= 3 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && e->c[1] && e->c[2]) {
            const tree_t * k[3] = { e->c[0], e->c[1], e->c[2] };
            return v_raku_mutate_writeback(cx, e->c[0]->v.sval, "hash_set_pure", k, 3, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_HASH_DELETE:
        if (cx.lang == IR_LANG_RKU && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && e->c[1]) {
            const tree_t * k[2] = { e->c[0], e->c[1] };
            return v_raku_mutate_writeback(cx, e->c[0]->v.sval, "hash_delete_pure", k, 2, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ARR_SET:
        if (cx.lang == IR_LANG_RKU && e->n >= 3 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && e->c[1] && e->c[2]) {
            const tree_t * k[3] = { e->c[0], e->c[1], e->c[2] };
            return v_raku_mutate_writeback(cx, e->c[0]->v.sval, "arr_set_pure", k, 3, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SCAN:
        return v_scan(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_IDX:
        if (cx.lang == IR_LANG_PAS && e->n >= 2 && e->c[0] && e->c[1]) {
            const tree_t * k[2] = { e->c[0], e->c[1] };
            return v_raku_det_call(cx, "arr_get", k, 2, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SMATCH:
        if (cx.lang == IR_LANG_RKU && e->n >= 3 && e->c[0] && e->c[1] && e->c[2]
            && e->c[2]->t == TT_QLIT && e->c[2]->v.sval && strcmp(e->c[2]->v.sval, "match") == 0) {
            const tree_t * k[2] = { e->c[0], e->c[1] };
            return v_raku_det_call(cx, "re_match", k, 2, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_CAPTURE:
        if (cx.lang == IR_LANG_RKU && e->n >= 1 && e->c[0]) {
            const tree_t * k[1] = { e->c[0] };
            return v_raku_det_call(cx, "re_capture", k, 1, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_NAMED_CAPTURE:
        if (cx.lang == IR_LANG_RKU && e->n >= 1 && e->c[0]) {
            const tree_t * k[1] = { e->c[0] };
            return v_raku_det_call(cx, "re_named_capture", k, 1, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FIELD:
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS:
    case TT_INDIRECT: case TT_IDENTICAL:
    case TT_CSET_UNION: case TT_CSET_DIFF: case TT_CSET_INTER:
    case TT_MAKELIST: case TT_VLIST: case TT_RECORD: case TT_NEW:
    case TT_FOR:
        if (cx.lang == IR_LANG_PAS)
            return v_pascal_for(cx, e, γ_in, ω_in, α_out, β_out);
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_PRINT_FH: case TT_SAY_FH:
    case TT_GLOBAL: case TT_LOCAL: case TT_STATIC_DECL: case TT_DECL: case TT_INITIAL: case TT_OPSYN:
    case TT_GOTO_U: case TT_GOTO_S: case TT_GOTO_F:
    case TT_TRY: case TT_DIE: case TT_UNLESS: case TT_DO_WHILE:
        if (cx.lang == IR_LANG_ICN && (e->t == TT_LOCAL || e->t == TT_GLOBAL || e->t == TT_STATIC_DECL)) {
            IR_t * nop = nalloc(cx, IR_SUCCEED); if (!nop) return NULL;
            set_succ_fail(nop, γ_in, ω_in);
            return ret(nop, α_out, β_out, nop, ω_in);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FOR_RANGE:
        if (cx.lang == IR_LANG_RKU && e->n >= 4 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const tree_t * lo = e->c[1]; const tree_t * hi = e->c[2]; const tree_t * body = e->c[3];
            int ex = (e->n >= 5 && e->c[4] && e->c[4]->t == TT_ILIT) ? (int) e->c[4]->v.ival : 0;
            tree_t * hi_eff = (tree_t *) hi;
            if (ex) {
                tree_t * one = ast_node_new(TT_ILIT); one->v.ival = 1;
                hi_eff = ast_node_new(TT_SUB); ast_push(hi_eff, (tree_t *) hi); ast_push(hi_eff, one);
            }
            tree_t * rng = ast_node_new(TT_TO); ast_push(rng, (tree_t *) lo); ast_push(rng, hi_eff);
            return v_raku_for(cx, rng, e->c[0]->v.sval, body, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
char * cset_try_fold(const tree_t * t) { (void) t; return NULL; }
static int pat_cset_arg(const tree_t * arg, const char ** sval_out, double * varflag_out) {
    if (!arg) return 0;
    if (arg->t == TT_QLIT) { *sval_out = arg->v.sval ? arg->v.sval : ""; *varflag_out = 0.0; return 1; }
    if (arg->t == TT_VAR)  { *sval_out = arg->v.sval ? arg->v.sval : ""; *varflag_out = 1.0; return 1; }
    { char * cs = cset_try_fold(arg); if (!cs) return 0; *sval_out = cs; *varflag_out = 0.0; return 1; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_pattern(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * n = NULL;
    switch (e->t) {
    case TT_QLIT: n = nalloc(cx, IR_PAT_LIT); if (n) n->sval = e->v.sval ? e->v.sval : ""; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    case TT_ARB:  return emit_leaf(cx, nalloc(cx, IR_PAT_ARB), γ_in, ω_in, α_out, β_out);
    case TT_REM:  return emit_leaf(cx, nalloc(cx, IR_PAT_REM), γ_in, ω_in, α_out, β_out);
    case TT_BAL:  return emit_leaf(cx, nalloc(cx, IR_PAT_BAL), γ_in, ω_in, α_out, β_out);
    case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX: {
        if (e->n < 1 || !e->c[0]) return NULL;
        const char * sv = NULL; double vf = 0.0;
        if (!pat_cset_arg(e->c[0], &sv, &vf)) return NULL;
        IR_e k = (e->t==TT_SPAN)?IR_PAT_SPAN : (e->t==TT_ANY)?IR_PAT_ANY
               : (e->t==TT_NOTANY)?IR_PAT_NOTANY : IR_PAT_BREAK;
        n = nalloc(cx, k); if (!n) return NULL;
        n->sval = sv;
        if (e->t==TT_SPAN) n->ival = (vf!=0.0)?1:0;
        else               n->dval = vf;
        if (e->t==TT_BREAKX) n->ival = 1; else if (e->t==TT_BREAK) n->ival = 0;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_SEQ: case TT_CAT: {
        const tree_t * kids[64];
        int nk = flatten_seq(e, e->t, kids, 64);
        if (nk < 1) return NULL;
        if (nk == 1) return lower2(cx, kids[0], γ_in, ω_in, α_out, β_out);
        return wire_seq(cx, IR_PAT_CAT, kids, nk, γ_in, ω_in, α_out, β_out);
    }
    case TT_ALT:
        if (e->n < 1) return NULL;
        return wire_alt(cx, IR_PAT_ALT, (const tree_t * const *) e->c, e->n, γ_in, ω_in, α_out, β_out);
    case TT_LEN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_LEN); if (!n) return NULL;
        if (e->c[0]->t == TT_VAR) { n->sval = e->c[0]->v.sval ? e->c[0]->v.sval : ""; n->dval = 1.0; }
        else { n->ival = e->c[0]->v.ival; n->dval = 0.0; }
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_POS: case TT_RPOS: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_POS); if (!n) return NULL;
        int is_rpos = (e->t == TT_RPOS);
        if (e->c[0]->t == TT_VAR) {
            n->sval = e->c[0]->v.sval ? e->c[0]->v.sval : "";
            n->dval = is_rpos ? 1.0 : 2.0;
        } else {
            n->ival = e->c[0]->v.ival;
            n->sval = is_rpos ? "r" : NULL;
            n->dval = 0.0;
        }
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_TAB: case TT_RTAB: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_TAB); if (!n) return NULL;
        int is_rtab = (e->t == TT_RTAB);
        if (e->c[0]->t == TT_VAR) {
            n->sval = e->c[0]->v.sval ? e->c[0]->v.sval : "";
            n->dval = is_rtab ? 1.0 : 2.0;
        } else {
            n->ival = e->c[0]->v.ival;
            n->sval = is_rtab ? "r" : NULL;
            n->dval = 0.0;
        }
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_FENCE: {
        n = nalloc(cx, IR_PAT_FENCE); if (!n) return NULL;
        if (e->n > 0 && e->c[0]) {
            IR_t * fα = NULL, * fβ = NULL;
            lcx_t bx = cx; bx.bounded = 1;
            set_succ_fail(n, γ_in, ω_in);
            IR_t * inner = lower2(cx, e->c[0], n, ω_in, &fα, &fβ);
            if (!inner) return NULL;
            return ret(n, α_out, β_out, fα, ω_in  );
        }
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_ABORT: {
        n = nalloc(cx, IR_PAT_ABORT); if (!n) return NULL;
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_FAIL: {
        n = nalloc(cx, IR_FAIL); if (!n) return NULL;
        lcx_t bx = cx; bx.bounded = 1;
        return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_SUCCEED: {
        n = nalloc(cx, IR_SUCCEED); if (!n) return NULL;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_ARBNO: {
        if (e->n < 1 || !e->c[0]) return NULL;
        int inner_cap = 64;
        IR_graph_t * inner_blk = IR_alloc(inner_cap, IR_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * isucc = IR_node_alloc(inner_blk, IR_SUCCEED);
        IR_t * ifail = IR_node_alloc(inner_blk, IR_FAIL);
        IR_t * iα = NULL, * iβ = NULL;
        lcx_t icx = { inner_blk, ROLE_PATTERN, 0, 0 };
        IR_t * inner_entry = lower2(icx, e->c[0], isucc, ifail, &iα, &iβ);
        if (!inner_entry) { IR_free(inner_blk); return NULL; }
        inner_blk->entry = iα;
        n = nalloc(cx, IR_PAT_ARBNO); if (!n) { IR_free(inner_blk); return NULL; }
        int stack_cap = 64;
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC((size_t)stack_cap * sizeof(int));
        az->cap = stack_cap;
        az->saved_delta = 0;
        n->counter = (int64_t)(intptr_t)az;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_CAPT_COND_ASGN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_ASSIGN_COND); if (!n) return NULL;
        n->sval = (e->n > 1 && e->c[1] && e->c[1]->v.sval) ? e->c[1]->v.sval : NULL;
        set_succ_fail(n, γ_in, ω_in);
        IR_t * iα = NULL, * iβ = NULL;
        IR_t * inner = lower2(cx, e->c[0], n, ω_in, &iα, &iβ);
        if (!inner) return NULL;
        n->α = iα;
        return ret(n, α_out, β_out, n, iβ ? iβ : ω_in);
    }
    case TT_CAPT_IMMED_ASGN: {
        if (e->n < 1 || !e->c[0]) return NULL;
        n = nalloc(cx, IR_PAT_ASSIGN_IMM); if (!n) return NULL;
        n->sval = (e->n > 1 && e->c[1] && e->c[1]->v.sval) ? e->c[1]->v.sval : NULL;
        set_succ_fail(n, γ_in, ω_in);
        IR_t * iα = NULL, * iβ = NULL;
        IR_t * inner = lower2(cx, e->c[0], n, ω_in, &iα, &iβ);
        if (!inner) return NULL;
        n->α = iα;
        return ret(n, α_out, β_out, n, iβ ? iβ : ω_in);
    }
    case TT_CAPT_CURSOR: {
        if (e->n < 1 || !e->c[0] || !e->c[0]->v.sval) return NULL;
        n = nalloc(cx, IR_PAT_ATP); if (!n) return NULL;
        n->sval = e->c[0]->v.sval;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_DEFER: {
        if (e->n < 1 || !e->c[0] || !e->c[0]->v.sval) return NULL;
        n = nalloc(cx, IR_PAT_DEFER); if (!n) return NULL;
        n->sval = e->c[0]->v.sval;
        n->ival = 1;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    case TT_VAR: {
        if (!e->v.sval) return NULL;
        const char * nm = e->v.sval;
        IR_e pk = (IR_e) 0; int is_prim = 1; int bnd = 0;
        if      (!strcmp(nm, "ARB"))     pk = IR_PAT_ARB;
        else if (!strcmp(nm, "REM"))     pk = IR_PAT_REM;
        else if (!strcmp(nm, "BAL"))     pk = IR_PAT_BAL;
        else if (!strcmp(nm, "ABORT"))   { pk = IR_PAT_ABORT; bnd = 1; }
        else if (!strcmp(nm, "FAIL"))    { pk = IR_FAIL;      bnd = 1; }
        else if (!strcmp(nm, "SUCCEED")) pk = IR_SUCCEED;
        else if (!strcmp(nm, "FENCE"))   { pk = IR_PAT_FENCE; bnd = 1; }
        else is_prim = 0;
        if (is_prim) {
            n = nalloc(cx, pk); if (!n) return NULL;
            lcx_t bx = cx; if (bnd) bx.bounded = 1;
            return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out);
        }
        n = nalloc(cx, IR_PAT_DEFER); if (!n) return NULL;
        n->sval = nm;
        n->ival = 0;
        return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out);
    }
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!arg_t) return NULL;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    call->sval = fn;
    call->ival = 1;
    call->dval = 1.0;
    lcx_t av = cx; av.role = ROLE_VALUE;
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * a = lower2(av, arg_t, call  , ω_in, &aα, &aβ);
    if (!a) return NULL;
    set_succ_fail(call, γ_in, ω_in);
    IR_t * call_resume = g_icn_postfix_resume ? aβ : ω_in;
    return ret(call, α_out, β_out, aα, call_resume);
}
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
static IR_t * lower_goal(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
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
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * lower2(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) { return ret(NULL, α_out, β_out, γ_in, ω_in); }
    switch (cx.role) {
    case ROLE_PATTERN: return lower_pattern(cx, e, γ_in, ω_in, α_out, β_out);
    case ROLE_GOAL:    return lower_goal(cx, e, γ_in, ω_in, α_out, β_out);
    case ROLE_VALUE:
    default:           return lower_value(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) ω_in;
    fprintf(stderr, "[lower2] UNHANDLED role=%d kind=%d\n", (int)cx.role, e ? (int)e->t : -1);
    return ret(NULL, α_out, β_out, NULL, NULL);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower2_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_VALUE, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_subject_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_VALUE, 0, bbg ? bbg->lang : 0, NULL, NULL };
    IR_t * subj = nalloc(cx, IR_SUBJECT);
    if (!subj) return NULL;
    IR_t * oα = NULL, * oβ = NULL;
    IR_t * op = lower2(cx, e, subj, ω_in, &oα, &oβ);
    if (!op) return NULL;
    (void) oβ;
    set_succ_fail(subj, γ_in, ω_in);
    return ret(subj, α_out, β_out, oα ? oα : subj, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_pat_build_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    if (!e || e->t != TT_QLIT) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const char * lit = e->v.sval ? e->v.sval : "";
    IR_t * sealed = nalloc(cx, IR_PAT_LIT);
    if (!sealed) return NULL;
    sealed->sval = lit;
    IR_t * ref = nalloc(cx, IR_REF_INVARIANT);
    if (!ref) return NULL;
    ref->sval = lit;
    IR_t * aux[1] = { sealed };
    bb_operand_aux_set(bbg, ref, aux, 1);
    lcx_t bx = cx; bx.bounded = 1;
    return emit_leaf(bx, ref, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_match_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    IR_t * m = nalloc(cx, IR_PAT_MATCH);
    if (!m) return NULL;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * el = lower2(cx, e, m, m, &eα, &eβ);
    if (!el) return NULL;
    (void) eβ;
    IR_t * entry = eα ? eα : el;
    IR_t * aux[1] = { entry };
    bb_operand_aux_set(bbg, m, aux, 1);
    set_succ_fail(m, γ_in, ω_in);
    return ret(m, α_out, β_out, m, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower2_pattern_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower2(cx, e, γ_in, ω_in, α_out, β_out);
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
