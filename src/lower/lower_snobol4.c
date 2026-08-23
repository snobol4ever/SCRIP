#include "rt/rt_arena.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lower.h"
#include "bb_program.h"
#include "parser/icon/icon_lex.h"
#include "zeta_choices.h"
int rt_zeta_port_mode(void);
int rt_kw_index(const char * kw);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_kw_static_slot(const char * kw) { return kw ? rt_kw_index(kw) : -1; }
extern void global_register(const char * name);
extern int stage2_proc_grow(stage2_t * s2);
typedef struct { const tree_t * arg; IR_t * prim; int str; long codes; const char * snapg; } sprearg_t;
typedef struct { IR_graph_t * g; IR_t * loop_exit; IR_t * loop_next; const char * result_name; IR_t * pat_fail; IR_t * pat_seal; sprearg_t pre[64]; int npre; } scx_t;
#define SNO_DEF_MAX 128
#define SNO_DEF_NAMES_MAX 64
typedef struct { const char * fname; const char * entry; const char * result_name; const char * names[SNO_DEF_NAMES_MAX]; int nnames; int nformals; } sno_def_t;
#define SNO_EXPR_MAX 4096
static struct { const char * name; const tree_t * expr; int salt; int want_name; } g_sno_exprs[SNO_EXPR_MAX];
static int g_sno_nexpr = 0;
static int g_sno_expr_salt = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_expr_salt_next(void) { g_sno_expr_salt++; }
static const char * sno_expr_collect(const tree_t * expr);
#define SNO_PAT_MAX 256
static struct { const char * name; const tree_t * pat; int salt; } g_sno_pats[SNO_PAT_MAX];
static int g_sno_npat = 0;
static int g_sno_uses_stmtkw = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
static int g_sno_uses_code = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_setexit_on(void) { const char * e = getenv("SCRIP_SETEXIT"); return (e && e[0] == '0') ? 0 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_scan_code_use(const tree_t * t) {
    if (!t || g_sno_uses_code) return;
    if (t->t == TT_FNC) {
        const char * fn = t->v.sval;
        if (!fn && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) fn = t->c[0]->v.sval;
        if (fn && !strcmp(fn, "CODE")) { g_sno_uses_code = 1; return; }
        if (fn && sno_setexit_on() && !strcmp(fn, "SETEXIT")) { g_sno_uses_code = 1; return; }
    }
    if ((t->t == TT_GOTO_U || t->t == TT_GOTO_S || t->t == TT_GOTO_F) && t->n > 0 && t->c[0]) {
        const tree_t * g0 = t->c[0];
        if (g0->t != TT_QLIT || (g0->v.sval && g0->v.sval[0] == '$')) { g_sno_uses_code = 1; return; }
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
static int zw5_on(void) { const char * e = getenv("SCRIP_ZW5"); return (e && *e == '0') ? 0 : 1; }
static const char * sfind_str(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0 && a->c[0]) ? a->c[0]->v.sval : NULL; }
static tree_t * sfind_expr(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0) ? a->c[0] : NULL; }
static void sno_reg_var(const char * nm) { if (nm && nm[0] && nm[0] != '&') global_register(lp_strdup(nm)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_expr_eq(const tree_t * a, const tree_t * b) {
    if (a == b) return 1; if (!a || !b) return 0; if (a->t != b->t || a->n != b->n) return 0; if (a->v.ival != b->v.ival) return 0;
    for (int i = 0; i < a->n; i++) if (!sno_expr_eq(a->c[i], b->c[i])) return 0; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_collect(const tree_t * expr) {
    if (!expr) sno_fatal("unevaluated-expression operator (*) with no operand", NULL);
    for (int i = 0; i < g_sno_nexpr; i++) if (g_sno_exprs[i].salt == g_sno_expr_salt && sno_expr_eq(g_sno_exprs[i].expr, expr)) return g_sno_exprs[i].name;
    if (g_sno_nexpr >= SNO_EXPR_MAX) sno_fatal("too many unevaluated expressions (*) in one program", NULL);
    char buf[32]; if (g_sno_expr_salt) snprintf(buf, sizeof buf, "EXPR$%dF%d", g_sno_nexpr, g_sno_expr_salt); else snprintf(buf, sizeof buf, "EXPR$%d", g_sno_nexpr);
    g_sno_exprs[g_sno_nexpr].name = lp_strdup(buf);
    g_sno_exprs[g_sno_nexpr].expr = expr;
    g_sno_exprs[g_sno_nexpr].salt = g_sno_expr_salt;
    g_sno_exprs[g_sno_nexpr].want_name = 0;
    return g_sno_exprs[g_sno_nexpr++].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_expr_collect_wn(const tree_t * expr) {
    const char * nm = sno_expr_collect(expr);
    for (int i = 0; i < g_sno_nexpr; i++) if (g_sno_exprs[i].name == nm) { g_sno_exprs[i].want_name = 1; break; }
    return nm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_binop_code(tree_e tt) {
    switch (tt) {
    case TT_ADD: return 0; case TT_SUB: return 1; case TT_MUL: return 2; case TT_DIV: return 3; case TT_POW: return 18; case TT_SEQ: return 11; case TT_CAT: return 11;
    default: return -1; }
}
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sx_sub_container_only(IR_t * sub) { const char * e = getenv("SCRIP_SUB_AGG"); if (!(e && *e == '0')) IR_LIT(sub).sval = "container-only"; }
static IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res);
static IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land);
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
static const char * g_sno_predef[SNO_DEF_MAX]; static int g_sno_npredef = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_predef_note(const char * fname) { for (int k = 0; k < g_sno_npredef; k++) if (!strcmp(g_sno_predef[k], fname)) return; if (g_sno_npredef < SNO_DEF_MAX) g_sno_predef[g_sno_npredef++] = fname; }
static int sno_predef_registered(const char * fname) { if (!fname) return 0; for (int k = 0; k < g_sno_npredef; k++) if (!strcmp(g_sno_predef[k], fname)) return 1; return 0; }
static const char * sno_t4_target(const char * op, int nops);
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
    IR_t * cmp = lc_build(g, IR_CMP_TEST, γ, ω); IR_LIT(cmp).ival = rk;
    IR_t * cb = lc_build(g, lex ? IR_COERCE_STRING : IR_COERCE_NUMERIC, cmp, ω); IR_LIT(cb).ival = c2;
    IR_t * ca = lc_build(g, lex ? IR_COERCE_STRING : IR_COERCE_NUMERIC, cb, ω); IR_LIT(ca).ival = c1;
    const tree_t * ax = (t->n > argbase + 0) ? t->c[argbase + 0] : NULL;
    const tree_t * bx = (t->n > argbase + 1) ? t->c[argbase + 1] : NULL;
    IR_t * ar = NULL; IR_t * br = NULL; IR_t * be; IR_t * ae;
    if (bx) be = sx_lower(cx, bx, ca, ω, &br);
    else { br = lc_build(g, IR_LIT_STRING, ca, ω); IR_LIT(br).sval = (char *) ""; be = br; }
    if (ax) ae = sx_lower(cx, ax, be, ω, &ar);
    else { ar = lc_build(g, IR_LIT_STRING, be, ω); IR_LIT(ar).sval = (char *) ""; ae = ar; }
    ir_operand_push(ca, ar); if (!lex) ir_operand_push(ca, br);
    ir_operand_push(cb, br); if (!lex) ir_operand_push(cb, ar);
    ir_operand_push(cmp, ca); ir_operand_push(cmp, cb);
    if (res) *res = cmp;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR-IDENT/DIFFER slice 2 (Lon directive s199).  REDEFINITION GUARD, measured not assumed (3 oracle probes, sbl -bf): DEFINE/OPSYN
   targeting IDENT or DIFFER, direct or via CODE()/EVAL, is a fatal ERROR 248 "attempted redefinition of system function" in real SPITBOL
   -- no program that redefines these can reach a second statement, so the ORACLE never exercises a live rebinding here.  SCRIP is more
   permissive than the oracle on this one point, though: a literal-prototype DEFINE('IDENT(...)') in THIS program is honored by the
   pre-existing sno_predef_registered() compile-time registry (same mechanism sno_pred_relop/TT_OPSYN above already defer to), so the
   caller below refuses the fast path whenever this program's own source binds the name -- a BEHAVIOUR check (does this program DEFINE
   the name), never a name list, and it is what keeps a redefining program's own SCRIP-native semantics unregressed by this rung.  The
   1/2-arg split is an operand count, never an admission test on top of that. */
static IR_t * sx_ident_differ(scx_t * cx, const tree_t * t, int argbase, int is_differ, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    IR_t * nd = lc_build(g, is_differ ? IR_DIFFER : IR_IDENT, γ, ω);
    const tree_t * ax = (t->n > argbase + 0) ? t->c[argbase + 0] : NULL;
    const tree_t * bx = (t->n > argbase + 1) ? t->c[argbase + 1] : NULL;
    IR_t * ar = NULL; IR_t * br = NULL; IR_t * be; IR_t * ae;
    if (bx) be = sx_lower(cx, bx, nd, ω, &br);
    else { br = lc_build(g, IR_LIT_STRING, nd, ω); IR_LIT(br).sval = (char *) ""; be = br; }
    if (!ax) sno_fatal("IDENT/DIFFER with no argument", NULL);
    ae = sx_lower(cx, ax, be, ω, &ar);
    ir_operand_push(nd, ar); ir_operand_push(nd, br);
    if (res) *res = nd;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_call_named(scx_t * cx, const char * name, const tree_t * t, int argbase, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * call = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(call).sval = (char *) lp_strdup(name);
    IR_t * sr0 = NULL; static int c2bb = -1; if (c2bb < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); c2bb = (e2 && *e2 == '1') ? 1 : 0; }
    if (c2bb) { sr0 = lc_build(cx->g, IR_DEFINE, call, ω); IR_LIT(sr0).sval = IR_LIT(call).sval; }
    IR_t * tail = sr0 ? sr0 : call;
    int nargs = t ? (t->n - argbase) : 0;
    IR_t * prev = NULL; IR_t * entry = tail;
    for (int k = 0; k < nargs; k++) {
        IR_t * ar = NULL; IR_t * ae = sx_lower(cx, t->c[argbase + k], (k == nargs - 1) ? tail : NULL, ω, &ar);
        if (k == 0) entry = ae;
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) { ir_operand_push(call, ar); if (sr0) ir_operand_push(sr0, ar); }
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
static const tree_t * sno_const_val(const char * ck);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = lc_build(cx->g, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; if (res) *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; if (res) *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
    case TT_NUL:  { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    case TT_VAR:  { sno_reg_var(t->v.sval); IR_t * nd = lc_build(cx->g, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; if (res) *res = nd; return nd; }
    case TT_KEYWORD: { if (t->v.sval) { char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval); const tree_t * cv = sno_const_val(cb); if (cv) return sx_lower(cx, cv, γ, ω, res); }
        IR_t * nd = lc_build(cx->g, IR_KEYWORD_SNOBOL4, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
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
            /*⭐⭐ `.VAR` OVER A LITERAL VARIABLE NAME IS A CONSTANT, AND IT IS NOW LOWERED AS ONE (hq_P s266).  It used to lower to a runtime IR_CALL of the builtin
               SNO$NAME with the name as a string literal operand -- which cost the whole by-name builtin dispatch chain (rt_call_arr_bl -> setjmp -> rt_call_arr_impl ->
               try_call_builtin_by_name_bl) and then bn_sno_name, whose body is rt_ws_strdup of a name the COMPILER already had in .rodata.  MEASURED at 644 Ir per
               evaluation: 43,487 evaluations per json parse = 12.8% of the whole deserializer, and 6,469 per claws5 parse.  `estr = .dummy` is the SNOBOL4 return-by-name
               idiom, so it sits in the body of EVERY deferred action.
               ⛔ The descriptor is byte-for-byte what bn_sno_name built -- { v = DT_N, slen = 0, s = <name text> } -- so nothing downstream can tell the difference; the
               only change is that the text is the permanent .rodata literal instead of a fresh workspace copy minted on every call.  IR_LIT_NAME joins the existing
               LITERAL family in bb_lit_scalar (INTEGER/STRING/CHARSET/REAL) rather than getting a private path. */
            sno_reg_var(t->c[0]->v.sval);
            IR_t * nl = lc_build(cx->g, IR_LIT_NAME, γ, ω); IR_LIT(nl).sval = t->c[0]->v.sval;
            if (res) *res = nl; return nl;
        }
        if (t->c[0]->t == TT_IDX && t->c[0]->n >= 2) {
            const tree_t * ix = t->c[0];
            IR_t * vr = NULL; IR_t * entry = sx_subscript_lv(cx, ix->c[0], (const tree_t * const *) &ix->c[1], ix->n - 1, ω, &vr);
            if (γ) lc_γ_to(vr, γ);
            if (res) *res = vr; return entry;
        }
        if (t->c[0]->t == TT_FNC) {
            const tree_t * fn = t->c[0]; const char * fname = fn->v.sval; int argbase = 0;
            if (!fname && fn->n > 0 && fn->c[0] && fn->c[0]->t == TT_VAR) { fname = fn->c[0]->v.sval; argbase = 1; }
            extern int rt_dat_field_of_any(const char *);
            if (fname && (fn->n - argbase) == 1 && rt_dat_field_of_any(fname)) {
                IR_t * br = NULL; IR_t * ea = sx_lower(cx, fn->c[argbase], NULL, ω, &br);
                IR_t * fv = lc_build(cx->g, IR_FIELD_VAR, γ, ω); IR_LIT(fv).sval = (char *) lp_strdup(fname);
                lc_γ_to(br, fv);
                ir_operand_push(fv, br);
                if (res) *res = fv; return ea;
            }
        }
        sno_fatal("name operator over this form is outside the landed subset", NULL);
    }
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX: {
        if (t->n < 1) sno_fatal("charset pattern function with missing operand", NULL);
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PBK";
        IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω); IR_LIT(kt).ival = (int64_t) t->t;
        IR_t * va = NULL; IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &va);
        lc_γ_to(kt, ea); lc_γ_to(va, mk);
        ir_operand_push(mk, kt); ir_operand_push(mk, va);
        if (res) *res = mk; return kt;
    }
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS: {
        if (t->n < 1) sno_fatal("integer pattern function with missing operand", NULL);
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PBN";
        IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω); IR_LIT(kt).ival = (int64_t) t->t;
        IR_t * va = NULL; IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &va);
        lc_γ_to(kt, ea); lc_γ_to(va, mk);
        ir_operand_push(mk, kt); ir_operand_push(mk, va);
        if (res) *res = mk; return kt;
    }
    case TT_ARB: case TT_REM: case TT_BAL: case TT_FAIL: case TT_SUCCEED: case TT_ABORT: {
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PB0";
        IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, mk, ω); IR_LIT(kt).ival = (int64_t) t->t;
        ir_operand_push(mk, kt);
        if (res) *res = mk; return kt;
    }
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: {
        const tree_t * tgt = (t->n > 1) ? t->c[1] : NULL;
        const char * vn = (tgt && tgt->t == TT_VAR) ? tgt->v.sval : NULL;
        if (vn) sno_reg_var(vn);
        if (!vn && tgt && tgt->t == TT_DEFER) { const char * bn = sno_expr_collect((tgt->n > 0) ? tgt->c[0] : NULL); char pb[40]; snprintf(pb, sizeof pb, "*%s", bn); vn = lp_strdup(pb); }
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PBC";
        IR_t * kt = lc_build(cx->g, IR_LIT_INTEGER, NULL, ω); IR_LIT(kt).ival = (int64_t) t->t;
        IR_t * nl; IR_t * es; IR_t * vs = NULL;
        if (vn) {
            nl = lc_build(cx->g, IR_LIT_STRING, NULL, ω); IR_LIT(nl).sval = (char *) vn;
            es = sx_lower(cx, t->c[0], NULL, ω, &vs);
            lc_γ_to(kt, nl); lc_γ_to(nl, es);
        } else if (tgt && tgt->t == TT_INDIRECT) {
            IR_t * nv = NULL; IR_t * en = sx_lower(cx, (tgt->n > 0) ? tgt->c[0] : NULL, NULL, ω, &nv);
            es = sx_lower(cx, t->c[0], NULL, ω, &vs);
            lc_γ_to(kt, en); lc_γ_to(nv, es); nl = nv;
        } else {
            sno_fatal("capture target in a runtime-built pattern is not a simple variable", NULL); return NULL;
        }
        lc_γ_to(vs, mk);
        ir_operand_push(mk, kt); ir_operand_push(mk, nl); ir_operand_push(mk, vs);
        if (res) *res = mk; return kt;
    }
    case TT_ARBNO: {
        if (t->n < 1) sno_fatal("ARBNO with missing operand", NULL);
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PARB";
        IR_t * vi = NULL; IR_t * ei = sx_lower(cx, t->c[0], NULL, ω, &vi);
        lc_γ_to(vi, mk);
        ir_operand_push(mk, vi);
        if (res) *res = mk; return ei;
    }
    case TT_FENCE: {
        if (t->n == 0) { IR_t * mk0 = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk0).sval = (char *) "SNO$PB0"; IR_t * kt0 = lc_build(cx->g, IR_LIT_INTEGER, mk0, ω); IR_LIT(kt0).ival = (int64_t) TT_FENCE; ir_operand_push(mk0, kt0); if (res) *res = mk0; return kt0; }
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PFEN";
        IR_t * vi = NULL; IR_t * ei = sx_lower(cx, t->c[0], NULL, ω, &vi);
        lc_γ_to(vi, mk);
        ir_operand_push(mk, vi);
        if (res) *res = mk; return ei;
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
            sx_sub_container_only(sub);
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
        const char * name = t->v.sval;
        if (!name || !*name) sno_fatal("OPSYN operator expression with no symbol", NULL);
        { const char * _tg = sno_t4_target(name, t->n); if (_tg && sno_predef_registered(_tg)) return sx_call_named(cx, _tg, t, 0, γ, ω, res); }
        return sx_call_named(cx, name, t, 0, γ, ω, res);
    }
    case TT_FNC: {
        const char * name = t->v.sval; int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        if (!name) sno_fatal("call with no resolvable name", NULL);
        if (!strcmp(name, "DEFINE")) {
            char fnb[128]; fnb[0] = 0;
            if (t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
                const char * sp = t->c[argbase]->v.sval; int k = 0;
                for (; sp[k] && sp[k] != '(' && sp[k] != ' ' && k < 127; k++) fnb[k] = sp[k];
                fnb[k] = 0;
            }
            if (fnb[0] && sno_predef_registered(fnb)) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
            sno_fatal("DEFINE in this expression position is outside the landed subset (literal-prototype DEFINE in a statement subject only; pattern/replacement-field and fragment DEFINE pending)", NULL);
        }
        { int lex = 0; long c1 = 0, c2 = 0; int rk = sno_pred_relop(name, &lex, &c1, &c2);
          if (rk >= 0 && t->n - argbase >= 1 && t->n - argbase <= 2) return sx_pred_cmp(cx, t, argbase, lex, rk, c1, c2, γ, ω, res); }
        { static int _idon = -1; if (_idon < 0) { const char * e = getenv("SCRIP_IDENT_INLINE"); _idon = (e && *e == '0') ? 0 : 1; }
          int nid = t->n - argbase;
          if (_idon && nid >= 1 && nid <= 2 && !sno_predef_registered(name)) {
            if (!strcmp(name, "IDENT"))  return sx_ident_differ(cx, t, argbase, 0, γ, ω, res);
            if (!strcmp(name, "DIFFER")) return sx_ident_differ(cx, t, argbase, 1, γ, ω, res);
          } }
        return sx_call_named(cx, name, t, argbase, γ, ω, res);
    }
    case TT_WHILE: case TT_UNTIL: {
        const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
        if (!C) sno_fatal("loop with no condition", NULL);
        int is_until = (t->t == TT_UNTIL);
        IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
        IR_t * cr = NULL;
        IR_t * ce = is_until ? sx_lower(cx, C, γ, gate, &cr) : sx_lower(cx, C, gate, γ, &cr);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = ce;
        IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        lc_γ_to(gate, be);
        if (res) *res = NULL;
        return ce;
    }
    case TT_DO_WHILE: {
        const tree_t * B = (t->n > 0) ? t->c[0] : NULL; const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
        if (!C) sno_fatal("do-while without condition outside the landed subset", NULL);
        IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
        IR_t * cr = NULL;
        IR_t * ce = sx_lower(cx, C, gate, γ, &cr);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = ce;
        IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        lc_γ_to(gate, be);
        if (res) *res = NULL;
        return be;
    }
    case TT_FOR: {
        const tree_t * INIT = (t->n > 0) ? t->c[0] : NULL; const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
        const tree_t * STEP = (t->n > 2) ? t->c[2] : NULL; const tree_t * B = (t->n > 3) ? t->c[3] : NULL;
        if (!C) sno_fatal("for-loop without condition outside the landed subset", NULL);
        IR_t * gate = lc_build(cx->g, IR_GOTO, NULL, NULL);
        IR_t * cr = NULL;
        IR_t * ce = sx_lower(cx, C, gate, γ, &cr);
        IR_t * se = STEP ? sx_lower(cx, STEP, ce, ω, NULL) : ce;
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = se;
        IR_t * be = B ? sco_branch(cx, B, se, ω) : se;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        lc_γ_to(gate, be);
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
    case TT_SCAN: {
        if (t->n < 2) sno_fatal("TT_SCAN with missing subject or pattern", NULL);
        IR_t * e = sno_lower_match(cx, t, NULL, 0, γ, ω, NULL);
        if (res) *res = NULL;
        return e;
    }
    case TT_ASSIGN: {
        const tree_t * L = (t->n > 0) ? t->c[0] : NULL; const tree_t * R = (t->n > 1) ? t->c[1] : NULL;
        if (!L) sno_fatal("TT_ASSIGN with no lhs", NULL);
        if (!R) sno_fatal("TT_ASSIGN with no rhs", NULL);
        if (L->t == TT_SCAN && L->n >= 2) {
            IR_t * e = sno_lower_match(cx, L, R, 1, γ, ω, NULL);
            if (res) *res = NULL;
            return e;
        }
        if (L->t == TT_SEQ && L->n >= 2) {
            extern tree_t * ast_stmt_new(tree_e kind);
            const tree_t * pat = (L->n == 2) ? L->c[1] : NULL;
            if (!pat) { tree_t * ps = ast_stmt_new(TT_SEQ); for (int k = 1; k < L->n; k++) ast_push(ps, (tree_t *) L->c[k]); pat = ps; }
            tree_t * sc = ast_stmt_new(TT_SCAN); ast_push(sc, (tree_t *) L->c[0]); ast_push(sc, (tree_t *) pat);
            IR_t * e = sno_lower_match(cx, sc, R, 1, γ, ω, NULL);
            if (res) *res = NULL;
            return e;
        }
        if (L->t == TT_VAR && L->v.sval) {
            sno_reg_var(L->v.sval);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = L->v.sval;
            if (R->t == TT_SCAN && R->n == 2 && R->c[0] && R->c[1]) {
                extern tree_t * ast_stmt_new(tree_e kind);
                tree_t * capt = ast_stmt_new(TT_CAPT_COND_ASGN);
                ast_push(capt, (tree_t *) R->c[1]);
                tree_t * tv = ast_node_new(TT_VAR); tv->v.sval = L->v.sval;
                ast_push(capt, tv);
                tree_t * sc = ast_stmt_new(TT_SCAN);
                ast_push(sc, (tree_t *) R->c[0]);
                ast_push(sc, capt);
                IR_t * e = sno_lower_match(cx, sc, NULL, 0, γ, ω, NULL);
                if (res) *res = NULL;
                return e;
            }
            IR_t * vr = NULL; IR_t * e = sx_lower(cx, R, asn, ω, &vr);
            ir_operand_push(asn, vr);
            if (res) *res = asn;
            return e;
        }
        if (L->t == TT_INDIRECT && L->n > 0) {
            IR_t * nv = NULL; IR_t * e1 = sx_nameval(cx, L->c[0], NULL, ω, &nv);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
            lc_γ_to(nv, e2);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, nv); ir_operand_push(asn, vv);
            if (res) *res = asn;
            return e1;
        }
        if (L->t == TT_IDX && L->n >= 2) {
            IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(cx, L->c[0], (const tree_t * const *) &L->c[1], L->n - 1, ω, &vr);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
            lc_γ_to(vr, e2);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, vr); ir_operand_push(asn, vv);
            if (res) *res = asn;
            return e1;
        }
        if (L->t == TT_FNC) {
            const char * fname = L->v.sval; int argbase = 0;
            if (!fname && L->n > 0 && L->c[0] && L->c[0]->t == TT_VAR) { fname = L->c[0]->v.sval; argbase = 1; }
            int fnargs = L->n - argbase;
            if (fname && !strcmp(fname, "ITEM") && fnargs >= 2) {
                IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(cx, L->c[argbase], (const tree_t * const *) &L->c[argbase + 1], fnargs - 1, ω, &vr);
                IR_t * vv = NULL; IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
                lc_γ_to(vr, e2);
                IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, vr); ir_operand_push(asn, vv);
                if (res) *res = asn;
                return e1;
            }
            { extern int rt_dat_field_of_any(const char *);
              if (fname && fnargs == 1 && rt_dat_field_of_any(fname)) {
                  IR_t * br = NULL; IR_t * e1 = sx_lower(cx, L->c[argbase], NULL, ω, &br);
                  IR_t * fv = lc_build(cx->g, IR_FIELD_VAR, NULL, ω); IR_LIT(fv).sval = (char *) lp_strdup(fname);
                  lc_γ_to(br, fv);
                  ir_operand_push(fv, br);
                  IR_t * vv = NULL; IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
                  lc_γ_to(fv, e2);
                  IR_t * asn = lc_build(cx->g, IR_ASSIGN_VAR, γ, ω);
                  lc_γ_to(vv, asn);
                  ir_operand_push(asn, fv); ir_operand_push(asn, vv);
                  if (res) *res = asn;
                  return e1;
              } }
        }
        if (L->t == TT_KEYWORD && L->v.sval && sno_kw_static_slot(L->v.sval) >= 0) {
            IR_t * kv = NULL; IR_t * ke = sx_lower(cx, R, NULL, ω, &kv);
            IR_t * kw = lc_build(cx->g, IR_KEYWORD_ASSIGN_SNOBOL4, γ, ω); IR_LIT(kw).sval = L->v.sval;
            lc_γ_to(kv, kw);
            ir_operand_push(kw, kv);
            if (res) *res = kw;
            return ke;
        }
        if (L->t == TT_KEYWORD && L->v.sval) {
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$KWSET";
            IR_t * nl = lc_build(cx->g, IR_LIT_STRING, NULL, ω); IR_LIT(nl).sval = L->v.sval;
            IR_t * vv = NULL; IR_t * e2 = sx_lower(cx, R, NULL, ω, &vv);
            lc_γ_to(nl, e2);
            lc_γ_to(vv, mk);
            ir_operand_push(mk, nl); ir_operand_push(mk, vv);
            if (res) *res = mk;
            return nl;
        }
        sno_fatal("TT_ASSIGN lhs form outside the landed subset (VAR/INDIRECT/IDX/ITEM/DATA-field/KEYWORD)", NULL);
        return NULL;
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
    case TT_GOTO_U: case TT_GOTO_S: case TT_GOTO_F: {
        const char * nm = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : t->v.sval;
        if (!nm || !nm[0]) sno_fatal("goto with no resolvable label", NULL);
        IR_t * land = bb_label_landing(nm);
        if (!land) sno_fatal("goto to unknown label", nm);
        IR_t * taken = lc_build(cx->g, IR_GOTO, land, NULL);
        if (res) *res = NULL;
        if (t->t == TT_GOTO_U) return taken;
        return lc_build(cx->g, IR_GOTO, (t->t == TT_GOTO_S) ? taken : γ, (t->t == TT_GOTO_S) ? γ : taken);
    }
    case TT_CAPT_CURSOR: {
        const tree_t * tgt = (t->n > 0) ? t->c[0] : NULL;
        if (!tgt || tgt->t != TT_VAR || !tgt->v.sval) sno_fatal("@ cursor-position capture target is not a simple variable", NULL);
        sno_reg_var(tgt->v.sval);
        IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$PCUR";
        IR_t * nl = lc_build(cx->g, IR_LIT_STRING, mk, ω); IR_LIT(nl).sval = (char *) tgt->v.sval;
        ir_operand_push(mk, nl);
        if (res) *res = mk; return nl;
    }
    case TT_VLIST: {
        /* SPITBOL parenthesised value list (e1, e2, ...) -- failure-driven alternation IN AN EXPRESSION: the value is the
           first arm that SUCCEEDS.  This is the ONLY SNOBOL4 form that catches a failure part-way through an expression and
           then resumes inside that same expression, so it is the only one that must put the zeta spine back where it found
           it: a box concedes on omega WITHOUT popping the operand cells it already carved (zdp_out_omega models exactly
           that), and normally the statement boundary is what cleans up.  Re-entering a sibling arm skips that boundary, so
           the enclosing concatenation would read cells belonging to the abandoned arm -- measured as 'p' (IDENT(y) 5, 9)
           yielding "9" instead of "p9" under cell-stack, while frame-rsp / cell-heap (static offsets) were already right.
           The cure is NOT a spine mark/restore (IR_BOUND/IR_UNMARK was tried and is the wrong tool): the real blocker is
           zd_plan() in the emitter, which claims ZETA DEPTH only along LINEAR gamma RUNS starting at statement heads.
           Arm 2 is reachable only through an omega edge, so it is never claimed, gets zd_on=0, and its boxes address
           cells at stale static offsets (measured: no `sub rsp,16` at the arm-2 head, writing [rsp+80] off a base that
           had already been unwound to the statement frontier).  Teaching zd_plan about a second entry into the middle
           of an expression is the actual rung -- see FINDING-2026-08-23-hq_C-vlist-expr-alternation. */
        static int g_vlist_alt = -1; if (g_vlist_alt < 0) { const char * e = getenv("SCRIP_VLIST_ALT"); g_vlist_alt = (e && *e != '0') ? 1 : 0; }   /* STILL OFF BY DEFAULT: turning it on trades "statement fails" for SILENT WRONG VALUES under cell-stack -- see the comment above */
        if (!g_vlist_alt || t->n <= 1) { const tree_t * first = (t->n > 0) ? t->c[0] : NULL; return sx_lower(cx, first, γ, ω, res); }
        static int g_vlist_n = 0; char nmb[24]; snprintf(nmb, sizeof nmb, "VLIST$%d", g_vlist_n++); char * tmpn = (char *) lp_strdup(nmb); sno_reg_var(tmpn);
        IR_t * jn = lc_build(cx->g, IR_VAR, γ, ω); IR_LIT(jn).sval = tmpn;
        IR_t * head = NULL; IR_t * nxt = ω;
        for (int i = t->n - 1; i >= 0; i--) { IR_t * asn = lc_build(cx->g, IR_ASSIGN, jn, ω); IR_LIT(asn).sval = tmpn; IR_t * vr = NULL; head = sx_lower(cx, t->c[i], asn, nxt, &vr); ir_operand_push(asn, vr); nxt = head; }
        if (res) *res = jn; return head;
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
        if (a->n > 0 && a->c[0] && a->c[0]->t == TT_INDIRECT && a->c[0]->n > 0 && a->c[0]->c[0] && a->c[0]->c[0]->t == TT_VAR && a->c[0]->c[0]->v.sval) { const char * v = a->c[0]->c[0]->v.sval; sno_reg_var(v); size_t ln = strlen(v); char * o = (char *) rt_ws_alloc(ln + 2); o[0] = '$'; memcpy(o + 1, v, ln); o[ln + 1] = 0; return o; }
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
static IR_t * sno_goto_target(IR_graph_t * g, const char * nm, IR_t * exitnd) {
    IR_t * l = (nm && nm[0] != '$') ? bb_label_landing(nm) : NULL;
    if (l) return l;
    if (!nm || !nm[0]) sno_fatal("goto to unknown label", "?");
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL);
    IR_LIT(gd).sval = lp_strdup(nm);
    return gd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_computed_target(IR_graph_t * g, scx_t * cx, const tree_t * expr, IR_t * exitnd) {
    static int g_igt_n = 0;
    char nmb[24]; snprintf(nmb, sizeof nmb, "IGT$%d", g_igt_n++);
    char * tmpn = lp_strdup(nmb); sno_reg_var(tmpn);
    size_t ln = strlen(tmpn); char * dn = (char *) rt_ws_alloc(ln + 2); dn[0] = '$'; memcpy(dn + 1, tmpn, ln); dn[ln + 1] = 0;
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL); IR_LIT(gd).sval = dn;
    IR_t * asn = lc_build(g, IR_ASSIGN, gd, gd); IR_LIT(asn).sval = tmpn;
    IR_t * vr = NULL; IR_t * ec = sx_lower(cx, expr, asn, gd, &vr);
    ir_operand_push(asn, vr);
    return ec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_goto_direct_target(IR_graph_t * g, scx_t * cx, const tree_t * expr, IR_t * exitnd) {
    static int g_dgt_n = 0;
    char nmb[24]; snprintf(nmb, sizeof nmb, "DGT$%d", g_dgt_n++);
    char * tmpn = lp_strdup(nmb); sno_reg_var(tmpn);
    size_t ln = strlen(tmpn); char * dn = (char *) rt_ws_alloc(ln + 2); dn[0] = '<'; memcpy(dn + 1, tmpn, ln); dn[ln + 1] = 0;
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, NULL); IR_LIT(gd).sval = dn;
    IR_t * asn = lc_build(g, IR_ASSIGN, gd, gd); IR_LIT(asn).sval = tmpn;
    IR_t * vr = NULL; IR_t * ec = sx_lower(cx, expr, asn, gd, &vr);
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
        const char * a = sno_qlit_fold(t->c[0]); if (!a) return NULL;
        const char * b = sno_qlit_fold(t->c[1]); if (!b) return NULL;
        size_t la = strlen(a), lb = strlen(b);
        char * o = (char *) rt_ws_alloc(la + lb + 1);
        memcpy(o, a, la); memcpy(o + la, b, lb); o[la + lb] = 0;
        return o;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_stmt_define(const tree_t * s, int * out_argbase) {
    const tree_t * subj = lc_stmt_subj(s);
    if (!subj || subj->t != TT_FNC) return NULL;
    const char * name = subj->v.sval; int argbase = 0;
    if (!name && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR) { name = subj->c[0]->v.sval; argbase = 1; }
    if (!name || strcmp(name, "DEFINE")) return NULL;
    if (sfind(s, ":eq") || sfind_expr(s, ":pat")) sno_fatal("DEFINE with a pattern or replacement field is outside the landed subset", NULL);
    if (subj->n <= argbase || !subj->c[argbase] || !sno_qlit_fold(subj->c[argbase]))
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
    d->nformals = d->nnames;
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
        if (k < nidx - 1) sx_sub_container_only(sub);
        lc_γ_to(ir, sub);
        ir_operand_push(sub, cur); ir_operand_push(sub, ir);
        cur = sub;
    }
    if (var_res) *var_res = cur;
    return entry;
}
extern int ir_is_generator_kind(IR_e t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_ω_to(IR_t * nd, IR_t * t) { if (t) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_resume_ω_to(IR_graph_t * g, int tail_idx, IR_t * nd, IR_t * t) {
    if (nd && nd->op == IR_MATCH_ALTERNATE && nd->n_operands == 0 && g && tail_idx + 1 < g->n) {
        IR_t * T = g->all[tail_idx + 1];
        if (T && T->op == IR_MATCH_ALTERNATE && T->n_operands > 0 && T->operands[0] == nd) {
            if (t) lc_γ_to_β(T, t); else lc_γ_to(T, t);
            return;
        }
    }
    if (nd && nd->op == IR_MATCH_ASSIGN_COND && nd->n_operands > 1 && nd->operands[1]) nd = nd->operands[1];
    sno_ω_to(nd, t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_e sno_pat_eff_kind(const tree_t * t) {
    if (!t) return TT_VAR;
    if (t->t != TT_VAR || !t->v.sval) return t->t;
    static const struct { const char * n; tree_e k; } m[] = {
        { "ABORT", TT_ABORT }, { "ARB",  TT_ARB  }, { "BAL", TT_BAL }, { "FAIL", TT_FAIL },
        { "FENCE", TT_FENCE }, { "REM",  TT_REM  }, { "SUCCEED", TT_SUCCEED }, { NULL, TT_VAR }
    };
    for (int i = 0; m[i].n; i++) if (!strcmp(t->v.sval, m[i].n)) return m[i].k;
    return TT_VAR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_fence(const tree_t * t) { return t && sno_pat_eff_kind(t) == TT_FENCE; }
static int sno_is_fence1(const tree_t * t) { return t && t->t == TT_FENCE && t->n > 0; }
static int sno_is_fence0(const tree_t * t) { return sno_is_fence(t) && !sno_is_fence1(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_seq_flatten_pat(const tree_t * t, const tree_t ** elems, int * ne) {
    if (!t) return;
    if (t->t == TT_SEQ) { sno_seq_flatten_pat((t->n > 0) ? t->c[0] : NULL, elems, ne); if (t->n > 1 && t->c[1]) { if (*ne >= 128) sno_fatal("pattern sequence too long (SN4-PAT cap 128)", NULL); elems[(*ne)++] = t->c[1]; } return; }
    if (*ne >= 128) sno_fatal("pattern sequence too long (SN4-PAT cap 128)", NULL);
    elems[(*ne)++] = t;
}
static const tree_t * sno_const_pat(const char * ck);
static const tree_t * sno_fz_tree(const char * var);
static const tree_t * sno_var_val(const char * nm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_cset_fold(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT) return a->v.sval ? a->v.sval : "";
    if (a->t == TT_ILIT) { char nb[24]; snprintf(nb, sizeof nb, "%lld", (long long) a->v.ival); char * ob = (char *) malloc(strlen(nb) + 1); if (!ob) return NULL; strcpy(ob, nb); return ob; }
    if (a->t == TT_VAR && a->v.sval) { static int _vf = -1; if (_vf < 0) { const char * e = getenv("SCRIP_PAT_INLINE"); _vf = (!e || *e != '0') ? 1 : 0; } if (_vf) { const tree_t * vv = sno_var_val(a->v.sval); if (vv && vv != a) return sno_cset_fold(vv); const tree_t * vt = sno_fz_tree(a->v.sval); if (vt && vt != a) return sno_cset_fold(vt); } return NULL; }
    if (a->t == TT_KEYWORD && a->v.sval) {
        static const struct { const char * n; const char * v; } kc[] = { { "lcase", "abcdefghijklmnopqrstuvwxyz" }, { "ucase", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" } };
        char lk[16]; size_t li = 0; for (; a->v.sval[li] && li < sizeof lk - 1; li++) lk[li] = (a->v.sval[li] >= 'A' && a->v.sval[li] <= 'Z') ? (char)(a->v.sval[li] - 'A' + 'a') : a->v.sval[li]; lk[li] = 0;
        for (size_t k = 0; k < sizeof kc / sizeof *kc; k++) if (!strcmp(lk, kc[k].n)) return kc[k].v;
        { char cb[130]; snprintf(cb, sizeof cb, "&%s", a->v.sval[0] == '&' ? a->v.sval + 1 : a->v.sval); const tree_t * cv = sno_const_val(cb); if (cv) return sno_cset_fold(cv); const tree_t * cp = sno_const_pat(cb); if (cp && cp != a) return sno_cset_fold(cp); }
    }
    if (a->t == TT_FNC && a->v.sval && (!strcmp(a->v.sval, "CHAR") || !strcmp(a->v.sval, "char")) && a->n == 1 && a->c[0] && a->c[0]->t == TT_ILIT && a->c[0]->v.ival >= 1 && a->c[0]->v.ival <= 255) {
        char * cb = (char *) malloc(2); if (!cb) return NULL; cb[0] = (char)(unsigned char) a->c[0]->v.ival; cb[1] = 0; return cb;
    }
    if (a->t == TT_SEQ || a->t == TT_CAT) {
        const char * l = sno_cset_fold((a->n > 0) ? a->c[0] : NULL); if (!l) return NULL;
        const char * r = sno_cset_fold((a->n > 1) ? a->c[1] : NULL); if (!r) return NULL;
        size_t ln = strlen(l), rn = strlen(r); char * buf = (char *) malloc(ln + rn + 1); if (!buf) return NULL;
        memcpy(buf, l, ln); memcpy(buf + ln, r, rn); buf[ln + rn] = 0; return buf;
    }
    return NULL;
}
static int sno_is_pattern_rhs(const tree_t * t);
static int sno_pat_supported(const tree_t * t);
static int sno_pat_contains_arbno(const tree_t * t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_arbno_chain_on(void) { return rt_zeta_port_mode() == ZC_PORT_FORTH; }
static const char * sno_pat_collect(const tree_t * pat);
static struct { const char * var; const char * procname; const tree_t * pat; } g_sno_fz[SNO_PAT_MAX];
static int g_sno_nfz = 0;
static int g_sno_fz_unsafe = 0;
static int g_sno_in_patproc = 0;
static int g_sno_pat_match_ctx = 0;
#define SNO_FZW_MAX 256
static const char * g_sno_fzw_name[SNO_FZW_MAX];
static int g_sno_fzw_cnt[SNO_FZW_MAX];
static int g_sno_nfzw = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fz_write(const char * nm) {
    if (!nm || !nm[0]) return;
    int i = 0; for (; i < g_sno_nfzw; i++) if (!strcmp(g_sno_fzw_name[i], nm)) break;
    if (i == g_sno_nfzw) { if (g_sno_nfzw >= SNO_FZW_MAX) { g_sno_fz_unsafe = 1; return; } g_sno_fzw_name[i] = nm; g_sno_fzw_cnt[i] = 0; g_sno_nfzw++; }
    g_sno_fzw_cnt[i]++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_fz_wrcount(const char * nm) { for (int i = 0; i < g_sno_nfzw; i++) if (!strcmp(g_sno_fzw_name[i], nm)) return g_sno_fzw_cnt[i]; return 0; }
static int g_sno_seal_enabled = 0;
static struct { const char * name; const tree_t * pat; const tree_t * val; } g_sno_seal[SNO_PAT_MAX]; static int g_sno_nseal = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_seal_note(const char * nm, const tree_t * pat) { if (!nm || !pat || g_sno_nseal >= SNO_PAT_MAX) return; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return; g_sno_seal[g_sno_nseal].name = nm; g_sno_seal[g_sno_nseal].pat = pat; g_sno_nseal++; }
static const tree_t * sno_seal_pat(const char * nm) { if (!g_sno_seal_enabled || !nm || (g_sno_fz_unsafe && !getenv("SCRIP_FZ_FORCE")) || sno_fz_wrcount(nm) != 1) return NULL;    for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return g_sno_seal[i].pat; return NULL; }
static int sno_pat_right_sealed(const tree_t * t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_const_feature(int set_off) { static int _cs = -1; if (set_off) { _cs = 0; return 0; } if (_cs < 0) { const char * e = getenv("SCRIP_CONST_STATIC"); _cs = (e && *e == '0') ? 0 : 1; } return _cs; }
static int sno_const_static_on(void) { return sno_const_feature(0); }
static const tree_t * sno_const_pat(const char * ck) { if (!sno_const_static_on() || !g_sno_seal_enabled || !ck) return NULL; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, ck)) return g_sno_seal[i].pat; return NULL; }
static int sno_const_t1_on(void) { static int _t = -1; if (_t < 0) { const char * e = getenv("SCRIP_CONST_T1"); _t = (e && *e == '0') ? 0 : 1; } return _t; }
static int sno_const_scalar_tree(const tree_t * t) { return t && (t->t == TT_ILIT || t->t == TT_FLIT || t->t == TT_QLIT); }
static void sno_const_note_val(const char * nm, const tree_t * val) { if (!nm || !val || g_sno_nseal >= SNO_PAT_MAX) return; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return; g_sno_seal[g_sno_nseal].name = nm; g_sno_seal[g_sno_nseal].val = val; g_sno_nseal++; }
static const tree_t * sno_const_val(const char * ck) { if (!sno_const_static_on() || !sno_const_t1_on() || !g_sno_seal_enabled || !ck) return NULL; if (rt_kw_index(ck) >= 0) return NULL; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, ck)) return g_sno_seal[i].val; return NULL; }
static const tree_t * sno_var_val(const char * nm) { if (!g_sno_seal_enabled || !nm || nm[0] == '&' || g_sno_fz_unsafe || sno_fz_wrcount(nm) != 1) return NULL; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return g_sno_seal[i].val; return NULL; }
static int sno_defer_sealed(const char * nm) { const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_right_sealed(p) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_dfree(const tree_t * t, int spine, int depth) {
    if (!t) return 1;
    if (depth > 48) return 0;
    switch (t->t) {
    case TT_DEFER: return 0;
    case TT_QLIT: case TT_ILIT: case TT_FLIT: case TT_CSET: case TT_NUL: return 1;
    case TT_REM: case TT_ARB: case TT_FAIL: case TT_SUCCEED: case TT_ABORT: case TT_BAL: return 1;
    case TT_VAR: { const char * nm = t->v.sval; if (!spine) return 1; if (nm && (!strcmp(nm, "REM") || !strcmp(nm, "ARB") || !strcmp(nm, "FENCE"))) return 1; const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_dfree(p, 1, depth + 1) : 0; }
    case TT_KEYWORD: { if (!sno_const_static_on()) return 0; if (!spine) return 1; if (!t->v.sval) return 0; char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval); const tree_t * p = sno_const_pat(cb); return p ? sno_pat_dfree(p, 1, depth + 1) : 0; }
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX: case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS: { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 0, depth + 1)) return 0; return 1; }
    case TT_SEQ: case TT_CAT: case TT_ALT: case TT_FENCE: case TT_ARBNO: { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 1, depth + 1)) return 0; return 1; }
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: case TT_CAPT_CURSOR: return 0;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_name_static(const char * nm) { const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_dfree(p, 1, 0) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fz_scan(const tree_t * t) {
    if (!t) return;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) sno_fz_write(t->c[1]->v.sval);
    if (t->t == TT_CAPT_CURSOR && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) sno_fz_write(t->c[0]->v.sval);
    if (t->t == TT_SWAP || t->t == TT_REVSWAP) for (int i = 0; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR) sno_fz_write(t->c[i]->v.sval);
    if (t->t == TT_FNC && t->v.sval) { const char * fn = t->v.sval;
        if (!strcmp(fn, "EVAL") || !strcmp(fn, "eval") || !strcmp(fn, "CODE") || !strcmp(fn, "code") || !strcmp(fn, "CONVERT") || !strcmp(fn, "convert")) g_sno_fz_unsafe = 1;
        if (!strcmp(fn, "CLEAR") || !strcmp(fn, "clear")) g_sno_fz_unsafe = 1;
        if (!strcmp(fn, "INPUT") || !strcmp(fn, "input")) { const tree_t * a0 = (t->n > 0) ? t->c[0] : NULL; const char * an = NULL;
            if (a0 && a0->t == TT_NAME && a0->n > 0 && a0->c[0] && a0->c[0]->t == TT_VAR) an = a0->c[0]->v.sval;
            else if (a0 && a0->t == TT_VAR) an = a0->v.sval;
            else if (a0 && a0->t == TT_QLIT) an = a0->v.sval;
            if (an && an[0]) { sno_fz_write(an); sno_fz_write(an); } else g_sno_fz_unsafe = 1; } }
    for (int i = 0; i < t->n; i++) sno_fz_scan(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fz_mark_defer(IR_graph_t * g, IR_t * nd, const char * nm) {
    if (g_sno_in_patproc || !nm) return;
    for (int i = 0; i < g_sno_nfz; i++) if (!strcmp(g_sno_fz[i].var, nm)) {
        IR_t * pl = lc_build(g, IR_LIT_STRING, NULL, NULL); IR_LIT(pl).sval = (char *) g_sno_fz[i].procname; ir_operand_push(nd, pl); return; }
}
static const char * g_sno_pro[128];
static int g_sno_npro = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prologue_add(const char * nm) { if (!nm || g_sno_npro >= 128) return; for (int i = 0; i < g_sno_npro; i++) if (!strcmp(g_sno_pro[i], nm)) return; g_sno_pro[g_sno_npro++] = nm; }
int sno_name_prologue_bound(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_sno_npro; i++) if (!strcmp(g_sno_pro[i], nm)) return 1; return 0; }
static const char * g_sno_encl[SNO_PAT_MAX]; static int g_sno_nencl = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_encl_add(const char * nm) { if (!nm) return;
    for (int i = 0; i < g_sno_nencl; i++) if (!strcmp(g_sno_encl[i], nm)) return; if (g_sno_nencl < SNO_PAT_MAX) g_sno_encl[g_sno_nencl++] = nm; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_encl_mark_all(const tree_t * t) { if (!t) return; if (t->t == TT_VAR && t->v.sval) sno_encl_add(t->v.sval); for (int i = 0; i < t->n; i++) sno_encl_mark_all(t->c[i]); }
static void sno_encl_scan(const tree_t * t) { if (!t) return; if (t->t == TT_ARBNO || sno_is_fence(t)) { sno_encl_mark_all(t); return; } for (int i = 0; i < t->n; i++) sno_encl_scan(t->c[i]); }
static int sno_encl_hostile(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_sno_nencl; i++) if (!strcmp(g_sno_encl[i], nm)) return 1; return 0; }
typedef struct { const char * op; const char * tgt; int arity; int poisoned; } sno_t4_t;
static sno_t4_t g_sno_t4[16];
static int g_sno_nt4 = 0, g_sno_t4_unsafe = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_t4_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_OPSYN_FOLD"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sno_t4_opchar(char c) { return !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '.'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_t4_scan(const tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) sno_t4_scan(t->c[i]);
    if (t->t != TT_FNC) return;
    const char * nm = t->v.sval; int ab = 0;
    if (!nm && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { nm = t->c[0]->v.sval; ab = 1; }
    if (!nm || strcmp(nm, "OPSYN")) return;
    { int na = t->n - ab;
      const tree_t * a0 = (na > 0) ? t->c[ab] : NULL;
      const tree_t * a1 = (na > 1) ? t->c[ab + 1] : NULL;
      const tree_t * a2 = (na > 2) ? t->c[ab + 2] : NULL;
      const char * op = (a0 && a0->t == TT_QLIT) ? a0->v.sval : NULL;
      const char * tg = (a1 && a1->t == TT_QLIT) ? a1->v.sval : NULL;
      long ar = (a2 && a2->t == TT_ILIT) ? a2->v.ival : -1;
      if (op && tg && *op && sno_t4_opchar(*op)) {
          if (na == 3 && (ar == 1 || ar == 2)) {
              for (int i = 0; i < g_sno_nt4; i++) if (!strcmp(g_sno_t4[i].op, op)) {
                  if (strcmp(g_sno_t4[i].tgt, tg) || g_sno_t4[i].arity != (int) ar) g_sno_t4[i].poisoned = 1;
                  return; }
              if (g_sno_nt4 < 16) { g_sno_t4[g_sno_nt4].op = op; g_sno_t4[g_sno_nt4].tgt = tg; g_sno_t4[g_sno_nt4].arity = (int) ar; g_sno_t4[g_sno_nt4].poisoned = 0; g_sno_nt4++; }
              else g_sno_t4_unsafe = 1;
              return; }
          g_sno_t4_unsafe = 1;
          return; }
      if (op && tg) return;
      g_sno_t4_unsafe = 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_t4_target(const char * op, int nops) {
    if (!sno_t4_on() || g_sno_t4_unsafe || !op) return NULL;
    for (int i = 0; i < g_sno_nt4; i++)
        if (!g_sno_t4[i].poisoned && !strcmp(g_sno_t4[i].op, op) && g_sno_t4[i].arity == nops) return g_sno_t4[i].tgt;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fz_build_table(const tree_t ** st, int nst) {
    g_sno_nfz = 0; g_sno_fz_unsafe = 0; g_sno_nfzw = 0; g_sno_npro = 0; g_sno_nencl = 0;
    g_sno_nt4 = 0; g_sno_t4_unsafe = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i]; if (!s) continue;
        sno_encl_scan(s);
        sno_t4_scan(s);
        const tree_t * subj = lc_stmt_subj(s); const tree_t * pat = sfind_expr(s, ":pat"); const tree_t * repl = sfind_expr(s, ":repl"); int has_eq = sfind(s, ":eq") != NULL;
        if (pat) sno_fz_scan(pat);
        if (repl) sno_fz_scan(repl);
        if (subj) sno_fz_scan(subj);
        if (subj && subj->t == TT_DEFINE && subj->n > 1 && subj->c[1] && subj->c[1]->t == TT_QLIT && subj->c[1]->v.sval) {
            sno_def_t d; sno_parse_define(subj->c[1]->v.sval, NULL, &d); for (int k = 0; k < d.nnames; k++) sno_fz_write(d.names[k]); if (d.fname) sno_fz_write(d.fname); continue; }
        int argbase = 0; const tree_t * dsub = sno_stmt_define(s, &argbase);
        if (dsub) { sno_def_t d; sno_parse_define(sno_qlit_fold(dsub->c[argbase]), NULL, &d); for (int k = 0; k < d.nnames; k++) sno_fz_write(d.names[k]); if (d.fname) sno_fz_write(d.fname); continue; }
        if (!has_eq) continue;
        if (subj && subj->t == TT_SCAN) { const tree_t * sv = (subj->n > 0) ? subj->c[0] : NULL; if (sv && sv->t == TT_VAR && sv->v.sval) sno_fz_write(sv->v.sval); else g_sno_fz_unsafe = 1; continue; }
        if (subj && subj->t == TT_VAR && subj->v.sval) {
            sno_fz_write(subj->v.sval);
            if (repl && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) sno_seal_note(subj->v.sval, repl);
            if (repl && repl->t == TT_QLIT && subj->v.sval && subj->v.sval[0] != '&') sno_const_note_val(subj->v.sval, repl);
            if (repl && g_sno_nfz < SNO_PAT_MAX && sno_is_pattern_rhs(repl) && sno_pat_supported(repl) && sno_pat_invariant(repl)) {
                g_sno_fz[g_sno_nfz].var = subj->v.sval; g_sno_fz[g_sno_nfz].pat = repl; g_sno_fz[g_sno_nfz].procname = NULL; g_sno_nfz++; }
            continue; }
        if (subj && subj->t == TT_KEYWORD) { if (subj->v.sval && repl) { const char * kn = subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval; if (!strcasecmp(kn, "USER_DECLARED_CONSTANTS") && repl->t == TT_ILIT && repl->v.ival == 0) sno_const_feature(1); }
            if (sno_const_static_on() && subj->v.sval && repl && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) { char cb[130]; snprintf(cb, sizeof cb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval); sno_seal_note(lp_strdup(cb), repl); }
            if (sno_const_static_on() && sno_const_t1_on() && subj->v.sval && sno_const_scalar_tree(repl)) { char vb[130]; snprintf(vb, sizeof vb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval); if (rt_kw_index(vb) < 0) sno_const_note_val(lp_strdup(vb), repl); } continue; }
        if (subj && subj->t == TT_IDX) continue;
        if (subj) g_sno_fz_unsafe = 1;
    }
    int keep = 0;
    for (int i = 0; i < g_sno_nfz; i++) if (!g_sno_fz_unsafe && sno_fz_wrcount(g_sno_fz[i].var) == 1) { g_sno_fz[keep] = g_sno_fz[i]; g_sno_fz[keep].procname = sno_pat_collect(g_sno_fz[i].pat); keep++; }
    g_sno_nfz = g_sno_fz_unsafe ? 0 : keep;
    if (getenv("SCRIP_FZ_DEBUG")) { fprintf(stderr, "[FZ5] unsafe=%d inlinable=%d\n", g_sno_fz_unsafe, g_sno_nfz);
        for (int i = 0; i < g_sno_nfz; i++) fprintf(stderr, "[FZ5]  %s -> %s\n", g_sno_fz[i].var, g_sno_fz[i].procname); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long sno_prearg_codes(int tt) {
    switch (tt) {
    case TT_ANY: return 59L | (59L << 16); case TT_BREAK: return 69L | (69L << 16); case TT_BREAKX: return 70L | (70L << 16); case TT_NOTANY: return 151L | (151L << 16); case TT_SPAN: return 188L | (188L << 16);
    case TT_LEN: return 120L | (121L << 16); case TT_POS: return 162L | (163L << 16); case TT_RTAB: return 181L | (182L << 16); case TT_TAB: return 183L | (184L << 16); case TT_RPOS: return 185L | (186L << 16);
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_pre_req(scx_t * cx, const tree_t * t, IR_t * prim) {
    const tree_t * arg = (t->n > 0) ? t->c[0] : NULL;
    if (arg && arg->t == TT_DEFER && arg->n > 0 && arg->c[0]) arg = arg->c[0];
    if (!arg || arg->t == TT_DEFER) sno_fatal("pattern primitive argument outside the operand-edge subset (missing or deferred *expr argument)", NULL);
    if (cx->npre >= 64) sno_fatal("too many runtime pattern-primitive arguments in one statement (operand-edge pre-chain limit 64)", NULL);
    cx->pre[cx->npre].arg = arg; cx->pre[cx->npre].prim = prim;
    cx->pre[cx->npre].str = (t->t == TT_ANY || t->t == TT_NOTANY || t->t == TT_SPAN || t->t == TT_BREAK || t->t == TT_BREAKX);
    cx->pre[cx->npre].codes = sno_prearg_codes(t->t); cx->pre[cx->npre].snapg = NULL; cx->npre++;
}
static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail);
static int sno_in_arbno = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_tree_has_varext(const IR_t * n, int d) { if (!n || d > 12) return 0; if (n->op == IR_MATCH_ARBNO || n->op == IR_MATCH_DEFER) return 1; for (int i = 0; i < n->n_operands; i++) if (sno_tree_has_varext(n->operands[i], d + 1)) return 1; return 0; }
static struct { const IR_t * nd; const IR_t * save; int nd_idx; int save_idx; int i_end; int fp_inner; } scd[64];
static int scd_n = 0;
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cap_defer_reset(void) { scd_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_cap_fc(IR_graph_t * g, IR_t * nd, IR_t * save, int before_i) {
    int fp_inner = 0; int walk_ok = fc_walk_range(g, before_i, g->n, 0, &fp_inner);
    if (!walk_ok) return;
    if (sno_in_arbno == 0) { extern void fc_save_register(const IR_t *); extern void fc_cond_register_with_save(const IR_t *, const IR_t *, int); fc_save_register(save); fc_cond_register_with_save(nd, save, fp_inner); return; }
    if (scd_n >= 64) return;
    scd[scd_n].nd = nd; scd[scd_n].save = save; scd[scd_n].nd_idx = before_i - 2; scd[scd_n].save_idx = before_i - 1; scd[scd_n].i_end = g->n; scd[scd_n].fp_inner = fp_inner; scd_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp) {
    extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_extent(const IR_t *, int *, int *);
    int lin = 1;
    for (int k = k0; k < k1; k++) {
        IR_t * x = g->all[k];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) {
            int _b = 0, _e = 0;
            if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > k + 1) k = _e - 1; continue; }
            lin = 0; continue;
        }
        { long fck; if (fc_geom(x, &fck)) { if (fp) *fp += (int)fck; continue; } }
        switch (x->op) {
        case IR_MATCH_LIT: case IR_MATCH_LEN: case IR_MATCH_ANY: case IR_MATCH_NOTANY:
        case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_ATP:
        case IR_MATCH_ASSIGN_SAVE: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_IMM:
        case IR_GOTO: break;
        case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL: if (!lit_ok) lin = 0; else if (fp) *fp += 16; break;
        default: lin = 0;
        }
    }
    return lin;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_tail_walk(IR_graph_t * g, int k0, int k1) {
    extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_extent(const IR_t *, int *, int *);
    for (int k = k0; k < k1 && k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) {
            int _b = 0, _e = 0;
            if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > k + 1) k = _e - 1; continue; }
            return 0;
        }
        if (x->op == IR_MATCH_DEFER) {
            static int _dtl = -1; if (_dtl < 0) { const char * _e = getenv("SCRIP_ARBNO_LATCH"); _dtl = _e ? (atoi(_e) != 0) : 0; }
            if (_dtl && x->seal == 2 && IR_LIT(x).sval && sno_name_prologue_bound(IR_LIT(x).sval)) {
                const char * _pn = 0; for (int _j = 0; _j < x->n_operands; _j++) { IR_t * _o = x->operands[_j]; if (_o && _o->op == IR_LIT_STRING && IR_LIT(_o).sval && !strncmp(IR_LIT(_o).sval, "PAT$", 4)) { _pn = IR_LIT(_o).sval; break; } }
                if (_pn) continue;
            }
            return 0;
        }
        { long fck; if (fc_geom(x, &fck)) continue; }
        switch (x->op) {
        case IR_MATCH_LIT: case IR_MATCH_LEN: case IR_MATCH_ANY: case IR_MATCH_NOTANY:
        case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_ATP:
        case IR_MATCH_ASSIGN_SAVE: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_IMM:
        case IR_GOTO: case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL: break;
        default: return 0;
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
static int sno_pat_contains_fence(const tree_t * t, int depth) { if (!t || depth > 64) return 0; if (sno_is_fence(t)) return 1; for (int i = 0; i < t->n; i++) if (sno_pat_contains_fence(t->c[i], depth + 1)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_seq_nary(scx_t * cx, const tree_t ** elems, int ne, IR_t * succ, IR_t * fail, IR_t ** out_rtail) {
    IR_graph_t * g = cx->g;
    IR_t * S = lc_build(g, IR_GOTO, succ, NULL);
    sno_ω_to(S, fail);
    IR_t * ent[128]; IR_t * res[128]; int lo[128];
    for (int i = 0; i < ne && ne < 128; i++) {
        int before = g->n;
        IR_t * ei = sno_pat_node(cx, elems[i], S, S);
        int _rb = before;
        while (_rb < g->n && g->all[_rb] && g->all[_rb]->op == IR_GOTO
               && g->all[_rb]->γ.node == S && g->all[_rb]->ω.node == S
               && g->all[_rb]->n_operands == 0) _rb++;
        IR_t * ri = (_rb < g->n) ? g->all[_rb] : ei;
        IR_t * ti = NULL;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == S) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S) { if (x->op == IR_GOTO && x->ω.node == S) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); ti = x; } x->γ.sz[3] = 0; }
        }
        if (sno_seq_tail() && ti) ri = ti;
        ent[i] = ei; res[i] = ri; lo[i] = before;
    }
    for (int i = 0; i < ne && ne < 128; i++) {
        IR_t * nxt = (i + 1 < ne) ? ent[i + 1] : succ;
        IR_t * prv = (i > 0 && (res[i - 1]->op != IR_MATCH_DEFER || (sno_defer_resume() && res[i - 1]->seal != 1))) ? res[i - 1] : fail;
        int lo_i = lo[i]; int hi_i = (i + 1 < ne) ? lo[i + 1] : g->n;
        for (int k = 0; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x || x == S) continue;
            if (k < lo_i || k >= hi_i) continue;
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
    S->γ.node = succ; S->γ.sz[0] = 0;
    if (out_rtail) *out_rtail = (ne > 0 && ne < 128) ? res[ne - 1] : NULL;
    return (ne > 0 && ne < 128) ? ent[0] : succ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_capt_name(const tree_t * tgt) {
    if (!tgt) return NULL;
    if (tgt->t == TT_VAR) return tgt->v.sval;
    if (tgt->t == TT_INDIRECT && tgt->n > 0 && tgt->c[0] && tgt->c[0]->t == TT_QLIT) return tgt->c[0]->v.sval;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_fz_tree(const char * nm) { if (!nm || g_sno_in_patproc || !g_sno_pat_match_ctx) return NULL; for (int i = 0; i < g_sno_nfz; i++) if (!strcmp(g_sno_fz[i].var, nm)) return g_sno_fz[i].pat; return NULL; }
static int sno_kw_chase(const char * nm, int op) { static const char * stk[24]; static int top = 0; if (op == 1) { if (nm && top < 24) { stk[top++] = nm; return 1; } return 0; } if (op == 2) { if (top > 0) top--; return 1; } if (op == 3) return top != 0; if (!nm) return 0; for (int i = 0; i < top; i++) if (!strcmp(stk[i], nm)) return 1; return 0; }
static int sno_kw_nest_ok(const char * nm) { static int _nn = -1; if (_nn < 0) { const char * e = getenv("SCRIP_CONST_NEST"); _nn = (e && *e == '0') ? 0 : 1; } return _nn ? !sno_kw_chase(nm, 0) : !sno_kw_chase((const char *)0, 3); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_inline_ok(const tree_t * t) {
    if (!t) return 1;
    switch (t->t) {
    case TT_QLIT: return 1;
    case TT_REM: return 1;
    case TT_VAR: return t->v.sval != NULL;
    case TT_KEYWORD: return t->v.sval != NULL;
    case TT_DEFER: return t->n > 0 && t->c[0] != NULL;
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX: return (t->n > 0) && sno_cset_fold(t->c[0]) != NULL;
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS: return (t->n > 0) && t->c[0] && t->c[0]->t == TT_ILIT;
    case TT_SEQ: case TT_CAT: case TT_ALT: { for (int i = 0; i < t->n; i++) if (!sno_pat_inline_ok(t->c[i])) return 0; return 1; }
    case TT_ARBNO: { static int _ia = -1; if (_ia < 0) { const char * e = getenv("SCRIP_PAT_INLINE_ARBNO"); _ia = (!e || *e != '0') ? 1 : 0; } return _ia && t->n > 0 && sno_pat_inline_ok(t->c[0]); }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_capt_body(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t ** out_itail) {
    IR_graph_t * g = cx->g;
    const tree_t * eff = t;
    if (eff && eff->t == TT_VAR && eff->v.sval) {
        static int _pi = -1; if (_pi < 0) { const char * e = getenv("SCRIP_PAT_INLINE"); _pi = (!e || *e != '0') ? 1 : 0; }
        if (_pi && !sno_encl_hostile(eff->v.sval)) { const tree_t * p = sno_fz_tree(eff->v.sval); if (p && sno_pat_inline_ok(p)) eff = p; }
    }
    if (sno_pat_eff_kind(eff) == TT_SEQ) {
        const tree_t * elems[128]; int ne = 0;
        sno_seq_flatten_pat(eff, elems, &ne);
        int nf = ne; for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { nf = i; break; }
        if (nf == ne && ne > 1) { IR_t * rt = NULL; IR_t * pe = sno_seq_nary(cx, elems, ne, succ, fail, &rt); *out_itail = rt ? rt : pe; return pe; }
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
        else sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_FAIL:    { IR_t * j = lc_build(g, IR_GOTO, NULL, NULL); sno_ω_to(j, fail); lc_γ_to(j, fail); return j; }
    case TT_SUCCEED: { IR_t * j = lc_build(g, IR_GOTO, succ, NULL); return j; }
    case TT_ABORT:   { IR_t * j = lc_build(g, IR_MATCH_ABORT, NULL, NULL); IR_t * k = cx->pat_seal ? cx->pat_seal : fail; sno_ω_to(j, k); lc_γ_to(j, k); return j; }
    case TT_SPAN: {
        IR_t * nd = lc_build(g, IR_MATCH_SPAN, succ, NULL);
        sno_ω_to(nd, fail);
        const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
        if (cs) { IR_LIT(nd).sval = (char *) cs; return nd; }
        { const tree_t * arg = (t->n > 0) ? t->c[0] : NULL;
          if (arg && arg->t == TT_VAR && arg->v.sval) {
              char pb[128]; snprintf(pb, sizeof pb, "*%s", arg->v.sval);
              IR_LIT(nd).sval = lp_strdup(pb);
              return nd;
          } }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_BREAK: case TT_BREAKX: {
        IR_t * nd = lc_build(g, (t->t == TT_BREAK) ? IR_MATCH_BREAK : IR_MATCH_BREAKX, succ, NULL);
        sno_ω_to(nd, fail);
        const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
        if (cs) IR_LIT(nd).sval = (char *) cs;
        else sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_TAB: case TT_RTAB: {
        IR_t * nd = lc_build(g, (t->t == TT_TAB) ? IR_MATCH_TAB : IR_MATCH_RTAB, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n <= 0 || !t->c[0]) sno_fatal("TAB/RTAB requires a count argument", NULL);
        if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
        if (t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_POS: case TT_RPOS: {
        IR_t * nd = lc_build(g, (t->t == TT_RPOS) ? IR_MATCH_RPOS : IR_MATCH_POS, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n <= 0 || !t->c[0]) sno_fatal("POS/RPOS requires a position argument", NULL);
        if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
        if (t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_FENCE:
        if (t->n > 0 && t->c[0] && !sno_in_arbno && !g_sno_in_patproc) {
            IR_t * F = lc_build(g, IR_MATCH_FENCE1, succ, NULL);
            sno_ω_to(F, fail);
            IR_LIT(F).ival = 1;
            int before_p = g->n;
            IR_t * pe = sno_pat_node(cx, t->c[0], F, F);
            IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
            for (int q = before_p; q < g->n; q++) { IR_t * x = g->all[q];
                if (!x) continue;
                if (x->ω.node == F) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                if (x->γ.node == F) { if (x->op == IR_GOTO && x->ω.node == F) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; } }
            ir_operand_push(F, pe);
            ir_operand_push(F, p_tail);
            return F;
        }
        return (t->n > 0 && t->c[0]) ? sno_pat_node(cx, t->c[0], succ, fail) : succ;
    case TT_DEFER: {
        const tree_t * in = (t->n > 0) ? t->c[0] : NULL;
        if (in && in->t == TT_VAR && in->v.sval) {
            { static int _p3 = -1; if (_p3 < 0) { const char * e = getenv("SCRIP_PAT_INLINE"); _p3 = (!e || *e != '0') ? 1 : 0; }
              if (_p3 && !sno_encl_hostile(in->v.sval)) { const tree_t * p = sno_fz_tree(in->v.sval); if (p && sno_pat_inline_ok(p)) return sno_pat_node(cx, p, succ, fail); } }
            IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = (char *) in->v.sval; sno_fz_mark_defer(g, nd, in->v.sval); nd->seal = sno_defer_sealed(in->v.sval) ? 1 : (sno_seal_pat(in->v.sval) ? 2 : 0);    nd->pat_static = sno_name_static(in->v.sval);    sno_ω_to(nd, fail); return nd; }
        if (in && in->t == TT_KEYWORD && in->v.sval) { static int _cn = -1; if (_cn < 0) { const char * e = getenv("SCRIP_CONST"); _cn = (e && *e == '0') ? 0 : 1; }
          if (_cn) { char cb[130]; snprintf(cb, sizeof cb, "&%s", in->v.sval[0] == '&' ? in->v.sval + 1 : in->v.sval);
            { static int _ci = -1; if (_ci < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); _ci = (e && *e == '0') ? 0 : 1; }
              if (_ci) { const tree_t * cp0 = sno_const_pat(cb); if (cp0 && g_sno_pat_match_ctx && !g_sno_in_patproc && sno_kw_nest_ok(cb) && sno_pat_inline_ok(cp0)) { char * ky0 = lp_strdup(cb); if (sno_kw_chase(ky0, 1)) { IR_t * r0 = sno_pat_node(cx, cp0, succ, fail); sno_kw_chase(NULL, 2); return r0; } } } }
            IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = lp_strdup(cb);
            { const tree_t * cp = sno_const_pat(cb); if (cp) nd->pat_static = sno_pat_dfree(cp, 1, 0); if (cp && !g_sno_fz_unsafe) nd->seal = 2; }
            sno_ω_to(nd, fail); return nd; } }
        { const char * bn = sno_expr_collect(in); char pb[40]; snprintf(pb, sizeof pb, "*%s", bn);
          IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = lp_strdup(pb); sno_ω_to(nd, fail); return nd; }
    }
    case TT_KEYWORD: {
        if (!t->v.sval) return succ;
        char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
        { const tree_t * cv = sno_const_val(cb); if (cv && cv->t == TT_QLIT) return sno_pat_node(cx, cv, succ, fail); }
        { static int _ck = -1; if (_ck < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); _ck = (e && *e == '0') ? 0 : 1; }
          if (_ck) { const tree_t * cp = sno_const_pat(cb); if (cp && g_sno_pat_match_ctx && !g_sno_in_patproc && sno_kw_nest_ok(cb) && sno_pat_inline_ok(cp)) { char * ky = lp_strdup(cb); if (sno_kw_chase(ky, 1)) { IR_t * r = sno_pat_node(cx, cp, succ, fail); sno_kw_chase(NULL, 2); return r; } } } }
        IR_t * mv = lc_build(g, IR_MATCH_DEFER, succ, NULL); sno_ω_to(mv, fail);
        if (cx->npre >= 0 && cx->npre < 64) { cx->pre[cx->npre].arg = t; cx->pre[cx->npre].prim = mv; cx->pre[cx->npre].str = 0; cx->pre[cx->npre].codes = 0; cx->pre[cx->npre].snapg = lp_strdup(cb); cx->npre++; }
        return mv;
    }
    case TT_VAR: {
        { static int _pi = -1; if (_pi < 0) { const char * e = getenv("SCRIP_PAT_INLINE"); _pi = (!e || *e != '0') ? 1 : 0; }
          if (_pi && !sno_encl_hostile(t->v.sval)) { const tree_t * p = sno_fz_tree(t->v.sval); if (p && sno_pat_inline_ok(p)) return sno_pat_node(cx, p, succ, fail); } }
        IR_t * mv = lc_build(g, IR_MATCH_DEFER, succ, NULL); sno_ω_to(mv, fail);
        if (cx->npre >= 0 && cx->npre < 64) { cx->pre[cx->npre].arg = t; cx->pre[cx->npre].prim = mv; cx->pre[cx->npre].str = 0; cx->pre[cx->npre].codes = 0; cx->pre[cx->npre].snapg = t->v.sval; cx->npre++; }
        return mv;
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
    case TT_BAL: {
        IR_t * nd = lc_build(g, IR_MATCH_BAL, succ, NULL);
        sno_ω_to(nd, fail);
        return nd;
    }
    case TT_ARBNO: {
        if (!(t->n > 0) || !t->c[0]) sno_fatal("ARBNO requires a pattern argument", NULL);
        if (sno_pat_contains_arbno(t->c[0]) && !sno_arbno_chain_on()) sno_fatal("nested ARBNO awaits the rsp iteration-frame chain (ZC_PORT_FORTH — GOAL-SNOBOL4-BB ZB-ITER-1a)", NULL);
        IR_t * R = lc_build(g, IR_MATCH_ARBNO, succ, NULL);
        sno_ω_to(R, fail);
        int before = g->n;
        IR_t * prev_seal = cx->pat_seal; cx->pat_seal = R;
        sno_in_arbno++;
        IR_t * ei = sno_pat_node(cx, t->c[0], R, R);
        sno_in_arbno--;
        cx->pat_seal = prev_seal;
        if (before >= g->n) sno_fatal("ARBNO body lowered to zero nodes (bare FENCE / null pattern body)", NULL);
        int _rb = before;
        while (_rb < g->n && g->all[_rb] && g->all[_rb]->op == IR_GOTO
               && g->all[_rb]->γ.node == R
               && g->all[_rb]->n_operands == 0) _rb++;
        IR_t * ri = (_rb < g->n) ? g->all[_rb] : ei;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == R) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == R) { if (x->op == IR_GOTO && x->ω.node == R) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
        }
        ir_operand_push(R, ei);
        ir_operand_push(R, ri);
        ir_operand_push(R, (g->n > before) ? g->all[g->n - 1] : ei);
        { const tree_t * belems[128]; int bne = 0; sno_seq_flatten_pat(t->c[0], belems, &bne);
          if (bne > 0 && sno_is_fence(belems[bne - 1])) ir_operand_push(R, R); }
        IR_LIT(R).ival = 1;
        return R;
    }
    case TT_LEN: {
        IR_t * nd = lc_build(g, IR_MATCH_LEN, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n <= 0 || !t->c[0]) sno_fatal("LEN requires a count argument", NULL);
        if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
        if (t->c[0]->t == TT_DEFER && t->c[0]->n > 0 && t->c[0]->c[0] && t->c[0]->c[0]->v.sval) {
            const tree_t * inner = t->c[0]->c[0];
            const char * vn = inner->v.sval;
            char pb[128]; snprintf(pb, sizeof pb, "*%s", vn ? vn : "");
            IR_LIT(nd).sval = lp_strdup(pb);
            return nd;
        }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_CAPT_COND_ASGN: {
        const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
        if (vn) sno_reg_var(vn);
        if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) { const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL; if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }
        if (!vn) { const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : (di && di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect(di); char pb[48]; snprintf(pb, sizeof pb, "*%s", bn); vn = lp_strdup(pb); } }
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
        {
            sno_cap_fc(g, nd, save, before_i);
        }
        { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(nd, g->n); }
        return save;
    }
    case TT_CAPT_IMMED_ASGN: {
        const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
        if (vn) sno_reg_var(vn);
        if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) { const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL; if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }
        if (!vn) { const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : (di && di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect(di); char pb[48]; snprintf(pb, sizeof pb, "*%s", bn); vn = lp_strdup(pb); } }
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
        {
            sno_cap_fc(g, nd, save, before_i);
        }
        { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(nd, g->n); }
        return save;
    }
    case TT_SEQ: {
        const tree_t * elems[128]; int ne = 0;
        sno_seq_flatten_pat(t, elems, &ne);
        int first_fence = ne; int first_f0 = ne;
        for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { first_fence = i; break; }
        for (int i = 0; i < ne; i++) if (sno_is_fence0(elems[i])) { first_f0 = i; break; }
        if (first_fence == ne)
            return ne == 1 ? sno_pat_node(cx, elems[0], succ, fail) : sno_seq_nary(cx, elems, ne, succ, fail, NULL);
        IR_t * cur_succ = succ; IR_t * right_tail = NULL; int right_tail_idx = -1; int right_sealed = 0;
        for (int i = ne - 1; i >= 0; ) {
            if (sno_is_fence(elems[i])) {
                const tree_t * inner = sno_is_fence1(elems[i]) ? elems[i]->c[0] : NULL;
                if (!inner || sno_in_arbno) right_sealed = 1;
                if (inner && sno_in_arbno) {
                    IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                    int f_idx = g->n;
                    IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                    sno_ω_to(F, fail_p);
                    IR_LIT(F).ival = 2;
                    int before_p = g->n;
                    IR_t * pe = sno_pat_node(cx, inner, F, F);
                    IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
                    for (int q = before_p; q < g->n; q++) { IR_t * x = g->all[q];
                        if (!x) continue;
                        if (x->ω.node == F) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                        if (x->γ.node == F) { if (x->op == IR_GOTO && x->ω.node == F) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; } }
                    ir_operand_push(F, pe);
                    ir_operand_push(F, p_tail);
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx;
                }
                else if (inner && !sno_in_arbno) {
                    IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                    int f_idx = g->n;
                    IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                    sno_ω_to(F, fail_p);
                    IR_LIT(F).ival = 1;
                    int before_p = g->n;
                    IR_t * pe = sno_pat_node(cx, inner, F, F);
                    IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
                    for (int q = before_p; q < g->n; q++) { IR_t * x = g->all[q];
                        if (!x) continue;
                        if (x->ω.node == F) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                        if (x->γ.node == F) { if (x->op == IR_GOTO && x->ω.node == F) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; } }
                    ir_operand_push(F, pe);
                    ir_operand_push(F, p_tail);
                    if (right_tail && !right_sealed) sno_resume_ω_to(g, right_tail_idx, right_tail, F);
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx; right_sealed = 0;
                }
                else if (i > 0) {
                    IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                    int f_idx = g->n;
                    IR_t * F = lc_build(g, IR_MATCH_FENCE0, cur_succ, NULL);
                    sno_ω_to(F, fail_p);
                    IR_LIT(F).ival = 0;
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx;
                }
                i--;
                continue;
            }
            int j = i; while (j > 0 && !sno_is_fence(elems[j - 1])) j--;
            int rn = i - j + 1;
            IR_t * fail_r = (j > first_f0) ? cx->pat_seal : fail;
            int before_r = g->n;
            IR_t * n_rt = NULL; IR_t * re = (rn == 1) ? sno_pat_node(cx, elems[j], cur_succ, fail_r) : sno_seq_nary(cx, elems + j, rn, cur_succ, fail_r, &n_rt);
            int _rb2 = before_r; while (_rb2 < g->n && g->all[_rb2] && g->all[_rb2]->op == IR_GOTO && g->all[_rb2]->n_operands == 0) _rb2++;
            IR_t * r_tail = n_rt ? n_rt : ((_rb2 < g->n) ? g->all[_rb2] : re);
            if (right_tail && !right_sealed && before_r < g->n) sno_resume_ω_to(g, right_tail_idx, right_tail, r_tail);
            cur_succ = re; right_tail = r_tail; right_tail_idx = before_r; right_sealed = 0;
            i = j - 1;
        }
        return cur_succ;
    }
    case TT_ALT: {
        const tree_t * alts[64]; int na = 0;
        const tree_t * rstack[64]; int nr = 0;
        const tree_t * cur = t;
        while (cur && cur->t == TT_ALT) {
            if (nr >= 64) sno_fatal("alternation with too many branches (SN4-PAT-3h cap 64)", NULL);
            rstack[nr++] = (cur->n > 1) ? cur->c[1] : NULL;
            cur = (cur->n > 0) ? cur->c[0] : NULL;
        }
        alts[na++] = cur;
        for (int i = nr - 1; i >= 0; i--) alts[na++] = rstack[i];
        if (na == 1) return sno_pat_node(cx, alts[0], succ, fail);
        IR_t * A = lc_build(g, IR_MATCH_ALTERNATE, succ, NULL);
        sno_ω_to(A, fail);
        (void)0;
        int fc_fp[16]; int fc_ab[16]; int fc_ae[16]; int fc_linear = (na <= 10);
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
            if (i < 16) { fc_fp[i] = fp_i; fc_ab[i] = before; fc_ae[i] = g->n; }
            ir_operand_push(A, ei);
            ir_operand_push(A, ri);
        }
        if (fc_linear) { extern void fc_alt_register(const IR_t *, int, const int *, const int *, const int *); extern void fc_arm_member_register(const IR_t *); fc_alt_register(A, (int)na, fc_fp, fc_ab, fc_ae); for (int _j = 0; _j < (int)na; _j++) for (int _k = fc_ab[_j]; _k < fc_ae[_j] && _k < g->n; _k++) if (g->all[_k]) fc_arm_member_register(g->all[_k]); }
        IR_LIT(A).ival = (long)na;
        return A;
    }
    case TT_FNC: {
        const char * name = t->v.sval; int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        static const struct { const char * n; tree_e k; } pm[] = { {"ANY",TT_ANY},{"NOTANY",TT_NOTANY},{"SPAN",TT_SPAN},{"BREAK",TT_BREAK},{"BREAKX",TT_BREAKX},{"LEN",TT_LEN},{"POS",TT_POS},{"RPOS",TT_RPOS},{"TAB",TT_TAB},{"RTAB",TT_RTAB},{"ARB",TT_ARB},{"ARBNO",TT_ARBNO},{"REM",TT_REM},{"FAIL",TT_FAIL},{"SUCCEED",TT_SUCCEED},{"FENCE",TT_FENCE},{"ABORT",TT_ABORT},{"BAL",TT_BAL},{NULL,TT_VAR} };
        tree_e pk = TT_VAR;
        if (name) for (int i = 0; pm[i].n; i++) if (!strcmp(name, pm[i].n)) { pk = pm[i].k; break; }
        if (pk != TT_VAR) { extern tree_t * ast_stmt_new(tree_e kind); tree_t * syn = ast_stmt_new(pk); for (int k = argbase; k < t->n; k++) ast_push(syn, (tree_t *) t->c[k]); return sno_pat_node(cx, syn, succ, fail); }
        { static int _ec = -1; if (_ec < 0) { const char * e = getenv("SCRIP_PAT_EAGER_CALL"); _ec = (!e || *e != '0') ? 1 : 0; }
          if (_ec && cx->npre >= 0 && cx->npre < 64) {
            IR_t * mvd = lc_build(g, IR_MATCH_DEFER, succ, NULL); sno_ω_to(mvd, fail);
            cx->pre[cx->npre].arg = t; cx->pre[cx->npre].prim = mvd; cx->pre[cx->npre].str = 0; cx->pre[cx->npre].codes = 0; cx->pre[cx->npre].snapg = name ? name : "$fnc"; cx->npre++;
            return mvd; } }
        IR_t * mv = lc_build(g, IR_MATCH_VALUE, succ, NULL); sno_ω_to(mv, fail);
        IR_t * vr = NULL; IR_t * ec = sx_lower(cx, t, mv, fail, &vr);
        if (vr) ir_operand_push(mv, vr);
        return ec;
    }
    default:
        sno_fatal("pattern element not in the SN4-PAT subset (LEN, literal, ANY, NOTANY, SPAN, BREAK, BREAKX, TAB, RTAB, POS, RPOS, REM, ARB; SEQ+ALT landed SN4-PAT-3h)", NULL);
    }
    return succ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_contains_arbno(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_ARBNO) return 1;
    for (int i = 0; i < t->n; i++) if (sno_pat_contains_arbno(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_supported(const tree_t * t) {
    if (!t) return 0;
    const tree_e k = sno_pat_eff_kind(t);
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
    if (k == TT_ARBNO) return t->n > 0 && t->c[0] && sno_pat_supported(t->c[0]) && (sno_arbno_chain_on() || !sno_pat_contains_arbno(t->c[0]));
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
    case TT_ABORT: case TT_SUCCEED:
    case TT_ALT: case TT_FENCE: case TT_ARBNO:
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX:
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS:
    case TT_ARB: case TT_REM: case TT_BAL:
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN:
        return 1;
    case TT_SEQ: case TT_CAT: { const tree_t * a = (t->n > 0) ? t->c[0] : NULL; const tree_t * b = (t->n > 1) ? t->c[1] : NULL;
        if ((a && a->t == TT_DEFER) || (b && b->t == TT_DEFER)) return 1; return sno_is_pattern_rhs(a) || sno_is_pattern_rhs(b); }
    case TT_VAR: { static int depth = 0; if (depth >= 32 || !t->v.sval) return 0; const tree_t * p = sno_seal_pat(t->v.sval); if (!p) return 0; depth++; int r = sno_is_pattern_rhs(p); depth--; return r; }
    case TT_KEYWORD: { static int kdepth = 0; if (!sno_const_static_on() || kdepth >= 32 || !t->v.sval) return 0; char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval); const tree_t * p = sno_const_pat(cb); if (!p) return 0; kdepth++; int r = sno_is_pattern_rhs(p); kdepth--; return r; }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_right_sealed(const tree_t * t) {
    if (!t) return 0;
    if (sno_is_fence(t)) return 1;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n > 1) return sno_pat_right_sealed(t->c[1]);
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0) return sno_pat_right_sealed(t->c[0]);
    if (t->t == TT_VAR && t->v.sval) { static int depth = 0; if (depth >= 32) return 0; const tree_t * p = sno_seal_pat(t->v.sval); if (!p) return 0; depth++; int r = sno_pat_right_sealed(p); depth--; return r; }
    if (g_sno_seal_enabled && t->t == TT_DEFER && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) return sno_pat_right_sealed(t->c[0]);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_patsalt_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATSALT"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_patname_salt_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATNAME_SALT"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_pat_collect(const tree_t * pat) {
    for (int i = 0; i < g_sno_npat; i++) if ((!sno_patsalt_on() || g_sno_pats[i].salt == g_sno_expr_salt) && sno_expr_eq(g_sno_pats[i].pat, pat)) return g_sno_pats[i].name;
    if (g_sno_npat >= SNO_PAT_MAX) sno_fatal("too many stored patterns in one program", NULL);
    char buf[32]; if (sno_patname_salt_on() && g_sno_expr_salt) snprintf(buf, sizeof buf, "PAT$%dF%d", g_sno_npat, g_sno_expr_salt); else snprintf(buf, sizeof buf, "PAT$%d", g_sno_npat);
    g_sno_pats[g_sno_npat].name = lp_strdup(buf);
    g_sno_pats[g_sno_npat].pat = pat;
    g_sno_pats[g_sno_npat].salt = g_sno_expr_salt;
    return g_sno_pats[g_sno_npat++].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land) {
    IR_graph_t * g = cx->g;
    cx->pat_fail = fJ; cx->pat_seal = fJ; cx->npre = 0;
    const tree_t * svt = (subj->n > 0) ? subj->c[0] : NULL;
    const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
    IR_t * land = fJ;
    if (out_land) { land = lc_build(g, IR_GOTO, fJ, NULL); *out_land = land; }
    IR_t * head = lc_build(g, IR_MATCH_BEGIN, NULL, land);
    { IR_t * sealJ = lc_build(g, IR_GOTO, head, NULL); memcpy(sealJ->γ.sz, "φ", 3); sealJ->γ.sz[3] = 0; cx->pat_seal = sealJ; }
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
    IR_t * release = lc_build(g, IR_MATCH_END, sJ, sno_cap_name_strict() ? cx->pat_seal : NULL);
    if (has_repl) IR_LIT(release).dval = 1.0;
    ir_operand_push(release, head);
    int before_pat = g->n;
    sno_cap_defer_reset();
    IR_t * pat_entry; { int _mcsv = g_sno_pat_match_ctx; g_sno_pat_match_ctx = 1; pat_entry = sno_pat_node(cx, ptt, release, head); g_sno_pat_match_ctx = _mcsv; }
    lc_γ_to(head, pat_entry);
    {
        int fp_stmt = 0; int fc_lin = (sno_in_arbno == 0) && fc_walk_range(g, before_pat, g->n, 1, &fp_stmt);
        if (fc_lin) { extern void fc_head_register(const IR_t *, int); fc_head_register(head, fp_stmt); }
        if (!fc_lin && rt_zc_frame_live() == ZC_FRAME_RSP) {
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
                        int n_wrap = 0; const IR_t * wsv[4]; const IR_t * wcd[4];
                        { const IR_t * inner_want = R;
                          for (;;) {
                              int found = -1;
                              for (int k = before_pat; k < i_arb; k++) { IR_t * x = g->all[k]; if (x && (x->op == IR_MATCH_ASSIGN_COND || x->op == IR_MATCH_ASSIGN_IMM) && x->n_operands > 1 && x->operands[0] == inner_want) { found = k; break; } }
                              if (found < 0) break;
                              IR_t * sv = (found + 1 < g->n) ? g->all[found + 1] : NULL;
                              if (!sv || sv->op != IR_MATCH_ASSIGN_SAVE || g->all[found]->operands[1] != sv || n_wrap >= 4) { cap_left = 1; break; }
                              wcd[n_wrap] = g->all[found]; wsv[n_wrap] = sv; n_wrap++;
                              inner_want = sv;
                          }
                          for (int k = before_pat; k < i_arb && !cap_left; k++) {
                              IR_t * x = g->all[k];
                              if (!x || (x->op != IR_MATCH_ASSIGN_SAVE && x->op != IR_MATCH_ASSIGN_COND && x->op != IR_MATCH_ASSIGN_IMM)) continue;
                              int used = 0; for (int w = 0; w < n_wrap; w++) if (x == wsv[w] || x == wcd[w]) { used = 1; break; }
                              if (used) continue;
                              { extern int fc_save_active(const IR_t *); extern int fc_cond_fp(const IR_t *);
                                if (x->op == IR_MATCH_ASSIGN_SAVE ? !fc_save_active(x) : fc_cond_fp(x) < 0) cap_left = 1; }
                          }
                        }
                        int cap_bad = 0;
                        char promo[64]; for (int d = 0; d < scd_n; d++) promo[d] = 0;
                        { extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_extent(const IR_t *, int *, int *); extern int fc_cond_fp(const IR_t *);
                          for (int d = 0; d < scd_n && !cap_bad; d++) {
                              if (scd[d].save_idx < i_b0 || scd[d].nd_idx >= g->n) { cap_bad = 1; break; }
                              int in_arm = 0;
                              for (int k = i_b0; k < g->n; k++) { IR_t * x = g->all[k]; if (!x || x->op != IR_MATCH_ALTERNATE || fc_alt_fpmax(x) < 0) continue; int _b = 0, _e = 0; if (fc_alt_extent(x, &_b, &_e) && scd[d].nd_idx >= _b && scd[d].nd_idx < _e) { in_arm = 1; break; } }
                              int nested = 0;
                              for (int e = 0; e < scd_n; e++) { if (e == d) continue; if (scd[e].nd_idx > scd[d].save_idx && scd[e].nd_idx < scd[d].i_end) { nested = 1; break; } }
                              if (in_arm || nested) cap_bad = 1; else promo[d] = 1;
                          }
                          if (!cap_bad) for (int k = i_b0; k < g->n; k++) {
                              IR_t * x = g->all[k];
                              if (!x || (x->op != IR_MATCH_ASSIGN_COND && x->op != IR_MATCH_ASSIGN_IMM)) continue;
                              if (fc_cond_fp(x) >= 0) continue;
                              int def_ok = 0; for (int d = 0; d < scd_n; d++) if (scd[d].nd == x && promo[d]) { def_ok = 1; break; }
                              if (!def_ok) { cap_bad = 1; break; }
                          }
                        }
                        tl_why = cap_left ? "cap-left" : cap_bad ? "cap-bad" : "walk";
                        int dfr_regs = 0;
                        if (!cap_left && !cap_bad) { int _hd = 0; for (int k = before_pat; k < g->n && !_hd; k++) { IR_t * x = g->all[k]; if (x && x->op == IR_MATCH_DEFER) _hd = 1; }
                                                     if (_hd) { int _ap = 0; for (int d = 0; d < scd_n && !_ap; d++) if (promo[d]) _ap = 1; dfr_regs = (n_wrap > 0 || _ap); if (dfr_regs) tl_why = "defer-caps"; } }
                        if (!cap_left && !cap_bad && !dfr_regs && fc_tail_walk(g, before_pat, i_arb) && fc_tail_walk(g, i_b0, i_b1 + 1) && fc_tail_walk(g, i_b1 + 1, g->n)) {
                            { extern void fc_save_register(const IR_t *); extern void fc_cond_register(const IR_t *, int);
                              for (int d = 0; d < scd_n; d++) if (promo[d]) { fc_save_register(scd[d].save); fc_cond_register(scd[d].nd, scd[d].fp_inner); }
                              for (int w = 0; w < n_wrap; w++) fc_save_register(wsv[w]); }
                            extern void fc_tail_candidate(const IR_t *, const IR_t *, int, int, int, int, int);
                            fc_tail_candidate(head, R, before_pat, i_arb, i_b0, i_b1, g->n);
                            { extern void fc_tail_wrap(const IR_t *, const IR_t *, const IR_t *);
                              for (int w = 0; w < n_wrap; w++) fc_tail_wrap(R, wsv[w], wcd[w]); }
                            tail_ok = 1;
                        }
                    }
                }
            }
            if (!tail_ok && getenv("SCRIP_TAIL_DIAG")) fprintf(stderr, "[TAIL-DIAG] refuse: %s\n", tl_why);
        }
    }
    IR_t * after = head;
    static int _preord = -1; if (_preord < 0) { const char * e = getenv("SCRIP_PRE_ORDER"); _preord = (e && *e == '0') ? 0 : 1; }
    for (int pi = _preord ? cx->npre - 1 : 0; _preord ? (pi >= 0) : (pi < cx->npre); pi += _preord ? -1 : 1) {
        if (cx->pre[pi].snapg) {
            static int g_snapctr = 0; char nb[32]; snprintf(nb, sizeof nb, "PATV$%d", g_snapctr++);
            char * gname = lp_strdup(nb); sno_reg_var(gname);
            IR_LIT(cx->pre[pi].prim).sval = gname;
            cx->pre[pi].prim->pat_static = 1;
            sno_fz_mark_defer(g, cx->pre[pi].prim, gname);
            IR_t * asnV = lc_build(g, IR_ASSIGN, after, fJ); IR_LIT(asnV).sval = gname;
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
    return subj_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_build_call_stub(const char * entry_label, const char * fname) {
    IR_graph_t * g = IR_alloc(64);
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t * sh4 = fname ? lc_build(g, IR_DEFINE, exitnd, failnd) : (IR_t *)0;
    if (sh4) { IR_LIT(sh4).ival = 4;
        IR_t * s41 = lc_build(g, IR_LIT_STRING, NULL, NULL); IR_LIT(s41).sval = (char *) fname;
        IR_t * s42 = lc_build(g, IR_LIT_STRING, NULL, NULL); IR_LIT(s42).sval = (char *) entry_label;
        ir_operand_push(sh4, s41); ir_operand_push(sh4, s42); }
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, sh4 ? sh4 : exitnd, failnd);
    IR_LIT(gd).sval = lp_strdup(entry_label);
    gd->seal = 1;
    g->entry = gd;
    return g;
}
static const tree_t * g_sno_prescan_top       = NULL;
static int            g_sno_expr_define_seen  = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name) {
    IR_graph_t * g = IR_alloc(nst * 16 + 256);
    scx_t cx; cx.g = g; cx.loop_exit = NULL; cx.loop_next = NULL; cx.result_name = result_name; cx.pat_fail = NULL; cx.pat_seal = NULL; cx.npre = 0;
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t ** anchor = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    IR_t ** fail_tgt = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    IR_t ** match_land = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    IR_t ** asgn_land = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    bb_label_registry_reset();
    for (int i = 0; i < nst; i++) {
        anchor[i] = lc_build(g, IR_GOTO, NULL, NULL);
        { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(anchor[i]).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }
        const char * lbl = sfind_str(st[i], ":lbl");
        if (lbl && lbl[0]) bb_label_registry_add(lp_strdup(lbl), anchor[i]);
    }
    bb_label_registry_add(lp_strdup("END"), exitnd);
    if (!result_name) {
        IR_t * rf = lc_build(g, IR_DEFINE, exitnd, failnd); IR_LIT(rf).ival = 1;
        IR_t * ff = lc_build(g, IR_DEFINE, exitnd, failnd); IR_LIT(ff).ival = 2;
        if (!bb_label_landing("RETURN"))  bb_label_registry_add(lp_strdup("RETURN"),  rf);
        if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), ff);
        if (!bb_label_landing("NRETURN")) { IR_t * nrl = lc_build(g, IR_LIT_STRING, NULL, ff); IR_LIT(nrl).sval = (char *) ""; IR_t * nnd = lc_build(g, IR_CALL, rf, ff); IR_LIT(nnd).sval = (char *) "SNO$NRET"; lc_γ_to(nrl, nnd); ir_operand_push(nnd, nrl); bb_label_registry_add(lp_strdup("NRETURN"), nrl); }
    } else {
        if (!bb_label_landing("RETURN"))  bb_label_registry_add(lp_strdup("RETURN"),  exitnd);
        if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), failnd);
        if (!bb_label_landing("NRETURN")) { IR_t * nrl = lc_build(g, IR_LIT_STRING, NULL, failnd); IR_LIT(nrl).sval = (char *) ""; IR_t * nnd = lc_build(g, IR_CALL, exitnd, failnd); IR_LIT(nnd).sval = (char *) "SNO$NRET"; lc_γ_to(nrl, nnd); ir_operand_push(nnd, nrl); bb_label_registry_add(lp_strdup("NRETURN"), nrl); }
    }
    g->entry = (nst > 0) ? anchor[entry_idx] : exitnd;
    int _pro_open = 0, _pro_close = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i];
        if (i == entry_idx) _pro_open = 1;
        if (_pro_close) _pro_open = 0;
        { extern void zls_group_mark_anchor(const IR_graph_t *, const char *, const IR_t *); const char * mlbl = sfind_str(s, ":lbl"); if (mlbl && mlbl[0]) zls_group_mark_anchor(g, lp_strdup(mlbl), anchor[i]); }
        IR_t * next = (i + 1 < nst) ? anchor[i + 1] : exitnd;
        if (sfind(s, ":end")) { lc_γ_to(anchor[i], exitnd); continue; }
        const char * goU = sgoto(s, TT_GOTO_U);
        const char * goS = sgoto(s, TT_GOTO_S);
        const char * goF = sgoto(s, TT_GOTO_F);
        const tree_t * exU = goU ? NULL : sgoto_expr(s, TT_GOTO_U);
        const tree_t * exS = goS ? NULL : sgoto_expr(s, TT_GOTO_S);
        const tree_t * exF = goF ? NULL : sgoto_expr(s, TT_GOTO_F);
        const tree_t * dcU = sgoto_direct(s, TT_GOTO_U);
        const tree_t * dcS = sgoto_direct(s, TT_GOTO_S);
        const tree_t * dcF = sgoto_direct(s, TT_GOTO_F);
        IR_t * sT = sno_goto_branch(g, &cx, goS, dcS, exS, exitnd); if (!sT) sT = sno_goto_branch(g, &cx, goU, dcU, exU, exitnd); if (!sT) sT = next;
        IR_t * fT = sno_goto_branch(g, &cx, goF, dcF, exF, exitnd); if (!fT) fT = sno_goto_branch(g, &cx, goU, dcU, exU, exitnd); if (!fT) fT = next;
        if (fT == next && !goF && !exF && !goU && !exU && sfind(s, ":nofail")) { IR_t *nf = lc_build(g, IR_CALL, exitnd, exitnd); IR_LIT(nf).sval = (char *)"SNO$NOFAIL"; fT = nf; }
        IR_t * stb = zw5_on() ? lc_build(g, IR_STATEMENT_END, sT, fT) : (IR_t *) NULL;
        if (stb) { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(stb).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }
        IR_t * sJ = lc_build(g, IR_GOTO, stb ? stb : sT, NULL);
        fail_tgt[i] = fT;
        IR_t * fJ = lc_build(g, IR_GOTO, fT, NULL);
        IR_t * fA = lc_build(g, IR_GOTO, fJ, NULL); asgn_land[i] = fA;
        if (is_def && is_def[i]) {
            static int _ab = -1; if (_ab < 0) { const char * _e = getenv("SCRIP_AB"); _ab = (_e && *_e == '1') ? 1 : 0; }
            if (_ab && !g_sno_expr_define_seen) {
                int _argbase = 0; const tree_t * dsub = sno_stmt_define(s, &_argbase);
                const tree_t * pnode = (dsub && dsub->n > _argbase) ? dsub->c[_argbase] : NULL;
                if (pnode && sno_qlit_fold(pnode)) {
                    sno_def_t d; sno_parse_define(sno_qlit_fold(pnode), NULL, &d);
                    int nsave = 1 + d.nnames;
                    IR_t * ab = lc_build(g, IR_DEFINE, exitnd, failnd);
                    IR_LIT(ab).sval = lp_strdup(d.fname);
                    ab->seal = d.nformals;
                    { IR_t * nm = lc_build(g, IR_LIT_STRING, ab, failnd); IR_LIT(nm).sval = lp_strdup(d.fname); ir_operand_push(ab, nm); }
                    for (int _k = 0; _k < d.nnames; _k++) { IR_t * nm = lc_build(g, IR_LIT_STRING, ab, failnd); IR_LIT(nm).sval = lp_strdup(d.names[_k]); ir_operand_push(ab, nm); }
                    if (g->ab_n < (int)(sizeof g->ab_nodes / sizeof *g->ab_nodes)) g->ab_nodes[g->ab_n++] = ab;
                    else fprintf(stderr, "WARN AB-1: ab_nodes[] full (>32 DEFINEs in one graph); activation block for '%s' will be missing from .s\n", d.fname);
                    { IR_t * bind = lc_build(g, IR_DEFINE, sJ, fA); IR_LIT(bind).sval = lp_strdup(d.fname);  lc_γ_to(anchor[i], bind); continue; }
                }
            }
            { int _argbase = 0; const tree_t * dsub = sno_stmt_define(s, &_argbase); const tree_t * pnode = (dsub && dsub->n > _argbase) ? dsub->c[_argbase] : NULL;
              if (pnode && sno_qlit_fold(pnode)) { sno_def_t d; sno_parse_define(sno_qlit_fold(pnode), NULL, &d);
                IR_t * bind = lc_build(g, IR_DEFINE, sJ, fA); IR_LIT(bind).sval = lp_strdup(d.fname); lc_γ_to(anchor[i], bind); continue; } }
            lc_γ_to(anchor[i], sJ); continue;
        }
        if (_pro_open && (goU || goS || goF || exU || exS || exF)) _pro_close = 1;
        const tree_t * subj = lc_stmt_subj(s);
        const tree_t * pat  = sfind_expr(s, ":pat");
        int has_eq = sfind(s, ":eq") != NULL;
        if (pat) sno_fatal("statement has a separate :pat field (stored-pattern form) — SN4-PAT-2 handles TT_SCAN match subjects only", NULL);
        if (subj && subj->t == TT_SCAN) {
            const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
            if (!sno_pat_supported(ptt)) {
                if (ptt && (ptt->t == TT_FNC || ptt->t == TT_INDIRECT || sno_is_pattern_rhs(ptt))) {
                    extern tree_t *ast_stmt_new(tree_e kind);
                    static int g_pattmp_n = 0;
                    char nmb[24]; snprintf(nmb, sizeof nmb, "PATTMP$%d", g_pattmp_n++);
                    char * tmpn = lp_strdup(nmb); sno_reg_var(tmpn);
                    IR_t * asn = lc_build(g, IR_ASSIGN, NULL, fA); IR_LIT(asn).sval = tmpn;
                    IR_t * vr = NULL; IR_t * ec = sx_lower(&cx, ptt, asn, fA, &vr);
                    ir_operand_push(asn, vr);
                    tree_t * dv = ast_stmt_new(TT_VAR); dv->v.sval = tmpn;
                    tree_t * dd = ast_stmt_new(TT_DEFER); ast_push(dd, dv);
                    tree_t * sc2 = ast_stmt_new(TT_SCAN); ast_push(sc2, (tree_t *) subj->c[0]); ast_push(sc2, dd);
                    IR_t * e2 = sno_lower_match(&cx, sc2, has_eq ? sfind_expr(s, ":repl") : NULL, has_eq, sJ, fJ, &match_land[i]);
                    lc_γ_to(asn, e2);
                    lc_γ_to(anchor[i], ec);
                    continue;
                }
                sno_fatal("pattern shape outside the SN4-PAT subset (LEN, literal, ANY, NOTANY, SPAN, BREAK, BREAKX, TAB, RTAB, POS, RPOS, REM, ARB)", NULL);
            }
            IR_t * e = sno_lower_match(&cx, subj, has_eq ? sfind_expr(s, ":repl") : NULL, has_eq, sJ, fJ, &match_land[i]);
            lc_γ_to(anchor[i], e);
            continue;
        }
        if (!subj) { lc_γ_to(anchor[i], sJ); continue; }
        if (!has_eq) {
            IR_t * r = NULL; IR_t * e = sx_lower(&cx, subj, sJ, fA, &r);
            lc_γ_to(anchor[i], e);
            continue;
        }
        tree_t * repl = sfind_expr(s, ":repl");
        if (subj->t == TT_VAR && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) {
            sno_reg_var(subj->v.sval);
            if (_pro_open && !result_name) sno_prologue_add(subj->v.sval);
            const char * bn = NULL;
            for (int fzi = 0; fzi < g_sno_nfz; fzi++) if (g_sno_fz[fzi].pat == repl) { bn = g_sno_fz[fzi].procname; break; }
            if (!bn) bn = sno_pat_collect(repl);
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fA); IR_LIT(asn).sval = subj->v.sval;
            IR_t * mk = lc_build(g, IR_CALL, asn, fA); IR_LIT(mk).sval = (char *) "SNO$MKPAT";
            IR_t * nl = lc_build(g, IR_LIT_STRING, mk, fA); IR_LIT(nl).sval = (char *) bn;
            ir_operand_push(mk, nl);
            ir_operand_push(asn, mk);
            IR_t * pae = nl;
            {
                IR_graph_t * tg = IR_alloc(256);
                scx_t tx; tx.g = tg; tx.loop_exit = NULL; tx.loop_next = NULL; tx.result_name = NULL; tx.pat_fail = NULL; tx.pat_seal = NULL; tx.npre = 0;
                IR_t * tok = lc_build(tg, IR_SUCCEED, NULL, NULL);
                IR_t * tno = lc_build(tg, IR_FAIL, NULL, NULL);
                tx.pat_fail = tno; tx.pat_seal = tno;
                sno_pat_node(&tx, repl, tok, tno);
                IR_t * pahead = NULL; IR_t * palast = NULL; const char * pbao = getenv("SCRIP_PB_ARGORDER");
                for (int api = 0; api < tx.npre; api++) {
                    char abuf[48]; snprintf(abuf, sizeof abuf, tx.pre[api].snapg ? "%s$V%d" : "%s$A%d", bn, api);
                    IR_t * asnA = lc_build(g, IR_ASSIGN, pae, fA); IR_LIT(asnA).sval = lp_strdup(abuf);
                    IR_t * av = NULL;
                    IR_t * ae = sx_lower(&cx, tx.pre[api].arg, asnA, fA, &av);
                    ir_operand_push(asnA, av);
                    if (pbao && *pbao == '0') { pae = ae; continue; }
                    if (!pahead) pahead = ae; else lc_γ_to(palast, ae);
                    palast = asnA;
                }
                if (pahead) pae = pahead;
                if (tx.npre > 0) { IR_t * ncnt = lc_build(g, IR_LIT_STRING, mk, fA); char cb[16]; snprintf(cb, sizeof cb, "%d", tx.npre); IR_LIT(ncnt).sval = lp_strdup(cb); ir_operand_push(mk, ncnt); lc_γ_to(nl, ncnt); }
            }
            lc_γ_to(anchor[i], pae);
            continue;
        }
        if (subj->t == TT_VAR) {
            sno_reg_var(subj->v.sval);
            if (repl && repl->t == TT_SCAN && repl->n == 2 && repl->c[0] && repl->c[1]) {
                extern tree_t * ast_stmt_new(tree_e kind);
                tree_t * capt = ast_stmt_new(TT_CAPT_COND_ASGN);
                ast_push(capt, (tree_t *) repl->c[1]);
                tree_t * tv = ast_node_new(TT_VAR); tv->v.sval = subj->v.sval;
                ast_push(capt, tv);
                tree_t * sc = ast_stmt_new(TT_SCAN);
                ast_push(sc, (tree_t *) repl->c[0]);
                ast_push(sc, capt);
                IR_t * e = sno_lower_match(&cx, sc, NULL, 0, sJ, fA, NULL);
                lc_γ_to(anchor[i], e);
                continue;
            }
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fA); IR_LIT(asn).sval = subj->v.sval;
            IR_t * vr = NULL; IR_t * e = sx_lower(&cx, repl, asn, fA, &vr);
            ir_operand_push(asn, vr);
            lc_γ_to(anchor[i], e);
            continue;
        }
        if (subj->t == TT_INDIRECT && subj->n > 0) {
            IR_t * nv = NULL; IR_t * e1 = sx_nameval(&cx, subj->c[0], NULL, fA, &nv);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(nv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, nv); ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_IDX && subj->n >= 2) {
            IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(&cx, subj->c[0], (const tree_t * const *) &subj->c[1], subj->n - 1, fA, &vr);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(vr, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
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
                IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(&cx, subj->c[argbase], (const tree_t * const *) &subj->c[argbase + 1], fnargs - 1, fA, &vr);
                IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
                lc_γ_to(vr, e2);
                IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, vr); ir_operand_push(asn, vv);
                lc_γ_to(anchor[i], e1);
                continue;
            }
            { extern int rt_dat_field_of_any(const char *);
              if (fname && fnargs == 1 && rt_dat_field_of_any(fname)) {
                  IR_t * br = NULL; IR_t * e1 = sx_lower(&cx, subj->c[argbase], NULL, fA, &br);
                  IR_t * fv = lc_build(g, IR_FIELD_VAR, NULL, fA); IR_LIT(fv).sval = (char *) lp_strdup(fname);
                  lc_γ_to(br, fv);
                  ir_operand_push(fv, br);
                  IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
                  lc_γ_to(fv, e2);
                  IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
                  lc_γ_to(vv, asn);
                  ir_operand_push(asn, fv); ir_operand_push(asn, vv);
                  lc_γ_to(anchor[i], e1);
                  continue;
              } }
        }
        if (subj->t == TT_KEYWORD && subj->v.sval && sno_kw_static_slot(subj->v.sval) >= 0) {
            IR_t * kv = NULL; IR_t * ke = sx_lower(&cx, repl, NULL, fA, &kv);
            IR_t * kw = lc_build(g, IR_KEYWORD_ASSIGN_SNOBOL4, sJ, fA); IR_LIT(kw).sval = subj->v.sval;
            lc_γ_to(kv, kw);
            ir_operand_push(kw, kv);
            lc_γ_to(anchor[i], ke);
            continue;
        }
        if (subj->t == TT_KEYWORD && subj->v.sval) {
            IR_t * mk = lc_build(g, IR_CALL, sJ, fA); IR_LIT(mk).sval = (char *) "SNO$KWSET";
            IR_t * nl = lc_build(g, IR_LIT_STRING, NULL, fA); IR_LIT(nl).sval = subj->v.sval;
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(nl, e2);
            lc_γ_to(vv, mk);
            ir_operand_push(mk, nl); ir_operand_push(mk, vv);
            lc_γ_to(anchor[i], nl);
            continue;
        }
        if (subj->t == TT_FNC) {
            IR_t * wl = lc_build(g, IR_LIT_STRING, NULL, fA); IR_LIT(wl).sval = (char *) "";
            IR_t * mk = lc_build(g, IR_CALL, sJ, fA); IR_LIT(mk).sval = (char *) "SNO$WANTNM";
            lc_γ_to(wl, mk); ir_operand_push(mk, wl);
            IR_t * cv = NULL; IR_t * e1 = sx_lower(&cx, subj, NULL, fA, &cv);
            lc_γ_to(mk, e1);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fA, &vv);
            lc_γ_to(cv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fA);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, cv); ir_operand_push(asn, vv);
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
            { int _null_stmt = !lc_stmt_subj(st[i]) && !sfind_str(st[i],":lbl") && !sgoto(st[i],TT_GOTO_U) && !sgoto_expr(st[i],TT_GOTO_U) && !sgoto_direct(st[i],TT_GOTO_U) && !sgoto(st[i],TT_GOTO_S) && !sgoto_expr(st[i],TT_GOTO_S) && !sgoto_direct(st[i],TT_GOTO_S) && !sgoto(st[i],TT_GOTO_F) && !sgoto_expr(st[i],TT_GOTO_F) && !sgoto_direct(st[i],TT_GOTO_F) && !sfind(st[i],":eq"); const tree_t * _sa = _null_stmt ? NULL : sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(sbeg).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }
            lc_γ_to(anchor[i], sbeg);
            if (match_land[i]) lc_γ_tag_β(match_land[i]);
            if (asgn_land[i]) { lc_γ_to(asgn_land[i], sbeg); lc_γ_tag_β(asgn_land[i]); }
        }
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
    for (int i = 0; i < nst; i++) {
        const char * ssrc = sfind_str(st[i], ":src");
        if (!ssrc) continue;
        IR_t * t = anchor[i];
        int hops = 0;
        while (t && t->op == IR_GOTO && t->γ.node && hops++ < 64) t = t->γ.node;
        if (t) bb_src_note(t, ssrc);
    }
    free(anchor);
    free(fail_tgt); free(asgn_land);
    free(match_land);
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
            const char * entry_opt = NULL;
            if (t->n > argbase + 1 && t->c[argbase + 1]) {
                const tree_t * ea = t->c[argbase + 1];
                if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
                else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
            }
            sno_def_t d; sno_parse_define(t->c[argbase]->v.sval, entry_opt, &d);
            if (t != g_sno_prescan_top) g_sno_expr_define_seen = 1;
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
int sno_expr_mark(void) { return g_sno_nexpr; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_expr_thunks_build(int x0) {
    int sv = g_sno_in_patproc;
    g_sno_in_patproc = 1;
    for (int xi = x0; xi < g_sno_nexpr; xi++) {
        { static int _xd = -1; if (_xd < 0) _xd = getenv("SCRIP_EXPR_DBG") ? 1 : 0; if (_xd) fprintf(stderr, "[EXPRDBG] %s want_name=%d expr.t=%d expr.sval=%.32s\n", g_sno_exprs[xi].name, g_sno_exprs[xi].want_name, g_sno_exprs[xi].expr ? (int)g_sno_exprs[xi].expr->t : -1, (g_sno_exprs[xi].expr && g_sno_exprs[xi].expr->v.sval) ? g_sno_exprs[xi].expr->v.sval : "?"); }
        IR_graph_t * gx = IR_alloc(256);
        scx_t ex; ex.g = gx; ex.loop_exit = NULL; ex.loop_next = NULL; ex.result_name = g_sno_exprs[xi].name; ex.pat_fail = NULL; ex.pat_seal = NULL; ex.npre = 0;
        IR_t * ok = lc_build(gx, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gx, IR_FAIL, NULL, NULL);
        IR_t * sJ = lc_build(gx, IR_GOTO, ok, NULL);
        IR_t * fJ = lc_build(gx, IR_GOTO, no, NULL);
        sno_reg_var(g_sno_exprs[xi].name);
        IR_t * asn = lc_build(gx, IR_ASSIGN, sJ, fJ); IR_LIT(asn).sval = (char *) g_sno_exprs[xi].name;
        IR_t * vr = NULL; IR_t * e = sx_lower(&ex, g_sno_exprs[xi].expr, asn, fJ, &vr);
        ir_operand_push(asn, vr);
        if (g_sno_exprs[xi].want_name) { IR_t * wn_lit = lc_build(gx, IR_LIT_STRING, NULL, fJ); IR_LIT(wn_lit).sval = (char *) ""; IR_t * wn_call = lc_build(gx, IR_CALL, NULL, fJ); IR_LIT(wn_call).sval = (char *) "SNO$WANTNM"; lc_γ_to(wn_lit, wn_call); lc_γ_to(wn_call, e); ir_operand_push(wn_call, wn_lit); gx->entry = wn_lit; } else { gx->entry = e; }
        { IR_t * ad = lc_build(gx, IR_DEFINE, gx->entry, fJ); IR_LIT(ad).ival = 3; gx->entry = ad; }
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
    g_sno_in_patproc = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pat_count(void) { return g_sno_npat; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_pat_carrier_build(scx_t * px, const tree_t * pat, IR_t * ok, IR_t * no, IR_t ** out_brt, int * out_pfenced) {
    IR_t * pe; *out_brt = NULL;
    int pfenced = sno_pat_contains_fence(pat, 0);
    if (getenv("SCRIP_FENCE_IGNORE")) pfenced = 0;
    { const tree_t * fel[128]; int fne = 0; sno_seq_flatten_pat(pat, fel, &fne); int topf = 0; for (int i = 0; i < fne; i++) if (sno_is_fence(fel[i])) { topf = 1; break; }
      pe = (sno_defer_resume() && fne > 1 && (!pfenced || (sno_fence_rtail() && !topf))) ? sno_seq_nary(px, fel, fne, ok, no, out_brt) : sno_pat_node(px, pat, ok, no); }
    *out_pfenced = pfenced;
    return pe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_pat_publish_body_root(IR_graph_t * gp, int before_pat, const tree_t * pat, IR_t * brt, int pfenced, const char * dbgname) {
    IR_t * rn = NULL;
    if (sno_defer_resume() && !pfenced) {
        rn = brt;
        if (!rn) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { rn = x2; break; } }
    }
    else if (sno_defer_resume() && pfenced) { extern int zdp_seam_tier(const IR_t *); const char * _fre = getenv("SCRIP_FENCE_RESUME");
        if (!(_fre && *_fre == '0')) { IR_t * _c = (sno_fence_rtail() && brt) ? brt : NULL; if (!_c) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { _c = x2; break; } } { int _t2 = zdp_seam_tier(_c); if (_t2 == 1 || _t2 == 3) rn = _c; } }
    }
    int rs = sno_pat_right_sealed(pat) ? 1 : 0;
    gp->body_root = (gp->n > before_pat && !rs) ? ((sno_defer_resume() && pfenced && !rn) ? NULL : (rn ? rn : gp->all[before_pat])) : NULL;
    if (getenv("SCRIP_RESUME_WHY") && !gp->body_root)
        { extern int zdp_seam_tier(const IR_t *); const IR_t * _fb = NULL; for (int k3 = before_pat; k3 < gp->n; k3++) { IR_t * x3 = gp->all[k3]; if (x3 && !(x3->op == IR_GOTO && x3->n_operands == 0)) { _fb = x3; break; } }
            fprintf(stderr, "[RESUME-NIL] pat=%s empty=%d right_sealed=%d pfenced=%d rn=%d brt=%d fb=%s fbtier=%d chain=%s|%s|%s|%s\n", dbgname ? dbgname : "?", !(gp->n > before_pat), rs, pfenced, rn ? 1 : 0, brt ? 1 : 0, _fb ? bb_op_name(_fb->op) : "-", zdp_seam_tier(_fb), (before_pat + 0 < gp->n) ? bb_op_name(gp->all[before_pat + 0]->op) : "-", (before_pat + 1 < gp->n) ? bb_op_name(gp->all[before_pat + 1]->op) : "-", (before_pat + 2 < gp->n) ? bb_op_name(gp->all[before_pat + 2]->op) : "-", (before_pat + 3 < gp->n) ? bb_op_name(gp->all[before_pat + 3]->op) : "-"); }
    if (getenv("SCRIP_RESUME_WHY"))
        fprintf(stderr, "[RTGRAPH] pat=%s pfenced=%d brt=%d rn=%d body_root_op=%d\n", dbgname ? dbgname : "?", pfenced, brt ? 1 : 0, rn ? 1 : 0, gp->body_root ? (int) gp->body_root->op : -1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_pat_thunks_build(int p0) {
    int sv = g_sno_in_patproc;
    g_sno_in_patproc = 1;
    for (int pi2 = p0; pi2 < g_sno_npat; pi2++) {
        IR_graph_t * gp = IR_alloc(512);
        scx_t px; px.g = gp; px.loop_exit = NULL; px.loop_next = NULL; px.result_name = NULL; px.pat_fail = NULL; px.pat_seal = NULL; px.npre = 0;
        IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
        px.pat_fail = no; px.pat_seal = no;
        int before_pat = gp->n;
        IR_t * brt = NULL; int pfenced = 0;
        IR_t * pe = sno_pat_carrier_build(&px, g_sno_pats[pi2].pat, ok, no, &brt, &pfenced);
        {
            extern tree_t *ast_stmt_new(tree_e kind);
            IR_t * paft = pe;
            for (int api = 0; api < px.npre; api++) {
                if (px.pre[api].snapg) { char vbuf[48]; snprintf(vbuf, sizeof vbuf, "%s$V%d", g_sno_pats[pi2].name, api); char * vg = lp_strdup(vbuf); sno_reg_var(vg); IR_LIT(px.pre[api].prim).sval = vg; sno_fz_mark_defer(gp, px.pre[api].prim, vg); continue; }
                IR_t * co = lc_build(gp, px.pre[api].str ? IR_COERCE_STRING : IR_COERCE_INTEGER, paft, no);
                IR_LIT(co).ival = px.pre[api].codes;
                char abuf[48]; snprintf(abuf, sizeof abuf, "%s$A%d", g_sno_pats[pi2].name, api);
                tree_t * tv = ast_stmt_new(TT_VAR); tv->v.sval = lp_strdup(abuf);
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
        sno_pat_publish_body_root(gp, before_pat, g_sno_pats[pi2].pat, brt, pfenced, g_sno_pats[pi2].name);
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
    g_sno_in_patproc = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t * lower_sno_stage2(const tree_t * prog) {
    g_sno_expr_define_seen = 0; g_sno_prescan_top = NULL;
    g_sno_seal_enabled = 1;
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    g_sno_nexpr = 0;
    g_sno_npat = 0;
    g_sno_uses_stmtkw = 0;
    g_sno_uses_code = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i]) { sno_scan_stmtkw(prog->c[i]); sno_scan_code_use(prog->c[i]); }
    sno_register_program(&g_stage2, prog);
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    const tree_t ** st = (const tree_t **) calloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    sno_fz_build_table(st, nst);
    sno_def_t defs[SNO_DEF_MAX]; int ndefs = 0; g_sno_npredef = 0;
    const tree_t * def_body[SNO_DEF_MAX]; for (int _k = 0; _k < SNO_DEF_MAX; _k++) def_body[_k] = NULL;
    int * is_def = (int *) calloc((size_t) nst, sizeof(int));
    for (int i = 0; i < nst; i++) {
        g_sno_prescan_top = lc_stmt_subj(st[i]);
        sno_prescan_expr(g_sno_prescan_top, defs, &ndefs);
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
        sno_def_t d; sno_parse_define(sno_qlit_fold(dsub->c[argbase]), entry_opt, &d);
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
    if (g_sno_uses_code) {
        int main_bb_idx = g_stage2.proc_table[pi].bb_idx;
        for (int i = 0; i < nst; i++) {
            const char * lbl = sfind_str(st[i], ":lbl");
            if (!lbl || !lbl[0]) continue;
            IR_t * anchor = bb_label_landing(lbl);
            if (!anchor) continue;
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
            g_stage2.proc_table[lpi].proc_entry_node = anchor;
            g_stage2.proc_table[lpi].bb_idx = main_bb_idx;
        }
    }
    if (!g_sno_uses_code) {
        int main_bb_idx2 = g_stage2.proc_table[pi].bb_idx;
        for (int di = 0; di < ndefs; di++) {
            if (def_body[di]) continue;
            const char * el = defs[di].entry;
            if (!el || !el[0]) continue;
            IR_t * anchor = bb_label_landing(el);
            if (!anchor) continue;
            char lname[256]; snprintf(lname, sizeof lname, "LBL__%s", el);
            int dup = 0;
            for (int q = 0; q < g_stage2.proc_count; q++) if (g_stage2.proc_table[q].name && !strcmp(g_stage2.proc_table[q].name, lname)) { dup = 1; break; }
            if (dup) continue;
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
            g_stage2.proc_table[lpi].bb_idx = main_bb_idx2;
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
            if (eidx < 0) continue;
            gf = sno_build_call_stub(defs[di].entry, defs[di].fname);
        }
        int fpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name = defs[di].fname;
        g_stage2.proc_table[fpi].proc = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].nparams = defs[di].nnames;
        g_stage2.proc_table[fpi].nformals = defs[di].nformals;
        for (int k = 0; k < defs[di].nnames && k < STAGE2_FRAME_SLOT_MAX; k++) g_stage2.proc_table[fpi].lower_sc.e[k].name = defs[di].names[k];
        g_stage2.proc_table[fpi].lower_sc.n = defs[di].nnames < STAGE2_FRAME_SLOT_MAX ? defs[di].nnames : STAGE2_FRAME_SLOT_MAX;
        g_stage2.proc_table[fpi].is_generator = 0;
        g_stage2.proc_table[fpi].dyn_scope = 1;
        g_stage2.proc_table[fpi].result_name = defs[di].result_name;
        g_stage2.proc_table[fpi].bb_idx = bb_program_add(&g_stage2.bbp, gf);
    }
    sno_expr_thunks_build(0);
    sno_pat_thunks_build(0);
    free((void *) st); free(is_def);
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
IR_graph_t * sno_pat_tree_graph_rt(const tree_t * pat) {
    IR_graph_t * gp = IR_alloc(512);
    scx_t px; px.g = gp; px.loop_exit = NULL; px.loop_next = NULL; px.result_name = NULL; px.pat_fail = NULL; px.pat_seal = NULL; px.npre = 0;
    IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
    IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
    px.pat_fail = no; px.pat_seal = no;
    int before_pat = gp->n;
    int rtc = sno_rtseq_resume();
    IR_t * brt = NULL; int pfenced = 0;
    IR_t * pe = rtc ? sno_pat_carrier_build(&px, pat, ok, no, &brt, &pfenced) : sno_pat_node(&px, pat, ok, no);
    if (px.npre > 0) sno_fatal("runtime-operand primitive reached the RT recipe graph builder — recipes must bake literal args (B-RE contract)", NULL);
    gp->entry = pe;
    gp->resumable_callable = 1;
    if (rtc) sno_pat_publish_body_root(gp, before_pat, pat, brt, pfenced, "RT$");
    else gp->body_root = (gp->n > before_pat && !sno_pat_right_sealed(pat)) ? gp->all[before_pat] : NULL;
    return gp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_lower_fragment_at(const tree_t * prog, int entry_idx) {
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    { extern void zls_reset(void); zls_reset(); }
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    if (nst == 0 || entry_idx < 0 || entry_idx >= nst) return NULL;
    const tree_t ** st = (const tree_t **) calloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    sno_fragment_reject_define(st, nst);
    g_sno_nfz = 0; g_sno_fz_unsafe = 1; g_sno_nencl = 0;
    g_sno_nt4 = 0; g_sno_t4_unsafe = 1;
    int seal_sv = g_sno_seal_enabled; g_sno_seal_enabled = 0;
    int * is_def = (int *) calloc((size_t) nst, sizeof(int));
    IR_graph_t * g = sno_build_graph(st, nst, entry_idx, is_def, NULL);
    { extern void optimizer_run(IR_graph_t *); extern void ir_drive_slot_assign(IR_graph_t *); if (g) { optimizer_run(g); ir_drive_slot_assign(g); } }
    free((void *) st); free(is_def);
    g_sno_seal_enabled = seal_sv;
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
