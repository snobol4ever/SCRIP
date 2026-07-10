#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lower.h"
#include "bb_program.h"
#include "parser/icon/icon_lex.h"
extern void global_register(const char * name);
extern int stage2_proc_grow(stage2_t * s2);
typedef struct { IR_graph_t * g; IR_t * loop_exit; IR_t * loop_next; const char * result_name; IR_t * pat_fail; IR_t * pat_seal; } scx_t;
#define SNO_DEF_MAX 128
#define SNO_DEF_NAMES_MAX 64
typedef struct { const char * fname; const char * entry; const char * result_name; const char * names[SNO_DEF_NAMES_MAX]; int nnames; } sno_def_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SNO_EXPR_MAX 256
static struct { const char * name; const tree_t * expr; } g_sno_exprs[SNO_EXPR_MAX];
static int g_sno_nexpr = 0;
static const char * sno_expr_collect(const tree_t * expr);
#define SNO_PAT_MAX 256
static struct { const char * name; const tree_t * pat; } g_sno_pats[SNO_PAT_MAX];
static int g_sno_npat = 0;
static int g_sno_uses_stmtkw = 0;
/*--- true iff the tree references &STNO/&STCOUNT/&LASTNO — those keywords need the per-statement rt_stmt_enter hook, others don't ---*/
static int sno_kw_is_stmt(const char * s) {
    if (!s) return 0; if (s[0] == '&') s++;
    char lk[16]; size_t i = 0; for (; s[i] && i < sizeof(lk) - 1; i++) lk[i] = (s[i] >= 'A' && s[i] <= 'Z') ? (char)(s[i] - 'A' + 'a') : s[i]; lk[i] = 0;
    return !strcmp(lk, "stno") || !strcmp(lk, "stcount") || !strcmp(lk, "lastno");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_scan_stmtkw(const tree_t * t) {
    if (!t || g_sno_uses_stmtkw) return;
    if (t->t == TT_KEYWORD && sno_kw_is_stmt(t->v.sval)) { g_sno_uses_stmtkw = 1; return; }
    for (int i = 0; i < t->n; i++) sno_scan_stmtkw(t->c[i]);
}
/*--- true iff the program calls CODE( ) anywhere — gates the LBL__ pseudo-proc export so programs that never
 *--- runtime-compile stay byte-identical (same use-gate discipline as g_sno_uses_stmtkw / rt_stmt_enter) ---*/
static int g_sno_uses_code = 0;
static void sno_scan_code_use(const tree_t * t) {
    if (!t || g_sno_uses_code) return;
    if (t->t == TT_FNC) {
        const char * fn = t->v.sval;
        if (!fn && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) fn = t->c[0]->v.sval;
        if (fn && !strcmp(fn, "CODE")) { g_sno_uses_code = 1; return; }
    }
    for (int i = 0; i < t->n; i++) sno_scan_code_use(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fatal(const char * what, const char * detail) {
    fprintf(stderr, "FATAL lower_snobol4 (GZ#5 subset): %s%s%s. Pattern matching, EVAL and CODE are outside the landed subset (IR_MATCH_* family pending); see GOAL-SNOBOL4-BB.md.\n",
            what, detail ? ": " : "", detail ? detail : "");
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sfind(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) { const tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sfind_str(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0 && a->c[0]) ? a->c[0]->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * sfind_expr(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0) ? a->c[0] : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_reg_var(const char * nm) { if (nm && nm[0] && nm[0] != '&') global_register(lp_strdup(nm)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_collect(const tree_t * expr) {
    if (!expr) sno_fatal("unevaluated-expression operator (*) with no operand", NULL);
    if (g_sno_nexpr >= SNO_EXPR_MAX) sno_fatal("too many unevaluated expressions (*) in one program", NULL);
    char buf[32]; snprintf(buf, sizeof buf, "EXPR$%d", g_sno_nexpr);
    g_sno_exprs[g_sno_nexpr].name = lp_strdup(buf);
    g_sno_exprs[g_sno_nexpr].expr = expr;
    return g_sno_exprs[g_sno_nexpr++].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_binop_code(tree_e tt) {
    switch (tt) {
    case TT_ADD: return 0; case TT_SUB: return 1; case TT_MUL: return 2; case TT_DIV: return 3; case TT_POW: return 18; case TT_SEQ: return 11; case TT_CAT: return 11;
    default: return -1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sco_branch(scx_t * cx, const tree_t * pg, IR_t * γ, IR_t * ω) {
    if (!pg) return γ;
    if (pg->t != TT_PROGRAM) { IR_t * r = NULL; return sx_lower(cx, pg, γ, ω, &r); }
    IR_t * entry = γ;
    for (int i = pg->n - 1; i >= 0; i--) {
        const tree_t * s = pg->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t * subj = lc_stmt_subj(s);
        if (!subj) continue;
        IR_t * r = NULL;
        entry = sx_lower(cx, subj, entry, entry, &r);
    }
    return entry;
}
static IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_binop(scx_t * cx, const tree_t * t, int code, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * op = lc_build(cx->g, IR_BINOP, γ, ω); IR_LIT(op).ival = code;
    IR_t * lr = NULL; IR_t * rr = NULL;
    IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &lr);
    IR_t * eb = sx_lower(cx, t->c[1], op, ω, &rr);
    lc_γ_to(lr, eb);
    ir_operand_push(op, lr); ir_operand_push(op, rr);
    if (res) *res = op; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * g_sno_predef[SNO_DEF_MAX]; static int g_sno_npredef = 0;
static void sno_predef_note(const char * fname) { for (int k = 0; k < g_sno_npredef; k++) if (!strcmp(g_sno_predef[k], fname)) return; if (g_sno_npredef < SNO_DEF_MAX) g_sno_predef[g_sno_npredef++] = fname; }
static int sno_predef_registered(const char * fname) { if (!fname) return 0; for (int k = 0; k < g_sno_npredef; k++) if (!strcmp(g_sno_predef[k], fname)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_tree_has_define_call(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC) {
        const char * name = t->v.sval;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) name = t->c[0]->v.sval;
        if (name && !strcmp(name, "DEFINE")) return 1;
    }
    for (int i = 0; i < t->n; i++) if (sno_tree_has_define_call(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_call_named(scx_t * cx, const char * name, const tree_t * t, int argbase, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * call = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(call).sval = (char *) lp_strdup(name);
    int nargs = t ? (t->n - argbase) : 0;
    IR_t * prev = NULL; IR_t * entry = call;
    for (int k = 0; k < nargs; k++) {
        IR_t * ar = NULL; IR_t * ae = sx_lower(cx, t->c[argbase + k], (k == nargs - 1) ? call : NULL, ω, &ar);
        if (k == 0) entry = ae;
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(call, ar);
    }
    if (res) *res = call; return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_nameval(scx_t * cx, const tree_t * inner, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$NAME";
    IR_t * nr = NULL; IR_t * ne = sx_lower(cx, inner, mk, ω, &nr);
    if (nr) ir_operand_push(mk, nr);
    if (res) *res = mk; return ne;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = lc_build(cx->g, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; if (res) *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; if (res) *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
    case TT_NUL:  { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    case TT_VAR:  { sno_reg_var(t->v.sval); IR_t * nd = lc_build(cx->g, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; if (res) *res = nd; return nd; }
    case TT_KEYWORD: { IR_t * nd = lc_build(cx->g, IR_KEYWORD_SNOBOL4, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
    case TT_DEFER: {
        const char * bn = sno_expr_collect((t->n > 0) ? t->c[0] : NULL);
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$MKEXPR";
        IR_t * nl = lc_build(cx->g, IR_LIT_STRING, mk, ω); IR_LIT(nl).sval = (char *) bn;
        ir_operand_push(mk, nl);
        if (res) *res = mk;
        return nl;
    }
    case TT_NAME: {
        if (t->n < 1 || !t->c[0]) sno_fatal("name operator with no operand", NULL);
        if (t->c[0]->t == TT_VAR && t->c[0]->v.sval) {
            sno_reg_var(t->c[0]->v.sval);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$NAME";
            IR_t * nl = lc_build(cx->g, IR_LIT_STRING, mk, ω); IR_LIT(nl).sval = t->c[0]->v.sval;
            ir_operand_push(mk, nl);
            if (res) *res = mk; return nl;
        }
        if (t->c[0]->t == TT_IDX && t->c[0]->n >= 2) {
            const tree_t * ix = t->c[0];
            IR_t * vr = NULL; IR_t * entry = sx_subscript_lv(cx, ix->c[0], (const tree_t * const *) &ix->c[1], ix->n - 1, ω, &vr);
            if (γ) lc_γ_to(vr, γ);
            if (res) *res = vr; return entry;
        }
        sno_fatal("name operator over this form is outside the landed subset", NULL);
    }
    case TT_ALT: {
        if (t->n < 2) sno_fatal("alternation with missing operand", NULL);
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PBALT";
        IR_t * vl = NULL; IR_t * el = sx_lower(cx, t->c[0], NULL, ω, &vl);
        IR_t * vr = NULL; IR_t * er = sx_lower(cx, t->c[1], NULL, ω, &vr);
        lc_γ_to(vl, er); lc_γ_to(vr, mk);
        ir_operand_push(mk, vl); ir_operand_push(mk, vr);
        if (res) *res = mk;
        return el;
    }
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_POW: case TT_SEQ: case TT_CAT:
        if (t->n < 2) sno_fatal("binary operator with missing operand", NULL);
        return sx_binop(cx, t, sno_binop_code(t->t), γ, ω, res);
    case TT_MNS: case TT_PLS: {
        if (t->n < 1) sno_fatal("unary operator with missing operand", NULL);
        IR_t * op = lc_build(cx->g, IR_UNOP, γ, ω); IR_LIT(op).ival = (long long) t->t;
        IR_t * ar = NULL; IR_t * ea = sx_lower(cx, t->c[0], op, ω, &ar);
        ir_operand_push(op, ar);
        if (res) *res = op; return ea;
    }
    case TT_INDIRECT: {
        if (t->n < 1) sno_fatal("indirect reference with no operand", NULL);
        if (t->c[0] && t->c[0]->t == TT_NAME) {
            IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
            IR_t * nr = NULL; IR_t * ne = sx_lower(cx, t->c[0], NULL, ω, &nr);
            lc_γ_to(nr, dr);
            ir_operand_push(dr, nr);
            if (res) *res = dr; return ne;
        }
        IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
        IR_t * vr = NULL; IR_t * ve = sx_nameval(cx, t->c[0], dr, ω, &vr);
        ir_operand_push(dr, vr);
        if (res) *res = dr; return ve;
    }
    case TT_IDX: {
        if (t->n < 2) sno_fatal("subscript with no index", NULL);
        IR_t * br = NULL; IR_t * entry = sx_lower(cx, t->c[0], NULL, ω, &br);
        IR_t * cur = br;
        for (int k = 1; k < t->n; k++) {
            IR_t * ir = NULL; IR_t * ie = sx_lower(cx, t->c[k], NULL, ω, &ir);
            lc_γ_to(cur, ie);
            IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
            lc_γ_to(ir, sub);
            ir_operand_push(sub, cur); ir_operand_push(sub, ir);
            cur = sub;
        }
        IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
        lc_γ_to(cur, dr);
        ir_operand_push(dr, cur);
        if (res) *res = dr; return entry;
    }
    case TT_OPSYN: {
        /* 1015 (s10): a user-redefinable operator (`@`/`|`/`&`/`~` after OPSYN('@', .DUPL, 2)) — the parser
         * hands the symbol in v.sval and the 1 (unary) or 2 (binary) operands as children.  Runtime `opsyn()`
         * (pattern_match.c) registers the symbol as a FUNCTION ALIAS, so the whole lowering is a by-name call
         * on the symbol itself; an unregistered symbol faults at call time exactly like an undefined function. */
        const char * name = t->v.sval;
        if (!name || !*name) sno_fatal("OPSYN operator expression with no symbol", NULL);
        return sx_call_named(cx, name, t, 0, γ, ω, res);
    }
    case TT_FNC: {
        const char * name = t->v.sval; int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        if (!name) sno_fatal("call with no resolvable name", NULL);
        if (!strcmp(name, "DEFINE")) {
            /* the prescan (sno_prescan_expr) hoist-registered this def iff it sits in a subject expression with a literal prototype; the call itself is then the null string and always succeeds (manual p219) */
            char fnb[128]; fnb[0] = 0;
            if (t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
                const char * sp = t->c[argbase]->v.sval; int k = 0;
                for (; sp[k] && sp[k] != '(' && sp[k] != ' ' && k < 127; k++) fnb[k] = sp[k];
                fnb[k] = 0;
            }
            if (fnb[0] && sno_predef_registered(fnb)) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
            sno_fatal("DEFINE in this expression position is outside the landed subset (literal-prototype DEFINE in a statement subject only; pattern/replacement-field and fragment DEFINE pending)", NULL);
        }
        return sx_call_named(cx, name, t, argbase, γ, ω, res);
    }
    case TT_WHILE: case TT_UNTIL: {
        const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
        if (!C) sno_fatal("loop with no condition", NULL);
        int is_until = (t->t == TT_UNTIL);
        IR_t * cr = NULL;
        IR_t * ce = is_until ? sx_lower(cx, C, γ, NULL, &cr) : sx_lower(cx, C, NULL, γ, &cr);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = ce;
        IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (cr) { if (is_until) { if (!cr->ω.node) lc_ω_to(cr, be); } else { if (!cr->γ.node) lc_γ_to(cr, be); } }
        if (res) *res = NULL;
        return ce;
    }
    case TT_DO_WHILE: {
        const tree_t * B = (t->n > 0) ? t->c[0] : NULL; const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
        if (!C) sno_fatal("do-while without condition outside the landed subset", NULL);
        IR_t * cr = NULL;
        IR_t * ce = sx_lower(cx, C, NULL, γ, &cr);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = ce;
        IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (cr) { if (!cr->γ.node) lc_γ_to(cr, be); }
        if (res) *res = NULL;
        return be;
    }
    case TT_FOR: {
        const tree_t * INIT = (t->n > 0) ? t->c[0] : NULL; const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
        const tree_t * STEP = (t->n > 2) ? t->c[2] : NULL; const tree_t * B = (t->n > 3) ? t->c[3] : NULL;
        if (!C) sno_fatal("for-loop without condition outside the landed subset", NULL);
        IR_t * cr = NULL;
        IR_t * ce = sx_lower(cx, C, NULL, γ, &cr);
        IR_t * se = STEP ? sx_lower(cx, STEP, ce, ω, NULL) : ce;
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = se;
        IR_t * be = B ? sco_branch(cx, B, se, ω) : se;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (cr) { if (!cr->γ.node) lc_γ_to(cr, be); }
        IR_t * ie = INIT ? sx_lower(cx, INIT, ce, ω, NULL) : ce;
        if (res) *res = NULL;
        return ie;
    }
    case TT_LOOP_BREAK: case TT_LOOP_NEXT: {
        if (t->n > 0 && t->c[0]) sno_fatal("labeled break/next outside the SCO-CF-3 subset", NULL);
        IR_t * tgt = (t->t == TT_LOOP_BREAK) ? cx->loop_exit : cx->loop_next;
        if (!tgt) sno_fatal("break/next outside any loop", NULL);
        IR_t * j = lc_build(cx->g, IR_GOTO, tgt, NULL);
        if (res) *res = NULL;
        return j;
    }
    case TT_ASSIGN: {
        const tree_t * L = (t->n > 0) ? t->c[0] : NULL; const tree_t * R = (t->n > 1) ? t->c[1] : NULL;
        if (!L || L->t != TT_VAR || !L->v.sval) sno_fatal("TT_ASSIGN lhs form outside the SCO-CF-2 subset (TT_VAR only)", NULL);
        if (!R) sno_fatal("TT_ASSIGN with no rhs", NULL);
        sno_reg_var(L->v.sval);
        IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = L->v.sval;
        IR_t * vr = NULL; IR_t * e = sx_lower(cx, R, asn, ω, &vr);
        ir_operand_push(asn, vr);
        if (res) *res = asn;
        return e;
    }
    case TT_IF: {
        const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * TH = (t->n > 1) ? t->c[1] : NULL; const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
        if (!C) sno_fatal("TT_IF with no condition", NULL);
        IR_t * th_entry = TH ? sco_branch(cx, TH, γ, ω) : γ;
        IR_t * el_entry = EL ? sco_branch(cx, EL, γ, ω) : γ;
        IR_t * cr = NULL; IR_t * ce = sx_lower(cx, C, th_entry, el_entry, &cr);
        if (res) *res = NULL;
        return ce;
    }
    case TT_NOT: {
        const tree_t * inner = (t->n > 0) ? t->c[0] : NULL;
        if (!inner) sno_fatal("TT_NOT with no operand", NULL);
        IR_t * gate = lc_build(cx->g, IR_GOTO, γ, NULL);
        IR_t * r = NULL;
        IR_t * e = sx_lower(cx, inner, ω, gate, &r);
        if (res) *res = gate;
        return e;
    }
    case TT_CASE: {
        if (t->n < 1 || !t->c[0]) sno_fatal("TT_CASE with no subject", NULL);
        const tree_t * subj = t->c[0];
        const tree_t * def_body = NULL;
        for (int i = 1; i + 1 < t->n; i += 2) if (t->c[i] && t->c[i]->t == TT_NUL) def_body = t->c[i + 1];
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        IR_t * else_tgt;
        if (def_body) { cx->loop_exit = γ; IR_t * db = sco_branch(cx, def_body, γ, ω); cx->loop_exit = sv_exit; else_tgt = db; }
        else else_tgt = γ;
        IR_t * chain = else_tgt;
        for (int i = t->n - 2; i >= 1; i -= 2) {
            const tree_t * v = t->c[i]; const tree_t * b = t->c[i + 1];
            if (v && v->t == TT_NUL) continue;
            cx->loop_exit = γ;
            IR_t * body_entry = sco_branch(cx, b, γ, ω);
            cx->loop_exit = sv_exit;
            tree_t * idc = ast_node_new(TT_FNC); idc->v.sval = (char *) "IDENT"; ast_push(idc, (tree_t *) subj); ast_push(idc, (tree_t *) v);
            IR_t * ir = NULL; IR_t * te = sx_lower(cx, idc, body_entry, chain, &ir);
            chain = te;
        }
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (res) *res = NULL;
        return chain;
    }
    case TT_AUGOP: {
        const tree_t * L = (t->n > 0) ? t->c[0] : NULL;
        if (!L || L->t != TT_VAR || !L->v.sval || t->n < 2) sno_fatal("TT_AUGOP outside the landed subset (simple-variable lhs only)", NULL);
        int code;
        switch ((int) t->v.ival) {
            case TK_AUGPLUS:  code = 0;  break;
            case TK_AUGMINUS: code = 1;  break;
            case TK_AUGSTAR:  code = 2;  break;
            case TK_AUGSLASH: code = 3;  break;
            case TK_AUGPOW:   code = 18; break;
            default: sno_fatal("TT_AUGOP operator outside the landed subset", NULL); code = 0;
        }
        sno_reg_var(L->v.sval);
        IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = L->v.sval;
        IR_t * vr = NULL; IR_t * e = sx_binop(cx, t, code, asn, ω, &vr);
        ir_operand_push(asn, vr);
        if (res) *res = asn;
        return e;
    }
    case TT_RETURN: case TT_NRETURN: case TT_PROC_FAIL: {
        const char * rl = (t->t == TT_PROC_FAIL) ? "FRETURN" : (t->t == TT_NRETURN) ? "NRETURN" : "RETURN";
        IR_t * j = lc_build(cx->g, IR_GOTO, bb_label_landing(rl), NULL);
        if (res) *res = NULL;
        if (t->t == TT_RETURN && t->n > 0 && t->c[0] && cx->result_name) {
            sno_reg_var(cx->result_name);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN, j, j); IR_LIT(asn).sval = (char *) cx->result_name;
            IR_t * vr = NULL; IR_t * e = sx_lower(cx, t->c[0], asn, j, &vr);
            ir_operand_push(asn, vr);
            return e;
        }
        return j;
    }
    default: {
        char buf[64]; snprintf(buf, sizeof buf, "tree kind %d", (int) t->t);
        sno_fatal("expression form not in the landed subset", buf);
        return NULL;
    }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sgoto(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind) continue;
        if (a->n > 0 && a->c[0] && a->c[0]->t == TT_QLIT && a->c[0]->v.sval) return a->c[0]->v.sval;
        sno_fatal("indirect/computed goto not in the landed subset", NULL);
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_target(IR_graph_t * g, const char * nm, IR_t * exitnd) {
    IR_t * l = (nm && nm[0] != '$') ? bb_label_landing(nm) : NULL;
    if (l) return l;
    if (!nm || !nm[0]) sno_fatal("goto to unknown label", "?");
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL);
    IR_LIT(gd).sval = lp_strdup(nm);
    return gd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_stmt_define(const tree_t * s, int * out_argbase) {
    const tree_t * subj = lc_stmt_subj(s);
    if (!subj || subj->t != TT_FNC) return NULL;
    const char * name = subj->v.sval; int argbase = 0;
    if (!name && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR) { name = subj->c[0]->v.sval; argbase = 1; }
    if (!name || strcmp(name, "DEFINE")) return NULL;
    if (sfind(s, ":eq") || sfind_expr(s, ":pat")) sno_fatal("DEFINE with a pattern or replacement field is outside the landed subset", NULL);
    if (subj->n <= argbase || !subj->c[argbase] || subj->c[argbase]->t != TT_QLIT || !subj->c[argbase]->v.sval)
        sno_fatal("DEFINE with a non-literal prototype string is outside the landed subset (runtime DEFINE pending)", NULL);
    if (out_argbase) *out_argbase = argbase;
    return subj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d) {
    char buf[512]; int bn = 0;
    for (const char * p = spec; *p && bn < (int) sizeof buf - 1; p++) if (*p != ' ' && *p != '\t') buf[bn++] = *p;
    buf[bn] = 0;
    char * par = strchr(buf, '(');
    char * cls = par ? strchr(par, ')') : NULL;
    if (!par || !cls) sno_fatal("DEFINE prototype missing parameter parentheses", spec);
    *par = 0; *cls = 0;
    if (!buf[0]) sno_fatal("DEFINE prototype missing function name", spec);
    d->fname = lp_strdup(buf);
    d->entry = (entry_opt && entry_opt[0]) ? lp_strdup(entry_opt) : d->fname;
    d->result_name = NULL;
    d->nnames = 0;
    for (char * seg = par + 1; seg && *seg; ) {
        char * cm = strchr(seg, ','); if (cm) *cm = 0;
        if (*seg && d->nnames < SNO_DEF_NAMES_MAX) d->names[d->nnames++] = lp_strdup(seg);
        seg = cm ? cm + 1 : NULL;
    }
    for (char * seg = cls + 1; seg && *seg; ) {
        char * cm = strchr(seg, ','); if (cm) *cm = 0;
        if (*seg && d->nnames < SNO_DEF_NAMES_MAX) d->names[d->nnames++] = lp_strdup(seg);
        seg = cm ? cm + 1 : NULL;
    }
    sno_reg_var(d->fname);
    for (int k = 0; k < d->nnames; k++) sno_reg_var(d->names[k]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res) {
    IR_t * br = NULL; IR_t * entry = sx_lower(cx, base, NULL, ω, &br);
    IR_t * cur = br;
    for (int k = 0; k < nidx; k++) {
        IR_t * ir = NULL; IR_t * ie = sx_lower(cx, idxs[k], NULL, ω, &ir);
        lc_γ_to(cur, ie);
        IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
        lc_γ_to(ir, sub);
        ir_operand_push(sub, cur); ir_operand_push(sub, ir);
        cur = sub;
    }
    if (var_res) *var_res = cur;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int ir_is_generator_kind(IR_e t);
static void sno_ω_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_resume_ω_to(IR_graph_t * g, int tail_idx, IR_t * nd, IR_t * t) {
    /* SN4-PAT-CAPTURE-STACK: re-point nd's exhaust-ω at a left generator t — but a capture COND's ω is
     * ALREADY the capture's inward resume edge (inner generator's β, or SAVE's pop); clobbering it would
     * sever the capture's own chain.  The capture's OUTWARD exhaust is its SAVE's ω (β pops, then ω), so
     * chain through operands[1] instead.  Everything else re-points directly (the pre-stack behaviour). */
    /* ALT-EXHAUST CHASE (2026-07-08 s7, found via the word1/word3/cap4 capture-then-alternation bracket):
     * an ALTERNATE construct's leftward exhaust is NOT its first-allocated save's ω (a dead edge) — it is
     * the trailing T-join's γ ("T reload arm exhausts leftward", TT_ALT), baked to the seq's `fail` at
     * build time.  Re-pointing save.ω therefore never resumed the left generator: 'A' ARB . V ('B'|'C')
     * exhausted both alternatives straight to HEAD, ARB never extended, the whole match wrongly failed
     * (oracle: succeeds, V='Y').  The trailing T is the node allocated immediately after the save
     * (TT_ALT allocates save then join with nothing between) with operands[0]==save — chase it and
     * re-point ITS γ, β-aware, exactly the edge TT_ALT itself aimed at `fail`. */
    if (nd && nd->op == IR_MATCH_ALTERNATE && nd->n_operands == 0 && g && tail_idx + 1 < g->n) {
        IR_t * T = g->all[tail_idx + 1];
        if (T && T->op == IR_MATCH_ALTERNATE && T->n_operands > 0 && T->operands[0] == nd) {
            if (t && ir_is_generator_kind(t->op)) lc_γ_to_β(T, t); else lc_γ_to(T, t);
            return;
        }
    }
    /* ARBNO-EXHAUST CHASE (2026-07-10 s14, found via the capture-then-ARBNO bracket, minimal probe
     * `'ab' ? POS(0) LEN(1) . V ARBNO(LEN(1)) RPOS(0)` — oracle succeeds V='a', SCRIP wrongly failed):
     * an ARBNO construct's first-allocated node is G, and G's ω is the REPURPOSED body-entry edge (the
     * TT_ARBNO arm's own comment) — the direct fallthrough repoint below CLOBBERED the body entry, so an
     * extension attempt jumped into the left generator's β instead of the body and the ARBNO could never
     * grow past its null yield (capture-then-ARB was fine: ARB's ω is a true fail edge — the discriminating
     * probe).  ARBNO's true leftward exhaust is F.ω (phase-2/5 exhaust box, allocated immediately after G
     * with operands[0]==G, mirroring the ALT arm's T identification) — chase and re-point THAT, β-aware,
     * exactly the edge the TT_ARBNO arm itself aimed at `fail`.  This is the third member of the class the
     * two arms above founded: constructs whose first-allocated ω is not their leftward exhaust. */
    if (nd && nd->op == IR_MATCH_ARBNO && g && tail_idx + 1 < g->n) {
        IR_t * Fx = g->all[tail_idx + 1];
        if (Fx && Fx->op == IR_MATCH_ARBNO && Fx->n_operands > 0 && Fx->operands[0] == nd) {
            sno_ω_to(Fx, t);
            return;
        }
    }
    if (nd && nd->op == IR_MATCH_ASSIGN_COND && nd->n_operands > 1 && nd->operands[1]) nd = nd->operands[1];
    sno_ω_to(nd, t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_fence(const tree_t * t) { return t && ((t->t == TT_FENCE) || (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "FENCE"))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_seq_flatten_pat(const tree_t * t, const tree_t ** elems, int * ne) {
    if (!t) return;
    if (t->t == TT_SEQ) { sno_seq_flatten_pat((t->n > 0) ? t->c[0] : NULL, elems, ne); sno_seq_flatten_pat((t->n > 1) ? t->c[1] : NULL, elems, ne); return; }
    if (*ne >= 128) sno_fatal("pattern sequence too long (SN4-PAT cap 128)", NULL);
    elems[(*ne)++] = t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_deterministic(const tree_t * t) {
    /* ZB-5 v1 gate: a body element that can yield MORE THAN ONE way (a generator, or an alternation whose
     * re-choice a completed iteration would need) makes ARBNO backtrack non-total — that needs the
     * per-iteration COLLECTION (v2).  Deterministic subtrees exhaust totally, so no iteration state is kept. */
    if (!t) return 1;
    if (t->t == TT_ALT || t->t == TT_ARB || t->t == TT_ARBNO || sno_is_fence(t)) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "ARB")) return 0;
    for (int i = 0; i < t->n; i++) if (!sno_pat_deterministic(t->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_is_arb(const tree_t * t) { return t && (t->t == TT_ARB || (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "ARB"))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_v2_tail_gen(const tree_t * t) {
    /* ZB-5 v2: the exhaust box resumes the body via F.γ → the body's FIRST-ALLOCATED node's β — the
     * rightmost leaf.  That leaf must therefore expose a resume surface: an alternation (SAVE.β dispatch),
     * ARB (β extend), or a capture whose COND.ω forwards to resume-or-pop.  A deterministic rightmost leaf
     * has no β (chaining THROUGH it is the Finding-B pass-through rung, not this one) — gate it. */
    if (!t) return 0;
    if (t->t == TT_ALT || sno_pat_is_arb(t)) return 1;
    if (t->t == TT_SEQ || t->t == TT_CAT) return t->n > 1 && sno_pat_v2_tail_gen(t->c[1]);
    if (t->t == TT_CAPT_COND_ASGN) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_v2_ok(const tree_t * t) {
    /* ZB-5 v2 body admission: ALT and ARB anywhere EXCEPT nested inside an alternative (the ALT-RESUME mark
     * records the NEXT alternative, so an inner generator's remaining ways would be skipped on resume);
     * nested ARBNO refused (an element's prev_rZ would point into a REALLOC-MOVABLE outer collection —
     * the RELOAD-LAW escape); FENCE refused (its seal jumps to the statement fail with rZ still repointed). */
    if (!t) return 1;
    if (t->t == TT_ARBNO || sno_is_fence(t)) return 0;
    if (t->t == TT_ALT) { const tree_t * l = (t->n > 0) ? t->c[0] : NULL; const tree_t * r = (t->n > 1) ? t->c[1] : NULL; return ((l && l->t == TT_ALT) ? sno_pat_v2_ok(l) : sno_pat_deterministic(l)) && sno_pat_deterministic(r); }
    for (int i = 0; i < t->n; i++) if (!sno_pat_v2_ok(t->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_fence_inner_ok(const tree_t * t) {
    /* v1-FENCE inner admission (2026-07-10): what may live INSIDE a FENCE(P) riding as an ARBNO body element.
     * Forward alternation (ALT), internal generator retry (ARB), SEQ, and every deterministic leaf are fine —
     * the v1 shared-slot frame provides the seal STRUCTURALLY (each extension re-enters the body α fresh; no
     * resume edge is ever stamped into the body from outside, so a committed iteration's ALT dispatch slot is
     * unreachable — SPITBOL manual Ch.19 FENCE(P): alternatives visible only moving forward, backup passes
     * through).  REFUSED (park, not fake): nested ARBNO (slot interaction unproven), nested/bare FENCE (bare
     * FENCE's backup semantics = whole-match fail, a DIFFERENT target), captures (capture-stack x seal unproven). */
    if (!t) return 0;
    if (t->t == TT_ARBNO || sno_is_fence(t)) return 0;
    if (t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN || t->t == TT_CAPT_CURSOR) return 0;
    if (t->t == TT_ALT || t->t == TT_SEQ) { for (int i = 0; i < t->n; i++) if (!sno_fence_inner_ok(t->c[i])) return 0; return 1; }
    if (t->t == TT_ARB || (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "ARB"))) return 1;
    return sno_pat_deterministic(t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_v1_fenced_ok(const tree_t * t) {
    /* v1-FENCE ARBNO body admission (2026-07-10, closes 116/142/151; any-position 145-shape same day): a body
     * whose non-deterministic elements are ALL FENCE(P) function-forms rides ARBNO v1 (phases 0/1/2)
     * UNCHANGED.  WHY v1 IS THE FENCE'S HOME: FENCE(P) commit semantics is EXACTLY v1's structural property —
     * one shared body frame, completed iterations unreachable; the v2 COLLECTION exists to enable the resume
     * the fence must PREVENT (v2's F.β body-resume is the leak, so fenced bodies must NOT ride v2).  This
     * answers the standing design question (which fail-target does a sealed element inside iteration N point
     * at): NEITHER the statement fJ NOR a local retry — the fenced group is deterministic-from-outside (β≡ω),
     * so its exhaust points where every v1 body element's fail already points: F (extend-failed = ARBNO
     * exhausted, shorter yields were already offered).  Fences may sit at ANY body position: the spine's seal
     * target is cx->pat_seal (== cx->pat_fail at statement level, == F while lowering an ARBNO body — set and
     * restored in the TT_ARBNO arm), so an element right of an in-body fence fails to F, not the statement. */
    const tree_t * elems[128]; int ne = 0; int nf = 0;
    if (!t) return 0;
    sno_seq_flatten_pat(t, elems, &ne);
    if (ne < 1) return 0;
    for (int i = 0; i < ne; i++) {
        const tree_t * e = elems[i];
        if (e && e->t == TT_FENCE && e->n > 0 && e->c[0]) { if (!sno_fence_inner_ok(e->c[0])) return 0; nf++; continue; }
        if (!sno_pat_deterministic(e)) return 0;
    }
    return nf > 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SNO_CCONST_MAX 256
static const char * sno_cset_fold(const tree_t * a);
static struct { const char * name; const char * val; int total; int clean; } g_sno_cconst[SNO_CCONST_MAX];
static int g_sno_ncconst = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cconst_write(const char * name, const char * litval) {
    if (!name) return;
    int i = 0; for (; i < g_sno_ncconst; i++) if (!strcmp(g_sno_cconst[i].name, name)) break;
    if (i == g_sno_ncconst) { if (g_sno_ncconst >= SNO_CCONST_MAX) return; g_sno_cconst[i].name = name; g_sno_cconst[i].val = NULL; g_sno_cconst[i].total = 0; g_sno_cconst[i].clean = 0; g_sno_ncconst++; }
    g_sno_cconst[i].total++;
    if (litval) { g_sno_cconst[i].clean++; g_sno_cconst[i].val = litval; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cconst_scan_writes(const tree_t * t) {
    if (!t) return;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) sno_cconst_write(t->c[1]->v.sval, NULL);
    if (t->t == TT_SWAP) for (int i = 0; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR) sno_cconst_write(t->c[i]->v.sval, NULL);
    for (int i = 0; i < t->n; i++) sno_cconst_scan_writes(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cconst_scan_indirect_target(const tree_t * t) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval) { sno_cconst_write(t->v.sval, NULL); return; }
    for (int i = 0; i < t->n; i++) sno_cconst_scan_indirect_target(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cconst_note_define_names(const sno_def_t * d) { for (int k = 0; k < d->nnames; k++) sno_cconst_write(d->names[k], NULL); if (d->fname) sno_cconst_write(d->fname, NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cconst_build_table(const tree_t ** st, int nst) {
    g_sno_ncconst = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i]; const tree_t * subj = lc_stmt_subj(s); const tree_t * pat = sfind_expr(s, ":pat"); const tree_t * repl = sfind_expr(s, ":repl"); int has_eq = sfind(s, ":eq") != NULL;
        if (pat) sno_cconst_scan_writes(pat);
        if (subj && subj->t == TT_SCAN && subj->n > 1) sno_cconst_scan_writes(subj->c[1]);
        if (subj && subj->t == TT_DEFINE && subj->n > 1 && subj->c[1] && subj->c[1]->t == TT_QLIT && subj->c[1]->v.sval) { sno_def_t d; sno_parse_define(subj->c[1]->v.sval, NULL, &d); sno_cconst_note_define_names(&d); continue; }
        int argbase = 0; const tree_t * dsub = sno_stmt_define(s, &argbase);
        if (dsub) { sno_def_t d; sno_parse_define(dsub->c[argbase]->v.sval, NULL, &d); sno_cconst_note_define_names(&d); continue; }
        if (!has_eq) continue;
        if (subj && subj->t == TT_VAR && subj->v.sval) { const char * fv = repl ? sno_cset_fold(repl) : NULL; sno_cconst_write(subj->v.sval, fv); }
        else if (subj) sno_cconst_scan_indirect_target(subj);
        if (repl) sno_cconst_scan_writes(repl);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_cconst_lookup(const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < g_sno_ncconst; i++) if (!strcmp(g_sno_cconst[i].name, name)) return (g_sno_cconst[i].total == 1 && g_sno_cconst[i].clean == 1) ? g_sno_cconst[i].val : NULL;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_cset_fold(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT) return a->v.sval ? a->v.sval : "";
    if (a->t == TT_KEYWORD && a->v.sval) {
        if (!strcmp(a->v.sval, "LCASE") || !strcmp(a->v.sval, "lcase")) return "abcdefghijklmnopqrstuvwxyz";
        if (!strcmp(a->v.sval, "UCASE") || !strcmp(a->v.sval, "ucase")) return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (a->t == TT_VAR && a->v.sval) return sno_cconst_lookup(a->v.sval);
    if (a->t == TT_SEQ || a->t == TT_CAT) {
        const char * l = sno_cset_fold((a->n > 0) ? a->c[0] : NULL); if (!l) return NULL;
        const char * r = sno_cset_fold((a->n > 1) ? a->c[1] : NULL); if (!r) return NULL;
        size_t ln = strlen(l), rn = strlen(r); char * buf = (char *) malloc(ln + rn + 1); if (!buf) return NULL;
        memcpy(buf, l, ln); memcpy(buf + ln, r, rn); buf[ln + rn] = 0; return buf;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_pattern_rhs(const tree_t * t);
static int sno_pat_supported(const tree_t * t);
static const char * sno_pat_collect(const tree_t * pat);
static struct { const char * var; const char * procname; const tree_t * pat; } g_sno_fz[SNO_PAT_MAX];
static int g_sno_nfz = 0;
static int g_sno_fz_unsafe = 0;
static int g_sno_in_patproc = 0;
static const char * g_sno_fzw_name[SNO_CCONST_MAX];
static int g_sno_fzw_cnt[SNO_CCONST_MAX];
static int g_sno_nfzw = 0;
static void sno_fz_write(const char * nm) {
    if (!nm || !nm[0]) return;
    int i = 0; for (; i < g_sno_nfzw; i++) if (!strcmp(g_sno_fzw_name[i], nm)) break;
    if (i == g_sno_nfzw) { if (g_sno_nfzw >= SNO_CCONST_MAX) { g_sno_fz_unsafe = 1; return; } g_sno_fzw_name[i] = nm; g_sno_fzw_cnt[i] = 0; g_sno_nfzw++; }
    g_sno_fzw_cnt[i]++;
}
static int sno_fz_wrcount(const char * nm) { for (int i = 0; i < g_sno_nfzw; i++) if (!strcmp(g_sno_fzw_name[i], nm)) return g_sno_fzw_cnt[i]; return 0; }
static void sno_fz_scan(const tree_t * t) {
    if (!t) return;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) sno_fz_write(t->c[1]->v.sval);
    if (t->t == TT_CAPT_CURSOR && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) sno_fz_write(t->c[0]->v.sval);
    if (t->t == TT_SWAP || t->t == TT_REVSWAP) for (int i = 0; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR) sno_fz_write(t->c[i]->v.sval);
    if (t->t == TT_FNC && t->v.sval) { const char * fn = t->v.sval;
        if (!strcmp(fn, "EVAL") || !strcmp(fn, "eval") || !strcmp(fn, "CODE") || !strcmp(fn, "code") || !strcmp(fn, "CONVERT") || !strcmp(fn, "convert")) g_sno_fz_unsafe = 1;
        if (!strcmp(fn, "INPUT") || !strcmp(fn, "input") || !strcmp(fn, "CLEAR") || !strcmp(fn, "clear")) g_sno_fz_unsafe = 1; }
    for (int i = 0; i < t->n; i++) sno_fz_scan(t->c[i]);
}
static int sno_pat_invariant(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_QLIT) return 1;
    if (t->t == TT_ANY || t->t == TT_NOTANY || t->t == TT_SPAN || t->t == TT_BREAK || t->t == TT_BREAKX) return sno_cset_fold((t->n > 0) ? t->c[0] : NULL) != NULL;
    if (t->t == TT_LEN || t->t == TT_TAB || t->t == TT_RTAB || t->t == TT_POS || t->t == TT_RPOS) return t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT;
    if (t->t == TT_REM || t->t == TT_ARB) return 1;
    if (t->t == TT_VAR) return t->v.sval && (!strcmp(t->v.sval, "REM") || !strcmp(t->v.sval, "ARB") || !strcmp(t->v.sval, "FENCE"));
    if (t->t == TT_FENCE) return t->n == 0 || sno_pat_invariant(t->c[0]);
    if (t->t == TT_ARBNO) return t->n > 0 && sno_pat_invariant(t->c[0]);
    if (t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) return t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && sno_pat_invariant(t->c[0]);
    if (t->t == TT_CAPT_CURSOR) return t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval != NULL;
    if (t->t == TT_SEQ || t->t == TT_ALT) return sno_pat_invariant((t->n > 0) ? t->c[0] : NULL) && sno_pat_invariant((t->n > 1) ? t->c[1] : NULL);
    return 0;
}
static void sno_fz_mark_defer(IR_graph_t * g, IR_t * nd, const char * nm) {
    if (g_sno_in_patproc || !nm) return;
    for (int i = 0; i < g_sno_nfz; i++) if (!strcmp(g_sno_fz[i].var, nm)) {
        IR_t * pl = lc_build(g, IR_LIT_STRING, NULL, NULL); IR_LIT(pl).sval = (char *) g_sno_fz[i].procname; ir_operand_push(nd, pl); return; }
}
static void sno_fz_build_table(const tree_t ** st, int nst) {
    g_sno_nfz = 0; g_sno_fz_unsafe = 0; g_sno_nfzw = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i]; if (!s) continue;
        const tree_t * subj = lc_stmt_subj(s); const tree_t * pat = sfind_expr(s, ":pat"); const tree_t * repl = sfind_expr(s, ":repl"); int has_eq = sfind(s, ":eq") != NULL;
        if (pat) sno_fz_scan(pat);
        if (repl) sno_fz_scan(repl);
        if (subj) sno_fz_scan(subj);
        if (subj && subj->t == TT_DEFINE && subj->n > 1 && subj->c[1] && subj->c[1]->t == TT_QLIT && subj->c[1]->v.sval) {
            sno_def_t d; sno_parse_define(subj->c[1]->v.sval, NULL, &d); for (int k = 0; k < d.nnames; k++) sno_fz_write(d.names[k]); if (d.fname) sno_fz_write(d.fname); continue; }
        int argbase = 0; const tree_t * dsub = sno_stmt_define(s, &argbase);
        if (dsub) { sno_def_t d; sno_parse_define(dsub->c[argbase]->v.sval, NULL, &d); for (int k = 0; k < d.nnames; k++) sno_fz_write(d.names[k]); if (d.fname) sno_fz_write(d.fname); continue; }
        if (!has_eq) continue;
        if (subj && subj->t == TT_SCAN) { const tree_t * sv = (subj->n > 0) ? subj->c[0] : NULL; if (sv && sv->t == TT_VAR && sv->v.sval) sno_fz_write(sv->v.sval); else g_sno_fz_unsafe = 1; continue; }
        if (subj && subj->t == TT_VAR && subj->v.sval) {
            sno_fz_write(subj->v.sval);
            if (repl && g_sno_nfz < SNO_PAT_MAX && sno_is_pattern_rhs(repl) && sno_pat_supported(repl) && sno_pat_invariant(repl)) {
                g_sno_fz[g_sno_nfz].var = subj->v.sval; g_sno_fz[g_sno_nfz].pat = repl; g_sno_fz[g_sno_nfz].procname = NULL; g_sno_nfz++; }
            continue; }
        if (subj && subj->t == TT_KEYWORD) continue;
        if (subj && subj->t == TT_IDX) continue;
        if (subj) g_sno_fz_unsafe = 1;
    }
    int keep = 0;
    for (int i = 0; i < g_sno_nfz; i++) if (!g_sno_fz_unsafe && sno_fz_wrcount(g_sno_fz[i].var) == 1) { g_sno_fz[keep] = g_sno_fz[i]; g_sno_fz[keep].procname = sno_pat_collect(g_sno_fz[i].pat); keep++; }
    g_sno_nfz = g_sno_fz_unsafe ? 0 : keep;
    if (getenv("SCRIP_FZ_DEBUG")) { fprintf(stderr, "[FZ5] unsafe=%d inlinable=%d\n", g_sno_fz_unsafe, g_sno_nfz);
        for (int i = 0; i < g_sno_nfz; i++) fprintf(stderr, "[FZ5]  %s -> %s\n", g_sno_fz[i].var, g_sno_fz[i].procname); }
}

static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail) {
    IR_graph_t * g = cx->g;
    if (!t) return succ;
    switch (t->t) {
    case TT_QLIT: {
        IR_t * nd = lc_build(g, IR_MATCH_LIT, succ, NULL);
        sno_ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) "";
        return nd;
    }
    case TT_ANY: case TT_NOTANY: {
        IR_t * nd = lc_build(g, (t->t == TT_ANY) ? IR_MATCH_ANY : IR_MATCH_NOTANY, succ, NULL);
        sno_ω_to(nd, fail);
        const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
        if (cs) IR_LIT(nd).sval = (char *) cs;
        else sno_fatal("ANY/NOTANY with a non-literal charset is outside the SN4-PAT subset", NULL);
        return nd;
    }
    case TT_SPAN: {
        IR_t * nd = lc_build(g, IR_MATCH_SPAN, succ, NULL);
        sno_ω_to(nd, fail);
        const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
        if (cs) IR_LIT(nd).sval = (char *) cs;
        else sno_fatal("SPAN with a non-literal charset is outside the SN4-PAT subset", NULL);
        return nd;
    }
    case TT_BREAK: case TT_BREAKX: {
        IR_t * nd = lc_build(g, (t->t == TT_BREAK) ? IR_MATCH_BREAK : IR_MATCH_BREAKX, succ, NULL);
        sno_ω_to(nd, fail);
        const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
        if (cs) IR_LIT(nd).sval = (char *) cs;
        else sno_fatal("BREAK/BREAKX with a non-literal charset is outside the SN4-PAT subset", NULL);
        return nd;
    }
    case TT_TAB: case TT_RTAB: {
        IR_t * nd = lc_build(g, (t->t == TT_TAB) ? IR_MATCH_TAB : IR_MATCH_RTAB, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n <= 0 || !t->c[0]) sno_fatal("TAB/RTAB requires a count argument", NULL);
        IR_t * argval = NULL;
        IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval);
        ir_operand_push(nd, argval);
        return arg_entry;
    }
    case TT_POS: case TT_RPOS: {
        IR_t * nd = lc_build(g, IR_MATCH_POS, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->t == TT_RPOS) IR_LIT(nd).sval = (char *) "r";
        if (t->n <= 0 || !t->c[0]) sno_fatal("POS/RPOS requires a position argument", NULL);
        IR_t * argval = NULL;
        IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval);
        ir_operand_push(nd, argval);
        return arg_entry;
    }
    case TT_FENCE:
        return (t->n > 0 && t->c[0]) ? sno_pat_node(cx, t->c[0], succ, fail) : succ;
    case TT_DEFER: {
        const tree_t * in = (t->n > 0) ? t->c[0] : NULL;
        if (in && in->t == TT_VAR && in->v.sval) { IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = (char *) in->v.sval; sno_fz_mark_defer(g, nd, in->v.sval); sno_ω_to(nd, fail); return nd; }
        { const char * bn = sno_expr_collect(in); char pb[40]; snprintf(pb, sizeof pb, "*%s", bn);
          IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = lp_strdup(pb); sno_ω_to(nd, fail); return nd; }
    }
    case TT_VAR: {
        const char * nm = t->v.sval;
        if (nm && !strcmp(nm, "REM")) { IR_t * nd = lc_build(g, IR_MATCH_REM, succ, NULL); sno_ω_to(nd, fail); return nd; }
        if (nm && !strcmp(nm, "ARB")) { IR_t * nd = lc_build(g, IR_MATCH_ARB, succ, NULL); sno_ω_to(nd, fail); return nd; }
        if (nm && !strcmp(nm, "FENCE")) return succ;
        { IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = (char *) nm; sno_fz_mark_defer(g, nd, nm); sno_ω_to(nd, fail); return nd; }
    }
    case TT_REM: {
        IR_t * nd = lc_build(g, IR_MATCH_REM, succ, NULL);
        sno_ω_to(nd, fail);
        return nd;
    }
    case TT_CAPT_CURSOR: {
        const tree_t * tgt = (t->n > 0) ? t->c[0] : NULL;
        if (!tgt || tgt->t != TT_VAR || !tgt->v.sval) sno_fatal("@ cursor-position capture target is not a simple variable", NULL);
        sno_reg_var(tgt->v.sval);
        IR_t * nd = lc_build(g, IR_MATCH_ATP, succ, NULL);
        IR_LIT(nd).sval = (char *) tgt->v.sval;
        sno_ω_to(nd, fail);
        return nd;
    }
    case TT_ARB: {
        IR_t * nd = lc_build(g, IR_MATCH_ARB, succ, NULL);
        sno_ω_to(nd, fail);
        return nd;
    }
    case TT_ARBNO: {
        /* ZB-5 SN4-PAT ARBNO v1 (deterministic body).  Three phases share IR_MATCH_ARBNO, IR_LIT.ival = phase.
         * G (0, generator, no operands; MUST be first-allocated so TT_SEQ's tail rule finds the β surface):
         *   α saves entry+yield cursors and jmps γ (null yield — SPITBOL shortest-first); β restores δ=yield,
         *   records cur_before, jmps ω — G's ω is REPURPOSED as the body-entry edge (the β-continuation); the
         *   construct's real fail exit lives on F.  K (1, operand[0]=G): body-success landing — null-progress
         *   guard (δ==cur_before → ω=F, the 4/28 zero-advance rule), else yield=δ, jmp γ (yield one more).
         * F (2, operand[0]=G): exhaust — δ=entry, jmp ω (outer fail); its template defines a β alias because
         *   body leaves stamp their fail edges via sno_ω_to and IR_MATCH_ARBNO is generator-kind.
         * Deterministic bodies exhaust totally (a completed iteration cannot re-choose), so NO per-iteration
         * COLLECTION is needed — that is the generator-body v2 requirement (ALT/ARB/ARBNO/FENCE inside). */
        if (!(t->n > 0) || !t->c[0]) sno_fatal("ARBNO requires a pattern argument", NULL);
        int v1f = sno_pat_v1_fenced_ok(t->c[0]);
        int a2 = v1f ? 0 : !sno_pat_deterministic(t->c[0]);
        if (a2 && !(sno_pat_v2_ok(t->c[0]) && sno_pat_v2_tail_gen(t->c[0]))) sno_fatal("ARBNO body outside the ZB-5 v2 subset (generator body needs a rightmost ALT/ARB/capture leaf; fenced generator bodies ride v1 only as a single rightmost FENCE(P); nested ARBNO and a generator inside an alternative are v3)", NULL);
        /* v2 (a2): phases 3/4/5 — per-iteration COLLECTION (ARCH-ZETA-LOCAL-STORAGE.md section 5f).  G.β pushes a
         * zeroed element {prev_rZ, cur_before} + body slot range and REPOINTS rZ into it, so body boxes' [r12+off]
         * become per-iteration; K reads the header, restores rZ, counts and yields (zero-advance → F.α, which
         * re-repoints and resumes THIS element's body β); F.β (body-fail) restores rZ, pops (i==0 → exhaust) and
         * resumes element i's body tail β via F.γ — stamped at the body's first-allocated (rightmost) leaf.
         * operands[0]/[1] on G bracket the body subgraph by allocation for the zls geometry post-pass. */
        IR_t * G = lc_build(g, IR_MATCH_ARBNO, succ, NULL); IR_LIT(G).ival = a2 ? 3 : 0;
        IR_t * F = lc_build(g, IR_MATCH_ARBNO, NULL, NULL); IR_LIT(F).ival = a2 ? 5 : 2; ir_operand_push(F, G); sno_ω_to(F, fail);
        IR_t * K = lc_build(g, IR_MATCH_ARBNO, succ, F);    IR_LIT(K).ival = a2 ? 4 : 1; ir_operand_push(K, G);
        int bi0 = g->n;
        /* v1-FENCE seal scope (2026-07-10): while the body lowers, an in-body fence's seal target is F —
         * a sealed element failing rightward exhausts THIS ARBNO (correct: shorter yields were already
         * offered), never the statement's pat_fail (the line-528 bug this replaces).  Save/restore, so
         * fences in the statement spine around the ARBNO keep the statement-level target. */
        IR_t * prev_seal = cx->pat_seal; cx->pat_seal = F;
        IR_t * be = sno_pat_node(cx, t->c[0], K, F);
        cx->pat_seal = prev_seal;
        lc_ω_to(G, be);
        if (a2) {
            if (bi0 >= g->n) sno_fatal("ARBNO v2 internal: body lowered to zero nodes", NULL);
            ir_operand_push(G, g->all[bi0]); ir_operand_push(G, g->all[g->n - 1]);
            IR_t * btail = g->all[bi0];
            if (ir_is_generator_kind(btail->op)) lc_γ_to_β(F, btail); else lc_γ_to(F, btail);
        }
        return G;
    }
    case TT_LEN: {
        IR_t * nd = lc_build(g, IR_MATCH_LEN, succ, NULL);
        sno_ω_to(nd, fail);
        long long n = 0;
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT) n = t->c[0]->v.ival;
        else sno_fatal("LEN with a non-literal count is outside the SN4-PAT-2 subset", NULL);
        IR_LIT(nd).ival = n;
        return nd;
    }
    case TT_CAPT_COND_ASGN: {
        const char * vn = (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) ? t->c[1]->v.sval : NULL;
        if (!vn || !(t->n > 0 && t->c[0])) sno_fatal("conditional capture target is not a simple variable (SN4-PAT-2 subset)", NULL);
        sno_reg_var(vn);
        /* SN4-PAT-CAPTURE-STACK (Lon directive 2026-07-05): capture spans [start-of-inner, current) on a
         * per-box STACK — SAVE.α pushes the open cursor, SAVE.β pops it, the COND at every inner yield
         * assigns from the top-of-stack frame — so the β-resume chain survives a generator between the
         * capture's open and close.  COND is allocated FIRST (first-allocated = the capture's tail for
         * TT_SEQ's re-point rule; both ops are generator-kind so re-points land β-wards).  The inner is
         * lowered with fail = SAVE: an inner exhaust lands on SAVE.β (pop, then ω → the capture's fail,
         * β-aware so a generator further left still resumes).  COND.ω is the backtrack-IN edge: a failing
         * right neighbour lands on COND.β (jmp ω) and resumes the inner generator's β — or, for a
         * deterministic inner, goes straight to SAVE.β (pop + fail leftward, the pre-stack destination). */
        IR_t * nd = lc_build(g, IR_MATCH_ASSIGN_COND, succ, NULL);  /* phase-1 COND, γ → succ */
        IR_LIT(nd).sval = (char *) vn;
        IR_t * save = lc_build(g, IR_MATCH_ASSIGN_SAVE, NULL, NULL); /* phase-0 SAVE: α push, β pop */
        IR_LIT(save).sval = (char *) vn;                           /* template's op_sval[0] guard */
        sno_ω_to(save, fail);                                      /* pop path exits to the capture's fail */
        int before_i = g->n;
        IR_t * pe = sno_pat_node(cx, t->c[0], nd, save);           /* inner pattern, γ → COND, fail → SAVE.β */
        IR_t * itail = (before_i < g->n) ? g->all[before_i] : pe;  /* inner rightmost leaf (first allocated) */
        lc_γ_to(save, pe);                                         /* SAVE.γ → inner entry */
        sno_ω_to(nd, ir_is_generator_kind(itail->op) ? itail : save); /* COND backtrack-in: resume or pop */
        ir_operand_push(nd, pe);                                   /* [0] inner entry */
        ir_operand_push(nd, save);                                 /* [1] SAVE → COND.op_off = save's slot */
        return save;                                               /* capture entry is the SAVE node */
    }
    case TT_CAPT_IMMED_ASGN: {
        const char * vn = (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) ? t->c[1]->v.sval : NULL;
        if (!vn || !(t->n > 0 && t->c[0])) sno_fatal("immediate capture target is not a simple variable (SN4-PAT-2 subset)", NULL);
        sno_reg_var(vn);
        /* $ immediate assignment: SAME span/capture-stack shape as . (TT_CAPT_COND_ASGN) above — the only
         * difference is IR_MATCH_ASSIGN_IMM vs _COND, which bb_match_capture()'s op_phase (2 vs 1) turns into
         * is_imm passed to rt_cap_assign_cursor.  Per the manual: $ commits at every inner yield regardless of
         * whether the overall match later fails; . is the same wiring, the outcome-dependence lives in rt_*. */
        IR_t * nd = lc_build(g, IR_MATCH_ASSIGN_IMM, succ, NULL);
        IR_LIT(nd).sval = (char *) vn;
        IR_t * save = lc_build(g, IR_MATCH_ASSIGN_SAVE, NULL, NULL);
        IR_LIT(save).sval = (char *) vn;
        sno_ω_to(save, fail);
        int before_i = g->n;
        IR_t * pe = sno_pat_node(cx, t->c[0], nd, save);
        IR_t * itail = (before_i < g->n) ? g->all[before_i] : pe;
        lc_γ_to(save, pe);
        sno_ω_to(nd, ir_is_generator_kind(itail->op) ? itail : save);
        ir_operand_push(nd, pe);
        ir_operand_push(nd, save);
        return save;
    }
    case TT_SEQ: {
        /* SN4-PAT-3h CAT: pattern concatenation A B — node-free in the live single-HEAD
         * design (the parked IR_MATCH_SEQUENCE was a subgraph success-sink; here success threads
         * straight to `succ`).  Wire A.γ → B.α → succ by lowering right-first, then left with
         * succ = right's entry.  Failure: a deterministic element's ω already points at `fail`
         * ( = head = retry-position, correct SNOBOL4 for SPAN/BREAK/LEN/… which never back off).
         * The ONLY resumable leaf today is ARB (a generator): if the left element is a
         * generator, the right element's failure must resume it (β) rather than advance the
         * whole attempt, so re-point right's tail-ω at the left tail via sno_ω_to (β-aware). */
        /* SN4-PAT FENCE: a fence in the spine seals — every element to its RIGHT fails to the
         * statement-level cx->pat_fail (no HEAD retry, no left-generator resume) instead of `fail`
         * (= HEAD).  Fence is node-free and transparent forward.  Fence-free sequences keep the
         * untouched 2-way path (zero behavioural change for every landed matcher). */
        /* SN4-PAT GROUP-TRANSPARENT SEQ (2026-07-08 s8, word3/cross bracket): concatenation is associative —
         * (A B) C ≡ A B C — so a parenthesized fence-free group MUST NOT be an opaque element: the old 2-way
         * branch took the right construct's resume surface as its FIRST-ALLOCATED node, which for a right-nested
         * group is its RIGHTMOST leaf; the group's LEFTMOST element's fail edge stayed at HEAD, so the left
         * generator never resumed ('AB CD' ? ARB . B (' ' 'C') "succeeded" at the slid anchor with B='').
         * Fix: recursively flatten every nested TT_SEQ into ONE element list and let the single pairwise loop
         * wire every seam uniformly (fences inside plain groups now also seal at their true spine position). */
        const tree_t * elems[128]; int ne = 0;
        sno_seq_flatten_pat(t, elems, &ne);
        int first_fence = ne;
        for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { first_fence = i; break; }
        IR_t * cur_succ = succ; IR_t * right_tail = NULL; int right_tail_idx = -1; int right_sealed = 0;
        for (int i = ne - 1; i >= 0; i--) {
            if (sno_is_fence(elems[i])) {                                           /* seals everything to its right; the element to its left cannot resume into it */
                right_sealed = 1;
                const tree_t * inner = (elems[i]->t == TT_FENCE && elems[i]->n > 0) ? elems[i]->c[0] : NULL;
                if (inner) {                                                        /* FENCE(P): lower P with the pre-seal fail so P retries normally on forward-fail; the seal blocks re-entry after success */
                    IR_t * fail_p = (i > first_fence) ? cx->pat_seal : fail;
                    int before_p = g->n;
                    IR_t * pe = sno_pat_node(cx, inner, cur_succ, fail_p);
                    IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
                    cur_succ = pe; right_tail = p_tail; right_tail_idx = before_p;
                }
                continue;
            }
            IR_t * fail_i = (i > first_fence) ? cx->pat_seal : fail;                 /* right of the fence: cut to the SEAL target (== statement-fail at top level, == F/exhaust inside an ARBNO body), never HEAD */
            int before_e = g->n;
            IR_t * ee = sno_pat_node(cx, elems[i], cur_succ, fail_i);
            IR_t * e_tail = (before_e < g->n) ? g->all[before_e] : ee;
            if (right_tail && !right_sealed && before_e < g->n && ir_is_generator_kind(e_tail->op)) sno_resume_ω_to(g, right_tail_idx, right_tail, e_tail);
            cur_succ = ee; right_tail = e_tail; right_tail_idx = before_e; right_sealed = 0;
        }
        return cur_succ;
    }
    case TT_ALT: {
        /* SN4-PAT-3h ALTERNATE (A | B | C).  Flatten the left-associative TT_ALT spine into a
         * left-to-right list, then wire: a phase-0 SAVE records the entry cursor; alternative i
         * fails to a phase-1 RESTORE that reloads the cursor and jumps to alternative i+1; the last
         * alternative fails to the outer `fail`; every alternative succeeds to `succ`. */
        const tree_t * alts[64]; int na = 0;
        const tree_t * rstack[64]; int nr = 0;
        const tree_t * cur = t;
        while (cur && cur->t == TT_ALT) {
            if (nr >= 64) sno_fatal("alternation with too many branches (SN4-PAT-3h cap 64)", NULL);
            rstack[nr++] = (cur->n > 1) ? cur->c[1] : NULL;
            cur = (cur->n > 0) ? cur->c[0] : NULL;
        }
        alts[na++] = cur;                                   /* leftmost */
        for (int i = nr - 1; i >= 0; i--) alts[na++] = rstack[i];
        if (na == 1) return sno_pat_node(cx, alts[0], succ, fail);
        /* ALT-RESUME (ZB-5 v2 prerequisite): each alternative i gets ONE join box J_{i+1} — its α is the
         * MARK (alternative i just succeeded: record J's own reload arm as the resume continuation in the
         * SAVE slot, jmp ω = outer succ), its β/L(0) is reload-cursor-and-try-alternative-i+1 (forward
         * fails land β-wards via sno_ω_to since ALTERNATE is generator-kind).  A trailing T = J_n is the
         * SAME box with γ = the outer fail (resume exhaust: restore entry cursor, fail leftward, β-aware).
         * A failing RIGHT neighbour lands SAVE.β (the construct tail is first-allocated) which dispatches
         * `jmp [slot+8]` — replaying exactly where forward-failure of the succeeded alternative would have
         * gone.  Residual (documented, gated for ARBNO v2 bodies): a generator nested INSIDE an alternative
         * has its remaining ways skipped on resume — the mark records the NEXT alternative, not the inner β. */
        IR_t * save = lc_build(g, IR_MATCH_ALTERNATE, NULL, NULL);    /* phase-0 (n_operands==0): save cursor + resume dispatch */
        IR_t * join = lc_build(g, IR_MATCH_ALTERNATE, NULL, succ);    /* trailing T = J_n: ω→outer succ (MARK exit) */
        ir_operand_push(join, save);
        if (fail && ir_is_generator_kind(fail->op)) lc_γ_to_β(join, fail); else lc_γ_to(join, fail); /* T reload arm exhausts leftward */
        for (int i = na - 1; i >= 1; i--) {
            IR_t * ei = sno_pat_node(cx, alts[i], join, join);        /* succ → J_{i+1}.α (MARK); fail → J_{i+1}.β (reload+try-next, β via sno_ω_to) */
            IR_t * ji = lc_build(g, IR_MATCH_ALTERNATE, ei, succ);    /* J_i: γ→alternative i's entry (its reload arm's target), ω→outer succ (MARK exit) */
            ir_operand_push(ji, save);
            join = ji;
        }
        IR_t * e0 = sno_pat_node(cx, alts[0], join, join);            /* leftmost: succ→J_1.α (MARK), fail→J_1.β (reload, try alternative 1) */
        lc_γ_to(save, e0);                                            /* SAVE proceeds to the first alternative */
        return save;
    }
    default:
        sno_fatal("pattern element not in the SN4-PAT subset (LEN, literal, ANY, NOTANY, SPAN, BREAK, BREAKX, TAB, RTAB, POS, RPOS, REM, ARB; SEQ+ALT landed SN4-PAT-3h)", NULL);
    }
    return succ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_supported(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FENCE) return t->n == 0 || sno_pat_supported(t->c[0]);
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "FENCE")) return 1;
    if (t->t == TT_QLIT) return 1;
    if (t->t == TT_ANY || t->t == TT_NOTANY) return sno_cset_fold((t->n > 0) ? t->c[0] : NULL) != NULL;
    if (t->t == TT_SPAN) return sno_cset_fold((t->n > 0) ? t->c[0] : NULL) != NULL;
    if (t->t == TT_BREAK || t->t == TT_BREAKX) return sno_cset_fold((t->n > 0) ? t->c[0] : NULL) != NULL;
    if (t->t == TT_TAB || t->t == TT_RTAB) return t->n > 0 && t->c[0] != NULL;
    if (t->t == TT_POS || t->t == TT_RPOS) return t->n > 0 && t->c[0] != NULL;
    if (t->t == TT_REM || t->t == TT_ARB) return 1;
    if (t->t == TT_ARBNO) return t->n > 0 && t->c[0] && sno_pat_supported(t->c[0]) && (sno_pat_deterministic(t->c[0]) || sno_pat_v1_fenced_ok(t->c[0]) || (sno_pat_v2_ok(t->c[0]) && sno_pat_v2_tail_gen(t->c[0])));
    if (t->t == TT_VAR) return t->v.sval != NULL;
    if (t->t == TT_DEFER) return t->n > 0 && t->c[0] != NULL;
    if (t->t == TT_LEN) return t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT;
    if (t->t == TT_CAPT_COND_ASGN) return t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && sno_pat_supported(t->c[0]);
    if (t->t == TT_CAPT_IMMED_ASGN) return t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && sno_pat_supported(t->c[0]);
    if (t->t == TT_CAPT_CURSOR) return t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval;
    if (t->t == TT_SEQ) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (t->t == TT_ALT) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_pattern_rhs(const tree_t * t) {
    if (!t) return 0;
    switch (t->t) {
    case TT_ALT: case TT_FENCE: case TT_ARBNO:
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX:
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS:
    case TT_ARB: case TT_REM: case TT_BAL:
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN:
        return 1;
    case TT_SEQ: case TT_CAT:
        return sno_is_pattern_rhs((t->n > 0) ? t->c[0] : NULL) || sno_is_pattern_rhs((t->n > 1) ? t->c[1] : NULL);
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_pat_collect(const tree_t * pat) {
    if (g_sno_npat >= SNO_PAT_MAX) sno_fatal("too many stored patterns in one program", NULL);
    char buf[32]; snprintf(buf, sizeof buf, "PAT$%d", g_sno_npat);
    g_sno_pats[g_sno_npat].name = lp_strdup(buf);
    g_sno_pats[g_sno_npat].pat = pat;
    return g_sno_pats[g_sno_npat++].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ) {
    IR_graph_t * g = cx->g;
    cx->pat_fail = fJ; cx->pat_seal = fJ;
    const tree_t * svt = (subj->n > 0) ? subj->c[0] : NULL;
    const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
    IR_t * head = lc_build(g, IR_MATCH_HEAD, NULL, fJ);
    /* SN4-REPL (doctrine stages 4/5): pattern-success → RELEASE (stashes end@head+24, flushes captures per
     * manual Ch.6 "before replacement") → replacement expression chain → SPLICE (rt_match_replace by name)
     * → sJ.  Slice 1: subject must be a plain variable lvalue — indirect/subscript splice targets pending. */
    IR_t * splice = NULL;
    if (has_repl) {
        if (!svt || svt->t != TT_VAR) sno_fatal("SN4-REPL slice 1: replacement subject must be a plain variable (indirect/subscript lvalue splice pending)", NULL);
        sno_reg_var(svt->v.sval);
        splice = lc_build(g, IR_MATCH_REPLACE, sJ, NULL);
        IR_LIT(splice).sval = svt->v.sval;
        ir_operand_push(splice, head);
        IR_t * rv = NULL; IR_t * re;
        if (repl_t) re = sx_lower(cx, repl_t, splice, fJ, &rv);
        else { re = lc_build(g, IR_LIT_STRING, splice, fJ); IR_LIT(re).sval = (char *) ""; rv = re; }
        ir_operand_push(splice, rv);
        sJ = re;
    }
    /* BB-OWNED-ζ statement-scope pivot (this session): release owns the success exit the way head's own ω
     * already owns the failure exit (head IS the scanner's single fixed exhaustion choke point; success has
     * no such fixed point of its own — it's whichever pattern element the match happens to end on — so this
     * node IS that fixed point, added for exactly this).  operand[0] = head, read at emit time via the same
     * operand[0]-owner convention IR_MATCH_ARBNO's non-owner phases already use to find role 0's slot. */
    IR_t * release = lc_build(g, IR_MATCH_RELEASE, sJ, NULL);
    if (has_repl) IR_LIT(release).dval = 1.0;
    ir_operand_push(release, head);
    IR_t * pat_entry = sno_pat_node(cx, ptt, release, head);
    lc_γ_to(head, pat_entry);
    IR_t * subjval = NULL;
    IR_t * subj_entry = sx_lower(cx, svt, head, fJ, &subjval);
    ir_operand_push(head, subjval);
    if (splice) ir_operand_push(splice, subjval);
    return subj_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name) {
    IR_graph_t * g = IR_alloc(nst * 16 + 256);
    scx_t cx; cx.g = g; cx.loop_exit = NULL; cx.loop_next = NULL; cx.result_name = result_name; cx.pat_fail = NULL; cx.pat_seal = NULL;
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t ** anchor = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    bb_label_registry_reset();
    for (int i = 0; i < nst; i++) {
        anchor[i] = lc_build(g, IR_GOTO, NULL, NULL);
        if (getenv("MONITOR_BIN")) { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) IR_LIT(anchor[i]).ival = _sa->c[0]->v.ival; }
        const char * lbl = sfind_str(st[i], ":lbl");
        if (lbl && lbl[0]) bb_label_registry_add(lp_strdup(lbl), anchor[i]);
    }
    bb_label_registry_add(lp_strdup("END"), exitnd);
    if (!bb_label_landing("RETURN"))  bb_label_registry_add(lp_strdup("RETURN"),  exitnd);
    if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), failnd);
    if (!bb_label_landing("NRETURN")) { IR_t * nrl = lc_build(g, IR_LIT_STRING, NULL, failnd); IR_LIT(nrl).sval = (char *) ""; IR_t * nnd = lc_build(g, IR_CALL, exitnd, failnd); IR_LIT(nnd).sval = (char *) "SNO$NRET"; lc_γ_to(nrl, nnd); ir_operand_push(nnd, nrl); bb_label_registry_add(lp_strdup("NRETURN"), nrl); }
    g->entry = (nst > 0) ? anchor[entry_idx] : exitnd;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i];
        { extern void zls_group_mark(const IR_graph_t *, const char *); const char * mlbl = sfind_str(s, ":lbl"); if (mlbl && mlbl[0]) zls_group_mark(g, lp_strdup(mlbl)); }
        IR_t * next = (i + 1 < nst) ? anchor[i + 1] : exitnd;
        if (sfind(s, ":end")) { lc_γ_to(anchor[i], exitnd); continue; }
        const char * goU = sgoto(s, TT_GOTO_U);
        const char * goS = sgoto(s, TT_GOTO_S);
        const char * goF = sgoto(s, TT_GOTO_F);
        IR_t * sT = goS ? sno_goto_target(g, goS, exitnd) : (goU ? sno_goto_target(g, goU, exitnd) : next);
        IR_t * fT = goF ? sno_goto_target(g, goF, exitnd) : (goU ? sno_goto_target(g, goU, exitnd) : next);
        IR_t * sJ = lc_build(g, IR_GOTO, sT, NULL);
        IR_t * fJ = lc_build(g, IR_GOTO, fT, NULL);
        if (is_def && is_def[i]) { lc_γ_to(anchor[i], sJ); continue; }
        const tree_t * subj = lc_stmt_subj(s);
        const tree_t * pat  = sfind_expr(s, ":pat");
        int has_eq = sfind(s, ":eq") != NULL;
        if (pat) sno_fatal("statement has a separate :pat field (stored-pattern form) — SN4-PAT-2 handles TT_SCAN match subjects only", NULL);
        if (subj && subj->t == TT_SCAN) {
            const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
            if (!sno_pat_supported(ptt)) {
                if (ptt && ptt->t == TT_FNC) {
                    extern tree_t *ast_stmt_new(tree_e kind);
                    static int g_pattmp_n = 0;
                    char nmb[24]; snprintf(nmb, sizeof nmb, "PATTMP$%d", g_pattmp_n++);
                    char * tmpn = lp_strdup(nmb); sno_reg_var(tmpn);
                    IR_t * asn = lc_build(g, IR_ASSIGN, NULL, fJ); IR_LIT(asn).sval = tmpn;
                    IR_t * vr = NULL; IR_t * ec = sx_lower(&cx, ptt, asn, fJ, &vr);
                    ir_operand_push(asn, vr);
                    tree_t * dv = ast_stmt_new(TT_VAR); dv->v.sval = tmpn;
                    tree_t * dd = ast_stmt_new(TT_DEFER); ast_push(dd, dv);
                    tree_t * sc2 = ast_stmt_new(TT_SCAN); ast_push(sc2, (tree_t *) subj->c[0]); ast_push(sc2, dd);
                    IR_t * e2 = sno_lower_match(&cx, sc2, has_eq ? sfind_expr(s, ":repl") : NULL, has_eq, sJ, fJ);
                    lc_γ_to(asn, e2);
                    lc_γ_to(anchor[i], ec);
                    continue;
                }
                sno_fatal("pattern shape outside the SN4-PAT subset (LEN, literal, ANY, NOTANY, SPAN, BREAK, BREAKX, TAB, RTAB, POS, RPOS, REM, ARB)", NULL);
            }
            IR_t * e = sno_lower_match(&cx, subj, has_eq ? sfind_expr(s, ":repl") : NULL, has_eq, sJ, fJ);
            lc_γ_to(anchor[i], e);
            continue;
        }
        if (!subj) { lc_γ_to(anchor[i], sJ); continue; }
        if (!has_eq) {
            IR_t * r = NULL; IR_t * e = sx_lower(&cx, subj, sJ, fJ, &r);
            lc_γ_to(anchor[i], e);
            continue;
        }
        tree_t * repl = sfind_expr(s, ":repl");
        if (subj->t == TT_VAR && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) {
            sno_reg_var(subj->v.sval);
            const char * bn = NULL;
            for (int fzi = 0; fzi < g_sno_nfz; fzi++) if (g_sno_fz[fzi].pat == repl) { bn = g_sno_fz[fzi].procname; break; }
            if (!bn) bn = sno_pat_collect(repl);
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fJ); IR_LIT(asn).sval = subj->v.sval;
            IR_t * mk = lc_build(g, IR_CALL, asn, fJ); IR_LIT(mk).sval = (char *) "SNO$MKPAT";
            IR_t * nl = lc_build(g, IR_LIT_STRING, mk, fJ); IR_LIT(nl).sval = (char *) bn;
            ir_operand_push(mk, nl);
            ir_operand_push(asn, mk);
            lc_γ_to(anchor[i], nl);
            continue;
        }
        if (subj->t == TT_VAR) {
            sno_reg_var(subj->v.sval);
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fJ); IR_LIT(asn).sval = subj->v.sval;
            IR_t * vr = NULL; IR_t * e = sx_lower(&cx, repl, asn, fJ, &vr);
            ir_operand_push(asn, vr);
            lc_γ_to(anchor[i], e);
            continue;
        }
        if (subj->t == TT_INDIRECT && subj->n > 0) {
            IR_t * nv = NULL; IR_t * e1 = sx_nameval(&cx, subj->c[0], NULL, fJ, &nv);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
            lc_γ_to(nv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, nv); ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_IDX && subj->n >= 2) {
            IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(&cx, subj->c[0], (const tree_t * const *) &subj->c[1], subj->n - 1, fJ, &vr);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
            lc_γ_to(vr, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, vr); ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_FNC) {
            const char * fname = subj->v.sval; int argbase = 0;
            if (!fname && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR) { fname = subj->c[0]->v.sval; argbase = 1; }
            int fnargs = subj->n - argbase;
            if (fname && !strcmp(fname, "ITEM") && fnargs >= 2) {
                IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(&cx, subj->c[argbase], (const tree_t * const *) &subj->c[argbase + 1], fnargs - 1, fJ, &vr);
                IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
                lc_γ_to(vr, e2);
                IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, vr); ir_operand_push(asn, vv);
                lc_γ_to(anchor[i], e1);
                continue;
            }
            { extern int rt_dat_field_of_any(const char *);
              if (fname && fnargs == 1 && rt_dat_field_of_any(fname)) {
                  IR_t * br = NULL; IR_t * e1 = sx_lower(&cx, subj->c[argbase], NULL, fJ, &br);
                  IR_t * fv = lc_build(g, IR_FIELD_VAR, NULL, fJ); IR_LIT(fv).sval = (char *) lp_strdup(fname);
                  lc_γ_to(br, fv);
                  ir_operand_push(fv, br);
                  IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
                  lc_γ_to(fv, e2);
                  IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
                  lc_γ_to(vv, asn);
                  ir_operand_push(asn, fv); ir_operand_push(asn, vv);
                  lc_γ_to(anchor[i], e1);
                  continue;
              } }
        }
        if (subj->t == TT_KEYWORD && subj->v.sval) {
            IR_t * mk = lc_build(g, IR_CALL, sJ, fJ); IR_LIT(mk).sval = (char *) "SNO$KWSET";
            IR_t * nl = lc_build(g, IR_LIT_STRING, NULL, fJ); IR_LIT(nl).sval = subj->v.sval;
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
            lc_γ_to(nl, e2);
            lc_γ_to(vv, mk);
            ir_operand_push(mk, nl); ir_operand_push(mk, vv);
            lc_γ_to(anchor[i], nl);
            continue;
        }
        if (subj->t == TT_FNC) {
            IR_t * wl = lc_build(g, IR_LIT_STRING, NULL, fJ); IR_LIT(wl).sval = (char *) "";
            IR_t * mk = lc_build(g, IR_CALL, sJ, fJ); IR_LIT(mk).sval = (char *) "SNO$WANTNM";
            lc_γ_to(wl, mk); ir_operand_push(mk, wl);
            IR_t * cv = NULL; IR_t * e1 = sx_lower(&cx, subj, NULL, fJ, &cv);
            lc_γ_to(mk, e1);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
            lc_γ_to(cv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, cv); ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], wl);
            continue;
        }
        sno_fatal("assignment subject form not in the landed subset", NULL);
    }
    if (g_sno_uses_stmtkw) {
        for (int i = 0; i < nst; i++) {
            if (is_def && is_def[i]) continue;
            IR_t * body = anchor[i]->γ.node;
            IR_t * hook = lc_build(g, IR_CALL, body, body); IR_LIT(hook).sval = (char *) "SNO$STMT";
            IR_t * num = lc_build(g, IR_LIT_INTEGER, hook, hook); IR_LIT(num).ival = (int64_t)(i + 1);
            ir_operand_push(hook, num);
            lc_γ_to(anchor[i], num);
        }
    }
    free(anchor);
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
static void sno_prescan_expr(const tree_t * t, sno_def_t * defs, int * ndefs) {
    if (!t) return;
    if (t->t == TT_FNC) {
        const char * name = t->v.sval; int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        if (name && !strcmp(name, "DATA") && t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
            extern void * dat_register(const char * spec); extern void * dat_find_type(const char * name);
            const char * sp = t->c[argbase]->v.sval;
            char nb[128]; int k = 0; for (; sp[k] && sp[k] != '(' && k < 127; k++) nb[k] = sp[k]; nb[k] = 0;
            if (nb[0] && !dat_find_type(nb)) dat_register(sp);
        }
        if (name && !strcmp(name, "OPSYN") && t->n - argbase == 2) {
            const char * an = sno_litname(t->c[argbase]);
            const char * on = sno_litname(t->c[argbase + 1]);
            if (an && on) {
                int fo = -1;
                for (int k = 0; k < *ndefs; k++) if (!strcmp(defs[k].fname, on)) { fo = k; break; }
                if (fo >= 0 && *ndefs < SNO_DEF_MAX) { sno_def_t d = defs[fo]; d.result_name = d.result_name ? d.result_name : d.fname; d.fname = lp_strdup(an); sno_reg_var(d.fname); defs[(*ndefs)++] = d; }
                else { extern void rt_builtin_synonym_add(const char *, const char *); rt_builtin_synonym_add(lp_strdup(an), lp_strdup(on)); }
            }
        }
        if (name && !strcmp(name, "DEFINE") && t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
            /* expression-position DEFINE (e.g. DIFFER(DEFINE('f(n)','entry'))) — hoist-register identically to the statement path (last-define-wins, the standing compile-time deviation); sx_lower then folds the call to null */
            const char * entry_opt = NULL;
            if (t->n > argbase + 1 && t->c[argbase + 1]) {
                const tree_t * ea = t->c[argbase + 1];
                if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
                else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
            }
            sno_def_t d; sno_parse_define(t->c[argbase]->v.sval, entry_opt, &d);
            sno_cconst_note_define_names(&d);
            sno_predef_note(d.fname);
            int fo = -1;
            for (int k = 0; k < *ndefs; k++) if (!strcmp(defs[k].fname, d.fname)) { fo = k; break; }
            if (fo >= 0) defs[fo] = d;
            else if (*ndefs < SNO_DEF_MAX) defs[(*ndefs)++] = d;
            else sno_fatal("too many DEFINEs in one program", d.fname);
        }
    }
    for (int i = 0; i < t->n; i++) sno_prescan_expr(t->c[i], defs, ndefs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t * lower_sno_stage2(const tree_t * prog) {
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    g_sno_nexpr = 0;
    g_sno_npat = 0;
    g_sno_uses_stmtkw = 0;
    g_sno_uses_code = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i]) { sno_scan_stmtkw(prog->c[i]); sno_scan_code_use(prog->c[i]); }
    sno_register_program(&g_stage2, prog);
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    if (nst == 0) return &g_stage2;
    const tree_t ** st = (const tree_t **) calloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    sno_cconst_build_table(st, nst);
    sno_fz_build_table(st, nst);
    sno_def_t defs[SNO_DEF_MAX]; int ndefs = 0; g_sno_npredef = 0;
    const tree_t * def_body[SNO_DEF_MAX]; for (int _k = 0; _k < SNO_DEF_MAX; _k++) def_body[_k] = NULL;
    int * is_def = (int *) calloc((size_t) nst, sizeof(int));
    for (int i = 0; i < nst; i++) {
        sno_prescan_expr(lc_stmt_subj(st[i]), defs, &ndefs);
        const tree_t * dfn = lc_stmt_subj(st[i]);
        if (dfn && dfn->t == TT_DEFINE) {
            is_def[i] = 1;
            const tree_t * pnode = (dfn->n > 1) ? dfn->c[1] : NULL;
            if (!pnode || pnode->t != TT_QLIT || !pnode->v.sval) sno_fatal("TT_DEFINE missing literal prototype string", NULL);
            sno_def_t d; sno_parse_define(pnode->v.sval, NULL, &d);
            const tree_t * body = (dfn->n > 2) ? dfn->c[2] : NULL;
            int found = -1;
            for (int k = 0; k < ndefs; k++) if (!strcmp(defs[k].fname, d.fname)) { found = k; break; }
            if (found >= 0) { defs[found] = d; def_body[found] = body; }
            else if (ndefs < SNO_DEF_MAX) { def_body[ndefs] = body; defs[ndefs++] = d; }
            else sno_fatal("too many DEFINEs in one program", d.fname);
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
        sno_def_t d; sno_parse_define(dsub->c[argbase]->v.sval, entry_opt, &d);
        int found = -1;
        for (int k = 0; k < ndefs; k++) if (!strcmp(defs[k].fname, d.fname)) { found = k; break; }
        if (found >= 0) defs[found] = d;
        else if (ndefs < SNO_DEF_MAX) defs[ndefs++] = d;
        else sno_fatal("too many DEFINEs in one program", d.fname);
    }
    IR_graph_t * g = sno_build_graph(st, nst, 0, is_def, NULL);
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = "main";
    g_stage2.proc_table[pi].proc = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].nparams = 0;
    g_stage2.proc_table[pi].is_generator = 0;
    g_stage2.proc_table[pi].dyn_scope = 0;
    g_stage2.proc_table[pi].result_name = NULL;
    g_stage2.proc_table[pi].bb_idx = bb_program_add(&g_stage2.bbp, g);
    /* EVAL/CODE (manual Ch.9): a CODE fragment may goto BACK into a main-program label (its `:S(L)F(DONE)`
     * fields), so every main label must be transfer-reachable at runtime.  Each labelled statement becomes a
     * pseudo-proc "LBL__<name>" — the SAME statement array re-lowered with entry_idx at that label, exactly the
     * entry-label DEFINE shape below — flowing through the existing proc registration in BOTH mode drivers
     * untouched.  rt_goto_transfer resolves fragment-registry first (fragment labels OVERRIDE main's, per the
     * manual), then these.  Use-gated on g_sno_uses_code => byte-zero perturbation for programs without CODE. */
    if (g_sno_uses_code) {
        for (int i = 0; i < nst; i++) {
            const char * lbl = sfind_str(st[i], ":lbl");
            if (!lbl || !lbl[0]) continue;
            IR_graph_t * gl = sno_build_graph(st, nst, i, is_def, NULL);
            char lname[256]; snprintf(lname, sizeof lname, "LBL__%s", lbl);
            int lpi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[lpi].name = lp_strdup(lname);
            g_stage2.proc_table[lpi].proc = NULL;
            g_stage2.proc_table[lpi].entry_pc = -1;
            g_stage2.proc_table[lpi].nparams = 0;
            g_stage2.proc_table[lpi].lower_sc.n = 0;
            g_stage2.proc_table[lpi].is_generator = 0;
            g_stage2.proc_table[lpi].dyn_scope = 0;
            g_stage2.proc_table[lpi].result_name = NULL;
            g_stage2.proc_table[lpi].bb_idx = bb_program_add(&g_stage2.bbp, gl);
        }
    }
    for (int di = 0; di < ndefs; di++) {
        IR_graph_t * gf;
        const char * rn = defs[di].result_name ? defs[di].result_name : defs[di].fname;
        if (def_body[di]) {
            const tree_t * bp = def_body[di];
            int bn = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bn++;
            const tree_t ** bst = (const tree_t **) calloc((size_t)(bn > 0 ? bn : 1), sizeof(tree_t *));
            int bk = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bst[bk++] = bp->c[i];
            int * bis = (int *) calloc((size_t)(bn > 0 ? bn : 1), sizeof(int));
            gf = sno_build_graph(bst, bn, 0, bis, rn);
            free((void *) bst); free(bis);
        } else {
            int eidx = -1;
            for (int i = 0; i < nst; i++) { const char * lbl = sfind_str(st[i], ":lbl"); if (lbl && !strcmp(lbl, defs[di].entry)) { eidx = i; break; } }
            if (eidx < 0) continue; /* entry label doesn't exist anywhere: this DEFINE is dead (never callable); SPITBOL only resolves an entry at call time, not at DEFINE time, so a program that never calls it must still run (132_pat_fence_eps_recur_shallow) */
            gf = sno_build_graph(st, nst, eidx, is_def, rn);
        }
        int fpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name = defs[di].fname;
        g_stage2.proc_table[fpi].proc = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].nparams = defs[di].nnames;
        for (int k = 0; k < defs[di].nnames && k < STAGE2_FRAME_SLOT_MAX; k++) g_stage2.proc_table[fpi].lower_sc.e[k].name = defs[di].names[k];
        g_stage2.proc_table[fpi].lower_sc.n = defs[di].nnames < STAGE2_FRAME_SLOT_MAX ? defs[di].nnames : STAGE2_FRAME_SLOT_MAX;
        g_stage2.proc_table[fpi].is_generator = 0;
        g_stage2.proc_table[fpi].dyn_scope = 1;
        g_stage2.proc_table[fpi].result_name = defs[di].result_name;
        g_stage2.proc_table[fpi].bb_idx = bb_program_add(&g_stage2.bbp, gf);
    }
    g_sno_in_patproc = 1;
    for (int xi = 0; xi < g_sno_nexpr; xi++) {
        IR_graph_t * gx = IR_alloc(256);
        scx_t ex; ex.g = gx; ex.loop_exit = NULL; ex.loop_next = NULL; ex.result_name = g_sno_exprs[xi].name; ex.pat_fail = NULL; ex.pat_seal = NULL;
        IR_t * ok = lc_build(gx, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gx, IR_FAIL, NULL, NULL);
        IR_t * sJ = lc_build(gx, IR_GOTO, ok, NULL);
        IR_t * fJ = lc_build(gx, IR_GOTO, no, NULL);
        sno_reg_var(g_sno_exprs[xi].name);
        IR_t * asn = lc_build(gx, IR_ASSIGN, sJ, fJ); IR_LIT(asn).sval = (char *) g_sno_exprs[xi].name;
        IR_t * vr = NULL; IR_t * e = sx_lower(&ex, g_sno_exprs[xi].expr, asn, fJ, &vr);
        ir_operand_push(asn, vr);
        gx->entry = e;
        int xpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[xpi].name = g_sno_exprs[xi].name;
        g_stage2.proc_table[xpi].proc = NULL;
        g_stage2.proc_table[xpi].entry_pc = -1;
        g_stage2.proc_table[xpi].nparams = 0;
        g_stage2.proc_table[xpi].lower_sc.n = 0;
        g_stage2.proc_table[xpi].is_generator = 0;
        g_stage2.proc_table[xpi].dyn_scope = 1;
        g_stage2.proc_table[xpi].result_name = g_sno_exprs[xi].name;
        g_stage2.proc_table[xpi].bb_idx = bb_program_add(&g_stage2.bbp, gx);
    }
    for (int pi2 = 0; pi2 < g_sno_npat; pi2++) {
        IR_graph_t * gp = IR_alloc(512);
        scx_t px; px.g = gp; px.loop_exit = NULL; px.loop_next = NULL; px.result_name = NULL; px.pat_fail = NULL; px.pat_seal = NULL;
        IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
        px.pat_fail = no; px.pat_seal = no;
        IR_t * pe = sno_pat_node(&px, g_sno_pats[pi2].pat, ok, no);
        gp->entry = pe;
        int ppi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[ppi].name = g_sno_pats[pi2].name;
        g_stage2.proc_table[ppi].proc = NULL;
        g_stage2.proc_table[ppi].entry_pc = -1;
        g_stage2.proc_table[ppi].nparams = 0;
        g_stage2.proc_table[ppi].lower_sc.n = 0;
        g_stage2.proc_table[ppi].is_generator = 0;
        g_stage2.proc_table[ppi].dyn_scope = 0;
        g_stage2.proc_table[ppi].result_name = NULL;
        g_stage2.proc_table[ppi].bb_idx = bb_program_add(&g_stage2.bbp, gp);
    }
    g_sno_in_patproc = 0;
    free((void *) st); free(is_def);
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EVAL/CODE runtime-lowering entries (directive lifted 2026-07-08, Lon: "We're ready now since we have
 * optimized BB-owned memory management").  These lower a runtime-parsed fragment to ONE emit-ready graph.
 * lower_snobol4(prog) — the EVAL shape: entry at statement 0 (runtime_eval.c's synthesized ZZEVALZZ program).
 * sno_lower_fragment_at(prog, entry_idx) — the CODE shape: same statement array entered AT a label's index
 * (identical to the entry-label DEFINE / LBL__ pseudo-proc shape in lower_sno_stage2 above), one call per
 * fragment label so every label lands on its own chain α.  Neither touches g_stage2 (the main program's
 * stage2 is dead by the time fragments compile — ir_delete_all ran — but staying off it keeps that a fact
 * about timing, not a dependency).  DEFINE inside a fragment is a loud fatal: lower would build the body
 * graph but nothing emits or registers it on this path — flagged, not faked. */
static void sno_fragment_reject_define(const tree_t ** st, int nst) {
    g_sno_npredef = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * dfn = lc_stmt_subj(st[i]);
        if (dfn && dfn->t == TT_DEFINE) sno_fatal("DEFINE inside a runtime-compiled CODE/EVAL fragment is outside the landed subset", NULL);
        if (sno_stmt_define(st[i], NULL)) sno_fatal("DEFINE inside a runtime-compiled CODE/EVAL fragment is outside the landed subset", NULL);
        if (sno_tree_has_define_call(st[i])) sno_fatal("DEFINE inside a runtime-compiled CODE/EVAL fragment is outside the landed subset", NULL);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_pat_tree_graph_rt(const tree_t * pat) {
    IR_graph_t * gp = IR_alloc(512);
    scx_t px; px.g = gp; px.loop_exit = NULL; px.loop_next = NULL; px.result_name = NULL; px.pat_fail = NULL; px.pat_seal = NULL;
    IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
    IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
    px.pat_fail = no; px.pat_seal = no;
    IR_t * pe = sno_pat_node(&px, pat, ok, no);
    gp->entry = pe;
    return gp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_lower_fragment_at(const tree_t * prog, int entry_idx) {
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    /* The zls_* compile-metadata tables (zeta_storage.c) are process-global and sized for the driver's
     * one-compile-per-process life; runtime fragment compiles would exhaust them (~24 EVALs hit the
     * za[1024] geometry cap — the eval_dynamic benchmark found it).  Every graph compiled so far is
     * sealed before any fragment lowers (main + procs emit before main runs, in both modes), so its
     * zls entries are dead weight; recycle the tables per fragment compile. */
    { extern void zls_reset(void); zls_reset(); }
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    if (nst == 0 || entry_idx < 0 || entry_idx >= nst) return NULL;
    const tree_t ** st = (const tree_t **) calloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    sno_fragment_reject_define(st, nst);
    sno_cconst_build_table(st, nst);
    g_sno_nfz = 0; g_sno_fz_unsafe = 1;
    int * is_def = (int *) calloc((size_t) nst, sizeof(int));
    IR_graph_t * g = sno_build_graph(st, nst, entry_idx, is_def, NULL);
    { extern void optimizer_run(IR_graph_t *); extern void ir_drive_slot_assign(IR_graph_t *); if (g) { optimizer_run(g); ir_drive_slot_assign(g); } }
    free((void *) st); free(is_def);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_snobol4(const tree_t * prog) {
    return sno_lower_fragment_at(prog, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_stmt_label(const tree_t * s) {
    return s ? sfind_str(s, ":lbl") : NULL;
}
