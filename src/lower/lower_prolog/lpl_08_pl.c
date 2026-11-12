#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "lower/lower_common.h"
#include "optimizer/ir_query.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_03_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_07_pl.h"
#include "lower/lower_prolog/lpl_08_pl.h"
static int pl_goal_is_dollar(const tree_t * g);
static IR_t * pl_lower_conj_guard(lcx_t * cx, const tree_t * const * gl, int ng, int k, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t ** redo_out, IR_t ** tail_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void collect_conj(const tree_t * t, lc_vec * out) {
    while (t) {
        if ((t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ",")) || t->t == TT_PROGRAM) {
            if (t->n == 0) return;
            for (int i = 0; i + 1 < t->n; i++) collect_conj(t->c[i], out);
            t = t->c[t->n - 1];
            continue;
        }
        lc_vec_push(out, &t);
        return;
    }
}
static void pl_kill_mark(lcx_t * cx, const tree_t * g, IR_t * nd) {
    int oi = -1;
    if (!cx->kill_gl || !g || !nd || nd->op != IR_CALL_PROC_STAGED || g->t != TT_FNC) return;
    for (int j = 0; j < cx->kill_ng && oi < 0; j++) if (cx->kill_gl[j] == g) oi = j;
    if (oi < 0 || oi >= cx->kill_ng - 1) return;
    for (int k = 0; k < g->n; k++) {
        const tree_t * a = g->c[k];
        IR_t * op = ir_call_arg(nd, k);
        int v = (a && a->t == TT_VAR) ? (int) a->v.ival : -1;
        if (v < 0 || v >= cx->kill_nv || cx->kill_last[v] != oi || (v < cx->nvalias && cx->valias[v])) continue;
        if (cx->kill_co[v] < oi || (cx->kill_first[v] == oi && cx->kill_co[v] == oi)) continue;
        if (!op || op->op != IR_VAR_REF || !IR_LIT(op).sval || strcmp(IR_LIT(op).sval, pl_vname(cx, v))) continue;
        op->seal = cx->kill_first[v] == oi ? IR_SEAL_KILL_HOME : IR_SEAL_KILL_VALUE;
    }
}
IR_t * pl_lower_conj(lcx_t * cx, const tree_t * const * gl, int ng, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t ** redo_out, IR_t ** tail_out) {
    const tree_t * gu[2 * (ng > 0 ? ng : 1)];
    { int nu = pl_undo_wrap(cx, gl, ng, gu); if (nu > ng) { gl = gu; ng = nu; } }
    if (cx->cur_key && cx->cutω == cx->clause_cutω) for (int k = 0; k < ng; k++) if (pl_goal_is_dollar(gl[k]))
        return pl_lower_conj_guard(cx, gl, ng, k, γtail, ωbase, entry_out, redo_out, tail_out);
    const tree_t * gt[ng > 0 ? ng : 1];
    if (ng > 1) {
        int nt = 0;
        for (int k = 0; k < ng; k++) if (!(gl[k] && (gl[k]->t == TT_QLIT || gl[k]->t == TT_NAME) && gl[k]->v.sval && !strcmp(gl[k]->v.sval, "true"))) gt[nt++] = gl[k];
        if (nt == 0) gt[nt++] = gl[ng - 1];
        if (nt < ng) { gl = gt; ng = nt; }
    }
    IR_t ** gn = (IR_t **) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** en = (IR_t **) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** rd = (IR_t **) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    int * rds = (int *) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(int));
    IR_t * next = γtail;
    int tail = cx->tail, tail_pop = cx->tail_pop;
    cx->tail = 0;
    cx->tail_pop = 0;
    for (int i = ng - 1; i >= 0; i--) {
        IR_t * e = NULL;
        cx->meta_redo_set = 0;
        cx->tail = (i == ng - 1) ? tail : 0;
        cx->tail_pop = (i == ng - 1) ? tail_pop : 0;
        IR_t * nd = goal(cx, gl[i], next, ωbase, &e);
        pl_kill_mark(cx, gl[i], nd);
        cx->tail = 0;
        cx->tail_pop = 0;
        rd[i] = cx->meta_redo_set ? cx->meta_redo : NULL;
        rds[i] = cx->meta_redo_set;
        cx->meta_redo_set = 0;
        gn[i] = nd;
        en[i] = e ? e : nd;
        next = en[i];
    }
    IR_t * last_res = ωbase;
    int last_res_beta = 0;
    for (int i = 0; i < ng; i++) {
        if (gn[i] && gn[i]->op == IR_CUT && IR_LIT(gn[i]).ival == cx->cut_scope) { lc_ω_to(gn[i], cx->cutω); last_res = cx->cutω; last_res_beta = 0; continue; }
        if (gn[i] && gn[i]->op == IR_GOTO) {
            if (last_res_beta) lc_γ_to_β(gn[i], last_res);
            else lc_γ_to(gn[i], last_res);
            if (rds[i] && rd[i]) { last_res = rd[i]; last_res_beta = 1; }
        } else if (last_res_beta) lc_ω_to_β(gn[i], last_res);
        else lc_ω_to(gn[i], last_res);
        if (gn[i] &&
            (gn[i]->op == IR_CALL_PROC_STAGED || gn[i]->op == IR_DISJUNCTION || gn[i]->op == IR_GATE || ir_is_generator_kind(gn[i]->op) ||
            (gn[i]->op == IR_CLEANUP && IR_LIT(gn[i]).ival == CLEANUP_RESUME))) {
            last_res = gn[i];
            last_res_beta = 1;
        }
    }
    if (redo_out) *redo_out = last_res_beta ? last_res : NULL;
    if (entry_out) *entry_out = (ng > 0) ? en[0] : γtail;
    if (tail_out) *tail_out = (ng > 0) ? gn[ng - 1] : NULL;
    IR_t * first = (ng > 0) ? gn[0] : NULL;
    ct_drop(gn);
    ct_drop(en);
    ct_drop(rd);
    ct_drop(rds);
    return first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_has_caret(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "^")) return 1;
    for (int i = 0; i < t->n; i++) if (pl_tree_has_caret(t->c[i])) return 1;
    return 0;
}
const tree_t * pl_caret_body(const tree_t * t) { int guard = 0; while (t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "^") && t->n == 2 && guard++ < 256) t = t->c[1]; return t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_tree_is_control(const tree_t * t) {
    return t && t->t == TT_FNC && t->v.sval && t->n == 2 && (!strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|") || !strcmp(t->v.sval, "->") || !strcmp(t->v.sval, "*->"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_goal_holds_cut(const tree_t * g) {
    if (!g) return 0;
    if (g->t == TT_CUT || ((g->t == TT_QLIT || g->t == TT_NAME) && g->v.sval && !strcmp(g->v.sval, "!"))) return 1;
    if (!pl_tree_is_control(g) && g->t != TT_PROGRAM && g->t != TT_IF) return 0;
    for (int i = 0; i < g->n; i++) if (pl_goal_holds_cut(g->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_noncallable_goal(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_ILIT || t->t == TT_FLIT) return 1;
    if (pl_tree_is_control(t)) { for (int i = 0; i < t->n; i++) if (pl_tree_noncallable_goal(t->c[i])) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_goal_arg_wants_guard(const tree_t * t) { return t && (t->t == TT_VAR || pl_tree_noncallable_goal(t)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_tree_control_var_leaf(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_VAR) return 1;
    if (pl_tree_is_control(t)) { for (int i = 0; i < t->n; i++) if (pl_tree_control_var_leaf(t->c[i])) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_is_ite(const tree_t * t) {
    return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval &&
        !strcmp(t->c[0]->v.sval, "->");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_softcut(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "*->") && t->n == 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_is_scite(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && pl_is_softcut(t->c[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_disj(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && !pl_is_ite(t) && !pl_is_scite(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_disj(const tree_t * t, lc_vec * out) { if (pl_is_disj(t)) { collect_disj(t->c[0], out); collect_disj(t->c[1], out); return; } lc_vec_push(out, &t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_disj_entry(lcx_t * cx, IR_t * e, IR_t * dj) {
    int guard = 0;
    while (e && e->op == IR_SUCCEED && e->γ.node && e->γ.node != dj && guard++ < 4096) e = e->γ.node;
    if (e && e->op != IR_SUCCEED) return e;
    { IR_t * g = lpl_build(cx, IR_GOTO, dj, dj); memcpy(g->γ.sz, "σ", 3); g->γ.sz[3] = 0; memcpy(g->ω.sz, "φ", 3); g->ω.sz[3] = 0; return g; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_conj_top_cut(const lc_vec * v) {
    for (int i = 0; i < v->n; i++) {
        const tree_t * g = ((const tree_t * const *) v->data)[i];
        if (g && (g->t == TT_CUT || ((g->t == TT_QLIT || g->t == TT_NAME) && g->v.sval && !strcmp(g->v.sval, "!")) || pl_goal_is_dollar(g))) return 1;
    }
    return 0;
}
static void pl_pop_ok(lcx_t * cx, IR_t * nd) { if (cx->pop_ok && nd) lc_vec_push(cx->pop_ok, &nd); }
void pl_pop_vet(lcx_t * cx, int from) {
    int any = 0, bad = 0;
    for (int k = from; k < cx->g->n; k++) {
        IR_t * x = cx->g->all[k];
        if (!x) continue;
        if (x->op == IR_CALL_PROC_STAGED && x->seal == PL_SEAL_TAIL_POP) any = 1;
        if (x->op == IR_DISJUNCTION || x->op == IR_TO || x->op == IR_TO_BY || x->op == IR_BOUND) {
            int ok = 0;
            IR_t * const * v = cx->pop_ok ? (IR_t * const *) cx->pop_ok->data : (IR_t * const *) 0;
            for (int j = 0; v && j < cx->pop_ok->n; j++) if (v[j] == x) { ok = 1; break; }
            if (!ok) bad = 1;
        }
    }
    if (any && bad) for (int k = from; k < cx->g->n; k++) { IR_t * x = cx->g->all[k]; if (x && x->op == IR_CALL_PROC_STAGED && x->seal == PL_SEAL_TAIL_POP) x->seal = PL_SEAL_TAIL; }
}
IR_t * pl_lower_disj(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    int tail = cx->tail, tail_pop = cx->tail_pop;
    cx->tail = 0;
    cx->tail_pop = 0;
    lc_vec bv;
    lc_vec_init(&bv, (int) sizeof(const tree_t *));
    collect_disj(t, &bv);
    const tree_t * const * br = (const tree_t * const *) bv.data;
    const tree_t * wide[32];
    int nb = bv.n;
    if (nb > 32) {
        int ite = 0;
        const tree_t * rest = br[nb - 1];
        for (int j = 0; j < nb; j++) if (br[j] && br[j]->t == TT_FNC && br[j]->v.sval && br[j]->n == 2 && (!strcmp(br[j]->v.sval, "->") || !strcmp(br[j]->v.sval, "*->"))) ite = 1;
        if (ite) pl_refuse("disjunction wider than 32 branches with an if-then arm", ";", 3);
        for (int j = nb - 2; j >= 31; j--) rest = pl_cc_fnc2(";", (tree_t *) br[j], (tree_t *) rest);
        for (int j = 0; j < 31; j++) wide[j] = br[j];
        wide[31] = pl_cc_fnc2(",", (tree_t *) pl_atom_goal("true"), (tree_t *) rest);
        br = wide;
        nb = 32;
    }
    IR_t * dj = lpl_build(cx, IR_DISJUNCTION, γnext, ωfail);
    for (int j = 0; j < nb; j++) {
        int before = cx->g->n;
        lc_vec glv;
        lc_vec_init(&glv, (int) sizeof(const tree_t *));
        collect_conj(br[j], &glv);
        IR_t * bentry = NULL;
        IR_t * redo = NULL;
        IR_t * dtail = NULL;
        int arm_pop = tail_pop && j == nb - 1;
        cx->tail = tail;
        cx->tail_pop = arm_pop;
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, dj, dj, &bentry, &redo, &dtail);
        cx->tail = 0;
        cx->tail_pop = 0;
        if (pl_seal_branch_tail(tail, arm_pop, dtail)) pl_pop_ok(cx, dj);
        for (int k = before; k < cx->g->n; k++) {
            IR_t * x = cx->g->all[k];
            if (!x) continue;
            if (x->γ.node == dj) { if (x->op == IR_GOTO && x->ω.node == dj) memcpy(x->γ.sz, "φ", 3); else memcpy(x->γ.sz, "σ", 3); x->γ.sz[3] = 0; }
            if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
        }
        ir_operand_push(dj, pl_disj_entry(cx, bentry ? bentry : (first ? first : dj), dj));
        ir_operand_push(dj, redo ? redo : (pl_conj_top_cut(&glv) ? pl_cut_redo(cx) : dj));
    }
    for (int j = 0; j < nb; j++) ir_operand_push(dj, NULL);
    IR_LIT(dj).ival = (long) nb;
    if (entry_out) *entry_out = dj;
    return dj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_mark_into(lcx_t * cx, int before, IR_t * tgt, const char * gsz, const char * osz) {
    for (int k = before; k < cx->g->n; k++) {
        IR_t * x = cx->g->all[k];
        if (!x) continue;
        if (gsz && x->γ.node == tgt) { memcpy(x->γ.sz, gsz, strlen(gsz) + 1); }
        if (osz && x->ω.node == tgt) { memcpy(x->ω.sz, osz, strlen(osz) + 1); }
    }
}
static int pl_cond_may_clean(const tree_t * t) {
    static const char * const meta[] = { "call", "findall", "bagof", "setof", "forall", "catch", "once", "ignore", "not", "aggregate_all", "setup_call_cleanup", "call_cleanup",
        "setup_call_catcher_cleanup", "with_output_to", "format", "apply", NULL };
    if (!t) return 0;
    if (t->t == TT_VAR) return 1;
    if (t->t != TT_FNC && t->t != TT_QLIT && t->t != TT_NAME) return 0;
    const char * nm = t->v.sval;
    int ar = t->t == TT_FNC ? t->n : 0;
    if (!nm) return 1;
    if (ar == 2 && (!strcmp(nm, ",") || !strcmp(nm, ";") || !strcmp(nm, "->") || !strcmp(nm, "*->"))) return pl_cond_may_clean(t->c[0]) || pl_cond_may_clean(t->c[1]);
    if (ar == 1 && !strcmp(nm, "\\+")) return pl_cond_may_clean(t->c[0]);
    for (int i = 0; meta[i]; i++) if (!strcmp(meta[i], nm)) return 1;
    return pl_file_defines(nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_probe_before(lcx_t * cx, IR_t * target, IR_t * mark, int needs_mark) {
    if (!pl_scc_armed() || (needs_mark && !mark)) return target;
    IR_t * pr = lpl_build(cx, IR_CLEANUP, NULL, target);
    IR_LIT(pr).ival = CLEANUP_PROBE;
    if (mark) ir_operand_push(pr, mark);
    IR_t * ce = NULL;
    IR_t * cn = goal(cx, pl_atom_goal("$scc_cut"), target, target, &ce);
    lc_γ_to(pr, ce ? ce : cn);
    return pr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_lower_ite(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    int tail = cx->tail, tail_pop = cx->tail_pop;
    cx->tail = 0;
    cx->tail_pop = 0;
    IR_t * ig = lpl_build(cx, IR_GATE, γnext, ωfail);
    IR_t * mark = lpl_build(cx, IR_BOUND, NULL, ig);
    pl_pop_ok(cx, mark);
    IR_t * unmk_c = lpl_build(cx, IR_UNMARK, ig, ωfail);
    ir_operand_push(unmk_c, mark);
    IR_LIT(unmk_c).ival = 5 | 32;
    IR_t * unmk_f = lpl_build(cx, IR_UNMARK, ig, ωfail);
    ir_operand_push(unmk_f, mark);
    IR_LIT(unmk_f).ival = 5;
    IR_t * arm_entry[2] = { NULL, NULL };
    const tree_t * arms[2];
    int nb = 0;
    arms[nb++] = T;
    if (E) arms[nb++] = E;
    for (int j = 0; j < nb; j++) {
        IR_t * ml = lpl_build(cx, IR_GATE_ARM, NULL, ωfail);
        lc_vec av;
        lc_vec_init(&av, (int) sizeof(const tree_t *));
        collect_conj(arms[j], &av);
        IR_t * ae = NULL;
        IR_t * redo = NULL;
        IR_t * atail = NULL;
        cx->tail = tail;
        cx->tail_pop = tail_pop;
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) av.data, av.n, ml, unmk_c, &ae, &redo, &atail);
        cx->tail = 0;
        cx->tail_pop = 0;
        pl_seal_branch_tail(tail, tail_pop, atail);
        IR_t * rt = redo ? redo : (pl_conj_top_cut(&av) ? pl_cut_redo(cx) : ig);
        ir_operand_push(ml, rt);
        ir_operand_push(ml, ig);
        ir_operand_push(ig, ml);
        IR_LIT(ig).ival = ig->n_operands;
        ml->seal = (rt != ig) ? 1 : 0;
        IR_LIT(ml).ival = ig->n_operands - 1;
        arm_entry[j] = ae ? ae : (first ? first : ml);
    }
    {
        int before = cx->g->n;
        lc_vec cv;
        lc_vec_init(&cv, (int) sizeof(const tree_t *));
        collect_conj(C, &cv);
        IR_t * ce = NULL;
        lc_γ_to(unmk_f, (nb > 1) ? arm_entry[1] : ig);
        IR_t * saveω = cx->cutω;
        IR_t * ccut = lpl_build(cx, IR_GOTO, unmk_f, unmk_f);
        cx->cutω = ccut;
        IR_t * savecutω = cx->cutω;
        cx->cutω = ccut;
        int save_scope = cx->cut_scope;
        cx->cut_scope = ++cx->scope_seq;
        IR_t * save_mark_0 = cx->cut_mark;
        cx->cut_mark = mark;
        IR_t * fence = lpl_build(cx, IR_UNMARK, arm_entry[0], unmk_c);
        ir_operand_push(fence, mark);
        IR_LIT(fence).ival = 2;
        IR_t * cfirst = pl_lower_conj(cx, (const tree_t * const *) cv.data, cv.n, pl_cond_may_clean(C) ? pl_probe_before(cx, fence, mark, 1) : fence, unmk_f, &ce, NULL, NULL);
        cx->cutω = savecutω;
        cx->cut_scope = save_scope;
        cx->cut_mark = save_mark_0;
        cx->cutω = saveω;
        lc_γ_to(mark, ce ? ce : (cfirst ? cfirst : arm_entry[0]));
        if (entry_out) *entry_out = mark;
    }
    return ig;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_sc_entry(IR_t * e) { int guard = 0; while (e && e->op == IR_SUCCEED && e->γ.node && guard++ < 4096) e = e->γ.node; return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_lower_softcut(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    int tail = cx->tail, tail_pop = cx->tail_pop, sc_pop = 0;
    cx->tail = 0;
    cx->tail_pop = 0;
    IR_t * ig = lpl_build(cx, IR_GATE, γnext, ωfail);
    IR_t * eg = lpl_build(cx, IR_GATE, γnext, ig);
    cx->g->keeps_choice = 1;
    IR_t * mark = lpl_build(cx, IR_BOUND, NULL, ig);
    IR_t * unmk_f = lpl_build(cx, IR_UNMARK, NULL, ig);
    ir_operand_push(unmk_f, mark);
    IR_LIT(unmk_f).ival = 1;
    IR_t * unmk_c = lpl_build(cx, IR_UNMARK, ig, ig);
    ir_operand_push(unmk_c, mark);
    IR_LIT(unmk_c).ival = 5;
    lc_γ_to_β(unmk_f, eg);
    IR_t * ml_e = lpl_build(cx, IR_GATE_ARM, NULL, ig);
    IR_t * ml_c = lpl_build(cx, IR_GATE_ARM, NULL, ig);
    IR_t * ml_t = lpl_build(cx, IR_GATE_ARM, NULL, ig);
    IR_t * credo = NULL;
    {
        lc_vec cv;
        lc_vec_init(&cv, (int) sizeof(const tree_t *));
        collect_conj(C, &cv);
        IR_t * ce = NULL;
        IR_t * savecutω = cx->cutω;
        cx->cutω = lpl_build(cx, IR_GOTO, unmk_f, unmk_f);
        int save_scope = cx->cut_scope;
        cx->cut_scope = ++cx->scope_seq;
        IR_t * save_mark_0 = cx->cut_mark;
        cx->cut_mark = mark;
        IR_t * cfirst = pl_lower_conj(cx, (const tree_t * const *) cv.data, cv.n, ml_c, unmk_f, &ce, &credo, NULL);
        cx->cutω = savecutω;
        cx->cut_scope = save_scope;
        cx->cut_mark = save_mark_0;
        lc_γ_to(ml_e, pl_sc_entry(ce ? ce : (cfirst ? cfirst : ml_c)));
    }
    IR_t * tredo = NULL;
    int sc_cut_in_then = 0;
    int sc_cut_in_else = 0;
    IR_t * sc_then_cut = NULL;
    IR_t * sc_else_cut = NULL;
    {
        lc_vec tv;
        lc_vec_init(&tv, (int) sizeof(const tree_t *));
        collect_conj(T, &tv);
        int before = cx->g->n;
        IR_t * te = NULL;
        IR_t * ttail = NULL;
        cx->tail = tail;
        cx->tail_pop = tail_pop && !credo;
        IR_t * tfirst = pl_lower_conj(cx, (const tree_t * const *) tv.data, tv.n, ml_t, credo ? credo : unmk_c, &te, &tredo, &ttail);
        cx->tail = 0;
        cx->tail_pop = 0;
        sc_pop |= pl_seal_branch_tail(tail, tail_pop && !credo, ttail);
        sc_cut_in_then = !tredo && pl_conj_top_cut(&tv);
        if (sc_cut_in_then) sc_then_cut = pl_cut_redo(cx);
        if (credo) pl_mark_into(cx, before, credo, "β", "β");
        lc_γ_to(ml_c, pl_sc_entry(te ? te : (tfirst ? tfirst : ml_t)));
    }
    IR_t * ee = NULL;
    if (E) {
        IR_t * ml_ee = lpl_build(cx, IR_GATE_ARM, NULL, ig);
        lc_vec ev;
        lc_vec_init(&ev, (int) sizeof(const tree_t *));
        collect_conj(E, &ev);
        IR_t * eredo = NULL;
        IR_t * etail = NULL;
        cx->tail = tail;
        cx->tail_pop = tail_pop;
        IR_t * efirst = pl_lower_conj(cx, (const tree_t * const *) ev.data, ev.n, ml_ee, unmk_c, &ee, &eredo, &etail);
        cx->tail = 0;
        cx->tail_pop = 0;
        sc_pop |= pl_seal_branch_tail(tail, tail_pop, etail);
        sc_cut_in_else = !eredo && pl_conj_top_cut(&ev);
        if (sc_cut_in_else) sc_else_cut = pl_cut_redo(cx);
        ir_operand_push(ml_ee, sc_cut_in_else ? sc_else_cut : (eredo ? eredo : unmk_c));
        ir_operand_push(ml_ee, ig);
        ir_operand_push(ig, ml_ee);
        IR_LIT(ig).ival = ig->n_operands;
        ml_ee->seal = (eredo || sc_cut_in_else) ? 1 : 0;
        IR_LIT(ml_ee).ival = ig->n_operands - 1;
        ee = pl_sc_entry(ee ? ee : (efirst ? efirst : ml_ee));
        if (!ee) ee = ml_ee;
    }
    if (sc_pop) pl_pop_ok(cx, mark);
    ir_operand_push(ml_e, E ? ee : eg);
    ir_operand_push(ml_e, eg);
    ir_operand_push(eg, ml_e);
    IR_LIT(eg).ival = eg->n_operands;
    ml_e->seal = 0;
    IR_LIT(ml_e).ival = eg->n_operands - 1;
    ir_operand_push(ml_c, eg);
    ir_operand_push(ml_c, eg);
    ir_operand_push(eg, ml_c);
    IR_LIT(eg).ival = eg->n_operands;
    ml_c->seal = 0;
    IR_LIT(ml_c).ival = eg->n_operands - 1;
    ir_operand_push(ml_t, sc_cut_in_then ? sc_then_cut : (tredo ? tredo : (credo ? credo : unmk_c)));
    ir_operand_push(ml_t, ig);
    ir_operand_push(ig, ml_t);
    IR_LIT(ig).ival = ig->n_operands;
    ml_t->seal = (tredo || credo || sc_cut_in_then) ? 1 : 0;
    IR_LIT(ml_t).ival = ig->n_operands - 1;
    lc_γ_to(mark, ml_e);
    if (entry_out) *entry_out = mark;
    return ig;
}
IR_t * pl_cleanup_node(lcx_t * cx, int kind, IR_t * open, IR_t * γ, IR_t * ω) {
    IR_t * n = lpl_build(cx, IR_CLEANUP, γ, ω);
    IR_LIT(n).ival = kind;
    if (open) ir_operand_push(n, open);
    return n;
}
int pl_dollar_user(int ar) { extern int pl_prelude_defines(const char *, int); return pl_file_defines("$", ar) && !pl_prelude_defines("$", ar); }
static int pl_goal_is_dollar(const tree_t * g) { return g && (g->t == TT_QLIT || g->t == TT_NAME) && g->v.sval && !strcmp(g->v.sval, "$") && !pl_dollar_user(0); }
static IR_t * pl_lower_conj_guard(lcx_t * cx, const tree_t * const * gl, int ng, int k, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t ** redo_out, IR_t ** tail_out) {
    IR_t * out = cx->cutω;
    IR_t * te = NULL;
    tree_t * callee = pl_det_guard_callee(cx->cur_key, gl, ng, k);
    IR_t * tn = goal(cx, pl_det_guard_throw(cx, callee, "nondet"), out, out, &te);
    IR_t * det = pl_cleanup_node(cx, CLEANUP_DET, (IR_t *) 0, γtail, te ? te : tn);
    IR_t * fe = pl_det_fail_road(cx, pl_det_guard_throw(cx, callee, "fail"), out);
    IR_t * re = NULL;
    IR_t * rredo = NULL;
    const tree_t * lastg = (k + 1 < ng) ? gl[ng - 1] : (const tree_t *) 0;
    char selfkey[lastg && lastg->v.sval ? strlen(lastg->v.sval) + 16 : 2];
    selfkey[0] = 0;
    if (lastg && lastg->v.sval) snprintf(selfkey, sizeof selfkey, "%s/%d", lastg->v.sval, lastg->t == TT_FNC ? lastg->n : 0);
    int selfcall = cx->cur_key && !strcmp(selfkey, cx->cur_key);
    if (selfcall) pl_lower_conj(cx, gl + k + 1, ng - k - 1, det, fe, &re, &rredo, tail_out);
    else { cx->tail = 0; cx->tail_pop = 0; pl_lower_conj(cx, gl + k + 1, ng - k - 1, det, fe, &re, &rredo, (IR_t **) 0); if (tail_out) *tail_out = (IR_t *) 0; }
    cx->tail = 0;
    cx->tail_pop = 0;
    const tree_t * pre[k + 1];
    for (int i = 0; i < k; i++) pre[i] = gl[i];
    pre[k] = pl_atom_goal("!");
    IR_t * pe = NULL;
    IR_t * first = pl_lower_conj(cx, pre, k + 1, re ? re : det, ωbase, &pe, (IR_t **) 0, (IR_t **) 0);
    if (redo_out) *redo_out = rredo;
    if (entry_out) *entry_out = pe ? pe : first;
    return first;
}
tree_t * pl_det_unqual_goal(const tree_t * G) {
    const char * colon = (G && (G->t == TT_FNC || G->t == TT_QLIT || G->t == TT_NAME) && G->v.sval) ? strchr(G->v.sval, ':') : (const char *) 0;
    if (!colon || colon == G->v.sval || !colon[1]) return (tree_t *) G;
    tree_t * c = ast_node_new(G->t);
    c->v = G->v;
    c->line = G->line;
    c->v.sval = ct_strdup(colon + 1);
    for (int i = 0; i < G->n; i++) ast_push(c, G->c[i]);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_lower_catch(lcx_t * cx, const tree_t * G, const tree_t * C, const tree_t * R, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * ig = lpl_build(cx, IR_GATE, γnext, ωfail);
    cx->g->keeps_choice = 1;
    IR_t * mark = lpl_build(cx, IR_BOUND, NULL, ig);
    IR_LIT(mark).ival = 64;
    IR_t * unmk = lpl_build(cx, IR_UNMARK, NULL, ig);
    ir_operand_push(unmk, mark);
    IR_LIT(unmk).ival = 4;
    tree_t * ct = ast_node_new(TT_FNC);
    ct->v.sval = (char *) "$catch_handle";
    ast_push(ct, (tree_t *) C);
    IR_t * he = NULL;
    IR_t * hc = pl_leaf_lv(cx, "$catch_handle", ct, 1, NULL, ig, &he);
    lc_γ_to(unmk, he ? he : hc);
    IR_t * arm_entry[2] = { NULL, NULL };
    const tree_t * arms[2] = { pl_goal_holds_cut(G) ? pl_cc_fnc1("call", (tree_t *) G) : G, pl_goal_holds_cut(R) ? pl_cc_fnc1("call", (tree_t *) R) : R };
    for (int j = 0; j < 2; j++) {
        IR_t * ml = lpl_build(cx, IR_GATE_ARM, NULL, ig);
        IR_t * saveω = cx->cutω;
        int save_scope = cx->cut_scope;
        cx->cutω = lpl_build(cx, IR_GOTO, ig, ig);
        cx->cut_scope = ++cx->scope_seq;
        IR_t * save_mark_0 = cx->cut_mark;
        cx->cut_mark = mark;
        lc_vec av;
        lc_vec_init(&av, (int) sizeof(const tree_t *));
        collect_conj(arms[j], &av);
        IR_t * ae = NULL;
        IR_t * redo = NULL;
        IR_t * pop = j ? (IR_t *) 0 : lpl_build(cx, IR_UNMARK, ml, ig);
        if (pop) { ir_operand_push(pop, mark); IR_LIT(pop).ival = 8; }
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) av.data, av.n, pop ? pop : ml, j ? ig : unmk, &ae, &redo, NULL);
        if (pop) lc_ω_to(pop, redo ? redo : ig);
        cx->cutω = saveω;
        cx->cut_scope = save_scope;
        cx->cut_mark = save_mark_0;
        ir_operand_push(ml, redo ? redo : ig);
        ir_operand_push(ml, ig);
        ir_operand_push(ig, ml);
        IR_LIT(ig).ival = ig->n_operands;
        ml->seal = redo ? 1 : 0;
        IR_LIT(ml).ival = ig->n_operands - 1;
        arm_entry[j] = ae ? ae : (first ? first : ml);
    }
    lc_γ_to(hc, arm_entry[1]);
    lc_γ_to(mark, arm_entry[0]);
    if (entry_out) *entry_out = mark;
    return ig;
}
