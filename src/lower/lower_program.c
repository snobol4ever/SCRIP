/*====================================================================================================================*/
/*====================================================================================================================*/
#define BB_DEFINE_NAMES
#include "lower.h"
#include "lower_internal.h"
#include "IR_interp_state.h"
#include "bb_program.h"
#include "../runtime/core/coerce.h"
#include "../runtime/builtins/resolution.h"
#include "../parser/prolog/term.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <gc/gc.h>
extern int junction_is(DESCR_t v);
extern int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint32_t polyglot_lang_mask(const tree_t * prog);
extern void polyglot_init(stage2_t * s2, const tree_t * prog, uint32_t lang_mask);
extern IR_t * lower_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
static const tree_t * g_nl_prog = NULL;
static int nl_on(int dflt) { const char * e = getenv("SCRIP_NL"); return e ? (atoi(e) != 0) : dflt; }
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * make_computed_goto(IR_graph_t * g, const tree_t * gexpr, IR_t * fall) {
    if (!g || !gexpr) return NULL;
    IR_graph_t * sub = IR_alloc(64, IR_LANG_SNO);
    if (!sub) return NULL;
    IR_t * vfail = IR_node_alloc(sub, IR_FAIL);
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * en = lower_value_entry(sub, gexpr, NULL  , vfail, &eα, &eβ);
    if (!en) { IR_free(sub); return NULL; }
    (void) eβ;
    sub->entry = eα ? eα : en;
    IR_t * gt = IR_node_alloc(g, IR_GOTO);
    if (!gt) { IR_free(sub); return NULL; }
    IR_EXEC(gt).counter = (int64_t)(intptr_t) sub;
    gt->ω.node = fall; memcpy(gt->ω.sz, "α", 3);
    return gt;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * make_indirect_goto(IR_graph_t * g, const char * gstr, IR_t * fall) {
    if (!gstr || gstr[0] != '$' || gstr[1] == '\0') return NULL;
    tree_t * v = ast_stmt_new(TT_VAR);
    if (!v) return NULL;
    v->v.sval = (char *) (gstr + 1);
    return make_computed_goto(g, v, fall);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail) {
    *rel_fail = 0;
    if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
    {
        int lj = junction_is(lv), rj = junction_is(rv);
        int num_rel = (op == BINOP_EQ || op == BINOP_NE || op == BINOP_LT ||
                       op == BINOP_LE || op == BINOP_GT || op == BINOP_GE);
        int str_rel = (op == BINOP_SEQ || op == BINOP_SNE || op == BINOP_SLT ||
                       op == BINOP_SLE || op == BINOP_SGT || op == BINOP_SGE);
        if ((lj || rj) && (num_rel || str_rel)) {
            DESCR_t jct    = lj ? lv : rv;
            DESCR_t scalar = lj ? rv : lv;
            int tt_op = (op == BINOP_EQ || op == BINOP_SEQ) ? TT_EQ : (op == BINOP_NE || op == BINOP_SNE) ? TT_NE :
                        (op == BINOP_LT || op == BINOP_SLT) ? TT_LT : (op == BINOP_LE || op == BINOP_SLE) ? TT_LE :
                        (op == BINOP_GT || op == BINOP_SGT) ? TT_GT : TT_GE;
            int numeric = str_rel ? 0 : (IS_INT_fn(scalar) || IS_REAL_fn(scalar));
            int truth = junction_collapse(scalar, jct, tt_op, numeric);
            *rel_fail = !truth;
            return truth ? rv : FAILDESCR;
        }
    }
    if ((op == BINOP_EQ || op == BINOP_NE || op == BINOP_LT || op == BINOP_LE || op == BINOP_GT || op == BINOP_GE) && IS_STR_fn(lv) && IS_STR_fn(rv) && !IS_CSET_fn(lv) && !IS_CSET_fn(rv)) {
        const char *ls = (lv.v == DT_S && lv.s) ? lv.s : "";
        const char *rs = (rv.v == DT_S && rv.s) ? rv.s : "";
        size_t ll = (lv.v == DT_S && lv.slen > 0) ? (size_t)lv.slen : strlen(ls);
        size_t rl = (rv.v == DT_S && rv.slen > 0) ? (size_t)rv.slen : strlen(rs);
        size_t nn = ll < rl ? ll : rl;
        int cmp = nn ? memcmp(ls, rs, nn) : 0;
        if (!cmp) cmp = (ll > rl) - (ll < rl);
        int ok;
        switch (op) {
        case BINOP_LT: ok = (cmp <  0); break;
        case BINOP_LE: ok = (cmp <= 0); break;
        case BINOP_GT: ok = (cmp >  0); break;
        case BINOP_GE: ok = (cmp >= 0); break;
        case BINOP_EQ: ok = (cmp == 0); break;
        default:       ok = (cmp != 0); break;
        }
        *rel_fail = !ok;
        return ok ? rv : FAILDESCR;
    }
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
        case BINOP_POW: { double base = either_real ? ld : (double)li; double exp2 = either_real ? rd : (double)ri;
                          real_result.v = DT_R; real_result.r = pow(base, exp2); return real_result; }
        case BINOP_LT: *rel_fail = !(either_real ? ld <  rd : li <  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_LE: *rel_fail = !(either_real ? ld <= rd : li <= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GT: *rel_fail = !(either_real ? ld >  rd : li >  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GE: *rel_fail = !(either_real ? ld >= rd : li >= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_EQ: *rel_fail = !(either_real ? ld == rd : li == ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_NE: *rel_fail = !(either_real ? ld != rd : li != ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_CONCAT: {
            DESCR_t ls_d; ls_d = descr_to_str(lv);
            DESCR_t rs_d; rs_d = descr_to_str(rv);
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
            DESCR_t ls_d = descr_to_str(lv);
            DESCR_t rs_d = descr_to_str(rv);
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
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_proc_gen(struct GeneratorState *gs) {
    if (!gs) return NULL;
    IR_graph_t *bbg = IR_alloc(4, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_PROC_GEN);
    if (!bb) return NULL;
    IR_EXEC(bb).counter = (int64_t)(uintptr_t)gs;
    bb->γ.node = NULL;
    bb->ω.node = NULL;
    bbg->entry = bb;
    return bbg;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static int lp_s_int(const tree_t *s, const char *tag) { const char *v = stmt_attr_str(stmt_attr_find(s, tag)); return v ? atoi(v) : 0; }
static tree_t *lp_s_expr(const tree_t *s, const char *tag) { return stmt_attr_expr(stmt_attr_find(s, tag)); }
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
extern IR_t * lower_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * g, IR_t * w, IR_t ** a, IR_t ** b);
/*--------------------------------------------------------------------------------------------------------------------*/
static int proc_subtree_has_suspend(const tree_t *n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_PROC_DECL || n->t == TT_SUB_DECL) return 0;
    for (int i = 0; i < n->n; i++) if (proc_subtree_has_suspend(n->c[i])) return 1;
    return 0;
}
static int proc_body_has_suspend(const tree_t *proc) {
    if (!proc) return 0;
    for (int i = 0; i < proc->n; i++) if (proc_subtree_has_suspend(proc->c[i])) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int lower_icon_body(const tree_t *proc) {
    extern IR_graph_t * lower_icon_proc(const tree_t *, const tree_t *);
    IR_graph_t * ng = lower_icon_proc(g_nl_prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static void pas_register_labels(IR_graph_t *g, const tree_t *n) {
    if (!n) return;
    if (n->t == TT_PROC_DECL || n->t == TT_SUB_DECL) return;
    if (n->t == TT_LABEL_DEF && n->v.sval) {
        IR_t *landing = IR_node_alloc(g, IR_SUCCEED);
        if (landing) bb_label_registry_add(n->v.sval, landing);
    }
    for (int i = 0; i < n->n; i++) pas_register_labels(g, n->c[i]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int lower_pascal_body(const tree_t *proc) {
    if (nl_on(1)) {
        extern IR_graph_t * lower_pascal_proc(const tree_t *, const tree_t *);
        IR_graph_t * ng = lower_pascal_proc(g_nl_prog, proc);
        if (!ng || !ng->entry) return -1;
        return bb_program_add(&g_stage2.bbp, ng);
    }
    if (!proc || proc->t != TT_PROC_DECL || proc->n < 3) return -1;
    const tree_t *body = proc->c[2];
    if (!body || body->t != TT_PROGRAM) return -1;
    int is_function = (proc->n >= 4 && proc->c[3] && proc->c[3]->t == TT_VAR && proc->c[3]->v.sval);
    IR_graph_t *g = IR_alloc(256, IR_LANG_PAS);
    if (!g) return -1;
    bb_label_registry_reset();
    for (int i = 0; i < body->n; i++) pas_register_labels(g, body->c[i]);
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *chain_end = PSUCC;
    if (is_function) {
        IR_t *PRET = IR_node_alloc(g, IR_RETURN);
        IR_t *PVAR = IR_node_alloc(g, IR_VAR);
        if (!PRET || !PVAR) return -1;
        IR_LIT(PRET).dval = 0.0;
        IR_LIT(PVAR).sval = proc->c[3]->v.sval;
        if (!ir_operand_push(PRET, PVAR)) return -1;
        PRET->γ.node = PSUCC; memcpy(PRET->γ.sz, "α", 3); PRET->ω.node = PSUCC; memcpy(PRET->ω.sz, "α", 3);
        chain_end = PRET;
    }
    IR_t *next_a = chain_end;
    int n_stmts = 0;
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t *s = body->c[i];
        if (!s) continue;
        const tree_t *expr = s;
        if (s->t == TT_STMT) { expr = lp_s_expr(s, ":subj"); if (!expr) continue; }
        n_stmts++;
        IR_t *a = NULL, *b = NULL;
        IR_t *top = lower_value_entry(g, (const tree_t *) expr, next_a, PFAIL, &a, &b);
        if (!top || !a) return -1;
        next_a = a;
    }
    if (n_stmts == 0) { if (is_function) { g->entry = chain_end; return bb_program_add(&g_stage2.bbp, g); } return -1; }
    g->entry = next_a;
    return bb_program_add(&g_stage2.bbp, g);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int lower_raku_body(const tree_t *proc) {
    if (!proc || proc->t != TT_SUB_DECL) return -1;
    int np = (int) proc->v.ival;
    int body_start = 1 + np;
    if (proc->n <= body_start) return -1;
    IR_graph_t *g = IR_alloc(256, IR_LANG_RKU);
    if (!g) return -1;
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *next_a = PSUCC;
    int n_stmts = 0;
    for (int i = proc->n - 1; i >= body_start; i--) {
        const tree_t *s = proc->c[i];
        if (!s) continue;
        const tree_t *expr = s;
        if (s->t == TT_STMT) { expr = lp_s_expr(s, ":subj"); if (!expr) continue; }
        n_stmts++;
        IR_t *a = NULL, *b = NULL;
        IR_t *top = lower_value_entry(g, (const tree_t *) expr, next_a, PFAIL, &a, &b);
        if (!top || !a) return -1;
        next_a = a;
    }
    if (n_stmts == 0) return -1;
    g->entry = next_a;
    return bb_program_add(&g_stage2.bbp, g);
}
/*====================================================================================================================*/
static int lower_pl_clause_graph(const tree_t *clause);
static int lower_pl_choice_graph(const tree_t *choice) {
    if (!choice || choice->t != TT_CHOICE || choice->n < 1) return -1;
    int n = choice->n;
    IR_graph_t **bodies = (IR_graph_t **)GC_MALLOC((size_t)n * sizeof(IR_graph_t *));
    if (!bodies) return -1;
    int any = 0;
    for (int ci = 0; ci < n; ci++) {
        const tree_t *cl = choice->c[ci];
        int bidx = lower_pl_clause_graph(cl);
        bodies[ci] = (bidx >= 0) ? g_stage2.bbp.table[bidx] : NULL;
        if (bodies[ci]) any = 1;
    }
    if (!any) return -1;
    IR_graph_t *g = IR_alloc(8, IR_LANG_PL);
    if (!g) return -1;
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *nd = IR_node_alloc(g, IR_CHOICE);
    if (!nd) return -1;
    bb_choice_state_t *zc = (bb_choice_state_t *)GC_MALLOC(sizeof *zc);
    if (!zc) return -1;
    memset(zc, 0, sizeof *zc);
    zc->bodies = bodies; zc->nbodies = n; zc->last_body = NULL; zc->cp = NULL; zc->cut_barrier = NULL;
    zc->idx_ok = 0; zc->idx_key = NULL;
    IR_LIT(nd).ival = (int64_t)(intptr_t)zc;
    nd->γ.node = PSUCC; memcpy(nd->γ.sz, "α", 3); nd->ω.node = PFAIL; memcpy(nd->ω.sz, "α", 3);
    g->entry = nd;
    return bb_program_add(&g_stage2.bbp, g);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void lower_pl_register_all_preds(void) {
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) {
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            if (!pe->key || !pe->choice) continue;
            const char *key = pe->key;
            const tree_t *ch = pe->choice;
            const char *slash = key ? strrchr(key, '/') : NULL;
            int ar = slash ? atoi(slash + 1) : 0;
            if (resolve_bb_lookup(key, ar)) continue;
            int bb_idx = -1;
            if (ch->t == TT_CLAUSE) {
                bb_idx = lower_pl_clause_graph(ch);
            } else if (ch->t == TT_CHOICE) {
                if (ch->n == 1) bb_idx = lower_pl_clause_graph(ch->c[0]);
                else            bb_idx = lower_pl_choice_graph(ch);
            }
            if (bb_idx >= 0) resolve_bb_register(key, ar, bb_idx);
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern IR_t * lower_clause_body_entry(IR_graph_t * bbg, const tree_t * clause, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out);
extern tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
static int lower_pl_clause_graph(const tree_t *clause) {
    if (!clause || clause->t != TT_CLAUSE) return -1;
    IR_graph_t *g = IR_alloc(256, IR_LANG_PL);
    if (!g) return -1;
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *α = NULL, *β = NULL;
    IR_t *top = lower_clause_body_entry(g, clause, PSUCC, PFAIL, &α, &β);
    if (!top || !α) return -1;
    g->entry = α;
    return bb_program_add(&g_stage2.bbp, g);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern tree_t *pl_assert_term(Term *t, int *functor_out, int *arity_out);
int pl_rt_assertz(Term *clause_term, int prepend) {
    int fid = -1, arity = 0;
    tree_t *clause = pl_assert_term(clause_term, &fid, &arity);
    if (!clause || clause->t != TT_CLAUSE) return 0;
    const char *fname = prolog_atom_name(fid);
    if (!fname) return 0;
    char key[256]; snprintf(key, sizeof key, "%s/%d", fname, arity);
    int body_idx = lower_pl_clause_graph(clause);
    if (body_idx < 0) return 0;
    IR_graph_t *body = g_stage2.bbp.table[body_idx];
    if (!body) return 0;
    Resolve_PredEntry_BB *entry = resolve_bb_lookup(key, arity);
    IR_graph_t *pred_cfg = entry ? bb_graph_of_pred(entry) : NULL;
    if (!pred_cfg || !pred_cfg->entry || pred_cfg->entry->op != IR_CHOICE) {
        IR_graph_t *prior = (pred_cfg && pred_cfg->entry) ? pred_cfg : NULL;
        IR_graph_t *cg = IR_alloc(8, IR_LANG_PL);
        if (!cg) return 0;
        IR_t *PSUCC = IR_node_alloc(cg, IR_SUCCEED);
        IR_t *PFAIL = IR_node_alloc(cg, IR_FAIL);
        IR_t *nd = IR_node_alloc(cg, IR_CHOICE);
        if (!nd) return 0;
        bb_choice_state_t *zc0 = (bb_choice_state_t *)GC_MALLOC(sizeof *zc0);
        if (!zc0) return 0;
        memset(zc0, 0, sizeof *zc0);
        if (prior) {
            IR_graph_t **pb = (IR_graph_t **)GC_MALLOC(sizeof(IR_graph_t *));
            if (!pb) return 0;
            pb[0] = prior;
            zc0->bodies = pb; zc0->nbodies = 1;
        } else {
            zc0->bodies = NULL; zc0->nbodies = 0;
        }
        zc0->idx_ok = 0; zc0->idx_key = NULL;
        IR_LIT(nd).ival = (int64_t)(intptr_t)zc0;
        nd->γ.node = PSUCC; memcpy(nd->γ.sz, "α", 3); nd->ω.node = PFAIL; memcpy(nd->ω.sz, "α", 3);
        (void)PSUCC; (void)PFAIL;
        cg->entry = nd;
        int cg_idx = bb_program_add(&g_stage2.bbp, cg);
        if (cg_idx < 0) return 0;
        resolve_bb_register(key, arity, cg_idx);
        pred_cfg = g_stage2.bbp.table[cg_idx];
    }
    bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(pred_cfg->entry).ival;
    if (!zc) return 0;
    int n = zc->nbodies;
    IR_graph_t **nb = (IR_graph_t **)GC_MALLOC((size_t)(n + 1) * sizeof(IR_graph_t *));
    if (!nb) return 0;
    if (prepend) {
        nb[0] = body;
        for (int i = 0; i < n; i++) nb[i + 1] = zc->bodies[i];
    } else {
        for (int i = 0; i < n; i++) nb[i] = zc->bodies[i];
        nb[n] = body;
    }
    zc->bodies = nb;
    zc->nbodies = n + 1;
    zc->idx_ok = 0; zc->idx_key = NULL;
    return 1;
}
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *lp_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *c = (char *) GC_MALLOC(n);
    if (c) memcpy(c, s, n);
    return c;
}
static int sno_parse_define_proto(const char *proto, char fname[64],
                                  char params[STAGE2_FRAME_SLOT_MAX][64], int *np,
                                  char locals[STAGE2_FRAME_SLOT_MAX][64], int *nl) {
    *np = 0; *nl = 0; if (fname) fname[0] = '\0';
    if (!proto || !fname) return 0;
    const char *p = proto;
    while (*p && isspace((unsigned char) *p)) p++;
    int fi = 0;
    while (*p && *p != '(' && !isspace((unsigned char) *p) && fi < 63) fname[fi++] = *p++;
    fname[fi] = '\0';
    if (fi == 0) return 0;
    while (*p && isspace((unsigned char) *p)) p++;
    if (*p == '(') {
        p++;
        while (*p && *p != ')') {
            while (*p && (isspace((unsigned char) *p) || *p == ',')) p++;
            if (!*p || *p == ')') break;
            int ci = 0; char buf[64];
            while (*p && *p != ',' && *p != ')' && !isspace((unsigned char) *p) && ci < 63) buf[ci++] = *p++;
            buf[ci] = '\0';
            if (ci > 0 && *np < STAGE2_FRAME_SLOT_MAX) { memcpy(params[*np], buf, (size_t) ci + 1); (*np)++; }
            while (*p && isspace((unsigned char) *p)) p++;
        }
        if (*p == ')') p++;
    }
    while (*p) {
        while (*p && (isspace((unsigned char) *p) || *p == ',')) p++;
        if (!*p) break;
        int ci = 0; char buf[64];
        while (*p && *p != ',' && !isspace((unsigned char) *p) && ci < 63) buf[ci++] = *p++;
        buf[ci] = '\0';
        if (ci > 0 && *nl < STAGE2_FRAME_SLOT_MAX) { memcpy(locals[*nl], buf, (size_t) ci + 1); (*nl)++; }
    }
    return 1;
}
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int pas_scope_chain(int pi, Scope **scs, int *dls, int *pis, int maxd) {
    int n = 0;
    int dl = g_stage2.proc_table[pi].decl_level;
    scs[n] = &g_stage2.proc_table[pi].lower_sc; dls[n] = dl; pis[n] = pi; n++;
    int want = dl - 1, at = pi;
    while (want >= 1 && n < maxd) {
        int found = -1;
        for (int j = at + 1; j < g_stage2.proc_count; j++) {
            const tree_t *pj = (const tree_t *) g_stage2.proc_table[j].proc;
            if (!pj || pj->t != TT_PROC_DECL || !g_stage2.proc_table[j].name || !strcmp(g_stage2.proc_table[j].name, "main")) continue;
            if (g_stage2.proc_table[j].decl_level == want) { found = j; break; }
        }
        if (found < 0) break;
        scs[n] = &g_stage2.proc_table[found].lower_sc; dls[n] = want; pis[n] = found; n++;
        at = found; want--;
    }
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void pas_rewrite_graph(IR_graph_t *g, Scope **scs, int *dls, int *pis, int nch);
static void pas_rewrite_node(IR_t *nd, Scope **scs, int *dls, int *pis, int nch) {
    if (!nd) return;
    if ((nd->op == IR_VAR || nd->op == IR_ASSIGN) && IR_LIT(nd).sval) {
        for (int c = 0; c < nch; c++) {
            int slot = scope_get(scs[c], IR_LIT(nd).sval);
            if (slot >= 0) {
                ProcEntry *pe = &g_stage2.proc_table[pis[c]];
                int isref = (slot < pe->nparams) && ((pe->byref_mask >> slot) & 1ull);
                IR_LIT(nd).ival = slot;
                IR_LIT(nd).dval = (double)(dls[0] - dls[c]);
                if (isref) nd->op = (nd->op == IR_VAR) ? IR_VAR_FRAME_REF : IR_ASSIGN_FRAME_REF;
                else       nd->op = (nd->op == IR_VAR) ? IR_VAR_FRAME : IR_ASSIGN_FRAME;
                break;
            }
        }
        return;
    }
    if (nd->op == IR_CALL && (IR_LIT(nd).dval == 2.0 || IR_LIT(nd).dval == 3.0) && IR_EXEC(nd).counter && IR_LIT(nd).ival > 0) {
        IR_graph_t **subs = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
        for (int j = 0; j < (int) IR_LIT(nd).ival; j++) if (subs[j]) pas_rewrite_graph(subs[j], scs, dls, pis, nch);
    }
}
static void pas_rewrite_graph(IR_graph_t *g, Scope **scs, int *dls, int *pis, int nch) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) pas_rewrite_node(g->all[i], scs, dls, pis, nch);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int lower_sno_nl(const tree_t *prog) {
    extern IR_graph_t * lower_snobol4(const tree_t *);
    extern int lower_snobol4_labels(const char ***, IR_t ***);
    extern IR_t * lower_snobol4_label(const char *);
    int sno_n = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        if (lp_s_int(s, ":lang") != LANG_SNO) return 0;
        sno_n++;
    }
    if (sno_n == 0) return 0;
    IR_graph_t *g = lower_snobol4(prog);
    if (!g || !g->entry) return 0;
    const char ** lnames = NULL; IR_t ** lnodes = NULL;
    int nlab = lower_snobol4_labels(&lnames, &lnodes);
    bb_label_registry_reset();
    for (int i = 0; i < nlab; i++) bb_label_registry_add(lnames[i], lnodes[i]);
    int bb_idx = bb_program_add(&g_stage2.bbp, g);
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name     = "main";
    g_stage2.proc_table[pi].proc     = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx   = bb_idx;
    g_stage2.proc_table[pi].nparams  = 0;
    for (int di = 0; di < prog->n; di++) {
        const tree_t *ds = prog->c[di];
        if (!ds || ds->t != TT_STMT) continue;
        tree_t *dsubj = lp_s_expr(ds, ":subj");
        if (!dsubj || dsubj->t != TT_FNC || !dsubj->v.sval || strcmp(dsubj->v.sval, "DEFINE")) continue;
        if (dsubj->n < 1 || !dsubj->c[0] || dsubj->c[0]->t != TT_QLIT || !dsubj->c[0]->v.sval) continue;
        char fname[64];
        char params[STAGE2_FRAME_SLOT_MAX][64]; int np = 0;
        char locals[STAGE2_FRAME_SLOT_MAX][64]; int nl = 0;
        if (!sno_parse_define_proto(dsubj->c[0]->v.sval, fname, params, &np, locals, &nl)) continue;
        IR_t *body = lower_snobol4_label(fname);
        if (!body) continue;
        IR_graph_t *fg = (IR_graph_t *) calloc(1, sizeof(IR_graph_t));
        if (!fg) continue;
        *fg = *g;
        fg->entry = body;
        int fidx = bb_program_add(&g_stage2.bbp, fg);
        int fpi  = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name     = strdup(fname);
        g_stage2.proc_table[fpi].proc     = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].bb_idx   = fidx;
        g_stage2.proc_table[fpi].nparams  = np;
        Scope *sc = &g_stage2.proc_table[fpi].lower_sc;
        sc->n = 0;
        for (int k = 0; k < np && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = strdup(params[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
        for (int k = 0; k < nl && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = strdup(locals[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
        if (sc->n < STAGE2_FRAME_SLOT_MAX) { sc->e[sc->n].name = strdup(fname); sc->e[sc->n].slot = sc->n; sc->n++; }
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_stage2(const tree_t *prog) {
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    g_nl_prog = prog;
    stage2_reset();
    uint32_t mask = polyglot_lang_mask(prog);
    polyglot_init(&g_stage2, prog, mask);
    if ((mask & (1u << LANG_SNO)) && !(nl_on(1) && lower_sno_nl(prog))) {
        IR_graph_t *g = IR_alloc(1024, IR_LANG_SNO);
        if (g) {
            IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
            IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
            IR_t *RET  = IR_node_alloc(g, IR_RETURN); IR_LIT(RET).dval  = 1.0; RET->ω.node = PFAIL; memcpy(RET->ω.sz, "α", 3);
            IR_t *FRET = IR_node_alloc(g, IR_RETURN); IR_LIT(FRET).dval = 2.0; FRET->ω.node = PFAIL; memcpy(FRET->ω.sz, "α", 3);
            const tree_t *stmts[1024]; IR_t *land[1024]; int ns = 0;
            for (int i = 0; i < prog->n && ns < 1024; i++) {
                const tree_t *s = prog->c[i];
                if (!s || s->t != TT_STMT) continue;
                if (lp_s_int(s, ":lang") != LANG_SNO) continue;
                stmts[ns] = s;
                land[ns]  = IR_node_alloc(g, IR_SUCCEED);
                ns++;
            }
            bb_label_registry_reset();
            for (int i = 0; i < ns; i++) {
                const char *li = stmt_attr_str(stmt_attr_find(stmts[i], ":lbl"));
                if (li) bb_label_registry_add(li, land[i]);
            }
            int built = 0;
            for (int i = 0; i < ns; i++) {
                const tree_t *s = stmts[i];
                IR_t *fall = (i + 1 < ns) ? land[i + 1] : PSUCC;
                IR_t *tgt_u = NULL, *tgt_s = NULL, *tgt_f = NULL;
                const char *gu = goto_node_str(stmt_goto_find(s, TT_GOTO_U));
                const char *gs = goto_node_str(stmt_goto_find(s, TT_GOTO_S));
                const char *gf = goto_node_str(stmt_goto_find(s, TT_GOTO_F));
                for (int j = 0; j < ns; j++) {
                    const char *lj = stmt_attr_str(stmt_attr_find(stmts[j], ":lbl"));
                    if (!lj) continue;
                    if (gu && !strcmp(gu, lj)) tgt_u = land[j];
                    if (gs && !strcmp(gs, lj)) tgt_s = land[j];
                    if (gf && !strcmp(gf, lj)) tgt_f = land[j];
                }
                if (gu && !tgt_u) tgt_u = make_indirect_goto(g, gu, fall);
                if (gs && !tgt_s) tgt_s = make_indirect_goto(g, gs, fall);
                if (gf && !tgt_f) tgt_f = make_indirect_goto(g, gf, fall);
                const tree_t *eu = goto_node_expr(stmt_goto_find(s, TT_GOTO_U));
                const tree_t *es = goto_node_expr(stmt_goto_find(s, TT_GOTO_S));
                const tree_t *ef = goto_node_expr(stmt_goto_find(s, TT_GOTO_F));
                if (eu && !tgt_u) tgt_u = make_computed_goto(g, eu, fall);
                if (es && !tgt_s) tgt_s = make_computed_goto(g, es, fall);
                if (ef && !tgt_f) tgt_f = make_computed_goto(g, ef, fall);
                if (gu && !tgt_u && !strcmp(gu, "END")) tgt_u = PSUCC;
                if (gs && !tgt_s && !strcmp(gs, "END")) tgt_s = PSUCC;
                if (gf && !tgt_f && !strcmp(gf, "END")) tgt_f = PSUCC;
                if (gu && !tgt_u) { if (!strcmp(gu, "RETURN") || !strcmp(gu, "NRETURN")) tgt_u = RET; else if (!strcmp(gu, "FRETURN")) tgt_u = FRET; }
                if (gs && !tgt_s) { if (!strcmp(gs, "RETURN") || !strcmp(gs, "NRETURN")) tgt_s = RET; else if (!strcmp(gs, "FRETURN")) tgt_s = FRET; }
                if (gf && !tgt_f) { if (!strcmp(gf, "RETURN") || !strcmp(gf, "NRETURN")) tgt_f = RET; else if (!strcmp(gf, "FRETURN")) tgt_f = FRET; }
                IR_t *γ_tgt = tgt_u ? tgt_u : (tgt_s ? tgt_s : fall);
                IR_t *ω_tgt = tgt_u ? tgt_u : (tgt_f ? tgt_f : fall);
                tree_t *subj = lp_s_expr(s, ":subj");
                if (!subj) {
                    land[i]->γ.node = tgt_u ? tgt_u : fall; memcpy(land[i]->γ.sz, "α", 3);
                    built = 1;
                    continue;
                }
                if (subj->t == TT_VAR && subj->v.sval &&
                    (!strcmp(subj->v.sval, "RETURN") || !strcmp(subj->v.sval, "FRETURN") || !strcmp(subj->v.sval, "NRETURN"))) {
                    land[i]->γ.node = (!strcmp(subj->v.sval, "FRETURN")) ? FRET : RET; memcpy(land[i]->γ.sz, "α", 3);
                    built = 1;
                    continue;
                }
                tree_t *expr = subj;
                if (stmt_attr_find(s, ":eq")) {
                    if (stmt_attr_find(s, ":pat")) { land[i]->γ.node = fall; memcpy(land[i]->γ.sz, "α", 3); continue; }
                    tree_t *repl = lp_s_expr(s, ":repl");
                    if (!repl) { land[i]->γ.node = fall; memcpy(land[i]->γ.sz, "α", 3); continue; }
                    if (subj->t == TT_SCAN && subj->n >= 2) {
                        tree_t *scn = ast_stmt_new(TT_SCAN);
                        if (!scn) { land[i]->γ.node = fall; memcpy(land[i]->γ.sz, "α", 3); continue; }
                        ast_push(scn, subj->c[0]);
                        ast_push(scn, subj->c[1]);
                        ast_push(scn, repl);
                        expr = scn;
                    } else {
                        tree_t *asn = ast_stmt_new(TT_ASSIGN);
                        if (!asn) { land[i]->γ.node = fall; memcpy(land[i]->γ.sz, "α", 3); continue; }
                        ast_push(asn, subj);
                        ast_push(asn, repl);
                        expr = asn;
                    }
                }
                IR_t *α = NULL, *β = NULL;
                IR_t *top = lower_value_entry(g, expr, γ_tgt, ω_tgt, &α, &β);
                if (!top || !α) { land[i]->γ.node = fall; memcpy(land[i]->γ.sz, "α", 3); continue; }
                land[i]->γ.node = α; memcpy(land[i]->γ.sz, "α", 3);
                built = 1;
            }
            if (built) {
                g->entry = (ns > 0) ? land[0] : PSUCC;
                (void) PFAIL;
                int bb_idx = bb_program_add(&g_stage2.bbp, g);
                int pi = stage2_proc_grow(&g_stage2);
                g_stage2.proc_table[pi].name     = "main";
                g_stage2.proc_table[pi].proc     = NULL;
                g_stage2.proc_table[pi].entry_pc = -1;
                g_stage2.proc_table[pi].bb_idx   = bb_idx;
                g_stage2.proc_table[pi].nparams  = 0;
                for (int di = 0; di < ns; di++) {
                    tree_t *dsubj = lp_s_expr(stmts[di], ":subj");
                    if (!dsubj || dsubj->t != TT_FNC || !dsubj->v.sval || strcmp(dsubj->v.sval, "DEFINE")) continue;
                    if (dsubj->n < 1 || !dsubj->c[0] || dsubj->c[0]->t != TT_QLIT || !dsubj->c[0]->v.sval) continue;
                    char fname[64];
                    char params[STAGE2_FRAME_SLOT_MAX][64]; int np = 0;
                    char locals[STAGE2_FRAME_SLOT_MAX][64]; int nl = 0;
                    if (!sno_parse_define_proto(dsubj->c[0]->v.sval, fname, params, &np, locals, &nl)) continue;
                    IR_t *body = NULL;
                    for (int j = 0; j < ns; j++) {
                        const char *lj = stmt_attr_str(stmt_attr_find(stmts[j], ":lbl"));
                        if (lj && !strcmp(lj, fname)) { body = land[j]; break; }
                    }
                    if (!body) continue;
                    IR_graph_t *fg = (IR_graph_t *) calloc(1, sizeof(IR_graph_t));
                    if (!fg) continue;
                    *fg = *g;
                    fg->entry = body;
                    int fidx = bb_program_add(&g_stage2.bbp, fg);
                    int fpi  = stage2_proc_grow(&g_stage2);
                    g_stage2.proc_table[fpi].name     = lp_strdup(fname);
                    g_stage2.proc_table[fpi].proc     = NULL;
                    g_stage2.proc_table[fpi].entry_pc = -1;
                    g_stage2.proc_table[fpi].bb_idx   = fidx;
                    g_stage2.proc_table[fpi].nparams  = np;
                    Scope *sc = &g_stage2.proc_table[fpi].lower_sc;
                    sc->n = 0;
                    for (int k = 0; k < np && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = lp_strdup(params[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
                    for (int k = 0; k < nl && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = lp_strdup(locals[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
                    if (sc->n < STAGE2_FRAME_SLOT_MAX) { sc->e[sc->n].name = lp_strdup(fname); sc->e[sc->n].slot = sc->n; sc->n++; }
                }
            }
        }
    }
    if (mask & (1u << LANG_PASCAL)) {
        for (int pi = 0; pi < g_stage2.proc_count; pi++) {
            const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
            if (!proc || proc->t != TT_PROC_DECL) continue;
            if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
            int bb_idx = lower_pascal_body(proc);
            if (bb_idx >= 0) {
                g_stage2.proc_table[pi].bb_idx = bb_idx;
                const tree_t *plist = (proc->n >= 2) ? proc->c[1] : NULL;
                g_stage2.proc_table[pi].nparams = plist ? plist->n : 0;
                int np = g_stage2.proc_table[pi].nparams;
                Scope *sc = &g_stage2.proc_table[pi].lower_sc;
                sc->n = 0;
                for (int k = 0; k < np && plist && k < plist->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                    const tree_t *pv = plist->c[k];
                    if (!pv || !pv->v.sval) continue;
                    sc->e[sc->n].name = lp_strdup(pv->v.sval);
                    sc->e[sc->n].slot = sc->n;
                    sc->n++;
                }
                const tree_t *locals = (proc->n >= 1) ? proc->c[proc->n - 1] : NULL;
                if (locals && locals->t == TT_VLIST) {
                    g_stage2.proc_table[pi].decl_level = (int) locals->v.ival;
                    for (int k = 0; k < locals->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                        const tree_t *lv = locals->c[k];
                        if (!lv || !lv->v.sval) continue;
                        if (scope_get(sc, lv->v.sval) >= 0) continue;
                        sc->e[sc->n].name = lp_strdup(lv->v.sval);
                        sc->e[sc->n].slot = sc->n;
                        sc->n++;
                    }
                }
            }
        }
        int pas_has_nesting = 0;
        for (int pi = 0; pi < g_stage2.proc_count; pi++) {
            const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
            if (proc && proc->t == TT_PROC_DECL && (g_stage2.proc_table[pi].decl_level > 1 || g_stage2.proc_table[pi].byref_mask)) { pas_has_nesting = 1; break; }
        }
        if (pas_has_nesting) {
            for (int pi = 0; pi < g_stage2.proc_count; pi++) {
                const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
                if (!proc || proc->t != TT_PROC_DECL) continue;
                if (!g_stage2.proc_table[pi].name || !strcmp(g_stage2.proc_table[pi].name, "main")) continue;
                int idx = g_stage2.proc_table[pi].bb_idx;
                if (idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx]) continue;
                Scope *scs[16]; int dls[16]; int pis[16];
                int nch = pas_scope_chain(pi, scs, dls, pis, 16);
                if (!nl_on(1)) pas_rewrite_graph(g_stage2.bbp.table[idx], scs, dls, pis, nch);
                g_stage2.bbp.table[idx]->nslots = g_stage2.proc_table[pi].lower_sc.n + 1;
            }
        }
        g_stage2.lang = IR_LANG_PAS;
        return &g_stage2;
    }
    if (mask & (1u << LANG_ICN)) {
        for (int pi = 0; pi < g_stage2.proc_count; pi++) {
            const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
            if (!proc || proc->t != TT_PROC_DECL) continue;
            g_stage2.proc_table[pi].is_generator = proc_body_has_suspend(proc);
        }
        for (int pi = 0; pi < g_stage2.proc_count; pi++) {
            const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
            if (!proc || proc->t != TT_PROC_DECL) continue;
            if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
            int bb_idx = lower_icon_body(proc);
            if (bb_idx >= 0) {
                g_stage2.proc_table[pi].bb_idx = bb_idx;
                const tree_t *plist = (proc->n >= 2) ? proc->c[1] : NULL;
                int np = g_stage2.proc_table[pi].nparams;
                Scope *sc = &g_stage2.proc_table[pi].lower_sc;
                sc->n = 0;
                for (int k = 0; k < np && plist && k < plist->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                    const tree_t *pv = plist->c[k];
                    if (!pv || !pv->v.sval) continue;
                    sc->e[sc->n].name = lp_strdup(pv->v.sval);
                    sc->e[sc->n].slot = sc->n;
                    sc->n++;
                }
            }
        }
        g_stage2.lang = IR_LANG_ICN;
        return &g_stage2;
    }
    if (mask & (1u << LANG_PL)) {
        const char *goal_key = NULL;
        char keybuf[128];
        for (int i = 0; i < prog->n; i++) {
            const tree_t *s = prog->c[i];
            if (!s || s->t != TT_STMT) continue;
            if (lp_s_int(s, ":lang") != LANG_PL) continue;
            const tree_t *subj = lp_s_expr(s, ":subj");
            if (!subj) continue;
            if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
                const tree_t *gt = subj->c[0];
                if (gt && (gt->t == TT_QLIT || gt->t == TT_NAME) && gt->v.sval) {
                    snprintf(keybuf, sizeof keybuf, "%s/0", gt->v.sval);
                    goal_key = keybuf;
                } else if (gt && gt->t == TT_FNC && gt->v.sval) {
                    snprintf(keybuf, sizeof keybuf, "%s/%d", gt->v.sval, gt->n);
                    goal_key = keybuf;
                }
            }
        }
        if (!goal_key) goal_key = "main/0";
        const tree_t *choice = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, goal_key);
        const tree_t *clause = NULL;
        if (choice) {
            if (choice->t == TT_CLAUSE) clause = choice;
            else if (choice->t == TT_CHOICE && choice->n >= 1) clause = choice->c[0];
        }
        if (clause) {
            int bb_idx = lower_pl_clause_graph(clause);
            if (bb_idx >= 0) {
                int pi = stage2_proc_grow(&g_stage2);
                g_stage2.proc_table[pi].name     = "main";
                g_stage2.proc_table[pi].proc     = NULL;
                g_stage2.proc_table[pi].entry_pc = -1;
                g_stage2.proc_table[pi].bb_idx   = bb_idx;
                g_stage2.proc_table[pi].nparams  = 0;
            }
        }
        lower_pl_register_all_preds();
        g_stage2.lang = IR_LANG_PL;
        return &g_stage2;
    }
    if (mask & (1u << LANG_RAKU)) {
        for (int pi = 0; pi < g_stage2.proc_count; pi++) {
            const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
            if (!proc || proc->t != TT_SUB_DECL) continue;
            if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
            int bb_idx = lower_raku_body(proc);
            if (bb_idx >= 0) g_stage2.proc_table[pi].bb_idx = bb_idx;
        }
        g_stage2.lang = IR_LANG_RKU;
        return &g_stage2;
    }
    g_stage2.lang = IR_LANG_SNO;
    return &g_stage2;
}
/*====================================================================================================================*/
