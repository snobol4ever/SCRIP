#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include <limits.h>
#include "ir/stage2.h"
#include "ir/stage2_pl.h"
#include "lower/lower_common.h"
#include "lower/pl_aggregate_names.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_02_pl.h"
#include "lower/lower_prolog/lpl_03_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
#include "lower/lower_prolog/lpl_06_pl.h"
#include "lower/lower_prolog/lpl_07_pl.h"
#include "lower/lower_prolog/lpl_08_pl.h"
#include "lower/lower_prolog/lpl_09_sh.h"
static const char * const pl_db_mq_builtins[] = { "assert", "asserta", "assertz", "retract", "retractall", "abolish", NULL };
static const char * const pl_pred_props[] = { "built_in", "dynamic", "static", "defined", "multifile", "discontiguous", "meta_predicate", "exported", "imported_from", "foreign", "iso",
    "number_of_clauses", "public", "private", "native", "tabled", "incremental", "volatile", "transparent", "nodebug", "visible", "template", "alias", "declared_in", "defined_in", "scope",
    "synchronized", "file", "line_count", "notrace", "thread_local", "non_terminal", "ssu", "quasi_quotation_syntax", "user", "monofile", "native_code", "built_in_fd", "control_construct",
    "prolog_file", "prolog_line", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * goal_inner(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    int tpop = cx->tail_pop;
    cx->tail_pop = 0;
    if (!t) return lpl_build(cx, IR_SUCCEED, γnext, ωfail);
    if (pl_tree_number(t)) return goal(cx, pl_cc_type_error("callable", t, "call", 1), γnext, ωfail, entry_out);
    if (t->t == TT_MAKELIST && t->n > 0) {
        tree_t * g = pl_cc_fnc1("consult", (tree_t *) t->c[0]);
        for (int i = 1; i < t->n; i++) g = pl_cc_fnc2(",", g, pl_cc_fnc1("consult", (tree_t *) t->c[i]));
        return goal(cx, g, γnext, ωfail, entry_out);
    }
    switch (t->t) {
        case TT_FNC:
        {
            const char * nm = t->v.sval ? t->v.sval : "?";
            if (!strcmp(nm, ":") && t->n == 2 && t->c[0] && (t->c[0]->t == TT_QLIT || t->c[0]->t == TT_NAME) && t->c[1] && t->c[0]->v.sval && strcmp(t->c[0]->v.sval, "user") &&
                g_stage2_pl.pl_dialect == PL_DIALECT_XSB) {
                const char * mn = t->c[0]->v.sval;
                char fp[fmt_len("%s.P", mn)], fl[fmt_len("%s.pl", mn)];
                snprintf(fp, sizeof fp, "%s.P", mn);
                snprintf(fl, sizeof fl, "%s.pl", mn);
                tree_t * beside =
                    pl_cc_fnc2(";", pl_cc_fnc3("$gnu_os", (tree_t *) pl_atom_goal("access"), (tree_t *) pl_atom_goal(ct_strdup(fp)), pl_cc_ilit(0)),
                    pl_cc_fnc3("$gnu_os", (tree_t *) pl_atom_goal("access"), (tree_t *) pl_atom_goal(ct_strdup(fl)), pl_cc_ilit(0)));
                tree_t * load = pl_cc_ite(beside, pl_cc_fnc1("ensure_loaded", (tree_t *) t->c[0]), (tree_t *) pl_atom_goal("true"));
                return goal(cx, pl_cc_fnc2(",", load, pl_cc_fnc2("$xsb_q", (tree_t *) t->c[0], (tree_t *) t->c[1])), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, ":") && t->n == 2 && t->c[0] && (t->c[0]->t == TT_QLIT || t->c[0]->t == TT_NAME) && t->c[0]->v.sval && strcmp(t->c[0]->v.sval, "user") && t->c[1] && t->c[1]->t == TT_VAR &&
                g_stage2_pl.pl_dialect != PL_DIALECT_XSB) return goal(cx, pl_cc_fnc2("$mod_call", (tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
            if ((!strcmp(nm, ":") || !strcmp(nm, "$xsb_q")) && t->n == 2 && t->c[0] && (t->c[0]->t == TT_QLIT || t->c[0]->t == TT_NAME) && t->c[1]) {
                const char * sv = cx->umod;
                IR_t * qr;
                cx->tail_pop = tpop;
                cx->umod = (t->c[0]->v.sval && strcmp(t->c[0]->v.sval, "user")) ? t->c[0]->v.sval : "";
                {
                    const tree_t * ig = t->c[1];
                    int qh = cx->umod[0] && ig->t == TT_FNC && ig->v.sval && !strcmp(ig->v.sval, "clause") && (ig->n == 2 || ig->n == 3) &&
                        !(ig->c[0] && ig->c[0]->t == TT_FNC && ig->c[0]->v.sval && !strcmp(ig->c[0]->v.sval, ":"));
                    if (qh) ig = ig->n == 2 ? pl_cc_fnc2("clause", pl_cc_fnc2(":", (tree_t *) pl_atom_goal(t->c[0]->v.sval), (tree_t *) ig->c[0]), (tree_t *) ig->c[1]) :
                        pl_cc_fnc3("clause", pl_cc_fnc2(":", (tree_t *) pl_atom_goal(t->c[0]->v.sval), (tree_t *) ig->c[0]), (tree_t *) ig->c[1], (tree_t *) ig->c[2]);
                    if (qh) cx->umod = sv;
                    if (!strcmp(nm, "$xsb_q") && g_stage2_pl.pl_dialect == PL_DIALECT_XSB && pl_xsb_sysmod(t->c[0]->v.sval) && ig->t == TT_FNC && ig->v.sval && ig->n >= 1 && !strchr(ig->v.sval, ':') &&
                        pl_user_file_defines(ig->v.sval, ig->n)) {
                        char pn[fmt_len("system:%s", ig->v.sval)];
                        tree_t * pg = pl_tree_copy(ig);
                        snprintf(pn, sizeof pn, "system:%s", ig->v.sval);
                        pg->v.sval = ct_strdup(pn);
                        ig = pl_cc_ite(pl_cc_fnc1("current_predicate", pl_cc_pi_ar(pg->v.sval, ig->n)), pg, (tree_t *) ig);
                    }
                    qr = goal(cx, ig, γnext, ωfail, entry_out);
                }
                cx->umod = sv;
                return qr;
            }
            if (!strcmp(nm, "$") && t->n == 1 && !pl_dollar_user(1)) return pl_lower_det_goal(cx, t->c[0], γnext, ωfail, entry_out);
            if (pl_swi_user_redefines(nm, t->n) && !pl_hook_dyn_is(nm, t->n)) return pl_user_call(cx, nm, t, t->n, γnext, ωfail, entry_out);
            if (!strcmp(nm, "$det_prop") && t->n == 3) return pl_lower_det_prop(cx, t->c[0], t->c[1], t->c[2], γnext, ωfail, entry_out);
            if (!strcmp(nm, "string_concat") && t->n == 3 && !pl_user_defines(nm, 3)) {
                tree_t * both = pl_cc_fnc2(",", pl_cc_fnc1("nonvar", (tree_t *) t->c[0]), pl_cc_fnc1("nonvar", (tree_t *) t->c[1]));
                tree_t * det = pl_cc_fnc3("$string_concat_det", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]);
                return goal(cx, pl_cc_ite(both, det, pl_cc_fnc3("$string_concat_split", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2])), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, ",")) {
                cx->tail_pop = tpop;
                lc_vec glv;
                lc_vec_init(&glv, (int) sizeof(const tree_t *));
                collect_conj(t, &glv);
                {
                    IR_t * cω = lpl_build(cx, IR_GOTO, ωfail, ωfail);
                    IR_t * ientry = NULL;
                    IR_t * iredo = NULL;
                    pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, γnext, cω, &ientry, &iredo, NULL);
                    cx->meta_redo = iredo;
                    cx->meta_redo_set = 1;
                    if (entry_out) *entry_out = ientry;
                    return cω;
                }
            }
            if (!strcmp(nm, "write") && t->n == 1) { IR_t * ne = NULL; IR_t * nd = pl_leaf(cx, "$write", t, 1, γnext, ωfail, &ne); return pl_curout_text_guard(cx, nd, ne, ωfail, entry_out); }
            if (!strcmp(nm, ";") || !strcmp(nm, "|")) {
                cx->tail_pop = tpop;
                if (pl_is_ite(t)) return pl_lower_ite(cx, t->c[0]->c[0], t->c[0]->c[1], t->c[1], γnext, ωfail, entry_out);
                if (pl_is_scite(t)) return pl_lower_softcut(cx, t->c[0]->c[0], t->c[0]->c[1], t->c[1], γnext, ωfail, entry_out);
                return pl_lower_disj(cx, t, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "->") && t->n == 2) { cx->tail_pop = tpop; return pl_lower_ite(cx, t->c[0], t->c[1], NULL, γnext, ωfail, entry_out); }
            if (!strcmp(nm, "*->") && t->n == 2) { cx->tail_pop = tpop; return pl_lower_softcut(cx, t->c[0], t->c[1], NULL, γnext, ωfail, entry_out); }
            if ((!strcmp(nm, "\\+") || !strcmp(nm, "not")) && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            if (!strcmp(nm, "once") && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("true"), pl_atom_goal("fail"), γnext, ωfail, entry_out);
            if (!strcmp(nm, "ignore") && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("true"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            if (!strcmp(nm, "forall") && t->n == 2) {
                tree_t * inner = ast_node_new(TT_FNC);
                inner->v.sval = (char *) "\\+";
                ast_push(inner, (tree_t *) t->c[1]);
                tree_t * conj = ast_node_new(TT_FNC);
                conj->v.sval = (char *) ",";
                ast_push(conj, (tree_t *) t->c[0]);
                ast_push(conj, inner);
                return pl_lower_ite(cx, conj, pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            }
            if ((!strcmp(nm, "call") && t->n >= 1) || (!strcmp(nm, "phrase") && (t->n == 2 || t->n == 3))) {
                const tree_t * xs[2];
                const tree_t * const * extra;
                int nextra;
                if (!strcmp(nm, "phrase")) {
                    xs[0] = t->c[1];
                    xs[1] = (t->n == 3) ? t->c[2] : pl_nil_term();
                    extra = xs;
                    nextra = 2;
                    int chk = pl_phrase_rt_checked() || g_stage2_pl.pl_apply_macros;
                    if (chk || !pl_phrase_plain_nt(t->c[0]))
                        return goal(cx, pl_cc_fnc3(chk ? "$phrase_rt" : "$phrase", (tree_t *) t->c[0], (tree_t *) xs[0], (tree_t *) xs[1]), γnext, ωfail, entry_out);
                } else {
                    extra = (const tree_t * const *) &t->c[1];
                    nextra = t->n - 1;
                }
                if (t->c[0] && t->c[0]->t == TT_VAR && nextra == 0 && !strcmp(nm, "call") && pl_dl_ctl_leaf_scope(cx)) return goal(cx, pl_dl_leaf_tree((tree_t *) t->c[0]), γnext, ωfail, entry_out);
                if (t->c[0] && t->c[0]->t == TT_VAR) return pl_meta_call_dyn(cx, t->c[0], extra, nextra, γnext, ωfail, entry_out);
                if (!strcmp(nm, "call") && nextra == 0 && t->c[0] && (t->c[0]->t == TT_CUT || ((t->c[0]->t == TT_QLIT || t->c[0]->t == TT_NAME) && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "!"))))
                    return goal(cx, pl_atom_goal("true"), γnext, ωfail, entry_out);
                const tree_t * ext = pl_meta_goal(t->c[0], extra, nextra);
                if (ext && pl_body_ill_typed(ext)) return goal(cx, pl_cc_type_error("callable", ext, nm, t->n), γnext, ωfail, entry_out);
                if (!ext)
                    return goal(cx, pl_cc_fnc1("throw", pl_cc_fnc2("error", pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("callable"), (tree_t *) t->c[0]), pl_cc_pi_ar(nm, t->n))), γnext, ωfail,
                    entry_out);
                if (pl_goal_holds_cut(ext)) return pl_meta_call_dyn(cx, t->c[0], extra, nextra, γnext, ωfail, entry_out);
                {
                    IR_t * callω = lpl_build(cx, IR_GOTO, ωfail, ωfail);
                    IR_t * saveω = cx->cutω;
                    cx->cutω = callω;
                    int save_scope = cx->cut_scope;
                    cx->cut_scope = ++cx->scope_seq;
                    IR_t * bank = pl_scc_armed() ? pl_cleanup_node(cx, CLEANUP_BANK, (IR_t *) 0, (IR_t *) 0, ωfail) : (IR_t *) 0;
                    IR_t * save_mark_0 = cx->cut_mark;
                    cx->cut_mark = bank;
                    lc_vec glv;
                    lc_vec_init(&glv, (int) sizeof(const tree_t *));
                    collect_conj(ext, &glv);
                    IR_t * ientry = NULL;
                    IR_t * iredo = NULL;
                    IR_t * itail = NULL;
                    int ctail = cx->tail;
                    pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, γnext, callω, &ientry, &iredo, &itail);
                    pl_seal_branch_tail(ctail, 0, itail);
                    cx->cutω = saveω;
                    cx->cut_scope = save_scope;
                    cx->cut_mark = save_mark_0;
                    cx->meta_redo = iredo;
                    cx->meta_redo_set = 1;
                    if (bank) { lc_γ_to(bank, ientry ? ientry : γnext); ientry = bank; }
                    if (pl_tree_is_control(ext) && pl_tree_control_var_leaf(ext)) {
                        IR_t * gge = NULL;
                        IR_t * gg = goal(cx, pl_cc_fnc1("$pl_goal_guard", (tree_t *) ext), ientry ? ientry : γnext, ωfail, &gge);
                        ientry = gge ? gge : gg;
                    }
                    if (entry_out) *entry_out = ientry;
                    return callω;
                }
            }
            if (!strcmp(nm, "throw") && t->n == 1) return pl_leaf(cx, "$throw", t, 1, ωfail, ωfail, entry_out);
            if (!strcmp(nm, "catch") && t->n == 3) return pl_lower_catch(cx, t->c[0], t->c[1], t->c[2], γnext, ωfail, entry_out);
            if (!strcmp(nm, "setup_call_cleanup") && t->n == 3) return pl_lower_scc(cx, t->c[0], t->c[1], NULL, t->c[2], γnext, ωfail, entry_out);
            if (!strcmp(nm, "setup_call_catcher_cleanup") && t->n == 4 && !pl_user_defines(nm, 4)) return pl_lower_scc(cx, t->c[0], t->c[1], t->c[2], t->c[3], γnext, ωfail, entry_out);
            if (!strcmp(nm, "call_cleanup") && t->n == 2 && !pl_user_defines(nm, 2)) return pl_lower_scc(cx, pl_atom_goal("true"), t->c[0], NULL, t->c[1], γnext, ωfail, entry_out);
            if (!strcmp(nm, "=") && t->n == 2) { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
            if (!strcmp(nm, "\\=") && t->n == 2) {
                tree_t * u = ast_node_new(TT_UNIFY);
                ast_push(u, (tree_t *) t->c[0]);
                ast_push(u, (tree_t *) t->c[1]);
                return pl_lower_ite(cx, u, pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "is") && t->n == 2) {
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$is_v";
                IR_t * xe = NULL;
                IR_t * xl = term_lval_e(cx, t->c[0], &xe);
                IR_t * ve = NULL;
                IR_t * v = (g_stage2_pl.pl_dialect == PL_DIALECT_XSB && t->c[1] && t->c[1]->t == TT_FNC && !pl_tree_is_big(t->c[1]) && !pl_var_nodes(t->c[1])) ? term_e(cx, t->c[1], &ve) :
                    lower_arith_val(cx, t->c[1], ωfail, &ve);
                lc_γ_to(xl, ve ? ve : v);
                lc_ω_to(xl, ωfail);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, xl);
                ir_operand_push(nd, v);
                if (entry_out) *entry_out = xe ? xe : xl;
                return nd;
            }
            {
                const char * csuf = (t->n == 2) ? pl_cmp_op_suffix(nm) : NULL;
                if (csuf) {
                    char nb[fmt_len("$cmp_%s", csuf)];
                    snprintf(nb, sizeof nb, "$cmp_%s", csuf);
                    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                    IR_LIT(nd).sval = ct_strdup(nb);
                    IR_t * ea = NULL;
                    IR_t * eb = NULL;
                    IR_t * a = lower_arith_val(cx, t->c[0], ωfail, &ea);
                    IR_t * b = lower_arith_val(cx, t->c[1], ωfail, &eb);
                    lc_γ_to(a, eb ? eb : b);
                    lc_ω_to(a, ωfail);
                    lc_γ_to(b, nd);
                    lc_ω_to(b, ωfail);
                    ir_operand_push(nd, a);
                    ir_operand_push(nd, b);
                    if (entry_out) *entry_out = ea ? ea : a;
                    return nd;
                }
            }
            if (!strcmp(nm, "tab") && t->n == 1 && !(t->c[0] && t->c[0]->t == TT_ILIT) && !pl_user_defines(nm, 1)) {
                tree_t * fv = pl_cc_freshvar();
                tree_t * f2 = pl_cc_freshvar();
                tree_t * f3 = pl_cc_freshvar();
                f2->v.ival = fv->v.ival;
                f3->v.ival = fv->v.ival;
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$tab";
                IR_t * ve = NULL;
                IR_t * v = term_e(cx, f3, &ve);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, v);
                {
                    IR_t * ge = NULL;
                    IR_t * g =
                        goal(cx,
                        pl_cc_fnc2(",", pl_cc_fnc2("is", fv, (tree_t *) t->c[0]),
                        pl_cc_ite(pl_cc_fnc1("integer", f2), (tree_t *) pl_atom_goal("true"), pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("integer"), f2), "tab"))), ve ? ve : v,
                        ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : g;
                }
                return nd;
            }
            if (!strcmp(nm, "tab") && t->n == 1) {
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$tab";
                IR_t * ve = NULL;
                IR_t * v = lower_arith_val(cx, t->c[0], ωfail, &ve);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, v);
                if (entry_out) *entry_out = ve ? ve : v;
                return nd;
            }
            if (!strcmp(nm, "tab") && t->n == 2) {
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$tab_s";
                IR_t * se = NULL;
                IR_t * sv = term_lval_e(cx, t->c[0], &se);
                IR_t * ve = NULL;
                IR_t * v = lower_arith_val(cx, t->c[1], ωfail, &ve);
                lc_γ_to(sv, ve ? ve : v);
                lc_ω_to(sv, ωfail);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, sv);
                ir_operand_push(nd, v);
                if (entry_out) *entry_out = se ? se : sv;
                return nd;
            }
            if (!strcmp(nm, "atom_concat") && t->n == 3) {
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$atom_concat_at";
                IR_t * to = lpl_build(cx, IR_TO, nd, ωfail);
                IR_LIT(to).sval = (char *) "ag:trl";
                IR_t * cnt = lpl_build(cx, IR_CALL, to, ωfail);
                IR_LIT(cnt).sval = "$atom_concat_n";
                IR_t * lo = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
                IR_LIT(lo).ival = 0;
                IR_t * ale = NULL, * ble = NULL, * cle = NULL;
                IR_t * al = term_lval_e(cx, t->c[0], &ale);
                IR_t * bl = term_lval_e(cx, t->c[1], &ble);
                IR_t * cl = term_lval_e(cx, t->c[2], &cle);
                IR_t * a2e = NULL, * b2e = NULL, * c2e = NULL;
                IR_t * a2 = term_lval_e(cx, t->c[0], &a2e);
                IR_t * b2 = term_lval_e(cx, t->c[1], &b2e);
                IR_t * c2 = term_lval_e(cx, t->c[2], &c2e);
                IR_t * gnl = lpl_build(cx, IR_LIT_STRING, NULL, ωfail);
                IR_LIT(gnl).sval = (char *) "atom_concat";
                IR_t * gchk = lpl_build(cx, IR_CALL, lo, ωfail);
                IR_LIT(gchk).sval = "$pl_anum_guard3";
                lc_γ_to(gnl, gchk);
                lc_ω_to(gnl, ωfail);
                ir_operand_push(gchk, gnl);
                ir_operand_push(gchk, al);
                ir_operand_push(gchk, bl);
                ir_operand_push(gchk, cl);
                lc_γ_to(al, ble ? ble : bl);
                lc_ω_to(al, ωfail);
                lc_γ_to(bl, cle ? cle : cl);
                lc_ω_to(bl, ωfail);
                lc_γ_to(cl, gnl);
                lc_ω_to(cl, ωfail);
                lc_γ_to(lo, a2e ? a2e : a2);
                lc_ω_to(lo, ωfail);
                lc_γ_to(a2, b2e ? b2e : b2);
                lc_ω_to(a2, ωfail);
                lc_γ_to(b2, c2e ? c2e : c2);
                lc_ω_to(b2, ωfail);
                lc_γ_to(c2, cnt);
                lc_ω_to(c2, ωfail);
                ir_operand_push(cnt, a2);
                ir_operand_push(cnt, b2);
                ir_operand_push(cnt, c2);
                ir_operand_push(to, lo);
                ir_operand_push(to, cnt);
                ir_operand_push(nd, to);
                ir_operand_push(nd, al);
                ir_operand_push(nd, bl);
                ir_operand_push(nd, cl);
                lc_ω_to_β(nd, to);
                if (entry_out) *entry_out = ale ? ale : al;
                return to;
            }
            if (!strcmp(nm, "sub_atom") && t->n == 5) {
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$sub_atom_at";
                IR_t * to = lpl_build(cx, IR_TO, nd, ωfail);
                IR_LIT(to).sval = (char *) "ag:trl";
                IR_t * ae = NULL;
                IR_t * av = term_e(cx, t->c[0], &ae);
                IR_t * cnt = lpl_build(cx, IR_CALL, to, ωfail);
                IR_LIT(cnt).sval = "$sub_atom_n";
                IR_t * av2e = NULL;
                IR_t * av2 = term_e(cx, t->c[0], &av2e);
                IR_t * lo = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
                IR_LIT(lo).ival = 0;
                IR_t * bl = NULL, * ll = NULL, * al = NULL, * sl = NULL;
                IR_t * be = NULL, * le = NULL, * ale = NULL, * se = NULL;
                bl = term_lval_e(cx, t->c[1], &be);
                ll = term_lval_e(cx, t->c[2], &le);
                al = term_lval_e(cx, t->c[3], &ale);
                sl = term_lval_e(cx, t->c[4], &se);
                IR_t * gnl = lpl_build(cx, IR_LIT_STRING, NULL, ωfail);
                IR_LIT(gnl).sval = (char *) "sub_atom";
                IR_t * gchk = lpl_build(cx, IR_CALL, lo, ωfail);
                IR_LIT(gchk).sval = "$pl_anum_guard5";
                lc_γ_to(gnl, gchk);
                lc_ω_to(gnl, ωfail);
                ir_operand_push(gchk, gnl);
                ir_operand_push(gchk, av);
                ir_operand_push(gchk, bl);
                ir_operand_push(gchk, ll);
                ir_operand_push(gchk, al);
                ir_operand_push(gchk, sl);
                lc_γ_to(bl, le ? le : ll);
                lc_ω_to(bl, ωfail);
                lc_γ_to(ll, ale ? ale : al);
                lc_ω_to(ll, ωfail);
                lc_γ_to(al, se ? se : sl);
                lc_ω_to(al, ωfail);
                lc_γ_to(sl, ae ? ae : av);
                lc_ω_to(sl, ωfail);
                lc_γ_to(av, gnl);
                lc_ω_to(av, ωfail);
                lc_γ_to(lo, av2e ? av2e : av2);
                lc_ω_to(lo, ωfail);
                lc_γ_to(av2, cnt);
                lc_ω_to(av2, ωfail);
                ir_operand_push(cnt, av2);
                ir_operand_push(cnt, bl);
                ir_operand_push(cnt, ll);
                ir_operand_push(cnt, al);
                ir_operand_push(cnt, sl);
                ir_operand_push(to, lo);
                ir_operand_push(to, cnt);
                ir_operand_push(nd, av);
                ir_operand_push(nd, to);
                ir_operand_push(nd, bl);
                ir_operand_push(nd, ll);
                ir_operand_push(nd, al);
                ir_operand_push(nd, sl);
                lc_ω_to_β(nd, to);
                if (entry_out) *entry_out = be ? be : bl;
                return to;
            }
            if (!strcmp(nm, "write_to_atom") && t->n == 2 && !pl_user_defines(nm, 2))
                return goal(cx, pl_cc_fnc2("with_output_to", pl_cc_fnc1("atom", (tree_t *) t->c[0]), pl_cc_fnc1("write", (tree_t *) t->c[1])), γnext, ωfail, entry_out);
            if (!strcmp(nm, "format_to_atom") && t->n == 3 && !pl_user_defines(nm, 3))
                return goal(cx, pl_cc_fnc2("with_output_to", pl_cc_fnc1("atom", (tree_t *) t->c[0]), pl_cc_fnc2("format", (tree_t *) t->c[1], (tree_t *) t->c[2])), γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_from_atom") && t->n == 2 && !pl_user_defines(nm, 2))
                return goal(cx, pl_cc_fnc3("atom_to_term", (tree_t *) t->c[0], (tree_t *) t->c[1], pl_cc_freshvar()), γnext, ωfail, entry_out);
            if (!strcmp(nm, "with_output_to") && t->n == 2 && !pl_user_defines(nm, 2)) {
                tree_t * ti = pl_cc_freshvar();
                tree_t * to = pl_cc_freshvar();
                tree_t * th = pl_cc_freshvar();
                tree_t * op = pl_cc_fnc3("$wot_open", ti, to, th);
                tree_t * so = pl_cc_fnc1("set_output", ti);
                tree_t * ev = pl_cc_freshvar();
                tree_t * ev2 = pl_cc_freshvar();
                ev2->v.ival = ev->v.ival;
                tree_t * dis = pl_cc_fnc3("$wot_discard", ti, to, th);
                tree_t * cg = pl_cc_fnc3("catch", pl_cc_fnc1("call", (tree_t *) t->c[1]), ev, pl_cc_fnc2(",", dis, pl_cc_fnc1("throw", ev2)));
                tree_t * cap = pl_cc_fnc4("$wot_capture", ti, to, th, (tree_t *) t->c[0]);
                tree_t * fb = pl_cc_fnc2(",", dis, pl_atom_goal("fail"));
                tree_t * ite = pl_cc_fnc2(";", pl_cc_fnc2("->", cg, cap), fb);
                tree_t * body = pl_cc_fnc2(",", op, pl_cc_fnc2(",", so, ite));
                body = pl_cc_ite(pl_cc_fnc1("$wot_stream_sink", (tree_t *) t->c[0]), pl_cc_fnc2("$wot_stream", (tree_t *) t->c[0], (tree_t *) t->c[1]), body);
                return goal(cx, body, γnext, ωfail, entry_out);
            }
            extern int pl_prelude_defines(const char *, int);
            if (!strcmp(nm, "aggregate_all") && t->n == 3 && (!pl_user_defines(nm, 3) || pl_prelude_defines(nm, 3))) {
                const tree_t * spec = t->c[0];
                const char * kind = pl_aggregate_all_native_kind(spec);
                const tree_t * tmpl = !kind ? (const tree_t *) 0 : (spec->t == TT_FNC ? spec->c[0] : pl_cc_ilit(0));
                if (kind && !strcmp(kind, "set")) {
                    tree_t * lv = pl_cc_freshvar();
                    tree_t * fa = pl_cc_fnc3("findall", (tree_t *) tmpl, (tree_t *) t->c[1], lv);
                    tree_t * so = pl_cc_fnc2("sort", lv, (tree_t *) t->c[2]);
                    return goal(cx, pl_cc_fnc2(",", fa, so), γnext, ωfail, entry_out);
                }
                if (kind) {
                    tree_t * lv = pl_cc_freshvar();
                    tree_t * fa = pl_cc_fnc3("findall", (tree_t *) tmpl, (tree_t *) t->c[1], lv);
                    tree_t * rd = pl_cc_fnc3("$aggregate_reduce", (tree_t *) pl_atom_goal(kind), lv, (tree_t *) t->c[2]);
                    return goal(cx, pl_cc_fnc2(",", fa, rd), γnext, ωfail, entry_out);
                }
            }
            if (g_stage2.depth_counted && !strcmp(nm, "findall") && t->n == 3 && !pl_dl_prelude_key(cx) && !(cx->cur_key && !strncmp(cx->cur_key, "$dl_", 4)))
                return goal(cx, pl_cc_fnc3(cx->cur_key && !strncmp(cx->cur_key, "$mc:findall/", 12) ? "$dl_f2" : "$dl_findall", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]), γnext,
                ωfail, entry_out);
            if (((!strcmp(nm, "findall") || !strcmp(nm, "bagof") || !strcmp(nm, "setof")) && t->n == 3) || (!strcmp(nm, "findall") && t->n == 4)) {
                int pl_find4 = t->n == 4;
                const char * fin = !strcmp(nm, "findall") ? (pl_find4 ? "$findall_result4" : "$findall_result") : (!strcmp(nm, "bagof") ? "$bagof_result" : "$setof_result");
                const tree_t * gt = pl_caret_body(t->c[1]);
                int wantg = pl_goal_arg_wants_guard(gt);
                int pl_isfind = !strcmp(nm, "findall");
                if (!pl_isfind && gt && gt->t == TT_VAR)
                    return goal(cx, pl_cc_fnc3(!strcmp(nm, "bagof") ? "$bagof_var" : "$setof_var", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]), γnext, ωfail, entry_out);
                int pl_fvn = pl_isfind ? 1 : pl_var_nodes(t->c[1]) + 1;
                int pl_fv[pl_fvn];
                int pl_nfv = pl_isfind ? 0 : pl_free_vars(t->c[0], t->c[1], pl_fv, pl_fvn);
                if (pl_nfv > 0) {
                    const char * gat = !strcmp(nm, "bagof") ? "$bagof_group_at" : "$setof_group_at";
                    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                    IR_LIT(nd).sval = (char *) gat;
                    IR_t * to = lpl_build(cx, IR_TO, nd, ωfail);
                    IR_LIT(to).sval = (char *) "ag:trl";
                    IR_t * cnt = lpl_build(cx, IR_CALL, to, ωfail);
                    IR_LIT(cnt).sval = "$bagof_group_n";
                    IR_t * lo = lpl_build(cx, IR_LIT_INTEGER, cnt, ωfail);
                    IR_LIT(lo).ival = 0;
                    IR_t * acc = lpl_build(cx, IR_CALL, NULL, ωfail);
                    IR_LIT(acc).sval = "$findall_new";
                    IR_t * add = lpl_build(cx, IR_CALL, NULL, ωfail);
                    IR_LIT(add).sval = "$findall_add";
                    IR_t * gentry = NULL;
                    IR_t * gredo = NULL;
                    lc_vec glv2;
                    lc_vec_init(&glv2, (int) sizeof(const tree_t *));
                    collect_conj(gt, &glv2);
                    IR_t * te2 = NULL;
                    IR_t * tv2 = term_e(cx, t->c[0], &te2);
                    IR_t ** wk = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    IR_t ** we = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    for (int i = 0; i < pl_nfv; i++) { wk[i] = lpl_build(cx, IR_VAR, NULL, ωfail); IR_LIT(wk[i]).sval = pl_vname(cx, pl_fv[i]); we[i] = NULL; }
                    IR_t * wce = NULL;
                    IR_t * wc = mkc_node(cx, "$w", pl_nfv, wk, we, &wce);
                    IR_t * pkids[2];
                    IR_t * pkes[2];
                    pkids[0] = wc;
                    pkes[0] = wce;
                    pkids[1] = tv2;
                    pkes[1] = te2;
                    IR_t * pe = NULL;
                    IR_t * pr = mkc_node(cx, "-", 2, pkids, pkes, &pe);
                    IR_t * mark2 = lpl_build(cx, IR_BOUND, NULL, ωfail);
                    IR_t * unmk2 = lpl_build(cx, IR_UNMARK, lo, ωfail);
                    ir_operand_push(unmk2, mark2);
                    IR_LIT(unmk2).ival = 5;
                    IR_t * bc2 = lpl_build(cx, IR_CALL, unmk2, ωfail);
                    IR_LIT(bc2).sval = "$ball_pending";
                    IR_t * gcutω2 = lpl_build(cx, IR_GOTO, unmk2, unmk2);
                    IR_t * gsaveω2 = cx->cutω;
                    cx->cutω = gcutω2;
                    int gsave_scope2 = cx->cut_scope;
                    cx->cut_scope = ++cx->scope_seq;
                    IR_t * save_mark_0 = cx->cut_mark;
                    cx->cut_mark = mark2;
                    IR_t * first2 = pl_lower_conj(cx, (const tree_t * const *) glv2.data, glv2.n, pe ? pe : pr, bc2, &gentry, &gredo, NULL);
                    cx->cutω = gsaveω2;
                    cx->cut_scope = gsave_scope2;
                    cx->cut_mark = save_mark_0;
                    lc_γ_to(acc, mark2);
                    lc_γ_to(mark2, gentry ? gentry : (first2 ? first2 : unmk2));
                    lc_γ_to(pr, add);
                    lc_ω_to(pr, lo);
                    ir_operand_push(add, acc);
                    ir_operand_push(add, pr);
                    if (gredo) lc_γ_to_β(add, gredo);
                    else lc_γ_to(add, lo);
                    lc_ω_to(add, lo);
                    IR_t ** rk = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    IR_t ** rke = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    for (int i = 0; i < pl_nfv; i++) { rk[i] = lpl_build(cx, IR_VAR_REF, NULL, ωfail); IR_LIT(rk[i]).sval = pl_vname(cx, pl_fv[i]); rke[i] = NULL; }
                    IR_t * wre = NULL;
                    IR_t * wr = mkc_node(cx, "$w", pl_nfv, rk, rke, &wre);
                    IR_t * re2 = NULL;
                    IR_t * rl2 = term_lval_e(cx, t->c[2], &re2);
                    lc_γ_to(rl2, wre ? wre : wr);
                    lc_ω_to(rl2, ωfail);
                    lc_γ_to(wr, acc);
                    lc_ω_to(wr, ωfail);
                    ir_operand_push(cnt, acc);
                    ir_operand_push(to, lo);
                    ir_operand_push(to, cnt);
                    ir_operand_push(nd, acc);
                    ir_operand_push(nd, to);
                    ir_operand_push(nd, wr);
                    ir_operand_push(nd, rl2);
                    lc_ω_to_β(nd, to);
                    ct_drop(wk);
                    ct_drop(we);
                    ct_drop(rk);
                    ct_drop(rke);
                    { IR_t * lge = NULL; IR_t * lg = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[2]), re2 ? re2 : rl2, ωfail, &lge); if (entry_out) *entry_out = lge ? lge : lg; }
                    return to;
                }
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = (char *) fin;
                IR_t * acc = lpl_build(cx, IR_CALL, NULL, ωfail);
                IR_LIT(acc).sval = "$findall_new";
                IR_t * add = lpl_build(cx, IR_CALL, NULL, ωfail);
                IR_LIT(add).sval = "$findall_add";
                IR_t * mark = lpl_build(cx, IR_BOUND, NULL, ωfail);
                IR_t * unmk = lpl_build(cx, IR_UNMARK, nd, ωfail);
                ir_operand_push(unmk, mark);
                IR_LIT(unmk).ival = 5;
                IR_t * gentry = NULL;
                IR_t * gredo = NULL;
                lc_vec glv;
                lc_vec_init(&glv, (int) sizeof(const tree_t *));
                collect_conj(pl_isfind ? (wantg ? gt : t->c[1]) : gt, &glv);
                IR_t * te = NULL;
                IR_t * tv = term_e(cx, t->c[0], &te);
                IR_t * bc = lpl_build(cx, IR_CALL, unmk, ωfail);
                IR_LIT(bc).sval = "$ball_pending";
                IR_t * gcutω = lpl_build(cx, IR_GOTO, unmk, unmk);
                IR_t * gsaveω = cx->cutω;
                cx->cutω = gcutω;
                int gsave_scope = cx->cut_scope;
                cx->cut_scope = ++cx->scope_seq;
                IR_t * save_mark_0 = cx->cut_mark;
                cx->cut_mark = mark;
                IR_t * first = pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, te ? te : tv, bc, &gentry, &gredo, NULL);
                cx->cutω = gsaveω;
                cx->cut_scope = gsave_scope;
                cx->cut_mark = save_mark_0;
                lc_γ_to(acc, mark);
                lc_γ_to(mark, gentry ? gentry : (first ? first : unmk));
                lc_γ_to(tv, add);
                lc_ω_to(tv, unmk);
                ir_operand_push(add, acc);
                ir_operand_push(add, tv);
                if (gredo) lc_γ_to_β(add, gredo);
                else lc_γ_to(add, unmk);
                lc_ω_to(add, unmk);
                IR_t * re = NULL;
                IR_t * rl = term_lval_e(cx, t->c[2], &re);
                lc_γ_to(rl, nd);
                lc_ω_to(rl, ωfail);
                ir_operand_push(nd, acc);
                ir_operand_push(nd, rl);
                lc_γ_to(rl, acc);
                if (pl_find4) { IR_t * t4e = NULL; IR_t * t4 = term_lval_e(cx, t->c[3], &t4e); lc_γ_to(t4, acc); lc_ω_to(t4, ωfail); lc_γ_to(rl, t4e ? t4e : t4); ir_operand_push(nd, t4); }
                if (wantg) {
                    IR_t * ge = NULL;
                    IR_t * gv = term_lval_e(cx, gt, &ge);
                    IR_t * gchk = lpl_build(cx, IR_CALL, re ? re : rl, ωfail);
                    IR_LIT(gchk).sval = "$pl_goal_guard";
                    ir_operand_push(gchk, gv);
                    lc_γ_to(gv, gchk);
                    lc_ω_to(gv, ωfail);
                    { IR_t * lge = NULL; IR_t * lg = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[2]), ge ? ge : gv, ωfail, &lge); if (entry_out) *entry_out = lge ? lge : lg; }
                } else {
                    IR_t * lge = NULL;
                    IR_t * lg = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[2]), re ? re : rl, ωfail, &lge);
                    if (entry_out) *entry_out = lge ? lge : lg;
                }
                if (pl_find4) {
                    IR_t * hd = entry_out ? *entry_out : nd;
                    IR_t * t4ge = NULL;
                    IR_t * t4g = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[3]), hd, ωfail, &t4ge);
                    if (entry_out) *entry_out = t4ge ? t4ge : t4g;
                }
                return nd;
            }
            if (!strcmp(nm, "between") && t->n == 3 && !pl_user_defines(nm, 3)) return goal(cx, pl_between_split(t), γnext, ωfail, entry_out);
            if ((!strcmp(nm, "$between_gen") || !strcmp(nm, "for")) && t->n == 3) {
                int forarg = !strcmp(nm, "for");
                const tree_t * lo_t = forarg ? t->c[1] : t->c[0];
                const tree_t * hi_t = forarg ? t->c[2] : t->c[1];
                const tree_t * vr_t = forarg ? t->c[0] : t->c[2];
                IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$is_v";
                IR_t * to = lpl_build(cx, IR_TO, nd, ωfail);
                IR_LIT(to).sval = (char *) "ag:trl";
                IR_t * loe = NULL;
                IR_t * lo = lower_arith_val(cx, lo_t, ωfail, &loe);
                IR_t * hie = NULL;
                IR_t * hi = lower_arith_val(cx, hi_t, ωfail, &hie);
                IR_t * xe = NULL;
                IR_t * xl = term_lval_e(cx, vr_t, &xe);
                lc_γ_to(xl, loe ? loe : lo);
                lc_ω_to(xl, ωfail);
                lc_γ_to(lo, hie ? hie : hi);
                lc_ω_to(lo, ωfail);
                IR_t * lov = lo;
                IR_t * hiv = hi;
                {
                    IR_t * chk = lpl_build(cx, IR_CALL, to, ωfail);
                    IR_LIT(chk).sval = "$pl_between_guard";
                    ir_operand_push(chk, lo);
                    ir_operand_push(chk, hi);
                    ir_operand_push(chk, xl);
                    lc_γ_to(hi, chk);
                    lc_ω_to(hi, ωfail);
                    IR_t * last = chk;
                    if (lo_t && lo_t->t == TT_VAR) {
                        IR_t * ve = NULL;
                        lov = lower_arith_val(cx, pl_cc_fnc1("+", (tree_t *) lo_t), ωfail, &ve);
                        lc_γ_to(last, ve ? ve : lov);
                        lc_ω_to(lov, ωfail);
                        last = lov;
                    }
                    if (hi_t && hi_t->t == TT_VAR) {
                        IR_t * ve = NULL;
                        hiv = lower_arith_val(cx, pl_cc_fnc1("+", (tree_t *) hi_t), ωfail, &ve);
                        lc_γ_to(last, ve ? ve : hiv);
                        lc_ω_to(hiv, ωfail);
                        last = hiv;
                    }
                    lc_γ_to(last, to);
                }
                ir_operand_push(to, lov);
                ir_operand_push(to, hiv);
                ir_operand_push(nd, xl);
                ir_operand_push(nd, to);
                lc_ω_to_β(nd, to);
                if (entry_out) *entry_out = xe ? xe : xl;
                return to;
            }
            if (!strcmp(nm, "current_output") && t->n == 1) return pl_leaf_guarded1(cx, "$current_output", "$pl_curstream_guard", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "current_input") && t->n == 1) return pl_leaf_guarded1(cx, "$current_input", "$pl_curstream_guard", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "set_output") && t->n == 1) return pl_leaf_guarded1_dir(cx, "$set_output", "$pl_stream_guard", "output", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "set_input") && t->n == 1) return pl_leaf_guarded1_dir(cx, "$set_input", "$pl_stream_guard", "input", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 2 && pl_tree_is_nil(t->c[1])) return pl_leaf_lv(cx, "$read", t, 1, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 2) return pl_leaf_lv(cx, "$read_term_opts", t, 2, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 3 && pl_tree_is_nil(t->c[2])) return pl_leaf_lv(cx, "$read_s", t, 2, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 3) return pl_leaf_lv(cx, "$read_term_opts_s", t, 3, γnext, ωfail, entry_out);
            if (!strcmp(nm, "$db_decls") && t->n >= 1) return pl_leaf_lv(cx, "$db_decls", t, t->n, γnext, ωfail, entry_out);
            if (t->n == 1 && !strcmp(nm, "$dl_dyn")) return pl_meta_call_dyn(cx, t->c[0], NULL, 0, γnext, ωfail, entry_out);
            if (t->n == 1 && (!strcmp(nm, "$dl_push") || !strcmp(nm, "$dl_reached") || !strcmp(nm, "$dl_false") || !strcmp(nm, "$dl_reset") || !strcmp(nm, "$dl_tick")))
                return pl_leaf_lv(cx, nm, t, 1, γnext, ωfail, entry_out);
            if (!strcmp(nm, "$db_data") && t->n == 1) {
                int dar = 0;
                const char * dpn = pl_head_key(t->c[0], &dar);
                if (!dpn) return goal(cx, pl_cc_perm_access(nm, 1), γnext, ωfail, entry_out);
                return pl_db_data(cx, pl_dyn_index_or_add(dpn, dar), t->c[0], γnext, ωfail, entry_out);
            }
            if (t->n == 1 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, ":") && t->c[0]->n == 2 && pl_name_in(nm, pl_db_mq_builtins)) {
                tree_t * g2 = ast_node_new(TT_FNC);
                g2->v = t->v;
                g2->line = t->line;
                int mq_swi = g_stage2_pl.pl_dialect == PL_DIALECT_SWI;
                if (mq_swi && strcmp(nm, "abolish")) { tree_t * mt = pl_db_mq_term((const char *) 0, t->c[0]); if (mt) { ast_push(g2, mt); return goal(cx, g2, γnext, ωfail, entry_out); } }
                int mq_user = (t->c[0]->c[0]->t == TT_QLIT || t->c[0]->c[0]->t == TT_NAME) && t->c[0]->c[0]->v.sval && !strcmp(t->c[0]->c[0]->v.sval, "user");
                if (t->c[0]->c[0] && (t->c[0]->c[0]->t == TT_VAR || (mq_swi && !mq_user))) {
                    tree_t * q = pl_cc_freshvar();
                    tree_t * q2 = ast_node_new(TT_VAR);
                    q2->v.ival = q->v.ival;
                    ast_push(g2, q2);
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc3(!mq_swi ? "$mod_head" : !strcmp(nm, "abolish") ? "$mq_db_pi" : "$mq_db_head", t->c[0]->c[0], t->c[0]->c[1], q), g2), γnext, ωfail,
                        entry_out);
                }
                ast_push(g2, t->c[0]->c[1]);
                return goal(cx, g2, γnext, ωfail, entry_out);
            }
            if ((!strcmp(nm, "assertz") || !strcmp(nm, "assert") || !strcmp(nm, "asserta")) && (t->n == 1 || t->n == 2)) {
                tree_t * mt = (g_stage2_pl.pl_dialect == PL_DIALECT_SWI && t->n == 1) ? pl_db_mq_term((const char *) 0, t->c[0]) : NULL;
                if (mt && mt != t->c[0]) *t->c[0] = *mt;
                pl_clause_unqual_head(t->c[0]);
            }
            if ((!strcmp(nm, "assertz") || !strcmp(nm, "assert") || !strcmp(nm, "asserta")) && t->n == 1) {
                int ar = 0;
                const char * pn;
                { const tree_t * bad = pl_clause_ill_typed(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 1), γnext, ωfail, entry_out); }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn)
                    return goal(cx,
                    pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal(nm)),
                    (!strcmp(nm, "asserta") ? pl_cc_fnc1("$db_asserta_t", (tree_t *) t->c[0]) : pl_cc_fnc3("$db_assertz_t", (tree_t *) t->c[0], pl_cc_ilit(-1), pl_cc_ilit(0)))), γnext, ωfail,
                    entry_out);
                if (pl_pi_is_static_builtin(pn, ar) || !pl_db_writable(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_leaf2(cx, !strcmp(nm, "asserta") ? "$db_asserta" : "$db_assertz", pl_dyn_index_or_add(pn, ar), t->c[0], γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    {
                        IR_t * first = se ? se : le;
                        if (pl_tree_has_var(t->c[0])) {
                            int hb = t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, ":-") && t->c[0]->n == 2 && pl_tree_has_var(t->c[0]->c[1]);
                            IR_t * ge = NULL;
                            pl_db_leaf2_tree(cx, "$db_t_guard", t->c[0], pl_atom_goal(hb ? "assert" : "assert_cyc"), first, ωfail, &ge);
                            if (ge) first = ge;
                        }
                        if (entry_out) *entry_out = first;
                    }
                    return nd;
                }
            }
            if ((!strcmp(nm, "asserta") || !strcmp(nm, "assertz") || !strcmp(nm, "assert")) && t->n == 2) {
                { const tree_t * bad = pl_clause_ill_typed(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 2), γnext, ωfail, entry_out); }
                return goal(cx,
                    pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[1], (tree_t *) pl_atom_goal("clref_out")),
                    pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("assert")),
                    pl_cc_fnc2(!strcmp(nm, "asserta") ? "$db_asserta_r" : "$db_assertz_r", (tree_t *) t->c[0], (tree_t *) t->c[1]))), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "clause") && (t->n == 2 || t->n == 3) && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, ":") && t->c[0]->n == 2 && t->c[0]->c[0] &&
                t->c[0]->c[1])
                return goal(cx,
                t->n == 2 ? pl_cc_fnc3("$pl_clause_q", (tree_t *) t->c[0]->c[0], (tree_t *) t->c[0]->c[1], (tree_t *) t->c[1]) :
                pl_cc_fnc4("$pl_clause_q", (tree_t *) t->c[0]->c[0], (tree_t *) t->c[0]->c[1], (tree_t *) t->c[1], (tree_t *) t->c[2]), γnext, ωfail, entry_out);
            if (!strcmp(nm, "clause") && t->n == 3) {
                {
                    const tree_t * bad = pl_tree_number(t->c[0]);
                    if (!bad) bad = pl_tree_number(t->c[1]);
                    if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 3), γnext, ωfail, entry_out);
                }
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum_ref(cx, pl_clause_target(t->c[0], t->c[1]), t->c[2], γnext, ωfail, &le);
                    IR_t * ge = NULL;
                    pl_db_leaf2_tree(cx, "$db_t_guard", t->c[2], pl_atom_goal("clref_opt"), le, ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : le;
                    return nd;
                }
            }
            if (!strcmp(nm, "erase") && t->n == 1) {
                tree_t * clr = pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("clref_in")), pl_cc_fnc1("$db_erase_ref", (tree_t *) t->c[0]));
                tree_t * er = pl_cc_fnc2(";", pl_cc_fnc2("->", pl_cc_fnc1("$rec_ref", (tree_t *) t->c[0]), pl_cc_fnc1("$rec_erase", (tree_t *) t->c[0])), clr);
                if (g_lower.pl.hook_dyn_n > 0) er = pl_cc_fnc2(",", er, pl_cc_fnc3("$tbl_dchange", (tree_t *) pl_atom_goal("erase"), (tree_t *) t->c[0], (tree_t *) pl_atom_goal("incr")));
                return goal(cx, er, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "retract") && t->n == 1) {
                int ar = 0;
                const char * pn;
                { const tree_t * bad = pl_clause_ill_typed(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 1), γnext, ωfail, entry_out); }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn) {
                    tree_t * tg = pl_cc_freshvar();
                    tree_t * tg2 = pl_cc_freshvar();
                    tree_t * tg3 = pl_cc_freshvar();
                    IR_t * le = NULL;
                    IR_t * nd;
                    IR_t * ge = NULL;
                    IR_t * te = NULL;
                    IR_t * tn;
                    tg2->v.ival = tg->v.ival;
                    tg3->v.ival = tg->v.ival;
                    nd = pl_db_enum(cx, -1, tg, 1, γnext, ωfail, &le);
                    pl_db_leaf2_tree(cx, "$db_t_guard", tg2, pl_atom_goal("retract"), le, ωfail, &ge);
                    tn = goal(cx, pl_cc_fnc2("$retract_target", (tree_t *) t->c[0], tg3), ge ? ge : le, ωfail, &te);
                    if (entry_out) *entry_out = te ? te : tn;
                    return nd;
                }
                if (pl_pi_is_static_builtin(pn, ar) || !pl_db_writable(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, pl_dyn_index_or_add(pn, ar), pl_clause_target(t->c[0], NULL), 1, γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
            }
            if (!strcmp(nm, "retractall") && t->n == 1) {
                int ar = 0;
                const char * pn;
                { const tree_t * bad = pl_tree_number(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 1), γnext, ωfail, entry_out); }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn)
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("retractall")), pl_cc_fnc1("$db_retractall_t", (tree_t *) t->c[0])), γnext,
                    ωfail, entry_out);
                if (pl_pi_is_static_builtin(pn, ar) || !pl_db_writable(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_leaf2(cx, "$db_retractall", pl_dyn_index_or_add(pn, ar), t->c[0], γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
            }
            if (!strcmp(nm, "display") && t->n == 1 && (g_stage2_pl.pl_dialect == PL_DIALECT_SWI) && !pl_user_defines(nm, 1))
                return goal(cx, pl_cc_fnc1("$display_swi", (tree_t *) t->c[0]), γnext, ωfail, entry_out);
            if (!strcmp(nm, "arg") && t->n == 3 && (g_stage2_pl.pl_dialect == PL_DIALECT_SWI) && t->c[0] && t->c[0]->t != TT_ILIT && !pl_user_defines(nm, 3) &&
                !(cx->cur_key && !strncmp(cx->cur_key, "$arg_swi", 8))) return goal(cx, pl_cc_fnc3("$arg_swi", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]), γnext, ωfail, entry_out);
            if (!strcmp(nm, "write_canonical") && (t->n == 1 || t->n == 2) && (g_stage2_pl.pl_dialect == PL_DIALECT_SWI) && !pl_user_defines(nm, t->n))
                return goal(cx, t->n == 1 ? pl_cc_fnc1("$write_canonical_swi", (tree_t *) t->c[0]) : pl_cc_fnc2("$write_canonical_swi", (tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail,
                entry_out);
            if (!strcmp(nm, "print") && (t->n == 1 || t->n == 2) && !pl_user_defines(nm, t->n) && !pl_user_defines("portray", 1) && !pl_db_owned("portray", 1)) {
                const char * wn = (g_stage2_pl.pl_dialect == PL_DIALECT_SWI) ? "writeq" : "write";
                return goal(cx, t->n == 1 ? pl_cc_fnc1(wn, (tree_t *) t->c[0]) : pl_cc_fnc2(wn, (tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "put") && t->n == 1 && t->c[0] && (t->c[0]->t == TT_FNC || t->c[0]->t == TT_MAKELIST) && !pl_user_defines(nm, 1)) {
                tree_t * v = pl_cc_freshvar();
                return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("is", v, (tree_t *) t->c[0]), pl_cc_fnc1("put_code", v)), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "abolish") && t->n == 2 && !pl_user_defines(nm, 2))
                return goal(cx, pl_cc_fnc1("abolish", pl_cc_fnc2("/", (tree_t *) t->c[0], (tree_t *) t->c[1])), γnext, ωfail, entry_out);
            if (!strcmp(nm, "abolish") && t->n == 1 && g_stage2_pl.pl_dialect == PL_DIALECT_XSB && t->c[0] && t->c[0]->v.sval &&
                (t->c[0]->t == TT_QLIT || t->c[0]->t == TT_NAME || (t->c[0]->t == TT_FNC && strcmp(t->c[0]->v.sval, "/") && strcmp(t->c[0]->v.sval, ":"))))
                return goal(cx, pl_cc_fnc1("abolish", pl_cc_fnc2("/", (tree_t *) pl_atom_goal(t->c[0]->v.sval), pl_cc_ilit(t->c[0]->t == TT_FNC ? t->c[0]->n : 0))), γnext, ωfail, entry_out);
            if (!strcmp(nm, "abolish") && t->n == 1) {
                int ar = 0;
                const char * pn;
                { tree_t * bad = pl_cc_spec_ill_typed(t->c[0], nm); if (bad) return goal(cx, bad, γnext, ωfail, entry_out); }
                pn = pl_spec_key(t->c[0], &ar);
                if (!pn)
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("abolish")), pl_cc_fnc1("$db_abolish_t", (tree_t *) t->c[0])), γnext, ωfail,
                    entry_out);
                if (pl_pi_is_static_builtin(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                if (!pl_db_owned(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_leaf1(cx, "$db_abolish", pl_dyn_index_or_add(pn, ar), γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
            }
            if ((!strcmp(nm, "nb_getval") || !strcmp(nm, "b_getval")) && t->n == 2 && pl_file_defines("exception", 3))
                return goal(cx, pl_cc_fnc2(nm[0] == 'n' ? "$nb_getval_hooked" : "$b_getval_hooked", (tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
            if (!strcmp(nm, "nb_delete") && t->n == 1) {
                const char * kk = pl_nb_key(t->c[0]);
                int k = kk ? pl_dyn_index_or_add(kk, -1) : -1;
                if (kk && k < 0) pl_refuse("global variable needs a root cell but the 64 compile-time root cells are exhausted --", kk, 10);
                return kk ? pl_db_leaf2(cx, "$nb_delete", k, t->c[0], γnext, ωfail, entry_out) : pl_db_leaf2_tree(cx, "$nb_delete", t->c[0], t->c[0], γnext, ωfail, entry_out);
            }
            if ((!strcmp(nm, "nb_setval") || !strcmp(nm, "nb_getval") || !strcmp(nm, "b_setval") || !strcmp(nm, "b_getval") || !strcmp(nm, "$nb_getval_raw") || !strcmp(nm, "$b_getval_raw")) &&
                t->n == 2) {
                const char * kk = pl_nb_key(t->c[0]);
                if (!kk) {
                    if (!strcmp(nm, "nb_setval")) return pl_db_leaf2_tree(cx, "$nb_setval", t->c[0], t->c[1], γnext, ωfail, entry_out);
                    if (!strcmp(nm, "b_setval")) return pl_nb_leaf_lv_tree(cx, "$b_setval", t->c[0], t->c[1], γnext, ωfail, entry_out);
                    {
                        IR_t * body_entry = NULL;
                        IR_t * nd = pl_nb_leaf_lv_tree(cx, "$nb_getval", t->c[0], t->c[1], γnext, ωfail, &body_entry);
                        IR_t * guard_entry = NULL;
                        pl_db_leaf2_tree(cx, "$pl_nb_getval_guard", t->c[0], t->c[0], body_entry, ωfail, &guard_entry);
                        if (entry_out) *entry_out = guard_entry;
                        return nd;
                    }
                }
                {
                    int k = pl_dyn_index_or_add(kk, -1);
                    if (k < 0) pl_refuse("global variable needs a root cell but the 64 compile-time root cells are exhausted --", kk, 10);
                    if (!strcmp(nm, "nb_setval")) return pl_db_leaf2(cx, "$nb_setval", k, t->c[1], γnext, ωfail, entry_out);
                    if (!strcmp(nm, "b_setval")) return pl_nb_leaf_lv(cx, "$b_setval", k, t->c[1], γnext, ωfail, entry_out);
                    {
                        IR_t * body_entry = NULL;
                        IR_t * nd = pl_nb_leaf_lv(cx, "$nb_getval", k, t->c[1], γnext, ωfail, &body_entry);
                        tree_t * kt = ast_node_new(TT_QLIT);
                        kt->v.sval = (char *) kk;
                        IR_t * guard_entry = NULL;
                        pl_db_leaf2(cx, "$pl_nb_getval_guard", k, kt, body_entry, ωfail, &guard_entry);
                        if (entry_out) *entry_out = guard_entry;
                        return nd;
                    }
                }
            }
            if (!strcmp(nm, "clause") && t->n == 2) {
                int ar = 0;
                const char * pn;
                {
                    const tree_t * bad = pl_tree_number(t->c[0]);
                    if (!bad) bad = pl_tree_number(t->c[1]);
                    if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 2), γnext, ωfail, entry_out);
                }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn) {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, -1, pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &le);
                    IR_t * ge = NULL;
                    pl_db_leaf2_tree(cx, "$db_t_guard", pl_clause_target(t->c[0], t->c[1]), pl_atom_goal("clause"), le, ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : le;
                    return nd;
                }
                if (pl_pi_is_static_builtin(pn, ar) && !pl_swi_user_redefines(pn, ar)) return goal(cx, pl_cc_perm_access(pn, ar), γnext, ωfail, entry_out);
                if (pl_db_owned(pn, ar)) {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, pl_dyn_index_or_add(pn, ar), pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
                if (!pl_file_defines(pn, ar)) {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, -1, pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &le);
                    IR_t * ge = NULL;
                    pl_db_leaf2_tree(cx, "$db_t_guard", pl_clause_target(t->c[0], t->c[1]), pl_atom_goal("clause"), le, ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : le;
                    return nd;
                }
                {
                    char key[fmt_len("%s/%d", pn, ar)];
                    snprintf(key, sizeof key, "%s/%d", pn, ar);
                    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
                    int k = pl_dyn_index_or_add(pn, ar);
                    if (k < 0 || !ch) pl_refuse("clause/2 on a file-defined predicate needs a root cell and the 64 compile-time root cells are exhausted --", pn, 10);
                    {
                        IR_t * enum_entry = NULL;
                        IR_t * to = pl_db_enum(cx, k, pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &enum_entry);
                        IR_t * next = enum_entry;
                        IR_t * first = NULL;
                        int n = (ch->t == TT_CHOICE) ? ch->n : 1;
                        for (int i = n - 1; i >= 0; i--) {
                            const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[i] : ch;
                            IR_t * se = NULL;
                            pl_db_leaf_seed(cx, k, i, pl_static_clause_term(cl, pn, ar, 1), pl_seed_file_term(pl_pred_file(pn, ar)), pl_seed_line_term(cl), next, ωfail, &se);
                            next = se;
                            first = se;
                        }
                        {
                            IR_t * body_first = first ? first : enum_entry;
                            IR_t * ge = NULL;
                            if (!pl_swi_user_redefines(pn, ar)) pl_db_leaf2_tree(cx, "$db_t_guard", pl_clause_target(t->c[0], t->c[1]), pl_atom_goal("clause"), body_first, ωfail, &ge);
                            if (entry_out) *entry_out = ge ? ge : body_first;
                            return to;
                        }
                    }
                }
            }
            {
                static const struct {
                    const char * n;
                    int a;
                    const char * k;
                    const char * leaf;
                } io[] = { { "put_code", 1, "put_code1", "$put_code" }, { "put_code", 2, "put_code2", "$put_code_s" }, { "put_char", 1, "put_char1", "$put_char" }, { "put_char", 2, "put_char2",
                    "$put_char_c_s" }, { "get_code", 1, "in_code1", "$get_code" }, { "get_code", 2, "in_code2", "$get_code_s" }, { "peek_code", 1, "in_code1", "$peek_code" }, { "peek_code", 2,
                    "in_code2", "$peek_code_s" }, { "get_char", 1, "in_char1", "$get_char" }, { "get_char", 2, "in_char2", "$get_char_s" }, { "peek_char", 1, "in_char1", "$peek_char" }, { "peek_char",
                    2, "in_char2", "$peek_char_s" }, { "get_byte", 1, "in_byte1", "$get_byte" }, { "get_byte", 2, "in_byte2", "$get_byte_s" }, { "peek_byte", 1, "in_byte1", "$peek_byte" },
                    { "peek_byte", 2, "in_byte2", "$peek_byte_s" }, { "get", 1, "in_code1", "$get_edin" }, { "get0", 1, "in_code1", "$get_code" }, { "skip", 1, "put_code1", "$skip" }, { NULL, 0, NULL,
                    NULL } };
                for (int i = 0; io[i].n; i++) if (t->n == io[i].a && !strcmp(nm, io[i].n) && !pl_user_defines(nm, t->n)) {
                    tree_t * raw = ast_node_new(TT_FNC);
                    raw->v.sval = (char *) io[i].leaf;
                    for (int k = 0; k < t->n; k++) ast_push(raw, (tree_t *) t->c[k]);
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$pl_ioarg", (tree_t *) pl_atom_goal(io[i].k), (tree_t *) t->c[t->n - 1]), raw), γnext, ωfail, entry_out);
                }
            }
            if (!strcmp(nm, "current_prolog_flag") && t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && !pl_user_defines(nm, 2)) {
                extern const char *rt_pl_flag_name(int);
                tree_t * names = ast_node_new(TT_MAKELIST);
                tree_t * alt;
                for (int i = 0; i < 128; i++) { const char * fnm = rt_pl_flag_name(i); if (!fnm) break; if (fnm[0] != '$') ast_push(names, (tree_t *) pl_atom_goal(fnm)); }
                alt = names->n ? pl_cc_fnc2("$flag_member", (tree_t *) t->c[0], names) : (tree_t *) 0;
                if (alt)
                    return goal(cx,
                    pl_cc_fnc2(",", pl_cc_ite(pl_cc_fnc1("var", (tree_t *) t->c[0]), alt, (tree_t *) pl_atom_goal("true")), pl_cc_fnc2("$current_prolog_flag", (tree_t *) t->c[0], (tree_t *) t->c[1])),
                    γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "current_op") && t->n == 3 && !pl_user_defines(nm, 3))
                return goal(cx,
                pl_cc_fnc2(",", pl_cc_fnc3("$pl_op_check", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]),
                pl_cc_gen2_desc("$pl_op_count", "$pl_op_nth", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2])), γnext, ωfail, entry_out);
            if (!strcmp(nm, "current_stream") && t->n == 3 && !pl_user_defines(nm, 3))
                return goal(cx, pl_cc_gen2("$pl_cs_count", "$pl_cs_nth", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]), γnext, ωfail, entry_out);
            if (!strcmp(nm, "stream_property") && t->n == 2 && !pl_user_defines(nm, 2)) {
                tree_t * s = (tree_t *) t->c[0];
                tree_t * p = (tree_t *) t->c[1];
                tree_t * a = pl_cc_freshvar();
                tree_t * named = pl_cc_fnc2(",", pl_cc_fnc1("nonvar", p), pl_cc_fnc2(",", pl_cc_fnc2("=", p, pl_cc_fnc1("alias", a)), pl_cc_fnc1("atom", a)));
                tree_t * bound = pl_cc_fnc2(";", pl_cc_fnc2(",", pl_cc_fnc1("nonvar", s), pl_cc_fnc1("nonvar", p)), named);
                {
                    tree_t * v = pl_cc_freshvar();
                    tree_t * multi =
                        pl_cc_fnc2(",", pl_cc_fnc2(";", pl_cc_fnc2("=", p, pl_cc_fnc1("alias", v)), pl_cc_fnc2("=", p, pl_cc_fnc1("mirror", v))),
                        pl_cc_fnc2(",", pl_cc_fnc1("var", v), pl_cc_fnc3("$gnu_os", (tree_t *) pl_atom_goal("sp_multi"), s, pl_cc_freshvar())));
                    bound = pl_cc_fnc2(";", pl_cc_fnc2(",", pl_cc_fnc2(",", pl_cc_fnc1("nonvar", s), pl_cc_fnc1("nonvar", p)), pl_cc_fnc1("\\+", multi)), named);
                }
                tree_t * once = pl_cc_ite(pl_cc_gen2("$pl_sp_count", "$pl_sp_nth", s, p, (tree_t *) 0), (tree_t *) pl_atom_goal("true"), (tree_t *) pl_atom_goal("fail"));
                tree_t * all = pl_cc_gen2("$pl_sp_count", "$pl_sp_nth", s, p, (tree_t *) 0);
                return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$pl_sp_check", s, p), pl_cc_ite(bound, once, all)), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "predicate_property") && t->n == 2 && !pl_user_defines(nm, 2)) {
                const tree_t * h = t->c[0];
                if (t->c[1] && t->c[1]->t == TT_FNC && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "imported_from") && t->c[1]->n == 1) {
                    int qh = h && h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2 && h->c[0] && h->c[1];
                    return goal(cx, pl_cc_fnc3("$pl_imported_from", qh ? (tree_t *) h->c[0] : (tree_t *) pl_atom_goal("user"), qh ? (tree_t *) h->c[1] : (tree_t *) h, (tree_t *) t->c[1]->c[0]),
                        γnext, ωfail, entry_out);
                }
                if (h && h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2 && h->c[0] && h->c[1]) {
                    tree_t * pg = pl_cc_fnc2("predicate_property", (tree_t *) h->c[1], (tree_t *) t->c[1]);
                    int lm = h->c[0]->t == TT_QLIT || h->c[0]->t == TT_NAME;
                    if (lm && g_stage2_pl.pl_dialect == PL_DIALECT_SWI && h->c[0]->v.sval && strcmp(h->c[0]->v.sval, "user")) {
                        tree_t * qh = pl_cc_freshvar();
                        tree_t * qg = pl_cc_ite(pl_cc_fnc3("$mod_q1", (tree_t *) h->c[0], (tree_t *) h->c[1], qh), pl_cc_fnc2("predicate_property", qh, (tree_t *) t->c[1]), pg);
                        return goal(cx, pl_cc_fnc2(",", pl_cc_fnc1("$pl_pp_enum", (tree_t *) h), qg), γnext, ωfail, entry_out);
                    }
                    if (lm) return goal(cx, pg, γnext, ωfail, entry_out);
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc1("ignore", pl_cc_fnc2("=", (tree_t *) h->c[0], (tree_t *) pl_atom_goal("user"))), pg), γnext, ωfail, entry_out);
                }
                const char * pn = (h && (h->t == TT_FNC || h->t == TT_QLIT || h->t == TT_NAME)) ? h->v.sval : (const char *) 0;
                int ar = (h && h->t == TT_FNC) ? h->n : 0;
                const tree_t * pp = t->c[1];
                if (pl_tree_number(h)) return goal(cx, (g_stage2_pl.pl_dialect == PL_DIALECT_SWI) ? (tree_t *) pl_atom_goal("fail") : pl_cc_type_error("callable", h, nm, 2), γnext, ωfail, entry_out);
                if (pp && (pp->t == TT_QLIT || pp->t == TT_NAME || pp->t == TT_FNC) && pp->v.sval && g_stage2_pl.pl_dialect != PL_DIALECT_SWI && !pl_name_in(pp->v.sval, pl_pred_props))
                    return goal(cx, pl_cc_throw_ar(pl_cc_fnc2("domain_error", (tree_t *) pl_atom_goal("predicate_property"), (tree_t *) pp), nm, 2), γnext, ωfail, entry_out);
                if (pp && (pp->t == TT_QLIT || pp->t == TT_NAME) && pp->v.sval && (!strcmp(pp->v.sval, "public") || !strcmp(pp->v.sval, "det")))
                    return goal(cx, pl_cc_fnc2("$pl_decl_flag", (tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
                if (!pn || ((g_stage2_pl.pl_dialect == PL_DIALECT_GNU) && !(!strcmp(pn, "portray") && ar == 1))) {
                    int dyn0 = pn && ((pl_dyn_index(pn, ar) >= 0) || pl_decl_dyn_is(pn, ar) || pl_rt_is_dynamic(pn, ar));
                    int def0 = pn && (dyn0 || pl_file_defines(pn, ar));
                    tree_t * tm0 = (pn && !def0) ? pl_meta_builtin_tree(pn, ar) : (tree_t *) 0;
                    const tree_t * um0 = (pn && def0) ? pl_decl_meta_of(pn, ar) : (const tree_t *) 0;
                    if (um0) {
                        tm0 = ast_node_new(TT_FNC);
                        tm0->v.sval = (char *) pn;
                        for (int i = 0; i < ar; i++) {
                            const tree_t * a = um0->c[i];
                            ast_push(tm0, (a && (a->t == TT_NAME || a->t == TT_QLIT) && a->v.sval && !strcmp(a->v.sval, "*")) ? (tree_t *) pl_atom_goal("?") : (tree_t *) a);
                        }
                    }
                    if (!tm0) return goal(cx, pl_pp_runtime(h, pp), γnext, ωfail, entry_out);
                    return goal(cx, pl_cc_fnc2(";", pl_pp_runtime(h, pp), pl_cc_fnc2("=", (tree_t *) t->c[1], pl_cc_fnc1("meta_predicate", tm0))), γnext, ωfail, entry_out);
                }
                {
                    int dyn = (pl_dyn_index(pn, ar) >= 0) || pl_decl_dyn_is(pn, ar) || pl_rt_is_dynamic(pn, ar);
                    int def = dyn || pl_file_defines(pn, ar);
                    tree_t * meta = def ? (tree_t *) 0 : pl_meta_builtin_tree(pn, ar);
                    int nonpred = (g_stage2_pl.pl_dialect == PL_DIALECT_SWI) && (!strcmp(pn, "?-") || !strcmp(pn, ":-") || !strcmp(pn, "|"));
                    int bi = !def && !nonpred && (meta || pl_det_leaf_name_wired(pn) || pl_rung_of(pn) != 0);
                    const tree_t * props[16];
                    const char * pf = (def && !dyn) ? pl_pred_file(pn, ar) : (const char *) 0;
                    char rp[PATH_MAX];
                    int np = 0;
                    const tree_t * um = def ? pl_decl_meta_of(pn, ar) : (const tree_t *) 0;
                    int portray1 = !strcmp(pn, "portray") && ar == 1;
                    const tree_t * props_pt[2];
                    int npt = 0;
                    if (portray1) { if (!dyn) props_pt[npt++] = pl_atom_goal("dynamic"); props_pt[npt++] = pl_atom_goal("multifile"); }
                    if (dyn) props[np++] = pl_atom_goal("dynamic");
                    else if (def && !portray1) props[np++] = pl_atom_goal("static");
                    if (pf && pl_file_key_path(pf, rp)) props[np++] = pl_cc_fnc1("file", (tree_t *) pl_atom_goal(lp_strdup(rp)));
                    if (def) props[np++] = pl_atom_goal("defined");
                    if (bi) { props[np++] = pl_atom_goal("built_in"); props[np++] = pl_atom_goal("defined"); props[np++] = pl_atom_goal("static"); }
                    if (def || bi) props[np++] = pl_atom_goal("visible");
                    if ((def || bi) && pl_pi_is_iso(pn, ar)) props[np++] = pl_atom_goal("iso");
                    if (!portray1 && pl_decl_mf_is(pn, ar)) props[np++] = pl_atom_goal("multifile");
                    if (meta) props[np++] = pl_cc_fnc1("meta_predicate", meta);
                    if (um) {
                        tree_t * tm = ast_node_new(TT_FNC);
                        tm->v.sval = (char *) pn;
                        for (int i = 0; i < ar; i++) {
                            const tree_t * a = um->c[i];
                            ast_push(tm, (a && (a->t == TT_NAME || a->t == TT_QLIT) && a->v.sval && !strcmp(a->v.sval, "*")) ? (tree_t *) pl_atom_goal("?") : (tree_t *) a);
                        }
                        props[np++] = pl_cc_fnc1("meta_predicate", tm);
                    }
                    for (int i = 0; i < npt && np < 16; i++) props[np++] = props_pt[i];
                    if (!np) return goal(cx, pl_pp_runtime(h, pp), γnext, ωfail, entry_out);
                    {
                        tree_t * alt = pl_cc_fnc2("=", (tree_t *) t->c[1], (tree_t *) props[np - 1]);
                        for (int i = np - 2; i >= 0; i--) alt = pl_cc_fnc2(";", pl_cc_fnc2("=", (tree_t *) t->c[1], (tree_t *) props[i]), alt);
                        if (def) alt = pl_cc_fnc2(";", alt, pl_cc_fnc2(",", pl_cc_fnc2("=", (tree_t *) t->c[1], pl_cc_fnc1("number_of_clauses", pl_cc_freshvar())), pl_pp_runtime(h, pp)));
                        return goal(cx, alt, γnext, ωfail, entry_out);
                    }
                }
            }
            if (!strcmp(nm, "current_predicate") && t->n == 1 && !pl_user_defines(nm, 1)) {
                tree_t * s = (tree_t *) t->c[0];
                tree_t * n;
                tree_t * a;
                tree_t * body;
                if (s && s->t == TT_FNC && s->v.sval && !strcmp(s->v.sval, ":") && s->n == 2) return goal(cx, pl_cc_fnc2("$cp_mq", s->c[0], s->c[1]), γnext, ωfail, entry_out);
                if (s && s->t == TT_FNC && s->v.sval && !strcmp(s->v.sval, "//") && s->n == 2 && s->c[1]) {
                    if (s->c[1]->t == TT_ILIT) return goal(cx, pl_cc_fnc1("current_predicate", pl_cc_fnc2("/", s->c[0], pl_cc_ilit((int) s->c[1]->v.ival + 2))), γnext, ωfail, entry_out);
                    if (s->c[1]->t == TT_VAR) {
                        tree_t * a2 = pl_cc_freshvar();
                        tree_t * a2b = pl_cc_freshvar();
                        a2b->v.ival = a2->v.ival;
                        tree_t * both =
                            pl_cc_fnc2(",", pl_cc_fnc1("current_predicate", pl_cc_fnc2("/", s->c[0], a2)),
                            pl_cc_fnc2(",", pl_cc_fnc2("is", s->c[1], pl_cc_fnc2("-", a2b, pl_cc_ilit(2))), pl_cc_fnc2(">=", s->c[1], pl_cc_ilit(0))));
                        return goal(cx, both, γnext, ωfail, entry_out);
                    }
                }
                { tree_t * bad = pl_cc_spec_ill_typed(s, nm); if (bad) return goal(cx, bad, γnext, ωfail, entry_out); }
                if (s && s->t == TT_FNC && s->v.sval && !strcmp(s->v.sval, "/") && s->n == 2) {
                    n = (tree_t *) s->c[0];
                    a = (tree_t *) s->c[1];
                    body = pl_cc_gen2("$pl_cp_count", "$pl_cp_nth", n, a, (tree_t *) 0);
                } else {
                    tree_t * n2;
                    tree_t * a2;
                    n = pl_cc_freshvar();
                    a = pl_cc_freshvar();
                    n2 = pl_cc_freshvar();
                    a2 = pl_cc_freshvar();
                    n2->v.ival = n->v.ival;
                    a2->v.ival = a->v.ival;
                    body = pl_cc_fnc2(",", pl_cc_fnc2("=", s, pl_cc_fnc2("/", n2, a2)), pl_cc_gen2("$pl_cp_count", "$pl_cp_nth", n, a, (tree_t *) 0));
                    {
                        tree_t * m = pl_cc_freshvar();
                        tree_t * p = pl_cc_freshvar();
                        tree_t * m2 = pl_cc_freshvar();
                        tree_t * p2 = pl_cc_freshvar();
                        m2->v.ival = m->v.ival;
                        p2->v.ival = p->v.ival;
                        tree_t * qualified = pl_cc_fnc2(",", pl_cc_fnc1("nonvar", s), pl_cc_fnc2("=", s, pl_cc_fnc2(":", m, p)));
                        tree_t * dn = pl_cc_freshvar();
                        tree_t * da = pl_cc_freshvar();
                        tree_t * dn2 = pl_cc_freshvar();
                        tree_t * da2 = pl_cc_freshvar();
                        tree_t * da3 = pl_cc_freshvar();
                        tree_t * da1 = pl_cc_freshvar();
                        tree_t * da1b = pl_cc_freshvar();
                        dn2->v.ival = dn->v.ival;
                        da2->v.ival = da->v.ival;
                        da3->v.ival = da->v.ival;
                        da1b->v.ival = da1->v.ival;
                        tree_t * dcg = pl_cc_fnc2(",", pl_cc_fnc1("nonvar", s), pl_cc_fnc2("=", s, pl_cc_fnc2("//", dn, da)));
                        tree_t * dcg_then =
                            pl_cc_fnc2(",", pl_cc_fnc1("current_predicate", pl_cc_fnc2("/", dn2, da1)),
                            pl_cc_fnc2(",", pl_cc_fnc2("is", da2, pl_cc_fnc2("-", da1b, pl_cc_ilit(2))), pl_cc_fnc2(">=", da3, pl_cc_ilit(0))));
                        tree_t * plain = pl_cc_fnc2(",", pl_cc_fnc1("$pl_cp_guard", s), body);
                        return goal(cx, pl_cc_ite(qualified, pl_cc_fnc2("$cp_mq", m2, p2), pl_cc_ite(dcg, dcg_then, plain)), γnext, ωfail, entry_out);
                    }
                }
                return goal(cx, pl_cc_fnc2(",", pl_cc_fnc1("$pl_cp_guard", s), body), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "dynamic") && t->n >= 1) {
                tree_t * g = pl_cc_fnc1("$pl_dynamic", (tree_t *) t->c[0]);
                for (int i = 1; i < t->n; i++) g = pl_cc_fnc2(",", g, pl_cc_fnc1("$pl_dynamic", (tree_t *) t->c[i]));
                return goal(cx, g, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "char_conversion") && t->n == 2) {
                (void) pl_dyn_index_or_add("$pl_cconv", 2);
                pl_dyn_rt_declare("$pl_cconv", 2);
                tree_t * in = (tree_t *) t->c[0];
                tree_t * out = (tree_t *) t->c[1];
                tree_t * put = pl_cc_ite(pl_cc_fnc2("==", in, out), (tree_t *) pl_atom_goal("true"), pl_cc_fnc1("asserta", pl_cc_fnc2("$pl_cconv", in, out)));
                tree_t * store = pl_cc_fnc2(",", pl_cc_fnc1("retractall", pl_cc_fnc2("$pl_cconv", in, pl_cc_freshvar())), put);
                tree_t * out_ck = pl_cc_ite(pl_cc_ischar(out), store, pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("character"), out), "char_conversion"));
                tree_t * out_gate = pl_cc_ite(pl_cc_fnc1("var", out), pl_cc_throw((tree_t *) pl_atom_goal("instantiation_error"), "char_conversion"), out_ck);
                tree_t * in_ck = pl_cc_ite(pl_cc_ischar(in), out_gate, pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("character"), in), "char_conversion"));
                tree_t * rewrite = pl_cc_ite(pl_cc_fnc1("var", in), pl_cc_throw((tree_t *) pl_atom_goal("instantiation_error"), "char_conversion"), in_ck);
                return goal(cx, rewrite, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "current_char_conversion") && t->n == 2) {
                (void) pl_dyn_index_or_add("$pl_cconv", 2);
                pl_dyn_rt_declare("$pl_cconv", 2);
                tree_t * in = (tree_t *) t->c[0];
                tree_t * out = (tree_t *) t->c[1];
                tree_t * lookup = pl_cc_fnc2("$pl_cconv", in, out);
                tree_t * bound_path =
                    pl_cc_ite(pl_cc_ischar(in), pl_cc_ite(lookup, (tree_t *) pl_atom_goal("true"), pl_cc_fnc2("=", out, in)),
                    pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("character"), in), "current_char_conversion"));
                tree_t * rewrite = pl_cc_ite(pl_cc_fnc1("var", in), lookup, bound_path);
                return goal(cx, rewrite, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "char_type") && t->n == 2 && !pl_user_defines(nm, 2)) {
                tree_t * ch = (tree_t *) t->c[0];
                tree_t * ty = (tree_t *) t->c[1];
                tree_t * bound = pl_cc_fnc2(",", pl_cc_fnc1("nonvar", ch), pl_cc_fnc1("nonvar", ty));
                return goal(cx, pl_cc_ite(bound, pl_cc_fnc2("$char_type_d", ch, ty), pl_cc_fnc3("$ctype", ch, ty, (tree_t *) pl_atom_goal("char"))), γnext, ωfail, entry_out);
            }
            {
                const char * ls = pl_det_leaf_sym(nm, t->n);
                if (ls && (g_stage2_pl.pl_dialect == PL_DIALECT_SWI || g_stage2_pl.pl_dialect == PL_DIALECT_XSB) && pl_swi_library_leaf(nm, t->n) && pl_user_file_defines(nm, t->n)) ls = (const char *) 0;
                if (ls && pl_swi_absent_leaf(nm, t->n)) ls = (const char *) 0;
                if (ls && !strcmp(nm, "print") && t->n == 1 && (pl_user_defines("portray", 1) || pl_db_owned("portray", 1))) ls = (const char *) 0;
                if (ls && (pl_user_defines("portray", 1) || pl_db_owned("portray", 1))) {
                    if (!strcmp(nm, "print") && t->n == 2) return goal(cx, pl_cc_print_via_format((tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
                    if (!strcmp(nm, "write_term") && t->n == 2 && pl_opts_are_portray_only(t->c[1]))
                        return goal(cx, pl_cc_print_via_format((tree_t *) 0, (tree_t *) t->c[0]), γnext, ωfail, entry_out);
                    if (!strcmp(nm, "write_term") && t->n == 3 && pl_opts_are_portray_only(t->c[2]))
                        return goal(cx, pl_cc_print_via_format((tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
                }
                if (ls) {
                    const char * gs = pl_anum_guard_sym(nm, t->n);
                    IR_t * ne = NULL;
                    IR_t * nd = gs ? pl_leaf_lv_guarded(cx, ls, gs, nm, t, t->n, γnext, ωfail, &ne) : pl_leaf_lv(cx, ls, t, t->n, γnext, ωfail, &ne);
                    if (pl_curout_writer(nm, t->n)) return pl_curout_text_guard(cx, nd, ne, ωfail, entry_out);
                    if (entry_out) *entry_out = ne;
                    return nd;
                }
            }
            if (!strcmp(nm, "use_module") && (t->n == 1 || t->n == 2) && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "library") && t->c[0]->n == 1 &&
                !pl_user_defines(nm, t->n)) {
                extern int pl_prelude_lib_spec_known(const tree_t *);
                int k = pl_prelude_lib_spec_known(t->c[0]->c[0]);
                tree_t * ee = pl_cc_fnc2("existence_error", (tree_t *) pl_atom_goal("source_sink"), pl_tree_copy(t->c[0]));
                if (k >= 0) return goal(cx, k ? (tree_t *) pl_atom_goal("true") : pl_cc_throw_ar(ee, nm, t->n), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "exists_source") && t->n == 1 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "library") && t->c[0]->n == 1 &&
                !pl_user_file_defines(nm, 1)) {
                extern int pl_prelude_lib_spec_known(const tree_t *);
                int k = pl_prelude_lib_spec_known(t->c[0]->c[0]);
                if (k >= 0) return goal(cx, k ? (tree_t *) pl_atom_goal("true") : (tree_t *) pl_atom_goal("fail"), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "use_module") && (t->n == 1 || t->n == 2) && t->c[0] && t->c[0]->t == TT_VAR && !pl_user_defines(nm, t->n))
                return goal(cx, (tree_t *) pl_atom_goal("true"), γnext, ωfail, entry_out);
            if (!strcmp(nm, "ensure_loaded") && t->n == 1 && !pl_user_defines(nm, 1)) {
                tree_t * o = ast_node_new(TT_MAKELIST);
                o->v.ival = 0;
                ast_push(o, pl_cc_fnc1("if", (tree_t *) pl_atom_goal("not_loaded")));
                return goal(cx, pl_cc_fnc2("load_files", (tree_t *) t->c[0], o), γnext, ωfail, entry_out);
            }
            { int r = pl_rung_of(nm); if (r && !g_runtime.eval.rt_fragment_emit && !g_stage2.rt_lowering && !pl_user_defines(nm, t->n) && !pl_det_leaf_name_wired(nm)) pl_refuse("builtin", nm, r); }
            if (pl_hook_dyn_is(nm, t->n)) return goal(cx, pl_cc_fnc1("$tbl_dcall", (tree_t *) t), γnext, ωfail, entry_out);
            return pl_user_call(cx, nm, t, t->n, γnext, ωfail, entry_out);
        }
        case TT_QLIT:
        case TT_NAME:
        {
            const char * nm = t->v.sval ? t->v.sval : "?";
            if (!strcmp(nm, "true")) return lpl_build(cx, IR_SUCCEED, γnext, ωfail);
            if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return lpl_build(cx, IR_GOTO, ωfail, ωfail);
            if (!strcmp(nm, "$wake_pending")) { IR_t * wk = lpl_build(cx, IR_CLEANUP, γnext, ωfail); IR_LIT(wk).ival = CLEANUP_WAKE; return wk; }
            if (!strcmp(nm, "$undo_pending")) { IR_t * uk = lpl_build(cx, IR_CLEANUP, γnext, ωfail); IR_LIT(uk).ival = CLEANUP_UNDO; return uk; }
            if (!strcmp(nm, "$tbl_hdr")) return lpl_build(cx, IR_TABLE_HDR, γnext, ωfail);
            if (!strcmp(nm, "nl")) {
                IR_t * ne = NULL;
                IR_t * nd = pl_leaf(cx, "$nl", t, 0, γnext, ωfail, &ne);
                IR_t * ge = NULL;
                IR_t * g = goal(cx, pl_cc_fnc2("$pl_ioarg", (tree_t *) pl_atom_goal("put_nl1"), (tree_t *) pl_atom_goal("[]")), ne ? ne : nd, ωfail, &ge);
                if (entry_out) *entry_out = ge ? ge : g;
                return nd;
            }
            if (!strcmp(nm, "repeat") && !pl_user_defines(nm, 0))
                return goal(cx, pl_between_own(pl_cc_fnc3("between", pl_cc_ilit(1), pl_cc_ilit(9223372036854775807LL), pl_cc_freshvar())), γnext, ωfail, entry_out);
            if (!strcmp(nm, "!") || (!strcmp(nm, "$") && !pl_dollar_user(0))) {
                IR_t * cn = lpl_build(cx, IR_CUT, γnext, cx->cutω);
                if (cx->cutω != cx->clause_cutω) IR_LIT(cn).ival = 1;
                if (entry_out) *entry_out = pl_probe_before(cx, cn, IR_LIT(cn).ival ? cx->cut_mark : (IR_t *) 0, IR_LIT(cn).ival != 0);
                return cn;
            }
            { const char * ls = pl_det_leaf_sym(nm, 0); if (ls) return pl_leaf_lv(cx, ls, t, 0, γnext, ωfail, entry_out); }
            { int r = pl_rung_of(nm); if (r && !g_runtime.eval.rt_fragment_emit && !g_stage2.rt_lowering && !pl_user_defines(nm, 0) && !pl_det_leaf_name_wired(nm)) pl_refuse("builtin", nm, r); }
            if (pl_hook_dyn_is(nm, 0)) return goal(cx, pl_cc_fnc1("$tbl_dcall", (tree_t *) t), γnext, ωfail, entry_out);
            return pl_user_call(cx, nm, t, 0, γnext, ωfail, entry_out);
        }
        case TT_CUT:
        {
            IR_t * cn = lpl_build(cx, IR_CUT, γnext, cx->cutω);
            IR_LIT(cn).ival = cx->cut_scope;
            if (entry_out) *entry_out = pl_probe_before(cx, cn, IR_LIT(cn).ival ? cx->cut_mark : (IR_t *) 0, IR_LIT(cn).ival != 0);
            return cn;
        }
        case TT_UNIFY:
        { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
        case TT_IF:
        return pl_lower_ite(cx, t->c[0], (t->n > 1) ? t->c[1] : NULL, (t->n > 2) ? t->c[2] : NULL, γnext, ωfail, entry_out);
        case TT_PROGRAM:
        return pl_lower_conj(cx, (const tree_t * const *) t->c, t->n, γnext, ωfail, entry_out, NULL, NULL);
        case TT_VAR:
        if (pl_dl_ctl_leaf_scope(cx)) return goal(cx, pl_dl_leaf_tree((tree_t *) t), γnext, ωfail, entry_out);
        return pl_meta_call_dyn(cx, t, NULL, 0, γnext, ωfail, entry_out);
        default:
        return lpl_build(cx, IR_SUCCEED, γnext, ωfail);
    }
}
