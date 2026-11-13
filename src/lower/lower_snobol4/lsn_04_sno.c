#include "lower/lower_snobol4/lsn_head.h"
#include <stdlib.h>
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_04_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_06_sh.h"
#include "lower/g_lower_sno.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sx_sub_container_only(IR_t * sub) { const char * e = getenv("SCRIP_SUB_AGG"); if (!(e && *e == '0')) IR_LIT(sub).sval = "container-only"; }
char * sno_scan_tmp(const char * pfx) {
    int scv = g_lower_sno.sno_scan_tmp_scv_n++;
    char nmb[fmt_len("%s$%d", pfx, scv)];
    snprintf(nmb, sizeof nmb, "%s$%d", pfx, scv);
    char * tn = (char *) lp_strdup(nmb);
    sno_reg_var(tn);
    return tn;
}
const tree_t * sno_pat_valued(scx_t * cx, const tree_t ** subj, int has_repl, const tree_t * pat, IR_t * ω, IR_t ** pent, IR_t ** pasn) {
    extern tree_t * ast_stmt_new(tree_e kind);
    *pent = NULL;
    *pasn = NULL;
    if (!pat || sno_pat_supported(pat)) return pat;
    const tree_t * sj = subj ? *subj : NULL;
    if (sj && !has_repl && sj->t != TT_VAR && sj->t != TT_QLIT && sj->t != TT_ILIT) {
        char * sn = sno_scan_tmp("SCS");
        IR_t * sasn = lc_build(cx->g, IR_ASSIGN, NULL, ω);
        IR_LIT(sasn).sval = sn;
        IR_t * sv = NULL;
        *pent = sx_lower(cx, sj, sasn, ω, &sv);
        ir_operand_push(sasn, sv);
        tree_t * st = ast_node_new(TT_VAR);
        st->v.sval = sn;
        *subj = st;
        *pasn = sasn;
    }
    char * pn = sno_scan_tmp("SCP");
    IR_t * pa = lc_build(cx->g, IR_ASSIGN, NULL, ω);
    IR_LIT(pa).sval = pn;
    IR_t * pv = NULL;
    IR_t * pe = sx_lower(cx, pat, pa, ω, &pv);
    ir_operand_push(pa, pv);
    if (*pasn) lc_γ_to(*pasn, pe);
    else *pent = pe;
    *pasn = pa;
    tree_t * dv = ast_node_new(TT_VAR);
    dv->v.sval = pn;
    tree_t * dd = ast_stmt_new(TT_DEFER);
    ast_push(dd, dv);
    return dd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_tree_has_scan(const tree_t * n, int d) {
    if (!n || d > 24) return 0;
    if (n->t == TT_SCAN) return 1;
    for (int i = 0; i < n->n; i++) if (sno_tree_has_scan(n->c[i], (n->t == TT_SEQ || n->t == TT_CAT) ? d : d + 1)) return 1;
    return 0;
}
IR_t * sx_binop(scx_t * cx, const tree_t * t, int code, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * op = lc_build(cx->g, IR_BINOP, γ, ω);
    IR_LIT(op).ival = code;
    op->strict = 2;
    IR_t * lr = NULL;
    IR_t * rr = NULL;
    if (sno_tree_has_scan(t->c[1], 0)) {
        char nmb[24];
        snprintf(nmb, sizeof nmb, "SCL$%d", g_lower_sno.sx_binop_scl_n++);
        char * tmpn = lp_strdup(nmb);
        sno_reg_var(tmpn);
        IR_t * asn = lc_build(cx->g, IR_ASSIGN, NULL, ω);
        IR_LIT(asn).sval = tmpn;
        IR_t * ea = sx_lower(cx, t->c[0], asn, ω, &lr);
        ir_operand_push(asn, lr);
        IR_t * rd = lc_build(cx->g, IR_VAR, op, ω);
        IR_LIT(rd).sval = tmpn;
        IR_t * eb = sx_lower(cx, t->c[1], NULL, ω, &rr);
        lc_γ_to(asn, eb);
        lc_γ_to(rr, rd);
        ir_operand_push(op, rd);
        ir_operand_push(op, rr);
        if (res) *res = op;
        return ea;
    }
    IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &lr);
    IR_t * eb = sx_lower(cx, t->c[1], op, ω, &rr);
    lc_γ_to(lr, eb);
    ir_operand_push(op, lr);
    ir_operand_push(op, rr);
    if (res) *res = op;
    return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_predef_note(const char * fname) {
    for (uint32_t k = 0; k < g_lower_sno.predef.len; k++) if (!strcmp(CV_AT(g_lower_sno.predef, const char *, k), fname)) return;
    CV_PUSH(g_lower_sno.predef, const char *) = fname;
}
void sno_bare_pat_note(const char * fname) {
    for (uint32_t k = 0; k < g_lower_sno.bare_pat.len; k++) if (!strcmp(CV_AT(g_lower_sno.bare_pat, const char *, k), fname)) return;
    CV_PUSH(g_lower_sno.bare_pat, const char *) = fname;
}
int sno_bare_pat_count(void) { return (int) g_lower_sno.bare_pat.len; }
const char * sno_bare_pat_name(int k) { return CV_AT(g_lower_sno.bare_pat, const char *, k); }
void sno_tdef_note(const char * fname) {
    for (uint32_t k = 0; k < g_lower_sno.tdef.len; k++) if (!strcmp(CV_AT(g_lower_sno.tdef, const char *, k), fname)) return;
    CV_PUSH(g_lower_sno.tdef, const char *) = lp_strdup(fname);
}
int sno_tdef_registered(const char * fname) {
    if (!fname) return 0;
    for (uint32_t k = 0; k < g_lower_sno.tdef.len; k++) if (!strcmp(CV_AT(g_lower_sno.tdef, const char *, k), fname)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_tree_has_define_call(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC) { const char * name = t->v.sval; if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) name = t->c[0]->v.sval; if (name && !strcmp(name, "DEFINE")) return 1; }
    for (int i = 0; i < t->n; i++) if (sno_tree_has_define_call(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pred_relop(const char * n, int * lex, long * c1, long * c2) {
    if (!n) return -1;
    *lex = 0;
    if (!strcmp(n, "EQ")) { *c1 = 101; *c2 = 102; return 0; }
    if (!strcmp(n, "NE")) { *c1 = 149; *c2 = 150; return 1; }
    if (!strcmp(n, "LT")) { *c1 = 147; *c2 = 148; return 2; }
    if (!strcmp(n, "LE")) { *c1 = 118; *c2 = 119; return 3; }
    if (!strcmp(n, "GT")) { *c1 = 111; *c2 = 112; return 4; }
    if (!strcmp(n, "GE")) { *c1 = 109; *c2 = 110; return 5; }
    *lex = 1;
    if (!strcmp(n, "LEQ")) { *c1 = 122; *c2 = 123; return 0; }
    if (!strcmp(n, "LNE")) { *c1 = 132; *c2 = 133; return 1; }
    if (!strcmp(n, "LLT")) { *c1 = 130; *c2 = 131; return 2; }
    if (!strcmp(n, "LLE")) { *c1 = 128; *c2 = 129; return 3; }
    if (!strcmp(n, "LGT")) { *c1 = 126; *c2 = 127; return 4; }
    if (!strcmp(n, "LGE")) { *c1 = 124; *c2 = 125; return 5; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_pred_cmp(scx_t * cx, const tree_t * t, int argbase, int lex, int rk, long c1, long c2, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    IR_t * cmp = lc_build(g, IR_CMP_TEST, γ, ω);
    IR_LIT(cmp).ival = rk;
    IR_t * ca = lc_build(g, lex ? IR_COERCE_STRING : IR_COERCE_NUMERIC, cmp, ω);
    IR_LIT(ca).ival = c1;
    IR_t * cb = lc_build(g, lex ? IR_COERCE_STRING : IR_COERCE_NUMERIC, ca, ω);
    IR_LIT(cb).ival = c2;
    const tree_t * ax = (t->n > argbase + 0) ? t->c[argbase + 0] : NULL;
    const tree_t * bx = (t->n > argbase + 1) ? t->c[argbase + 1] : NULL;
    IR_t * ar = NULL;
    IR_t * br = NULL;
    IR_t * be;
    IR_t * ae;
    if (bx) be = sx_lower(cx, bx, cb, ω, &br);
    else { br = lc_build(g, IR_LIT_STRING, cb, ω); IR_LIT(br).sval = (char *) ""; be = br; }
    if (ax) ae = sx_lower(cx, ax, be, ω, &ar);
    else { ar = lc_build(g, IR_LIT_STRING, be, ω); IR_LIT(ar).sval = (char *) ""; ae = ar; }
    ir_operand_push(ca, ar);
    if (!lex) ir_operand_push(ca, br);
    ir_operand_push(cb, br);
    if (!lex) ir_operand_push(cb, ar);
    ir_operand_push(cmp, ca);
    ir_operand_push(cmp, cb);
    if (res) *res = cmp;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_ident_differ(scx_t * cx, const tree_t * t, int argbase, int is_differ, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    IR_t * nd = lc_build(g, is_differ ? IR_DIFFER : IR_IDENT, γ, ω);
    const tree_t * ax = (t->n > argbase + 0) ? t->c[argbase + 0] : NULL;
    const tree_t * bx = (t->n > argbase + 1) ? t->c[argbase + 1] : NULL;
    IR_t * ar = NULL;
    IR_t * br = NULL;
    IR_t * be;
    IR_t * ae;
    if (bx) be = sx_lower(cx, bx, nd, ω, &br);
    else { br = lc_build(g, IR_LIT_STRING, nd, ω); IR_LIT(br).sval = (char *) ""; be = br; }
    if (!ax) sno_fatal("IDENT/DIFFER with no argument", NULL);
    ae = sx_lower(cx, ax, be, ω, &ar);
    ir_operand_push(nd, ar);
    ir_operand_push(nd, br);
    if (res) *res = nd;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_defer_seal(scx_t * cx, IR_t * nd) { if (cx && cx->pat_seal && nd) ir_operand_push(nd, cx->pat_seal); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_thunk_abort_exit(IR_graph_t * g, IR_t * no) { IR_t * ab = lc_build(g, IR_MATCH_ABORT, no, no); IR_LIT(ab).ival = 1; return ab; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_arm_result(IR_t * rv) {
    if (rv) switch (rv->op) { case IR_GOTO: case IR_SUCCEED: case IR_FAIL: case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: return NULL; default: break; }
    return rv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_ident_inline_on(void) {
    if (g_lower_sno.sno_ident_inline_on_on < 0) { const char * e = getenv("SCRIP_IDENT_INLINE"); g_lower_sno.sno_ident_inline_on_on = (e && *e == '0') ? 0 : 1; }
    return g_lower_sno.sno_ident_inline_on_on;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_vl_quiet(const tree_t * t) {
    if (!t) return 1;
    switch (t->t) {
        case TT_QLIT:
        case TT_ILIT:
        case TT_FLIT:
        case TT_NUL:
        return 1;
        case TT_VAR:
        { const char * nm = t->v.sval; return !(nm && (!strcmp(nm, "INPUT") || !strcmp(nm, "TERMINAL"))); }
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        case TT_DIV:
        case TT_POW:
        case TT_SEQ:
        case TT_CAT:
        case TT_MNS:
        case TT_PLS:
        case TT_VLIST:
        if (t->t != TT_VLIST && t->n > 1 && sno_tree_has_scan(t->c[1], 0)) return 0;
        for (int i = 0; i < t->n; i++) if (!sno_vl_quiet(t->c[i])) return 0;
        return 1;
        case TT_FNC:
        {
            const char * name = t->v.sval;
            int argbase = 0;
            if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
            int na = t->n - argbase;
            if (!name || na < 1 || na > 2) return 0;
            int lex = 0;
            long c1 = 0, c2 = 0;
            int inl = sno_pred_relop(name, &lex, &c1, &c2) >= 0 || (sno_ident_inline_on() && !sno_predef_registered(name) && (!strcmp(name, "IDENT") || !strcmp(name, "DIFFER")));
            if (!inl) return 0;
            for (int i = argbase; i < t->n; i++) if (!sno_vl_quiet(t->c[i])) return 0;
            return 1;
        }
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_vlist_arms(scx_t * cx, const tree_t * t, int j, char * tn, IR_t * jn, IR_t * ω) {
    IR_graph_t * g = cx->g;
    IR_t * next = ω;
    if (j + 1 < t->n) next = sno_vlist_arms(cx, t, j + 1, tn, jn, ω);
    IR_t * asn = lc_build(g, IR_ASSIGN, jn, next);
    IR_LIT(asn).sval = tn;
    int before = g->n;
    IR_t * ar = NULL;
    IR_t * ej = sx_lower(cx, t->c[j], asn, next, &ar);
    ar = sno_arm_result(ar);
    if (!ar) {
        int upto = g->n;
        ar = lc_build(g, IR_LIT_STRING, asn, next);
        IR_LIT(ar).sval = (char *) "";
        for (int k = before; k < upto; k++) { IR_t * x = g->all[k]; if (x && x->γ.node == asn) x->γ.node = ar; }
        if (ej == asn) ej = ar;
    }
    ir_operand_push(asn, ar);
    return ej;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_idx_container(scx_t * cx, const tree_t * t, IR_t * ω, IR_t ** res) {
    if (t->n < 1) sno_fatal("subscript with no container", NULL);
    IR_t * br = NULL;
    IR_t * entry = sx_lower(cx, t->c[0], NULL, ω, &br);
    IR_t * cur = br;
    if (t->n == 1) {
        tree_t * ek = ast_node_new(TT_QLIT);
        ek->v.sval = (char *) "";
        IR_t * ir = NULL;
        IR_t * ie = sx_lower(cx, ek, NULL, ω, &ir);
        lc_γ_to(cur, ie);
        IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
        sx_sub_container_only(sub);
        lc_γ_to(ir, sub);
        ir_operand_push(sub, cur);
        ir_operand_push(sub, ir);
        cur = sub;
    } else if (t->n == 3) {
        IR_t * i1 = NULL;
        IR_t * e1 = sx_lower(cx, t->c[1], NULL, ω, &i1);
        lc_γ_to(cur, e1);
        IR_t * i2 = NULL;
        IR_t * e2 = sx_lower(cx, t->c[2], NULL, ω, &i2);
        lc_γ_to(i1, e2);
        IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
        IR_LIT(sub).sval = "nd2";
        lc_γ_to(i2, sub);
        ir_operand_push(sub, cur);
        ir_operand_push(sub, i1);
        ir_operand_push(sub, i2);
        cur = sub;
    } else {
        for (int k = 1; k < t->n; k++) {
            IR_t * ir = NULL;
            IR_t * ie = sx_lower(cx, t->c[k], NULL, ω, &ir);
            lc_γ_to(cur, ie);
            IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
            sx_sub_container_only(sub);
            lc_γ_to(ir, sub);
            ir_operand_push(sub, cur);
            ir_operand_push(sub, ir);
            cur = sub;
        }
    }
    if (res) *res = cur;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_xname_lv(scx_t * cx, const tree_t * nexp, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    char * tn = sno_scan_tmp("SNO$XW");
    char * xn = sno_scan_tmp("SNO$XN");
    IR_t * join = lc_build(g, IR_VAR, NULL, ω);
    IR_LIT(join).sval = tn;
    IR_t * asn_d = lc_build(g, IR_ASSIGN, join, ω);
    IR_LIT(asn_d).sval = tn;
    IR_t * fb = lc_build(g, IR_CALL, asn_d, ω);
    IR_LIT(fb).sval = (char *) "SNO$NAME";
    ir_operand_push(asn_d, fb);
    tree_t * ev = ast_node_new(TT_FNC);
    ev->v.sval = (char *) "EVAL";
    tree_t * xv = ast_node_new(TT_VAR);
    xv->v.sval = xn;
    ast_push(ev, xv);
    IR_t * asn_e = lc_build(g, IR_ASSIGN, join, ω);
    IR_LIT(asn_e).sval = tn;
    IR_t * evr = NULL;
    IR_t * ee = sx_lower(cx, ev, asn_e, ω, &evr);
    ir_operand_push(asn_e, evr);
    IR_t * xa = lc_build(g, IR_ASSIGN, ee, ω);
    IR_LIT(xa).sval = xn;
    IR_t * fr = NULL;
    IR_t * fe = sx_lower(cx, nexp, fb, ω, &fr);
    ir_operand_push(fb, fr);
    IR_t * xt = lc_build(g, IR_CALL, xa, fe);
    IR_LIT(xt).sval = (char *) "SNO$XLV";
    ir_operand_push(xa, xt);
    IR_t * vr = NULL;
    IR_t * ve = sx_lower(cx, nexp, xt, ω, &vr);
    ir_operand_push(xt, vr);
    *res = join;
    return ve;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_scan_body(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (t->n < 2) sno_fatal("TT_SCAN with missing subject or pattern", NULL);
    extern tree_t * ast_stmt_new(tree_e kind);
    IR_t * pent = NULL;
    IR_t * pasn = NULL;
    const tree_t * sbj = t->c[0];
    const tree_t * pat = sno_pat_valued(cx, &sbj, 0, t->c[1], ω, &pent, &pasn);
    if (res) {
        char * tmpn = sno_scan_tmp("SCV");
        tree_t * capt = ast_stmt_new(TT_CAPT_COND_ASGN);
        capt->slen = 240;
        ast_push(capt, (tree_t *) pat);
        tree_t * tv = ast_node_new(TT_VAR);
        tv->v.sval = tmpn;
        ast_push(capt, tv);
        tree_t * sc = ast_stmt_new(TT_SCAN);
        ast_push(sc, (tree_t *) sbj);
        ast_push(sc, capt);
        IR_t * rd = lc_build(cx->g, IR_VAR, γ, ω);
        IR_LIT(rd).sval = tmpn;
        IR_t * e = sno_lower_match(cx, sc, NULL, 0, rd, ω, NULL);
        *res = rd;
        if (pasn) { lc_γ_to(pasn, e); return pent; }
        return e;
    }
    if (pasn) {
        tree_t * sc = ast_stmt_new(TT_SCAN);
        ast_push(sc, (tree_t *) sbj);
        ast_push(sc, (tree_t *) pat);
        IR_t * e = sno_lower_match(cx, sc, NULL, 0, γ, ω, NULL);
        lc_γ_to(pasn, e);
        return pent;
    }
    IR_t * e = sno_lower_match(cx, t, NULL, 0, γ, ω, NULL);
    return e;
}
