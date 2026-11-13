#include "lower/lower_snobol4/lsn_head.h"
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
#include "lower/lower_snobol4/lsn_08_sno.h"
#include "lower/lower_snobol4/lsn_09_sno.h"
#include "lower/g_lower_sno.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_ω_to(IR_t * nd, IR_t * t) { if (t) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_resume_ω_to(IR_graph_t * g, int tail_idx, IR_t * nd, IR_t * t) {
    if (nd && nd->op == IR_MATCH_ALTERNATE && nd->n_operands == 0 && g && tail_idx + 1 < g->n) {
        IR_t * T = g->all[tail_idx + 1];
        if (T && T->op == IR_MATCH_ALTERNATE && T->n_operands > 0 && T->operands[0] == nd) { if (t) lc_γ_to_β(T, t); else lc_γ_to(T, t); return; }
    }
    if (nd && nd->op == IR_MATCH_ASSIGN_COND && nd->n_operands > 1 && nd->operands[1]) nd = nd->operands[1];
    if (nd && t && nd->op == IR_GOTO && nd->n_operands == 0 && nd->γ.node && nd->γ.node == nd->ω.node && !strcmp(nd->γ.sz, "β")) lc_γ_to_β(nd, t);
    sno_ω_to(nd, t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_emit_order_last(IR_graph_t * g, int lo, IR_t * stop, IR_t * nd, char * vis, IR_t ** last) {
    if (!nd || nd == stop) return;
    int ix = -1;
    for (int k = lo; k < g->n; k++) if (g->all[k] == nd) { ix = k; break; }
    if (ix < 0 || vis[ix - lo]) return;
    vis[ix - lo] = 1;
    *last = nd;
    sno_emit_order_last(g, lo, stop, nd->γ.node, vis, last);
    sno_emit_order_last(g, lo, stop, nd->ω.node, vis, last);
    for (int j = 0; j < nd->n_operands; j++) sno_emit_order_last(g, lo, stop, nd->operands[j], vis, last);
}
static void sno_elem_pre(scx_t * cx, const tree_t * tg, IR_t * prim, IR_t * prim2) {
    tree_t * nx = ast_node_new(TT_NAME);
    ast_push(nx, (tree_t *) tg);
    cx->pre[cx->npre].arg = nx;
    cx->pre[cx->npre].prim = prim;
    cx->pre[cx->npre].prim2 = prim2;
    cx->pre[cx->npre].str = 0;
    cx->pre[cx->npre].codes = 0;
    cx->pre[cx->npre].snapg = sno_elem_snapg;
    cx->pre[cx->npre].ppos = 0;
    cx->npre++;
}
static void sno_capt_ordasg(scx_t * cx, const tree_t * tg) {
    if (!cx->capro || cx->snap || !tg || tg->t != TT_DEFER || tg->n < 1 || !tg->c[0] || tg->c[0]->t != TT_INDIRECT || tg->c[0]->n < 1 || !tg->c[0]->c[0] || tg->c[0]->c[0]->t != TT_VAR ||
        cx->npre < 0 || cx->npre >= 64) return;
    const char * nm = tg->c[0]->c[0]->v.sval;
    for (int k = 0; k < cx->capro->n; k++) {
        const tree_t * a = cx->capro->v[k];
        if (!a || a->t != TT_ASSIGN || a->n < 2 || !a->c[0] || a->c[0]->t != TT_VAR || !a->c[0]->v.sval || strcmp(a->c[0]->v.sval, nm)) continue;
        cx->pre[cx->npre] = (sprearg_t){ .arg = a };
        cx->npre++;
        cx->capro->v[k] = NULL;
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_lambda_kind(const char * n, int nargs) {
    if (!n || nargs != 1) return 0;
    if (!strcmp(n, "LAMBDA") || !strcmp(n, "\xce\x9b")) return 1;
    if (!strcmp(n, "lambda") || !strcmp(n, "\xce\xbb")) return 2;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_tree_has_varext(const IR_t * n, int d) {
    if (!n || d > 12) return 0;
    if (n->op == IR_MATCH_ARBNO || n->op == IR_MATCH_DEFER) return 1;
    for (int i = 0; i < n->n_operands; i++) if (sno_tree_has_varext(n->operands[i], d + 1)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_cap_defer_reset(void) { g_lower_sno.scd.len = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_capt_name(const tree_t * tgt) {
    if (!tgt) return NULL;
    if (tgt->t == TT_VAR) return tgt->v.sval;
    if (tgt->t == TT_KEYWORD && tgt->v.sval && tgt->v.sval[0]) { const char * kn = tgt->v.sval; return sno_lead_name("&&", kn[0] == '&' ? kn + 1 : kn); }
    if (tgt->t == TT_INDIRECT && tgt->n > 0 && tgt->c[0] && tgt->c[0]->t == TT_QLIT) return (tgt->c[0]->v.sval && tgt->c[0]->v.sval[0]) ? tgt->c[0]->v.sval : NULL;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_lead_name(const char * lead, const char * nm) {
    size_t ll = strlen(lead), n = strlen(nm);
    char tmp[ll + n + 1];
    memcpy(tmp, lead, ll);
    memcpy(tmp + ll, nm, n + 1);
    return lp_strdup(tmp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_cursor_target(const tree_t * tgt) {
    const char * vn = sno_capt_name(tgt);
    if (vn) { sno_reg_var(vn); return vn; }
    if (!tgt || tgt->t != TT_DEFER) return NULL;
    const tree_t * di = (tgt->n > 0) ? tgt->c[0] : NULL;
    if (!di) return NULL;
    if (di->t == TT_VAR && di->v.sval && di->v.sval[0]) { sno_reg_var(di->v.sval); return di->v.sval; }
    const char * bn = (di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : ((di->t == TT_INDIRECT && di->n > 0 && di->c[0]) || (di->t == TT_IDX && di->n > 0) || sno_item_argbase(di) >= 0) ?
        sno_expr_collect_nm(di) : (di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect_nm(di);
    return sno_lead_name("*", bn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_kw_chase(const scx_t * cx, const char * nm) { for (const sno_kw_link_t * l = cx->kw_chain; l; l = l->up) if (!strcmp(l->nm, nm)) return 1; return 0; }
static int sno_kw_nest_ok(const scx_t * cx, const char * nm) {
    if (g_lower_sno.sno_kw_nest_ok_nn < 0) { const char * e = getenv("SCRIP_CONST_NEST"); g_lower_sno.sno_kw_nest_ok_nn = (e && *e == '0') ? 0 : 1; }
    return g_lower_sno.sno_kw_nest_ok_nn ? !sno_kw_chase(cx, nm) : cx->kw_chain == NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail) {
    IR_graph_t * g = cx->g;
    const int pp = cx->ppos;
    cx->ppos = 0;
    if (!t) return succ;
    switch (sno_pat_eff_kind(t)) {
        case TT_QLIT:
        { IR_t * nd = lc_build(g, IR_MATCH_LIT, succ, NULL); sno_ω_to(nd, fail); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; return nd; }
        case TT_ANY:
        case TT_NOTANY:
        {
            IR_t * nd = lc_build(g, (t->t == TT_ANY) ? IR_MATCH_ANY : IR_MATCH_NOTANY, succ, NULL);
            sno_ω_to(nd, fail);
            const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
            if (cs) { IR_LIT(nd).sval = (char *) cs; return nd; }
            {
                const tree_t * arg = (t->n > 0) ? t->c[0] : NULL;
                if (arg && arg->t == TT_DEFER && arg->n > 0 && arg->c[0]) { IR_LIT(nd).sval = (char *) sno_pat_defer_arg_name(arg->c[0], 0); nd->pat_static = 1; return nd; }
            }
            sno_pre_req(cx, t, nd);
            return nd;
        }
        case TT_FAIL:
        { IR_t * j = lc_build(g, IR_GOTO, NULL, NULL); sno_ω_to(j, fail); if (fail) lc_γ_to_β(j, fail); else lc_γ_to(j, fail); return j; }
        case TT_SUCCEED:
        { IR_t * j = lc_build(g, IR_GOTO, succ, NULL); return j; }
        case TT_ABORT:
        { IR_t * j = lc_build(g, IR_MATCH_ABORT, NULL, NULL); IR_t * k = cx->pat_seal ? cx->pat_seal : fail; sno_ω_to(j, k); lc_γ_to(j, k); return j; }
        case TT_SPAN:
        {
            IR_t * nd = lc_build(g, IR_MATCH_SPAN, succ, NULL);
            sno_ω_to(nd, fail);
            const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
            if (cs) { IR_LIT(nd).sval = (char *) cs; return nd; }
            {
                const tree_t * arg = (t->n > 0) ? t->c[0] : NULL;
                if (arg && arg->t == TT_DEFER && arg->n > 0 && arg->c[0]) { IR_LIT(nd).sval = (char *) sno_pat_defer_arg_name(arg->c[0], 0); nd->pat_static = 1; return nd; }
            }
            sno_pre_req(cx, t, nd);
            return nd;
        }
        case TT_BREAK:
        case TT_BREAKX:
        {
            IR_t * nd = lc_build(g, (t->t == TT_BREAK) ? IR_MATCH_BREAK : IR_MATCH_BREAKX, succ, NULL);
            sno_ω_to(nd, fail);
            const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
            if (cs) { IR_LIT(nd).sval = (char *) cs; return nd; }
            {
                const tree_t * arg = (t->n > 0) ? t->c[0] : NULL;
                if (arg && arg->t == TT_DEFER && arg->n > 0 && arg->c[0]) { IR_LIT(nd).sval = (char *) sno_pat_defer_arg_name(arg->c[0], 0); nd->pat_static = 1; return nd; }
            }
            sno_pre_req(cx, t, nd);
            return nd;
        }
        case TT_TAB:
        case TT_RTAB:
        {
            IR_t * nd = lc_build(g, (t->t == TT_TAB) ? IR_MATCH_TAB : IR_MATCH_RTAB, succ, NULL);
            sno_ω_to(nd, fail);
            if (t->n <= 0 || !t->c[0]) sno_fatal("TAB/RTAB requires a count argument", NULL);
            if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
            if (t->c[0]->t == TT_DEFER && t->c[0]->n > 0 && t->c[0]->c[0]) { IR_LIT(nd).sval = (char *) sno_pat_defer_arg_name(t->c[0]->c[0], 1); return nd; }
            if (t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
            sno_pre_req(cx, t, nd);
            return nd;
        }
        case TT_POS:
        case TT_RPOS:
        {
            IR_t * nd = lc_build(g, (t->t == TT_RPOS) ? IR_MATCH_RPOS : IR_MATCH_POS, succ, NULL);
            sno_ω_to(nd, fail);
            if (t->n <= 0 || !t->c[0]) sno_fatal("POS/RPOS requires a position argument", NULL);
            if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
            if (t->c[0]->t == TT_DEFER && t->c[0]->n > 0 && t->c[0]->c[0]) { IR_LIT(nd).sval = (char *) sno_pat_defer_arg_name(t->c[0]->c[0], 1); return nd; }
            if (t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
            sno_pre_req(cx, t, nd);
            return nd;
        }
        case TT_FLUSH:
        { IR_t * F = lc_build(g, IR_MATCH_FENCE0, succ, NULL); sno_ω_to(F, fail); IR_LIT(F).ival = SNO_FENCE_LIT_FLUSH; return F; }
        case TT_FENCE:
        if (t->n > 0 && t->c[0]) {
            IR_t * F = lc_build(g, IR_MATCH_FENCE1, succ, NULL);
            sno_ω_to(F, fail);
            IR_LIT(F).ival = SNO_FENCE_LIT_ARG;
            int before_p = g->n;
            cx->ppos = 259;
            IR_t * pe = sno_pat_node(cx, t->c[0], F, F);
            IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
            for (int q = before_p; q < g->n; q++) {
                IR_t * x = g->all[q];
                if (!x) continue;
                if (x->ω.node == F) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                if (x->γ.node == F) { if (x->op == IR_GOTO && x->ω.node == F) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
            }
            ir_operand_push(F, pe);
            ir_operand_push(F, p_tail);
            { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(F, g->n); }
            return F;
        }
        if (t->n == 0) { IR_t * F = lc_build(g, IR_MATCH_FENCE0, succ, NULL); sno_ω_to(F, cx->pat_seal ? cx->pat_seal : fail); IR_LIT(F).ival = SNO_FENCE_LIT_BARE; return F; }
        return (t->n > 0 && t->c[0]) ? sno_pat_node(cx, t->c[0], succ, fail) : succ;
        case TT_DEFER:
        {
            const tree_t * in = (t->n > 0) ? t->c[0] : NULL;
            if (in && in->t == TT_VAR && in->v.sval) {
                IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL);
                IR_LIT(nd).sval = (char *) in->v.sval;
                sno_defer_seal(cx, nd);
                sno_ω_to(nd, fail);
                return nd;
            }
            if (in && in->t == TT_KEYWORD && in->v.sval) {
                if (g_lower_sno.sno_pat_node_cn < 0) { const char * e = getenv("SCRIP_CONST"); g_lower_sno.sno_pat_node_cn = (e && *e == '0') ? 0 : 1; }
                if (g_lower_sno.sno_pat_node_cn) {
                    char cb[fmt_len("&%s", in->v.sval[0] == '&' ? in->v.sval + 1 : in->v.sval)];
                    snprintf(cb, sizeof cb, "&%s", in->v.sval[0] == '&' ? in->v.sval + 1 : in->v.sval);
                    {
                        if (g_lower_sno.sno_pat_node_ci < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); g_lower_sno.sno_pat_node_ci = (e && *e == '0') ? 0 : 1; }
                        if (g_lower_sno.sno_pat_node_ci) {
                            const tree_t * cp0 = sno_const_pat(cb);
                            if (cp0 && g_lower_sno.pat_match_ctx && !g_lower_sno.in_patproc && sno_kw_nest_ok(cx, cb) && sno_pat_inline_ok(cp0)) {
                                sno_kw_link_t l0 = { cb, cx->kw_chain };
                                cx->kw_chain = &l0;
                                IR_t * r0 = sno_pat_node(cx, cp0, succ, fail);
                                cx->kw_chain = l0.up;
                                return r0;
                            }
                        }
                    }
                    IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL);
                    IR_LIT(nd).sval = lp_strdup(cb);
                    sno_defer_seal(cx, nd);
                    { const tree_t * cp = sno_const_pat(cb); if (cp) nd->pat_static = sno_pat_dfree(cp, 1, 0); if (cp) nd->seal = 2; }
                    sno_ω_to(nd, fail);
                    return nd;
                }
            }
            {
                const char * bn = sno_expr_collect(in);
                char pb[fmt_len("*%s", bn)];
                snprintf(pb, sizeof pb, "*%s", bn);
                IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL);
                IR_LIT(nd).sval = lp_strdup(pb);
                sno_defer_seal(cx, nd);
                sno_ω_to(nd, fail);
                return nd;
            }
        }
        case TT_KEYWORD:
        {
            if (!t->v.sval) return succ;
            char cb[fmt_len("&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval)];
            snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
            { const tree_t * cv = sno_const_val(cb); if (cv && cv->t == TT_QLIT) return sno_pat_node(cx, cv, succ, fail); }
            {
                if (g_lower_sno.sno_pat_node_ck < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); g_lower_sno.sno_pat_node_ck = (e && *e == '0') ? 0 : 1; }
                if (g_lower_sno.sno_pat_node_ck) {
                    const tree_t * cp = sno_const_pat(cb);
                    if (cp && g_lower_sno.pat_match_ctx && !g_lower_sno.in_patproc && sno_kw_nest_ok(cx, cb) && sno_pat_inline_ok(cp)) {
                        sno_kw_link_t l = { cb, cx->kw_chain };
                        cx->kw_chain = &l;
                        IR_t * r = sno_pat_node(cx, cp, succ, fail);
                        cx->kw_chain = l.up;
                        return r;
                    }
                }
            }
            IR_t * mv = lc_build(g, IR_MATCH_DEFER, succ, NULL);
            sno_defer_seal(cx, mv);
            sno_ω_to(mv, fail);
            if (cx->npre >= 0 && cx->npre < 64) {
                cx->pre[cx->npre].arg = t;
                cx->pre[cx->npre].prim = mv;
                cx->pre[cx->npre].str = 0;
                cx->pre[cx->npre].codes = 0;
                cx->pre[cx->npre].snapg = lp_strdup(cb);
                cx->pre[cx->npre].ppos = 0;
                cx->npre++;
            }
            return mv;
        }
        case TT_VAR:
        {
            IR_t * mv = lc_build(g, IR_MATCH_DEFER, succ, NULL);
            sno_defer_seal(cx, mv);
            sno_ω_to(mv, fail);
            if (cx->npre >= 0 && cx->npre < 64) {
                cx->pre[cx->npre].arg = t;
                cx->pre[cx->npre].prim = mv;
                cx->pre[cx->npre].str = 0;
                cx->pre[cx->npre].codes = 0;
                cx->pre[cx->npre].snapg = t->v.sval;
                cx->pre[cx->npre].ppos = pp;
                cx->npre++;
            }
            return mv;
        }
        case TT_REM:
        { IR_t * nd = lc_build(g, IR_MATCH_REM, succ, NULL); sno_ω_to(nd, fail); return nd; }
        case TT_CAPT_CURSOR:
        {
            const tree_t * ctg = (t->n > 0) ? t->c[0] : NULL;
            const char * cvn = sno_cursor_target(ctg);
            const int ek = !cvn && sno_elem_here(cx, ctg, 1);
            if (ek) cvn = "PAT$0$V0";
            if (!cvn) sno_fatal("@ cursor-position capture target is not a simple variable", NULL);
            IR_t * nd = lc_build(g, IR_MATCH_ATP, succ, NULL);
            IR_LIT(nd).sval = (char *) cvn;
            sno_ω_to(nd, fail);
            if (ek) sno_elem_pre(cx, ctg, nd, NULL);
            else sno_capt_ordasg(cx, ctg);
            return nd;
        }
        case TT_ARB:
        { IR_t * nd = lc_build(g, IR_MATCH_ARB, succ, NULL); sno_ω_to(nd, fail); return nd; }
        case TT_BAL:
        { IR_t * nd = lc_build(g, IR_MATCH_BAL, succ, NULL); sno_ω_to(nd, fail); return nd; }
        case TT_ARBNO:
        {
            if (!(t->n > 0) || !t->c[0]) sno_fatal("ARBNO requires a pattern argument", NULL);
            IR_t * R = lc_build(g, IR_MATCH_ARBNO, succ, NULL);
            sno_ω_to(R, fail);
            int before = g->n;
            g_lower_sno.in_arbno++;
            cx->seq_rlast = NULL;
            cx->seq_rlast_for = NULL;
            cx->ppos = 61;
            IR_t * ei = sno_pat_node(cx, t->c[0], R, R);
            IR_t * rl = (cx->seq_rlast && cx->seq_rlast_for == ei) ? cx->seq_rlast : NULL;
            IR_t * rtl = (rl && cx->seq_rtail) ? cx->seq_rtail : NULL;
            g_lower_sno.in_arbno--;
            if (before >= g->n) sno_fatal("ARBNO body lowered to zero nodes (bare FENCE / null pattern body)", NULL);
            int _rb = before;
            while (_rb < g->n && g->all[_rb] && g->all[_rb]->op == IR_GOTO && g->all[_rb]->γ.node == R && g->all[_rb]->n_operands == 0) _rb++;
            IR_t * ri = (_rb < g->n) ? g->all[_rb] : ei;
            for (int k = before; k < g->n; k++) {
                IR_t * x = g->all[k];
                if (!x) continue;
                if (x->ω.node == R) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                if (x->γ.node == R) { if (x->op == IR_GOTO && x->ω.node == R) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
            }
            ir_operand_push(R, ei);
            ir_operand_push(R, rl ? (rtl ? rtl : ei) : ri);
            if (rl) { char * vis = (char *) ct_zalloc((size_t)(g->n - before), 1); sno_emit_order_last(g, before, R, ei, vis, &rl); ct_drop(vis); }
            ir_operand_push(R, rl ? rl : ((g->n > before) ? g->all[g->n - 1] : ei));
            { sno_tvec_t bv = {0}; sno_seq_flatten_pat(t->c[0], &bv); if (bv.n > 0 && sno_is_fence0(bv.v[bv.n - 1])) ir_operand_push(R, R); ct_drop(bv.v); ct_drop(bv.rc); }
            IR_LIT(R).ival = 1;
            return R;
        }
        case TT_LEN:
        {
            IR_t * nd = lc_build(g, IR_MATCH_LEN, succ, NULL);
            sno_ω_to(nd, fail);
            if (t->n <= 0 || !t->c[0]) sno_fatal("LEN requires a count argument", NULL);
            if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
            if (t->c[0]->t == TT_DEFER && t->c[0]->n > 0 && t->c[0]->c[0]) { IR_LIT(nd).sval = (char *) sno_pat_defer_arg_name(t->c[0]->c[0], 1); return nd; }
            sno_pre_req(cx, t, nd);
            return nd;
        }
        case TT_CAPT_COND_ASGN:
        {
            const int ek = t->n > 1 && sno_elem_here(cx, t->c[1], 0);
            const char * vn = ek ? "PAT$0$V0" : (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
            if (vn && !ek) sno_reg_var(vn);
            if (!vn && t->n > 1) vn = sno_capt_item_name(t->c[1]);
            if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) {
                const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL;
                if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }
                if (!vn) {
                    const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval :
                        ((di && di->t == TT_INDIRECT && di->n > 0 && di->c[0]) || (di && di->t == TT_IDX && di->n > 0) || sno_item_argbase(di) >= 0) ? sno_expr_collect_nm(di) :
                        (di && di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect(di);
                    char pb[fmt_len("*%s", bn)];
                    snprintf(pb, sizeof pb, "*%s", bn);
                    vn = lp_strdup(pb);
                }
            }
            if (!vn || !(t->n > 0 && t->c[0])) sno_fatal("conditional capture target is not a simple variable (SN4-PAT-2 subset)", NULL);
            IR_t * nd = lc_build(g, IR_MATCH_ASSIGN_COND, succ, NULL);
            IR_LIT(nd).sval = (char *) vn;
            IR_t * save = lc_build(g, IR_MATCH_ASSIGN_SAVE, NULL, NULL);
            IR_LIT(save).sval = (char *) vn;
            sno_ω_to(save, fail);
            int before_i = g->n;
            IR_t * itail = NULL;
            cx->ppos = (pp && t->slen == 240) ? pp : 30;
            IR_t * pe = sno_capt_body(cx, t->c[0], nd, save, &itail);
            if (ek) sno_elem_pre(cx, t->c[1], nd, save);
            else sno_capt_ordasg(cx, t->n > 1 ? t->c[1] : NULL);
            lc_γ_to(save, pe);
            sno_ω_to(nd, itail);
            ir_operand_push(nd, pe);
            ir_operand_push(nd, save);
            { sno_cap_fc(g, nd, save, before_i); }
            { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(nd, g->n); }
            return save;
        }
        case TT_CAPT_IMMED_ASGN:
        {
            const int ek = t->n > 1 && sno_elem_here(cx, t->c[1], 0);
            const char * vn = ek ? "PAT$0$V0" : (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
            if (vn && !ek) sno_reg_var(vn);
            if (!vn && t->n > 1) vn = sno_capt_item_name(t->c[1]);
            if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) {
                const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL;
                if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }
                if (!vn) {
                    const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval :
                        ((di && di->t == TT_INDIRECT && di->n > 0 && di->c[0]) || (di && di->t == TT_IDX && di->n > 0) || sno_item_argbase(di) >= 0) ? sno_expr_collect_nm(di) :
                        (di && di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect(di);
                    char pb[fmt_len("*%s", bn)];
                    snprintf(pb, sizeof pb, "*%s", bn);
                    vn = lp_strdup(pb);
                }
            }
            if (!vn || !(t->n > 0 && t->c[0])) sno_fatal("immediate capture target is not a simple variable (SN4-PAT-2 subset)", NULL);
            IR_t * nd = lc_build(g, IR_MATCH_ASSIGN_IMM, succ, NULL);
            IR_LIT(nd).sval = (char *) vn;
            IR_t * save = lc_build(g, IR_MATCH_ASSIGN_SAVE, NULL, NULL);
            IR_LIT(save).sval = (char *) vn;
            sno_ω_to(save, fail);
            int before_i = g->n;
            IR_t * itail = NULL;
            cx->ppos = (pp && t->slen == 240) ? pp : 25;
            IR_t * pe = sno_capt_body(cx, t->c[0], nd, save, &itail);
            if (ek) sno_elem_pre(cx, t->c[1], nd, save);
            else sno_capt_ordasg(cx, t->n > 1 ? t->c[1] : NULL);
            lc_γ_to(save, pe);
            sno_ω_to(nd, itail);
            ir_operand_push(nd, pe);
            ir_operand_push(nd, save);
            { sno_cap_fc(g, nd, save, before_i); }
            { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(nd, g->n); }
            return save;
        }
        case TT_SEQ:
        {
            sno_tvec_t ev = {0};
            sno_seq_flatten_pat(t, &ev);
            const tree_t ** elems = ev.v;
            int ne = ev.n;
            int first_fence = ne;
            int first_f0 = ne;
            for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { first_fence = i; break; }
            for (int i = 0; i < ne; i++) if (sno_is_fence0(elems[i])) { first_f0 = i; break; }
            if (first_fence == ne) {
                IR_t * r0 = ne == 1 ? (cx->ppos = pp, sno_pat_node(cx, elems[0], succ, fail)) : sno_seq_nary(cx, elems, ev.rc, ne, succ, fail, NULL);
                ct_drop(ev.v);
                ct_drop(ev.rc);
                return r0;
            }
            IR_t * cur_succ = succ;
            IR_t * right_tail = NULL;
            int right_tail_idx = -1;
            int right_sealed = 0;
            IR_t * rlast = NULL;
            IR_t * rtail = NULL;
            int first_seg = 1;
            for (int i = ne - 1; i >= 0; ) {
                if (sno_is_fence(elems[i])) {
                    const tree_t * inner = sno_is_fence1(elems[i]) ? elems[i]->c[0] : NULL;
                    if (!inner) right_sealed = 1;
                    if (inner && g_lower_sno.in_arbno) {
                        IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                        int f_idx = g->n;
                        IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                        sno_ω_to(F, fail_p);
                        IR_LIT(F).ival = SNO_FENCE_LIT_ARG_IN_ARBNO;
                        int before_p = g->n;
                        cx->ppos = 259;
                        IR_t * pe = sno_pat_node(cx, inner, F, F);
                        IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
                        for (int q = before_p; q < g->n; q++) {
                            IR_t * x = g->all[q];
                            if (!x) continue;
                            if (x->ω.node == F) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                            if (x->γ.node == F) { if (x->op == IR_GOTO && x->ω.node == F) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
                        }
                        ir_operand_push(F, pe);
                        ir_operand_push(F, p_tail);
                        { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(F, g->n); }
                        if (first_seg) rtail = F;
                        if (right_tail && !right_sealed) sno_resume_ω_to(g, right_tail_idx, right_tail, F);
                        cur_succ = F;
                        right_tail = F;
                        right_tail_idx = f_idx;
                        right_sealed = 0;
                    } else if (inner && !g_lower_sno.in_arbno) {
                        IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                        int f_idx = g->n;
                        IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                        sno_ω_to(F, fail_p);
                        IR_LIT(F).ival = SNO_FENCE_LIT_ARG;
                        int before_p = g->n;
                        cx->ppos = 259;
                        IR_t * pe = sno_pat_node(cx, inner, F, F);
                        IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
                        for (int q = before_p; q < g->n; q++) {
                            IR_t * x = g->all[q];
                            if (!x) continue;
                            if (x->ω.node == F) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                            if (x->γ.node == F) { if (x->op == IR_GOTO && x->ω.node == F) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
                        }
                        ir_operand_push(F, pe);
                        ir_operand_push(F, p_tail);
                        { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(F, g->n); }
                        if (right_tail && !right_sealed) sno_resume_ω_to(g, right_tail_idx, right_tail, F);
                        cur_succ = F;
                        right_tail = F;
                        right_tail_idx = f_idx;
                        right_sealed = 0;
                    } else if (i > 0 || sno_is_flush(elems[i])) {
                        IR_t * fail_p = cx->pat_seal ? cx->pat_seal : fail;
                        int f_idx = g->n;
                        IR_t * F = lc_build(g, IR_MATCH_FENCE0, cur_succ, NULL);
                        sno_ω_to(F, fail_p);
                        IR_LIT(F).ival = sno_is_flush(elems[i]) ? SNO_FENCE_LIT_FLUSH : SNO_FENCE_LIT_BARE;
                        cur_succ = F;
                        right_tail = F;
                        right_tail_idx = f_idx;
                    }
                    if (!rlast && g->n > 0) rlast = g->all[g->n - 1];
                    first_seg = 0;
                    i--;
                    continue;
                }
                int j = i;
                while (j > 0 && !sno_is_fence(elems[j - 1])) j--;
                int rn = i - j + 1;
                IR_t * fail_r = (j > first_f0) ? cx->pat_seal : fail;
                int before_r = g->n;
                IR_t * n_rt = NULL;
                IR_t * re = (rn == 1) ? (cx->ppos = ev.rc[j] ? 9 : 8, sno_pat_node(cx, elems[j], cur_succ, fail_r)) : sno_seq_nary(cx, elems + j, ev.rc + j, rn, cur_succ, fail_r, &n_rt);
                int _rb2 = before_r;
                while (_rb2 < g->n && g->all[_rb2] && g->all[_rb2]->op == IR_GOTO && g->all[_rb2]->n_operands == 0) _rb2++;
                IR_t * r_ti = NULL;
                if (!n_rt && sno_seq_tail()) for (int k = before_r; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->γ.node == cur_succ && !(x->op == IR_GOTO && x->n_operands == 0)) r_ti = x; }
                IR_t * r_tail = n_rt ? n_rt : r_ti ? r_ti : ((_rb2 < g->n) ? g->all[_rb2] : re);
                if (right_tail && !right_sealed && before_r < g->n) sno_resume_ω_to(g, right_tail_idx, right_tail, r_tail);
                cur_succ = re;
                right_tail = (n_rt && _rb2 < g->n) ? g->all[_rb2] : r_tail;
                right_tail_idx = (n_rt && _rb2 < g->n) ? _rb2 : before_r;
                right_sealed = 0;
                if (!rlast && g->n > 0) rlast = g->all[g->n - 1];
                if (first_seg) rtail = r_tail;
                first_seg = 0;
                i = j - 1;
            }
            ct_drop(ev.v);
            ct_drop(ev.rc);
            cx->seq_rlast = rlast;
            cx->seq_rlast_for = cur_succ;
            cx->seq_rtail = rtail;
            return cur_succ;
        }
        case TT_ALT:
        {
            int nr = 0;
            const tree_t * cur = t;
            while (cur && cur->t == TT_ALT) { nr++; cur = (cur->n > 0) ? cur->c[0] : NULL; }
            int na = nr + 1;
            const tree_t ** alts = (const tree_t **) ct_alloc((size_t) na * sizeof *alts);
            alts[0] = cur;
            cur = t;
            for (int i = nr; i >= 1; i--) { alts[i] = (cur->n > 1) ? cur->c[1] : NULL; cur = (cur->n > 0) ? cur->c[0] : NULL; }
            if (na == 1) { const tree_t * only = alts[0]; ct_drop(alts); cx->ppos = pp; return sno_pat_node(cx, only, succ, fail); }
            IR_t * A = lc_build(g, IR_MATCH_ALTERNATE, succ, NULL);
            sno_ω_to(A, fail);
            (void)0;
            int fc_fp[na];
            int fc_ab[na];
            int fc_ae[na];
            int fc_linear = (na <= 10);
            for (int i = 0; i < na; i++) {
                int before = g->n;
                cx->ppos = (i == na - 1) ? 5 : 6;
                IR_t * ei = sno_pat_node(cx, alts[i], A, A);
                IR_t * ri = (before < g->n) ? g->all[before] : ei;
                IR_t * ti = NULL;
                int fp_i = 0;
                for (int k = before; k < g->n; k++) {
                    IR_t * x = g->all[k];
                    if (!x) continue;
                    if (x->ω.node == A) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                    if (x->γ.node == A) { if (x->op == IR_GOTO && x->ω.node == A) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); ti = x; } x->γ.sz[3] = 0; }
                }
                if (sno_alt_tail() && ti) ri = ti;
                if (!fc_walk_range(g, before, g->n, 0, &fp_i)) fc_linear = 0;
                fc_fp[i] = fp_i;
                fc_ab[i] = before;
                fc_ae[i] = g->n;
                ir_operand_push(A, ei);
                ir_operand_push(A, ri);
            }
            if (fc_linear) {
                extern void fc_alt_register(const IR_t *, int, const int *, const int *, const int *);
                extern void fc_arm_member_register(const IR_t *);
                fc_alt_register(A, (int)na, fc_fp, fc_ab, fc_ae);
                for (int _j = 0; _j < (int)na; _j++) for (int _k = fc_ab[_j]; _k < fc_ae[_j] && _k < g->n; _k++) if (g->all[_k]) fc_arm_member_register(g->all[_k]);
            }
            IR_LIT(A).ival = (long)na;
            ct_drop(alts);
            return A;
        }
        case TT_FNC:
        {
            const char * name = t->v.sval;
            int argbase = 0;
            if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
            {
                int lk = sno_lambda_kind(name, t->n - argbase);
                if (lk) {
                    const tree_t * ex = (t->n > argbase) ? t->c[argbase] : NULL;
                    if (lk == 2)
                        sno_fatal(
                        "lambda(expr), the CONDITIONAL pattern lambda, is not implemented yet -- the IMMEDIATE form LAMBDA(expr) is; the conditional form queues a thunk on the dcap frame and that ar"
                        "m is unlanded", NULL);
                    IR_t * box = lc_build(g, IR_MATCH_LAMBDA, succ, NULL);
                    sno_ω_to(box, fail);
                    IR_LIT(box).ival = 0;
                    { IR_t * val = NULL; IR_t * entry = sx_lower(cx, ex, NULL, fail, &val); lc_γ_to(val, box); return entry ? entry : box; }
                }
            }
            static const struct {
                const char * n;
                tree_e k;
            } pm[] = { {"ANY",TT_ANY},{"NOTANY",TT_NOTANY},{"SPAN",TT_SPAN},{"BREAK",TT_BREAK},{"BREAKX",TT_BREAKX},{"LEN",TT_LEN},{"POS",TT_POS},{"RPOS",TT_RPOS},{"TAB",TT_TAB},{"RTAB",TT_RTAB},
                {"ARB",TT_ARB},{"ARBNO",TT_ARBNO},{"REM",TT_REM},{"FAIL",TT_FAIL},{"SUCCEED",TT_SUCCEED},{"FENCE",TT_FENCE},{"ABORT",TT_ABORT},{"BAL",TT_BAL},{NULL,TT_VAR} };
            tree_e pk = TT_VAR;
            if (name && !sno_predef_registered(name)) for (int i = 0; pm[i].n; i++) if (!strcmp(name, pm[i].n)) { pk = pm[i].k; break; }
            if (pk != TT_VAR) {
                extern tree_t * ast_stmt_new(tree_e kind);
                tree_t * syn = ast_stmt_new(pk);
                for (int k = argbase; k < t->n; k++) ast_push(syn, (tree_t *) t->c[k]);
                return sno_pat_node(cx, syn, succ, fail);
            }
            {
                const tree_t * ca = (name && t->n - argbase == 1 && !strcmp(name, "CHAR") && !sno_predef_registered(name)) ? t->c[argbase] : NULL;
                if (ca && ca->t == TT_ILIT && ca->v.ival >= 1 && ca->v.ival <= 255) {
                    tree_t * q = ast_node_new(TT_QLIT);
                    char * cb = (char *) ct_alloc(2);
                    cb[0] = (char)(unsigned char) ca->v.ival;
                    cb[1] = 0;
                    q->v.sval = cb;
                    return sno_pat_node(cx, q, succ, fail);
                }
            }
            {
                if (g_lower_sno.sno_pat_node_ec < 0) { const char * e = getenv("SCRIP_PAT_EAGER_CALL"); g_lower_sno.sno_pat_node_ec = (!e || *e != '0') ? 1 : 0; }
                if (g_lower_sno.sno_pat_node_ec && cx->npre >= 0 && cx->npre < 64) {
                    IR_t * mvd = lc_build(g, IR_MATCH_DEFER, succ, NULL);
                    sno_defer_seal(cx, mvd);
                    sno_ω_to(mvd, fail);
                    cx->pre[cx->npre].arg = t;
                    cx->pre[cx->npre].prim = mvd;
                    cx->pre[cx->npre].str = 0;
                    cx->pre[cx->npre].codes = 0;
                    cx->pre[cx->npre].snapg = name ? name : "$fnc";
                    cx->pre[cx->npre].ppos = 0;
                    cx->npre++;
                    return mvd;
                }
            }
            IR_t * mv = lc_build(g, IR_MATCH_VALUE, succ, NULL);
            sno_ω_to(mv, fail);
            IR_t * vr = NULL;
            IR_t * ec = sx_lower(cx, t, mv, fail, &vr);
            if (vr) ir_operand_push(mv, vr);
            return ec;
        }
        default:
        sno_fatal("pattern element not in the SN4-PAT subset (LEN, literal, ANY, NOTANY, SPAN, BREAK, BREAKX, TAB, RTAB, POS, RPOS, REM, ARB; SEQ+ALT landed SN4-PAT-3h)", NULL);
    }
    return succ;
}
