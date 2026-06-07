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
/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_internal.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* wire_det_builtin1 forward decl removed — now non-static, declared in lower_internal.h */
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * nalloc(lcx_t cx, IR_e kind) { return IR_node_alloc(cx.bbg, kind); }
/*--------------------------------------------------------------------------------------------------------------------*/
void set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in) {
    if (!n) return;
    if (!n->γ && γ_in) n->γ = γ_in;
    if (!n->ω && ω_in) n->ω = ω_in;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β) {
    if (α_out) *α_out = α;
    if (β_out) *β_out = β;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * name; IR_t * landing; } bb_label_entry_t;
static bb_label_entry_t g_bb_labels[1024];
static int              g_bb_label_n = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_label_registry_reset(void) { g_bb_label_n = 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_label_registry_add(const char * name, IR_t * landing) {
    if (!name || !landing || g_bb_label_n >= 1024) return;
    g_bb_labels[g_bb_label_n].name = name; g_bb_labels[g_bb_label_n].landing = landing; g_bb_label_n++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * bb_label_landing(const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < g_bb_label_n; i++)
        if (g_bb_labels[i].name && !strcmp(g_bb_labels[i].name, name)) return g_bb_labels[i].landing;
    if (!strcmp(name, "END")) return NULL;
    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * wire_seq(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (nkids < 1) return NULL;
    IR_t * node = nalloc(cx, kind);
    if (!node) return NULL;
    IR_t * entry[64]; IR_t * resume[64]; IR_t * apply[64];
    if (nkids > 64) return NULL;
    for (int i = nkids - 1; i >= 0; i--) {
        if (!kids[i]) return NULL;
        IR_t * γi = (i + 1 < nkids) ? entry[i + 1] : node;
        IR_t * αi = NULL, * βi = NULL;
        IR_t * c = lower_program(cx, kids[i], γi, ω_in  , &αi, &βi);
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
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, entry[0], resume[nkids - 1]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * wire_alt(lcx_t cx, IR_e kind, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
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
        IR_t * arm = lower_program(cx, kids[j], arm_succ, ωj, &αj, &βj);
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
int flatten_seq(const tree_t * e, tree_e kind, const tree_t ** out, int cap) {
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
int tm(const tree_t * e, tree_e kind, int nargs, ...) {
    if (!e || e->t != kind || e->n < nargs) return 0;
    va_list ap; va_start(ap, nargs);
    for (int i = 0; i < nargs; i++) { const tree_t ** slot = va_arg(ap, const tree_t **); if (slot) *slot = e->c[i]; }
    va_end(ap);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int tm_g(const tree_t * e, tree_e kind, const char * tag, int nargs, ...) {
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
           t == IR_PAT_LIT || t == IR_PAT_ARB || t == IR_PAT_REM || t == IR_PAT_SPAN || t == IR_PAT_SPAN_VAR || t == IR_PAT_ANY || t == IR_PAT_NOTANY ||
           t == IR_PAT_BREAK || t == IR_PAT_BREAKX || t == IR_PAT_LEN || t == IR_PAT_TAB || t == IR_PAT_RTAB || t == IR_PAT_ARBNO || t == IR_PAT_DEFER ||
           t == IR_PAT_BAL ||
           t == IR_PAT_ASSIGN_COND || t == IR_PAT_ASSIGN_IMM || t == IR_PAT_ATP;
}
IR_t * emit_leaf(lcx_t cx, IR_t * n, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
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
    case TT_ILIT:    n = nalloc(cx, IR_LIT_I);   if (n) IR_LIT(n).ival = e->v.ival; break;
    case TT_FLIT:    n = nalloc(cx, IR_LIT_F);   if (n) IR_LIT(n).dval = e->v.dval; break;
    case TT_QLIT:    n = nalloc(cx, IR_LIT_S);   if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; break;
    case TT_CSET:    n = nalloc(cx, IR_LIT_S);   if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; break;
    case TT_NUL: case TT_NULL: n = nalloc(cx, IR_LIT_NUL); break;
    case TT_VAR:     n = nalloc(cx, IR_VAR);     if (n) IR_LIT(n).sval = e->v.sval; break;
    case TT_NAME:    n = nalloc(cx, IR_VAR);     if (n) IR_LIT(n).sval = e->v.sval; break;
    case TT_KEYWORD: n = nalloc(cx, IR_KEYWORD); if (n) IR_LIT(n).sval = e->v.sval; break;
    default: return NULL;
    }
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
        IR_t * operand = lower_program(cx, e->c[0], NULL, ω_in, &oα, &oβ);
        if (!operand) return NULL;
        bang->α = oα ? oα : operand;
        set_succ_fail(bang, γ_in, ω_in);
        return ret(bang, α_out, β_out, bang, bang);
    }
    IR_t * un = nalloc(cx, IR_UNOP);
    if (!un) return NULL;
    IR_LIT(un).sval = e->v.sval;
    IR_LIT(un).ival = (int64_t) e->t;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * child = lower_program(cx, e->c[0], un  , ω_in, &eα, &eβ);
    if (!child) return NULL;
    set_succ_fail(un, γ_in, ω_in);
    return ret(un, α_out, β_out, eα, eβ);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
int tt_is_relational(tree_e t) {
    return t==TT_LT||t==TT_LE||t==TT_GT||t==TT_GE||t==TT_EQ||t==TT_NE
         ||t==TT_LLT||t==TT_LLE||t==TT_LGT||t==TT_LGE||t==TT_LEQ||t==TT_LNE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
int tt_to_binop(tree_e t) {
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
    IR_LIT(bin).sval = e->v.sval;
    IR_LIT(bin).ival = (int64_t) tt_to_binop(e->t);
    IR_LIT(bin).dval = tt_is_relational(e->t) ? 1.0 : 0.0;
    IR_t * e1α=NULL, * e1β=NULL, * e2α=NULL, * e2β=NULL;
    IR_t * c1 = lower_program(cx, e->c[0], NULL  , ω_in, &e1α, &e1β);
    if (!c1) return NULL;
    IR_t * c2 = lower_program(cx, e->c[1], bin  , e1β  , &e2α, &e2β);
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
    IR_LIT(node).sval = "ag";
    if (e->t == TT_TO_BY && e->n >= 3 && e->c[2]) {
        int64_t bits = 0; int isr = 0;
        if (to_by_const_step(e->c[2], &bits, &isr)) { IR_LIT(node).ival = bits; if (isr) IR_LIT(node).sval = "ar"; }
    }
    IR_t * fα=NULL,*fβ=NULL,*tα=NULL,*tβ=NULL;
    IR_t * lo = lower_program(cx, e->c[0], NULL  , ω_in, &fα, &fβ);
    if (!lo) return NULL;
    IR_t * hi = lower_program(cx, e->c[1], node  , fβ  , &tα, &tβ);
    if (!hi) return NULL;
    if (!lo->γ) lo->γ = tα;
    IR_t * bounds[2] = { lo, hi };
    bb_operand_aux_set(cx.bbg, node, bounds, 2);
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, fα, node  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * wire_if(lcx_t cx, const tree_t * e, int else_succeeds, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return NULL;
    IR_t * node = nalloc(cx, IR_IF);
    if (!node) return NULL;
    lcx_t cb = cx; cb.bounded = 1;
    IR_t * c1α=NULL,*c1β=NULL, *c2α=NULL,*c2β=NULL, *c3α=NULL,*c3β=NULL;
    IR_t * thenα = NULL, * elseα = NULL;
    IR_t * c2 = lower_program(cx, e->c[1], γ_in, ω_in, &c2α, &c2β);
    if (!c2) return NULL;
    thenα = c2α;
    if (e->n >= 3 && e->c[2]) {
        IR_t * c3 = lower_program(cx, e->c[2], γ_in, ω_in, &c3α, &c3β);
        if (!c3) return NULL;
        elseα = c3α;
    } else {
        elseα = else_succeeds ? γ_in : ω_in;
    }
    IR_t * c1 = lower_program(cb, e->c[0], thenα  , elseα  , &c1α, &c1β);
    if (!c1) return NULL;
    node->α = c1α;
    set_succ_fail(node, γ_in, ω_in);
    return ret(node, α_out, β_out, c1α, node  );
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * v_if(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    return wire_if(cx, e, 0, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
lcx_t bounded(lcx_t cx) { cx.bounded = 1; return cx; }
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t with_loop(lcx_t cx, IR_t * lω, IR_t * lnext) { cx.loop_ω = lω; cx.loop_next = lnext; return cx; }
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_graph_t * lower_value_subgraph(lcx_t cx, const tree_t * e) {
    IR_graph_t * blk = IR_alloc(256, cx.lang);
    if (!blk) return NULL;
    IR_t * vfail = IR_node_alloc(blk, IR_FAIL);
    lcx_t vcx = cx; vcx.bbg = blk;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * en = lower_program(vcx, e, NULL, vfail, &eα, &eβ);
    if (!en) { IR_free(blk); return NULL; }
    (void) eβ;
    blk->entry = eα ? eα : en;
    return blk;
}
static IR_t * v_conj(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * kids[64];
    int nk = flatten_seq(e, e->t, kids, 64);
    if (nk < 1) return NULL;
    return wire_seq(cx, IR_CONJ, kids, nk, γ_in, ω_in, α_out, β_out);
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
    IR_t * gen = lower_program(cx, e->c[0], NULL  , ev  , &g1α, &g1β);
    if (!gen) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower_program(bounded(cx), e->c[1], g1β  , g1β  , &b2α, &b2β);
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
IR_t * v_raku_det_call(lcx_t cx, const char * fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!fn || nkids < 0) return NULL;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = GC_strdup(fn);
    IR_LIT(call).ival = nkids;
    IR_LIT(call).dval = 2.0;
    IR_graph_t ** blks = (IR_graph_t **) calloc((size_t)(nkids > 0 ? nkids : 1), sizeof(IR_graph_t *));
    if (!blks) return NULL;
    lcx_t mv = cx; mv.role = ROLE_VALUE;
    for (int i = 0; i < nkids; i++) {
        if (!kids[i]) { free(blks); return NULL; }
        blks[i] = lower_value_subgraph(mv, kids[i]);
        if (!blks[i]) { free(blks); return NULL; }
    }
    IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    set_succ_fail(call, γ_in, ω_in);
    return ret(call, α_out, β_out, call  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_raku_mutate_writeback(lcx_t cx, const char * target, const char * pure_fn, const tree_t * const * kids, int nkids, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!target || !pure_fn || nkids < 1) return NULL;
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = GC_strdup(target);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * call = v_raku_det_call(cx, pure_fn, kids, nkids, as  , ω_in, &cα, &cβ);
    if (!call) return NULL;
    (void) cβ;
    set_succ_fail(as, γ_in, ω_in);
    return ret(as, α_out, β_out, cα  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * v_while(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return NULL;
    IR_t * wh = nalloc(cx, IR_WHILE);
    if (!wh) return NULL;
    IR_t * c1α=NULL,*c1β=NULL;
    IR_t * cond = lower_program(bounded(cx), e->c[0], NULL  , wh  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower_program(with_loop(bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
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
    IR_t * cond = lower_program(bounded(cx), e->c[0], un  , NULL  , &c1α, &c1β);
    if (!cond) return NULL;
    if (e->n >= 2 && e->c[1]) {
        IR_t * b2α=NULL,*b2β=NULL;
        IR_t * body = lower_program(with_loop(bounded(cx), γ_in, c1α), e->c[1], c1α  , c1α  , &b2α, &b2β);
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
    IR_t * body = lower_program(with_loop(bounded(cx), γ_in, rp), e->c[0], rp  , rp  , &eα, &eβ);
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
IR_t * v_det_call(lcx_t cx, const tree_t * e, int allow_generator, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * call = nalloc(cx, IR_CALL); if (!call) return NULL;
    IR_LIT(call).sval = e->c[0]->v.sval;
    int nargs = e->n - 1;
    IR_LIT(call).ival = nargs;
    IR_LIT(call).dval = 3.0;
    if (nargs > 0) {
        IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
        if (!blks) return NULL;
        lcx_t ac = cx; ac.role = ROLE_VALUE;
        for (int i = 0; i < nargs; i++) {
            blks[i] = lower_value_subgraph(ac, e->c[i + 1]);
            if (!blks[i]) { free(blks); return NULL; }
        }
        IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    }
    set_succ_fail(call, γ_in, ω_in);
    IR_t * call_beta = (allow_generator) ? call : ω_in;
    return ret(call, α_out, β_out, call, call_beta);
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
    IR_t * ce = lower_program(bounded(cx), e->c[0], ω_in  , nt  , &eα, &eβ);
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
    int lhs_is_var = (lhs_t->t == TT_VAR);
    if (!lhs_is_var) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = lhs_t->v.sval ? lhs_t->v.sval : "";
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower_program(cx, rhs_t, as  , ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    (void) rβ;
    set_succ_fail(as, γ_in, ω_in);
    return ret(as, α_out, β_out, rα, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_value_shared(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    switch (e->t) {
    case TT_ILIT: case TT_FLIT: case TT_QLIT: case TT_CSET:
    case TT_NUL:  case TT_NULL: case TT_VAR:  case TT_NAME: case TT_KEYWORD:
        return v_literal(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUCCEED: { IR_t * n = nalloc(cx, IR_SUCCEED); if (!n) return NULL; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FAIL:    { IR_t * n = nalloc(cx, IR_FAIL);    if (!n) return NULL; lcx_t bx = cx; bx.bounded = 1; return emit_leaf(bx, n, γ_in, ω_in, α_out, β_out); }
    case TT_MATCH_UNARY:
        return v_unop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL:
    case TT_RANDOM: case TT_CSET_COMPL: case TT_ITERATE: case TT_INTERROGATE:
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
        return v_every(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_WHILE:
        return v_while(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_UNTIL:
        return v_until(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_REPEAT:
        return v_repeat(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_NOT:
        return v_not(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_BREAK:
        return v_loop_break(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_NEXT:
        return v_loop_next(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ASSIGN:
        return v_assign(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_RETURN:
    case TT_NRETURN:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUSPEND:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_INITIAL:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FNC: case TT_PROC_FAIL: case TT_SWAP: case TT_AUGOP: case TT_REVASSIGN: case TT_REVSWAP: case TT_LIMIT: case TT_CASE: {
        if (e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return wire_det_builtin1(cx, e->c[1], fn, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
    case TT_SCAN:
        return v_scan(cx, e, γ_in, ω_in, α_out, β_out);
    default:
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * wire_det_builtin1(lcx_t cx, const tree_t * arg_t, const char * fn, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!arg_t) return NULL;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = fn;
    IR_LIT(call).ival = 1;
    IR_LIT(call).dval = 1.0;
    lcx_t av = cx; av.role = ROLE_VALUE;
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * a = lower_program(av, arg_t, call  , ω_in, &aα, &aβ);
    if (!a) return NULL;
    set_succ_fail(call, γ_in, ω_in);
    IR_t * call_resume = g_icn_postfix_resume ? aβ : ω_in;
    return ret(call, α_out, β_out, aα, call_resume);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β) {
    if (!e) return iref(NULL, α, β, γ, ω);
    if (cx.lang == IR_LANG_ICN) { IR_t * aα = NULL, * aβ = NULL; IR_t * r = lower_icn(cx, e, γ, ω, &aα, &aβ); return iref(r, α, β, aα, aβ); }
    if (cx.lang == IR_LANG_SNO || cx.lang == IR_LANG_SCO || cx.lang == IR_LANG_REB) return lower_sno(cx, e, γ, ω, α, β);
    if (cx.lang == IR_LANG_RKU) return lower_rku(cx, e, γ, ω, α, β);
    if (cx.lang == IR_LANG_PAS) return lower_pas(cx, e, γ, ω, α, β);
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * r;
    switch (cx.role) {
    case ROLE_PATTERN: r = lower_pattern(cx, e, γ, ω, &aα, &aβ); break;
    case ROLE_GOAL:    r = lower_goal(cx, e, γ, ω, &aα, &aβ); break;
    case ROLE_VALUE:
    default:           r = lower_value_shared(cx, e, γ, ω, &aα, &aβ); break;
    }
    return iref(r, α, β, aα, aβ);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_program(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_ref_t rα = {0}, rβ = {0};
    IR_t * r = lower(cx, e, γ_in, ω_in, &rα, &rβ);
    if (α_out) *α_out = rα.node;
    if (β_out) *β_out = rβ.node;
    return r;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) ω_in;
    fprintf(stderr, "[lower] UNHANDLED role=%d kind=%d\n", (int)cx.role, e ? (int)e->t : -1);
    return ret(NULL, α_out, β_out, NULL, NULL);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_VALUE, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower_program(cx, e, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_subject_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_VALUE, 0, bbg ? bbg->lang : 0, NULL, NULL };
    IR_t * subj = nalloc(cx, IR_SUBJECT);
    if (!subj) return NULL;
    IR_t * oα = NULL, * oβ = NULL;
    IR_t * op = lower_program(cx, e, subj, ω_in, &oα, &oβ);
    if (!op) return NULL;
    (void) oβ;
    set_succ_fail(subj, γ_in, ω_in);
    return ret(subj, α_out, β_out, oα ? oα : subj, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_pat_build_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    if (!e || e->t != TT_QLIT) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const char * lit = e->v.sval ? e->v.sval : "";
    IR_t * sealed = nalloc(cx, IR_PAT_LIT);
    if (!sealed) return NULL;
    IR_LIT(sealed).sval = lit;
    IR_t * ref = nalloc(cx, IR_REF_INVARIANT);
    if (!ref) return NULL;
    IR_LIT(ref).sval = lit;
    IR_t * aux[1] = { sealed };
    bb_operand_aux_set(bbg, ref, aux, 1);
    lcx_t bx = cx; bx.bounded = 1;
    return emit_leaf(bx, ref, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_match_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    IR_t * m = nalloc(cx, IR_PAT_MATCH);
    if (!m) return NULL;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * el = lower_program(cx, e, m, m, &eα, &eβ);
    if (!el) return NULL;
    (void) eβ;
    IR_t * entry = eα ? eα : el;
    IR_t * aux[1] = { entry };
    bb_operand_aux_set(bbg, m, aux, 1);
    set_succ_fail(m, γ_in, ω_in);
    return ret(m, α_out, β_out, m, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_pattern_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower_program(cx, e, γ_in, ω_in, α_out, β_out);
}