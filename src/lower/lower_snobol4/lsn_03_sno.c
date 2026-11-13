#include "lower/lower_snobol4/lsn_head.h"
#include "runtime/core/g_runtime_eval.h"
#include <stdlib.h>
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_snobol4/lsn_01_sno.h"
#include "lower/lower_snobol4/lsn_02_sno.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_04_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_06_sh.h"
#include "lower/lower_snobol4/lsn_07_sno.h"
#include "lower/lower_snobol4/lsn_09_sno.h"
static IR_t * sco_stmt_hook(scx_t * cx, const tree_t * s, IR_t * body);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_fatal(const char * what, const char * detail) {
    fprintf(stderr, "FATAL lower_snobol4 (GZ#5 subset): %s%s%s. Pattern matching, EVAL and CODE are outside the landed subset (IR_MATCH_* family pending); see GOAL-SNOBOL4-BB.md.\n", what,
        detail ? ": " : "", detail ? detail : "");
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * sfind(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) { const tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zw5_on(void) { const char * e = getenv("SCRIP_ZW5"); return (e && *e == '0') ? 0 : 1; }
const char * sfind_str(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0 && a->c[0]) ? a->c[0]->v.sval : NULL; }
tree_t * sfind_expr(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0) ? a->c[0] : NULL; }
void sno_reg_var(const char * nm) { extern void global_register_copy(const char *); if (nm && nm[0] && nm[0] != '&' && !(nm[0] == '*' && (unsigned char) nm[1] == 1)) global_register_copy(nm); }
IR_t * sco_branch(scx_t * cx, const tree_t * pg, IR_t * γ, IR_t * ω) {
    if (!pg) return γ;
    if (pg->t != TT_PROGRAM) { IR_t * r = NULL; return sx_lower(cx, pg, γ, ω, &r); }
    IR_t * entry = γ;
    for (int i = pg->n - 1; i >= 0; i--) {
        const tree_t * s = pg->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t * subj = lc_stmt_subj(s);
        if (!subj) continue;
        if (sfind(s, ":eq") && !sfind(s, ":pat")) { tree_t * a = ast_node_new(TT_ASSIGN); ast_push(a, (tree_t *) subj); ast_push(a, sfind_expr(s, ":repl")); subj = a; }
        IR_t * r = NULL;
        IR_t * tj = lc_build(cx->g, IR_SETEXIT_TEST, entry, entry);
        entry = sco_stmt_hook(cx, s, sx_lower(cx, subj, tj, tj, &r));
    }
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sgoto(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind) continue;
        if (a->n > 0 && a->c[0] && a->c[0]->t == TT_QLIT && a->c[0]->v.sval) return a->c[0]->v.sval;
        if (a->n > 0 && a->c[0] && a->c[0]->t == TT_INDIRECT && a->c[0]->n > 0 && a->c[0]->c[0] && a->c[0]->c[0]->t == TT_VAR && a->c[0]->c[0]->v.sval) {
            const char * v = a->c[0]->c[0]->v.sval;
            sno_reg_var(v);
            size_t ln = strlen(v);
            char * o = (char *) ct_alloc(ln + 2);
            o[0] = '$';
            memcpy(o + 1, v, ln);
            o[ln + 1] = 0;
            return o;
        }
        return NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_stmt_is_blank(const tree_t * s) {
    if (!s) return 1;
    if (lp_s_expr(s, ":subj") || lp_s_expr(s, ":lbl") || lp_s_expr(s, ":pat") || lp_s_expr(s, ":repl") || lp_s_expr(s, ":end")) return 0;
    for (int k = 0; k < s->n; k++) { const tree_t * c = s->c[k]; if (c && (c->t == TT_GOTO_U || c->t == TT_GOTO_S || c->t == TT_GOTO_F || c->t == TT_GOTO_DIRECT)) return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sco_stmt_hook(scx_t * cx, const tree_t * s, IR_t * body) {
    if (!s || !body || lp_s_int(s, ":nocount") || sno_stmt_is_blank(s)) return body;
    long stno = cx->stno_base + (long) lp_s_int(s, ":stno");
    long line = (long) lp_s_int(s, ":line");
    if (!line) line = (long) lp_s_int(s, ":lline");
    if (g_lower.sno.uses_stmtkw) {
        IR_t * tj = lc_build(cx->g, IR_SETEXIT_TEST, body, body);
        IR_t * hook = lc_build(cx->g, IR_STMT_MARK, tj, tj);
        IR_LIT(hook).ival = (int64_t) stno;
        hook->pat_static = (int) line;
        hook->seal = IR_SEAL_STMT_HOOK;
        return hook;
    }
    return body;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_special_chain(IR_graph_t * g, const char * dn, IR_t * tail) {
    if (g_lower.sno.sno_goto_special_chain_sp < 0) { const char * e = getenv("SCRIP_GOTO_SPECIAL_TRANSFER"); g_lower.sno.sno_goto_special_chain_sp = (e && *e == '0') ? 0 : 1; }
    if (!g_lower.sno.sno_goto_special_chain_sp || !dn || (dn[0] != '$' && dn[0] != '@')) return tail;
    static const char sc[3] = { 'N', 'F', 'R' };
    static const char * const sn[3] = { "NRETURN", "FRETURN", "RETURN" };
    size_t dl = strlen(dn);
    IR_t * head = tail;
    for (int k = 0; k < 3; k++) {
        IR_t * land = bb_label_landing(sn[k]);
        if (!land) continue;
        char * te = (char *) ct_alloc(dl + 3);
        te[0] = '^';
        te[1] = sc[k];
        memcpy(te + 2, dn, dl + 1);
        IR_t * t = lc_build(g, IR_GOTO_DEFERRED, land, head);
        IR_LIT(t).sval = te;
        head = t;
    }
    return head;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_label_reserved(const char * nm) {
    static const char * const rs[7] = { "RETURN", "FRETURN", "NRETURN", "END", "CONTINUE", "SCONTINUE", "ABORT" };
    for (int k = 0; k < 7; k++) if (!strcmp(nm, rs[k])) return 1;
    return 0;
}
static IR_t * sno_label_trace_wrap(IR_graph_t * g, const char * nm, IR_t * land) {
    if (!g_lower.sno.traces_a_label || !nm || !nm[0] || !land) return land;
    if (sno_label_reserved(nm)) return land;
    IR_t * hook = lc_build(g, IR_STMT_MARK, land, land);
    hook->seal = IR_SEAL_LABEL_HOOK;
    IR_LIT(hook).sval = lp_strdup(nm);
    hook->pat_static = 0;
    bb_src_note(hook, "sno_label_hook", 0);
    return hook;
}
static IR_t * sno_goto_target(IR_graph_t * g, const char * nm, IR_t * exitnd) {
    IR_t * l = (nm && nm[0] != '$') ? bb_label_landing(nm) : NULL;
    if (l && (g_rt_eval.rt_fragment_emit || g_lower.sno.calls_code) && !sno_label_reserved(nm)) l = NULL;
    if (l) return l;
    if (!nm || !nm[0]) sno_fatal("goto to unknown label", "?");
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL);
    IR_LIT(gd).sval = lp_strdup(nm);
    return sno_goto_special_chain(g, IR_LIT(gd).sval, gd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_computed_target(IR_graph_t * g, scx_t * cx, const tree_t * expr, IR_t * exitnd) {
    if (g_lower.sno.sno_goto_computed_target_bn < 0) { const char * e = getenv("SCRIP_GOTO_CALL_BYNAME"); g_lower.sno.sno_goto_computed_target_bn = (e && *e == '0') ? 0 : 1; }
    int by_name = g_lower.sno.sno_goto_computed_target_bn && expr && expr->t == TT_FNC;
    char nmb[24];
    snprintf(nmb, sizeof nmb, "IGT$%d", g_lower.sno.sno_goto_computed_target_igt_n++);
    char * tmpn = lp_strdup(nmb);
    sno_reg_var(tmpn);
    size_t ln = strlen(tmpn);
    char * dn = (char *) ct_alloc(ln + 2);
    dn[0] = by_name ? '@' : '$';
    memcpy(dn + 1, tmpn, ln);
    dn[ln + 1] = 0;
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL);
    IR_LIT(gd).sval = dn;
    IR_t * chain = sno_goto_specials_impossible(expr) ? gd : sno_goto_special_chain(g, dn, gd);
    IR_t * asn = lc_build(g, IR_ASSIGN, chain, gd);
    IR_LIT(asn).sval = tmpn;
    IR_t * vr = NULL;
    IR_t * ec = by_name ? sno_lower_byname(cx, expr, asn, gd, &vr) : sx_lower(cx, expr, asn, gd, &vr);
    ir_operand_push(asn, vr);
    return ec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_direct_target(IR_graph_t * g, scx_t * cx, const tree_t * expr, IR_t * exitnd, IR_t ** gdp, int retake) {
    char nmb[24];
    snprintf(nmb, sizeof nmb, "DGT$%d", g_lower.sno.sno_goto_direct_target_dgt_n++);
    char * tmpn = lp_strdup(nmb);
    sno_reg_var(tmpn);
    size_t ln = strlen(tmpn);
    char * dn = (char *) ct_alloc(ln + 3);
    dn[0] = '<';
    if (retake) dn[1] = '!';
    memcpy(dn + 1 + retake, tmpn, ln);
    dn[ln + 1 + retake] = 0;
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL);
    if (gdp) *gdp = gd;
    IR_LIT(gd).sval = dn;
    IR_t * asn = lc_build(g, IR_ASSIGN, gd, gd);
    IR_LIT(asn).sval = tmpn;
    IR_t * vr = NULL;
    IR_t * ec = sx_lower(cx, expr, asn, gd, &vr);
    ir_operand_push(asn, vr);
    return ec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_branch(IR_graph_t * g, scx_t * cx, const char * nm, const tree_t * dc, const tree_t * ex, IR_t * exitnd, IR_t ** gdp) {
    if (gdp) *gdp = NULL;
    if (nm) return sno_goto_target(g, nm, exitnd);
    if (dc) return sno_goto_direct_target(g, cx, dc, exitnd, gdp, 0);
    if (ex) return sno_goto_computed_target(g, cx, ex, exitnd);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_qlit_fold(const tree_t * t) {
    if (!t) return NULL;
    if (t->t == TT_QLIT) return t->v.sval ? t->v.sval : "";
    if ((t->t == TT_CAT || t->t == TT_SEQ) && t->n >= 2) {
        const char * a = sno_qlit_fold(t->c[0]);
        if (!a) return NULL;
        const char * b = sno_qlit_fold(t->c[1]);
        if (!b) return NULL;
        size_t la = strlen(a), lb = strlen(b);
        char * o = (char *) ct_alloc(la + lb + 1);
        memcpy(o, a, la);
        memcpy(o + la, b, lb);
        o[la + lb] = 0;
        return o;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * sno_stmt_define(const tree_t * s, int * out_argbase) {
    const tree_t * subj = lc_stmt_subj(s);
    if (subj && subj->t == TT_DEFINE && subj->n > 1 && subj->c[1] && subj->c[1]->t == TT_QLIT && subj->c[1]->v.sval) { if (out_argbase) *out_argbase = 1; return subj; }
    if (!subj || subj->t != TT_FNC) return NULL;
    const char * name = subj->v.sval;
    int argbase = 0;
    if (!name && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR) { name = subj->c[0]->v.sval; argbase = 1; }
    if (!name || strcmp(name, "DEFINE")) return NULL;
    if (sfind(s, ":eq") || sfind_expr(s, ":pat")) sno_fatal("DEFINE with a pattern or replacement field is outside the landed subset", NULL);
    if (subj->n <= argbase || !subj->c[argbase] || !sno_qlit_fold(subj->c[argbase])) return NULL;
    if (sno_define_entry_computed(subj, argbase)) return NULL;
    if (out_argbase) *out_argbase = argbase;
    return subj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name, long stno_base, long end_line, long code_line) {
    IR_graph_t * g = IR_alloc(nst * 16 + 256);
    scx_t cx;
    cx.g = g;
    cx.wn_tree = NULL;
    cx.kw_chain = NULL;
    cx.loop_exit = NULL;
    cx.loop_next = NULL;
    cx.result_name = result_name;
    cx.pat_fail = NULL;
    cx.pat_seal = NULL;
    cx.npre = 0;
    cx.snap = 0;
    cx.capro = NULL;
    cx.capro_cand = NULL;
    cx.ppos = 0;
    cx.prog_nstmt = (long)nst + 1 + stno_base;
    cx.prog_endline = code_line;
    cx.stno_base = stno_base;
    cx.seq_rlast = NULL;
    cx.seq_rlast_for = NULL;
    cx.seq_rtail = NULL;
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t ** anchor = (IR_t **) ct_zalloc((size_t) nst, sizeof(IR_t *));
    IR_t ** fail_tgt = (IR_t **) ct_zalloc((size_t) nst, sizeof(IR_t *));
    IR_t ** match_land = (IR_t **) ct_zalloc((size_t) nst, sizeof(IR_t *));
    IR_t ** asgn_land = (IR_t **) ct_zalloc((size_t) nst, sizeof(IR_t *));
    bb_label_registry_reset();
    IR_t * endnd = exitnd;
    if (g_lower.sno.uses_stmtkw && end_line > 0) {
        IR_t * ehook = lc_build(g, IR_STMT_MARK, exitnd, exitnd);
        IR_LIT(ehook).ival = (int64_t)(nst + 1) + stno_base;
        ehook->pat_static = (int) end_line;
        ehook->seal = IR_SEAL_STMT_HOOK;
        endnd = ehook;
    }
    for (int i = 0; i < nst; i++) {
        anchor[i] = lc_build(g, IR_GOTO, NULL, NULL);
        {
            const tree_t * _sa = sfind(st[i], ":stno");
            if (_sa && _sa->n > 0 && _sa->c[0]) {
                const tree_t * _c = _sa->c[0];
                IR_LIT(anchor[i]).ival = stno_base + ((_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0));
            }
        }
        const char * lbl = sfind_str(st[i], ":lbl");
        if (lbl && lbl[0]) bb_label_registry_add(lp_strdup(lbl), anchor[i]);
    }
    bb_label_registry_add(lp_strdup("END"), endnd);
    if (!result_name) {
        IR_t * rf = lc_build(g, IR_DEFINE_RETURN, exitnd, failnd);
        IR_t * ff = lc_build(g, IR_DEFINE_FRETURN, exitnd, failnd);
        if (!bb_label_landing("RETURN")) bb_label_registry_add(lp_strdup("RETURN"), rf);
        if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), ff);
        if (!bb_label_landing("NRETURN")) {
            IR_t * nrl = lc_build(g, IR_LIT_STRING, NULL, ff);
            IR_LIT(nrl).sval = (char *) "";
            IR_t * nnd = lc_build(g, IR_CALL, rf, ff);
            IR_LIT(nnd).sval = (char *) "SNO$NRET";
            lc_γ_to(nrl, nnd);
            ir_operand_push(nnd, nrl);
            bb_label_registry_add(lp_strdup("NRETURN"), nrl);
        }
    } else {
        if (!bb_label_landing("RETURN")) bb_label_registry_add(lp_strdup("RETURN"), exitnd);
        if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), failnd);
        if (!bb_label_landing("NRETURN")) {
            IR_t * nrl = lc_build(g, IR_LIT_STRING, NULL, failnd);
            IR_LIT(nrl).sval = (char *) "";
            IR_t * nnd = lc_build(g, IR_CALL, exitnd, failnd);
            IR_LIT(nnd).sval = (char *) "SNO$NRET";
            lc_γ_to(nrl, nnd);
            ir_operand_push(nnd, nrl);
            bb_label_registry_add(lp_strdup("NRETURN"), nrl);
        }
    }
    g->entry = (nst > 0) ? anchor[entry_idx] : exitnd;
    int _pro_open = 0, _pro_close = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i];
        if (i == entry_idx) _pro_open = 1;
        if (_pro_close) _pro_open = 0;
        {
            extern void zls_group_mark_anchor(const IR_graph_t *, const char *, const IR_t *);
            const char * mlbl = sfind_str(s, ":lbl");
            if (mlbl && mlbl[0]) zls_group_mark_anchor(g, lp_strdup(mlbl), anchor[i]);
        }
        IR_t * next = (i + 1 < nst) ? anchor[i + 1] : endnd;
        if (sfind(s, ":end")) { lc_γ_to(anchor[i], endnd); continue; }
        const char * goU = sgoto(s, TT_GOTO_U);
        const char * goS = sgoto(s, TT_GOTO_S);
        const char * goF = sgoto(s, TT_GOTO_F);
        const tree_t * exU = goU ? NULL : sgoto_expr(s, TT_GOTO_U);
        const tree_t * exS = goS ? NULL : sgoto_expr(s, TT_GOTO_S);
        const tree_t * exF = goF ? NULL : sgoto_expr(s, TT_GOTO_F);
        const tree_t * dcU = sgoto_direct(s, TT_GOTO_U);
        const tree_t * dcS = sgoto_direct(s, TT_GOTO_S);
        const tree_t * dcF = sgoto_direct(s, TT_GOTO_F);
        IR_t * sGd = NULL, * fGd = NULL;
        IR_t * sT = sno_goto_branch(g, &cx, goS, dcS, exS, exitnd, &sGd);
        const char * sTnm = goS ? goS : (IR_t *)0 == sT ? (const char *)0 : goS;
        if (!sT) { sT = sno_goto_branch(g, &cx, goU, dcU, exU, exitnd, &sGd); sTnm = goU; }
        if (!sT) { sT = next; sTnm = (const char *)0; }
        IR_t * fT = sno_goto_branch(g, &cx, goF, dcF, exF, exitnd, &fGd);
        const char * fTnm = goF;
        const tree_t * fdc = (!goF && dcF) ? dcF : NULL;
        if (!fT) { fT = sno_goto_branch(g, &cx, goU, dcU, exU, exitnd, &fGd); fTnm = goU; fdc = (!goU && dcU) ? dcU : NULL; }
        if (!fT) { fT = next; fTnm = (const char *)0; }
        if (sTnm) sT = sno_label_trace_wrap(g, sTnm, sT);
        if (fTnm) fT = sno_label_trace_wrap(g, fTnm, fT);
        if (fT == next && !goF && !exF && !goU && !exU && sfind(s, ":nofail")) {
            IR_t *nf = lc_build(g, IR_CALL, exitnd, exitnd);
            IR_LIT(nf).sval = (char *)"SNO$NOFAIL";
            bb_src_note(nf, "sno_nofail", 0);
            fT = nf;
        }
        if (!sfind(s, ":evalstmt")) fT = lc_build(g, IR_SETEXIT_TEST, fT, fT);
        if (sGd || fGd) {
            IR_t * rGd = NULL;
            IR_t * rtk = fdc ? sno_goto_direct_target(g, &cx, fdc, exitnd, &rGd, 1) : fT;
            if (rGd) lc_γ_to(rGd, rGd);
            if (sGd) lc_γ_to(sGd, rtk);
            if (fGd) lc_γ_to(fGd, rtk);
        }
        IR_t * stb = zw5_on() ? lc_build(g, IR_STATEMENT_END, sT, fT) : (IR_t *) NULL;
        if (stb) {
            const tree_t * _sa = sfind(st[i], ":stno");
            if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(stb).ival = stno_base + ((_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0)); }
        }
        IR_t * sJ = lc_build(g, IR_GOTO, stb ? stb : sT, NULL);
        fail_tgt[i] = fT;
        IR_t * fJ = lc_build(g, IR_GOTO, fT, NULL);
        IR_t * fA = lc_build(g, IR_GOTO, fJ, NULL);
        asgn_land[i] = fA;
        if (is_def && is_def[i]) {
            {
                int _argbase = 0;
                const tree_t * dsub = sno_stmt_define(s, &_argbase);
                if (dsub && sno_def_entry_absent(dsub, _argbase)) {
                    IR_t * sp = lc_build(g, IR_LIT_STRING, NULL, fA);
                    IR_LIT(sp).sval = lp_strdup(sno_qlit_fold(dsub->c[_argbase]));
                    IR_t * call = lc_build(g, IR_CALL, sJ, fA);
                    IR_LIT(call).sval = (char *) "DEFINE";
                    ir_operand_push(call, sp);
                    IR_t * last = sp;
                    {
                        const char * es = sno_define_entry_opt(dsub, _argbase);
                        if (es) { IR_t * ep = lc_build(g, IR_LIT_STRING, NULL, fA); IR_LIT(ep).sval = lp_strdup(es); ir_operand_push(call, ep); lc_γ_to(last, ep); last = ep; }
                    }
                    lc_γ_to(last, call);
                    lc_γ_to(anchor[i], sp);
                    continue;
                }
                const tree_t * pnode = (dsub && dsub->n > _argbase) ? dsub->c[_argbase] : NULL;
                if (pnode && sno_qlit_fold(pnode)) {
                    sno_def_t d;
                    sno_parse_define(sno_qlit_fold(pnode), sno_define_entry_opt(dsub, _argbase), &d);
                    IR_t * bind = lc_build(g, IR_DEFINE_BIND, sJ, fA);
                    IR_LIT(bind).sval = lp_strdup(d.fname);
                    sno_bind_attach_entry(g, bind, d.entry, fA);
                    sno_bind_attach_proto(g, bind, &d, fA);
                    lc_γ_to(anchor[i], bind);
                    continue;
                }
            }
            lc_γ_to(anchor[i], sJ);
            continue;
        }
        if (_pro_open && (goU || goS || goF || exU || exS || exF)) _pro_close = 1;
        const tree_t * subj = lc_stmt_subj(s);
        const tree_t * pat = sfind_expr(s, ":pat");
        int has_eq = sfind(s, ":eq") != NULL;
        if (pat) {
            extern tree_t *ast_stmt_new(tree_e kind);
            tree_t * sc = ast_stmt_new(TT_SCAN);
            if (subj) ast_push(sc, (tree_t *) subj);
            else { tree_t * es = ast_stmt_new(TT_QLIT); es->v.sval = lp_strdup(""); ast_push(sc, es); }
            ast_push(sc, pat);
            subj = sc;
        }
        if (subj && subj->t == TT_SCAN) {
            sno_tvec_t cpro = { 0 };
            {
                const tree_t * p0 = (subj->n > 1) ? subj->c[1] : NULL;
                const tree_t * p1 = sno_capt_elem_rewrite(p0, &cpro);
                if (p1 != p0) { extern tree_t *ast_stmt_new(tree_e kind); tree_t * sc3 = ast_stmt_new(TT_SCAN); ast_push(sc3, subj->c[0]); ast_push(sc3, (tree_t *) p1); subj = sc3; }
            }
            const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
            cx.capro_cand = &cpro;
            if (!sno_pat_supported(ptt)) {
                if (ptt) {
                    extern tree_t *ast_stmt_new(tree_e kind);
                    IR_t * ec = NULL;
                    IR_t * asn = NULL;
                    const tree_t * sbj = subj->c[0];
                    const tree_t * dd = sno_pat_valued(&cx, &sbj, has_eq, ptt, fA, &ec, &asn);
                    tree_t * sc2 = ast_stmt_new(TT_SCAN);
                    ast_push(sc2, (tree_t *) sbj);
                    ast_push(sc2, (tree_t *) dd);
                    IR_t * e2 = sno_lower_match(&cx, sc2, has_eq ? sfind_expr(s, ":repl") : NULL, has_eq, sJ, fJ, &match_land[i]);
                    cx.capro_cand = NULL;
                    lc_γ_to(asn, e2);
                    lc_γ_to(anchor[i], sno_capt_prologue(&cx, &cpro, ec, fA));
                    continue;
                }
                sno_fatal("pattern match with no pattern operand", NULL);
            }
            IR_t * e = sno_lower_match(&cx, subj, has_eq ? sfind_expr(s, ":repl") : NULL, has_eq, sJ, fJ, &match_land[i]);
            cx.capro_cand = NULL;
            lc_γ_to(anchor[i], sno_capt_prologue(&cx, &cpro, e, fA));
            continue;
        }
        if (!subj) { lc_γ_to(anchor[i], sJ); continue; }
        if (!has_eq) { IR_t * r = NULL; IR_t * e = sx_lower(&cx, subj, sJ, fA, &r); lc_γ_to(anchor[i], e); continue; }
        tree_t * repl = sfind_expr(s, ":repl");
        if (subj->t == TT_VAR && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) {
            sno_reg_var(subj->v.sval);
            if (_pro_open && !result_name) sno_prologue_add(subj->v.sval);
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fA);
            IR_LIT(asn).sval = subj->v.sval;
            IR_t * mkv = NULL;
            IR_t * pae = sno_mkpat_emit(&cx, repl, asn, fA, &mkv);
            ir_operand_push(asn, mkv);
            lc_γ_to(anchor[i], pae);
            continue;
        }
        if (subj->t == TT_VAR) {
            sno_reg_var(subj->v.sval);
            if (repl && repl->t == TT_SCAN && repl->n == 2 && repl->c[0] && repl->c[1]) {
                extern tree_t * ast_stmt_new(tree_e kind);
                IR_t * pent = NULL;
                IR_t * pasn = NULL;
                const tree_t * sbj = repl->c[0];
                sno_tvec_t cpro = { 0 };
                const tree_t * rpat = sno_capt_elem_rewrite(repl->c[1], &cpro);
                const tree_t * pat = sno_pat_valued(&cx, &sbj, 0, rpat, fA, &pent, &pasn);
                tree_t * capt = ast_stmt_new(TT_CAPT_COND_ASGN);
                capt->slen = 240;
                ast_push(capt, (tree_t *) pat);
                tree_t * tv = ast_node_new(TT_VAR);
                tv->v.sval = subj->v.sval;
                ast_push(capt, tv);
                tree_t * sc = ast_stmt_new(TT_SCAN);
                ast_push(sc, (tree_t *) sbj);
                ast_push(sc, capt);
                cx.capro_cand = &cpro;
                IR_t * e = sno_lower_match(&cx, sc, NULL, 0, sJ, fA, NULL);
                cx.capro_cand = NULL;
                if (pasn) { lc_γ_to(pasn, e); e = pent; }
                lc_γ_to(anchor[i], sno_capt_prologue(&cx, &cpro, e, fA));
                continue;
            }
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fA);
            IR_LIT(asn).sval = subj->v.sval;
            IR_t * vr = NULL;
            IR_t * e = sx_lower(&cx, repl, asn, fA, &vr);
            ir_operand_push(asn, vr);
            lc_γ_to(anchor[i], e);
            continue;
        }
        if (subj->t == TT_INDIRECT && subj->n > 0) {
            IR_t * nv = NULL;
            IR_t * e1 = (g_lower.sno.uses_xname && subj->c[0]->t == TT_VAR) ? sno_xname_lv(&cx, subj->c[0], fA, &nv) : sx_nameval(&cx, subj->c[0], NULL, fA, &nv);
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(nv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, nv);
            ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_IDX && subj->n == 1) {
            tree_t * ek = ast_node_new(TT_QLIT);
            ek->v.sval = (char *) "";
            const tree_t * ekp = ek;
            IR_t * vr = NULL, * fb = NULL, * fi = NULL;
            IR_t * e1 = sx_subscript_lv_fused(&cx, subj->c[0], (const tree_t * const *) &ekp, 1, fA, &vr, &fb, &fi);
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            IR_t * asn;
            if (fb) {
                lc_γ_to(fb, e2);
                asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, fb);
                ir_operand_push(asn, fi);
                ir_operand_push(asn, vv);
                sx_sub_container_only(asn);
            } else {
                lc_γ_to(vr, e2);
                asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, vr);
                ir_operand_push(asn, vv);
            }
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_IDX && subj->n >= 2) {
            IR_t * vr = NULL, * fb = NULL, * fi = NULL;
            IR_t * e1 = sx_subscript_lv_fused(&cx, subj->c[0], (const tree_t * const *) &subj->c[1], subj->n - 1, fA, &vr, &fb, &fi);
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            IR_t * asn;
            if (fb) {
                lc_γ_to(fb, e2);
                asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, fb);
                ir_operand_push(asn, fi);
                ir_operand_push(asn, vv);
                sx_sub_container_only(asn);
            } else {
                lc_γ_to(vr, e2);
                asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, vr);
                ir_operand_push(asn, vv);
            }
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_FNC) {
            const char * fname = subj->v.sval;
            int argbase = 0;
            if (!fname && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR) { fname = subj->c[0]->v.sval; argbase = 1; }
            int fnargs = subj->n - argbase;
            if (fname && !strcmp(fname, "ITEM") && fnargs >= 2) {
                IR_t * vr = NULL;
                IR_t * e1 = sx_subscript_lv(&cx, subj->c[argbase], (const tree_t * const *) &subj->c[argbase + 1], fnargs - 1, fA, &vr);
                IR_t * vv = NULL;
                IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
                lc_γ_to(vr, e2);
                IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, vr);
                ir_operand_push(asn, vv);
                lc_γ_to(anchor[i], e1);
                continue;
            }
            {
                extern int rt_dat_field_of_any(const char *);
                if (fname && fnargs == 1 && rt_dat_field_of_any(fname)) {
                    IR_t * br = NULL;
                    IR_t * e1 = sx_lower(&cx, subj->c[argbase], NULL, fA, &br);
                    IR_t * fv = lc_build(g, IR_FIELD_VAR, NULL, fA);
                    IR_LIT(fv).sval = (char *) lp_strdup(fname);
                    lc_γ_to(br, fv);
                    ir_operand_push(fv, br);
                    IR_t * vv = NULL;
                    IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
                    lc_γ_to(fv, e2);
                    IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                    lc_γ_to(vv, asn);
                    ir_operand_push(asn, fv);
                    ir_operand_push(asn, vv);
                    lc_γ_to(anchor[i], e1);
                    continue;
                }
            }
        }
        if (subj->t == TT_KEYWORD && subj->v.sval && sno_kw_static_slot(subj->v.sval) >= 0) {
            IR_t * kv = NULL;
            IR_t * ke = sx_lower(&cx, repl, NULL, fA, &kv);
            IR_t * kw = lc_build(g, IR_KW_ASSIGN_SNOBOL4, sJ, fA);
            IR_LIT(kw).sval = subj->v.sval;
            lc_γ_to(kv, kw);
            ir_operand_push(kw, kv);
            lc_γ_to(anchor[i], ke);
            continue;
        }
        if (subj->t == TT_KEYWORD && subj->v.sval) {
            IR_t * mk = lc_build(g, IR_CALL, sJ, fA);
            IR_LIT(mk).sval = (char *) "SNO$KWSET";
            IR_t * nl = lc_build(g, IR_LIT_STRING, NULL, fA);
            IR_LIT(nl).sval = subj->v.sval;
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(nl, e2);
            lc_γ_to(vv, mk);
            ir_operand_push(mk, nl);
            ir_operand_push(mk, vv);
            lc_γ_to(anchor[i], nl);
            continue;
        }
        if (subj->t == TT_FNC) {
            IR_t * cv = NULL;
            IR_t * e1 = sno_lower_byname(&cx, subj, NULL, fA, &cv);
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(cv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, cv);
            ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], e1);
            continue;
        }
        sno_fatal("assignment subject form not in the landed subset", NULL);
    }
    if (zw5_on()) {
        for (int i = 0; i < nst; i++) {
            IR_t * fb = anchor[i] ? anchor[i]->γ.node : NULL;
            if (!fb) continue;
            IR_t * sbeg = lc_build(g, IR_STATEMENT_BEGIN, fb, fail_tgt[i]);
            {
                int _null_stmt = !lc_stmt_subj(st[i]) && !sfind_str(st[i],":lbl") && !sgoto(st[i],TT_GOTO_U) && !sgoto_expr(st[i],TT_GOTO_U) && !sgoto_direct(st[i],TT_GOTO_U) &&
                    !sgoto(st[i],TT_GOTO_S) && !sgoto_expr(st[i],TT_GOTO_S) && !sgoto_direct(st[i],TT_GOTO_S) && !sgoto(st[i],TT_GOTO_F) && !sgoto_expr(st[i],TT_GOTO_F) &&
                    !sgoto_direct(st[i],TT_GOTO_F) && !sfind(st[i],":eq");
                const tree_t * _sa = _null_stmt ? NULL : sfind(st[i], ":stno");
                if (_sa && _sa->n > 0 && _sa->c[0]) {
                    const tree_t * _c = _sa->c[0];
                    IR_LIT(sbeg).ival = stno_base + ((_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0));
                    {
                        extern void emit_stno_src_note(long long, int, const char *);
                        extern const char * stmt_src_get_file(void);
                        int _lln = lp_s_int(st[i], ":lline");
                        if (!_lln) _lln = lp_s_int(st[i], ":line");
                        const char * _sf = sfind_str(st[i], ":kwfile");
                        if (!_sf || !*_sf) _sf = sfind_str(st[i], ":file");
                        if (!_sf || !*_sf) _sf = stmt_src_get_file();
                        emit_stno_src_note((long long)IR_LIT(sbeg).ival, _lln, _sf);
                    }
                }
            }
            lc_γ_to(anchor[i], sbeg);
            if (match_land[i]) lc_γ_tag_β(match_land[i]);
            if (asgn_land[i]) { lc_γ_to(asgn_land[i], sbeg); lc_γ_tag_β(asgn_land[i]); }
        }
    }
    { const char * _sk = getenv("SCRIP_SNO_STMTKW"); if (_sk && *_sk == '1') { g_lower.sno.uses_stmtkw = 1; g_lower.sno.traces_a_label = 1; } }
    if (g_lower.sno.uses_stmtkw) {
        const char * _stmtkw_cur_file = (const char *) 0;
        for (int i = 0; i < nst; i++) {
            if (lp_s_int(st[i], ":nocount") || sno_stmt_is_blank(st[i])) continue;
            IR_t * body = anchor[i]->γ.node;
            IR_t * tj = lc_build(g, IR_SETEXIT_TEST, body, body);
            IR_t * hook = lc_build(g, IR_STMT_MARK, tj, tj);
            IR_LIT(hook).ival = (int64_t)(i + 1) + stno_base;
            int _lln = lp_s_int(st[i], ":lline");
            if (!_lln) _lln = lp_s_int(st[i], ":line");
            hook->pat_static = _lln;
            hook->seal = IR_SEAL_STMT_HOOK;
            IR_t * entry = hook;
            {
                extern const char * stmt_src_get_file(void);
                const char * _sf = sfind_str(st[i], ":kwfile");
                if (!_sf || !*_sf) _sf = sfind_str(st[i], ":file");
                if (!_sf || !*_sf) _sf = stmt_src_get_file();
                if (!_sf) _sf = "";
                if (!_stmtkw_cur_file || strcmp(_stmtkw_cur_file, _sf) != 0) {
                    _stmtkw_cur_file = _sf;
                    IR_t * fhook = lc_build(g, IR_CALL, body, body);
                    IR_LIT(fhook).sval = (char *) "SNO$STMT";
                    IR_t * fnum = lc_build(g, IR_LIT_INTEGER, fhook, fhook);
                    IR_LIT(fnum).ival = (int64_t) -1;
                    IR_t * flnn = lc_build(g, IR_LIT_INTEGER, fhook, fhook);
                    IR_LIT(flnn).ival = (int64_t)_lln;
                    IR_t * fpn = lc_build(g, IR_LIT_STRING, fhook, fhook);
                    IR_LIT(fpn).sval = (char *) _sf;
                    lc_γ_to(fnum, flnn);
                    lc_γ_to(flnn, fpn);
                    ir_operand_push(fhook, fnum);
                    ir_operand_push(fhook, flnn);
                    ir_operand_push(fhook, fpn);
                    lc_γ_to(hook, fnum);
                }
            }
            lc_γ_to(anchor[i], entry);
        }
    }
    for (int i = 0; i < nst; i++) {
        const char * ssrc = sfind_str(st[i], ":src");
        if (!ssrc) continue;
        IR_t * t = anchor[i];
        int hops = 0;
        while (t && t->op == IR_GOTO && t->γ.node && hops++ < 64) t = t->γ.node;
        if (t) { int _ln = lp_s_int(st[i], ":line"); bb_src_note(t, ssrc, lp_s_int(st[i], ":incl") ? -_ln : _ln); }
    }
    ct_drop(anchor);
    ct_drop(fail_tgt);
    ct_drop(asgn_land);
    ct_drop(match_land);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_stmt_label(const tree_t * s) { return s ? sfind_str(s, ":lbl") : NULL; }
