#include "lower/lower_snobol4/lsn_head.h"
#include <stdlib.h>
#include "ir/ir_graph.h"
#include "ir/ir_graph_scrip_ir_api.h"
#include "lower/lower_common.h"
#include "lower/lower_snobol4/lsn_02_sno.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_04_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_06_sh.h"
#include "lower/lower_snobol4/lsn_08_sno.h"
#include "lower/lower_snobol4/lsn_09_sno.h"
#include "lower/g_lower_sno.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "runtime/builtins/gen_expr_t_defined.h"
static const char * sno_pat_collect(const tree_t * pat);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pat_dfree(const tree_t * t, int spine, int depth) {
    if (!t) return 1;
    if (depth > 48) return 0;
    switch (t->t) {
        case TT_DEFER:
        return 0;
        case TT_QLIT:
        case TT_ILIT:
        case TT_FLIT:
        case TT_CSET:
        case TT_NUL:
        return 1;
        case TT_REM:
        case TT_ARB:
        case TT_FAIL:
        case TT_SUCCEED:
        case TT_ABORT:
        case TT_BAL:
        case TT_FLUSH:
        return 1;
        case TT_VAR:
        { const char * nm = t->v.sval; if (!spine) return 1; if (nm && (!strcmp(nm, "REM") || !strcmp(nm, "ARB") || !strcmp(nm, "FENCE") || !strcmp(nm, "FLUSH"))) return 1; return 0; }
        case TT_KEYWORD:
        {
            if (!sno_const_static_on()) return 0;
            if (!spine) return 1;
            if (!t->v.sval) return 0;
            char cb[130];
            snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
            const tree_t * p = sno_const_pat(cb);
            return p ? sno_pat_dfree(p, 1, depth + 1) : 0;
        }
        case TT_ANY:
        case TT_NOTANY:
        case TT_SPAN:
        case TT_BREAK:
        case TT_BREAKX:
        case TT_LEN:
        case TT_TAB:
        case TT_RTAB:
        case TT_POS:
        case TT_RPOS:
        { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 0, depth + 1)) return 0; return 1; }
        case TT_SEQ:
        case TT_CAT:
        case TT_ALT:
        case TT_FENCE:
        case TT_ARBNO:
        { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 1, depth + 1)) return 0; return 1; }
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        case TT_CAPT_CURSOR:
        return 0;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_prologue_add(const char * nm) {
    if (!nm) return;
    for (uint32_t i = 0; i < g_lower_sno.pro.len; i++) if (!strcmp(CV_AT(g_lower_sno.pro, const char *, i), nm)) return;
    CV_PUSH(g_lower_sno.pro, const char *) = nm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_cap_fc(IR_graph_t * g, IR_t * nd, IR_t * save, int before_i) {
    int fp_inner = 0;
    int walk_ok = fc_walk_range(g, before_i, g->n, 0, &fp_inner);
    if (!walk_ok) return;
    if (g_lower_sno.in_arbno == 0) {
        extern void fc_save_register(const IR_t *);
        extern void fc_cond_register_with_save(const IR_t *, const IR_t *, int);
        fc_save_register(save);
        fc_cond_register_with_save(nd, save, fp_inner);
        return;
    }
    { sno_scd_t x; x.nd = nd; x.save = save; x.nd_idx = before_i - 2; x.save_idx = before_i - 1; x.i_end = g->n; x.fp_inner = fp_inner; x.promo = 0; CV_PUSH(g_lower_sno.scd, sno_scd_t) = x; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp) {
    extern int fc_alt_fpmax(const IR_t *);
    extern int fc_alt_extent(const IR_t *, int *, int *);
    int lin = 1;
    for (int k = k0; k < k1; k++) {
        IR_t * x = g->all[k];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) { int _b = 0, _e = 0; if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > k + 1) k = _e - 1; continue; } lin = 0; continue; }
        { long fck; if (fc_geom(x, &fck)) { if (fp) *fp += (int)fck; continue; } }
        switch (x->op) {
            case IR_MATCH_LIT:
            case IR_MATCH_LEN:
            case IR_MATCH_ANY:
            case IR_MATCH_NOTANY:
            case IR_MATCH_POS:
            case IR_MATCH_RPOS:
            case IR_MATCH_ATP:
            case IR_MATCH_ASSIGN_SAVE:
            case IR_MATCH_ASSIGN_COND:
            case IR_MATCH_ASSIGN_IMM:
            case IR_GOTO:
            break;
            case IR_LIT_INTEGER:
            case IR_LIT_STRING:
            case IR_LIT_DSTAR:
            case IR_LIT_THUNK:
            case IR_LIT_REAL:
            if (!lit_ok) lin = 0;
            else if (fp) *fp += 16;
            break;
            default:
            lin = 0;
        }
    }
    return lin;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_tail_walk(IR_graph_t * g, int k0, int k1) {
    extern int fc_alt_fpmax(const IR_t *);
    extern int fc_alt_extent(const IR_t *, int *, int *);
    for (int k = k0; k < k1 && k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) { int _b = 0, _e = 0; if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > k + 1) k = _e - 1; continue; } return 0; }
        if (x->op == IR_MATCH_DEFER) {
            if (g_lower_sno.fc_tail_walk_dtl < 0) { const char * _e = getenv("SCRIP_ARBNO_LATCH"); g_lower_sno.fc_tail_walk_dtl = _e ? (atoi(_e) != 0) : 0; }
            if (g_lower_sno.fc_tail_walk_dtl && x->seal == 2 && IR_LIT(x).sval && sno_name_prologue_bound(IR_LIT(x).sval)) {
                const char * _pn = 0;
                for (int _j = 0; _j < x->n_operands; _j++) {
                    IR_t * _o = x->operands[_j];
                    if (_o && ir_is_lit_text_kind(_o->op) && IR_LIT(_o).sval && !strncmp(IR_LIT(_o).sval, "PAT$", 4)) { _pn = IR_LIT(_o).sval; break; }
                }
                if (_pn) continue;
            }
            return 0;
        }
        { long fck; if (fc_geom(x, &fck)) continue; }
        switch (x->op) {
            case IR_MATCH_LIT:
            case IR_MATCH_LEN:
            case IR_MATCH_ANY:
            case IR_MATCH_NOTANY:
            case IR_MATCH_POS:
            case IR_MATCH_RPOS:
            case IR_MATCH_ATP:
            case IR_MATCH_ASSIGN_SAVE:
            case IR_MATCH_ASSIGN_COND:
            case IR_MATCH_ASSIGN_IMM:
            case IR_GOTO:
            case IR_LIT_INTEGER:
            case IR_LIT_STRING:
            case IR_LIT_DSTAR:
            case IR_LIT_THUNK:
            case IR_LIT_REAL:
            break;
            default:
            return 0;
        }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_patsalt_on(void) {
    if (g_lower_sno.sno_patsalt_on_v < 0) { const char * e = getenv("SCRIP_PATSALT"); g_lower_sno.sno_patsalt_on_v = (e && *e == '0') ? 0 : 1; }
    return g_lower_sno.sno_patsalt_on_v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_patname_salt_on(void) {
    if (g_lower_sno.sno_patname_salt_on_v < 0) { const char * e = getenv("SCRIP_PATNAME_SALT"); g_lower_sno.sno_patname_salt_on_v = (e && *e == '0') ? 0 : 1; }
    return g_lower_sno.sno_patname_salt_on_v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_pat_collect(const tree_t * pat) {
    for (int i = 0; i < (int) g_lower_sno.pats.len; i++)
        if ((!sno_patsalt_on() || CV_AT(g_lower_sno.pats, sno_pat_ent_t, i).salt == g_lower_sno.expr_salt) && sno_expr_eq(CV_AT(g_lower_sno.pats, sno_pat_ent_t, i).pat, pat))
        return CV_AT(g_lower_sno.pats, sno_pat_ent_t, i).name;
    char buf[32];
    if (sno_patname_salt_on() && g_lower_sno.expr_salt) snprintf(buf, sizeof buf, "PAT$%dF%d", (int) g_lower_sno.pats.len, g_lower_sno.expr_salt);
    else snprintf(buf, sizeof buf, "PAT$%d", (int) g_lower_sno.pats.len);
    { sno_pat_ent_t x; x.name = lp_strdup(buf); x.pat = pat; x.salt = g_lower_sno.expr_salt; CV_PUSH(g_lower_sno.pats, sno_pat_ent_t) = x; return x.name; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_mkpat_emit(scx_t * cx, const tree_t * pat, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    const char * bn = sno_pat_collect(pat);
    IR_t * mk = lc_build(g, IR_CALL, γ, ω);
    IR_LIT(mk).sval = (char *) "SNO$MKPAT";
    IR_t * nl = lc_build(g, IR_LIT_THUNK, NULL, ω);
    IR_LIT(nl).sval = (char *) bn;
    ir_operand_push(mk, nl);
    IR_graph_t * tg = IR_alloc(256);
    scx_t tx;
    tx.g = tg;
    tx.wn_tree = NULL;
    tx.loop_exit = NULL;
    tx.loop_next = NULL;
    tx.result_name = NULL;
    tx.pat_fail = NULL;
    tx.pat_seal = NULL;
    tx.npre = 0;
    tx.snap = 1;
    tx.capro = NULL;
    tx.capro_cand = NULL;
    tx.ppos = 0;
    tx.prog_nstmt = 0;
    tx.stno_base = 0;
    IR_t * tok = lc_build(tg, IR_SUCCEED, NULL, NULL);
    IR_t * tno = lc_build(tg, IR_FAIL, NULL, NULL);
    tx.pat_fail = tno;
    tx.pat_seal = sno_thunk_abort_exit(tg, tno);
    sno_pat_node(&tx, pat, tok, tno);
    IR_t * last = nl;
    for (int api = 0; api < tx.npre; api++) {
        IR_t * av = NULL;
        IR_t * ae = sx_lower(cx, tx.pre[api].arg, NULL, ω, &av);
        lc_γ_to(last, ae);
        last = av;
        if (tx.pre[api].str && tx.pre[api].codes) {
            IR_t * co = lc_build(g, IR_COERCE_STRING, NULL, ω);
            IR_LIT(co).ival = tx.pre[api].codes;
            lc_γ_to(av, co);
            ir_operand_push(co, av);
            last = co;
        } else if (!tx.pre[api].str && tx.pre[api].codes && tx.pre[api].arg && tx.pre[api].arg->t != TT_DEFER) {
            IR_t * co = lc_build(g, IR_COERCE_INTEGER, NULL, ω);
            IR_LIT(co).ival = tx.pre[api].codes;
            lc_γ_to(av, co);
            ir_operand_push(co, av);
            last = co;
        }
        if (tx.pre[api].ppos) {
            IR_t * pck = lc_build(g, IR_CALL, NULL, ω);
            IR_LIT(pck).sval = (char *) "SNO$PATCK";
            IR_t * pcl = lc_build(g, IR_LIT_INTEGER, pck, ω);
            IR_LIT(pcl).ival = tx.pre[api].ppos;
            lc_γ_to(last, pcl);
            ir_operand_push(pck, last);
            ir_operand_push(pck, pcl);
            last = pck;
        }
        ir_operand_push(mk, last);
    }
    lc_γ_to(last, mk);
    if (res) *res = mk;
    return nl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_lower_match_body(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land) {
    IR_graph_t * g = cx->g;
    cx->pat_fail = fJ;
    cx->pat_seal = fJ;
    cx->npre = 0;
    const tree_t * svt = (subj->n > 0) ? subj->c[0] : NULL;
    const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
    IR_t * land = fJ;
    if (out_land) { land = lc_build(g, IR_GOTO, fJ, NULL); *out_land = land; }
    IR_t * head = lc_build(g, IR_MATCH_BEGIN, NULL, land);
    { IR_t * sealJ = lc_build(g, IR_GOTO, head, NULL); memcpy(sealJ->γ.sz, "φ", 3); sealJ->γ.sz[3] = 0; cx->pat_seal = sealJ; }
    IR_t * splice = NULL;
    IR_t * lv_pro = NULL;
    IR_t * lv_entry = NULL;
    const char * lv_tmp = NULL;
    if (has_repl && svt && svt->t != TT_VAR && (svt->t == TT_IDX || svt->t == TT_INDIRECT || svt->t == TT_FNC)) {
        char nb[64], vb[64];
        snprintf(nb, sizeof nb, "SN4$RPLN%d", g->n);
        snprintf(vb, sizeof vb, "SN4$RPLV%d", g->n);
        const char * nn = lp_strdup(nb);
        lv_tmp = lp_strdup(vb);
        sno_reg_var(nn);
        sno_reg_var(lv_tmp);
        tree_t * nmx;
        if (svt->t == TT_IDX || svt->t == TT_FNC) { nmx = ast_node_new(TT_NAME); ast_push(nmx, (tree_t *) svt); } else { nmx = (tree_t *) ((svt->n > 0) ? svt->c[0] : NULL); }
        if (!nmx) sno_fatal("SN4-REPL slice 2: indirect replacement subject has no operand", NULL);
        lv_pro = lc_build(g, IR_ASSIGN, NULL, fJ);
        IR_LIT(lv_pro).sval = (char *) nn;
        IR_t * nv = NULL;
        IR_t * ne = sx_lower(cx, nmx, lv_pro, fJ, &nv);
        ir_operand_push(lv_pro, nv);
        lv_pro->pat_static = 0;
        { tree_t * iv = ast_node_new(TT_VAR); iv->v.sval = (char *) nn; tree_t * ind = ast_node_new(TT_INDIRECT); ast_push(ind, iv); svt = ind; }
        lv_entry = ne;
    }
    if (has_repl) {
        if (!svt || (svt->t != TT_VAR && !lv_tmp)) sno_fatal("SN4-REPL slice 1: replacement subject must be a plain variable (indirect/subscript lvalue splice pending)", NULL);
        const char * tgt = lv_tmp ? lv_tmp : svt->v.sval;
        sno_reg_var(tgt);
        IR_t * wb_entry = sJ;
        if (lv_tmp) {
            tree_t * nv2 = ast_node_new(TT_VAR);
            nv2->v.sval = (char *) lv_tmp;
            tree_t * iv2 = ast_node_new(TT_VAR);
            iv2->v.sval = (char *) IR_LIT(lv_pro).sval;
            tree_t * ind2 = ast_node_new(TT_INDIRECT);
            ast_push(ind2, iv2);
            tree_t * asg = ast_node_new(TT_ASSIGN);
            ast_push(asg, ind2);
            ast_push(asg, nv2);
            IR_t * wres = NULL;
            wb_entry = sx_lower(cx, asg, sJ, fJ, &wres);
        }
        splice = lc_build(g, IR_MATCH_REPLACE, wb_entry, NULL);
        IR_LIT(splice).sval = (char *) tgt;
        ir_operand_push(splice, head);
        IR_t * rv = NULL;
        IR_t * re;
        if (repl_t) re = sx_lower(cx, repl_t, splice, fJ, &rv);
        else { re = lc_build(g, IR_LIT_STRING, splice, fJ); IR_LIT(re).sval = (char *) ""; rv = re; }
        ir_operand_push(splice, rv);
        sJ = re;
    }
    IR_t * release = lc_build(g, IR_MATCH_END, sJ, sno_cap_name_strict() ? cx->pat_seal : NULL);
    if (has_repl) IR_LIT(release).dval = 1.0;
    ir_operand_push(release, head);
    int before_pat = g->n;
    sno_cap_defer_reset();
    IR_t * pat_entry;
    {
        int _mcsv = g_lower_sno.pat_match_ctx;
        g_lower_sno.pat_match_ctx = 1;
        cx->ppos = 240;
        cx->capro = cx->capro_cand;
        pat_entry = sno_pat_node(cx, ptt, release, head);
        cx->capro = NULL;
        g_lower_sno.pat_match_ctx = _mcsv;
    }
    lc_γ_to(head, pat_entry);
    {
        int fp_stmt = 0;
        int fc_lin = (g_lower_sno.in_arbno == 0) && fc_walk_range(g, before_pat, g->n, 1, &fp_stmt);
        if (fc_lin) { extern void fc_head_register(const IR_t *, int); fc_head_register(head, fp_stmt); }
        if (!fc_lin) {
            int tail_ok = 0;
            const char * tl_why = (cx->npre != 0) ? "npre" : has_repl ? "repl" : "gate";
            if (cx->npre == 0 && !has_repl) {
                int i_arb = -1, n_arb = 0;
                for (int k = before_pat; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->op == IR_MATCH_ARBNO) { n_arb++; i_arb = k; } }
                tl_why = (n_arb == 0) ? "no-arbno" : (n_arb > 1) ? "multi-arbno" : tl_why;
                if (n_arb == 1) {
                    IR_t * R = g->all[i_arb];
                    int i_b0 = -1, i_b1 = -1;
                    if (R->n_operands >= 3) for (int j = before_pat; j < g->n; j++) { if (g->all[j] == R->operands[1]) i_b0 = j; if (g->all[j] == R->operands[2]) i_b1 = j; }
                    tl_why = "body-range";
                    if (i_b0 > i_arb && i_b1 >= i_b0) {
                        int cap_left = 0;
                        int n_wrap = 0;
                        const IR_t * wsv[i_arb - before_pat + 1];
                        const IR_t * wcd[i_arb - before_pat + 1];
                        {
                            const IR_t * inner_want = R;
                            for (;;) {
                                int found = -1;
                                for (int k = before_pat; k < i_arb; k++) {
                                    IR_t * x = g->all[k];
                                    if (x && (x->op == IR_MATCH_ASSIGN_COND || x->op == IR_MATCH_ASSIGN_IMM) && x->n_operands > 1 && x->operands[0] == inner_want) { found = k; break; }
                                }
                                if (found < 0) break;
                                IR_t * sv = (found + 1 < g->n) ? g->all[found + 1] : NULL;
                                if (!sv || sv->op != IR_MATCH_ASSIGN_SAVE || g->all[found]->operands[1] != sv || n_wrap >= i_arb - before_pat + 1) { cap_left = 1; break; }
                                wcd[n_wrap] = g->all[found];
                                wsv[n_wrap] = sv;
                                n_wrap++;
                                inner_want = sv;
                            }
                            for (int k = before_pat; k < i_arb && !cap_left; k++) {
                                IR_t * x = g->all[k];
                                if (!x || (x->op != IR_MATCH_ASSIGN_SAVE && x->op != IR_MATCH_ASSIGN_COND && x->op != IR_MATCH_ASSIGN_IMM)) continue;
                                int used = 0;
                                for (int w = 0; w < n_wrap; w++) if (x == wsv[w] || x == wcd[w]) { used = 1; break; }
                                if (used) continue;
                                {
                                    extern int fc_save_active(const IR_t *);
                                    extern int fc_cond_fp(const IR_t *);
                                    if (x->op == IR_MATCH_ASSIGN_SAVE ? !fc_save_active(x) : fc_cond_fp(x) < 0) cap_left = 1;
                                }
                            }
                        }
                        int cap_bad = 0;
                        for (uint32_t d = 0; d < g_lower_sno.scd.len; d++) CV_AT(g_lower_sno.scd, sno_scd_t, d).promo = 0;
                        {
                            extern int fc_alt_fpmax(const IR_t *);
                            extern int fc_alt_extent(const IR_t *, int *, int *);
                            extern int fc_cond_fp(const IR_t *);
                            for (int d = 0; d < (int) g_lower_sno.scd.len && !cap_bad; d++) {
                                if (CV_AT(g_lower_sno.scd, sno_scd_t, d).save_idx < i_b0 || CV_AT(g_lower_sno.scd, sno_scd_t, d).nd_idx >= g->n) { cap_bad = 1; break; }
                                int in_arm = 0;
                                for (int k = i_b0; k < g->n; k++) {
                                    IR_t * x = g->all[k];
                                    if (!x || x->op != IR_MATCH_ALTERNATE || fc_alt_fpmax(x) < 0) continue;
                                    int _b = 0, _e = 0;
                                    if (fc_alt_extent(x, &_b, &_e) && CV_AT(g_lower_sno.scd, sno_scd_t, d).nd_idx >= _b && CV_AT(g_lower_sno.scd, sno_scd_t, d).nd_idx < _e) { in_arm = 1; break; }
                                }
                                int nested = 0;
                                for (int e = 0; e < (int) g_lower_sno.scd.len; e++) {
                                    if (e == d) continue;
                                    if (CV_AT(g_lower_sno.scd, sno_scd_t, e).nd_idx > CV_AT(g_lower_sno.scd, sno_scd_t, d).save_idx &&
                                        CV_AT(g_lower_sno.scd, sno_scd_t, e).nd_idx < CV_AT(g_lower_sno.scd, sno_scd_t, d).i_end) {
                                        nested = 1;
                                        break;
                                    }
                                }
                                if (in_arm || nested) cap_bad = 1;
                                else CV_AT(g_lower_sno.scd, sno_scd_t, d).promo = 1;
                            }
                            if (!cap_bad) for (int k = i_b0; k < g->n; k++) {
                                IR_t * x = g->all[k];
                                if (!x || (x->op != IR_MATCH_ASSIGN_COND && x->op != IR_MATCH_ASSIGN_IMM)) continue;
                                if (fc_cond_fp(x) >= 0) continue;
                                int def_ok = 0;
                                for (int d = 0; d < (int) g_lower_sno.scd.len; d++) if (CV_AT(g_lower_sno.scd, sno_scd_t, d).nd == x && CV_AT(g_lower_sno.scd, sno_scd_t, d).promo) {
                                    def_ok = 1;
                                    break;
                                }
                                if (!def_ok) { cap_bad = 1; break; }
                            }
                        }
                        tl_why = cap_left ? "cap-left" : cap_bad ? "cap-bad" : "walk";
                        int dfr_regs = 0;
                        if (!cap_left && !cap_bad) {
                            int _hd = 0;
                            for (int k = before_pat; k < g->n && !_hd; k++) { IR_t * x = g->all[k]; if (x && x->op == IR_MATCH_DEFER) _hd = 1; }
                            if (_hd) {
                                int _ap = 0;
                                for (int d = 0; d < (int) g_lower_sno.scd.len && !_ap; d++) if (CV_AT(g_lower_sno.scd, sno_scd_t, d).promo) _ap = 1;
                                dfr_regs = (n_wrap > 0 || _ap);
                                if (dfr_regs) tl_why = "defer-caps";
                            }
                        }
                        if (!cap_left && !cap_bad && !dfr_regs && fc_tail_walk(g, before_pat, i_arb) && fc_tail_walk(g, i_b0, i_b1 + 1) && fc_tail_walk(g, i_b1 + 1, g->n)) {
                            {
                                extern void fc_save_register(const IR_t *);
                                extern void fc_cond_register(const IR_t *, int);
                                for (int d = 0; d < (int) g_lower_sno.scd.len; d++) if (CV_AT(g_lower_sno.scd, sno_scd_t, d).promo) {
                                    fc_save_register(CV_AT(g_lower_sno.scd, sno_scd_t, d).save);
                                    fc_cond_register(CV_AT(g_lower_sno.scd, sno_scd_t, d).nd, CV_AT(g_lower_sno.scd, sno_scd_t, d).fp_inner);
                                }
                                for (int w = 0; w < n_wrap; w++) fc_save_register(wsv[w]);
                            }
                            extern void fc_tail_candidate(const IR_t *, const IR_t *, int, int, int, int, int);
                            fc_tail_candidate(head, R, before_pat, i_arb, i_b0, i_b1, g->n);
                            { extern void fc_tail_wrap(const IR_t *, const IR_t *, const IR_t *); for (int w = 0; w < n_wrap; w++) fc_tail_wrap(R, wsv[w], wcd[w]); }
                            tail_ok = 1;
                        }
                    }
                }
            }
            if (!tail_ok && getenv("SCRIP_TAIL_DIAG")) fprintf(stderr, "[TAIL-DIAG] refuse: %s\n", tl_why);
        }
    }
    IR_t * after = head;
    if (g_lower_sno.sno_lower_match_preord < 0) { const char * e = getenv("SCRIP_PRE_ORDER"); g_lower_sno.sno_lower_match_preord = (e && *e == '0') ? 0 : 1; }
    for (int pi = g_lower_sno.sno_lower_match_preord ? cx->npre - 1 : 0; g_lower_sno.sno_lower_match_preord ? (pi >= 0) : (pi < cx->npre); pi += g_lower_sno.sno_lower_match_preord ? -1 : 1) {
        if (!cx->pre[pi].prim) { IR_t * av = NULL; after = sx_lower(cx, cx->pre[pi].arg, after, fJ, &av); continue; }
        if (cx->pre[pi].snapg) {
            char nb[32];
            snprintf(nb, sizeof nb, "PATV$%d", g_lower_sno.sno_lower_match_snapctr++);
            char * gname = lp_strdup(nb);
            sno_reg_var(gname);
            IR_LIT(cx->pre[pi].prim).sval = gname;
            cx->pre[pi].prim->pat_static = 1;
            IR_t * asnV = lc_build(g, IR_ASSIGN, after, fJ);
            IR_LIT(asnV).sval = gname;
            IR_t * av = NULL;
            IR_t * pck = NULL;
            IR_t * pcl = NULL;
            if (cx->pre[pi].ppos) { pck = lc_build(g, IR_CALL, asnV, fJ); IR_LIT(pck).sval = (char *) "SNO$PATCK"; pcl = lc_build(g, IR_LIT_INTEGER, pck, fJ); IR_LIT(pcl).ival = cx->pre[pi].ppos; }
            IR_t * ae = sx_lower(cx, cx->pre[pi].arg, pcl ? pcl : asnV, fJ, &av);
            if (pck) { ir_operand_push(pck, av); ir_operand_push(pck, pcl); ir_operand_push(asnV, pck); } else ir_operand_push(asnV, av);
            after = ae;
            continue;
        }
        IR_t * co = lc_build(g, cx->pre[pi].str ? IR_COERCE_STRING : IR_COERCE_INTEGER, after, fJ);
        IR_LIT(co).ival = cx->pre[pi].codes;
        IR_t * av = NULL;
        IR_t * ae = sx_lower(cx, cx->pre[pi].arg, co, fJ, &av);
        ir_operand_push(co, av);
        ir_operand_push(cx->pre[pi].prim, co);
        after = ae;
    }
    cx->npre = 0;
    IR_t * subjval = NULL;
    IR_t * subj_entry = sx_lower(cx, svt, after, fJ, &subjval);
    ir_operand_push(head, subjval);
    if (splice) ir_operand_push(splice, subjval);
    if (lv_pro) { lc_γ_to(lv_pro, subj_entry); return lv_entry; }
    return subj_entry;
}
