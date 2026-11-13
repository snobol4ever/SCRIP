#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ir/stage2.h"
#include "ir/stage2_pl.h"
#include "lower/lower_common.h"
#include "runtime/rt/prolog_atom.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_02_pl.h"
#include "lower/lower_prolog/lpl_03_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
#include "lower/lower_prolog/lpl_08_pl.h"
#include "lower/g_lower_pl.h"
static int pl_ssu_clause(const tree_t * cl, int ar);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_refuse(const char * what, const char * detail, int rung) {
    fprintf(stderr, "scrip: prolog: %s%s%s is not on the ladder yet -- rung %d lands it (ARCH-PROLOG-BYRD-BOX-TRANSLATION.md sec E; rung 0 is hello world)\n", what, detail ? " " : "",
        detail ? detail : "", rung);
    exit(2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_atom_goal(const char * nm) { tree_t * n = ast_node_new(TT_QLIT); n->v.sval = (char *) lp_strdup(nm); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_cc_fnc1(const char * f, tree_t * a0) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) f; ast_push(n, a0); return n; }
tree_t * pl_cc_fnc2(const char * f, tree_t * a0, tree_t * a1) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) f; ast_push(n, a0); ast_push(n, a1); return n; }
tree_t * pl_meta_var(const char * nm) { tree_t * v = ast_node_new(TT_VAR); v->v.sval = (char *) nm; return v; }
tree_t * pl_cc_ilit(long long v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
tree_t * pl_cc_ite(tree_t * c, tree_t * th, tree_t * el) { return pl_cc_fnc2(";", pl_cc_fnc2("->", c, th), el); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_undo_probe_tree(const tree_t * t) { return t && t->line == -2035; }
static const tree_t * pl_undo_probe(void) {
    tree_t * c = (tree_t *) pl_atom_goal("$undo_pending"), * th = (tree_t *) pl_atom_goal("$undo_run"), * el = (tree_t *) pl_atom_goal("true"), * ite = pl_cc_ite(c, th, el);
    c->line = th->line = el->line = ite->line = ite->c[0]->line = -2035;
    return ite;
}
int pl_undo_wrap(const lcx_t * cx, const tree_t * const * gl, int ng, const tree_t ** out) {
    int n = 0;
    if (ng <= 0 || !g_stage2_pl.pl_undo_armed || !cx->cur_key || cx->cur_key[0] == '$' || !strcmp(cx->cur_key, "undo/1")) return ng;
    for (int k = 0; k < ng; k++) { if (!pl_undo_probe_tree(gl[k]) && !(k > 0 && pl_undo_probe_tree(gl[k - 1]))) out[n++] = pl_undo_probe(); out[n++] = gl[k]; }
    return n;
}
tree_t * pl_cc_scite(tree_t * c, tree_t * th, tree_t * el) { return pl_cc_fnc2(";", pl_cc_fnc2("*->", c, th), el); }
static tree_t * pl_cc_pi(const char * nm) { return pl_cc_fnc2("/", (tree_t *) pl_atom_goal(nm), pl_cc_ilit(2)); }
tree_t * pl_cc_throw(tree_t * formal, const char * ctx_nm) { return pl_cc_fnc1("throw", pl_cc_fnc2("error", formal, pl_cc_pi(ctx_nm))); }
tree_t * pl_cc_fnc3(const char * f, tree_t * a0, tree_t * a1, tree_t * a2) {
    tree_t * n = ast_node_new(TT_FNC);
    n->v.sval = (char *) f;
    ast_push(n, a0);
    ast_push(n, a1);
    ast_push(n, a2);
    return n;
}
tree_t * pl_cc_pi_ar(const char * nm, int ar) { return pl_cc_fnc2("/", (tree_t *) pl_atom_goal(nm), pl_cc_ilit(ar)); }
tree_t * pl_cc_perm_static(const char * nm, int ar) {
    return pl_cc_fnc1("throw",
        pl_cc_fnc2("error", pl_cc_fnc3("permission_error", (tree_t *) pl_atom_goal("modify"), (tree_t *) pl_atom_goal("static_procedure"), pl_cc_pi_ar(nm, ar)), pl_cc_pi_ar(nm, ar)));
}
tree_t * pl_cc_ischar(tree_t * x) { return pl_cc_fnc2(",", pl_cc_fnc1("atom", x), pl_cc_fnc2("atom_length", x, pl_cc_ilit(1))); }
tree_t * pl_cc_fnc4(const char * f, tree_t * a0, tree_t * a1, tree_t * a2, tree_t * a3) {
    tree_t * n = ast_node_new(TT_FNC);
    n->v.sval = (char *) f;
    ast_push(n, a0);
    ast_push(n, a1);
    ast_push(n, a2);
    ast_push(n, a3);
    return n;
}
tree_t * pl_cc_fnc5(const char * f, tree_t * a0, tree_t * a1, tree_t * a2, tree_t * a3, tree_t * a4) { tree_t * n = pl_cc_fnc4(f, a0, a1, a2, a3); ast_push(n, a4); return n; }
tree_t * pl_cc_freshvar(void) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = g_lower_pl.fresh_next++; return v; }
tree_t * pl_cc_gen2(const char * count_leaf, const char * nth_leaf, tree_t * a0, tree_t * a1, tree_t * a2) {
    tree_t * cv = pl_cc_freshvar();
    tree_t * cv2 = pl_cc_freshvar();
    tree_t * iv = pl_cc_freshvar();
    tree_t * iv2 = pl_cc_freshvar();
    cv2->v.ival = cv->v.ival;
    iv2->v.ival = iv->v.ival;
    tree_t * gen = pl_cc_fnc2(",", pl_cc_fnc1(count_leaf, cv), pl_between_own(pl_cc_fnc3("between", pl_cc_ilit(1), cv2, iv)));
    tree_t * pick = a2 ? pl_cc_fnc4(nth_leaf, iv2, a0, a1, a2) : pl_cc_fnc3(nth_leaf, iv2, a0, a1);
    return pl_cc_fnc2(",", gen, pick);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_cc_print_via_format(tree_t * stream, tree_t * term) {
    tree_t * l = ast_node_new(TT_MAKELIST);
    l->v.ival = 0;
    ast_push(l, term);
    return stream ? pl_cc_fnc3("format", stream, (tree_t *) pl_atom_goal("~p"), l) : pl_cc_fnc2("format", (tree_t *) pl_atom_goal("~p"), l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_opts_are_portray_only(const tree_t * l) {
    int portray = 0;
    if (!l || l->t != TT_MAKELIST || l->v.ival == 1) return 0;
    for (int i = 0; i < l->n; i++) {
        const tree_t * o = l->c[i];
        const tree_t * a = (o && o->t == TT_FNC && o->n == 1) ? o->c[0] : (const tree_t *) 0;
        if (!a || (a->t != TT_QLIT && a->t != TT_NAME) || !a->v.sval || strcmp(a->v.sval, "true")) return 0;
        if (!strcmp(o->v.sval, "portray") || !strcmp(o->v.sval, "portrayed")) portray = 1;
        else if (strcmp(o->v.sval, "numbervars")) return 0;
    }
    return portray;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_cc_gen2_desc(const char * count_leaf, const char * nth_leaf, tree_t * a0, tree_t * a1, tree_t * a2) {
    tree_t * cv = pl_cc_freshvar();
    tree_t * cv2 = pl_cc_freshvar();
    tree_t * cv3 = pl_cc_freshvar();
    tree_t * jv = pl_cc_freshvar();
    tree_t * jv2 = pl_cc_freshvar();
    tree_t * iv = pl_cc_freshvar();
    tree_t * iv2 = pl_cc_freshvar();
    cv2->v.ival = cv->v.ival;
    cv3->v.ival = cv->v.ival;
    jv2->v.ival = jv->v.ival;
    iv2->v.ival = iv->v.ival;
    tree_t * down = pl_cc_fnc2("is", iv, pl_cc_fnc2("-", pl_cc_fnc2("+", cv3, pl_cc_ilit(1)), jv2));
    tree_t * gen = pl_cc_fnc2(",", pl_cc_fnc1(count_leaf, cv), pl_cc_fnc2(",", pl_between_own(pl_cc_fnc3("between", pl_cc_ilit(1), cv2, jv)), down));
    return pl_cc_fnc2(",", gen, pl_cc_fnc4(nth_leaf, iv2, a0, a1, a2));
}
tree_t * pl_cc_key_pi(const char * key) {
    const char * sl = key ? strrchr(key, '/') : (const char *) 0;
    if (!sl || sl == key) return pl_cc_freshvar();
    char * nm = ct_strndup(key, (size_t)(sl - key));
    char * colon = strchr(nm, ':');
    if (colon && colon != nm && colon[1]) { *colon = 0; return pl_cc_fnc2(":", (tree_t *) pl_atom_goal(nm), pl_cc_pi_ar(colon + 1, atoi(sl + 1))); }
    return pl_cc_pi_ar(nm, atoi(sl + 1));
}
static tree_t * pl_cc_det_throw(tree_t * culprit, const char * found, const char * how, tree_t * ctx) {
    tree_t * de = pl_cc_fnc4("determinism_error", culprit, (tree_t *) pl_atom_goal("det"), (tree_t *) pl_atom_goal(found), (tree_t *) pl_atom_goal(how));
    return pl_cc_fnc1("throw", pl_cc_fnc2("error", de, pl_cc_fnc2("context", ctx, pl_cc_freshvar())));
}
IR_t * pl_det_fail_road(lcx_t * cx, tree_t * fthrow, IR_t * out) {
    tree_t * bv = pl_cc_freshvar();
    tree_t * bv2 = pl_cc_freshvar();
    bv2->v.ival = bv->v.ival;
    IR_t * re = NULL;
    IR_t * fe = NULL;
    IR_t * he = NULL;
    IR_t * rn = goal(cx, pl_cc_fnc1("throw", bv2), out, out, &re);
    IR_t * fn = goal(cx, fthrow, out, out, &fe);
    IR_t * hn = pl_leaf_lv(cx, "$catch_handle", pl_cc_fnc1("$catch_handle", bv), 1, re ? re : rn, fe ? fe : fn, &he);
    return he ? he : hn;
}
tree_t * pl_det_guard_throw(lcx_t * cx, const tree_t * callee, const char * found) {
    tree_t * own = pl_cc_det_throw(pl_cc_key_pi(cx->cur_key), found, callee ? "guard_in_caller" : "guard", callee ? callee : pl_cc_key_pi(cx->cur_key));
    if (!callee) return own;
    tree_t * far = pl_cc_det_throw(callee, found, "guard_in_caller", callee);
    return pl_cc_ite(pl_cc_fnc2("current_prolog_flag", (tree_t *) pl_atom_goal("debug"), (tree_t *) pl_atom_goal("true")), own, far);
}
static tree_t * pl_det_prop_throw(const tree_t * rel, const tree_t * dbg, const char * found) {
    tree_t * far = pl_cc_det_throw((tree_t *) rel, found, "property", (tree_t *) rel);
    tree_t * own = pl_cc_det_throw((tree_t *) dbg, found, "property", (tree_t *) rel);
    return pl_cc_ite(pl_cc_fnc2("current_prolog_flag", (tree_t *) pl_atom_goal("debug"), (tree_t *) pl_atom_goal("true")), own, far);
}
IR_t * pl_lower_det_prop(lcx_t * cx, const tree_t * rel, const tree_t * dbg, const tree_t * G, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * out = lpl_build(cx, IR_GOTO, ωfail, ωfail);
    IR_t * te = NULL;
    IR_t * tn = goal(cx, pl_det_prop_throw(rel, dbg, "nondet"), out, out, &te);
    IR_t * bank = pl_cleanup_node(cx, CLEANUP_BANK, (IR_t *) 0, (IR_t *) 0, ωfail);
    IR_t * det = pl_cleanup_node(cx, CLEANUP_DET, bank, γnext, te ? te : tn);
    IR_t * fe = pl_det_fail_road(cx, pl_det_prop_throw(rel, dbg, "fail"), out);
    IR_t * ce = NULL;
    IR_t * cn = goal(cx, pl_cc_fnc1("call", (tree_t *) G), det, fe, &ce);
    cx->meta_redo_set = 0;
    lc_γ_to(bank, ce ? ce : cn);
    if (entry_out) *entry_out = bank;
    return out;
}
IR_t * pl_lower_det_goal(lcx_t * cx, const tree_t * G, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * out = lpl_build(cx, IR_GOTO, ωfail, ωfail);
    IR_t * te = NULL;
    tree_t * culprit = pl_det_unqual_goal(G);
    IR_t * tn = goal(cx, pl_cc_det_throw(culprit, "nondet", "goal", pl_cc_key_pi(cx->cur_key)), out, out, &te);
    IR_t * bank = pl_cleanup_node(cx, CLEANUP_BANK, (IR_t *) 0, (IR_t *) 0, ωfail);
    IR_t * det = pl_cleanup_node(cx, CLEANUP_DET, bank, γnext, te ? te : tn);
    IR_t * fe = pl_det_fail_road(cx, pl_cc_det_throw(culprit, "fail", "goal", pl_cc_key_pi(cx->cur_key)), out);
    IR_t * ce = NULL;
    IR_t * cn = goal(cx, pl_cc_fnc1("call", (tree_t *) G), det, fe, &ce);
    cx->meta_redo_set = 0;
    lc_γ_to(bank, ce ? ce : cn);
    if (entry_out) *entry_out = bank;
    return out;
}
static int pl_tree_is_inf(const tree_t * t) { return t && (t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval && (!strcmp(t->v.sval, "inf") || !strcmp(t->v.sval, "infinite")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_between_split(const tree_t * t) {
    tree_t * lo = (tree_t *) t->c[0];
    tree_t * hi = (tree_t *) t->c[1];
    tree_t * x = (tree_t *) t->c[2];
    tree_t * gen;
    if (pl_tree_is_inf(hi)) gen = pl_cc_fnc3("$between_gen", lo, pl_cc_ilit(9223372036854775807LL), x);
    else if (hi && hi->t == TT_VAR) gen = pl_cc_ite(pl_cc_fnc2("$between_special", lo, hi), pl_cc_fnc3("$between_big", lo, hi, x), pl_cc_fnc3("$between_gen", lo, hi, x));
    else gen = pl_cc_fnc3("$between_gen", lo, hi, x);
    if (x && x->t == TT_ILIT) return pl_cc_fnc3("$between_test", lo, hi, x);
    if (x && x->t != TT_VAR) return gen;
    return pl_cc_ite(pl_cc_fnc1("nonvar", x), pl_cc_fnc3("$between_test", lo, hi, x), gen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_scc_why(const tree_t * K, const tree_t * C, tree_t * why) { return K ? pl_cc_fnc2(",", pl_cc_fnc2("=", (tree_t *) K, why), (tree_t *) C) : C; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_lower_scc(lcx_t * cx, const tree_t * S, const tree_t * G, const tree_t * K, const tree_t * C, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    const char * snm = K ? "setup_call_catcher_cleanup" : "setup_call_cleanup";
    int sar = K ? 4 : 3;
    if (pl_tree_number(C)) return goal(cx, pl_cc_type_error("callable", C, snm, sar), γnext, ωfail, entry_out);
    tree_t * vchk = (C && C->t == TT_VAR) ? pl_cc_ite(pl_cc_fnc1("var", (tree_t *) C), pl_cc_throw_ar((tree_t *) pl_atom_goal("instantiation_error"), snm, sar), (tree_t *) pl_atom_goal("true")) :
        (tree_t *) 0;
    if (!pl_scc_armed()) {
        tree_t * ball = pl_meta_var("$SccBall");
        const tree_t * cx_exc = pl_scc_why(K, C, pl_cc_fnc1("exception", pl_meta_var("$SccBall")));
        tree_t * quiet = pl_cc_fnc2(",", pl_cc_fnc3("catch", pl_cc_fnc1("once", (tree_t *) cx_exc), pl_meta_var("$SccIgn"), (tree_t *) pl_atom_goal("true")), pl_cc_fnc1("throw", ball));
        tree_t * ran =
            pl_cc_ite(pl_cc_fnc3("catch", (tree_t *) G, ball, quiet),
            pl_cc_ite(pl_cc_fnc1("once", (tree_t *) pl_scc_why(K, C, (tree_t *) pl_atom_goal("exit"))), (tree_t *) pl_atom_goal("true"), (tree_t *) pl_atom_goal("true")),
            pl_cc_fnc2(",", pl_cc_fnc1("once", (tree_t *) pl_scc_why(K, C, (tree_t *) pl_atom_goal("fail"))), (tree_t *) pl_atom_goal("fail")));
        if (vchk) ran = pl_cc_fnc2(",", vchk, ran);
        return goal(cx, pl_cc_fnc2(",", pl_cc_fnc1("once", (tree_t *) S), ran), γnext, ωfail, entry_out);
    }
    IR_t * resume = pl_cleanup_node(cx, CLEANUP_RESUME, (IR_t *) 0, (IR_t *) 0, ωfail);
    IR_t * mark = lpl_build(cx, IR_BOUND, NULL, resume);
    IR_t * open = pl_cleanup_node(cx, CLEANUP_OPEN, (IR_t *) 0, (IR_t *) 0, resume);
    ir_operand_push(resume, open);
    IR_t * drain = pl_cleanup_node(cx, CLEANUP_DRAIN, open, (IR_t *) 0, resume);
    IR_t * exh = lpl_build(cx, IR_UNMARK, NULL, resume);
    ir_operand_push(exh, mark);
    IR_LIT(exh).ival = 4;
    lc_γ_to(drain, exh);
    IR_t * pop_b = pl_cleanup_node(cx, CLEANUP_POP, open, (IR_t *) 0, resume);
    IR_t * pop_f = pl_cleanup_node(cx, CLEANUP_POP, open, (IR_t *) 0, resume);
    tree_t * bv = pl_cc_freshvar();
    IR_t * he = NULL;
    IR_t * hc = pl_leaf_lv(cx, "$catch_handle", pl_cc_fnc1("$catch_handle", bv), 1, NULL, pop_f, &he);
    lc_γ_to(exh, he ? he : hc);
    lc_γ_to(hc, pop_b);
    {
        tree_t * bv2 = pl_cc_freshvar();
        tree_t * bv3 = pl_cc_freshvar();
        bv2->v.ival = bv->v.ival;
        bv3->v.ival = bv->v.ival;
        const tree_t * ce = pl_scc_why(K, C, pl_cc_fnc1("exception", bv3));
        const tree_t * gl[2] = { pl_cc_fnc3("catch", pl_cc_fnc1("once", (tree_t *) ce), pl_cc_freshvar(), (tree_t *) pl_atom_goal("true")), pl_cc_fnc1("throw", bv2) };
        IR_t * e = NULL;
        IR_t * f = pl_lower_conj(cx, gl, 2, resume, resume, &e, NULL, NULL);
        lc_γ_to(pop_b, e ? e : f);
    }
    {
        const tree_t * gl[2] = { pl_cc_fnc1("once", (tree_t *) pl_scc_why(K, C, (tree_t *) pl_atom_goal("fail"))), pl_atom_goal("fail") };
        IR_t * e = NULL;
        IR_t * f = pl_lower_conj(cx, gl, 2, resume, resume, &e, NULL, NULL);
        lc_γ_to(pop_f, e ? e : f);
    }
    IR_t * commit = lpl_build(cx, IR_UNMARK, NULL, resume);
    ir_operand_push(commit, mark);
    IR_LIT(commit).ival = 2;
    IR_t * pop_d = pl_cleanup_node(cx, CLEANUP_POP, open, (IR_t *) 0, resume);
    lc_γ_to(commit, pop_d);
    {
        IR_t * ie = NULL;
        IR_t * ig = pl_lower_ite(cx, pl_scc_why(K, C, (tree_t *) pl_atom_goal("exit")), pl_atom_goal("true"), pl_atom_goal("true"), γnext, resume, &ie);
        lc_γ_to(pop_d, ie ? ie : ig);
    }
    IR_t * test = pl_cleanup_node(cx, CLEANUP_TEST, open, γnext, commit);
    IR_t * ge = NULL;
    IR_t * gc = goal(cx, G, test, drain, &ge);
    IR_t * te = NULL;
    IR_t * tv = term_e(cx, pl_scc_why(K, C, (tree_t *) pl_atom_goal("!")), &te);
    lc_ω_to(tv, resume);
    lc_γ_to(mark, te ? te : tv);
    lc_γ_to(tv, open);
    ir_operand_push(open, drain);
    ir_operand_push(open, tv);
    lc_γ_to(open, ge ? ge : gc);
    ir_operand_push(resume, gc);
    ir_operand_push(resume, drain);
    ir_operand_push(resume, mark);
    {
        const tree_t * pre[2] = { pl_cc_fnc1("once", (tree_t *) S), vchk };
        IR_t * pe = NULL;
        IR_t * pf = pl_lower_conj(cx, pre, vchk ? 2 : 1, mark, ωfail, &pe, NULL, NULL);
        if (entry_out) *entry_out = pe ? pe : pf;
    }
    return resume;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_leaf_lv(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_lval_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first ? first : nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dyn_index(const char * name, int arity) {
    if (!name) return -1;
    for (int i = 0; i < g_stage2_pl.pl_dyn_n; i++) if (g_stage2_pl.pl_dyn_name[i] && !strcmp(g_stage2_pl.pl_dyn_name[i], name) && g_stage2_pl.pl_dyn_arity[i] == arity) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_rt_is_dynamic(const char * nm, int ar) {
    extern int rt_pl_db_key_is_dynamic_na(void *, const char *, int);
    extern int rt_pl_db_key_cell_na(void *, const char *, int);
    return g_stage2.rt_lowering && g_stage2.rt_root && (rt_pl_db_key_cell_na(g_stage2.rt_root, nm, ar) >= 0 || rt_pl_db_key_is_dynamic_na(g_stage2.rt_root, nm, ar));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dyn_index_or_add(const char * name, int arity) {
    if (g_stage2.rt_lowering) {
        extern int rt_pl_db_cell_for(void *, const char *, int);
        int rk;
        if (!g_stage2.rt_root)
            pl_refuse("a clause compiled at run time names a root cell but its compile carries no root -- the registry road needs the standing frame (ARCH-PROLOG-C-OUT-OF-THE-BOX 5) --", name, 10);
        rk = rt_pl_db_cell_for(g_stage2.rt_root, name, arity);
        if (rk < 0) pl_refuse("a clause compiled at run time names a root cell but the root's registry could not grow its overflow vector --", name, 10);
        return rk;
    }
    { int k = pl_dyn_index(name, arity); if (k >= 0) return k; pl_dyn_mark(ct_strdup(name), arity); return pl_dyn_index(name, arity); }
}
static int pl_object_defines(const char * nm, int ar) {
    extern int rt_proc_is_registered(const char *);
    char key[fmt_len("%s/%d", nm, ar)];
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    return g_stage2.harness_warm == 2 && rt_proc_is_registered(key);
}
int pl_user_defines(const char * nm, int ar) { return pl_file_defines(nm, ar) || pl_rt_registered_static(nm, ar) || pl_object_defines(nm, ar); }
tree_t * pl_between_own(tree_t * t) { return pl_user_defines("between", 3) ? pl_between_split(t) : t; }
IR_t * pl_db_seed_file(lcx_t * cx, const char * pn, int ar, IR_t * next, IR_t * ωfail, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!pl_file_defines(pn, ar)) return next;
    {
        char key[fmt_len("%s/%d", pn, ar)];
        snprintf(key, sizeof key, "%s/%d", pn, ar);
        const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
        int k = pl_dyn_index_or_add(pn, ar);
        if (k < 0 || !ch) pl_refuse("a dynamic predicate with file clauses needs a root cell and the 64 compile-time root cells are exhausted --", pn, 10);
        {
            IR_t * first = NULL;
            int n = (ch->t == TT_CHOICE) ? ch->n : 1;
            for (int i = n - 1; i >= 0; i--) {
                const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[i] : ch;
                IR_t * se = NULL;
                pl_db_leaf_seed(cx, k, i, pl_static_clause_term(cl, pn, ar, 0), pl_seed_file_term(pl_pred_file(pn, ar)), pl_seed_line_term(cl), next, ωfail, &se);
                next = se;
                first = se;
            }
            if (entry_out) *entry_out = first;
            return next;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_head_key(const tree_t * a, int * ar_out) {
    const tree_t * h = a;
    if (!h) return NULL;
    if (h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":-") && h->n == 2) h = h->c[0];
    if (!h) return NULL;
    if (h->t == TT_FNC && h->v.sval) { if (ar_out) *ar_out = h->n; return h->v.sval; }
    if ((h->t == TT_QLIT || h->t == TT_NAME) && h->v.sval) { if (ar_out) *ar_out = 0; return h->v.sval; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_db_mq_term(const char * mod, tree_t * t) {
    while (t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ":") && t->n == 2 && t->c[0] && (t->c[0]->t == TT_QLIT || t->c[0]->t == TT_NAME) && t->c[0]->v.sval && t->c[1]) {
        mod = strcmp(t->c[0]->v.sval, "user") ? t->c[0]->v.sval : (const char *) 0;
        t = t->c[1];
    }
    if (!t) return NULL;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ":") && t->n == 2) return NULL;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ":-") && t->n == 2) {
        tree_t * h = pl_db_mq_term(mod, t->c[0]);
        tree_t * r;
        if (!h) return NULL;
        r = ast_node_new(TT_FNC);
        r->v = t->v;
        r->line = t->line;
        ast_push(r, h);
        ast_push(r, t->c[1]);
        return r;
    }
    if (!mod) return t;
    if (t->v.sval && (t->t == TT_FNC || t->t == TT_QLIT || t->t == TT_NAME)) { size_t ml = strlen(mod); if (!strncmp(t->v.sval, mod, ml) && t->v.sval[ml] == ':') return t; }
    if (t->t == TT_FNC && t->v.sval) {
        char qn[fmt_len("%s:%s", mod, t->v.sval)];
        tree_t * r = ast_node_new(TT_FNC);
        snprintf(qn, sizeof qn, "%s:%s", mod, t->v.sval);
        r->v = t->v;
        r->v.sval = ct_strdup(qn);
        r->line = t->line;
        for (int i = 0; i < t->n; i++) ast_push(r, t->c[i]);
        return r;
    }
    if ((t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval) {
        char qn[fmt_len("%s:%s", mod, t->v.sval)];
        tree_t * r = ast_node_new(t->t);
        snprintf(qn, sizeof qn, "%s:%s", mod, t->v.sval);
        r->v = t->v;
        r->v.sval = ct_strdup(qn);
        r->line = t->line;
        return r;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_clause_unqual_head(tree_t * c) {
    tree_t * h = (c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, ":-") && c->n == 2) ? c->c[0] : NULL;
    if (h && h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2 && h->c[0] && h->c[0]->t != TT_VAR && h->c[1]) c->c[0] = h->c[1];
    return c;
}
tree_t * pl_meta_builtin_tree(const char * pn, int ar) {
    extern int rt_pl_meta_builtin(const char *, int, int, const char * const **);
    const char * const * toks;
    tree_t * tm;
    if (!pn || ar <= 0 || !rt_pl_meta_builtin(pn, ar, (g_stage2_pl.pl_dialect == PL_DIALECT_GNU), &toks)) return (tree_t *) 0;
    tm = ast_node_new(TT_FNC);
    tm->v.sval = (char *) pn;
    for (int i = 0; i < ar; i++) ast_push(tm, (toks[i][0] >= '0' && toks[i][0] <= '9' && !toks[i][1]) ? pl_cc_ilit(toks[i][0] - '0') : (tree_t *) pl_atom_goal(toks[i]));
    return tm;
}
tree_t * pl_cc_throw_ar(tree_t * formal, const char * nm, int ar) { return pl_cc_fnc1("throw", pl_cc_fnc2("error", formal, pl_cc_pi_ar(nm, ar))); }
tree_t * pl_cc_type_error(const char * type, const tree_t * culprit, const char * nm, int ar) {
    return pl_cc_throw_ar(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal(type), (tree_t *) culprit), nm, ar);
}
tree_t * pl_cc_perm_access(const char * pn, int ar) {
    return pl_cc_fnc1("throw",
        pl_cc_fnc2("error", pl_cc_fnc3("permission_error", (tree_t *) pl_atom_goal("access"), (tree_t *) pl_atom_goal("private_procedure"), pl_cc_pi_ar(pn, ar)), pl_cc_pi_ar("clause", 2)));
}
tree_t * pl_cc_spec_ill_typed(const tree_t * s, const char * nm) {
    if (!s || s->t == TT_VAR) return NULL;
    if (!(s->t == TT_FNC && s->v.sval && !strcmp(s->v.sval, "/") && s->n == 2)) return pl_cc_type_error("predicate_indicator", s, nm, 1);
    {
        const tree_t * n = s->c[0];
        const tree_t * a = s->c[1];
        if (n && n->t != TT_VAR && !(n->t == TT_QLIT || n->t == TT_NAME)) return strcmp(nm, "abolish") ? pl_cc_type_error("predicate_indicator", s, nm, 1) : pl_cc_type_error("atom", n, nm, 1);
        if (a && a->t != TT_VAR && a->t != TT_ILIT) return strcmp(nm, "abolish") ? pl_cc_type_error("predicate_indicator", s, nm, 1) : pl_cc_type_error("integer", a, nm, 1);
        if (a && a->t == TT_ILIT && a->v.ival < 0 && !strcmp(nm, "abolish")) return pl_cc_throw_ar(pl_cc_fnc2("domain_error", (tree_t *) pl_atom_goal("not_less_than_zero"), (tree_t *) a), nm, 1);
        if (a && a->t == TT_ILIT && a->v.ival > PROLOG_MAX_ARITY && !strcmp(nm, "abolish")) return pl_cc_throw_ar(pl_cc_fnc1("representation_error", (tree_t *) pl_atom_goal("max_arity")), nm, 1);
        return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_clause_target(const tree_t * c, const tree_t * body) {
    if (!body && c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, ":-") && c->n == 2) return (tree_t *) c;
    {
        tree_t * tg = ast_node_new(TT_FNC);
        tg->v.sval = (char *) ":-";
        ast_push(tg, (tree_t *) c);
        if (body) ast_push(tg, (tree_t *) body);
        else { tree_t * tr = ast_node_new(TT_QLIT); tr->v.sval = (char *) "true"; ast_push(tg, tr); }
        return tg;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_tree_copy(const tree_t * t) {
    if (!t) return NULL;
    { tree_t * c = ast_node_new(t->t); c->v = t->v; c->line = t->line; c->slen = t->slen; for (int i = 0; i < t->n; i++) ast_push(c, pl_tree_copy(t->c[i])); return c; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_tree_var_occ(const tree_t * t) { int c = 0; if (!t) return 0; if (t->t == TT_VAR) return 1; for (int i = 0; i < t->n; i++) c += pl_tree_var_occ(t->c[i]); return c; }
void pl_tree_renumber_vars(tree_t * t, long long * slots, int * n, int cap, int base) {
    if (!t) return;
    if (t->t == TT_VAR) {
        int slot = -1;
        long long key = t->v.ival;
        for (int i = 0; i < *n; i++) if (slots[i] == key) { slot = i; break; }
        if (slot < 0 && *n < cap) { slots[*n] = key; slot = *n; (*n)++; }
        t->v.ival = base + (slot < 0 ? cap - 1 : slot);
        return;
    }
    for (int i = 0; i < t->n; i++) pl_tree_renumber_vars(t->c[i], slots, n, cap, base);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_body_term(const tree_t * b) {
    if (!b) return (tree_t *) pl_atom_goal("true");
    if (b->t == TT_VAR) { tree_t * c = ast_node_new(TT_FNC); c->v.sval = (char *) "call"; ast_push(c, pl_tree_copy(b)); return c; }
    if (b->t == TT_IF) {
        tree_t * ar = ast_node_new(TT_FNC);
        ar->v.sval = (char *) "->";
        ast_push(ar, pl_body_term(b->n > 0 ? b->c[0] : NULL));
        ast_push(ar, pl_body_term(b->n > 1 ? b->c[1] : NULL));
        if (b->v.ival) return ar;
        { tree_t * d = ast_node_new(TT_FNC); d->v.sval = (char *) ";"; ast_push(d, ar); ast_push(d, (b->n > 2) ? pl_body_term(b->c[2]) : (tree_t *) pl_atom_goal("fail")); return d; }
    }
    if (b->t == TT_PROGRAM) {
        tree_t * acc = NULL;
        for (int i = b->n - 1; i >= 0; i--) {
            tree_t * g = pl_body_term(b->c[i]);
            if (!acc) acc = g;
            else { tree_t * c = ast_node_new(TT_FNC); c->v.sval = (char *) ","; ast_push(c, g); ast_push(c, acc); acc = c; }
        }
        return acc ? acc : (tree_t *) pl_atom_goal("true");
    }
    if (b->t == TT_FNC && b->v.sval && b->n >= 2 && (!strcmp(b->v.sval, ",") || !strcmp(b->v.sval, ";") || !strcmp(b->v.sval, "->"))) {
        tree_t * c = ast_node_new(TT_FNC);
        c->v.sval = b->v.sval;
        for (int i = 0; i < b->n; i++) ast_push(c, pl_body_term(b->c[i]));
        return c;
    }
    return pl_tree_copy(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_var_uses(const tree_t * t, long long k) { int c = 0; if (!t) return 0; if (t->t == TT_VAR) return t->v.ival == k; for (int i = 0; i < t->n; i++) c += pl_var_uses(t->c[i], k); return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_goal_uses(const tree_t * g, long long k) {
    if (g->t == TT_FNC && g->n == 2 && g->v.sval && !strcmp(g->v.sval, "=") && (g->c[0]->t == TT_VAR) != (g->c[1]->t == TT_VAR)) return pl_var_uses(g->c[0]->t == TT_VAR ? g->c[1] : g->c[0], k);
    return pl_var_uses(g, k);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_true_goal(const tree_t * g) { return g && (g->t == TT_QLIT || g->t == TT_NAME) && g->v.sval && !strcmp(g->v.sval, "true"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_moved_unify(tree_t ** ha, int ar, tree_t ** gl, int ng) {
    int gi[ng > 0 ? ng : 1], ai[ng > 0 ? ng : 1], na = 0, nn = 0;
    char mv[ng > 0 ? ng : 1], qk[ng > 0 ? ng : 1], drop[ng > 0 ? ng : 1];
    tree_t * tv[ng > 0 ? ng : 1];
    tree_t * out[ng > 0 ? ng : 1];
    memset(drop, 0, sizeof drop);
    for (int i = 0; i < ng; i++) {
        const tree_t * g = gl[i];
        if (pl_true_goal(g)) continue;
        if (!(g->t == TT_FNC && g->n == 2 && g->v.sval && !strcmp(g->v.sval, "="))) break;
        for (int s = 0; s < 2; s++) {
            const tree_t * v = g->c[s], * o = g->c[1 - s];
            int a = -1;
            if (v->t != TT_VAR || o->t == TT_VAR) continue;
            for (int j = 0; j < ar && a < 0; j++) if (ha[j]->t == TT_VAR && ha[j]->v.ival == v->v.ival) a = j;
            for (int q = 0; q < na && a >= 0; q++) if (ai[q] == a) a = -1;
            if (a < 0) continue;
            gi[na] = i;
            ai[na] = a;
            tv[na] = g->c[1 - s];
            na++;
            break;
        }
    }
    if (!na) return ng;
    for (int q = 0; q < na; q++) {
        long long k = ha[ai[q]]->v.ival;
        int uses = 0;
        for (int j = 0; j < ar; j++) if (j != ai[q]) uses += pl_var_uses(ha[j], k);
        for (int i = 0; i < ng; i++) uses += pl_goal_uses(gl[i], k);
        mv[q] = uses == 0;
        drop[gi[q]] = 1;
    }
    for (int q = 0; q < na; q++) { long long k = ha[ai[q]]->v.ival; qk[q] = 0; for (int r = 0; r < na; r++) if (r != q && pl_var_uses(tv[r], k)) qk[q] = 1; }
    for (int q = 0; q < na; q++) if (mv[q]) ha[ai[q]] = tv[q];
    for (int a = 0; a < ar; a++) for (int q = 0; q < na; q++) if (ai[q] == a && !mv[q]) out[nn++] = pl_cc_fnc2("=", pl_tree_copy(ha[a]), qk[q] ? pl_tree_copy(ha[a]) : tv[q]);
    for (int i = 0; i < ng; i++) if (!drop[i]) out[nn++] = gl[i];
    for (int i = 0; i < nn; i++) gl[i] = out[i];
    return nn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_static_clause_term(const tree_t * cl, const char * pn, int ar, int moved) {
    tree_t * h;
    tree_t * bt = NULL;
    int nc = (cl && cl->t == TT_CLAUSE) ? cl->n : 0;
    int ng = nc > ar ? nc - ar : 0;
    tree_t * ha[ar > 0 ? ar : 1];
    tree_t * gl[ng > 0 ? ng : 1];
    for (int i = 0; i < ar; i++) ha[i] = (i < nc) ? pl_tree_copy(cl->c[i]) : pl_meta_var("_");
    for (int i = 0; i < ng; i++) gl[i] = pl_body_term(cl->c[ar + i]);
    if (moved && ar > 0 && nc >= ar && g_stage2_pl.pl_dialect == PL_DIALECT_SWI && !pl_ssu_clause(cl, ar)) ng = pl_moved_unify(ha, ar, gl, ng);
    if (ar > 0) { h = ast_node_new(TT_FNC); h->v.sval = ct_strdup(pn); for (int i = 0; i < ar; i++) ast_push(h, ha[i]); } else { h = ast_node_new(TT_QLIT); h->v.sval = ct_strdup(pn); }
    for (int i = ng - 1; i >= 0; i--) { tree_t * g = gl[i]; if (!bt) bt = g; else { tree_t * c = ast_node_new(TT_FNC); c->v.sval = (char *) ","; ast_push(c, g); ast_push(c, bt); bt = c; } }
    if (!bt) { bt = ast_node_new(TT_QLIT); bt->v.sval = (char *) "true"; }
    {
        tree_t * tg = pl_clause_target(h, bt);
        int occ = pl_tree_var_occ(tg) + 1;
        long long slots[occ];
        int n = 0;
        pl_tree_renumber_vars(tg, slots, &n, occ, g_lower_pl.seed_var_base);
        g_lower_pl.seed_var_base += (n > 0 ? n : 1);
        return tg;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_file_key_path(const char *pf, char *rp) {
    if (g_stage2_pl.pl_source_path && !strcmp(pf, g_stage2_pl.pl_source_path)) { snprintf(rp, PATH_MAX, "%s", pf); return 1; }
    return realpath(pf, rp) != NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_seed_file_term(const char * pf) { char rp[PATH_MAX]; if (pf && pl_file_key_path(pf, rp)) return (tree_t *) pl_atom_goal(lp_strdup(rp)); return pl_cc_ilit(-1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_seed_line_term(const tree_t * cl) { return pl_cc_ilit(cl && cl->line > 0 ? cl->line : 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_db_leaf_seed(lcx_t * cx, int k, int i, const tree_t * arg, const tree_t * fl, const tree_t * ln, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = "$db_seed_once";
    IR_t * kn = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(kn).ival = k;
    IR_t * in = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(in).ival = i;
    IR_t * te = NULL, * fe = NULL, * le = NULL;
    IR_t * tv = term_e(cx, arg, &te);
    IR_t * fv = term_e(cx, fl, &fe);
    IR_t * lv = term_e(cx, ln, &le);
    lc_γ_to(kn, in);
    lc_ω_to(kn, ωfail);
    lc_γ_to(in, te ? te : tv);
    lc_ω_to(in, ωfail);
    lc_γ_to(tv, fe ? fe : fv);
    lc_ω_to(tv, ωfail);
    lc_γ_to(fv, le ? le : lv);
    lc_ω_to(fv, ωfail);
    lc_γ_to(lv, nd);
    lc_ω_to(lv, ωfail);
    ir_operand_push(nd, kn);
    ir_operand_push(nd, in);
    ir_operand_push(nd, tv);
    ir_operand_push(nd, fv);
    ir_operand_push(nd, lv);
    if (entry_out) *entry_out = kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_dl_leaf_tree(tree_t * v) {
    tree_t * is_fail = pl_cc_fnc2(";", pl_cc_fnc2("==", v, (tree_t *) pl_atom_goal("fail")), pl_cc_fnc2("==", v, (tree_t *) pl_atom_goal("false")));
    return pl_cc_ite(pl_cc_fnc2("==", v, (tree_t *) pl_atom_goal("true")), (tree_t *) pl_atom_goal("true"), pl_cc_ite(is_fail, (tree_t *) pl_atom_goal("fail"), pl_cc_fnc1("$dl_dyn", v)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_curout_writer(const char * nm, int n) {
    static const struct {
        const char * n;
        int a;
    } w[] = { { "write", 1 }, { "writeq", 1 }, { "print", 1 }, { "write_canonical", 1 }, { "write_term", 2 }, { "writeln", 1 }, { "format", 1 }, { "format", 2 }, { NULL, 0 } };
    for (int i = 0; w[i].n; i++) if (w[i].a == n && !strcmp(w[i].n, nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_curout_text_guard(lcx_t * cx, IR_t * nd, IR_t * ne, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * ge = NULL;
    IR_t * g = goal(cx, pl_cc_fnc2("$pl_ioarg", (tree_t *) pl_atom_goal("put_text1"), (tree_t *) pl_atom_goal("[]")), ne ? ne : nd, ωfail, &ge);
    if (entry_out) *entry_out = ge ? ge : g;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_pp_runtime(const tree_t * h, const tree_t * pp) {
    tree_t * cv = pl_cc_freshvar();
    tree_t * cv2 = pl_cc_freshvar();
    tree_t * iv = pl_cc_freshvar();
    tree_t * iv2 = pl_cc_freshvar();
    cv2->v.ival = cv->v.ival;
    iv2->v.ival = iv->v.ival;
    tree_t * pc = pl_cc_fnc1("$pl_pp_pcheck", (tree_t *) pp);
    return pl_cc_fnc2(",", pl_cc_fnc1("$pl_pp_enum", (tree_t *) h),
        pl_cc_fnc2(",", pl_cc_fnc1("$pl_pp_guard", (tree_t *) h),
        pl_cc_fnc2(",", pc,
        pl_cc_fnc2(",", pl_cc_fnc2("$pl_pp_count", (tree_t *) h, cv),
        pl_cc_fnc2(",", pl_between_own(pl_cc_fnc3("between", pl_cc_ilit(1), cv2, iv)), pl_cc_fnc3("$pl_pp_nth", iv2, (tree_t *) h, (tree_t *) pp))))));
}
static void * pl_runtime_define_pred_g(const char * key, const tree_t * choice, int arity, IR_graph_t ** gout, void * rt_root) {
    return pl_runtime_define_pred_x(key, choice, arity, gout, -1, -1, (int *)0, rt_root);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_mc_wrapper_term(const char * fn, int ar) {
    if (ar <= 0) { tree_t * a = ast_node_new(TT_QLIT); a->v.sval = ct_strdup(fn); return a; }
    {
        tree_t * t = ast_node_new(TT_FNC);
        t->v.sval = ct_strdup(fn);
        for (int i = 0; i < ar; i++) { char b[24]; tree_t * v = ast_node_new(TT_VAR); snprintf(b, sizeof b, "_A%d", i); v->v.sval = ct_strdup(b); ast_push(t, v); }
        return t;
    }
}
static void * pl_mc_define(const char * wkey, const char * wname, int ar, tree_t * body, IR_graph_t ** gout, void * rt_root) {
    extern tree_t * pl_runtime_clause_tree(tree_t *);
    tree_t * pair = pl_cc_fnc2(":-", pl_mc_wrapper_term(wname, ar), body);
    tree_t * cl = pl_runtime_clause_tree(pair);
    tree_t * ch;
    if (!cl) return (void *)0;
    ch = ast_node_new(TT_CHOICE);
    ch->v.sval = ct_strdup(wkey);
    ast_push(ch, cl);
    return pl_runtime_define_pred_g(wkey, ch, ar, gout, rt_root);
}
void * pl_runtime_define_goal_wrapper(const char * wkey, const char * nm, int ar, void * rt_root) {
    extern int rt_pl_unknown_suppress(const char *);
    IR_graph_t * g = NULL;
    void * fn;
    if (!wkey || !nm || ar < 0) return (void *)0;
    nm = ct_strdup(nm);
    char wname[fmt_len("$mc:%s", nm)];
    snprintf(wname, sizeof wname, "$mc:%s", nm);
    if (!strcmp(nm, ":") && ar == 2 && rt_proc_is_registered("$mod_call/2")) return pl_mc_define(wkey, wname, ar, pl_mc_wrapper_term("$mod_call", 2), NULL, rt_root);
    fn = pl_mc_define(wkey, wname, ar, pl_mc_wrapper_term(nm, ar), &g, rt_root);
    if (!fn) return (void *)0;
    if (!pl_mc_graph_fell_back(g, nm, ar)) return fn;
    {
        char key[fmt_len("%s/%d", nm, ar)];
        tree_t * body;
        snprintf(key, sizeof key, "%s/%d", nm, ar);
        if (rt_pl_unknown_suppress(key)) body = (tree_t *) pl_atom_goal("fail");
        else body =
            pl_cc_fnc2(";", pl_cc_fnc2("->", pl_cc_fnc2("$pl_declared", (tree_t *) pl_atom_goal(nm), pl_cc_ilit(ar)), (tree_t *) pl_atom_goal("fail")),
            pl_cc_fnc1("throw", pl_cc_fnc2("error", pl_cc_fnc2("existence_error", (tree_t *) pl_atom_goal("procedure"), pl_cc_pi_ar(nm, ar)), pl_cc_pi_ar(nm, ar))));
        return pl_mc_define(wkey, wname, ar, body, NULL, rt_root);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char * pl_pi_text(const char * nm, int ar, int tail) {
    int n = snprintf(NULL, 0, tail ? "%s/%d" : "%s%d", nm, ar);
    char * r = (char *) ct_alloc((size_t) n + 1);
    snprintf(r, (size_t) n + 1, tail ? "%s/%d" : "%s%d", nm, ar);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_clause_term_key(const tree_t * h, cv_t * keys, int * all) {
    char * key;
    int ar = 0;
    const char * pn;
    while (h && h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2) h = h->c[1];
    if (!h) return;
    pn = pl_head_key(h, &ar);
    if (!pn) { if (!pl_tree_number(h)) *all = 1; return; }
    key = pl_pi_text(pn, ar, 1);
    for (uint32_t i = 0; i < keys->len; i++) if (!strcmp(CV_AT(*keys, const char *, i), key)) { ct_drop(key); return; }
    CV_PUSH(*keys, const char *) = key;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_clause_term_walk(const tree_t * t, cv_t * keys, int * all) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "clause") && (t->n == 2 || t->n == 3) && t->c[0]) pl_clause_term_key(t->c[0], keys, all);
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "listing") && (t->n == 1 || t->n == 2)) *all = 1;
    if (t->t == TT_FNC && t->v.sval && ((!strcmp(t->v.sval, "clause_property") && t->n == 2) || (!strcmp(t->v.sval, "clause_info") && (t->n == 4 || t->n == 5)))) *all = 1;
    for (int i = 0; i < t->n; i++) pl_clause_term_walk(t->c[i], keys, all);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_seed_unmod(tree_t * t, const char * mod, size_t ml) {
    if (!t || !mod || !t->v.sval || (t->t != TT_FNC && t->t != TT_QLIT)) return t;
    if (t->t == TT_FNC && (!strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "->") || !strcmp(t->v.sval, "*->") || !strcmp(t->v.sval, "\\+"))) {
        for (int i = 0; i < t->n; i++) t->c[i] = pl_seed_unmod(t->c[i], mod, ml);
        return t;
    }
    if (!strncmp(t->v.sval, mod, ml) && t->v.sval[ml] == ':' && t->v.sval[ml + 1]) t->v.sval = t->v.sval + ml + 1;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_seed_clause_unmod(tree_t * tg, const char * mod, size_t ml) {
    if (mod && tg && tg->t == TT_FNC && tg->v.sval && !strcmp(tg->v.sval, ":-") && tg->n == 2) tg->c[1] = pl_seed_unmod(tg->c[1], mod, ml);
    return tg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_seed_forward_target(const tree_t * ch, const char * nm, int ar, const char ** mod, size_t * ml) {
    const tree_t * f = ch->t == TT_CHOICE ? (ch->n == 1 ? ch->c[0] : NULL) : ch;
    const tree_t * b = (f && f->t == TT_CLAUSE && f->n == ar + 1) ? f->c[ar] : NULL;
    const char * cn = (b && b->v.sval && (ar > 0 ? (b->t == TT_FNC && b->n == ar) : b->t == TT_QLIT)) ? strrchr(b->v.sval, ':') : NULL;
    if (!cn || cn == b->v.sval || strcmp(cn + 1, nm)) return NULL;
    for (int k = 0; k < ar; k++) if (f->c[k]->t != TT_VAR || b->c[k]->t != TT_VAR || f->c[k]->v.ival != b->c[k]->v.ival) return NULL;
    {
        char * q = pl_pi_text(b->v.sval, ar, 1);
        const tree_t * tch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, q);
        if (!tch || tch == ch) return NULL;
        *mod = b->v.sval;
        *ml = (size_t) (cn - b->v.sval);
        return tch;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_static_seed_pred(const char * key, const tree_t * ch, cv_t * procs) {
    const char * slash = strrchr(key, '/');
    char * nm;
    size_t nl;
    int ar, n;
    const char * kn;
    const char * mod = NULL;
    size_t ml = 0;
    if (!slash || key[0] == '$' || !ch || (ch->t == TT_CHOICE && ch->n < 1)) return;
    ar = atoi(slash + 1);
    nl = (size_t) (slash - key);
    nm = (char *) ct_alloc(nl + 1);
    memcpy(nm, key, nl);
    nm[nl] = 0;
    if (pl_dyn_index(nm, ar) >= 0 || pl_decl_dyn_is(nm, ar)) { ct_drop(nm); return; }
    { const char * cm = strchr(nm, ':'); const tree_t * tch = pl_seed_forward_target(ch, nm, ar, &mod, &ml); if (tch) ch = tch; else if (cm && cm != nm) { mod = nm; ml = (size_t) (cm - nm); } }
    n = ch->t == TT_CHOICE ? ch->n : 1;
    kn = ct_strdup(key);
    for (int lo = 0; lo < n && procs->len < PL_SEED_PROCS_MAX; lo += PL_SEED_CHUNK) {
        int hi = lo + PL_SEED_CHUNK < n ? lo + PL_SEED_CHUNK : n, m = 0, save_base = g_lower_pl.seed_var_base;
        const tree_t ** sg = (const tree_t **) ct_zalloc((size_t) (hi - lo), sizeof(const tree_t *));
        g_lower_pl.seed_var_base = 0;
        for (int i = lo; i < hi; i++) {
            const tree_t * sc = ch->t == TT_CHOICE ? ch->c[i] : ch;
            sg[m++] =
                pl_cc_fnc5("$db_seed_once", (tree_t *) pl_atom_goal(kn), pl_cc_ilit(i), pl_seed_clause_unmod(pl_static_clause_term(sc, nm, ar, 1), mod, ml), pl_seed_file_term(pl_pred_file(nm, ar)),
                pl_seed_line_term(sc));
        }
        g_lower_pl.seed_var_base = save_base;
        {
            IR_graph_t * g = pl_body_graph(sg, m);
            int bb_idx = bb_program_add(&g_stage2.bbp, g);
            if (bb_idx >= 0) {
                char * pn = pl_pi_text("$db_seedS", (int) procs->len, 0);
                char * rk = pl_pi_text(pn, 0, 1);
                pl_bb_register(rk, 0, bb_idx);
                pl_new_proc(rk, 0, bb_idx);
                CV_PUSH(*procs, const char *) = pn;
            }
        }
        ct_drop(sg);
    }
    ct_drop(nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_attv_w(void) { return pl_cc_ite((tree_t *) pl_atom_goal("$wake_pending"), (tree_t *) pl_atom_goal("$wakeup"), (tree_t *) pl_atom_goal("true")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ssu_clause(const tree_t * cl, int ar) {
    const tree_t * nk = (cl && cl->t == TT_CLAUSE && cl->n == ar + 1) ? cl->c[ar] : (const tree_t *) 0;
    return nk && nk->t == TT_FNC && nk->n == 2 && nk->v.sval && !strcmp(nk->v.sval, "=>") && nk->c[0] && nk->c[0]->t == TT_PROGRAM && nk->c[1] && nk->c[1]->t == TT_PROGRAM;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_ssu_var(int i) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = i; return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_ssu_lower_clause(const tree_t * cl, int ar) {
    const tree_t * nk = cl->c[ar];
    tree_t * nc = ast_node_new(TT_CLAUSE);
    nc->v = cl->v;
    nc->line = cl->line;
    for (int i = 0; i < ar; i++) ast_push(nc, pl_ssu_var(i));
    if (ar > 0) {
        tree_t * sv = pl_cc_freshvar();
        tree_t * g = pl_cc_fnc1("$ssu_check", sv);
        for (int i = ar - 1; i >= 0; i--) g = pl_cc_fnc2(",", pl_cc_fnc2("=", pl_tree_copy(cl->c[i]), pl_ssu_var(i)), g);
        ast_push(nc, pl_cc_fnc1("$ssu_begin", pl_tree_copy(sv)));
        ast_push(nc, pl_cc_fnc2("->", g, (tree_t *) pl_atom_goal("true")));
    }
    for (int i = 0; i < nk->c[0]->n; i++) ast_push(nc, pl_tree_copy(nk->c[0]->c[i]));
    ast_push(nc, ast_node_new(TT_CUT));
    for (int i = 0; i < nk->c[1]->n; i++) ast_push(nc, pl_tree_copy(nk->c[1]->c[i]));
    return nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_ssu_nomatch(const tree_t * proto, const char * key, int ar) {
    const char * sl = strrchr(key, '/');
    char nm[sl - key + 1];
    memcpy(nm, key, (size_t)(sl - key));
    nm[sl - key] = 0;
    tree_t * nc = ast_node_new(TT_CLAUSE);
    nc->v = proto->v;
    nc->line = proto->line;
    for (int i = 0; i < ar; i++) ast_push(nc, pl_ssu_var(i));
    const char * colon = strchr(nm, ':');
    int qual = colon && colon > nm && colon[1] && colon[1] != ':';
    tree_t * g = ast_node_new(ar ? TT_FNC : TT_QLIT);
    g->v.sval = lp_strdup(qual ? colon + 1 : nm);
    for (int i = 0; i < ar; i++) ast_push(g, pl_ssu_var(i));
    tree_t * pi = pl_cc_pi_ar(lp_strdup(qual ? colon + 1 : nm), ar);
    if (qual) {
        const char * mod = ct_strndup(nm, (size_t)(colon - nm));
        g = pl_cc_fnc2(":", (tree_t *) pl_atom_goal(mod), g);
        pi = pl_cc_fnc2(":", (tree_t *) pl_atom_goal(mod), pi);
    } else {
        extern const char *pl_prelude_lib_of(const char *, int);
        const char * lib = !pl_user_defines(nm, ar) ? pl_prelude_lib_of(nm, ar) : (const char *) 0;
        if (lib) { g = pl_cc_fnc2(":", (tree_t *) pl_atom_goal(lib), g); pi = pl_cc_fnc2(":", (tree_t *) pl_atom_goal(lib), pi); }
    }
    tree_t * ex = pl_cc_fnc2("existence_error", (tree_t *) pl_atom_goal("matching_rule"), g);
    ast_push(nc, pl_cc_fnc1("throw", pl_cc_fnc2("error", ex, pl_cc_fnc2("context", pi, pl_ssu_var(ar)))));
    return nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ssu_choice(const tree_t * subj) {
    if (!subj || subj->t != TT_CHOICE || !subj->v.sval || !strrchr(subj->v.sval, '/')) return 0;
    int ar = atoi(strrchr(subj->v.sval, '/') + 1);
    for (int k = 0; k < subj->n; k++) if (pl_ssu_clause(subj->c[k], ar)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_ssu_rewrite(const tree_t * prog) {
    int any = 0;
    for (int i = 0; i < prog->n; i++) { const tree_t * s = prog->c[i]; any |= pl_ssu_choice((s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL); }
    if (!any) return prog;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (!pl_ssu_choice(subj)) { ast_push(np, (tree_t *) s); continue; }
        int ar = atoi(strrchr(subj->v.sval, '/') + 1);
        tree_t * ch = ast_node_new(TT_CHOICE);
        ch->v = subj->v;
        ch->line = subj->line;
        for (int k = 0; k < subj->n; k++) ast_push(ch, pl_ssu_clause(subj->c[k], ar) ? pl_ssu_lower_clause(subj->c[k], ar) : pl_tree_copy(subj->c[k]));
        if (subj->n && pl_ssu_clause(subj->c[subj->n - 1], ar)) ast_push(ch, pl_ssu_nomatch(subj->c[subj->n - 1], subj->v.sval, ar));
        ast_push(np, pl_table_stmt(s, ch));
    }
    return np;
}
