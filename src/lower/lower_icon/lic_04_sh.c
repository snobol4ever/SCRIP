#include "lower/lower_icon/lic_head.h"
#include <string.h>
#include "runtime/core/g_runtime.h"
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "runtime/builtins/gen_runtime.h"
#include "lower/lower_common.h"
#include "lower/lc_vec.h"
#include "lower/lower_icon/lic_01_sh.h"
#include "lower/lower_icon/lic_03_sh.h"
#include "lower/lower_icon/lic_04_sh.h"
static IR_t * lower_arm(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv);
int icn_gen_wiring(const IR_t * t) {
    if (!t) return 0;
    if (ir_is_generator_kind(t->op)) return 1;
    if (t->op == IR_SCAN_TAB || t->op == IR_SCAN_MOVE) return 1;
    return t->op == IR_CALL && IR_LIT(t).sval && (!strcmp(IR_LIT(t).sval, "tab") || !strcmp(IR_LIT(t).sval, "move"));
}
void lic_γ_to(IR_t * nd, IR_t * t) { if (t && icn_gen_wiring(t)) lc_γ_to_β(nd, t); else lc_γ_to(nd, t); }
void lic_ω_to(IR_t * nd, IR_t * t) { if (t && icn_gen_wiring(t)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_tree_is_cursor_mover(const tree_t * a) {
    if (!a) return 0;
    if (a->t == TT_STMT) a = lic_stmt_subj(a);
    if (!a) return 0;
    if (a->t == TT_MATCH_UNARY) return 1;
    if ((a->t == TT_REVASSIGN || a->t == TT_ASSIGN) && a->n >= 2) return icn_tree_is_cursor_mover(a->c[1]);
    if (a->t != TT_FNC) return 0;
    const char * nm = (a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR) ? a->c[0]->v.sval : NULL;
    return nm && (!strcmp(nm, "tab") || !strcmp(nm, "move"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_scan_kind_for(const char * s) {
    if (!s) return 0;
    if (!strcmp(s,"tab")) return (int)IR_SCAN_TAB;
    if (!strcmp(s,"move")) return (int)IR_SCAN_MOVE;
    if (!strcmp(s,"upto")) return (int)IR_SCAN_UPTO;
    if (!strcmp(s,"any")) return (int)IR_SCAN_ANY;
    if (!strcmp(s,"many")) return (int)IR_SCAN_MANY;
    if (!strcmp(s,"find")) return (int)IR_SCAN_FIND;
    if (!strcmp(s,"match")) return (int)IR_SCAN_MATCH;
    if (!strcmp(s,"pos")) return (int)IR_SCAN_POS;
    if (!strcmp(s,"bal")) return (int)IR_SCAN_BAL;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_retag_scan_body(icx_t * cx, int lo) {
    IR_graph_t * g = cx ? cx->g : (IR_graph_t *)0;
    if (!g || !g->all || lo < 0) return;
    for (int i = lo; i < g->n; i++) {
        IR_t * nd = g->all[i];
        const char * nm;
        if (!nd) continue;
        nm = IR_LIT(nd).sval;
        if ((nd->op == IR_CALL || nd->op == IR_CALL_BUILTIN || nd->op == IR_CALL_ICON || nd->op == IR_CALL_BUILTIN_GEN) && nm && nd->n_operands == 1) {
            int k = icn_scan_kind_for(nm);
            if (k && !icn_is_proc(nm) && !icn_builtin_reassigned(nm) && !icn_is_local(cx, nm) && !icn_is_own_global(cx, nm)) nd->op = (IR_e) k;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_tree_is_literal(const tree_t * t) { return t && (t->t == TT_QLIT || t->t == TT_ILIT || t->t == TT_FLIT || t->t == TT_CSET); }
IR_t * lower_runerr_111(icx_t * cx, const tree_t * other, const tree_t * bad, IR_t * γ, IR_t * ω, IR_t ** res) {
    tree_t * call = ast_node_new(TT_FNC);
    tree_t * fnv = ast_node_new(TT_VAR);
    fnv->v.sval = (char *) "runerr";
    ast_push(call, fnv);
    tree_t * code = ast_node_new(TT_ILIT);
    code->v.ival = 111;
    ast_push(call, code);
    ast_push(call, (tree_t *) bad);
    tree_t * seq = ast_node_new(TT_CONJ);
    if (other) ast_push(seq, (tree_t *) other);
    ast_push(seq, call);
    return lic_lower(cx, seq, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *
    lower_scan_impl(icx_t * cx, const tree_t * subj_t, const tree_t * lv_t, const tree_t * body_t, const tree_t * wi_t, const tree_t * wj_t, IR_t * γ, IR_t * ω, IR_t ** res, IR_t ** lv_out) {
    IR_t * enter = lic_build(cx, IR_SCAN_ENTER, NULL, ω);
    IR_t * leave_succ = lic_build(cx, IR_SCAN, γ, ω);
    IR_t * leave_fail = lic_build(cx, IR_SCAN, ω, ω);
    icn_mark_γ_fail_conduit(leave_fail);
    ir_operand_push(leave_succ, enter);
    ir_operand_push(leave_fail, enter);
    IR_t * succ_tramp = IR_node_alloc(cx->g, IR_GOTO);
    lc_γ_to(succ_tramp, leave_succ);
    lc_ω_to(succ_tramp, leave_succ);
    IR_t * fail_tramp = IR_node_alloc(cx->g, IR_GOTO);
    lc_γ_to(fail_tramp, leave_fail);
    lc_ω_to(fail_tramp, leave_fail);
    int scan_body_lo = cx->g->n;
    icn_scan_fr sf = { enter, cx->scan_top };
    cx->scan_top = &sf;
    cx->scan_sp++;
    IR_t * bv = NULL;
    IR_t * b_entry = lic_lower(cx, body_t, succ_tramp, fail_tramp, &bv);
    cx->scan_sp--;
    cx->scan_top = sf.up;
    if (IR_LIT(enter).dval == 3.0) { IR_LIT(leave_succ).dval = 3.0; IR_LIT(leave_fail).dval = 3.0; }
    IR_t * body_beta = cx->beta;
    if (bv) ir_operand_push(leave_succ, bv);
    icn_retag_scan_body(cx, scan_body_lo);
    lc_γ_to(enter, b_entry);
    cx->beta = ω;
    IR_t * sr = NULL;
    IR_t * s_entry = NULL;
    IR_t * wr[2] = { NULL, NULL };
    if (subj_t && (wi_t || wj_t)) {
        const tree_t * src[3] = { subj_t, wi_t, wj_t };
        IR_t * rr[3] = { NULL, NULL, NULL };
        IR_t * prev = NULL;
        IR_t * aω = ω;
        for (int k = 0; k < 3; k++) {
            if (!src[k]) continue;
            IR_t * ar = NULL;
            cx->beta = aω;
            IR_t * ae = lic_lower(cx, src[k], NULL, aω, &ar);
            aω = cx->beta;
            if (!s_entry) s_entry = ae;
            if (prev) lc_γ_to(prev, ae);
            prev = ar;
            rr[k] = ar;
        }
        lc_γ_to(prev, enter);
        sr = rr[0];
        wr[0] = rr[1];
        wr[1] = rr[2];
    } else if (subj_t) s_entry = lic_lower(cx, subj_t, enter, ω, &sr);
    else {
        IR_t * lv = NULL;
        s_entry = lower_lvalue_var(cx, lv_t, ω, &lv);
        if (!s_entry || !lv) return NULL;
        IR_t * dr = lic_build(cx, IR_DEREF, enter, ω);
        ir_operand_push(dr, lv);
        lc_γ_to(lv, dr);
        sr = dr;
        if (lv_out) *lv_out = lv;
    }
    ir_operand_push(enter, sr);
    if (wi_t || wj_t) { ir_operand_push(enter, wr[0]); ir_operand_push(enter, wr[1]); }
    if (sr && sr->op == IR_SCAN && IR_LIT(sr).dval != 3.0) IR_LIT(sr).dval = 4.0;
    IR_t * subj_beta = cx->beta;
    if (subj_beta && subj_beta != ω) { lic_γ_to(leave_fail, subj_beta); lic_ω_to(leave_fail, subj_beta); }
    int body_resumes = (bv && icn_gen_wiring(bv)) || (body_beta && body_beta != ω && body_beta != fail_tramp && body_beta != succ_tramp);
    if (body_resumes && !(bv && icn_gen_wiring(bv))) ir_operand_push(leave_succ, body_beta);
    cx->beta = body_resumes ? leave_succ : ((subj_beta && subj_beta != ω) ? subj_beta : ω);
    {
        const tree_t * bt = body_t;
        while (bt && ((bt->t == TT_CONJ && bt->n >= 2) || (bt->t == TT_SEQ_EXPR && bt->n >= 1) || bt->t == TT_STMT)) bt = (bt->t == TT_STMT) ? lic_stmt_subj(bt) : bt->c[bt->n - 1];
        if (icn_tree_is_kw_var(bt)) { IR_t * kr = lic_build(cx, IR_KW_ICON, γ, ω); IR_LIT(kr).sval = (char *) bt->v.sval; lc_γ_to(leave_succ, kr); *res = kr; return s_entry; }
    }
    *res = leave_succ;
    return s_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long icn_first_line(const tree_t * t) { if (!t) return 0; if (t->line > 0) return t->line; for (int i = 0; i < t->n; i++) { long l = icn_first_line(t->c[i]); if (l > 0) return l; } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_has_rec(const tree_t * t) {
    if (!t) return 0;
    switch (t->t) {
        case TT_VAR:
        if (t->v.sval && t->v.sval[0] == '&') return 1;
        break;
        case TT_ILIT:
        case TT_FLIT:
        case TT_QLIT:
        case TT_CSET:
        case TT_NUL:
        case TT_ALTERNATE:
        case TT_REPALT:
        case TT_LOOP_BREAK:
        case TT_LOOP_NEXT:
        case TT_CASE:
        case TT_IF:
        case TT_UNLESS:
        case TT_BANG_BINARY:
        case TT_NOT:
        case TT_SEQ_EXPR:
        case TT_EVERY:
        case TT_WHILE:
        case TT_UNTIL:
        case TT_REPEAT:
        case TT_STMT:
        break;
        default:
        return 1;
    }
    for (int i = 0; i < t->n; i++) if (icn_has_rec(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long icn_part_line(const tree_t * t) { return icn_has_rec(t) ? icn_first_line(t) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_parts_split(long a, long b) { return a > 0 && b > 0 && a != b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_part_mark(icx_t * cx, long line, IR_t * next, int back) {
    IR_t * m = lic_build(cx, IR_LINE_MARK, NULL, NULL);
    m->pat_static = line;
    if (back) { lic_γ_to(m, next); lic_ω_to(m, next); } else { lc_γ_to_α(m, next); lc_ω_to_α(m, next); }
    if (cx->links || cx->want_lines) IR_LIT(m).sval = (char *) cx->file;
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_tree_has_loop_ctl(const tree_t * n) {
    if (!n) return 0;
    if (n->t == TT_LOOP_BREAK || n->t == TT_LOOP_NEXT) return 1;
    for (int i = 0; i < n->n; i++) if (icn_tree_has_loop_ctl(n->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lic_lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL;
    if (C && (C->t == TT_SUSPEND || C->t == TT_RETURN || C->t == TT_PROC_FAIL) && !icn_tree_has_loop_ctl(C)) { IR_t * cv = NULL; IR_t * ce = lic_lower(cx, C, ω, ω, &cv); *res = cv; return ce; }
    const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    long cl = icn_first_line(C), bl = icn_part_line(B);
    int split = icn_parts_split(cl, bl);
    IR_t * W = lic_build(cx, IR_GOTO, ω, ω);
    lic_γ_to(W, ω);
    lic_ω_to(W, ω);
    IR_t * slb = cx->loop_break_beta;
    cx->loop_break_beta = NULL;
    IR_t * slf = cx->loop_fail;
    cx->loop_fail = ω;
    IR_t * sle = cx->loop_exit;
    IR_t * sln = cx->loop_next;
    int slns = cx->loop_next_ssp;
    IR_t * bres = lic_build(cx, IR_VAR, γ, ω);
    IR_LIT(bres).sval = (char *) "__break_result";
    cx->loop_exit = bres;
    icn_loop_fr lf = { cx->loop_exit, NULL, cx->loop_fail, cx->loop_top };
    cx->loop_top = &lf;
    cx->loop_sp++;
    IR_t * BENT = lic_build(cx, IR_GOTO, γ, ω);
    IR_t * cval = NULL;
    IR_t * centry = lic_lower(cx, C, BENT, W, &cval);
    IR_t * mark = lic_build(cx, IR_BOUND, NULL, NULL);
    lc_γ_to_α(mark, centry);
    lc_ω_to_α(mark, centry);
    IR_t * CENT = lic_build(cx, IR_GOTO, NULL, NULL);
    lc_γ_to_α(CENT, mark);
    lc_ω_to_α(CENT, mark);
    IR_t * back = split ? icn_part_mark(cx, cl, CENT, 0) : CENT;
    IR_t * unmk = lic_build(cx, IR_UNMARK, back, back);
    ir_operand_push(unmk, mark);
    cx->loop_next = unmk;
    cx->loop_next_ssp = cx->scan_sp;
    lf.next = unmk;
    IR_t * bval = NULL;
    cx->discard = B;
    IR_t * b_entry = lic_lower(cx, B, unmk, unmk, &bval);
    if (split) b_entry = icn_part_mark(cx, bl, b_entry, 0);
    cx->loop_sp--;
    cx->loop_top = lf.up;
    lc_γ_to(BENT, b_entry);
    lc_ω_to(BENT, b_entry);
    cx->loop_exit = sle;
    cx->loop_next = sln;
    cx->loop_fail = slf;
    cx->loop_next_ssp = slns;
    { IR_t * lbb = cx->loop_break_beta; cx->loop_break_beta = slb; if (lbb) { cx->beta = lbb; *res = bres; return CENT; } }
    *res = bres;
    return CENT;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    long cl = icn_first_line(C), bl = icn_part_line(B);
    int split = icn_parts_split(cl, bl);
    IR_t * U = lic_build(cx, IR_GOTO, ω, ω);
    lic_γ_to(U, ω);
    lic_ω_to(U, ω);
    IR_t * BENT = lic_build(cx, IR_GOTO, γ, ω);
    IR_t * slb = cx->loop_break_beta;
    cx->loop_break_beta = NULL;
    IR_t * slf = cx->loop_fail;
    cx->loop_fail = ω;
    IR_t * sle = cx->loop_exit;
    IR_t * sln = cx->loop_next;
    int slns = cx->loop_next_ssp;
    IR_t * bres = lic_build(cx, IR_VAR, γ, ω);
    IR_LIT(bres).sval = (char *) "__break_result";
    cx->loop_exit = bres;
    IR_t * cval = NULL;
    IR_t * centry = lic_lower(cx, C, U, BENT, &cval);
    IR_t * mark = lic_build(cx, IR_BOUND, NULL, NULL);
    lc_γ_to_α(mark, centry);
    lc_ω_to_α(mark, centry);
    IR_t * CENT = lic_build(cx, IR_GOTO, NULL, NULL);
    lc_γ_to_α(CENT, mark);
    lc_ω_to_α(CENT, mark);
    IR_t * back = split ? icn_part_mark(cx, cl, CENT, 0) : CENT;
    IR_t * unmk = lic_build(cx, IR_UNMARK, back, back);
    ir_operand_push(unmk, mark);
    cx->loop_next = unmk;
    cx->loop_next_ssp = cx->scan_sp;
    IR_t * b_entry;
    if (B) {
        icn_loop_fr lf = { cx->loop_exit, cx->loop_next, cx->loop_fail, cx->loop_top };
        cx->loop_top = &lf;
        cx->loop_sp++;
        IR_t * bval = NULL;
        cx->discard = B;
        b_entry = lic_lower(cx, B, unmk, unmk, &bval);
        cx->loop_sp--;
        cx->loop_top = lf.up;
    } else b_entry = unmk;
    if (split && B) b_entry = icn_part_mark(cx, bl, b_entry, 0);
    lc_γ_to(BENT, b_entry);
    lc_ω_to(BENT, b_entry);
    cx->loop_exit = sle;
    cx->loop_next = sln;
    cx->loop_fail = slf;
    cx->loop_next_ssp = slns;
    { IR_t * lbb = cx->loop_break_beta; cx->loop_break_beta = slb; if (lbb) { cx->beta = lbb; *res = bres; return CENT; } }
    *res = bres;
    return CENT;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lic_lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
    IR_t * H = lic_build(cx, IR_GOTO, NULL, ω);
    IR_t * mark = lic_build(cx, IR_BOUND, NULL, NULL);
    IR_t * unmk = lic_build(cx, IR_UNMARK, H, H);
    ir_operand_push(unmk, mark);
    IR_t * slb = cx->loop_break_beta;
    cx->loop_break_beta = NULL;
    IR_t * slf = cx->loop_fail;
    cx->loop_fail = ω;
    IR_t * sle = cx->loop_exit;
    IR_t * sln = cx->loop_next;
    int slns = cx->loop_next_ssp;
    IR_t * bres = lic_build(cx, IR_VAR, γ, ω);
    IR_LIT(bres).sval = (char *) "__break_result";
    cx->loop_exit = bres;
    cx->loop_next = unmk;
    cx->loop_next_ssp = cx->scan_sp;
    icn_loop_fr lf = { cx->loop_exit, cx->loop_next, cx->loop_fail, cx->loop_top };
    cx->loop_top = &lf;
    cx->loop_sp++;
    IR_t * bval = NULL;
    cx->discard = B;
    IR_t * b_entry = lic_lower(cx, B, unmk, unmk, &bval);
    cx->loop_sp--;
    cx->loop_top = lf.up;
    if (icn_parts_split(t->line, icn_part_line(B))) b_entry = icn_part_mark(cx, icn_part_line(B), b_entry, 0);
    lc_γ_to_α(mark, b_entry);
    lc_ω_to_α(mark, b_entry);
    lc_γ_to(H, mark);
    lc_ω_to(H, mark);
    cx->loop_exit = sle;
    cx->loop_next = sln;
    cx->loop_fail = slf;
    cx->loop_next_ssp = slns;
    { IR_t * lbb = cx->loop_break_beta; cx->loop_break_beta = slb; if (lbb) { cx->beta = lbb; *res = bres; return H; } }
    cx->beta = γ;
    *res = bres;
    return H;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * icn_arm_result(IR_t * rv) {
    if (rv) switch (rv->op) { case IR_GOTO: case IR_SUCCEED: case IR_FAIL: case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: return NULL; default: break; }
    return rv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * icn_dj_α_entry(IR_graph_t * g, IR_t * dj) { (void) g; return dj; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_arm(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv) {
    if (lv == 3) return lower_result_operand(cx, t, γ, ω, res);
    if (lv == 1) { IR_t * b4 = cx->beta; IR_t * ar = NULL; IR_t * e = lower_lvalue_var(cx, t, ω, &ar); if (e && ar) { lc_γ_to(ar, γ); *res = ar; return e; } cx->beta = b4; }
    return lic_lower(cx, t, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_seq_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv) {
    int al = cx->key_after_line;
    lc_vec Sv;
    lc_vec_init(&Sv, (int) sizeof(const tree_t *));
    for (int i = 0; i < t->n; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) s = lic_stmt_subj(s); if (s) lc_vec_push(&Sv, &s); }
    const tree_t ** S = (const tree_t **) Sv.data;
    int k = Sv.n;
    if (k == 0) { IR_t * nv = lic_build(cx, IR_VAR, γ, ω); IR_LIT(nv).sval = (char *) "&null"; *res = nv; return nv; }
    if (k == 1 && (t->v.ival != ICN_IF_FROM_INITIAL || (S[0]->line <= 0 && t->line <= 0))) return lower_arm(cx, S[0], γ, ω, res, lv);
    if (k == 1) {
        long ln = (S[0]->line > 0) ? S[0]->line : t->line;
        IR_t * e1 = lower_arm(cx, S[0], γ, ω, res, lv);
        e1 = cx->want_lines ? icn_line_hook(cx, ln, e1) : icn_line_mark(cx, ln, e1);
        return icn_trace_stmt_wrap(cx, ln, e1, ω);
    }
    IR_t * SEQX = lic_build(cx, IR_CONJUNCTION, γ, ω);
    IR_t ** val = (IR_t **) ct_zalloc((size_t) k, sizeof(IR_t *));
    IR_t ** ent = (IR_t **) ct_zalloc((size_t) k, sizeof(IR_t *));
    IR_t * succ = SEQX;
    if (t->t == TT_SEQ_EXPR) {
        IR_t * failt = ω;
        IR_t * last_beta = ω;
        IR_t * rb = NULL;
        for (int i = k - 1; i >= 0; i--) {
            val[i] = NULL;
            ent[i] = lower_arm(cx, S[i], succ, failt, &val[i], i == k - 1 ? lv : 0);
            if (i == k - 1) last_beta = cx->beta;
            if (!rb && is_resumable(S[i])) rb = cx->beta;
            if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ);
            succ = ent[i];
            failt = ent[i];
            if (i > 0 && ent[i] && ir_is_generator_kind(ent[i]->op)) { IR_t * SENT = lic_build(cx, IR_GOTO, NULL, NULL); lc_γ_to(SENT, ent[i]); lc_ω_to(SENT, ent[i]); succ = SENT; failt = SENT; }
            if (t->t == TT_SEQ_EXPR && S[i]->line > 0) {
                IR_t * se = succ;
                se = cx->want_lines ? icn_line_hook(cx, S[i]->line, se) : icn_line_mark(cx, S[i]->line, se);
                bb_src_note(se, i == 0 ? "nstmt" : "stmt", (int) S[i]->line);
                se = icn_trace_stmt_wrap(cx, S[i]->line, se, ω);
                if (succ == ent[i]) ent[i] = se;
                succ = se;
                failt = se;
            }
        }
        if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
        cx->conj_resumable = rb;
        cx->beta = last_beta;
        *res = SEQX;
        return ent[0];
    }
    IR_t * last_beta = ω;
    IR_t * rb = NULL;
    IR_t ** bet = (IR_t **) ct_zalloc((size_t) k, sizeof(IR_t *));
    IR_t ** jn = (IR_t **) ct_zalloc((size_t) k, sizeof(IR_t *));
    for (int i = k - 1; i >= 0; i--) {
        val[i] = NULL;
        cx->beta = ω;
        IR_t * failt = ω;
        if (i > 0) { jn[i] = lic_build(cx, IR_GOTO, ω, ω); failt = jn[i]; }
        int n0 = cx->g->n;
        if (i == k - 1 && t->t == TT_CONJ) cx->after_line = al;
        ent[i] = lower_arm(cx, S[i], succ, failt, &val[i], i == k - 1 ? lv : 0);
        bet[i] = cx->beta;
        if (i == k - 1) last_beta = cx->beta;
        if (!rb && is_resumable(S[i])) rb = cx->beta;
        if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ);
        for (int j = n0; j < cx->g->n; j++) {
            IR_t * x = cx->g->all[j];
            if (!x) continue;
            if (x->γ.node == succ && (unsigned char) x->γ.sz[0] == 0xce && (unsigned char) x->γ.sz[1] == 0xb2) lc_γ_to(x, succ);
            if (x->ω.node == succ && (unsigned char) x->ω.sz[0] == 0xce && (unsigned char) x->ω.sz[1] == 0xb2) lc_ω_to(x, succ);
        }
        succ = ent[i];
    }
    int lr = -1;
    int lr_cm = 0;
    for (int i = 0; i < k; i++) {
        if (i > 0 && jn[i]) {
            IR_t * tgt = ω;
            if (lr >= 0) tgt = (bet[lr] && bet[lr] != ω) ? bet[lr] : val[lr];
            if (lr >= 0 && lr_cm && tgt && tgt != ω) { lc_γ_to_β(jn[i], tgt); lc_ω_to_β(jn[i], tgt); } else { lic_γ_to(jn[i], tgt); lic_ω_to(jn[i], tgt); }
        }
        if (is_resumable(S[i]) || icn_tree_is_cursor_mover(S[i])) { lr = i; lr_cm = (!is_resumable(S[i]) && icn_tree_is_cursor_mover(S[i])); }
    }
    if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
    if (lr >= 0) rb = (bet[lr] && bet[lr] != ω) ? bet[lr] : val[lr];
    cx->conj_resumable = rb;
    cx->beta = (last_beta && last_beta != ω) ? last_beta : (rb ? rb : last_beta);
    *res = SEQX;
    return ent[0];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_limit_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv) {
    IR_t * lim = lic_build(cx, IR_LIMIT, γ, ω);
    IR_t * lr = NULL;
    IR_t * ee = lic_lower(cx, (t->n > 1) ? t->c[1] : NULL, lim, ω, &lr);
    IR_t * inner_beta = cx->beta;
    cx->conj_resumable = NULL;
    IR_t * er = NULL;
    IR_t * ge = (t->n > 0 && t->c[0]) ? lower_arm(cx, t->c[0], lim, ω, &er, lv) : lic_lower(cx, NULL, lim, ω, &er);
    IR_t * gen_beta = cx->beta;
    if (t->n > 0 && t->c[0] && t->c[0]->t == TT_CONJ && t->c[0]->n > 0 && !is_resumable(t->c[0]->c[t->c[0]->n - 1]) && cx->conj_resumable && cx->conj_resumable != ω) gen_beta = cx->conj_resumable;
    IR_t * nv = NULL;
    if (er && (er->op == IR_SUSPEND || er->op == IR_RETURN || er->op == IR_GOTO)) { nv = lic_build(cx, IR_VAR, lim, ω); IR_LIT(nv).sval = (char *) "&null"; }
    ir_operand_push(lim, nv ? nv : er);
    ir_operand_push(lim, lr);
    IR_t * gate = NULL;
    if (lr) {
        gate = lic_build(cx, IR_LIMIT_GATE, NULL, ω);
        ir_operand_push(gate, lr);
        lic_γ_to(gate, ge);
        if (gate->γ.node == ge) lc_γ_to(gate, ge);
        lic_γ_to(lr, gate);
        if (lr->γ.node == gate) lc_γ_to(lr, gate);
    } else {
        lic_γ_to(lr, ge);
        if (lr && lr->γ.node == ge) lc_γ_to(lr, ge);
    }
    if (er && !nv) lc_γ_to(er, lim);
    if (gen_beta && gen_beta != ω && gen_beta != er) ir_operand_push(lim, gen_beta);
    if (gate) ir_operand_push(lim, gate);
    (void)inner_beta;
    cx->beta = (gen_beta && gen_beta != ω) ? lim : gen_beta;
    *res = lim;
    return ee;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_if_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * TH = (t->n > 1) ? t->c[1] : NULL;
    const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
    IR_graph_t * g = cx->g;
    int n = EL ? 2 : 1;
    long cl = (t->line > 0 || cx->discard == t) ? icn_first_line(C) : 0;
    IR_t * dj = lc_build(g, IR_DISJUNCTION, NULL, NULL);
    lic_γ_to(dj, γ);
    lic_ω_to(dj, ω);
    IR_t * fs = IR_node_alloc(g, IR_FAIL);
    IR_t * entv[n];
    IR_t * resumev[n];
    IR_t * resv[n];
    for (int j = 0; j < n; j++) {
        const tree_t * ARM = j ? EL : TH;
        int before = g->n;
        IR_t * ar = NULL;
        cx->beta = dj;
        if (cl > 0) cx->discard = ARM;
        IR_t * aent = lower_arm(cx, ARM, dj, ω, &ar, lv);
        IR_t * ab = cx->beta;
        int arm_end = g->n;
        int ab_in_arm = 0;
        if (ab && ab != dj) for (int k = before; k < arm_end; k++) if (g->all[k] == ab) { ab_in_arm = 1; break; }
        if (aent && aent != dj && icn_parts_split(cl, icn_part_line(ARM))) aent = icn_part_mark(cx, icn_part_line(ARM), aent, 0);
        if (j == 0) {
            IR_t * aent0 = aent;
            int cbefore = g->n;
            IR_t * cval = NULL;
            IR_t * cmark = NULL;
            IR_t * cunmk = NULL;
            if (C && icn_tree_calls_a_generator(C)) {
                cmark = lc_build(g, IR_BOUND, NULL, NULL);
                cunmk = lc_build(g, IR_UNMARK, NULL, NULL);
                IR_LIT(cunmk).ival = 16;
                lc_γ_to_α(cunmk, aent0);
                lc_ω_to_α(cunmk, aent0);
                ir_operand_push(cunmk, cmark);
            }
            aent = lic_lower(cx, C, cunmk ? cunmk : aent, dj, &cval);
            if (cmark) { lc_γ_to_α(cmark, aent); lc_ω_to_α(cmark, aent); aent = cmark; }
            (void) cval;
            for (int k = cbefore; k < g->n; k++) {
                IR_t * x = g->all[k];
                if (!x) continue;
                if (x->γ.node == aent0 && (unsigned char) x->γ.sz[0] == 0xce && (unsigned char) x->γ.sz[1] == 0xb2) memcpy(x->γ.sz, "α!", 4);
                if (x->ω.node == aent0 && (unsigned char) x->ω.sz[0] == 0xce && (unsigned char) x->ω.sz[1] == 0xb2) memcpy(x->ω.sz, "α!", 4);
            }
        }
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == dj) { if ((x->op == IR_GOTO && x->ω.node == dj) || icn_γ_is_fail_conduit(x)) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
        }
        entv[j] = aent;
        resumev[j] = ab_in_arm ? ab : fs;
        resv[j] = ar;
    }
    for (int j = 0; j < n; j++) { ir_operand_push(dj, entv[j]); ir_operand_push(dj, resumev[j]); }
    for (int j = 0; j < n; j++) ir_operand_push(dj, icn_arm_result(resv[j]));
    IR_LIT(dj).ival = (long) n;
    cx->beta = dj;
    *res = dj;
    return icn_dj_α_entry(g, dj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_arm_mark_edges(IR_graph_t * g, int before, IR_t * dj) {
    for (int k = before; k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x) continue;
        if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
        if (x->γ.node == dj) { if ((x->op == IR_GOTO && x->ω.node == dj) || icn_γ_is_fail_conduit(x)) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_case_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv) {
    IR_graph_t * g = cx->g;
    if (t->n < 1 || !t->c[0]) { IR_t * s = lic_build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    int nc = t->n - 1, npairs = nc / 2, n = npairs + (nc % 2 == 1), dk = -1;
    for (int j = 0; j < npairs; j++) if (t->c[1 + 2 * j] && t->c[1 + 2 * j]->t == TT_NUL) { dk = j; break; }
    long sl = (t->line > 0 || cx->discard == t) ? icn_first_line(t->c[0]) : 0;
    IR_t * sr = NULL;
    IR_t * se = lic_lower(cx, t->c[0], NULL, ω, &sr);
    if (n == 0) { lic_γ_to(sr, ω); cx->beta = ω; *res = sr; return se; }
    IR_t * dj = lc_build(g, IR_DISJUNCTION, NULL, NULL);
    lic_γ_to(dj, γ);
    lic_ω_to(dj, ω);
    IR_t * fs = IR_node_alloc(g, IR_FAIL);
    IR_t ** entv = (IR_t **) ct_zalloc((size_t) n, sizeof *entv);
    IR_t ** resumev = (IR_t **) ct_zalloc((size_t) n, sizeof *resumev);
    IR_t ** resv = (IR_t **) ct_zalloc((size_t) n, sizeof *resv);
    for (int j = 0; j < n; j++) {
        int pj = (dk < 0 || j < dk) ? j : (j == n - 1) ? dk : j + 1;
        const tree_t * SEL = (pj < npairs && pj != dk) ? t->c[1 + 2 * pj] : NULL;
        const tree_t * BODY = (pj < npairs) ? t->c[2 + 2 * pj] : t->c[t->n - 1];
        int before = g->n;
        IR_t * ar = NULL;
        cx->beta = dj;
        if (sl > 0) cx->discard = BODY;
        IR_t * aent = lower_arm(cx, BODY, dj, ω, &ar, lv);
        IR_t * ab = cx->beta;
        int arm_end = g->n;
        int ab_in_arm = 0;
        if (ab && ab != dj) for (int k = before; k < arm_end; k++) if (g->all[k] == ab) { ab_in_arm = 1; break; }
        if (aent && aent != dj && icn_parts_split(sl, icn_part_line(BODY))) aent = icn_part_mark(cx, icn_part_line(BODY), aent, 0);
        if (SEL) {
            IR_t * aent0 = aent;
            cx->beta = dj;
            IR_t * kn = NULL;
            IR_t * ke = lic_lower(cx, SEL, NULL, dj, &kn);
            IR_t * kβ = cx->beta;
            IR_t * idc = lic_build(cx, IR_BINOP_TEST, aent0, dj);
            IR_LIT(idc).ival = BINOP_EQV;
            lc_γ_to_α(idc, aent0);
            ir_operand_push(idc, sr);
            ir_operand_push(idc, kn);
            lic_γ_to(kn, idc);
            if (kβ && kβ != dj && kβ != ω) lc_ω_to_β(idc, kβ);
            aent = ke ? ke : idc;
            if (ke && is_resumable(SEL)) { IR_t * KENT = lic_build(cx, IR_GOTO, NULL, NULL); lc_γ_to_α(KENT, ke); lc_ω_to_α(KENT, ke); aent = KENT; }
        }
        icn_arm_mark_edges(g, before, dj);
        entv[j] = aent;
        resumev[j] = ab_in_arm ? ab : fs;
        resv[j] = ar;
    }
    for (int j = 0; j < n; j++) { ir_operand_push(dj, entv[j]); ir_operand_push(dj, resumev[j]); }
    for (int j = 0; j < n; j++) ir_operand_push(dj, icn_arm_result(resv[j]));
    IR_LIT(dj).ival = (long) n;
    lc_γ_to_α(sr, icn_dj_α_entry(g, dj));
    ct_drop(entv);
    ct_drop(resumev);
    ct_drop(resv);
    cx->beta = dj;
    *res = dj;
    return se;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * E = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    long el = icn_first_line(E), bl = icn_part_line(B);
    int split = icn_parts_split(el, bl);
    IR_t * slb = cx->loop_break_beta;
    cx->loop_break_beta = NULL;
    IR_t * bres = lic_build(cx, IR_VAR, γ, ω);
    IR_LIT(bres).sval = (char *) "__break_result";
    IR_t * sle = cx->loop_exit;
    IR_t * sln = cx->loop_next;
    IR_t * slf = cx->loop_fail;
    int slns = cx->loop_next_ssp;
    cx->loop_exit = bres;
    cx->loop_fail = ω;
    cx->loop_next_ssp = cx->scan_sp;
    icn_loop_fr lf = { cx->loop_exit, NULL, cx->loop_fail, cx->loop_top };
    cx->loop_top = &lf;
    cx->loop_sp++;
    IR_t * eval = NULL;
    cx->loop_next = NULL;
    IR_t * e_entry = lic_lower(cx, E, NULL, ω, &eval);
    IR_t * gen_beta = cx->beta;
    cx->loop_sp--;
    cx->loop_top = lf.up;
    if (gen_beta == ω) gen_beta = lic_build(cx, IR_GOTO, ω, ω);
    cx->loop_next = gen_beta;
    IR_t * bval = NULL;
    (void) bval;
    IR_t * b_entry;
    if (B) {
        IR_t * mark = lic_build(cx, IR_BOUND, NULL, NULL);
        IR_t * back = split ? icn_part_mark(cx, el, gen_beta, 1) : gen_beta;
        IR_t * unmk = lic_build(cx, IR_UNMARK, back, back);
        ir_operand_push(unmk, mark);
        cx->loop_next = unmk;
        cx->loop_next_ssp = cx->scan_sp;
        icn_loop_fr lfb = { cx->loop_exit, cx->loop_next, cx->loop_fail, cx->loop_top };
        cx->loop_top = &lfb;
        cx->loop_sp++;
        cx->discard = B;
        b_entry = lic_lower(cx, B, unmk, unmk, &bval);
        cx->loop_sp--;
        cx->loop_top = lfb.up;
        if (split) b_entry = icn_part_mark(cx, bl, b_entry, 0);
        lc_γ_to_α(mark, b_entry);
        lc_ω_to_α(mark, b_entry);
        b_entry = mark;
    } else {
        b_entry = gen_beta;
    }
    cx->loop_exit = sle;
    cx->loop_next = sln;
    cx->loop_fail = slf;
    cx->loop_next_ssp = slns;
    if (!(eval && eval->op == IR_SUSPEND) && !(eval && eval->op == IR_GOTO && eval->γ.node == bres)) lic_γ_to(eval, b_entry);
    { IR_t * lbb = cx->loop_break_beta; cx->loop_break_beta = slb; if (lbb) { cx->beta = lbb; *res = bres; return e_entry; } }
    cx->beta = ω;
    *res = bres;
    return e_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_tree_mentions_kw(const tree_t * t, const char * kw) {
    if (!t) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, kw)) return 1;
    for (int i = 0; i < t->n; i++) if (icn_tree_mentions_kw(t->c[i], kw)) return 1;
    return 0;
}
