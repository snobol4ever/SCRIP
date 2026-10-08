#include "rt/rt_arena.h"
#include "ct_arena.h"
#include "ct_vec.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <setjmp.h>
#include "lower.h"
#include "bb_program.h"
#include "parsers/icon/icon_lex.h"
#include "snobol4_system_fns.h"
#include "../runtime/core/core_errjmp.h"
int rt_kw_index(const char * kw);
static int sno_sub_val_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_SUB_VAL"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_kw_static_slot(const char * kw) { return kw ? rt_kw_index(kw) : -1; }
extern void global_register(const char * name);
extern int stage2_proc_grow(stage2_t * s2);
typedef struct { const tree_t * arg; IR_t * prim; int str; long codes; const char * snapg; } sprearg_t;
typedef struct {
    IR_graph_t * g;
    IR_t * loop_exit;
    IR_t * loop_next;
    const char * result_name;
    IR_t * pat_fail;
    IR_t * pat_seal;
    sprearg_t pre[64];
    int npre;
    long prog_nstmt;
    long stno_base;
    IR_t * seq_rlast;
    IR_t * seq_rlast_for;
    IR_t * seq_rtail;
} scx_t;
#define SNO_DEF_NAMES_MAX 64
typedef struct { const char * fname; const char * entry; const char * result_name; const char * names[SNO_DEF_NAMES_MAX]; int nnames; int nformals; } sno_def_t;
static int sno_fname_is_multiproto(const char * fname);
typedef struct { const char * name; const tree_t * expr; int salt; int want_name; } sno_expr_ent_t;
static cv_t g_sno_exprs;
static int g_sno_expr_salt = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_expr_salt_next(void) { g_sno_expr_salt++; }
static const char * sno_expr_collect(const tree_t * expr);
typedef struct { const char * name; const tree_t * pat; int salt; } sno_pat_ent_t;
static cv_t g_sno_pats;
static int g_sno_uses_stmtkw = 0;
static int g_sno_traces_a_label = 0;
static int g_sno_uses_code = 0;
static int g_sno_calls_code = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_setexit_on(void) { const char * e = getenv("SCRIP_SETEXIT"); return (e && e[0] == '0') ? 0 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_define_entry_computed(const tree_t * t, int argbase) {
    const tree_t * en = (t && t->n > argbase + 1) ? t->c[argbase + 1] : NULL;
    return en && en->t != TT_QLIT && !(en->t == TT_NAME && en->n > 0 && en->c[0] && en->c[0]->t == TT_VAR);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_scan_code_use(const tree_t * t) {
    if (!t || g_sno_uses_code) return;
    if (t->t == TT_DEFINE) { const tree_t * pr = t->n > 1 ? t->c[1] : NULL; if (!pr || pr->t != TT_QLIT || sno_define_entry_computed(t, 1)) { g_sno_uses_code = 1; return; } }
    if (t->t == TT_FNC) {
        const char * fn = t->v.sval;
        if (!fn && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) fn = t->c[0]->v.sval;
        if (fn && !strcmp(fn, "CODE")) { g_sno_uses_code = 1; g_sno_calls_code = 1; return; }
        if (fn && sno_setexit_on() && !strcmp(fn, "SETEXIT")) { g_sno_uses_code = 1; return; }
        if (fn && !strcmp(fn, "DEFINE")) {
            int ab = t->v.sval ? 0 : 1;
            if (t->n <= ab || !t->c[ab] || t->c[ab]->t != TT_QLIT) { g_sno_uses_code = 1; return; }
            if (sno_define_entry_computed(t, ab)) { g_sno_uses_code = 1; return; }
        }
        if (fn && (!strcmp(fn, "OPSYN") || !strcmp(fn, "APPLY"))) {
            int ab = t->v.sval ? 0 : 1, k = ab + (fn[0] == 'O' ? 1 : 0);
            const tree_t * src = t->n > k ? t->c[k] : NULL;
            if (src && (fn[0] == 'O' ? src->t != TT_QLIT : 0)) { g_sno_uses_code = 1; return; }
            if (src && src->t == TT_QLIT && src->v.sval && !strcmp(src->v.sval, "DEFINE")) { g_sno_uses_code = 1; return; }
        }
    }
    if ((t->t == TT_GOTO_U || t->t == TT_GOTO_S || t->t == TT_GOTO_F) && t->n > 0 && t->c[0]) {
        const tree_t * g0 = t->c[0];
        if (g0->t != TT_QLIT || (g0->v.sval && g0->v.sval[0] == '$')) { g_sno_uses_code = 1; return; }
    }
    for (int i = 0; i < t->n; i++) sno_scan_code_use(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fatal(const char * what, const char * detail) {
    fprintf(stderr, "FATAL lower_snobol4 (GZ#5 subset): %s%s%s. Pattern matching, EVAL and CODE are outside the landed subset (IR_MATCH_* family pending); see GOAL-SNOBOL4-BB.md.\n", what,
        detail ? ": " : "", detail ? detail : "");
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sfind(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) { const tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zw5_on(void) { const char * e = getenv("SCRIP_ZW5"); return (e && *e == '0') ? 0 : 1; }
static const char * sfind_str(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0 && a->c[0]) ? a->c[0]->v.sval : NULL; }
static tree_t * sfind_expr(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0) ? a->c[0] : NULL; }
static void sno_reg_var(const char * nm) { extern void global_register_copy(const char *); if (nm && nm[0] && nm[0] != '&') global_register_copy(nm); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_expr_eq(const tree_t * a, const tree_t * b) {
    if (a == b) return 1;
    if (!a || !b) return 0;
    if (a->t != b->t || a->n != b->n) return 0;
    if (a->v.ival != b->v.ival) return 0;
    for (int i = 0; i < a->n; i++) if (!sno_expr_eq(a->c[i], b->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_collect(const tree_t * expr) {
    if (!expr) sno_fatal("unevaluated-expression operator (*) with no operand", NULL);
    for (int i = 0; i < (int) g_sno_exprs.len; i++)
        if (CV_AT(g_sno_exprs, sno_expr_ent_t, i).salt == g_sno_expr_salt && strncmp(CV_AT(g_sno_exprs, sno_expr_ent_t, i).name, "EXPRNM$", 7) &&
        sno_expr_eq(CV_AT(g_sno_exprs, sno_expr_ent_t, i).expr, expr)) return CV_AT(g_sno_exprs, sno_expr_ent_t, i).name;
    const char * xv = (expr->t == TT_VAR && expr->v.sval && expr->v.sval[0] && !strchr(expr->v.sval, '$') && strlen(expr->v.sval) < 100) ? expr->v.sval : "";
    char buf[fmt_len("EXPR$%dF%d$%s", (int) g_sno_exprs.len, g_sno_expr_salt, xv)];
    if (g_sno_expr_salt) snprintf(buf, sizeof buf, "EXPR$%dF%d", (int) g_sno_exprs.len, g_sno_expr_salt);
    else snprintf(buf, sizeof buf, "EXPR$%d", (int) g_sno_exprs.len);
    if (*xv) { size_t bl = strlen(buf); snprintf(buf + bl, sizeof buf - bl, "$%s", xv); }
    { sno_expr_ent_t x; x.name = lp_strdup(buf); x.expr = expr; x.salt = g_sno_expr_salt; x.want_name = 0; CV_PUSH(g_sno_exprs, sno_expr_ent_t) = x; return x.name; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_collect_nm(const tree_t * expr) {
    if (!expr) sno_fatal("unevaluated-expression operator (*) with no operand", NULL);
    for (int i = 0; i < (int) g_sno_exprs.len; i++)
        if (CV_AT(g_sno_exprs, sno_expr_ent_t, i).salt == g_sno_expr_salt && CV_AT(g_sno_exprs, sno_expr_ent_t, i).want_name && sno_expr_eq(CV_AT(g_sno_exprs, sno_expr_ent_t, i).expr, expr))
        return CV_AT(g_sno_exprs, sno_expr_ent_t, i).name;
    char buf[40];
    if (g_sno_expr_salt) snprintf(buf, sizeof buf, "EXPRNM$%dF%d", (int) g_sno_exprs.len, g_sno_expr_salt);
    else snprintf(buf, sizeof buf, "EXPRNM$%d", (int) g_sno_exprs.len);
    { sno_expr_ent_t x; x.name = lp_strdup(buf); x.expr = expr; x.salt = g_sno_expr_salt; x.want_name = 1; CV_PUSH(g_sno_exprs, sno_expr_ent_t) = x; return x.name; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_collect_wn(const tree_t * expr) {
    const char * nm = sno_expr_collect(expr);
    for (int i = 0; i < (int) g_sno_exprs.len; i++) if (CV_AT(g_sno_exprs, sno_expr_ent_t, i).name == nm) { CV_AT(g_sno_exprs, sno_expr_ent_t, i).want_name = 1; break; }
    return nm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_binop_code(tree_e tt) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int code; const char * msg; } sno_pe_t;
static int sno_pe_patname(const char * s) {
    return s && (!strcmp(s, "ARB") || !strcmp(s, "BAL") || !strcmp(s, "REM") || !strcmp(s, "FAIL") || !strcmp(s, "SUCCEED") || !strcmp(s, "ABORT") || !strcmp(s, "FENCE"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_csetcode(tree_e k) { return k == TT_ANY ? 59 : k == TT_NOTANY ? 151 : k == TT_SPAN ? 188 : k == TT_BREAK ? 69 : k == TT_BREAKX ? 70 : 0; }
static int sno_pe_patfn(tree_e k) {
    return k == TT_ANY || k == TT_NOTANY || k == TT_SPAN || k == TT_BREAK || k == TT_BREAKX || k == TT_LEN || k == TT_POS || k == TT_RPOS || k == TT_TAB || k == TT_RTAB || k == TT_ARBNO ||
        k == TT_FENCE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_valfn(const char * s) {
    extern int core_fn_is_c(const char *);
    return s &&
        (!strcmp(s, "LN") || !strcmp(s, "OR") || !strcmp(s, "ABS") || !strcmp(s, "AND") || !strcmp(s, "COS") || !strcmp(s, "EXP") || !strcmp(s, "SIN") || !strcmp(s, "TAN") || !strcmp(s, "XOR") ||
        !strcmp(s, "ATAN") || !strcmp(s, "CHAR") || !strcmp(s, "CHOP") || !strcmp(s, "LPAD") || !strcmp(s, "RPAD") || !strcmp(s, "SQRT") || !strcmp(s, "COMPL") || !strcmp(s, "REMDR") ||
        !strcmp(s, "REPLACE") || !strcmp(s, "REVERSE") || !strcmp(s, "DATATYPE")) && core_fn_is_c(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_op(tree_e op, const char * fn, DESCR_t * av, int n, DESCR_t * out, sno_pe_t * e) {
    extern int g_core_errjmp_n;
    extern long g_error;
    extern void core_icn_op_ctx_clear(void);
    extern long g_icn_errnumber;
    extern const char * g_icn_errtext;
    extern DESCR_t g_icn_errvalue;
    extern int g_icn_err_valid;
    extern DESCR_t rt_num_neg_sno(DESCR_t);
    extern DESCR_t rt_num_pos(DESCR_t);
    extern DESCR_t rt_num_arith_sno(DESCR_t, DESCR_t, int);
    extern DESCR_t sno_concat_d(DESCR_t, DESCR_t);
    extern DESCR_t rt_call_arr_bl_sn4(const char *, DESCR_t *, int, int);
    long snum = g_icn_errnumber;
    const char * stxt = g_icn_errtext;
    DESCR_t sval = g_icn_errvalue;
    int svalid = g_icn_err_valid;
    long esv = g_error;
    int my = g_core_errjmp_n;
    DESCR_t r = FAILDESCR;
    core_errjmp_t ej;
    g_error = -2;
    g_icn_err_valid = 0;
    int jc = setjmp(ej.jb);
    if (!jc) {
        core_errjmp_push(&ej);
        g_core_errjmp_n = my + 1;
        r = op == TT_FNC ? rt_call_arr_bl_sn4(fn, av, n, -1) : op == TT_MNS ? rt_num_neg_sno(av[0]) : op == TT_PLS ? rt_num_pos(av[0]) : op == TT_SEQ ? sno_concat_d(av[0], av[1]) :
            rt_num_arith_sno(av[0], av[1], sno_binop_code(op));
    }
    core_errjmp_pop(&ej, my);
    g_error = esv;
    core_icn_op_ctx_clear();
    if (g_icn_err_valid) { e->code = (int)g_icn_errnumber; e->msg = g_icn_errtext; } else if (jc) { e->code = jc; e->msg = ""; }
    g_icn_errnumber = snum;
    g_icn_errtext = stxt;
    g_icn_errvalue = sval;
    g_icn_err_valid = svalid;
    if (jc || e->code || r.v == DT_FAIL) return 0;
    *out = r;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pe_walk(const tree_t * t, DESCR_t * v, sno_pe_t * e) {
    extern char alphabet[257];
    DESCR_t a, b, av[2];
    int k, all;
    if (!t || e->code) return 0;
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
        if (!t->c[1] || t->c[1]->t != TT_VAR) { sno_pe_walk(t->c[1], &b, e); all = 0; }
        if (!all || e->code) return 0;
        *v = (DESCR_t){ .v = DT_P };
        return 1;
        case TT_FNC:
        {
            DESCR_t fa[t->n > 0 ? t->n : 1];
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
            extern const char * rt_coerce_errtext(int);
            if (pc && t->n == 1 && (IS_NULL_fn(a) || a.v == DT_P)) { e->code = pc; e->msg = rt_coerce_errtext(pc); return 0; }
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
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * sno_mkpat_emit(scx_t * cx, const tree_t * pat, IR_t * γ, IR_t * ω, IR_t ** res);
static int sno_mkpat_here(const tree_t * t);
static IR_t * sx_idx_container(scx_t * cx, const tree_t * t, IR_t * ω, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sco_stmt_hook(scx_t * cx, const tree_t * s, IR_t * body);
static IR_t * sco_branch(scx_t * cx, const tree_t * pg, IR_t * γ, IR_t * ω) {
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
static void sx_sub_container_only(IR_t * sub) { const char * e = getenv("SCRIP_SUB_AGG"); if (!(e && *e == '0')) IR_LIT(sub).sval = "container-only"; }
static IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res);
static IR_t * sx_subscript_lv_fused(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res, IR_t ** fuse_base, IR_t ** fuse_idx);
static IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land);
static int sno_pat_supported(const tree_t * t);
static char * sno_scan_tmp(const char * pfx) {
    static int g_scv_n = 0;
    int scv = g_scv_n++;
    char nmb[fmt_len("%s$%d", pfx, scv)];
    snprintf(nmb, sizeof nmb, "%s$%d", pfx, scv);
    char * tn = (char *) lp_strdup(nmb);
    sno_reg_var(tn);
    return tn;
}
static const tree_t * sno_pat_valued(scx_t * cx, const tree_t ** subj, int has_repl, const tree_t * pat, IR_t * ω, IR_t ** pent, IR_t ** pasn) {
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
    for (int i = 0; i < n->n; i++) if (sno_tree_has_scan(n->c[i], d + 1)) return 1;
    return 0;
}
static IR_t * sx_binop(scx_t * cx, const tree_t * t, int code, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * op = lc_build(cx->g, IR_BINOP, γ, ω);
    IR_LIT(op).ival = code;
    op->strict = 2;
    IR_t * lr = NULL;
    IR_t * rr = NULL;
    if (sno_tree_has_scan(t->c[1], 0)) {
        static int g_scl_n = 0;
        char nmb[24];
        snprintf(nmb, sizeof nmb, "SCL$%d", g_scl_n++);
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
static int sno_def_entry_absent(const tree_t * subj, int argbase);
static const char * sno_qlit_fold(const tree_t * t);
static void sno_entry_seen_push(cv_t * seen, const char * el);
static void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d);
static const char * sno_define_entry_opt(const tree_t * dsub, int argbase);
static void sno_bind_attach_proto(IR_graph_t * g, IR_t * bind, const sno_def_t * d, IR_t * fail);
static void sno_bind_attach_entry(IR_graph_t * g, IR_t * bind, const char * entry, IR_t * fail);
static cv_t g_sno_predef;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_predef_note(const char * fname) {
    for (uint32_t k = 0; k < g_sno_predef.len; k++) if (!strcmp(CV_AT(g_sno_predef, const char *, k), fname)) return;
    CV_PUSH(g_sno_predef, const char *) = fname;
}
static int sno_predef_registered(const char * fname) {
    if (!fname) return 0;
    for (uint32_t k = 0; k < g_sno_predef.len; k++) if (!strcmp(CV_AT(g_sno_predef, const char *, k), fname)) return 1;
    return 0;
}
static cv_t g_sno_tdef;
static void sno_tdef_note(const char * fname) {
    for (uint32_t k = 0; k < g_sno_tdef.len; k++) if (!strcmp(CV_AT(g_sno_tdef, const char *, k), fname)) return;
    CV_PUSH(g_sno_tdef, const char *) = lp_strdup(fname);
}
static int sno_tdef_registered(const char * fname) { if (!fname) return 0; for (uint32_t k = 0; k < g_sno_tdef.len; k++) if (!strcmp(CV_AT(g_sno_tdef, const char *, k), fname)) return 1; return 0; }
static const char * sno_t4_target(const char * op, int nops);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_tree_has_define_call(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC) { const char * name = t->v.sval; if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) name = t->c[0]->v.sval; if (name && !strcmp(name, "DEFINE")) return 1; }
    for (int i = 0; i < t->n; i++) if (sno_tree_has_define_call(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pred_relop(const char * n, int * lex, long * c1, long * c2) {
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
static IR_t * sx_pred_cmp(scx_t * cx, const tree_t * t, int argbase, int lex, int rk, long c1, long c2, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    IR_t * cmp = lc_build(g, IR_CMP_TEST, γ, ω);
    IR_LIT(cmp).ival = rk;
    IR_t * cb = lc_build(g, lex ? IR_COERCE_STRING : IR_COERCE_NUMERIC, cmp, ω);
    IR_LIT(cb).ival = c2;
    IR_t * ca = lc_build(g, lex ? IR_COERCE_STRING : IR_COERCE_NUMERIC, cb, ω);
    IR_LIT(ca).ival = c1;
    const tree_t * ax = (t->n > argbase + 0) ? t->c[argbase + 0] : NULL;
    const tree_t * bx = (t->n > argbase + 1) ? t->c[argbase + 1] : NULL;
    IR_t * ar = NULL;
    IR_t * br = NULL;
    IR_t * be;
    IR_t * ae;
    if (bx) be = sx_lower(cx, bx, ca, ω, &br);
    else { br = lc_build(g, IR_LIT_STRING, ca, ω); IR_LIT(br).sval = (char *) ""; be = br; }
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
static IR_t * sx_ident_differ(scx_t * cx, const tree_t * t, int argbase, int is_differ, IR_t * γ, IR_t * ω, IR_t ** res) {
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
static IR_t * sx_call_named(scx_t * cx, const char * name, const tree_t * t, int argbase, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * call = lc_build(cx->g, IR_CALL, γ, ω);
    IR_LIT(call).sval = (char *) lp_strdup(name);
    call->strict = 2;
    IR_t * sr0 = NULL;
    static int c2bb = -1;
    if (c2bb < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); c2bb = (e2 && *e2 == '1') ? 1 : 0; }
    if (c2bb) { sr0 = lc_build(cx->g, IR_DEFINE, call, ω); IR_LIT(sr0).sval = IR_LIT(call).sval; }
    IR_t * tail = sr0 ? sr0 : call;
    int nargs = t ? (t->n - argbase) : 0;
    IR_t * prev = NULL;
    IR_t * entry = tail;
    for (int k = 0; k < nargs; k++) {
        IR_t * ar = NULL;
        IR_t * ae = sx_lower(cx, t->c[argbase + k], (k == nargs - 1) ? tail : NULL, ω, &ar);
        if (k == 0) entry = ae;
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) { ir_operand_push(call, ar); if (sr0) ir_operand_push(sr0, ar); }
    }
    if (res) *res = call;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_cursor_target(const tree_t * tgt);
static const char * sno_lead_name(const char * lead, const char * nm);
static const char * sno_capt_name(const tree_t * tgt);
static IR_t * sx_nameval(scx_t * cx, const tree_t * inner, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
    IR_LIT(mk).sval = (char *) "SNO$NAME";
    IR_t * nr = NULL;
    IR_t * ne = sx_lower(cx, inner, mk, ω, &nr);
    if (nr) ir_operand_push(mk, nr);
    if (res) *res = mk;
    return ne;
}
static const tree_t * sno_const_val(const char * ck);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_defer_seal(scx_t * cx, IR_t * nd) { if (cx && cx->pat_seal && nd) ir_operand_push(nd, cx->pat_seal); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_thunk_abort_exit(IR_graph_t * g, IR_t * no) { IR_t * ab = lc_build(g, IR_MATCH_ABORT, no, no); IR_LIT(ab).ival = 1; return ab; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_arm_result(IR_t * rv) {
    if (rv) switch (rv->op) { case IR_GOTO: case IR_SUCCEED: case IR_FAIL: case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: return NULL; default: break; }
    return rv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_ident_inline_on(void) { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_IDENT_INLINE"); on = (e && *e == '0') ? 0 : 1; } return on; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_vl_quiet(const tree_t * t) {
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
static IR_t * sno_vlist_arms(scx_t * cx, const tree_t * t, int j, char * tn, IR_t * jn, IR_t * ω) {
    IR_graph_t * g = cx->g;
    IR_t * next = ω;
    if (j + 1 < t->n) { IR_t * jr = jn; if (j + 2 < t->n) { jr = lc_build(g, IR_VAR, jn, ω); IR_LIT(jr).sval = tn; } next = sno_vlist_arms(cx, t, j + 1, tn, jr, ω); }
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
static IR_t * sx_idx_container(scx_t * cx, const tree_t * t, IR_t * ω, IR_t ** res) {
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
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    if (sno_mkpat_here(t)) return sno_mkpat_emit(cx, t, γ, ω, res);
    switch (t->t) {
        case TT_ILIT:
        { IR_t * nd = lc_build(cx->g, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; if (res) *res = nd; return nd; }
        case TT_FLIT:
        { IR_t * nd = lc_build(cx->g, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; if (res) *res = nd; return nd; }
        case TT_QLIT:
        { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
        case TT_NUL:
        { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
        case TT_VAR:
        { sno_reg_var(t->v.sval); IR_t * nd = lc_build(cx->g, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; if (res) *res = nd; return nd; }
        case TT_KEYWORD:
        {
            if (t->v.sval) {
                char cb[fmt_len("&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval)];
                snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
                const tree_t * cv = sno_const_val(cb);
                if (cv) return sx_lower(cx, cv, γ, ω, res);
            }
            IR_t * nd = lc_build(cx->g, IR_KW_SNOBOL4, γ, ω);
            IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) "";
            if (res) *res = nd;
            return nd;
        }
        case TT_DEFER:
        {
            const char * bn = sno_expr_collect((t->n > 0) ? t->c[0] : NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$MKEXPR";
            IR_t * nl = lc_build(cx->g, IR_LIT_STRING, mk, ω);
            IR_LIT(nl).sval = (char *) bn;
            nl->seal = IR_SEAL_DSTAR_REF;
            ir_operand_push(mk, nl);
            if (res) *res = mk;
            return nl;
        }
        case TT_NAME:
        {
            if (t->n < 1 || !t->c[0]) sno_fatal("name operator with no operand", NULL);
            if (t->c[0]->t == TT_VAR && t->c[0]->v.sval) {
                sno_reg_var(t->c[0]->v.sval);
                IR_t * nl = lc_build(cx->g, IR_LIT_NAME, γ, ω);
                IR_LIT(nl).sval = t->c[0]->v.sval;
                if (res) *res = nl;
                return nl;
            }
            if (t->c[0]->t == TT_IDX && t->c[0]->n >= 2) {
                const tree_t * ix = t->c[0];
                IR_t * vr = NULL;
                IR_t * entry = sx_subscript_lv(cx, ix->c[0], (const tree_t * const *) &ix->c[1], ix->n - 1, ω, &vr);
                if (γ) lc_γ_to(vr, γ);
                if (res) *res = vr;
                return entry;
            }
            if (t->c[0]->t == TT_FNC) {
                const tree_t * fn = t->c[0];
                const char * fname = fn->v.sval;
                int argbase = 0;
                if (!fname && fn->n > 0 && fn->c[0] && fn->c[0]->t == TT_VAR) { fname = fn->c[0]->v.sval; argbase = 1; }
                extern int rt_dat_field_of_any(const char *);
                if (fname && (fn->n - argbase) == 1 && rt_dat_field_of_any(fname)) {
                    IR_t * br = NULL;
                    IR_t * ea = sx_lower(cx, fn->c[argbase], NULL, ω, &br);
                    IR_t * fv = lc_build(cx->g, IR_FIELD_VAR, γ, ω);
                    IR_LIT(fv).sval = (char *) lp_strdup(fname);
                    lc_γ_to(br, fv);
                    ir_operand_push(fv, br);
                    if (res) *res = fv;
                    return ea;
                }
                IR_t * wl = lc_build(cx->g, IR_LIT_STRING, NULL, ω);
                IR_LIT(wl).sval = (char *) "";
                IR_t * mk = lc_build(cx->g, IR_CALL, NULL, ω);
                IR_LIT(mk).sval = (char *) "SNO$WANTNM";
                lc_γ_to(wl, mk);
                ir_operand_push(mk, wl);
                IR_t * cv = NULL;
                IR_t * e1 = sx_lower(cx, t->c[0], γ, ω, &cv);
                lc_γ_to(mk, e1);
                if (res) *res = cv;
                return wl;
            }
            if (t->c[0]->t == TT_KEYWORD && t->c[0]->v.sval) {
                const char * kn = t->c[0]->v.sval;
                const char * cb = sno_lead_name("&&", kn[0] == '&' ? kn + 1 : kn);
                IR_t * nl = lc_build(cx->g, IR_LIT_NAME, γ, ω);
                IR_LIT(nl).sval = (char *) cb;
                if (res) *res = nl;
                return nl;
            }
            if (t->c[0]->t == TT_INDIRECT && t->c[0]->n > 0 && t->c[0]->c[0]) return sx_nameval(cx, t->c[0]->c[0], γ, ω, res);
            sno_fatal("name operator over this form is outside the landed subset", NULL);
        }
        case TT_ANY:
        case TT_NOTANY:
        case TT_SPAN:
        case TT_BREAK:
        case TT_BREAKX:
        {
            if (t->n < 1) sno_fatal("charset pattern function with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBK";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            IR_t * va = NULL;
            IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &va);
            lc_γ_to(kt, ea);
            lc_γ_to(va, mk);
            ir_operand_push(mk, kt);
            ir_operand_push(mk, va);
            if (res) *res = mk;
            return kt;
        }
        case TT_LEN:
        case TT_TAB:
        case TT_RTAB:
        case TT_POS:
        case TT_RPOS:
        {
            if (t->n < 1) sno_fatal("integer pattern function with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBN";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            IR_t * va = NULL;
            IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &va);
            lc_γ_to(kt, ea);
            lc_γ_to(va, mk);
            ir_operand_push(mk, kt);
            ir_operand_push(mk, va);
            if (res) *res = mk;
            return kt;
        }
        case TT_ARB:
        case TT_REM:
        case TT_BAL:
        case TT_FAIL:
        case TT_SUCCEED:
        case TT_ABORT:
        case TT_FLUSH:
        {
            if (t->v.sval && sno_predef_registered(t->v.sval)) return sx_call_named(cx, t->v.sval, t, 0, γ, ω, res);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PB0";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, mk, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            ir_operand_push(mk, kt);
            if (res) *res = mk;
            return kt;
        }
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        {
            const tree_t * tgt = (t->n > 1) ? t->c[1] : NULL;
            const char * vn = (tgt && tgt->t == TT_VAR) ? tgt->v.sval : sno_capt_name(tgt);
            if (vn) sno_reg_var(vn);
            if (!vn && tgt && tgt->t == TT_DEFER) {
                const char * bn = sno_expr_collect((tgt->n > 0) ? tgt->c[0] : NULL);
                char pb[fmt_len("*%s", bn)];
                snprintf(pb, sizeof pb, "*%s", bn);
                vn = lp_strdup(pb);
            }
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBC";
            IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω);
            IR_LIT(kt).ival = (int64_t) t->t;
            IR_t * nl;
            IR_t * es;
            IR_t * vs = NULL;
            if (vn) {
                nl = lc_build(cx->g, IR_LIT_STRING, NULL, ω);
                IR_LIT(nl).sval = (char *) vn;
                es = sx_lower(cx, t->c[0], NULL, ω, &vs);
                lc_γ_to(kt, nl);
                lc_γ_to(nl, es);
            } else if (tgt && tgt->t == TT_INDIRECT) {
                IR_t * nv = NULL;
                IR_t * en = sx_lower(cx, (tgt->n > 0) ? tgt->c[0] : NULL, NULL, ω, &nv);
                es = sx_lower(cx, t->c[0], NULL, ω, &vs);
                lc_γ_to(kt, en);
                lc_γ_to(nv, es);
                nl = nv;
            } else if (tgt && tgt->t == TT_FNC) {
                IR_t * wl = lc_build(cx->g, IR_LIT_STRING, NULL, ω);
                IR_LIT(wl).sval = (char *) "";
                IR_t * wm = lc_build(cx->g, IR_CALL, NULL, ω);
                IR_LIT(wm).sval = (char *) "SNO$WANTNM";
                lc_γ_to(wl, wm);
                ir_operand_push(wm, wl);
                IR_t * nv = NULL;
                IR_t * en = sx_lower(cx, tgt, NULL, ω, &nv);
                lc_γ_to(wm, en);
                es = sx_lower(cx, t->c[0], NULL, ω, &vs);
                lc_γ_to(kt, wl);
                lc_γ_to(nv, es);
                nl = nv;
            } else {
                sno_fatal("capture target in a runtime-built pattern is not a simple variable", NULL);
                return NULL;
            }
            lc_γ_to(vs, mk);
            ir_operand_push(mk, kt);
            ir_operand_push(mk, nl);
            ir_operand_push(mk, vs);
            if (res) *res = mk;
            return kt;
        }
        case TT_ARBNO:
        {
            if (t->n < 1) sno_fatal("ARBNO with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PARB";
            IR_t * vi = NULL;
            IR_t * ei = sx_lower(cx, t->c[0], NULL, ω, &vi);
            lc_γ_to(vi, mk);
            ir_operand_push(mk, vi);
            if (res) *res = mk;
            return ei;
        }
        case TT_FENCE:
        {
            if (t->n == 0) {
                IR_t * mk0 = lc_build(cx->g, IR_CALL, γ, ω);
                IR_LIT(mk0).sval = (char *) "SNO$PB0";
                IR_t * kt0 = lc_build(cx->g, IR_LIT_INTEGER, mk0, ω);
                IR_LIT(kt0).ival = (int64_t) TT_FENCE;
                ir_operand_push(mk0, kt0);
                if (res) *res = mk0;
                return kt0;
            }
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PFEN";
            IR_t * vi = NULL;
            IR_t * ei = sx_lower(cx, t->c[0], NULL, ω, &vi);
            lc_γ_to(vi, mk);
            ir_operand_push(mk, vi);
            if (res) *res = mk;
            return ei;
        }
        case TT_ALT:
        {
            if (t->n < 2) sno_fatal("alternation with missing operand", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PBALT";
            IR_t * vl = NULL;
            IR_t * el = sx_lower(cx, t->c[0], NULL, ω, &vl);
            IR_t * vr = NULL;
            IR_t * er = sx_lower(cx, t->c[1], NULL, ω, &vr);
            lc_γ_to(vl, er);
            lc_γ_to(vr, mk);
            ir_operand_push(mk, vl);
            ir_operand_push(mk, vr);
            if (res) *res = mk;
            return el;
        }
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        case TT_DIV:
        case TT_POW:
        case TT_SEQ:
        case TT_CAT:
        if (t->n < 2) sno_fatal("binary operator with missing operand", NULL);
        return sx_binop(cx, t, sno_binop_code(t->t), γ, ω, res);
        case TT_MNS:
        case TT_PLS:
        {
            if (t->n < 1) sno_fatal("unary operator with missing operand", NULL);
            IR_t * op = lc_build(cx->g, IR_UNOP, γ, ω);
            IR_LIT(op).ival = (long long) t->t;
            op->strict = 2;
            IR_t * ar = NULL;
            IR_t * ea = sx_lower(cx, t->c[0], op, ω, &ar);
            ir_operand_push(op, ar);
            if (res) *res = op;
            return ea;
        }
        case TT_INDIRECT:
        {
            if (t->n < 1) sno_fatal("indirect reference with no operand", NULL);
            if (t->c[0] && t->c[0]->t == TT_NAME) {
                IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
                IR_t * nr = NULL;
                IR_t * ne = sx_lower(cx, t->c[0], NULL, ω, &nr);
                lc_γ_to(nr, dr);
                ir_operand_push(dr, nr);
                if (res) *res = dr;
                return ne;
            }
            IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
            IR_t * vr = NULL;
            IR_t * ve = sx_nameval(cx, t->c[0], dr, ω, &vr);
            ir_operand_push(dr, vr);
            if (res) *res = dr;
            return ve;
        }
        case TT_IDX:
        {
            IR_t * cur = NULL;
            IR_t * entry = sx_idx_container(cx, t, ω, &cur);
            if (cur && cur->op == IR_SUBSCRIPT && t->n == 2 && IR_LIT(cur).sval && !strcmp(IR_LIT(cur).sval, "container-only")) {
                IR_LIT(cur).sval = "container-value";
                if (sno_sub_val_on()) { lc_γ_to(cur, γ); if (res) *res = cur; return entry; }
            }
            IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
            lc_γ_to(cur, dr);
            ir_operand_push(dr, cur);
            if (res) *res = dr;
            return entry;
        }
        case TT_OPSYN:
        {
            const char * name = t->v.sval;
            if (!name || !*name) sno_fatal("OPSYN operator expression with no symbol", NULL);
            { const char * _tg = sno_t4_target(name, t->n); if (_tg && (sno_predef_registered(_tg) || sno_tdef_registered(_tg))) return sx_call_named(cx, _tg, t, 0, γ, ω, res); }
            return sx_call_named(cx, name, t, 0, γ, ω, res);
        }
        case TT_FNC:
        {
            const char * name = t->v.sval;
            int argbase = 0;
            if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
            if (!name) sno_fatal("call with no resolvable name", NULL);
            if (!strcmp(name, "DEFINE")) {
                char fnb[(t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) ? strlen(t->c[argbase]->v.sval) + 1 : 1];
                fnb[0] = 0;
                if (t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
                    const char * sp = t->c[argbase]->v.sval;
                    int k = 0;
                    for (; sp[k] && sp[k] != '(' && sp[k] != ' '; k++) fnb[k] = sp[k];
                    fnb[k] = 0;
                }
                if (sno_define_entry_computed(t, argbase)) fnb[0] = 0;
                if (fnb[0] && sno_predef_registered(fnb) && !sno_def_entry_absent(t, argbase) && t->c[argbase] && sno_qlit_fold(t->c[argbase])) {
                    sno_def_t d;
                    sno_parse_define(sno_qlit_fold(t->c[argbase]), sno_define_entry_opt(t, argbase), &d);
                    IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω);
                    IR_LIT(nd).sval = (char *) "";
                    IR_t * bind = lc_build(cx->g, IR_DEFINE, nd, ω);
                    IR_LIT(bind).sval = lp_strdup(d.fname);
                    sno_bind_attach_entry(cx->g, bind, d.entry, ω);
                    sno_bind_attach_proto(cx->g, bind, &d, ω);
                    if (res) *res = nd;
                    return bind;
                }
                if (fnb[0] && sno_predef_registered(fnb) && !sno_fname_is_multiproto(fnb) && !sno_def_entry_absent(t, argbase)) {
                    IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω);
                    IR_LIT(nd).sval = (char *) "";
                    if (res) *res = nd;
                    return nd;
                }
                if (fnb[0] && !({ extern int g_rt_fragment_emit; g_rt_fragment_emit; }) && !sno_fname_is_multiproto(fnb) && !sno_def_entry_absent(t, argbase))
                    sno_fatal(
                    "DEFINE in this expression position is outside the landed subset (literal-prototype DEFINE in a statement subject only; pattern/replacement-field and fragment DEFINE pending)",
                    NULL);
            }
            if (!strcmp(name, "CODE") && (t->n - argbase) == 1 && cx->prog_nstmt > 0) {
                IR_t * ar = NULL;
                IR_t * ae = sx_lower(cx, t->c[argbase], NULL, ω, &ar);
                IR_t * cl = lc_build(cx->g, IR_CALL, γ, ω);
                IR_LIT(cl).sval = (char *) "CODE";
                IR_t * bn = lc_build(cx->g, IR_LIT_INTEGER, cl, ω);
                IR_LIT(bn).ival = (int64_t) cx->prog_nstmt;
                lc_γ_to(ar, bn);
                ir_operand_push(cl, ar);
                ir_operand_push(cl, bn);
                if (res) *res = cl;
                return ae;
            }
            {
                int lex = 0;
                long c1 = 0, c2 = 0;
                int rk = sno_pred_relop(name, &lex, &c1, &c2);
                if (rk >= 0 && t->n - argbase >= 1 && t->n - argbase <= 2) return sx_pred_cmp(cx, t, argbase, lex, rk, c1, c2, γ, ω, res);
            }
            {
                int nid = t->n - argbase;
                if (sno_ident_inline_on() && nid >= 1 && nid <= 2 && !sno_predef_registered(name)) {
                    if (!strcmp(name, "IDENT")) return sx_ident_differ(cx, t, argbase, 0, γ, ω, res);
                    if (!strcmp(name, "DIFFER")) return sx_ident_differ(cx, t, argbase, 1, γ, ω, res);
                }
            }
            return sx_call_named(cx, name, t, argbase, γ, ω, res);
        }
        case TT_WHILE:
        case TT_UNTIL:
        {
            const tree_t * C = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
            if (!C) sno_fatal("loop with no condition", NULL);
            int is_until = (t->t == TT_UNTIL);
            IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
            IR_t * cr = NULL;
            IR_t * ce = is_until ? sx_lower(cx, C, γ, lc_build(cx->g, IR_SETEXIT_TEST, gate, gate), &cr) : sx_lower(cx, C, gate, lc_build(cx->g, IR_SETEXIT_TEST, γ, γ), &cr);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = ce;
            IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            lc_γ_to(gate, be);
            if (res) *res = NULL;
            return ce;
        }
        case TT_DO_WHILE:
        {
            const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
            if (!C) sno_fatal("do-while without condition outside the landed subset", NULL);
            IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
            IR_t * cr = NULL;
            IR_t * ce = sx_lower(cx, C, gate, lc_build(cx->g, IR_SETEXIT_TEST, γ, γ), &cr);
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = ce;
            IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            lc_γ_to(gate, be);
            if (res) *res = NULL;
            return be;
        }
        case TT_FOR:
        {
            const tree_t * INIT = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
            const tree_t * STEP = (t->n > 2) ? t->c[2] : NULL;
            const tree_t * B = (t->n > 3) ? t->c[3] : NULL;
            if (!C) sno_fatal("for-loop without condition outside the landed subset", NULL);
            IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
            IR_t * cr = NULL;
            IR_t * ce = sx_lower(cx, C, gate, lc_build(cx->g, IR_SETEXIT_TEST, γ, γ), &cr);
            IR_t * se = STEP ? sx_lower(cx, STEP, ce, ω, NULL) : ce;
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            cx->loop_exit = γ;
            cx->loop_next = se;
            IR_t * be = B ? sco_branch(cx, B, se, ω) : se;
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            lc_γ_to(gate, be);
            IR_t * ie = INIT ? sx_lower(cx, INIT, ce, ω, NULL) : ce;
            if (res) *res = NULL;
            return ie;
        }
        case TT_LOOP_BREAK:
        case TT_LOOP_NEXT:
        {
            if (t->n > 0 && t->c[0]) sno_fatal("labeled break/next outside the SCO-CF-3 subset", NULL);
            IR_t * tgt = (t->t == TT_LOOP_BREAK) ? cx->loop_exit : cx->loop_next;
            if (!tgt) sno_fatal("break/next outside any loop", NULL);
            IR_t * j = lc_build(cx->g, IR_GOTO, tgt, NULL);
            if (res) *res = NULL;
            return j;
        }
        case TT_SCAN:
        {
            if (t->n < 2) sno_fatal("TT_SCAN with missing subject or pattern", NULL);
            extern tree_t * ast_stmt_new(tree_e kind);
            IR_t * pent = NULL;
            IR_t * pasn = NULL;
            const tree_t * sbj = t->c[0];
            const tree_t * pat = sno_pat_valued(cx, &sbj, 0, t->c[1], ω, &pent, &pasn);
            if (res) {
                char * tmpn = sno_scan_tmp("SCV");
                tree_t * capt = ast_stmt_new(TT_CAPT_COND_ASGN);
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
        case TT_ASSIGN:
        {
            const tree_t * L = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * R = (t->n > 1) ? t->c[1] : NULL;
            if (!L) sno_fatal("TT_ASSIGN with no lhs", NULL);
            if (!R) sno_fatal("TT_ASSIGN with no rhs", NULL);
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
                    IR_t * wl = lc_build(cx->g, IR_LIT_STRING, NULL, ω);
                    IR_LIT(wl).sval = (char *) "";
                    IR_t * mk = lc_build(cx->g, IR_CALL, NULL, ω);
                    IR_LIT(mk).sval = (char *) "SNO$WANTNM";
                    lc_γ_to(wl, mk);
                    ir_operand_push(mk, wl);
                    IR_t * cv = NULL;
                    IR_t * e1 = sx_lower(cx, L, NULL, ω, &cv);
                    lc_γ_to(mk, e1);
                    IR_t * vv = NULL;
                    IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
                    lc_γ_to(cv, e2);
                    IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
                    lc_γ_to(vv, asn);
                    ir_operand_push(asn, cv);
                    ir_operand_push(asn, vv);
                    if (res) *res = asn;
                    return wl;
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
        case TT_IF:
        {
            const tree_t * C = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * TH = (t->n > 1) ? t->c[1] : NULL;
            const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
            if (!C) sno_fatal("TT_IF with no condition", NULL);
            IR_t * th_entry = TH ? sco_branch(cx, TH, γ, ω) : γ;
            IR_t * el_entry = EL ? sco_branch(cx, EL, γ, ω) : γ;
            IR_t * cr = NULL;
            IR_t * ce = sx_lower(cx, C, th_entry, lc_build(cx->g, IR_SETEXIT_TEST, el_entry, el_entry), &cr);
            if (res) *res = NULL;
            return ce;
        }
        case TT_NOT:
        {
            const tree_t * inner = (t->n > 0) ? t->c[0] : NULL;
            if (!inner) sno_fatal("TT_NOT with no operand", NULL);
            IR_t * gate = lc_build(cx->g, IR_LIT_STRING, γ, NULL);
            IR_LIT(gate).sval = (char *) "";
            IR_t * r = NULL;
            IR_t * e = sx_lower(cx, inner, ω, gate, &r);
            if (res) *res = gate;
            return e;
        }
        case TT_CASE:
        {
            if (t->n < 1 || !t->c[0]) sno_fatal("TT_CASE with no subject", NULL);
            const tree_t * subj = t->c[0];
            const tree_t * def_body = NULL;
            for (int i = 1; i + 1 < t->n; i += 2) if (t->c[i] && t->c[i]->t == TT_NUL) def_body = t->c[i + 1];
            IR_t * sv_exit = cx->loop_exit;
            IR_t * sv_next = cx->loop_next;
            IR_t * else_tgt;
            if (def_body) { cx->loop_exit = γ; IR_t * db = sco_branch(cx, def_body, γ, ω); cx->loop_exit = sv_exit; else_tgt = db; } else else_tgt = γ;
            IR_t * chain = else_tgt;
            for (int i = t->n - 2; i >= 1; i -= 2) {
                const tree_t * v = t->c[i];
                const tree_t * b = t->c[i + 1];
                if (v && v->t == TT_NUL) continue;
                cx->loop_exit = γ;
                IR_t * body_entry = sco_branch(cx, b, γ, ω);
                cx->loop_exit = sv_exit;
                tree_t * idc = ast_node_new(TT_FNC);
                idc->v.sval = (char *) "IDENT";
                ast_push(idc, (tree_t *) subj);
                ast_push(idc, (tree_t *) v);
                IR_t * ir = NULL;
                IR_t * te = sx_lower(cx, idc, body_entry, lc_build(cx->g, IR_SETEXIT_TEST, chain, chain), &ir);
                chain = te;
            }
            cx->loop_exit = sv_exit;
            cx->loop_next = sv_next;
            if (res) *res = NULL;
            return chain;
        }
        case TT_AUGOP:
        {
            const tree_t * L = (t->n > 0) ? t->c[0] : NULL;
            if (!L || L->t != TT_VAR || !L->v.sval || t->n < 2) sno_fatal("TT_AUGOP outside the landed subset (simple-variable lhs only)", NULL);
            int code;
            switch ((int) t->v.ival) {
                case TK_AUGPLUS:
                code = 0;
                break;
                case TK_AUGMINUS:
                code = 1;
                break;
                case TK_AUGSTAR:
                code = 2;
                break;
                case TK_AUGSLASH:
                code = 3;
                break;
                case TK_AUGMOD:
                code = 4;
                break;
                case TK_AUGPOW:
                code = (int)BINOP_POW_PROMOTE;
                break;
                default:
                sno_fatal("TT_AUGOP operator outside the landed subset", NULL);
                code = 0;
            }
            sno_reg_var(L->v.sval);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω);
            IR_LIT(asn).sval = L->v.sval;
            IR_t * vr = NULL;
            IR_t * e = sx_binop(cx, t, code, asn, ω, &vr);
            ir_operand_push(asn, vr);
            if (res) *res = asn;
            return e;
        }
        case TT_RETURN:
        case TT_NRETURN:
        case TT_PROC_FAIL:
        {
            const char * rl = (t->t == TT_PROC_FAIL) ? "FRETURN" : (t->t == TT_NRETURN) ? "NRETURN" : "RETURN";
            IR_t * j = lc_build(cx->g, IR_GOTO, bb_label_landing(rl), NULL);
            if (res) *res = NULL;
            if (t->t == TT_RETURN && t->n > 0 && t->c[0] && cx->result_name) {
                sno_reg_var(cx->result_name);
                IR_t * asn = lc_build(cx->g, IR_ASSIGN, j, j);
                IR_LIT(asn).sval = (char *) cx->result_name;
                IR_t * vr = NULL;
                IR_t * e = sx_lower(cx, t->c[0], asn, j, &vr);
                ir_operand_push(asn, vr);
                return e;
            }
            return j;
        }
        case TT_GOTO_U:
        case TT_GOTO_S:
        case TT_GOTO_F:
        {
            const char * nm = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : t->v.sval;
            if (!nm || !nm[0]) sno_fatal("goto with no resolvable label", NULL);
            IR_t * land = bb_label_landing(nm);
            if (!land && (!strcmp(nm, "CONTINUE") || !strcmp(nm, "SCONTINUE") || !strcmp(nm, "ABORT"))) {
                land = lc_build(cx->g, IR_GOTO_DEFERRED, bb_label_landing("END"), NULL);
                IR_LIT(land).sval = lp_strdup(nm);
            }
            if (!land) sno_fatal("goto to unknown label", nm);
            IR_t * taken = lc_build(cx->g, IR_GOTO, land, NULL);
            if (res) *res = NULL;
            if (t->t == TT_GOTO_U) return taken;
            return lc_build(cx->g, IR_GOTO, (t->t == TT_GOTO_S) ? taken : γ, (t->t == TT_GOTO_S) ? γ : taken);
        }
        case TT_CAPT_CURSOR:
        {
            const char * cvn = sno_cursor_target((t->n > 0) ? t->c[0] : NULL);
            if (!cvn) sno_fatal("@ cursor-position capture target is not a simple variable", NULL);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω);
            IR_LIT(mk).sval = (char *) "SNO$PCUR";
            IR_t * nl = lc_build(cx->g, IR_LIT_STRING, mk, ω);
            IR_LIT(nl).sval = (char *) cvn;
            ir_operand_push(mk, nl);
            if (res) *res = mk;
            return nl;
        }
        case TT_VLIST:
        {
            if (t->n <= 1) { const tree_t * first = (t->n > 0) ? t->c[0] : NULL; return sx_lower(cx, first, γ, ω, res); }
            {
                int cf = 1;
                for (int j = 0; j + 1 < t->n && cf; j++) cf = sno_vl_quiet(t->c[j]);
                if (cf) {
                    char * tn = sno_scan_tmp("SNO$VL");
                    IR_t * join = lc_build(cx->g, IR_VAR, γ, ω);
                    IR_LIT(join).sval = tn;
                    if (res) *res = join;
                    return sno_vlist_arms(cx, t, 0, tn, join, ω);
                }
            }
            IR_graph_t * g = cx->g;
            IR_t * dj = lc_build(g, IR_DISJUNCTION, γ, ω);
            int n = t->n;
            IR_t * resv[n];
            for (int j = 0; j < n; j++) {
                int before = g->n;
                IR_t * ar = NULL;
                IR_t * ej = sx_lower(cx, t->c[j], dj, dj, &ar);
                for (int k = before; k < g->n; k++) {
                    IR_t * x = g->all[k];
                    if (!x) continue;
                    if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                    if (x->γ.node == dj) { if (x->op == IR_GOTO && x->ω.node == dj) memcpy(x->γ.sz, "φ", 3); else memcpy(x->γ.sz, "σ", 3); x->γ.sz[3] = 0; }
                }
                ir_operand_push(dj, ej);
                ir_operand_push(dj, dj);
                resv[j] = ar;
            }
            for (int j = 0; j < n; j++) ir_operand_push(dj, sno_arm_result(resv[j]));
            IR_LIT(dj).ival = (long) n;
            { extern void fc_vdj_register(const IR_t *); fc_vdj_register(dj); }
            if (res) *res = dj;
            return dj;
        }
        case TT_INTERROGATE:
        {
            if (t->n < 1 || !t->c[0]) sno_fatal("? interrogation with no operand", NULL);
            IR_t * nul = lc_build(cx->g, IR_LIT_STRING, γ, ω);
            IR_LIT(nul).sval = (char *) "";
            IR_t * entry = sx_lower(cx, t->c[0], nul, ω, NULL);
            if (res) *res = nul;
            return entry;
        }
        default:
        { char buf[64]; snprintf(buf, sizeof buf, "tree kind %d", (int) t->t); sno_fatal("expression form not in the landed subset", buf); return NULL; }
    }
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
static const tree_t * sgoto_expr(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind || a->n == 0 || !a->c[0]) continue;
        const tree_t * g0 = a->c[0];
        if (g0->t == TT_QLIT) return NULL;
        if (g0->t == TT_INDIRECT && g0->n > 0 && g0->c[0] && g0->c[0]->t == TT_VAR) return NULL;
        if (g0->t == TT_GOTO_DIRECT) return NULL;
        return g0;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sgoto_direct(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind || a->n == 0 || !a->c[0]) continue;
        if (a->c[0]->t == TT_GOTO_DIRECT) return (a->c[0]->n > 0) ? a->c[0]->c[0] : NULL;
        return NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_const_prefix(const tree_t * t) {
    if (!t) return NULL;
    if (t->t == TT_QLIT) return t->v.sval ? t->v.sval : "";
    if ((t->t == TT_CAT || t->t == TT_SEQ) && t->n >= 1) return sno_expr_const_prefix(t->c[0]);
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
    if (g_sno_uses_stmtkw) {
        IR_t * hook = lc_build(cx->g, IR_CALL, body, body);
        IR_LIT(hook).sval = (char *) "SNO$STMT";
        IR_t * num = lc_build(cx->g, IR_LIT_INTEGER, hook, hook);
        IR_LIT(num).ival = (int64_t) stno;
        IR_t * lnn = lc_build(cx->g, IR_LIT_INTEGER, hook, hook);
        IR_LIT(lnn).ival = (int64_t) line;
        lc_γ_to(num, lnn);
        ir_operand_push(hook, num);
        ir_operand_push(hook, lnn);
        return num;
    }
    return body;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_goto_specials_impossible(const tree_t * expr) {
    const char * pfx = sno_expr_const_prefix(expr);
    if (!pfx || !*pfx) return 0;
    static const char * const sn3[3] = { "NRETURN", "FRETURN", "RETURN" };
    for (int k = 0; k < 3; k++) { size_t lp = strlen(pfx), ls = strlen(sn3[k]), m = lp < ls ? lp : ls; if (!strncmp(pfx, sn3[k], m)) return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_special_chain(IR_graph_t * g, const char * dn, IR_t * tail) {
    static int _sp = -1;
    if (_sp < 0) { const char * e = getenv("SCRIP_GOTO_SPECIAL_TRANSFER"); _sp = (e && *e == '0') ? 0 : 1; }
    if (!_sp || !dn || (dn[0] != '$' && dn[0] != '@')) return tail;
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
    if (!g_sno_traces_a_label || !nm || !nm[0] || !land) return land;
    if (sno_label_reserved(nm)) return land;
    IR_t * hook = lc_build(g, IR_CALL, land, land);
    IR_LIT(hook).sval = (char *) "SNO$STMT";
    IR_t * nmn = lc_build(g, IR_LIT_STRING, hook, hook);
    IR_LIT(nmn).sval = lp_strdup(nm);
    ir_operand_push(hook, nmn);
    bb_src_note(nmn, "sno_label_hook", 0);
    return nmn;
}
static IR_t * sno_goto_target(IR_graph_t * g, const char * nm, IR_t * exitnd) {
    extern int g_rt_fragment_emit;
    IR_t * l = (nm && nm[0] != '$') ? bb_label_landing(nm) : NULL;
    if (l && (g_rt_fragment_emit || g_sno_calls_code) && !sno_label_reserved(nm)) l = NULL;
    if (l) return l;
    if (!nm || !nm[0]) sno_fatal("goto to unknown label", "?");
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL);
    IR_LIT(gd).sval = lp_strdup(nm);
    return sno_goto_special_chain(g, IR_LIT(gd).sval, gd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_computed_target(IR_graph_t * g, scx_t * cx, const tree_t * expr, IR_t * exitnd) {
    static int g_igt_n = 0;
    static int _bn = -1;
    if (_bn < 0) { const char * e = getenv("SCRIP_GOTO_CALL_BYNAME"); _bn = (e && *e == '0') ? 0 : 1; }
    int by_name = _bn && expr && expr->t == TT_FNC;
    char nmb[24];
    snprintf(nmb, sizeof nmb, "IGT$%d", g_igt_n++);
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
    IR_t * ec = sx_lower(cx, expr, asn, gd, &vr);
    ir_operand_push(asn, vr);
    if (!by_name) return ec;
    IR_t * wn_lit = lc_build(g, IR_LIT_STRING, NULL, gd);
    IR_LIT(wn_lit).sval = (char *) "";
    IR_t * wn_call = lc_build(g, IR_CALL, NULL, gd);
    IR_LIT(wn_call).sval = (char *) "SNO$WANTNM";
    lc_γ_to(wn_lit, wn_call);
    lc_γ_to(wn_call, ec);
    ir_operand_push(wn_call, wn_lit);
    return wn_lit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_direct_target(IR_graph_t * g, scx_t * cx, const tree_t * expr, IR_t * exitnd) {
    static int g_dgt_n = 0;
    char nmb[24];
    snprintf(nmb, sizeof nmb, "DGT$%d", g_dgt_n++);
    char * tmpn = lp_strdup(nmb);
    sno_reg_var(tmpn);
    size_t ln = strlen(tmpn);
    char * dn = (char *) ct_alloc(ln + 2);
    dn[0] = '<';
    memcpy(dn + 1, tmpn, ln);
    dn[ln + 1] = 0;
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL);
    IR_LIT(gd).sval = dn;
    IR_t * asn = lc_build(g, IR_ASSIGN, gd, gd);
    IR_LIT(asn).sval = tmpn;
    IR_t * vr = NULL;
    IR_t * ec = sx_lower(cx, expr, asn, gd, &vr);
    ir_operand_push(asn, vr);
    return ec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_branch(IR_graph_t * g, scx_t * cx, const char * nm, const tree_t * dc, const tree_t * ex, IR_t * exitnd) {
    if (nm) return sno_goto_target(g, nm, exitnd);
    if (dc) return sno_goto_direct_target(g, cx, dc, exitnd);
    if (ex) return sno_goto_computed_target(g, cx, ex, exitnd);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_qlit_fold(const tree_t * t) {
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
static void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d);
static int sno_def_entry_absent(const tree_t * subj, int argbase) {
    const char * entry_opt = NULL;
    sno_def_t d;
    if (!subj || subj->n <= argbase || !subj->c[argbase] || !sno_qlit_fold(subj->c[argbase])) return 0;
    if (subj->n > argbase + 1 && subj->c[argbase + 1]) {
        const tree_t * ea = subj->c[argbase + 1];
        if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
        else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
        else return 0;
    }
    sno_parse_define(sno_qlit_fold(subj->c[argbase]), entry_opt, &d);
    if (sn4_sysfn_protected(d.fname)) return 1;
    if (!g_sno_uses_code) return 0;
    { const char * el = (d.entry && d.entry[0]) ? d.entry : d.fname; return (el && el[0] && !bb_label_landing(el)) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_stmt_define(const tree_t * s, int * out_argbase) {
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
static const char * sno_define_entry_opt(const tree_t * dsub, int argbase) {
    if (!dsub || dsub->n <= argbase + 1 || !dsub->c[argbase + 1]) return NULL;
    const tree_t * ea = dsub->c[argbase + 1];
    if (ea->t == TT_QLIT && ea->v.sval) return ea->v.sval;
    if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) return ea->c[0]->v.sval;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static cv_t g_sno_multiproto;
static int sno_fname_is_multiproto(const char * fname) { for (uint32_t k = 0; k < g_sno_multiproto.len; k++) if (!strcmp(CV_AT(g_sno_multiproto, const char *, k), fname)) return 1; return 0; }
static const char * sno_proto_encode(const sno_def_t * d) {
    size_t need = (size_t) fmt_len("%d|", d->nformals);
    for (int k = 0; k < d->nnames; k++) need += (size_t) fmt_len("%s%s", k ? "," : "", d->names[k]) - 1;
    char e[need];
    int n = snprintf(e, need, "%d|", d->nformals);
    for (int k = 0; k < d->nnames && (size_t)n < need - 1; k++) n += snprintf(e + n, need - (size_t)n, "%s%s", k ? "," : "", d->names[k]);
    return lp_strdup(e);
}
static cv_t g_sno_proto_fn;
static cv_t g_sno_proto_enc;
static void sno_proto_note(const sno_def_t * d) {
    const char * enc = sno_proto_encode(d);
    for (uint32_t k = 0; k < g_sno_proto_fn.len; k++) if (!strcmp(CV_AT(g_sno_proto_fn, const char *, k), d->fname)) {
        if (strcmp(CV_AT(g_sno_proto_enc, const char *, k), enc) && !sno_fname_is_multiproto(d->fname)) CV_PUSH(g_sno_multiproto, const char *) = lp_strdup(d->fname);
        CV_AT(g_sno_proto_enc, const char *, k) = enc;
        return;
    }
    { const char * fn = lp_strdup(d->fname); CV_PUSH(g_sno_proto_fn, const char *) = fn; CV_PUSH(g_sno_proto_enc, const char *) = enc; }
}
static void sno_bind_attach_proto(IR_graph_t * g, IR_t * bind, const sno_def_t * d, IR_t * fail) {
    if (!sno_fname_is_multiproto(d->fname)) return;
    IR_t * pr = lc_build(g, IR_LIT_STRING, bind, fail);
    IR_LIT(pr).sval = (char *) sno_proto_encode(d);
    ir_operand_push(bind, pr);
}
static void sno_bind_attach_entry(IR_graph_t * g, IR_t * bind, const char * entry, IR_t * fail) {
    if (!g || !bind || !entry || !entry[0]) return;
    IR_t * en = lc_build(g, IR_LIT_NAME, bind, fail);
    IR_LIT(en).sval = lp_strdup(entry);
    ir_operand_push(bind, en);
    bind->pat_static = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d) {
    char buf[strlen(spec) + 1];
    int bn = 0;
    for (const char * p = spec; *p; p++) if (*p != ' ' && *p != '\t') buf[bn++] = *p;
    buf[bn] = 0;
    char * par = strchr(buf, '(');
    char * cls = par ? strchr(par, ')') : NULL;
    if (!par || !cls) sno_fatal("DEFINE prototype missing parameter parentheses", spec);
    *par = 0;
    *cls = 0;
    if (!buf[0]) sno_fatal("DEFINE prototype missing function name", spec);
    d->fname = lp_strdup(buf);
    d->entry = (entry_opt && entry_opt[0]) ? lp_strdup(entry_opt) : d->fname;
    d->result_name = NULL;
    d->nnames = 0;
    for (char * seg = par + 1; seg && *seg; ) {
        char * cm = strchr(seg, ',');
        if (cm) *cm = 0;
        if (*seg && d->nnames < SNO_DEF_NAMES_MAX) d->names[d->nnames++] = lp_strdup(seg);
        seg = cm ? cm + 1 : NULL;
    }
    d->nformals = d->nnames;
    for (char * seg = cls + 1; seg && *seg; ) {
        char * cm = strchr(seg, ',');
        if (cm) *cm = 0;
        if (*seg && d->nnames < SNO_DEF_NAMES_MAX) d->names[d->nnames++] = lp_strdup(seg);
        seg = cm ? cm + 1 : NULL;
    }
    sno_reg_var(d->fname);
    for (int k = 0; k < d->nnames; k++) sno_reg_var(d->names[k]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res) {
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
static IR_t * sx_subscript_lv_fused(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res, IR_t ** fuse_base, IR_t ** fuse_idx) {
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
extern int ir_is_generator_kind(IR_e t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_ω_to(IR_t * nd, IR_t * t) { if (t) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_resume_ω_to(IR_graph_t * g, int tail_idx, IR_t * nd, IR_t * t) {
    if (nd && nd->op == IR_MATCH_ALTERNATE && nd->n_operands == 0 && g && tail_idx + 1 < g->n) {
        IR_t * T = g->all[tail_idx + 1];
        if (T && T->op == IR_MATCH_ALTERNATE && T->n_operands > 0 && T->operands[0] == nd) { if (t) lc_γ_to_β(T, t); else lc_γ_to(T, t); return; }
    }
    if (nd && nd->op == IR_MATCH_ASSIGN_COND && nd->n_operands > 1 && nd->operands[1]) nd = nd->operands[1];
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_e sno_pat_eff_kind(const tree_t * t) {
    if (!t) return TT_VAR;
    if ((t->t == TT_ARB || t->t == TT_BAL || t->t == TT_REM || t->t == TT_FAIL || t->t == TT_SUCCEED || t->t == TT_ABORT) && t->v.sval && sno_predef_registered(t->v.sval)) return TT_FNC;
    if ((t->t != TT_VAR && t->t != TT_KEYWORD) || !t->v.sval) return t->t;
    static const struct {
        const char * n;
        tree_e k;
    } m[] = { { "ABORT", TT_ABORT }, { "ARB", TT_ARB }, { "BAL", TT_BAL }, { "FAIL", TT_FAIL }, { "FENCE", TT_FENCE }, { "FLUSH", TT_FLUSH }, { "REM", TT_REM }, { "SUCCEED", TT_SUCCEED }, { NULL,
        TT_VAR } };
    const char * nm = t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval;
    for (int i = 0; m[i].n; i++) if (!strcmp(nm, m[i].n)) return m[i].k;
    return t->t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_fence(const tree_t * t) { if (!t) return 0; const tree_e k = sno_pat_eff_kind(t); return k == TT_FENCE || k == TT_FLUSH; }
static int sno_is_flush(const tree_t * t) { return t && sno_pat_eff_kind(t) == TT_FLUSH; }
static int sno_is_fence1(const tree_t * t) { return t && t->t == TT_FENCE && t->n > 0; }
static int sno_is_fence0(const tree_t * t) { return sno_is_fence(t) && !sno_is_fence1(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const tree_t ** v; int n, cap; } sno_tvec_t;
static void sno_tvec_push(sno_tvec_t * e, const tree_t * t) {
    if (e->n >= e->cap) { e->cap = e->cap ? e->cap * 2 : 16; e->v = (const tree_t **) ct_grow((void *) e->v, (size_t) e->cap * sizeof *e->v); }
    e->v[e->n++] = t;
}
static void sno_seq_flatten_pat(const tree_t * t, sno_tvec_t * e) {
    if (!t) return;
    if (t->t == TT_SEQ) { sno_seq_flatten_pat((t->n > 0) ? t->c[0] : NULL, e); if (t->n > 1 && t->c[1]) sno_tvec_push(e, t->c[1]); return; }
    sno_tvec_push(e, t);
}
static const tree_t * sno_const_pat(const char * ck);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_cset_fold(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT) return a->v.sval ? a->v.sval : "";
    if (a->t == TT_ILIT) { char nb[24]; snprintf(nb, sizeof nb, "%lld", (long long) a->v.ival); char * ob = (char *) ct_alloc(strlen(nb) + 1); if (!ob) return NULL; strcpy(ob, nb); return ob; }
    if (a->t == TT_KEYWORD && a->v.sval) {
        static const struct { const char * n; const char * v; } kc[] = { { "lcase", "abcdefghijklmnopqrstuvwxyz" }, { "ucase", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" } };
        char lk[strlen(a->v.sval) + 1];
        size_t li = 0;
        for (; a->v.sval[li]; li++) lk[li] = (a->v.sval[li] >= 'A' && a->v.sval[li] <= 'Z') ? (char)(a->v.sval[li] - 'A' + 'a') : a->v.sval[li];
        lk[li] = 0;
        for (size_t k = 0; k < sizeof kc / sizeof *kc; k++) if (!strcmp(lk, kc[k].n)) return kc[k].v;
        {
            char cb[fmt_len("&%s", a->v.sval[0] == '&' ? a->v.sval + 1 : a->v.sval)];
            snprintf(cb, sizeof cb, "&%s", a->v.sval[0] == '&' ? a->v.sval + 1 : a->v.sval);
            const tree_t * cv = sno_const_val(cb);
            if (cv) return sno_cset_fold(cv);
            const tree_t * cp = sno_const_pat(cb);
            if (cp && cp != a) return sno_cset_fold(cp);
        }
    }
    if (a->t == TT_FNC && a->v.sval && (!strcmp(a->v.sval, "CHAR") || !strcmp(a->v.sval, "char")) && a->n == 1 && a->c[0] && a->c[0]->t == TT_ILIT && a->c[0]->v.ival >= 1 && a->c[0]->v.ival <= 255) {
        char * cb = (char *) ct_alloc(2);
        if (!cb) return NULL;
        cb[0] = (char)(unsigned char) a->c[0]->v.ival;
        cb[1] = 0;
        return cb;
    }
    if (a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "SUBSTR") && a->n == 3 && a->c[0] && a->c[0]->t == TT_KEYWORD && a->c[0]->v.sval &&
        !strcmp(a->c[0]->v.sval + (a->c[0]->v.sval[0] == '&'), "ALPHABET") && a->c[1] && a->c[1]->t == TT_ILIT && a->c[2] && a->c[2]->t == TT_ILIT) {
        long i = (long) a->c[1]->v.ival, n = (long) a->c[2]->v.ival;
        if (i < 2 || n < 1 || i + n - 1 > 256) return NULL;
        char * cb = (char *) ct_alloc((size_t) n + 1);
        if (!cb) return NULL;
        for (long k = 0; k < n; k++) cb[k] = (char)(unsigned char)(i - 1 + k);
        cb[n] = 0;
        return cb;
    }
    if (a->t == TT_SEQ || a->t == TT_CAT) {
        const char * l = sno_cset_fold((a->n > 0) ? a->c[0] : NULL);
        if (!l) return NULL;
        const char * r = sno_cset_fold((a->n > 1) ? a->c[1] : NULL);
        if (!r) return NULL;
        size_t ln = strlen(l), rn = strlen(r);
        char * buf = (char *) ct_alloc(ln + rn + 1);
        if (!buf) return NULL;
        memcpy(buf, l, ln);
        memcpy(buf + ln, r, rn);
        buf[ln + rn] = 0;
        return buf;
    }
    return NULL;
}
static int sno_is_pattern_rhs(const tree_t * t);
static int sno_pat_supported(const tree_t * t);
static const char * sno_pat_collect(const tree_t * pat);
static int g_sno_in_patproc = 0;
static int g_sno_pat_match_ctx = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_sno_seal_enabled = 0;
typedef struct { const char * name; const tree_t * pat; const tree_t * val; } sno_seal_ent_t;
static cv_t g_sno_seal;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_seal_note(const char * nm, const tree_t * pat) {
    if (!nm || !pat) return;
    for (uint32_t i = 0; i < g_sno_seal.len; i++) if (!strcmp(CV_AT(g_sno_seal, sno_seal_ent_t, i).name, nm)) return;
    { sno_seal_ent_t x; x.name = nm; x.pat = pat; x.val = NULL; CV_PUSH(g_sno_seal, sno_seal_ent_t) = x; }
}
static int sno_pat_right_sealed(const tree_t * t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_const_feature(int set_off) {
    static int _cs = -1;
    if (set_off) { _cs = 0; return 0; }
    if (_cs < 0) { const char * e = getenv("SCRIP_CONST_STATIC"); _cs = (e && *e == '0') ? 0 : 1; }
    return _cs;
}
static int sno_const_static_on(void) { return sno_const_feature(0); }
static const tree_t * sno_const_pat(const char * ck) {
    if (!sno_const_static_on() || !g_sno_seal_enabled || !ck) return NULL;
    for (int i = 0; i < (int) g_sno_seal.len; i++) if (!strcmp(CV_AT(g_sno_seal, sno_seal_ent_t, i).name, ck)) return CV_AT(g_sno_seal, sno_seal_ent_t, i).pat;
    return NULL;
}
static int sno_const_t1_on(void) { static int _t = -1; if (_t < 0) { const char * e = getenv("SCRIP_CONST_T1"); _t = (e && *e == '0') ? 0 : 1; } return _t; }
static int sno_const_scalar_tree(const tree_t * t) { return t && (t->t == TT_ILIT || t->t == TT_FLIT || t->t == TT_QLIT); }
static void sno_const_note_val(const char * nm, const tree_t * val) {
    if (!nm || !val) return;
    for (uint32_t i = 0; i < g_sno_seal.len; i++) if (!strcmp(CV_AT(g_sno_seal, sno_seal_ent_t, i).name, nm)) return;
    { sno_seal_ent_t x; x.name = nm; x.pat = NULL; x.val = val; CV_PUSH(g_sno_seal, sno_seal_ent_t) = x; }
}
static const tree_t * sno_protected_kw_const(const char * ck) {
    const char * v = !strcmp(ck, "&UCASE") ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : !strcmp(ck, "&LCASE") ? "abcdefghijklmnopqrstuvwxyz" : (const char *) 0;
    if (!v) return (const tree_t *) 0;
    { tree_t * q = ast_node_new(TT_QLIT); q->v.sval = (char *) v; return q; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_const_val(const char * ck) {
    if (ck) { const tree_t * pk = sno_protected_kw_const(ck); if (pk) return pk; }
    if (!sno_const_static_on() || !sno_const_t1_on() || !g_sno_seal_enabled || !ck) return NULL;
    if (rt_kw_index(ck) >= 0) return NULL;
    for (int i = 0; i < (int) g_sno_seal.len; i++) if (!strcmp(CV_AT(g_sno_seal, sno_seal_ent_t, i).name, ck)) return CV_AT(g_sno_seal, sno_seal_ent_t, i).val;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_dfree(const tree_t * t, int spine, int depth) {
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
static cv_t g_sno_pro;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prologue_add(const char * nm) {
    if (!nm) return;
    for (uint32_t i = 0; i < g_sno_pro.len; i++) if (!strcmp(CV_AT(g_sno_pro, const char *, i), nm)) return;
    CV_PUSH(g_sno_pro, const char *) = nm;
}
int sno_name_prologue_bound(const char * nm) { if (!nm) return 0; for (uint32_t i = 0; i < g_sno_pro.len; i++) if (!strcmp(CV_AT(g_sno_pro, const char *, i), nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * op; const char * tgt; int arity; int poisoned; } sno_t4_t;
static cv_t g_sno_t4;
static int g_sno_t4_unsafe = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_t4_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_OPSYN_FOLD"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sno_t4_opchar(char c) { return !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '.'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_t4_scan(const tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) sno_t4_scan(t->c[i]);
    if (t->t != TT_FNC) return;
    const char * nm = t->v.sval;
    int ab = 0;
    if (!nm && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { nm = t->c[0]->v.sval; ab = 1; }
    if (!nm || strcmp(nm, "OPSYN")) return;
    {
        int na = t->n - ab;
        const tree_t * a0 = (na > 0) ? t->c[ab] : NULL;
        const tree_t * a1 = (na > 1) ? t->c[ab + 1] : NULL;
        const tree_t * a2 = (na > 2) ? t->c[ab + 2] : NULL;
        const char * op = (a0 && a0->t == TT_QLIT) ? a0->v.sval : NULL;
        const char * tg = (a1 && a1->t == TT_QLIT) ? a1->v.sval : NULL;
        long ar = (a2 && a2->t == TT_ILIT) ? a2->v.ival : -1;
        if (op && tg && *op && sno_t4_opchar(*op)) {
            if (na == 3 && (ar == 1 || ar == 2)) {
                for (int i = 0; i < (int) g_sno_t4.len; i++) if (!strcmp(CV_AT(g_sno_t4, sno_t4_t, i).op, op)) {
                    if (strcmp(CV_AT(g_sno_t4, sno_t4_t, i).tgt, tg) || CV_AT(g_sno_t4, sno_t4_t, i).arity != (int) ar) CV_AT(g_sno_t4, sno_t4_t, i).poisoned = 1;
                    return;
                }
                { sno_t4_t x; x.op = op; x.tgt = tg; x.arity = (int) ar; x.poisoned = 0; CV_PUSH(g_sno_t4, sno_t4_t) = x; }
                return;
            }
            g_sno_t4_unsafe = 1;
            return;
        }
        if (op && tg) return;
        g_sno_t4_unsafe = 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_t4_target(const char * op, int nops) {
    if (!sno_t4_on() || g_sno_t4_unsafe || !op) return NULL;
    for (int i = 0; i < (int) g_sno_t4.len; i++) if (!CV_AT(g_sno_t4, sno_t4_t, i).poisoned && !strcmp(CV_AT(g_sno_t4, sno_t4_t, i).op, op) && CV_AT(g_sno_t4, sno_t4_t, i).arity == nops)
        return CV_AT(g_sno_t4, sno_t4_t, i).tgt;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prog_scan(const tree_t ** st, int nst) {
    g_sno_pro.len = 0;
    g_sno_t4.len = 0;
    g_sno_t4_unsafe = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i];
        if (!s) continue;
        sno_t4_scan(s);
        const tree_t * subj = lc_stmt_subj(s);
        const tree_t * repl = sfind_expr(s, ":repl");
        int has_eq = sfind(s, ":eq") != NULL;
        if (!has_eq || !subj || subj->t != TT_KEYWORD || !subj->v.sval) continue;
        if (repl) {
            const char * kn = subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval;
            if (!strcasecmp(kn, "USER_DECLARED_CONSTANTS") && repl->t == TT_ILIT && repl->v.ival == 0) sno_const_feature(1);
        }
        if (sno_const_static_on() && repl && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) {
            char cb[130];
            snprintf(cb, sizeof cb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval);
            sno_seal_note(lp_strdup(cb), repl);
        }
        if (sno_const_static_on() && sno_const_t1_on() && sno_const_scalar_tree(repl)) {
            char vb[130];
            snprintf(vb, sizeof vb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval);
            if (rt_kw_index(vb) < 0) sno_const_note_val(lp_strdup(vb), repl);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long sno_prearg_codes(int tt) {
    switch (tt) {
        case TT_ANY:
        return 59L | (59L << 16);
        case TT_BREAK:
        return 69L | (69L << 16);
        case TT_BREAKX:
        return 70L | (70L << 16);
        case TT_NOTANY:
        return 151L | (151L << 16);
        case TT_SPAN:
        return 188L | (188L << 16);
        case TT_LEN:
        return 120L | (121L << 16);
        case TT_POS:
        return 162L | (163L << 16);
        case TT_RTAB:
        return 181L | (182L << 16);
        case TT_TAB:
        return 183L | (184L << 16);
        case TT_RPOS:
        return 185L | (186L << 16);
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_pat_defer_arg_name(const tree_t * inner, int lead_star) {
    if (inner && inner->t == TT_VAR && inner->v.sval && inner->v.sval[0]) {
        char b[fmt_len("%s%s", lead_star ? "*" : "", inner->v.sval)];
        snprintf(b, sizeof b, "%s%s", lead_star ? "*" : "", inner->v.sval);
        return lp_strdup(b);
    }
    const char * xn = sno_expr_collect(inner);
    char b[fmt_len("%s*%s", lead_star ? "*" : "", xn)];
    snprintf(b, sizeof b, "%s*%s", lead_star ? "*" : "", xn);
    return lp_strdup(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_pre_req(scx_t * cx, const tree_t * t, IR_t * prim) {
    const tree_t * arg = (t->n > 0) ? t->c[0] : NULL;
    if (arg && arg->t == TT_DEFER && arg->n > 0 && arg->c[0]) arg = arg->c[0];
    if (!arg || arg->t == TT_DEFER) sno_fatal("pattern primitive argument outside the operand-edge subset (missing or deferred *expr argument)", NULL);
    if (cx->npre >= 64) sno_fatal("too many runtime pattern-primitive arguments in one statement (operand-edge pre-chain limit 64)", NULL);
    cx->pre[cx->npre].arg = arg;
    cx->pre[cx->npre].prim = prim;
    cx->pre[cx->npre].str = (t->t == TT_ANY || t->t == TT_NOTANY || t->t == TT_SPAN || t->t == TT_BREAK || t->t == TT_BREAKX);
    cx->pre[cx->npre].codes = sno_prearg_codes(t->t);
    cx->pre[cx->npre].snapg = NULL;
    cx->npre++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_lambda_kind(const char * n, int nargs) {
    if (!n || nargs != 1) return 0;
    if (!strcmp(n, "LAMBDA") || !strcmp(n, "\xce\x9b")) return 1;
    if (!strcmp(n, "lambda") || !strcmp(n, "\xce\xbb")) return 2;
    return 0;
}
static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail);
static int sno_in_arbno = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_tree_has_varext(const IR_t * n, int d) {
    if (!n || d > 12) return 0;
    if (n->op == IR_MATCH_ARBNO || n->op == IR_MATCH_DEFER) return 1;
    for (int i = 0; i < n->n_operands; i++) if (sno_tree_has_varext(n->operands[i], d + 1)) return 1;
    return 0;
}
typedef struct { const IR_t * nd; const IR_t * save; int nd_idx; int save_idx; int i_end; int fp_inner; int promo; } sno_scd_t;
static cv_t scd;
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cap_defer_reset(void) { scd.len = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cap_fc(IR_graph_t * g, IR_t * nd, IR_t * save, int before_i) {
    int fp_inner = 0;
    int walk_ok = fc_walk_range(g, before_i, g->n, 0, &fp_inner);
    if (!walk_ok) return;
    if (sno_in_arbno == 0) {
        extern void fc_save_register(const IR_t *);
        extern void fc_cond_register_with_save(const IR_t *, const IR_t *, int);
        fc_save_register(save);
        fc_cond_register_with_save(nd, save, fp_inner);
        return;
    }
    { sno_scd_t x; x.nd = nd; x.save = save; x.nd_idx = before_i - 2; x.save_idx = before_i - 1; x.i_end = g->n; x.fp_inner = fp_inner; x.promo = 0; CV_PUSH(scd, sno_scd_t) = x; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp) {
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
            static int _dtl = -1;
            if (_dtl < 0) { const char * _e = getenv("SCRIP_ARBNO_LATCH"); _dtl = _e ? (atoi(_e) != 0) : 0; }
            if (_dtl && x->seal == 2 && IR_LIT(x).sval && sno_name_prologue_bound(IR_LIT(x).sval)) {
                const char * _pn = 0;
                for (int _j = 0; _j < x->n_operands; _j++) {
                    IR_t * _o = x->operands[_j];
                    if (_o && _o->op == IR_LIT_STRING && IR_LIT(_o).sval && !strncmp(IR_LIT(_o).sval, "PAT$", 4)) { _pn = IR_LIT(_o).sval; break; }
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
            case IR_LIT_REAL:
            break;
            default:
            return 0;
        }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_cap_name_strict(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_CAP_NAME_STRICT"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sno_rtseq_resume(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_RTSEQ_RESUME"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sno_defer_resume(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_RESUME"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sno_seq_tail(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_SEQ_TAIL"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sno_alt_tail(void) { const char * e = getenv("SCRIP_ALT_TAIL"); return (e && *e == '0') ? 0 : 1; }
static int sno_fence_rtail(void) { const char * e = getenv("SCRIP_FENCE_RTAIL"); return (e && *e == '0') ? 0 : 1; }
static int sno_pat_contains_fence(const tree_t * t, int depth) {
    if (!t || depth > 64) return 0;
    if (sno_is_fence(t)) return 1;
    for (int i = 0; i < t->n; i++) if (sno_pat_contains_fence(t->c[i], depth + 1)) return 1;
    return 0;
}
static int sno_pat_contains_fence0(const tree_t * t, int depth) {
    if (!t || depth > 64) return 0;
    if (sno_is_fence0(t)) return 1;
    for (int i = 0; i < t->n; i++) if (sno_pat_contains_fence0(t->c[i], depth + 1)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_seq_nary(scx_t * cx, const tree_t ** elems, int ne, IR_t * succ, IR_t * fail, IR_t ** out_rtail) {
    IR_graph_t * g = cx->g;
    IR_t * S = lc_build(g, IR_GOTO, succ, NULL);
    sno_ω_to(S, fail);
    IR_t ** ent = (IR_t **) ct_alloc((size_t) (ne > 0 ? ne : 1) * sizeof *ent);
    IR_t ** res = (IR_t **) ct_alloc((size_t) (ne > 0 ? ne : 1) * sizeof *res);
    int * lo = (int *) ct_alloc((size_t) (ne > 0 ? ne : 1) * sizeof *lo);
    for (int i = 0; i < ne; i++) {
        int before = g->n;
        IR_t * ei = sno_pat_node(cx, elems[i], S, S);
        int _rb = before;
        while (_rb < g->n && g->all[_rb] && g->all[_rb]->op == IR_GOTO && g->all[_rb]->γ.node == S && g->all[_rb]->ω.node == S && g->all[_rb]->n_operands == 0) _rb++;
        IR_t * ri = (_rb < g->n) ? g->all[_rb] : ei;
        IR_t * ti = NULL;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == S) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S) { if (x->op == IR_GOTO && x->ω.node == S) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); ti = x; } x->γ.sz[3] = 0; }
        }
        if (sno_seq_tail() && ti) ri = ti;
        ent[i] = ei;
        res[i] = ri;
        lo[i] = before;
    }
    for (int i = 0; i < ne; i++) {
        IR_t * nxt = (i + 1 < ne) ? ent[i + 1] : succ;
        IR_t * prv = (i > 0 && (res[i - 1]->op != IR_MATCH_DEFER || (sno_defer_resume() && res[i - 1]->seal != 1))) ? res[i - 1] : fail;
        int lo_i = lo[i];
        int hi_i = (i + 1 < ne) ? lo[i + 1] : g->n;
        for (int k = (lo_i > 0 ? lo_i : 0); k < g->n && k < hi_i; k++) {
            IR_t * x = g->all[k];
            if (!x || x == S) continue;
            if (x->ω.node == S && x->ω.sz[0] == (char)0xcf && (unsigned char)x->ω.sz[1] == 0x86) { x->ω.node = prv; memcpy(x->ω.sz, "β", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x86) { x->γ.node = prv; memcpy(x->γ.sz, "β", 3); x->γ.sz[3] = 0; }
            if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x83) { x->γ.node = nxt; x->γ.sz[0] = 0; }
        }
    }
    for (int k = 0; k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x || x == S) continue;
        if (x->ω.node == S && x->ω.sz[0] == (char)0xcf && (unsigned char)x->ω.sz[1] == 0x86) { x->ω.node = fail; memcpy(x->ω.sz, "β", 3); x->ω.sz[3] = 0; }
        if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x86) { x->γ.node = fail; memcpy(x->γ.sz, "β", 3); x->γ.sz[3] = 0; }
        if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x83) { x->γ.node = succ; x->γ.sz[0] = 0; }
    }
    S->γ.node = succ;
    S->γ.sz[0] = 0;
    if (out_rtail) *out_rtail = (ne > 0) ? res[ne - 1] : NULL;
    { IR_t * first = (ne > 0) ? ent[0] : succ; ct_drop(lo); ct_drop(res); ct_drop(ent); return first; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_capt_name(const tree_t * tgt) {
    if (!tgt) return NULL;
    if (tgt->t == TT_VAR) return tgt->v.sval;
    if (tgt->t == TT_KEYWORD && tgt->v.sval && tgt->v.sval[0]) { const char * kn = tgt->v.sval; return sno_lead_name("&&", kn[0] == '&' ? kn + 1 : kn); }
    if (tgt->t == TT_INDIRECT && tgt->n > 0 && tgt->c[0] && tgt->c[0]->t == TT_QLIT) return tgt->c[0]->v.sval;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_lead_name(const char * lead, const char * nm) {
    size_t ll = strlen(lead), n = strlen(nm);
    char tmp[ll + n + 1];
    memcpy(tmp, lead, ll);
    memcpy(tmp + ll, nm, n + 1);
    return lp_strdup(tmp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_cursor_target(const tree_t * tgt) {
    const char * vn = sno_capt_name(tgt);
    if (vn) { sno_reg_var(vn); return vn; }
    if (!tgt || tgt->t != TT_DEFER) return NULL;
    const tree_t * di = (tgt->n > 0) ? tgt->c[0] : NULL;
    if (!di) return NULL;
    if (di->t == TT_VAR && di->v.sval && di->v.sval[0]) { sno_reg_var(di->v.sval); return di->v.sval; }
    const char * bn = (di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : ((di->t == TT_INDIRECT && di->n > 0 && di->c[0]) || (di->t == TT_IDX && di->n > 0)) ? sno_expr_collect_nm(di) :
        (di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect_nm(di);
    return sno_lead_name("*", bn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_kw_chase(const char * nm, int op) {
    static const char * stk[24];
    static int top = 0;
    if (op == 1) { if (nm && top < 24) { stk[top++] = nm; return 1; } return 0; }
    if (op == 2) { if (top > 0) top--; return 1; }
    if (op == 3) return top != 0;
    if (!nm) return 0;
    for (int i = 0; i < top; i++) if (!strcmp(stk[i], nm)) return 1;
    return 0;
}
static int sno_kw_nest_ok(const char * nm) {
    static int _nn = -1;
    if (_nn < 0) { const char * e = getenv("SCRIP_CONST_NEST"); _nn = (e && *e == '0') ? 0 : 1; }
    return _nn ? !sno_kw_chase(nm, 0) : !sno_kw_chase((const char *)0, 3);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_inline_ok(const tree_t * t) {
    if (!t) return 1;
    switch (t->t) {
        case TT_QLIT:
        return 1;
        case TT_REM:
        return 1;
        case TT_VAR:
        return t->v.sval != NULL;
        case TT_KEYWORD:
        return t->v.sval != NULL;
        case TT_DEFER:
        return t->n > 0 && t->c[0] != NULL;
        case TT_ANY:
        case TT_NOTANY:
        case TT_SPAN:
        case TT_BREAK:
        case TT_BREAKX:
        return (t->n > 0) && sno_cset_fold(t->c[0]) != NULL;
        case TT_LEN:
        case TT_TAB:
        case TT_RTAB:
        case TT_POS:
        case TT_RPOS:
        return (t->n > 0) && t->c[0] && t->c[0]->t == TT_ILIT;
        case TT_SEQ:
        case TT_CAT:
        case TT_ALT:
        { for (int i = 0; i < t->n; i++) if (!sno_pat_inline_ok(t->c[i])) return 0; return 1; }
        case TT_ARBNO:
        { static int _ia = -1; if (_ia < 0) { const char * e = getenv("SCRIP_PAT_INLINE_ARBNO"); _ia = (!e || *e != '0') ? 1 : 0; } return _ia && t->n > 0 && sno_pat_inline_ok(t->c[0]); }
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_capt_body(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t ** out_itail) {
    IR_graph_t * g = cx->g;
    const tree_t * eff = t;
    if (sno_pat_eff_kind(eff) == TT_SEQ) {
        sno_tvec_t ev = {0};
        sno_seq_flatten_pat(eff, &ev);
        const tree_t ** elems = ev.v;
        int ne = ev.n;
        int nf = ne;
        for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { nf = i; break; }
        if (nf == ne && ne > 1) { IR_t * rt = NULL; IR_t * pe = sno_seq_nary(cx, elems, ne, succ, fail, &rt); ct_drop(ev.v); *out_itail = rt ? rt : pe; return pe; }
        ct_drop(ev.v);
    }
    int before = g->n;
    IR_t * pe = sno_pat_node(cx, t, succ, fail);
    IR_t * raw = (before < g->n) ? g->all[before] : pe;
    *out_itail = (raw && raw->op == IR_GOTO && raw->n_operands == 0 && raw->γ.node == succ && raw->ω.node == fail) ? pe : raw;
    return pe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail) {
    IR_graph_t * g = cx->g;
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
                static int _cn = -1;
                if (_cn < 0) { const char * e = getenv("SCRIP_CONST"); _cn = (e && *e == '0') ? 0 : 1; }
                if (_cn) {
                    char cb[fmt_len("&%s", in->v.sval[0] == '&' ? in->v.sval + 1 : in->v.sval)];
                    snprintf(cb, sizeof cb, "&%s", in->v.sval[0] == '&' ? in->v.sval + 1 : in->v.sval);
                    {
                        static int _ci = -1;
                        if (_ci < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); _ci = (e && *e == '0') ? 0 : 1; }
                        if (_ci) {
                            const tree_t * cp0 = sno_const_pat(cb);
                            if (cp0 && g_sno_pat_match_ctx && !g_sno_in_patproc && sno_kw_nest_ok(cb) && sno_pat_inline_ok(cp0)) {
                                char * ky0 = lp_strdup(cb);
                                if (sno_kw_chase(ky0, 1)) { IR_t * r0 = sno_pat_node(cx, cp0, succ, fail); sno_kw_chase(NULL, 2); return r0; }
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
                static int _ck = -1;
                if (_ck < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); _ck = (e && *e == '0') ? 0 : 1; }
                if (_ck) {
                    const tree_t * cp = sno_const_pat(cb);
                    if (cp && g_sno_pat_match_ctx && !g_sno_in_patproc && sno_kw_nest_ok(cb) && sno_pat_inline_ok(cp)) {
                        char * ky = lp_strdup(cb);
                        if (sno_kw_chase(ky, 1)) { IR_t * r = sno_pat_node(cx, cp, succ, fail); sno_kw_chase(NULL, 2); return r; }
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
                cx->npre++;
            }
            return mv;
        }
        case TT_REM:
        { IR_t * nd = lc_build(g, IR_MATCH_REM, succ, NULL); sno_ω_to(nd, fail); return nd; }
        case TT_CAPT_CURSOR:
        {
            const char * cvn = sno_cursor_target((t->n > 0) ? t->c[0] : NULL);
            if (!cvn) sno_fatal("@ cursor-position capture target is not a simple variable", NULL);
            IR_t * nd = lc_build(g, IR_MATCH_ATP, succ, NULL);
            IR_LIT(nd).sval = (char *) cvn;
            sno_ω_to(nd, fail);
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
            IR_t * prev_seal = cx->pat_seal;
            cx->pat_seal = R;
            sno_in_arbno++;
            cx->seq_rlast = NULL;
            cx->seq_rlast_for = NULL;
            IR_t * ei = sno_pat_node(cx, t->c[0], R, R);
            IR_t * rl = (cx->seq_rlast && cx->seq_rlast_for == ei) ? cx->seq_rlast : NULL;
            IR_t * rtl = (rl && cx->seq_rtail) ? cx->seq_rtail : NULL;
            sno_in_arbno--;
            cx->pat_seal = prev_seal;
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
            { sno_tvec_t bv = {0}; sno_seq_flatten_pat(t->c[0], &bv); if (bv.n > 0 && sno_is_fence0(bv.v[bv.n - 1])) ir_operand_push(R, R); ct_drop(bv.v); }
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
            const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
            if (vn) sno_reg_var(vn);
            if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) {
                const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL;
                if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }
                if (!vn) {
                    const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval :
                        ((di && di->t == TT_INDIRECT && di->n > 0 && di->c[0]) || (di && di->t == TT_IDX && di->n > 0)) ? sno_expr_collect_nm(di) : (di && di->t == TT_FNC && di->n > 0) ?
                        sno_expr_collect_wn(di) : sno_expr_collect(di);
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
            IR_t * pe = sno_capt_body(cx, t->c[0], nd, save, &itail);
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
            const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
            if (vn) sno_reg_var(vn);
            if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) {
                const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL;
                if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }
                if (!vn) {
                    const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval :
                        ((di && di->t == TT_INDIRECT && di->n > 0 && di->c[0]) || (di && di->t == TT_IDX && di->n > 0)) ? sno_expr_collect_nm(di) : (di && di->t == TT_FNC && di->n > 0) ?
                        sno_expr_collect_wn(di) : sno_expr_collect(di);
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
            IR_t * pe = sno_capt_body(cx, t->c[0], nd, save, &itail);
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
            if (first_fence == ne) { IR_t * r0 = ne == 1 ? sno_pat_node(cx, elems[0], succ, fail) : sno_seq_nary(cx, elems, ne, succ, fail, NULL); ct_drop(ev.v); return r0; }
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
                    if (inner && sno_in_arbno) {
                        IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                        int f_idx = g->n;
                        IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                        sno_ω_to(F, fail_p);
                        IR_LIT(F).ival = SNO_FENCE_LIT_ARG_IN_ARBNO;
                        int before_p = g->n;
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
                    } else if (inner && !sno_in_arbno) {
                        IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                        int f_idx = g->n;
                        IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                        sno_ω_to(F, fail_p);
                        IR_LIT(F).ival = SNO_FENCE_LIT_ARG;
                        int before_p = g->n;
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
                IR_t * re = (rn == 1) ? sno_pat_node(cx, elems[j], cur_succ, fail_r) : sno_seq_nary(cx, elems + j, rn, cur_succ, fail_r, &n_rt);
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
            if (na == 1) { const tree_t * only = alts[0]; ct_drop(alts); return sno_pat_node(cx, only, succ, fail); }
            IR_t * A = lc_build(g, IR_MATCH_ALTERNATE, succ, NULL);
            sno_ω_to(A, fail);
            (void)0;
            int fc_fp[na];
            int fc_ab[na];
            int fc_ae[na];
            int fc_linear = (na <= 10);
            for (int i = 0; i < na; i++) {
                int before = g->n;
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
                static int _ec = -1;
                if (_ec < 0) { const char * e = getenv("SCRIP_PAT_EAGER_CALL"); _ec = (!e || *e != '0') ? 1 : 0; }
                if (_ec && cx->npre >= 0 && cx->npre < 64) {
                    IR_t * mvd = lc_build(g, IR_MATCH_DEFER, succ, NULL);
                    sno_defer_seal(cx, mvd);
                    sno_ω_to(mvd, fail);
                    cx->pre[cx->npre].arg = t;
                    cx->pre[cx->npre].prim = mvd;
                    cx->pre[cx->npre].str = 0;
                    cx->pre[cx->npre].codes = 0;
                    cx->pre[cx->npre].snapg = name ? name : "$fnc";
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_supported(const tree_t * t) {
    if (!t) return 0;
    const tree_e k = sno_pat_eff_kind(t);
    if (k == TT_FLUSH) return 1;
    if (k == TT_FENCE) return t->n == 0 || sno_pat_supported(t->c[0]);
    if (k == TT_QLIT) return 1;
    if (k == TT_ANY || k == TT_NOTANY) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_SPAN) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_BREAK || k == TT_BREAKX) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_TAB || k == TT_RTAB) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_POS || k == TT_RPOS) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_REM || k == TT_ARB) return 1;
    if (k == TT_ABORT || k == TT_FAIL) return 1;
    if (k == TT_BAL) return 1;
    if (k == TT_SUCCEED) return 0;
    if (k == TT_ARBNO) return t->n > 0 && t->c[0] && sno_pat_supported(t->c[0]);
    if (k == TT_VAR) return t->v.sval != NULL;
    if (k == TT_KEYWORD) return t->v.sval != NULL;
    if (k == TT_DEFER) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_LEN) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_CAPT_COND_ASGN) return t->n > 1 && t->c[1] && (sno_capt_name(t->c[1]) != NULL || t->c[1]->t == TT_DEFER) && sno_pat_supported(t->c[0]);
    if (k == TT_CAPT_IMMED_ASGN) return t->n > 1 && t->c[1] && (sno_capt_name(t->c[1]) != NULL || t->c[1]->t == TT_DEFER) && sno_pat_supported(t->c[0]);
    if (k == TT_CAPT_CURSOR) return t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval;
    if (k == TT_SEQ) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (k == TT_ALT) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (k == TT_FNC) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_pattern_rhs(const tree_t * t) {
    if (!t) return 0;
    switch (sno_pat_eff_kind(t)) {
        case TT_ABORT:
        case TT_SUCCEED:
        case TT_ALT:
        case TT_FENCE:
        case TT_FLUSH:
        case TT_ARBNO:
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
        case TT_ARB:
        case TT_REM:
        case TT_BAL:
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        return 1;
        case TT_SEQ:
        case TT_CAT:
        {
            const tree_t * a = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * b = (t->n > 1) ? t->c[1] : NULL;
            if ((a && a->t == TT_DEFER) || (b && b->t == TT_DEFER)) return 1;
            return sno_is_pattern_rhs(a) || sno_is_pattern_rhs(b);
        }
        case TT_KEYWORD:
        {
            static int kdepth = 0;
            if (!sno_const_static_on() || kdepth >= 32 || !t->v.sval) return 0;
            char cb[130];
            snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
            const tree_t * p = sno_const_pat(cb);
            if (!p) return 0;
            kdepth++;
            int r = sno_is_pattern_rhs(p);
            kdepth--;
            return r;
        }
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_null_lit(const tree_t * t) { return t && ((t->t == TT_QLIT && (!t->v.sval || !t->v.sval[0])) || t->t == TT_NUL); }
static int sno_mkpat_here(const tree_t * t) {
    static int _on = -1;
    if (_on < 0) { const char * e = getenv("SCRIP_MKPAT_ANYWHERE"); _on = (e && *e == '0') ? 0 : 1; }
    if (!_on || !t || t->t == TT_VAR || t->t == TT_KEYWORD || t->t == TT_INDIRECT || t->t == TT_DEFER || !sno_is_pattern_rhs(t) || !sno_pat_supported(t)) return 0;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n > 1 && (sno_null_lit(t->c[0]) || sno_null_lit(t->c[1]))) return 0;
    IR_graph_t * tg = IR_alloc(256);
    scx_t tx;
    tx.g = tg;
    tx.loop_exit = NULL;
    tx.loop_next = NULL;
    tx.result_name = NULL;
    tx.pat_fail = NULL;
    tx.pat_seal = NULL;
    tx.npre = 0;
    tx.prog_nstmt = 0;
    tx.stno_base = 0;
    IR_t * tok = lc_build(tg, IR_SUCCEED, NULL, NULL);
    IR_t * tno = lc_build(tg, IR_FAIL, NULL, NULL);
    tx.pat_fail = tno;
    tx.pat_seal = sno_thunk_abort_exit(tg, tno);
    sno_pat_node(&tx, t, tok, tno);
    return tx.npre == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_right_sealed(const tree_t * t) {
    if (!t) return 0;
    if (sno_is_fence(t)) return 1;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n > 1) return sno_is_fence1(t->c[1]) ? sno_pat_right_sealed(t->c[0]) : sno_pat_right_sealed(t->c[1]);
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0) return sno_pat_right_sealed(t->c[0]);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_patsalt_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATSALT"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_patname_salt_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATNAME_SALT"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_pat_collect(const tree_t * pat) {
    for (int i = 0; i < (int) g_sno_pats.len; i++) if ((!sno_patsalt_on() || CV_AT(g_sno_pats, sno_pat_ent_t, i).salt == g_sno_expr_salt) && sno_expr_eq(CV_AT(g_sno_pats, sno_pat_ent_t, i).pat, pat))
        return CV_AT(g_sno_pats, sno_pat_ent_t, i).name;
    char buf[32];
    if (sno_patname_salt_on() && g_sno_expr_salt) snprintf(buf, sizeof buf, "PAT$%dF%d", (int) g_sno_pats.len, g_sno_expr_salt);
    else snprintf(buf, sizeof buf, "PAT$%d", (int) g_sno_pats.len);
    { sno_pat_ent_t x; x.name = lp_strdup(buf); x.pat = pat; x.salt = g_sno_expr_salt; CV_PUSH(g_sno_pats, sno_pat_ent_t) = x; return x.name; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_mkpat_emit(scx_t * cx, const tree_t * pat, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    const char * bn = sno_pat_collect(pat);
    IR_t * mk = lc_build(g, IR_CALL, γ, ω);
    IR_LIT(mk).sval = (char *) "SNO$MKPAT";
    IR_t * nl = lc_build(g, IR_LIT_STRING, NULL, ω);
    IR_LIT(nl).sval = (char *) bn;
    nl->seal = IR_SEAL_THUNK_REF;
    ir_operand_push(mk, nl);
    IR_graph_t * tg = IR_alloc(256);
    scx_t tx;
    tx.g = tg;
    tx.loop_exit = NULL;
    tx.loop_next = NULL;
    tx.result_name = NULL;
    tx.pat_fail = NULL;
    tx.pat_seal = NULL;
    tx.npre = 0;
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
        if (tx.pre[api].str && tx.pre[api].codes) { IR_t * co = lc_build(g, IR_COERCE_STRING, NULL, ω); IR_LIT(co).ival = tx.pre[api].codes; lc_γ_to(av, co); ir_operand_push(co, av); last = co; }
        ir_operand_push(mk, last);
    }
    lc_γ_to(last, mk);
    if (res) *res = mk;
    return nl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land) {
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
    { int _mcsv = g_sno_pat_match_ctx; g_sno_pat_match_ctx = 1; pat_entry = sno_pat_node(cx, ptt, release, head); g_sno_pat_match_ctx = _mcsv; }
    lc_γ_to(head, pat_entry);
    {
        int fp_stmt = 0;
        int fc_lin = (sno_in_arbno == 0) && fc_walk_range(g, before_pat, g->n, 1, &fp_stmt);
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
                        for (uint32_t d = 0; d < scd.len; d++) CV_AT(scd, sno_scd_t, d).promo = 0;
                        {
                            extern int fc_alt_fpmax(const IR_t *);
                            extern int fc_alt_extent(const IR_t *, int *, int *);
                            extern int fc_cond_fp(const IR_t *);
                            for (int d = 0; d < (int) scd.len && !cap_bad; d++) {
                                if (CV_AT(scd, sno_scd_t, d).save_idx < i_b0 || CV_AT(scd, sno_scd_t, d).nd_idx >= g->n) { cap_bad = 1; break; }
                                int in_arm = 0;
                                for (int k = i_b0; k < g->n; k++) {
                                    IR_t * x = g->all[k];
                                    if (!x || x->op != IR_MATCH_ALTERNATE || fc_alt_fpmax(x) < 0) continue;
                                    int _b = 0, _e = 0;
                                    if (fc_alt_extent(x, &_b, &_e) && CV_AT(scd, sno_scd_t, d).nd_idx >= _b && CV_AT(scd, sno_scd_t, d).nd_idx < _e) { in_arm = 1; break; }
                                }
                                int nested = 0;
                                for (int e = 0; e < (int) scd.len; e++) {
                                    if (e == d) continue;
                                    if (CV_AT(scd, sno_scd_t, e).nd_idx > CV_AT(scd, sno_scd_t, d).save_idx && CV_AT(scd, sno_scd_t, e).nd_idx < CV_AT(scd, sno_scd_t, d).i_end) { nested = 1; break; }
                                }
                                if (in_arm || nested) cap_bad = 1;
                                else CV_AT(scd, sno_scd_t, d).promo = 1;
                            }
                            if (!cap_bad) for (int k = i_b0; k < g->n; k++) {
                                IR_t * x = g->all[k];
                                if (!x || (x->op != IR_MATCH_ASSIGN_COND && x->op != IR_MATCH_ASSIGN_IMM)) continue;
                                if (fc_cond_fp(x) >= 0) continue;
                                int def_ok = 0;
                                for (int d = 0; d < (int) scd.len; d++) if (CV_AT(scd, sno_scd_t, d).nd == x && CV_AT(scd, sno_scd_t, d).promo) { def_ok = 1; break; }
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
                                for (int d = 0; d < (int) scd.len && !_ap; d++) if (CV_AT(scd, sno_scd_t, d).promo) _ap = 1;
                                dfr_regs = (n_wrap > 0 || _ap);
                                if (dfr_regs) tl_why = "defer-caps";
                            }
                        }
                        if (!cap_left && !cap_bad && !dfr_regs && fc_tail_walk(g, before_pat, i_arb) && fc_tail_walk(g, i_b0, i_b1 + 1) && fc_tail_walk(g, i_b1 + 1, g->n)) {
                            {
                                extern void fc_save_register(const IR_t *);
                                extern void fc_cond_register(const IR_t *, int);
                                for (int d = 0; d < (int) scd.len; d++) if (CV_AT(scd, sno_scd_t, d).promo) {
                                    fc_save_register(CV_AT(scd, sno_scd_t, d).save);
                                    fc_cond_register(CV_AT(scd, sno_scd_t, d).nd, CV_AT(scd, sno_scd_t, d).fp_inner);
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
    static int _preord = -1;
    if (_preord < 0) { const char * e = getenv("SCRIP_PRE_ORDER"); _preord = (e && *e == '0') ? 0 : 1; }
    for (int pi = _preord ? cx->npre - 1 : 0; _preord ? (pi >= 0) : (pi < cx->npre); pi += _preord ? -1 : 1) {
        if (cx->pre[pi].snapg) {
            static int g_snapctr = 0;
            char nb[32];
            snprintf(nb, sizeof nb, "PATV$%d", g_snapctr++);
            char * gname = lp_strdup(nb);
            sno_reg_var(gname);
            IR_LIT(cx->pre[pi].prim).sval = gname;
            cx->pre[pi].prim->pat_static = 1;
            IR_t * asnV = lc_build(g, IR_ASSIGN, after, fJ);
            IR_LIT(asnV).sval = gname;
            IR_t * av = NULL;
            IR_t * ae = sx_lower(cx, cx->pre[pi].arg, asnV, fJ, &av);
            ir_operand_push(asnV, av);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_build_call_stub(const char * entry_label, const char * fname) {
    IR_graph_t * g = IR_alloc(64);
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t * sh4 = fname ? lc_build(g, IR_DEFINE, exitnd, failnd) : (IR_t *)0;
    if (sh4) {
        IR_LIT(sh4).ival = 4;
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
    IR_t * sh4 = lc_build(g, IR_DEFINE, exitnd, failnd);
    IR_LIT(sh4).ival = 4;
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
static const tree_t * g_sno_prescan_top = NULL;
static int g_sno_expr_define_seen = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_exprdef_note(cv_t * v, const char * f) { if (!f) return; for (uint32_t k = 0; k < v->len; k++) if (!strcmp(CV_AT(*v, const char *, k), f)) return; CV_PUSH(*v, const char *) = f; }
static int sno_exprdef_seen(const cv_t * v, const char * f) { if (!f) return 0; for (uint32_t k = 0; k < v->len; k++) if (!strcmp(CV_AT(*v, const char *, k), f)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name, long stno_base, long end_line) {
    IR_graph_t * g = IR_alloc(nst * 16 + 256);
    scx_t cx;
    cx.g = g;
    cx.loop_exit = NULL;
    cx.loop_next = NULL;
    cx.result_name = result_name;
    cx.pat_fail = NULL;
    cx.pat_seal = NULL;
    cx.npre = 0;
    cx.prog_nstmt = (long)nst + 1 + stno_base;
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
    if (g_sno_uses_stmtkw && end_line > 0) {
        IR_t * ehook = lc_build(g, IR_CALL, exitnd, exitnd);
        IR_LIT(ehook).sval = (char *) "SNO$STMT";
        IR_t * enum_ = lc_build(g, IR_LIT_INTEGER, ehook, ehook);
        IR_LIT(enum_).ival = (int64_t)(nst + 1) + stno_base;
        IR_t * elnn = lc_build(g, IR_LIT_INTEGER, ehook, ehook);
        IR_LIT(elnn).ival = (int64_t)end_line;
        lc_γ_to(enum_, elnn);
        ir_operand_push(ehook, enum_);
        ir_operand_push(ehook, elnn);
        endnd = enum_;
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
        IR_t * rf = lc_build(g, IR_DEFINE, exitnd, failnd);
        IR_LIT(rf).ival = 1;
        IR_t * ff = lc_build(g, IR_DEFINE, exitnd, failnd);
        IR_LIT(ff).ival = 2;
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
        IR_t * sT = sno_goto_branch(g, &cx, goS, dcS, exS, exitnd);
        const char * sTnm = goS ? goS : (IR_t *)0 == sT ? (const char *)0 : goS;
        if (!sT) { sT = sno_goto_branch(g, &cx, goU, dcU, exU, exitnd); sTnm = goU; }
        if (!sT) { sT = next; sTnm = (const char *)0; }
        IR_t * fT = sno_goto_branch(g, &cx, goF, dcF, exF, exitnd);
        const char * fTnm = goF;
        if (!fT) { fT = sno_goto_branch(g, &cx, goU, dcU, exU, exitnd); fTnm = goU; }
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
                    IR_t * bind = lc_build(g, IR_DEFINE, sJ, fA);
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
            const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
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
                    lc_γ_to(asn, e2);
                    lc_γ_to(anchor[i], ec);
                    continue;
                }
                sno_fatal("pattern match with no pattern operand", NULL);
            }
            IR_t * e = sno_lower_match(&cx, subj, has_eq ? sfind_expr(s, ":repl") : NULL, has_eq, sJ, fJ, &match_land[i]);
            lc_γ_to(anchor[i], e);
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
                const tree_t * pat = sno_pat_valued(&cx, &sbj, 0, repl->c[1], fA, &pent, &pasn);
                tree_t * capt = ast_stmt_new(TT_CAPT_COND_ASGN);
                ast_push(capt, (tree_t *) pat);
                tree_t * tv = ast_node_new(TT_VAR);
                tv->v.sval = subj->v.sval;
                ast_push(capt, tv);
                tree_t * sc = ast_stmt_new(TT_SCAN);
                ast_push(sc, (tree_t *) sbj);
                ast_push(sc, capt);
                IR_t * e = sno_lower_match(&cx, sc, NULL, 0, sJ, fA, NULL);
                if (pasn) { lc_γ_to(pasn, e); e = pent; }
                lc_γ_to(anchor[i], e);
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
            IR_t * e1 = sx_nameval(&cx, subj->c[0], NULL, fA, &nv);
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
            IR_t * wl = lc_build(g, IR_LIT_STRING, NULL, fA);
            IR_LIT(wl).sval = (char *) "";
            IR_t * mk = lc_build(g, IR_CALL, sJ, fA);
            IR_LIT(mk).sval = (char *) "SNO$WANTNM";
            lc_γ_to(wl, mk);
            ir_operand_push(mk, wl);
            IR_t * cv = NULL;
            IR_t * e1 = sx_lower(&cx, subj, NULL, fA, &cv);
            lc_γ_to(mk, e1);
            IR_t * vv = NULL;
            IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(cv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, cv);
            ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], wl);
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
                        const char * _sf = sfind_str(st[i], ":file");
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
    { const char * _sk = getenv("SCRIP_SNO_STMTKW"); if (_sk && *_sk == '1') { g_sno_uses_stmtkw = 1; g_sno_traces_a_label = 1; } }
    if (g_sno_uses_stmtkw) {
        const char * _stmtkw_cur_file = (const char *) 0;
        for (int i = 0; i < nst; i++) {
            if (lp_s_int(st[i], ":nocount") || sno_stmt_is_blank(st[i])) continue;
            IR_t * body = anchor[i]->γ.node;
            IR_t * hook = lc_build(g, IR_CALL, body, body);
            IR_LIT(hook).sval = (char *) "SNO$STMT";
            IR_t * num = lc_build(g, IR_LIT_INTEGER, hook, hook);
            IR_LIT(num).ival = (int64_t)(i + 1) + stno_base;
            int _lln = lp_s_int(st[i], ":lline");
            if (!_lln) _lln = lp_s_int(st[i], ":line");
            IR_t * lnn = lc_build(g, IR_LIT_INTEGER, hook, hook);
            IR_LIT(lnn).ival = (int64_t)_lln;
            lc_γ_to(num, lnn);
            ir_operand_push(hook, num);
            ir_operand_push(hook, lnn);
            {
                extern const char * stmt_src_get_file(void);
                const char * _sf = sfind_str(st[i], ":file");
                if (!_sf || !*_sf) _sf = stmt_src_get_file();
                if (!_sf) _sf = "";
                if (!_stmtkw_cur_file || strcmp(_stmtkw_cur_file, _sf) != 0) {
                    _stmtkw_cur_file = _sf;
                    IR_t * fpn = lc_build(g, IR_LIT_STRING, hook, hook);
                    IR_LIT(fpn).sval = (char *) _sf;
                    lc_γ_to(lnn, fpn);
                    ir_operand_push(hook, fpn);
                }
            }
            lc_γ_to(anchor[i], num);
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
static void sno_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        const tree_t * subj = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!subj) continue;
        const char * lbl = stmt_attr_str(stmt_attr_find(s, ":lbl"));
        if (mod_idx >= 0 && lbl && *lbl) s2->module_registry.mods[mod_idx].core_label_count++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_litname(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT && a->v.sval) return a->v.sval;
    if (a->t == TT_NAME && a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval) return a->c[0]->v.sval;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_def_put(cv_t * defs, cv_t * bodies, const sno_def_t * d, const tree_t * body) { sno_def_t c = *d; CV_PUSH(*defs, sno_def_t) = c; CV_PUSH(*bodies, const tree_t *) = body; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prescan_expr(const tree_t * t, cv_t * defs, cv_t * bodies, cv_t * exprdef_names, cv_t * entries) {
    if (!t) return;
    if (t->t == TT_FNC) {
        const char * name = t->v.sval;
        int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        if (name && !strcmp(name, "DATA") && t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
            extern void * dat_register(const char * spec);
            extern void * dat_find_type(const char * name);
            extern void dat_set_live(const char * name, int live);
            extern int dat_spec_is_current(const char * spec);
            const char * sp = t->c[argbase]->v.sval;
            char nb[strlen(sp) + 1];
            int k = 0;
            for (; sp[k] && sp[k] != '('; k++) nb[k] = sp[k];
            nb[k] = 0;
            int prot = (nb[0] && sn4_sysfn_protected(nb)) ? 1 : 0;
            if (!prot && sp[k] == '(') {
                const char * fs = sp + k + 1;
                const char * fe = strchr(fs, ')');
                if (!fe) fe = fs + strlen(fs);
                while (fs < fe && !prot) {
                    const char * cm = fs;
                    while (cm < fe && *cm != ',') cm++;
                    const char * b = fs;
                    while (b < cm && (*b == ' ' || *b == '\t')) b++;
                    const char * e2 = cm;
                    while (e2 > b && (e2[-1] == ' ' || e2[-1] == '\t')) e2--;
                    size_t fl = (size_t)(e2 - b);
                    if (fl > 0) { char fb[fl + 1]; memcpy(fb, b, fl); fb[fl] = 0; if (sn4_sysfn_protected(fb)) prot = 1; }
                    fs = (cm < fe) ? cm + 1 : fe;
                }
            }
            if (nb[0] && !prot && !dat_spec_is_current(sp)) { dat_register(sp); dat_set_live(nb, 0); }
        }
        if (name && !strcmp(name, "OPSYN") && t->n - argbase == 2) {
            const char * an = sno_litname(t->c[argbase]);
            const char * on = sno_litname(t->c[argbase + 1]);
            if (an && on) {
                int fo = -1, fa = -1;
                for (uint32_t k = 0; k < defs->len; k++) if (!strcmp(CV_AT(*defs, sno_def_t, k).fname, on)) { fo = (int) k; break; }
                for (uint32_t k = 0; k < defs->len; k++) if (!strcmp(CV_AT(*defs, sno_def_t, k).fname, an)) { fa = (int) k; break; }
                if (fa >= 0 && !CV_AT(*defs, sno_def_t, fa).result_name) fo = -2;
                if (fo >= 0) {
                    sno_def_t d = CV_AT(*defs, sno_def_t, fo);
                    d.result_name = d.result_name ? d.result_name : d.fname;
                    d.fname = lp_strdup(an);
                    sno_reg_var(d.fname);
                    sno_def_put(defs, bodies, &d, NULL);
                } else if (fo == -1) {
                    extern void rt_builtin_synonym_add(const char *, const char *);
                    rt_builtin_synonym_add(lp_strdup(an), lp_strdup(on));
                }
            }
        }
        if (name && !strcmp(name, "DEFINE") && t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval && !sno_define_entry_computed(t, argbase)) {
            const char * entry_opt = NULL;
            if (t->n > argbase + 1 && t->c[argbase + 1]) {
                const tree_t * ea = t->c[argbase + 1];
                if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
                else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
            }
            sno_def_t d;
            sno_parse_define(t->c[argbase]->v.sval, entry_opt, &d);
            if (sn4_sysfn_protected(d.fname)) return;
            sno_proto_note(&d);
            sno_entry_seen_push(entries, d.entry);
            if (t != g_sno_prescan_top) { g_sno_expr_define_seen = 1; sno_exprdef_note(exprdef_names, d.fname); }
            sno_predef_note(d.fname);
            int fo = -1;
            for (uint32_t k = 0; k < defs->len; k++) if (!strcmp(CV_AT(*defs, sno_def_t, k).fname, d.fname)) { fo = (int) k; break; }
            if (fo >= 0) CV_AT(*defs, sno_def_t, fo) = d;
            else sno_def_put(defs, bodies, &d, NULL);
        }
    }
    for (int i = 0; i < t->n; i++) sno_prescan_expr(t->c[i], defs, bodies, exprdef_names, entries);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prescan_repl_defines(const tree_t * t, cv_t * defs, cv_t * bodies, cv_t * exprdef_names, cv_t * entries) {
    if (!t) return;
    if (t->t == TT_FNC) {
        const char * name = t->v.sval;
        int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        (void) argbase;
        if (name && !strcmp(name, "DEFINE")) {
            const tree_t * top = g_sno_prescan_top;
            g_sno_prescan_top = NULL;
            sno_prescan_expr(t, defs, bodies, exprdef_names, entries);
            g_sno_prescan_top = top;
            return;
        }
    }
    for (int i = 0; i < t->n; i++) sno_prescan_repl_defines(t->c[i], defs, bodies, exprdef_names, entries);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_expr_mark(void) { return (int) g_sno_exprs.len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_expr_thunks_build(int x0) {
    int sv = g_sno_in_patproc;
    g_sno_in_patproc = 1;
    for (int xi = x0; xi < (int) g_sno_exprs.len; xi++) {
        {
            static int _xd = -1;
            if (_xd < 0) _xd = getenv("SCRIP_EXPR_DBG") ? 1 : 0;
            if (_xd)
                fprintf(stderr, "[EXPRDBG] %s want_name=%d expr.t=%d expr.sval=%.32s\n", CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name, CV_AT(g_sno_exprs, sno_expr_ent_t, xi).want_name,
                CV_AT(g_sno_exprs, sno_expr_ent_t, xi).expr ? (int)CV_AT(g_sno_exprs, sno_expr_ent_t, xi).expr->t : -1,
                (CV_AT(g_sno_exprs, sno_expr_ent_t, xi).expr && CV_AT(g_sno_exprs, sno_expr_ent_t, xi).expr->v.sval) ? CV_AT(g_sno_exprs, sno_expr_ent_t, xi).expr->v.sval : "?");
        }
        IR_graph_t * gx = IR_alloc(256);
        scx_t ex;
        ex.g = gx;
        ex.loop_exit = NULL;
        ex.loop_next = NULL;
        ex.result_name = CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name;
        ex.pat_fail = NULL;
        ex.pat_seal = NULL;
        ex.npre = 0;
        ex.prog_nstmt = 0;
        ex.stno_base = 0;
        IR_t * ok = lc_build(gx, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gx, IR_FAIL, NULL, NULL);
        IR_t * sJ = lc_build(gx, IR_GOTO, ok, NULL);
        IR_t * fJ = lc_build(gx, IR_GOTO, no, NULL);
        sno_reg_var(CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name);
        IR_t * asn = lc_build(gx, IR_ASSIGN, sJ, fJ);
        IR_LIT(asn).sval = (char *) CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name;
        const tree_t * xe = CV_AT(g_sno_exprs, sno_expr_ent_t, xi).expr;
        if (CV_AT(g_sno_exprs, sno_expr_ent_t, xi).want_name && xe && xe->t == TT_INDIRECT && xe->n > 0 && xe->c[0] && CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name &&
            !strncmp(CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name, "EXPRNM$", 7)) xe = xe->c[0];
        IR_t * vr = NULL;
        int xe_is_idx_wn = CV_AT(g_sno_exprs, sno_expr_ent_t, xi).want_name && xe && xe->t == TT_IDX;
        IR_t * e = xe_is_idx_wn ? sx_idx_container(&ex, xe, fJ, &vr) : sx_lower(&ex, xe, asn, fJ, &vr);
        if (xe_is_idx_wn) lc_γ_to(vr, asn);
        ir_operand_push(asn, vr);
        if (CV_AT(g_sno_exprs, sno_expr_ent_t, xi).want_name) {
            IR_t * wn_lit = lc_build(gx, IR_LIT_STRING, NULL, fJ);
            IR_LIT(wn_lit).sval = (char *) "";
            IR_t * wn_call = lc_build(gx, IR_CALL, NULL, fJ);
            IR_LIT(wn_call).sval = (char *) "SNO$WANTNM";
            lc_γ_to(wn_lit, wn_call);
            lc_γ_to(wn_call, e);
            ir_operand_push(wn_call, wn_lit);
            gx->entry = wn_lit;
        } else {
            gx->entry = e;
        }
        { IR_t * ad = lc_build(gx, IR_DEFINE, gx->entry, fJ); IR_LIT(ad).ival = 3; gx->entry = ad; }
        int xpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[xpi].name = CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name;
        g_stage2.proc_table[xpi].proc = NULL;
        g_stage2.proc_table[xpi].entry_pc = -1;
        g_stage2.proc_table[xpi].nparams = 0;
        g_stage2.proc_table[xpi].lower_sc.n = 0;
        g_stage2.proc_table[xpi].is_generator = 0;
        g_stage2.proc_table[xpi].dyn_scope = 1;
        g_stage2.proc_table[xpi].thunk_kind = PROC_THUNK_EXPR;
        g_stage2.proc_table[xpi].result_name = CV_AT(g_sno_exprs, sno_expr_ent_t, xi).name;
        g_stage2.proc_table[xpi].bb_idx = bb_program_add(&g_stage2.bbp, gx);
    }
    g_sno_in_patproc = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pat_count(void) { return (int) g_sno_pats.len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_pat_carrier_build(scx_t * px, const tree_t * pat, IR_t * ok, IR_t * no, IR_t ** out_brt, int * out_pfenced) {
    IR_t * pe;
    *out_brt = NULL;
    int pfenced = sno_pat_contains_fence(pat, 0);
    if (getenv("SCRIP_FENCE_IGNORE")) pfenced = 0;
    {
        sno_tvec_t fv = {0};
        sno_seq_flatten_pat(pat, &fv);
        const tree_t ** fel = fv.v;
        int fne = fv.n;
        int topf = 0;
        for (int i = 0; i < fne; i++) if (sno_is_fence(fel[i])) { topf = 1; break; }
        if (fne > 1 && !topf && !sno_pat_contains_fence0(pat, 0)) pfenced = 0;
        px->seq_rlast = NULL;
        px->seq_rlast_for = NULL;
        px->seq_rtail = NULL;
        pe = (sno_defer_resume() && fne > 1 && (!pfenced || (sno_fence_rtail() && !topf))) ? sno_seq_nary(px, fel, fne, ok, no, out_brt) : sno_pat_node(px, pat, ok, no);
        if (topf && !*out_brt && px->seq_rlast_for == pe && px->seq_rtail) *out_brt = px->seq_rtail;
        ct_drop(fv.v);
    }
    *out_pfenced = pfenced;
    return pe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_right_fence0(const tree_t * t) {
    if (!t) return 0;
    if (sno_is_fence0(t) || sno_is_flush(t)) return 1;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n > 1) return sno_is_fence1(t->c[1]) ? sno_pat_right_fence0(t->c[0]) : sno_pat_right_fence0(t->c[1]);
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0) return sno_pat_right_fence0(t->c[0]);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_pat_publish_body_root(IR_graph_t * gp, int before_pat, const tree_t * pat, IR_t * brt, int pfenced, const char * dbgname, IR_t * abort_nd) {
    IR_t * rn = NULL;
    if (sno_defer_resume() && !pfenced) {
        rn = brt;
        if (!rn) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { rn = x2; break; } }
    } else if (sno_defer_resume() && pfenced) {
        extern int zdp_seam_tier(const IR_t *);
        const char * _fre = getenv("SCRIP_FENCE_RESUME");
        if (!(_fre && *_fre == '0')) {
            IR_t * _c = (sno_fence_rtail() && brt) ? brt : NULL;
            if (!_c) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { _c = x2; break; } }
            { int _t2 = zdp_seam_tier(_c); if (_t2 == 1 || _t2 == 2 || _t2 == 3) rn = _c; }
        }
    }
    int rs = sno_pat_right_sealed(pat) ? 1 : 0;
    gp->body_root = (gp->n > before_pat && !rs) ? ((sno_defer_resume() && pfenced && !rn) ? NULL : (rn ? rn : gp->all[before_pat])) : NULL;
    if (abort_nd && sno_pat_right_fence0(pat)) gp->body_root = abort_nd;
    if (getenv("SCRIP_RESUME_WHY") && !gp->body_root) {
        extern int zdp_seam_tier(const IR_t *);
        const IR_t * _fb = NULL;
        for (int k3 = before_pat; k3 < gp->n; k3++) { IR_t * x3 = gp->all[k3]; if (x3 && !(x3->op == IR_GOTO && x3->n_operands == 0)) { _fb = x3; break; } }
        fprintf(stderr, "[RESUME-NIL] pat=%s empty=%d right_sealed=%d pfenced=%d rn=%d brt=%d fb=%s fbtier=%d chain=%s|%s|%s|%s\n", dbgname ? dbgname : "?", !(gp->n > before_pat), rs, pfenced,
            rn ? 1 : 0, brt ? 1 : 0, _fb ? bb_op_name(_fb->op) : "-", zdp_seam_tier(_fb), (before_pat + 0 < gp->n) ? bb_op_name(gp->all[before_pat + 0]->op) : "-",
            (before_pat + 1 < gp->n) ? bb_op_name(gp->all[before_pat + 1]->op) : "-", (before_pat + 2 < gp->n) ? bb_op_name(gp->all[before_pat + 2]->op) : "-",
            (before_pat + 3 < gp->n) ? bb_op_name(gp->all[before_pat + 3]->op) : "-");
    }
    if (getenv("SCRIP_RESUME_WHY"))
        fprintf(stderr, "[RTGRAPH] pat=%s pfenced=%d brt=%d rn=%d body_root_op=%d\n", dbgname ? dbgname : "?", pfenced, brt ? 1 : 0, rn ? 1 : 0, gp->body_root ? (int) gp->body_root->op : -1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_pat_thunks_build(int p0) {
    int sv = g_sno_in_patproc;
    g_sno_in_patproc = 1;
    for (int pi2 = p0; pi2 < (int) g_sno_pats.len; pi2++) {
        IR_graph_t * gp = IR_alloc(512);
        scx_t px;
        px.g = gp;
        px.loop_exit = NULL;
        px.loop_next = NULL;
        px.result_name = NULL;
        px.pat_fail = NULL;
        px.pat_seal = NULL;
        px.npre = 0;
        px.prog_nstmt = 0;
        px.stno_base = 0;
        IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
        px.pat_fail = no;
        px.pat_seal = sno_thunk_abort_exit(gp, no);
        int before_pat = gp->n;
        IR_t * brt = NULL;
        int pfenced = 0;
        IR_t * pe = sno_pat_carrier_build(&px, CV_AT(g_sno_pats, sno_pat_ent_t, pi2).pat, ok, no, &brt, &pfenced);
        {
            extern tree_t *ast_stmt_new(tree_e kind);
            IR_t * paft = pe;
            for (int api = 0; api < px.npre; api++) {
                if (px.pre[api].snapg) {
                    char vbuf[fmt_len("%s$V%d", CV_AT(g_sno_pats, sno_pat_ent_t, pi2).name, api)];
                    snprintf(vbuf, sizeof vbuf, "%s$V%d", CV_AT(g_sno_pats, sno_pat_ent_t, pi2).name, api);
                    char * vg = lp_strdup(vbuf);
                    sno_reg_var(vg);
                    IR_LIT(px.pre[api].prim).sval = vg;
                    continue;
                }
                IR_t * co = lc_build(gp, px.pre[api].str ? IR_COERCE_STRING : IR_COERCE_INTEGER, paft, no);
                IR_LIT(co).ival = px.pre[api].codes;
                char abuf[fmt_len("%s$V%d", CV_AT(g_sno_pats, sno_pat_ent_t, pi2).name, api)];
                snprintf(abuf, sizeof abuf, "%s$V%d", CV_AT(g_sno_pats, sno_pat_ent_t, pi2).name, api);
                tree_t * tv = ast_stmt_new(TT_VAR);
                tv->v.sval = lp_strdup(abuf);
                IR_t * av = NULL;
                IR_t * ae = sx_lower(&px, tv, co, no, &av);
                ir_operand_push(co, av);
                ir_operand_push(px.pre[api].prim, co);
                paft = ae;
            }
            px.npre = 0;
            pe = paft;
        }
        gp->entry = pe;
        gp->resumable_callable = 1;
        sno_pat_publish_body_root(gp, before_pat, CV_AT(g_sno_pats, sno_pat_ent_t, pi2).pat, brt, pfenced, CV_AT(g_sno_pats, sno_pat_ent_t, pi2).name, px.pat_seal);
        int ppi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[ppi].name = CV_AT(g_sno_pats, sno_pat_ent_t, pi2).name;
        g_stage2.proc_table[ppi].proc = NULL;
        g_stage2.proc_table[ppi].entry_pc = -1;
        g_stage2.proc_table[ppi].nparams = 0;
        g_stage2.proc_table[ppi].lower_sc.n = 0;
        g_stage2.proc_table[ppi].is_generator = 0;
        g_stage2.proc_table[ppi].dyn_scope = 0;
        g_stage2.proc_table[ppi].thunk_kind = PROC_THUNK_PATTERN;
        g_stage2.proc_table[ppi].result_name = NULL;
        g_stage2.proc_table[ppi].bb_idx = bb_program_add(&g_stage2.bbp, gp);
    }
    g_sno_in_patproc = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_register_entry_label(const char * el, int bb_idx) {
    if (!el || !el[0]) return;
    IR_t * anchor = bb_label_landing(el);
    if (!anchor) return;
    char lname[strlen(el) + 6];
    snprintf(lname, sizeof lname, "LBL__%s", el);
    for (int q = 0; q < g_stage2.proc_count; q++) if (g_stage2.proc_table[q].name && !strcmp(g_stage2.proc_table[q].name, lname)) return;
    int lpi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[lpi].name = lp_strdup(lname);
    g_stage2.proc_table[lpi].proc = NULL;
    g_stage2.proc_table[lpi].entry_pc = -1;
    g_stage2.proc_table[lpi].nparams = 0;
    g_stage2.proc_table[lpi].lower_sc.n = 0;
    g_stage2.proc_table[lpi].is_generator = 0;
    g_stage2.proc_table[lpi].dyn_scope = 0;
    g_stage2.proc_table[lpi].result_name = NULL;
    g_stage2.proc_table[lpi].proc_entry_node = anchor;
    g_stage2.proc_table[lpi].bb_idx = bb_idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_entry_seen_push(cv_t * seen, const char * el) {
    if (!el || !el[0]) return;
    for (uint32_t k = 0; k < seen->len; k++) if (!strcmp(CV_AT(*seen, const char *, k), el)) return;
    CV_PUSH(*seen, const char *) = el;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_indirect_lit_to_var(tree_t * t) {
    if (!t || t->t == TT_GOTO_U || t->t == TT_GOTO_S || t->t == TT_GOTO_F) return;
    if (t->t == TT_INDIRECT && t->n == 1 && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval && t->c[0]->v.sval[0] && t->c[0]->v.sval[0] != '&' && t->c[0]->v.sval[0] != '*') {
        t->v.sval = t->c[0]->v.sval;
        t->t = TT_VAR;
        t->n = 0;
        return;
    }
    for (int i = 0; i < t->n; i++) sno_indirect_lit_to_var(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_rsv_name(const char * nm) { return nm && (!strcmp(nm, "epsilon") || !strcmp(nm, "NULL") || !strcmp(nm, "null")); }
static int sno_rsv_nullval(const tree_t * t) {
    static const char * const pr[] = { "IDENT", "DIFFER", "EQ", "NE", "LT", "LE", "GT", "GE", "LGT", "LLT", "LGE", "LLE", "LEQ", "LNE", NULL };
    if (!t || t->t == TT_NUL || t->t == TT_INTERROGATE || (t->t == TT_QLIT && (!t->v.sval || !t->v.sval[0])) || (t->t == TT_VAR && sno_rsv_name(t->v.sval))) return 1;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n == 2) return sno_rsv_nullval(t->c[0]) && sno_rsv_nullval(t->c[1]);
    if (t->t == TT_FNC && t->v.sval) for (int i = 0; pr[i]; i++) if (!strcmp(t->v.sval, pr[i])) return 1;
    return 0;
}
static int sno_rsv_err(const tree_t * s, const char * nm, const char * what, int quiet) {
    if (quiet) return 1;
    extern const char * stmt_src_get_file(void);
    const char * f = s ? sfind_str(s, ":file") : NULL;
    if (!f || !*f) f = stmt_src_get_file();
    long l = s ? (long) lp_s_int(s, ":lline") : 0;
    if (!l && s) l = (long) lp_s_int(s, ":line");
    fprintf(stderr, "%s:%ld: error: %s is a reserved constant, the zero-length string -- %s\n", (f && *f) ? f : "scrip", l, nm, what);
    return 1;
}
static int sno_rsv_proto(const char * p, const tree_t * s, int quiet) {
    int bad = 0;
    const char * q = p ? strchr(p, '(') : NULL;
    if (!q) return 0;
    for (q++; *q; ) {
        while (*q == ' ' || *q == ',' || *q == ')') q++;
        const char * b = q;
        while (*q && *q != ',' && *q != ')' && *q != ' ') q++;
        size_t n = (size_t)(q - b);
        if (q == b) continue;
        char nm[n + 1];
        memcpy(nm, b, n);
        nm[n] = 0;
        if (sno_rsv_name(nm)) bad += sno_rsv_err(s, nm, "it cannot be a parameter or a local", quiet);
    }
    return bad;
}
static int sno_rsv_walk(tree_t * t, const tree_t * s, int quiet);
static int sno_rsv_stmt(tree_t * s, int quiet) {
    tree_t * sa = NULL;
    tree_t * subj = NULL;
    tree_t * repl = NULL;
    int eq = 0, pat = 0, bad = 0;
    for (int i = 0; i < s->n; i++) {
        tree_t * a = s->c[i];
        if (!a || a->t != TT_ATTR || !a->v.sval) continue;
        if (!strcmp(a->v.sval, ":subj")) { sa = a; subj = a->n > 0 ? a->c[0] : NULL; } else if (!strcmp(a->v.sval, ":repl")) repl = a->n > 0 ? a->c[0] : NULL;
        else if (!strcmp(a->v.sval, ":eq")) eq = 1;
        else if (!strcmp(a->v.sval, ":pat")) pat = 1;
    }
    if (eq && !pat && subj && subj->t == TT_VAR && sno_rsv_name(subj->v.sval)) {
        if (!sno_rsv_nullval(repl)) return sno_rsv_err(s, subj->v.sval, "it cannot be assigned", quiet);
        if (repl && repl->t != TT_NUL) sa->c[0] = repl;
        else { tree_t * q = ast_node_new(TT_QLIT); q->v.sval = (char *) ""; sa->c[0] = q; }
        int k = 0;
        for (int i = 0; i < s->n; i++) { tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && (!strcmp(a->v.sval, ":eq") || !strcmp(a->v.sval, ":repl"))) continue; s->c[k++] = a; }
        s->n = k;
    }
        else if (eq && subj &&
        ((pat && subj->t == TT_VAR && sno_rsv_name(subj->v.sval)) || (subj->t == TT_SCAN && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR && sno_rsv_name(subj->c[0]->v.sval))))
        return sno_rsv_err(s, subj->t == TT_VAR ? subj->v.sval : subj->c[0]->v.sval, "it cannot be the subject of a replacement", quiet);
    for (int i = 0; i < s->n; i++) {
        tree_t * a = s->c[i];
        if (!a) continue;
        if (a->t == TT_ATTR && a->v.sval && (!strcmp(a->v.sval, ":lbl") || !strncmp(a->v.sval, ":go", 3))) continue;
        bad += sno_rsv_walk(a, s, quiet);
    }
    return bad;
}
static int sno_rsv_walk(tree_t * t, const tree_t * s, int quiet) {
    if (!t) return 0;
    int bad = 0, from = 0;
    switch (t->t) {
        case TT_STMT:
        return sno_rsv_stmt(t, quiet);
        case TT_GOTO_U:
        case TT_GOTO_S:
        case TT_GOTO_F:
        return 0;
        case TT_VAR:
        if (sno_rsv_name(t->v.sval)) { t->t = TT_QLIT; t->v.sval = (char *) ""; t->n = 0; }
        return 0;
        case TT_NAME:
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) return 0;
        break;
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        if (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && sno_rsv_name(t->c[1]->v.sval)) return sno_rsv_err(s, t->c[1]->v.sval, "it cannot be a capture target", quiet) +
            sno_rsv_walk(t->c[0], s, quiet);
        break;
        case TT_CAPT_CURSOR:
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && sno_rsv_name(t->c[0]->v.sval)) return sno_rsv_err(s, t->c[0]->v.sval, "it cannot be a capture target", quiet);
        break;
        case TT_ASSIGN:
        if (t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && sno_rsv_name(t->c[0]->v.sval)) {
            if (!sno_rsv_nullval(t->c[1])) return sno_rsv_err(s, t->c[0]->v.sval, "it cannot be assigned", quiet);
            if (!t->c[1]) { t->t = TT_QLIT; t->v.sval = (char *) ""; t->n = 0; return 0; }
            *t = *t->c[1];
            return sno_rsv_walk(t, s, quiet);
        }
        break;
        case TT_DEFINE:
        if (t->n > 1 && t->c[1] && t->c[1]->t == TT_QLIT) bad += sno_rsv_proto(t->c[1]->v.sval, s, quiet);
        break;
        case TT_FNC:
        if (!t->v.sval && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) from = 1;
        {
            const char * fn = t->v.sval ? t->v.sval : (from ? t->c[0]->v.sval : NULL);
            if (fn && !strcmp(fn, "DEFINE") && t->n > from && t->c[from] && t->c[from]->t == TT_QLIT) bad += sno_rsv_proto(t->c[from]->v.sval, s, quiet);
        }
        break;
        default:
        break;
    }
    for (int i = from; i < t->n; i++) bad += sno_rsv_walk(t->c[i], s, quiet);
    return bad;
}
static int sno_rsv_program(const tree_t * prog, int quiet) { int bad = 0; for (int i = 0; prog && i < prog->n; i++) bad += sno_rsv_walk((tree_t *) prog->c[i], NULL, quiet); return bad; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int snobol4_list_event_count(void);
extern int snobol4_list_event_line(int);
extern int snobol4_list_event_on(int);
extern const char *snobol4_list_event_text(int);
typedef struct { char *buf; size_t len; size_t cap; } sno_list_sb_t;
static void sno_list_sb_append(sno_list_sb_t * sb, const char * s, size_t n) {
    if (sb->len + n + 1 > sb->cap) {
        size_t newcap = sb->cap ? sb->cap * 2 : 1024;
        while (newcap < sb->len + n + 1) newcap *= 2;
        char * g = ct_grow(sb->buf, newcap);
        if (!g) return;
        sb->buf = g;
        sb->cap = newcap;
    }
    memcpy(sb->buf + sb->len, s, n);
    sb->len += n;
    sb->buf[sb->len] = '\0';
}
static void sno_list_sb_puts(sno_list_sb_t * sb, const char * s) { sno_list_sb_append(sb, s, strlen(s)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * sno_list_build_listing(const tree_t * prog) {
    int nev = snobol4_list_event_count();
    if (nev == 0 || !prog) return NULL;
    sno_list_sb_t sb = {0};
    int ei = 0, list_on = 0, banner_done = 0, si = 0;
    for (;;) {
        int have_ev = (ei < nev);
        int ev_line = have_ev ? snobol4_list_event_line(ei) : 0;
        const tree_t * snode = NULL;
        int s_line = 0;
        for (int j = si; j < prog->n; j++) {
            if (prog->c[j] && (prog->c[j]->t == TT_STMT || prog->c[j]->t == TT_END)) { snode = prog->c[j]; s_line = lp_s_int(snode, ":line"); si = j; break; }
            si = j + 1;
        }
        int have_st = (snode != NULL);
        if (!have_ev && !have_st) break;
        if (have_ev && (!have_st || ev_line < s_line)) {
            if (snobol4_list_event_on(ei)) {
                if (!banner_done) {
                    time_t now = time(NULL);
                    const char * ct = ctime(&now);
                    size_t cl = ct ? strlen(ct) : 0;
                    if (cl && ct[cl - 1] == '\n') cl--;
                    char tb[cl + 1];
                    memcpy(tb, ct ? ct : "", cl);
                    tb[cl] = '\0';
                    sno_list_sb_puts(&sb, "\n\n\nmacro spitbol version 1.0\nx86-64  ");
                    sno_list_sb_puts(&sb, tb);
                    sno_list_sb_puts(&sb, "\n\n\n\n");
                    { const char * hdr = "page 1"; int pad = 118 - (int) strlen(hdr); if (pad < 0) pad = 0; for (int k = 0; k < pad; k++) sno_list_sb_append(&sb, " ", 1); sno_list_sb_puts(&sb, hdr); }
                    sno_list_sb_append(&sb, "\n", 1);
                    sno_list_sb_append(&sb, "\n", 1);
                    banner_done = 1;
                }
                sno_list_sb_puts(&sb, "        ");
                sno_list_sb_puts(&sb, snobol4_list_event_text(ei));
                sno_list_sb_append(&sb, "\n", 1);
                list_on = 1;
            } else {
                list_on = 0;
            }
            ei++;
        } else {
            if (list_on) {
                const char * txt;
                const char * ent = (snode->t == TT_END) ? sfind_str(snode, ":entry") : NULL;
                char endbuf[(ent && ent[0]) ? fmt_len("END %s", ent) : 4];
                if (snode->t == TT_END) {
                    if (ent && ent[0]) snprintf(endbuf, sizeof endbuf, "END %s", ent);
                    else snprintf(endbuf, sizeof endbuf, "END");
                    txt = endbuf;
                } else {
                    txt = sfind_str(snode, ":src");
                    if (!txt) txt = "";
                }
                int stno = lp_s_int(snode, ":stno");
                char numbuf[24];
                int nn = snprintf(numbuf, sizeof numbuf, "%-8d", stno);
                if (nn < 0) nn = 0;
                if (nn >= (int) sizeof(numbuf)) nn = (int) sizeof(numbuf) - 1;
                sno_list_sb_append(&sb, numbuf, (size_t) nn);
                sno_list_sb_puts(&sb, txt);
                sno_list_sb_append(&sb, "\n", 1);
            }
            si++;
        }
    }
    if (sb.len > 0 && sb.buf[sb.len - 1] == '\n') { sb.len--; sb.buf[sb.len] = '\0'; }
    return sb.buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t * lower_sno_stage2(const tree_t * prog) {
    { extern void gva_keyword_refuse_seed_snobol4(void); gva_keyword_refuse_seed_snobol4(); }
    g_sno_expr_define_seen = 0;
    g_sno_prescan_top = NULL;
    g_sno_seal_enabled = 1;
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    for (int i = 0; i < prog->n; i++) sno_indirect_lit_to_var((tree_t *) prog->c[i]);
    { int rsv = sno_rsv_program(prog, 0); if (rsv) { fprintf(stderr, "scrip: %d reserved-constant error(s) -- no code generated\n", rsv); exit(1); } }
    g_sno_exprs.len = 0;
    g_sno_pats.len = 0;
    g_sno_uses_stmtkw = 0;
    g_sno_traces_a_label = 0;
    g_sno_uses_code = 0;
    g_sno_calls_code = 0;
    g_sno_multiproto.len = 0;
    g_sno_proto_fn.len = 0;
    g_sno_proto_enc.len = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i]) sno_scan_code_use(prog->c[i]);
    { const char * _sk = getenv("SCRIP_SNO_STMTKW"); if (_sk && *_sk == '1') { g_sno_uses_stmtkw = 1; g_sno_traces_a_label = 1; } }
    sno_register_program(&g_stage2, prog);
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    const tree_t ** st = (const tree_t **) ct_zalloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    sno_prog_scan(st, nst);
    cv_t defs = {0}, def_body = {0};
    g_sno_predef.len = 0;
    g_sno_tdef.len = 0;
    cv_t def_entry_all = {0};
    int * is_def = (int *) ct_zalloc((size_t) nst, sizeof(int));
    cv_t stmt_bind_fname = {0};
    cv_t exprdef_names = {0};
    for (int i = 0; i < nst; i++) {
        g_sno_prescan_top = lc_stmt_subj(st[i]);
        sno_prescan_expr(g_sno_prescan_top, &defs, &def_body, &exprdef_names, &def_entry_all);
        { const tree_t * rp = sfind_expr(st[i], ":repl"); if (rp) sno_prescan_repl_defines(rp, &defs, &def_body, &exprdef_names, &def_entry_all); }
        const tree_t * dfn = lc_stmt_subj(st[i]);
        if (dfn && dfn->t == TT_DEFINE) {
            is_def[i] = 1;
            const tree_t * pnode = (dfn->n > 1) ? dfn->c[1] : NULL;
            if (!pnode || pnode->t != TT_QLIT || !pnode->v.sval) sno_fatal("TT_DEFINE missing literal prototype string", NULL);
            sno_def_t d;
            sno_parse_define(pnode->v.sval, NULL, &d);
            if (sn4_sysfn_protected(d.fname)) continue;
            sno_tdef_note(d.fname);
            const tree_t * body = (dfn->n > 2) ? dfn->c[2] : NULL;
            int found = -1;
            for (uint32_t k = 0; k < defs.len; k++) if (!strcmp(CV_AT(defs, sno_def_t, k).fname, d.fname)) { found = (int) k; break; }
            if (!body) sno_entry_seen_push(&def_entry_all, d.entry);
            if (found >= 0) { CV_AT(defs, sno_def_t, found) = d; CV_AT(def_body, const tree_t *, found) = body; } else sno_def_put(&defs, &def_body, &d, body);
            CV_PUSH(stmt_bind_fname, const char *) = d.fname;
            continue;
        }
        int argbase = 0;
        const tree_t * dsub = sno_stmt_define(st[i], &argbase);
        if (!dsub) continue;
        is_def[i] = 1;
        const char * entry_opt = NULL;
        if (dsub->n > argbase + 1 && dsub->c[argbase + 1]) {
            const tree_t * ea = dsub->c[argbase + 1];
            if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
            else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
        }
        sno_def_t d;
        sno_parse_define(sno_qlit_fold(dsub->c[argbase]), entry_opt, &d);
        if (sn4_sysfn_protected(d.fname)) continue;
        int found = -1;
        for (uint32_t k = 0; k < defs.len; k++) if (!strcmp(CV_AT(defs, sno_def_t, k).fname, d.fname)) { found = (int) k; break; }
        sno_entry_seen_push(&def_entry_all, d.entry);
        sno_proto_note(&d);
        if (found >= 0) CV_AT(defs, sno_def_t, found) = d;
        else sno_def_put(&defs, &def_body, &d, NULL);
        CV_PUSH(stmt_bind_fname, const char *) = d.fname;
    }
    int main_entry_idx = 0;
    for (int i = 0; i < prog->n; i++) {
        if (!prog->c[i] || prog->c[i]->t != TT_END) continue;
        const char * end_entry = sfind_str(prog->c[i], ":entry");
        if (!end_entry || !end_entry[0]) break;
        for (int k = 0; k < nst; k++) { const char * klbl = sfind_str(st[k], ":lbl"); if (klbl && !strcmp(klbl, end_entry)) { main_entry_idx = k; break; } }
        break;
    }
    long _end_line = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_END) { _end_line = (long) lp_s_int(prog->c[i], ":line"); break; }
    IR_graph_t * g = sno_build_graph(st, nst, main_entry_idx, is_def, NULL, 0, _end_line);
    {
        IR_t * prelude_head = NULL;
        IR_t * prelude_tail = NULL;
        for (uint32_t di = 0; di < defs.len; di++) {
            const char * dfn = CV_AT(defs, sno_def_t, di).fname;
            int covered = 0;
            for (uint32_t k = 0; k < stmt_bind_fname.len; k++) if (!strcmp(CV_AT(stmt_bind_fname, const char *, k), dfn)) { covered = 1; break; }
            if (covered || !sno_exprdef_seen(&exprdef_names, dfn)) continue;
            IR_t * pfail = lc_build(g, IR_FAIL, NULL, NULL);
            IR_t * pbind = lc_build(g, IR_DEFINE, NULL, pfail);
            IR_LIT(pbind).sval = lp_strdup(dfn);
            if (!prelude_head) prelude_head = pbind;
            else lc_γ_to(prelude_tail, pbind);
            prelude_tail = pbind;
        }
        if (prelude_head) { lc_γ_to(prelude_tail, g->entry); g->entry = prelude_head; }
        { IR_t * qt = lc_build(g, IR_CALL, g->entry, g->entry); IR_LIT(qt).sval = (char *) "$quit_trap_320"; g->entry = qt; }
        { IR_t * fpm = lc_build(g, IR_CALL, g->entry, g->entry); IR_LIT(fpm).sval = (char *) "$fp_model_spitbol"; g->entry = fpm; }
        {
            char * listing = sno_list_build_listing(prog);
            if (listing && listing[0]) {
                IR_t * lit = lc_build(g, IR_LIT_STRING, g->entry, g->entry);
                IR_LIT(lit).sval = listing;
                IR_t * call = lc_build(g, IR_CALL, g->entry, g->entry);
                IR_LIT(call).sval = (char *) "SNO$LIST";
                lc_γ_to(lit, call);
                ir_operand_push(call, lit);
                g->entry = lit;
            }
        }
    }
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = "main";
    g_stage2.proc_table[pi].proc = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].nparams = 0;
    g_stage2.proc_table[pi].is_generator = 0;
    g_stage2.proc_table[pi].dyn_scope = 0;
    g_stage2.proc_table[pi].result_name = NULL;
    g_stage2.proc_table[pi].bb_idx = bb_program_add(&g_stage2.bbp, g);
    { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) { g_stage2.module_registry.mods[mi].nprocs++; if (g_stage2.module_registry.main_mod < 0) g_stage2.module_registry.main_mod = mi; } }
    if (g_sno_uses_code) {
        int main_bb_idx = g_stage2.proc_table[pi].bb_idx;
        for (int i = 0; i < nst; i++) {
            const char * lbl = sfind_str(st[i], ":lbl");
            if (!lbl || !lbl[0]) continue;
            IR_t * anchor = bb_label_landing(lbl);
            if (!anchor) continue;
            char lname[strlen(lbl) + 6];
            snprintf(lname, sizeof lname, "LBL__%s", lbl);
            int lpi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[lpi].name = lp_strdup(lname);
            g_stage2.proc_table[lpi].proc = NULL;
            g_stage2.proc_table[lpi].entry_pc = -1;
            g_stage2.proc_table[lpi].nparams = 0;
            g_stage2.proc_table[lpi].lower_sc.n = 0;
            g_stage2.proc_table[lpi].is_generator = 0;
            g_stage2.proc_table[lpi].dyn_scope = 0;
            g_stage2.proc_table[lpi].result_name = NULL;
            g_stage2.proc_table[lpi].proc_entry_node = anchor;
            g_stage2.proc_table[lpi].bb_idx = main_bb_idx;
        }
    }
    if (!g_sno_uses_code) {
        int main_bb_idx2 = g_stage2.proc_table[pi].bb_idx;
        for (uint32_t di = 0; di < defs.len; di++) if (!CV_AT(def_body, const tree_t *, di)) sno_register_entry_label(CV_AT(defs, sno_def_t, di).entry, main_bb_idx2);
        for (uint32_t ei = 0; ei < def_entry_all.len; ei++) sno_register_entry_label(CV_AT(def_entry_all, const char *, ei), main_bb_idx2);
    }
    for (uint32_t di = 0; di < defs.len; di++) {
        const sno_def_t d0 = CV_AT(defs, sno_def_t, di);
        const tree_t * dbody = CV_AT(def_body, const tree_t *, di);
        IR_graph_t * gf;
        const char * rn = d0.result_name ? d0.result_name : d0.fname;
        if (dbody) {
            const tree_t * bp = dbody;
            int bn = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bn++;
            const tree_t ** bst = (const tree_t **) ct_zalloc((size_t)(bn > 0 ? bn : 1), sizeof(tree_t *));
            int bk = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bst[bk++] = bp->c[i];
            int * bis = (int *) ct_zalloc((size_t)(bn > 0 ? bn : 1), sizeof(int));
            gf = sno_build_graph(bst, bn, 0, bis, rn, 0, 0);
            ct_drop((void *) bst);
            ct_drop(bis);
        } else {
            int eidx = -1;
            for (int i = 0; i < nst; i++) { const char * lbl = sfind_str(st[i], ":lbl"); if (lbl && !strcmp(lbl, d0.entry)) { eidx = i; break; } }
            if (eidx < 0) continue;
            gf = sno_build_call_stub(d0.entry, d0.fname);
        }
        int fpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name = d0.fname;
        g_stage2.proc_table[fpi].proc = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].nparams = d0.nnames;
        g_stage2.proc_table[fpi].nformals = d0.nformals;
        stage2_scope_reserve(&g_stage2.proc_table[fpi].lower_sc, d0.nnames);
        for (int k = 0; k < d0.nnames; k++) g_stage2.proc_table[fpi].lower_sc.e[k].name = d0.names[k];
        g_stage2.proc_table[fpi].lower_sc.n = d0.nnames;
        g_stage2.proc_table[fpi].is_generator = 0;
        g_stage2.proc_table[fpi].dyn_scope = 1;
        g_stage2.proc_table[fpi].result_name = rn;
        gf->multi_proto = sno_fname_is_multiproto(d0.fname);
        g_stage2.proc_table[fpi].bb_idx = bb_program_add(&g_stage2.bbp, gf);
        if (dbody && g_sno_uses_code) {
            int fbi = g_stage2.proc_table[fpi].bb_idx;
            for (int i = 0; i < dbody->n; i++) if (dbody->c[i] && dbody->c[i]->t == TT_STMT) sno_register_entry_label(sfind_str(dbody->c[i], ":lbl"), fbi);
        }
    }
    sno_expr_thunks_build(0);
    {
        int xdone = sno_expr_mark();
        int pdone = 0;
        for (;;) {
            int pn = sno_pat_count();
            if (pn > pdone) { sno_pat_thunks_build(pdone); pdone = pn; }
            int xn = sno_expr_mark();
            if (xn > xdone) { sno_expr_thunks_build(xdone); xdone = xn; }
            if (sno_pat_count() == pdone && sno_expr_mark() == xdone) break;
        }
    }
    ct_drop((void *) st);
    ct_drop(is_def);
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_pat_tree_graph_rt(const tree_t * pat) {
    IR_graph_t * gp = IR_alloc(512);
    scx_t px;
    px.g = gp;
    px.loop_exit = NULL;
    px.loop_next = NULL;
    px.result_name = NULL;
    px.pat_fail = NULL;
    px.pat_seal = NULL;
    px.npre = 0;
    px.prog_nstmt = 0;
    px.stno_base = 0;
    IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
    IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
    px.pat_fail = no;
    px.pat_seal = sno_thunk_abort_exit(gp, no);
    int before_pat = gp->n;
    int rtc = sno_rtseq_resume();
    int rt_xmark = sno_expr_mark();
    IR_t * brt = NULL;
    int pfenced = 0;
    IR_t * pe = rtc ? sno_pat_carrier_build(&px, pat, ok, no, &brt, &pfenced) : sno_pat_node(&px, pat, ok, no);
    if (px.npre > 0) sno_fatal("runtime-operand primitive reached the RT recipe graph builder — recipes must bake literal args (B-RE contract)", NULL);
    if (sno_expr_mark() != rt_xmark) sno_fatal("deferred pattern-primitive argument reached the RT recipe graph builder — its expression thunk has no builder on this path (B-RE contract)", NULL);
    gp->entry = pe;
    gp->resumable_callable = 1;
    if (rtc) sno_pat_publish_body_root(gp, before_pat, pat, brt, pfenced, "RT$", px.pat_seal);
    else gp->body_root = (gp->n > before_pat && !sno_pat_right_sealed(pat)) ? gp->all[before_pat] : NULL;
    return gp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_lower_fragment_at(const tree_t * prog, int entry_idx, long stno_base) {
    if (!prog || prog->t != TT_PROGRAM || sno_rsv_program(prog, 1)) return NULL;
    { extern void zls_reset(void); zls_reset(); }
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    if (nst == 0 || entry_idx < 0 || entry_idx >= nst) return NULL;
    const tree_t ** st = (const tree_t **) ct_zalloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    g_sno_predef.len = 0;
    g_sno_tdef.len = 0;
    g_sno_t4.len = 0;
    g_sno_t4_unsafe = 1;
    int seal_sv = g_sno_seal_enabled;
    g_sno_seal_enabled = 0;
    int * is_def = (int *) ct_zalloc((size_t) nst, sizeof(int));
    IR_graph_t * g = sno_build_graph(st, nst, entry_idx, is_def, NULL, stno_base, 0);
    { extern void optimizer_run(IR_graph_t *); extern void ir_drive_slot_assign(IR_graph_t *); if (g) { optimizer_run(g); ir_drive_slot_assign(g); } }
    ct_drop((void *) st);
    ct_drop(is_def);
    g_sno_seal_enabled = seal_sv;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_snobol4(const tree_t * prog) { return sno_lower_fragment_at(prog, 0, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_stmt_label(const tree_t * s) { return s ? sfind_str(s, ":lbl") : NULL; }
