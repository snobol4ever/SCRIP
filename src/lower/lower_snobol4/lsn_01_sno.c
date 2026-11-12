#include "lower/lower_snobol4/lsn_head.h"
#include "runtime/core/g_runtime.h"
#include <stdlib.h>
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "lower/lower_snobol4.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "runtime/core/core_errjmp.h"
#include "lower/lower_snobol4/lsn_01_sno.h"
#include "lower/lower_snobol4/lsn_02_sno.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_04_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_06_sh.h"
#include "lower/lower_snobol4/lsn_08_sno.h"
int rt_kw_index(const char * kw);
int sno_sub_val_on(void) {
    if (g_lower.sno.sno_sub_val_on_v < 0) { const char * e = getenv("SCRIP_SUB_VAL"); g_lower.sno.sno_sub_val_on_v = (e && *e == '0') ? 0 : 1; }
    return g_lower.sno.sno_sub_val_on_v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_kw_static_slot(const char * kw) { return kw ? rt_kw_index(kw) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_expr_salt_next(void) { g_lower.sno.expr_salt++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_binop_code(tree_e tt) {
    switch (tt) {
        case TT_ADD:
        return 0;
        case TT_SUB:
        return 1;
        case TT_MUL:
        return 2;
        case TT_DIV:
        return 3;
        case TT_POW:
        return (int)BINOP_POW_PROMOTE;
        case TT_SEQ:
        return (int)BINOP_CONCAT_SNO;
        case TT_CAT:
        return (int)BINOP_CONCAT_SNO;
        default:
        return -1;
    }
}
static int sno_pe_patname(const char * s) {
    return s && (!strcmp(s, "ARB") || !strcmp(s, "BAL") || !strcmp(s, "REM") || !strcmp(s, "FAIL") || !strcmp(s, "SUCCEED") || !strcmp(s, "ABORT") || !strcmp(s, "FENCE"));
}
static const char * const sno_predef_pat_names[] = { "ABORT", "ARB", "BAL", "FAIL", "FENCE", "FLUSH", "REM", "SUCCEED" };
int sno_predef_pattern_name(const char * s) {
    if (!s) return 0;
    for (size_t i = 0; i < sizeof sno_predef_pat_names / sizeof sno_predef_pat_names[0]; i++) if (!strcmp(s, sno_predef_pat_names[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_csetcode(tree_e k) { return k == TT_ANY ? 59 : k == TT_NOTANY ? 151 : k == TT_SPAN ? 188 : k == TT_BREAK ? 69 : k == TT_BREAKX ? 70 : 0; }
static int sno_pe_intcode(tree_e k) { return k == TT_LEN ? 120 : k == TT_POS ? 162 : k == TT_RPOS ? 185 : k == TT_TAB ? 183 : k == TT_RTAB ? 181 : 0; }
static tree_e sno_pe_fnkind(const char * s) {
    return !s ? TT_VAR : !strcmp(s, "LEN") ? TT_LEN : !strcmp(s, "POS") ? TT_POS : !strcmp(s, "RPOS") ? TT_RPOS : !strcmp(s, "TAB") ? TT_TAB : !strcmp(s, "RTAB") ? TT_RTAB : !strcmp(s, "SPAN") ?
        TT_SPAN : !strcmp(s, "BREAK") ? TT_BREAK : !strcmp(s, "BREAKX") ? TT_BREAKX : !strcmp(s, "ANY") ? TT_ANY : !strcmp(s, "NOTANY") ? TT_NOTANY : TT_VAR;
}
int sno_pe_patfn(tree_e k) {
    return k == TT_ANY || k == TT_NOTANY || k == TT_SPAN || k == TT_BREAK || k == TT_BREAKX || k == TT_LEN || k == TT_POS || k == TT_RPOS || k == TT_TAB || k == TT_RTAB || k == TT_ARBNO ||
        k == TT_FENCE;
}
int sno_name_wrappable(const tree_t * e) {
    if (!e) return 0;
    if (e->t == TT_VAR || e->t == TT_IDX || e->t == TT_INDIRECT || e->t == TT_KEYWORD || e->t == TT_FNC) return 1;
    if (e->t == TT_DEFER) return e->n > 0 && e->c[0] && e->c[0]->t == TT_VAR;
    return sno_pe_patfn(e->t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_valfn(const char * s) {
    extern int core_fn_is_c(const char *);
    return s &&
        (!strcmp(s, "LN") || !strcmp(s, "OR") || !strcmp(s, "ABS") || !strcmp(s, "AND") || !strcmp(s, "COS") || !strcmp(s, "EXP") || !strcmp(s, "SIN") || !strcmp(s, "TAN") || !strcmp(s, "XOR") ||
        !strcmp(s, "ATAN") || !strcmp(s, "CHAR") || !strcmp(s, "CHOP") || !strcmp(s, "LPAD") || !strcmp(s, "RPAD") || !strcmp(s, "SQRT") || !strcmp(s, "COMPL") || !strcmp(s, "REMDR") ||
        !strcmp(s, "REPLACE") || !strcmp(s, "REVERSE") || !strcmp(s, "DATATYPE") || !strcmp(s, "SIZE")) && core_fn_is_c(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_op(tree_e op, const char * fn, DESCR_t * av, int n, DESCR_t * out, sno_pe_t * e) {
    extern int g_core_errjmp_n;
    extern void core_icn_op_ctx_clear(void);
    extern DESCR_t rt_num_neg_sno(DESCR_t);
    extern DESCR_t rt_num_pos(DESCR_t);
    extern DESCR_t rt_num_pos_sno(DESCR_t);
    extern DESCR_t rt_num_arith_sno(DESCR_t, DESCR_t, int);
    extern DESCR_t c_sno_concat_d(DESCR_t, DESCR_t);
    extern DESCR_t rt_call_arr_bl_sn4(const char *, DESCR_t *, int, int);
    long snum = g_runtime.misc.icn_errnumber;
    const char * stxt = g_runtime.misc.icn_errtext;
    DESCR_t sval = g_runtime.misc.icn_errvalue;
    int svalid = g_runtime.misc.icn_err_valid;
    long esv = g_runtime.abi.error;
    int my = g_core_errjmp_n;
    DESCR_t r = FAILDESCR;
    core_errjmp_t ej;
    g_runtime.abi.error = -2;
    g_runtime.misc.icn_err_valid = 0;
    int jc = setjmp(ej.jb);
    if (!jc) {
        core_errjmp_push(&ej);
        g_core_errjmp_n = my + 1;
        r = op == TT_FNC ? rt_call_arr_bl_sn4(fn, av, n, -1) : op == TT_MNS ? rt_num_neg_sno(av[0]) : op == TT_PLS ? rt_num_pos_sno(av[0]) : op == TT_SEQ ? c_sno_concat_d(av[0], av[1]) :
            rt_num_arith_sno(av[0], av[1], sno_binop_code(op));
    }
    core_errjmp_pop(&ej, my);
    g_runtime.abi.error = esv;
    core_icn_op_ctx_clear();
    if (g_runtime.misc.icn_err_valid) { e->code = (int)g_runtime.misc.icn_errnumber; e->msg = g_runtime.misc.icn_errtext; } else if (jc) { e->code = jc; e->msg = ""; }
    g_runtime.misc.icn_errnumber = snum;
    g_runtime.misc.icn_errtext = stxt;
    g_runtime.misc.icn_errvalue = sval;
    g_runtime.misc.icn_err_valid = svalid;
    if (jc || e->code || r.v == DT_FAIL) return 0;
    *out = r;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_value_form(const tree_t * t) {
    if (!t) return 0;
    switch (t->t) {
        case TT_ILIT:
        case TT_FLIT:
        case TT_QLIT:
        case TT_NAME:
        case TT_CAPT_CURSOR:
        case TT_PLS:
        case TT_MNS:
        case TT_NOT:
        case TT_INTERROGATE:
        case TT_ASSIGN:
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        case TT_DIV:
        case TT_POW:
        case TT_SEQ:
        case TT_CAT:
        case TT_ALT:
        case TT_SCAN:
        case TT_VLIST:
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        return 1;
        default:
        return 0;
    }
}
int sno_goto_operand_value(const tree_t * t) { return sno_pe_value_form(t); }
static const tree_t * sno_seq_head(const tree_t * t) { while (t && (t->t == TT_SEQ || t->t == TT_CAT) && t->n == 2) t = t->c[0]; return t; }
static int sno_pe_target_value(const tree_t * t) {
    if (t && t->t == TT_SCAN && t->n == 2) return sno_pe_value_form(t->c[0]);
    if (t && (t->t == TT_SEQ || t->t == TT_CAT) && t->n == 2) return sno_pe_value_form(sno_seq_head(t));
    return sno_pe_value_form(t);
}
static int sno_pe_byname_value(const tree_t * t) {
    if (!t) return 0;
    if ((t->t == TT_NAME || t->t == TT_CAPT_CURSOR) && t->n == 1) return sno_pe_value_form(t->c[0]);
    if (t->t == TT_OPSYN && t->n == 1 && t->v.sval && !strcmp(t->v.sval, "&")) return sno_pe_value_form(t->c[0]);
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n == 2) return sno_pe_value_form(t->c[1]);
    if (t->t == TT_ASSIGN && t->n == 2) return sno_pe_target_value(t->c[0]);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_walk(const tree_t * t, DESCR_t * v, sno_pe_t * e) {
    extern const char alphabet[257];
    DESCR_t a, b, av[2];
    int k, all;
    if (!t || e->code) return 0;
    if ((t->t == TT_NAME || t->t == TT_CAPT_CURSOR) && t->n == 1 && t->c[0] && t->c[0]->t == TT_INDIRECT && t->c[0]->n == 1 && sno_pe_walk(t->c[0]->c[0], &a, e) && (a.v == DT_P || a.v == DT_E)) {
        e->code = 239;
        e->msg = "indirection operand is not name";
        return 0;
    }
    if (sno_pe_byname_value(t)) { e->code = 212; e->msg = SNO_PE_212; return 0; }
    switch (t->t) {
        case TT_ILIT:
        *v = INTVAL(t->v.ival);
        return 1;
        case TT_FLIT:
        *v = REALVAL(t->v.dval);
        return 1;
        case TT_QLIT:
        *v = t->v.sval ? STRVAL(t->v.sval) : NULVCL;
        return 1;
        case TT_NUL:
        *v = NULVCL;
        return 1;
        case TT_VAR:
        if (!sno_pe_patname(t->v.sval)) return 0;
        *v = (DESCR_t){ .v = DT_P };
        return 1;
        case TT_KEYWORD:
        if (sno_pe_patname(t->v.sval)) { *v = (DESCR_t){ .v = DT_P }; return 1; }
        if (t->v.sval && !strcmp(t->v.sval, "ALPHABET")) { *v = BSTRVAL(alphabet, 256); return 1; }
        if (t->v.sval && !strcmp(t->v.sval, "LCASE")) { *v = STRVAL("abcdefghijklmnopqrstuvwxyz"); return 1; }
        if (t->v.sval && !strcmp(t->v.sval, "UCASE")) { *v = STRVAL("ABCDEFGHIJKLMNOPQRSTUVWXYZ"); return 1; }
        return 0;
        case TT_NAME:
        if (t->n == 1 && t->c[0] && t->c[0]->t == TT_VAR) { *v = NAMEVAL(t->c[0]->v.sval); return 1; }
        if (t->n == 1 && t->c[0] && t->c[0]->t == TT_KEYWORD && t->c[0]->v.sval && t->c[0]->v.sval[0]) {
            const char * kn = t->c[0]->v.sval;
            *v = NAMEVAL(sno_lead_name("&&", kn[0] == '&' ? kn + 1 : kn));
            return 1;
        }
        if (t->n == 1 && t->c[0] && t->c[0]->t == TT_DEFER && t->c[0]->n == 1 && t->c[0]->c[0] && t->c[0]->c[0]->t == TT_VAR) { *v = NAMEVAL(t->c[0]->c[0]->v.sval); return 1; }
        break;
        case TT_CAPT_CURSOR:
        if (t->n == 1 && t->c[0] && t->c[0]->t == TT_VAR) { *v = (DESCR_t){ .v = DT_P }; return 1; }
        break;
        case TT_DEFER:
        for (k = 0; k < t->n; k++) sno_pe_walk(t->c[k], &a, e);
        if (e->code) return 0;
        *v = (DESCR_t){ .v = DT_E };
        return 1;
        case TT_MNS:
        case TT_PLS:
        if (t->n != 1) break;
        if (!sno_pe_walk(t->c[0], &av[0], e)) return 0;
        return sno_pe_op(t->t, NULL, av, 1, v, e);
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        case TT_DIV:
        case TT_POW:
        if (t->n != 2) break;
        all = sno_pe_walk(t->c[0], &av[0], e);
        all = sno_pe_walk(t->c[1], &av[1], e) && all;
        if (!all || e->code) return 0;
        return sno_pe_op(t->t, NULL, av, 2, v, e);
        case TT_SEQ:
        case TT_CAT:
        for (k = 0, all = 1; k < t->n; k++) {
            if (!sno_pe_walk(t->c[k], &b, e)) { all = 0; continue; }
            if (!all) continue;
            if (!k) { a = b; continue; }
            if (a.v == DT_P || a.v == DT_E || b.v == DT_P || b.v == DT_E) { a = (DESCR_t){ .v = DT_P }; continue; }
            av[0] = a;
            av[1] = b;
            if (!sno_pe_op(TT_SEQ, NULL, av, 2, &a, e)) all = 0;
        }
        if (!all || !t->n || e->code) return 0;
        *v = a;
        return 1;
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        if (t->n != 2) break;
        all = sno_pe_walk(t->c[0], &a, e);
        if (all && !e->code && t->c[1] && t->c[1]->t == TT_INDIRECT && t->c[1]->n == 1 && sno_pe_walk(t->c[1]->c[0], &b, e) && (b.v == DT_P || b.v == DT_E)) {
            e->code = 239;
            e->msg = "indirection operand is not name";
            return 0;
        }
        if (!t->c[1] || t->c[1]->t != TT_VAR) { sno_pe_walk(t->c[1], &b, e); all = 0; }
        if (!all || e->code) return 0;
        *v = (DESCR_t){ .v = DT_P };
        return 1;
        case TT_FNC:
        {
            DESCR_t fa[t->n > 0 ? t->n : 1];
            tree_e pk = sno_pe_fnkind(t->v.sval);
            if (pk != TT_VAR) { tree_t pt = *t; pt.t = pk; sno_pe_walk(&pt, &a, e); if (e->code) return 0; }
            for (k = 0, all = sno_pe_valfn(t->v.sval); k < t->n; k++) all = sno_pe_walk(t->c[k], &fa[k], e) && fa[k].v != DT_P && fa[k].v != DT_E && all;
            if (!all || e->code) return 0;
            return sno_pe_op(TT_FNC, t->v.sval, fa, t->n, v, e);
        }
        default:
        if (t->t != TT_ALT && !sno_pe_patfn(t->t)) break;
        for (k = 0, all = 1; k < t->n; k++) all = sno_pe_walk(t->c[k], &a, e) && all;
        if (!all || e->code) return 0;
        {
            int pc = sno_pe_csetcode(t->t);
            int ic = sno_pe_intcode(t->t);
            extern const char * rt_coerce_errtext(int);
            extern int rt_name_textless(DESCR_t);
            if (pc && t->n == 1 && (IS_NULL_fn(a) || a.v == DT_P || rt_name_textless(a))) { e->code = pc; e->msg = rt_coerce_errtext(pc); return 0; }
            if (ic && t->n == 1 && a.v == DT_N) { e->code = ic; e->msg = rt_coerce_errtext(ic); return 0; }
            if (ic && t->n == 1 && a.v != DT_E && a.v != DT_P) {
                extern int rt_num_arg_d(const DESCR_t *, int64_t *);
                int64_t n = 0;
                if (!rt_num_arg_d(&a, &n)) { e->code = ic; e->msg = rt_coerce_errtext(ic); return 0; }
                if (n < 0 || n > 16777216) { e->code = ic + 1; e->msg = rt_coerce_errtext(ic + 1); return 0; }
            }
        }
        *v = (DESCR_t){ .v = DT_P };
        return 1;
    }
    for (k = 0; k < t->n; k++) sno_pe_walk(t->c[k], &a, e);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_preeval_expr(const tree_t * t, const char ** msg) { sno_pe_t e = { 0, (const char *)0 }; DESCR_t v; sno_pe_walk(t, &v, &e); if (msg) *msg = e.msg; return e.code > 0 ? e.code : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_preeval_stmt(const tree_t * s, const char ** msg) {
    const tree_t * part[6] = { 0 };
    int k, c = 0;
    if (!s || s->t != TT_STMT) return 0;
    part[0] = stmt_attr_expr(stmt_attr_find(s, ":subj"));
    part[1] = stmt_attr_expr(stmt_attr_find(s, ":pat"));
    part[2] = stmt_attr_expr(stmt_attr_find(s, ":repl"));
    part[3] = goto_node_expr(stmt_goto_find(s, TT_GOTO_S));
    part[4] = goto_node_expr(stmt_goto_find(s, TT_GOTO_F));
    part[5] = goto_node_expr(stmt_goto_find(s, TT_GOTO_U));
    if (stmt_attr_find(s, ":eq") && part[0] && (part[1] ? sno_pe_value_form(part[0]) : sno_pe_target_value(part[0]))) { if (msg) *msg = SNO_PE_212; return 212; }
    for (k = 0; k < 6 && !c; k++) if (part[k]) c = sno_preeval_expr(part[k], msg);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_preeval_program(const tree_t * prog) {
    extern void sno_error_voice(int, const char *, int);
    const char * m;
    int c;
    for (int i = 0; prog && i < prog->n; i++) { m = (const char *)0; c = sno_preeval_stmt(prog->c[i], &m); if (c) sno_error_voice(c, m ? m : "", lp_s_int(prog->c[i], ":line")); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_call_named(scx_t * cx, const char * name, const tree_t * t, int argbase, IR_t * γ, IR_t * ω, IR_t ** res) {
    int bn = (t && cx->wn_tree == t);
    if (bn) cx->wn_tree = NULL;
    IR_t * after = γ;
    IR_t * chk = NULL;
    IR_t * nlit = NULL;
    if (bn) {
        chk = lc_build(cx->g, IR_CALL, γ, ω);
        IR_LIT(chk).sval = (char *) "SNO$CHKNM";
        nlit = lc_build(cx->g, IR_LIT_STRING, chk, ω);
        IR_LIT(nlit).sval = (char *) lp_strdup(name);
        after = nlit;
    }
    IR_t * call = lc_build(cx->g, IR_CALL, after, ω);
    IR_LIT(call).sval = (char *) lp_strdup(name);
    call->strict = 2;
    IR_t * sr0 = NULL;
    if (g_lower.sno.sx_call_named_c2bb < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); g_lower.sno.sx_call_named_c2bb = (e2 && *e2 == '1') ? 1 : 0; }
    if (g_lower.sno.sx_call_named_c2bb) { sr0 = lc_build(cx->g, IR_DEFINE_CALL, call, ω); IR_LIT(sr0).sval = IR_LIT(call).sval; }
    IR_t * tail = sr0 ? sr0 : call;
    if (bn) {
        IR_t * wm = lc_build(cx->g, IR_CALL, tail, ω);
        IR_LIT(wm).sval = (char *) "SNO$WANTNM";
        IR_t * wl = lc_build(cx->g, IR_LIT_STRING, wm, ω);
        IR_LIT(wl).sval = (char *) "";
        ir_operand_push(wm, wl);
        tail = wl;
    }
    int nargs = t ? (t->n - argbase) : 0;
    IR_t * prev = NULL;
    IR_t * entry = tail;
    for (int k = 0; k < nargs; k++) {
        IR_t * ar = NULL;
        IR_t * mkn = NULL;
        if (bn && nargs == 1 && !strcmp(name, "EVAL")) { mkn = lc_build(cx->g, IR_CALL, tail, ω); IR_LIT(mkn).sval = (char *) "SNO$BYNAME"; }
        IR_t * ae = sx_lower(cx, t->c[argbase + k], (k == nargs - 1) ? (mkn ? mkn : tail) : NULL, ω, &ar);
        if (mkn) { ir_operand_push(mkn, ar); ar = mkn; }
        if (k == 0) entry = ae;
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) { ir_operand_push(call, ar); if (sr0) ir_operand_push(sr0, ar); }
    }
    if (chk) { ir_operand_push(chk, call); ir_operand_push(chk, nlit); }
    if (res) *res = chk ? chk : call;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_lower_byname(scx_t * cx, const tree_t * expr, IR_t * γ, IR_t * ω, IR_t ** res) {
    cx->wn_tree = expr;
    IR_t * e1 = sx_lower(cx, expr, γ, ω, res);
    if (cx->wn_tree != expr) return e1;
    cx->wn_tree = NULL;
    IR_t * wl = lc_build(cx->g, IR_LIT_STRING, NULL, ω);
    IR_LIT(wl).sval = (char *) "";
    IR_t * mk = lc_build(cx->g, IR_CALL, NULL, ω);
    IR_LIT(mk).sval = (char *) "SNO$WANTNM";
    lc_γ_to(wl, mk);
    ir_operand_push(mk, wl);
    lc_γ_to(mk, e1);
    return wl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_item_argbase(const tree_t * fn) {
    if (!fn || fn->t != TT_FNC) return -1;
    const char * fname = fn->v.sval;
    int argbase = 0;
    if (!fname && fn->n > 0 && fn->c[0] && fn->c[0]->t == TT_VAR) { fname = fn->c[0]->v.sval; argbase = 1; }
    return (fname && !strcmp(fname, "ITEM") && (fn->n - argbase) >= 2) ? argbase : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_capt_item_name(const tree_t * tgt) {
    if (sno_item_argbase(tgt) < 0) return NULL;
    const char * bn = sno_expr_collect_nm(tgt);
    char pb[fmt_len("*%s", bn)];
    snprintf(pb, sizeof pb, "*%s", bn);
    return lp_strdup(pb);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_nameval(scx_t * cx, const tree_t * inner, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
    IR_LIT(mk).sval = (char *) "SNO$NAME";
    IR_t * nr = NULL;
    IR_t * ne = sx_lower(cx, inner, mk, ω, &nr);
    if (nr) ir_operand_push(mk, nr);
    if (res) *res = mk;
    return ne;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res) {
    IR_t * br = NULL;
    IR_t * entry = sx_lower(cx, base, NULL, ω, &br);
    IR_t * cur = br;
    if (nidx == 2) {
        IR_t * i1 = NULL;
        IR_t * e1 = sx_lower(cx, idxs[0], NULL, ω, &i1);
        lc_γ_to(cur, e1);
        IR_t * i2 = NULL;
        IR_t * e2 = sx_lower(cx, idxs[1], NULL, ω, &i2);
        lc_γ_to(i1, e2);
        IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
        IR_LIT(sub).sval = "nd2-lv";
        lc_γ_to(i2, sub);
        ir_operand_push(sub, cur);
        ir_operand_push(sub, i1);
        ir_operand_push(sub, i2);
        cur = sub;
    } else {
        for (int k = 0; k < nidx; k++) {
            IR_t * ir = NULL;
            IR_t * ie = sx_lower(cx, idxs[k], NULL, ω, &ir);
            lc_γ_to(cur, ie);
            IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
            if (k < nidx - 1) sx_sub_container_only(sub);
            lc_γ_to(ir, sub);
            ir_operand_push(sub, cur);
            ir_operand_push(sub, ir);
            cur = sub;
        }
    }
    if (var_res) *var_res = cur;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_subscript_lv_fused(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res, IR_t ** fuse_base, IR_t ** fuse_idx) {
    const char * e = getenv("SCRIP_SUBASSIGN_FUSE");
    if (nidx != 1 || (e && *e == '0')) return sx_subscript_lv(cx, base, idxs, nidx, ω, var_res);
    IR_t * br = NULL;
    IR_t * entry = sx_lower(cx, base, NULL, ω, &br);
    IR_t * ir = NULL;
    IR_t * ie = sx_lower(cx, idxs[0], NULL, ω, &ir);
    lc_γ_to(br, ie);
    IR_t * ck = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
    { const char * a = getenv("SCRIP_SUB_AGG"); IR_LIT(ck).sval = (a && *a == '0') ? "lv-check" : "lv-check-container-only"; }
    lc_γ_to(ir, ck);
    ir_operand_push(ck, br);
    ir_operand_push(ck, ir);
    *fuse_base = ck;
    *fuse_idx = ir;
    if (var_res) *var_res = NULL;
    return entry;
}
int sno_capt_elem_tg(const tree_t * tg, int curs) {
    return tg && (tg->t == TT_IDX || (curs && tg->t == TT_INDIRECT) || (tg->t == TT_FNC && sno_item_argbase(tg) < 0) || sno_pe_patfn(tg->t) || (!curs && tg->t == TT_INDIRECT && !sno_capt_name(tg)));
}
int sno_elem_here(const scx_t * cx, const tree_t * tg, int curs) { return cx->snap && cx->npre >= 0 && cx->npre < 64 && sno_capt_elem_tg(tg, curs); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_assign_body(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * L = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * R = (t->n > 1) ? t->c[1] : NULL;
    if (!L) sno_fatal("TT_ASSIGN with no lhs", NULL);
    if (!R) sno_fatal("TT_ASSIGN with no rhs", NULL);
    if ((L->t == TT_SEQ || L->t == TT_CAT) && L->n == 2 && (L->c[0]->t == TT_SEQ || L->c[0]->t == TT_CAT) && L->c[0]->n == 2) {
        extern tree_t * ast_stmt_new(tree_e kind);
        const tree_t * head = sno_seq_head(L);
        tree_t * rest = NULL;
        int ns = 0;
        for (const tree_t * q = L; q != head; q = q->c[0]) ns++;
        const tree_t * spine[ns];
        ns = 0;
        for (const tree_t * q = L; q != head; q = q->c[0]) spine[ns++] = q->c[1];
        for (int k = ns - 1; k >= 0; k--) { if (!rest) { rest = (tree_t *) spine[k]; continue; } tree_t * sq = ast_stmt_new(TT_SEQ); ast_push(sq, rest); ast_push(sq, (tree_t *) spine[k]); rest = sq; }
        tree_t * sc0 = ast_stmt_new(TT_SCAN);
        ast_push(sc0, (tree_t *) head);
        ast_push(sc0, rest);
        L = sc0;
    }
    if (L->t == TT_SEQ && L->n == 2 && !sno_pe_value_form(L->c[0]) && res) {
        extern tree_t * ast_stmt_new(tree_e kind);
        tree_t * sc1 = ast_stmt_new(TT_SCAN);
        ast_push(sc1, (tree_t *) L->c[0]);
        ast_push(sc1, (tree_t *) L->c[1]);
        L = sc1;
    }
    if (L->t == TT_SCAN && L->n >= 2) {
        extern tree_t * ast_stmt_new(tree_e kind);
        IR_t * pent = NULL;
        IR_t * pasn = NULL;
        const tree_t * pat = sno_pat_valued(cx, NULL, 1, L->c[1], ω, &pent, &pasn);
        if (pasn) { tree_t * l2 = ast_stmt_new(TT_SCAN); ast_push(l2, (tree_t *) L->c[0]); ast_push(l2, (tree_t *) pat); L = l2; }
        if (res && L->c[0] && L->c[0]->t == TT_VAR && L->c[0]->v.sval) {
            IR_t * rv = lc_build(cx->g, IR_VAR, γ, ω);
            IR_LIT(rv).sval = L->c[0]->v.sval;
            IR_t * ev = sno_lower_match(cx, L, R, 1, rv, ω, NULL);
            *res = rv;
            if (pasn) { lc_γ_to(pasn, ev); return pent; }
            return ev;
        }
        IR_t * e = sno_lower_match(cx, L, R, 1, γ, ω, NULL);
        if (res) *res = NULL;
        if (pasn) { lc_γ_to(pasn, e); return pent; }
        return e;
    }
    if (L->t == TT_SEQ && L->n >= 2) {
        extern tree_t * ast_stmt_new(tree_e kind);
        const tree_t * pat = (L->n == 2) ? L->c[1] : NULL;
        if (!pat) { tree_t * ps = ast_stmt_new(TT_SEQ); for (int k = 1; k < L->n; k++) ast_push(ps, (tree_t *) L->c[k]); pat = ps; }
        IR_t * pent = NULL;
        IR_t * pasn = NULL;
        pat = sno_pat_valued(cx, NULL, 1, pat, ω, &pent, &pasn);
        tree_t * sc = ast_stmt_new(TT_SCAN);
        ast_push(sc, (tree_t *) L->c[0]);
        ast_push(sc, (tree_t *) pat);
        IR_t * e = sno_lower_match(cx, sc, R, 1, γ, ω, NULL);
        if (res) *res = NULL;
        if (pasn) { lc_γ_to(pasn, e); e = pent; }
        return e;
    }
    if (L->t == TT_VAR && L->v.sval) {
        sno_reg_var(L->v.sval);
        IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω);
        IR_LIT(asn).sval = L->v.sval;
        if (!res && R->t == TT_SCAN && R->n == 2 && R->c[0] && R->c[1]) {
            extern tree_t * ast_stmt_new(tree_e kind);
            IR_t * pent = NULL;
            IR_t * pasn = NULL;
            const tree_t * sbj = R->c[0];
            const tree_t * pat = sno_pat_valued(cx, &sbj, 0, R->c[1], ω, &pent, &pasn);
            tree_t * capt = ast_stmt_new(TT_CAPT_COND_ASGN);
            capt->slen = 240;
            ast_push(capt, (tree_t *) pat);
            tree_t * tv = ast_node_new(TT_VAR);
            tv->v.sval = L->v.sval;
            ast_push(capt, tv);
            tree_t * sc = ast_stmt_new(TT_SCAN);
            ast_push(sc, (tree_t *) sbj);
            ast_push(sc, capt);
            IR_t * e = sno_lower_match(cx, sc, NULL, 0, γ, ω, NULL);
            if (pasn) { lc_γ_to(pasn, e); return pent; }
            return e;
        }
        IR_t * vr = NULL;
        IR_t * e = sx_lower(cx, R, asn, ω, &vr);
        ir_operand_push(asn, vr);
        if (res) *res = asn;
        return e;
    }
    if (L->t == TT_INDIRECT && L->n > 0) {
        IR_t * nv = NULL;
        IR_t * e1 = sx_nameval(cx, L->c[0], NULL, ω, &nv);
        IR_t * vv = NULL;
        IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
        lc_γ_to(nv, e2);
        IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
        lc_γ_to(vv, asn);
        ir_operand_push(asn, nv);
        ir_operand_push(asn, vv);
        if (res) *res = asn;
        return e1;
    }
    if (L->t == TT_IDX && L->n >= 2) {
        IR_t * vr = NULL, * fb = NULL, * fi = NULL;
        IR_t * e1 = sx_subscript_lv_fused(cx, L->c[0], (const tree_t * const *) &L->c[1], L->n - 1, ω, &vr, &fb, &fi);
        IR_t * vv = NULL;
        IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
        IR_t * asn;
        if (fb) {
            lc_γ_to(fb, e2);
            asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, fb);
            ir_operand_push(asn, fi);
            ir_operand_push(asn, vv);
            sx_sub_container_only(asn);
        } else {
            lc_γ_to(vr, e2);
            asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, vr);
            ir_operand_push(asn, vv);
        }
        if (res) *res = asn;
        return e1;
    }
    if (L->t == TT_FNC) {
        const char * fname = L->v.sval;
        int argbase = 0;
        if (!fname && L->n > 0 && L->c[0] && L->c[0]->t == TT_VAR) { fname = L->c[0]->v.sval; argbase = 1; }
        int fnargs = L->n - argbase;
        if (fname && !strcmp(fname, "ITEM") && fnargs >= 2) {
            IR_t * vr = NULL;
            IR_t * e1 = sx_subscript_lv(cx, L->c[argbase], (const tree_t * const *) &L->c[argbase + 1], fnargs - 1, ω, &vr);
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
            lc_γ_to(vr, e2);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, vr);
            ir_operand_push(asn, vv);
            if (res) *res = asn;
            return e1;
        }
        {
            extern int rt_dat_field_of_any(const char *);
            if (fname && fnargs == 1 && rt_dat_field_of_any(fname)) {
                IR_t * br = NULL;
                IR_t * e1 = sx_lower(cx, L->c[argbase], NULL, ω, &br);
                IR_t * fv = lc_build(cx->g, IR_FIELD_VAR, NULL, ω);
                IR_LIT(fv).sval = (char *) lp_strdup(fname);
                lc_γ_to(br, fv);
                ir_operand_push(fv, br);
                IR_t * vv = NULL;
                IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
                lc_γ_to(fv, e2);
                IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, fv);
                ir_operand_push(asn, vv);
                if (res) *res = asn;
                return e1;
            }
        }
        {
            IR_t * cv = NULL;
            IR_t * e1 = sno_lower_byname(cx, L, NULL, ω, &cv);
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
            lc_γ_to(cv, e2);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, cv);
            ir_operand_push(asn, vv);
            if (res) *res = asn;
            return e1;
        }
    }
    if (L->t == TT_KEYWORD && L->v.sval && sno_kw_static_slot(L->v.sval) >= 0) {
        IR_t * kv = NULL;
        IR_t * ke = sx_lower(cx, R, NULL, ω, &kv);
        IR_t * kw = lc_build(cx->g, IR_KW_ASSIGN_SNOBOL4, γ, ω);
        IR_LIT(kw).sval = L->v.sval;
        lc_γ_to(kv, kw);
        ir_operand_push(kw, kv);
        if (res) *res = kw;
        return ke;
    }
    if (L->t == TT_KEYWORD && L->v.sval) {
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
        IR_LIT(mk).sval = (char *) "SNO$KWSET";
        IR_t * nl = lc_build(cx->g, IR_LIT_STRING, NULL, ω);
        IR_LIT(nl).sval = L->v.sval;
        IR_t * vv = NULL;
        IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
        lc_γ_to(nl, e2);
        lc_γ_to(vv, mk);
        ir_operand_push(mk, nl);
        ir_operand_push(mk, vv);
        if (res) *res = mk;
        return nl;
    }
    sno_fatal("TT_ASSIGN lhs form outside the landed subset (VAR/INDIRECT/IDX/ITEM/DATA-field/KEYWORD)", NULL);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_build_call_stub(const char * entry_label, const char * fname) {
    IR_graph_t * g = IR_alloc(64);
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t * sh4 = fname ? lc_build(g, IR_DEFINE_SHIM, exitnd, failnd) : (IR_t *)0;
    if (sh4) {
        IR_t * s41 = lc_build(g, IR_LIT_STRING, NULL, NULL);
        IR_LIT(s41).sval = (char *) fname;
        IR_t * s42 = lc_build(g, IR_LIT_STRING, NULL, NULL);
        IR_LIT(s42).sval = (char *) entry_label;
        ir_operand_push(sh4, s41);
        ir_operand_push(sh4, s42);
    }
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, sh4 ? sh4 : exitnd, failnd);
    IR_LIT(gd).sval = lp_strdup(entry_label);
    gd->seal = 1;
    g->entry = gd;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_build_rt_shim_graph(const char * fname, const char * entry_label, const char ** stable_out) {
    const char * fn = fname ? ct_strdup(fname) : (const char *)0;
    const char * el = !entry_label ? (const char *)0 : (entry_label == fname) ? fn : ct_strdup(entry_label);
    if (!fn || !el) return (IR_graph_t *)0;
    if (stable_out) *stable_out = fn;
    IR_graph_t * g = IR_alloc(16);
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t * sh4 = lc_build(g, IR_DEFINE_RT_SHIM, exitnd, failnd);
    IR_t * s41 = lc_build(g, IR_LIT_STRING, NULL, NULL);
    IR_LIT(s41).sval = (char *) fn;
    IR_t * s42 = lc_build(g, IR_LIT_STRING, NULL, NULL);
    IR_LIT(s42).sval = (char *) el;
    IR_t * s43 = lc_build(g, IR_LIT_INTEGER, NULL, NULL);
    IR_LIT(s43).ival = 1;
    ir_operand_push(sh4, s41);
    ir_operand_push(sh4, s42);
    ir_operand_push(sh4, s43);
    g->entry = sh4;
    return g;
}
