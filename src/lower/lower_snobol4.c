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
extern void global_register(const char * name);
extern int stage2_proc_grow(stage2_t * s2);
typedef struct { const tree_t * arg; IR_t * prim; int str; long codes; } sprearg_t;
typedef struct { IR_graph_t * g; IR_t * loop_exit; IR_t * loop_next; const char * result_name; IR_t * pat_fail; IR_t * pat_seal; sprearg_t pre[64]; int npre; } scx_t;
#define SNO_DEF_MAX 128
#define SNO_DEF_NAMES_MAX 64
typedef struct { const char * fname; const char * entry; const char * result_name; const char * names[SNO_DEF_NAMES_MAX]; int nnames; int nformals; } sno_def_t;   /* NPSPLIT (s22w): nformals = the (…) segment count alone; nnames stays the FULL save set (formals then locals, in prototype order — formals-first is load-bearing for arg index -> gk mapping in the slim install). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SNO_EXPR_MAX 4096
static struct { const char * name; const tree_t * expr; int salt; } g_sno_exprs[SNO_EXPR_MAX];
static int g_sno_nexpr = 0;
static int g_sno_expr_salt = 0;
void sno_expr_salt_next(void) { g_sno_expr_salt++; }
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
static int zw5_on(void) { const char * e = getenv("SCRIP_ZW5"); return (e && *e == '0') ? 0 : 1; }   /* ZW-5 KILLSWITCH, DEFAULT ON (polarity flipped at O-1 lighting, s23q).  SCRIP_ZW5=0 reverts to last-operator staging (pre-mint regime, byte-identical to parent bed9244 with the gate off).  No cached static: process-uniform regime switch, not per-graph; caching a getenv inside a graph-taking function is the s21x process-scope-flag-driving-graph-scope-regime defect signature. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sfind_str(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0 && a->c[0]) ? a->c[0]->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * sfind_expr(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0) ? a->c[0] : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    /* SNOBOL4 relational predicate decomposition (Lon directive 2026-07-10): argX chain -> argY chain ->
     * COERCE(self,other) x2 -> CMP_TEST.  Numeric coercion is a JOINT decision (either operand real -> both
     * real; strings parse int-then-real; null = 0); each coerce validates only SELF and raises its own
     * oracle-pinned POSITIONAL error code, first position first (GT('x','y') -> 111 not 112).  Lexical family
     * rides the existing IR_COERCE_STRING with empty allowed (typecode only).  Success value = null string. */
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
static IR_t * sx_call_named(scx_t * cx, const char * name, const tree_t * t, int argbase, IR_t * γ, IR_t * ω, IR_t ** res) {
    /* CALL2BB slice 1 (Lon directive s21x: "DEFINE, when CONSTANT FOLDED, emits exactly TWO BBs: IR_SAVE_RESTORE and IR_CALL") — behind SCRIP_CALL2BB=1 the call site becomes the two-BB pair: role-0
     * IR_SAVE_RESTORE (carves its OWN slots, saves the fname/formals/locals save-set, opens the pcall residue, installs staged args into the NV globals) chained γ-wise INTO the slimmed IR_CALL (frame
     * dance + transfer + restore landings).  sr0 carries the SAME sval (shared strdup, read-only) and the SAME arg operand list so its drive arm marshals slots exactly like the call family; args chain
     * terminates at sr0 instead of call.  Gate OFF = this function byte-identical to its prior body (sr0 never built, tail == call). */
    IR_t * call = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(call).sval = (char *) lp_strdup(name);
    IR_t * sr0 = NULL; static int c2bb = -1; if (c2bb < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); c2bb = (e2 && *e2 == '1') ? 1 : 0; }
    if (c2bb) { sr0 = lc_build(cx->g, IR_SAVE_RESTORE, call, ω); IR_LIT(sr0).sval = IR_LIT(call).sval; }   /* UNION-TAG (IR.h: sval/ival alias): writing sval IS the role-0 mark — the slice-1 `ival = 0` here was a dead store the sval write clobbered; the dispatch normalizes pointer-vs-1..3 back to role 0..3 */
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
        /* pattern-VALUE construction (assignment time), both forms: n==0 = FENCE0 (the bare variable — SNO$PB0 builds the primitive pattern object); n>0 = FENCE1 (FENCE(P) — SNO$PFEN wraps P's value) */
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
        { int lex = 0; long c1 = 0, c2 = 0; int rk = sno_pred_relop(name, &lex, &c1, &c2);
          if (rk >= 0 && t->n - argbase >= 1 && t->n - argbase <= 2) return sx_pred_cmp(cx, t, argbase, lex, rk, c1, c2, γ, ω, res); }
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
        const tree_t * first = (t->n > 0) ? t->c[0] : NULL;
        return sx_lower(cx, first, γ, ω, res);
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
/*--- computed indirect goto :($(expr)) — the parser strips the `$` and hands the goto field the operand tree
 *--- directly (TT_CAT/TT_VAR/etc, never a plain static QLIT).  Returns that operand tree, or NULL for the
 *--- static / simple-$var forms sgoto already resolves. ---*/
static const tree_t * sgoto_expr(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind || a->n == 0 || !a->c[0]) continue;
        const tree_t * g0 = a->c[0];
        if (g0->t == TT_QLIT) return NULL;
        if (g0->t == TT_INDIRECT && g0->n > 0 && g0->c[0] && g0->c[0]->t == TT_VAR) return NULL;
        return g0;
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
/*--- computed indirect goto :($(expr)) (manual "Indirect Gotos", multi-way branch).  Evaluate the operand
 *--- expression into a hidden IGT$n global, then reuse the simple `$var` path: IR_GOTO_DEFERRED("$IGT$n")
 *--- -> rt_goto_transfer reads IGT$n's string value and transfers to that label.  Operand evaluates at
 *--- TRANSFER time (only the taken branch reaches this target), matching SPITBOL goto-field semantics. ---*/
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
    d->nformals = d->nnames;   /* NPSPLIT (s22w): the formals loop above just finished; everything the next loop appends is a local. */
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
static void sno_ω_to(IR_t * nd, IR_t * t) { if (t) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
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
            if (t) lc_γ_to_β(T, t); else lc_γ_to(T, t);
            return;
        }
    }
    if (nd && nd->op == IR_MATCH_ASSIGN_COND && nd->n_operands > 1 && nd->operands[1]) nd = nd->operands[1];
    sno_ω_to(nd, t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SN4-BAREKW (s34): the SEVEN argument-less pattern keywords are UNREACHABLE from the parser.
 * pat_prim_kind() (snobol4.y:37) is consulted at EXACTLY ONE grammar site — snobol4.y:195,
 * `T_FUNCTION T_LPAREN` — i.e. only when a name is FOLLOWED BY A PAREN.  ABORT/ARB/BAL/FAIL/
 * FENCE/REM/SUCCEED take no arguments, so they never reach it; they fall to snobol4.y:196
 * (`T_IDENT -> TT_VAR`) and arrive here as plain variables.  Three of them (ARB/REM/FENCE) were
 * historically bandaged by strcmp INSIDE the TT_VAR arm below; the other four were not, and so
 * lowered to IR_MATCH_DEFER(<name>) = a deferred read of an unset variable — which is why
 * ABORT/SUCCEED were SILENT WRONG ANSWERS and `case TT_ABORT:`/`TT_BAL:`/`TT_SUCCEED:`/`TT_FAIL:`
 * in sno_pat_node have never once executed.  This normalizer is the ONE place the bare names are
 * resolved; it subsumes the three strcmp bandages.  Manual Ch.18 p.203: these are protected
 * built-in pattern variables ("Unlike SNOBOL4, these variables cannot be altered"), so promoting a
 * TT_VAR of these names to its primitive kind cannot shadow a user variable.
 * The grammar is NOT touched: REPO-SCRIP.md forbids running bison against the committed .y/.tab.c. */
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
/* FENCE0 vs FENCE1 (Lon naming ruling, 2026-07-23 s133).  TWO distinct primitives share the TT_FENCE tree kind, split by arity:
 * FENCE0 = the protected pattern VARIABLE (bare `FENCE`, manual Ch.4/18: matches the null string moving forward; the scanner backing up through it FAILS THE WHOLE ATTEMPT — pat_seal, not the left
 *          neighbor, and not HEAD's anchor advance: FENCE-first "effectively anchors").  As a four-port box its entire body would be two unconditional jmps — α→γ, β→ω — so it stays NODE-FREE: the
 *          spine walk erases it into pure edge rewiring (right-of-fence ω edges aimed at the seal target, resume repoints skipped).  Its ζ sync point (crossing forward kills everything to its LEFT)
 *          is the recorded sync-3 residue: the restore watermark is the ENCLOSING bracket's (HEAD / ARBNO iteration), not its own α — plumbing deferred.
 * FENCE1 = the SPITBOL FUNCTION `FENCE(P)` (manual ln 9328: matches as P, but on backup the alternatives WITHIN P are invisible — one-shot commit; elements LEFT of the fence stay live).  Also pure
 *          rewiring for match CONTROL, but since s133 it mints a physical IR_MATCH_FENCE1 box, existing solely as the SYNC-POINT ζ RELEASE bracket: α records the watermark, the σ commit glue
 *          bulk-restores it (bb_match_fence1.cpp) — wiring cannot move rsp, so the release is the one thing that NEEDS instructions.
 * sno_is_fence() remains the umbrella (the spine splitter and the ARBNO tail-seal marker treat both forms identically: each seals its right). */
static int sno_is_fence(const tree_t * t) { return t && sno_pat_eff_kind(t) == TT_FENCE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_fence1(const tree_t * t) { return t && t->t == TT_FENCE && t->n > 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_fence0(const tree_t * t) { return sno_is_fence(t) && !sno_is_fence1(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_seq_flatten_pat(const tree_t * t, const tree_t ** elems, int * ne) {
    if (!t) return;
    if (t->t == TT_SEQ) { sno_seq_flatten_pat((t->n > 0) ? t->c[0] : NULL, elems, ne); if (t->n > 1 && t->c[1]) { if (*ne >= 128) sno_fatal("pattern sequence too long (SN4-PAT cap 128)", NULL); elems[(*ne)++] = t->c[1]; } return; }
    if (*ne >= 128) sno_fatal("pattern sequence too long (SN4-PAT cap 128)", NULL);
    elems[(*ne)++] = t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_cset_fold(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT) return a->v.sval ? a->v.sval : "";
    if (a->t == TT_KEYWORD && a->v.sval) {
        static const struct { const char * n; const char * v; } kc[] = { { "lcase", "abcdefghijklmnopqrstuvwxyz" }, { "ucase", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" } };
        char lk[16]; size_t li = 0; for (; a->v.sval[li] && li < sizeof lk - 1; li++) lk[li] = (a->v.sval[li] >= 'A' && a->v.sval[li] <= 'Z') ? (char)(a->v.sval[li] - 'A' + 'a') : a->v.sval[li]; lk[li] = 0;
        for (size_t k = 0; k < sizeof kc / sizeof *kc; k++) if (!strcmp(lk, kc[k].n)) return kc[k].v;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_pattern_rhs(const tree_t * t);
static int sno_pat_supported(const tree_t * t);
static int sno_pat_contains_arbno(const tree_t * t);
static int sno_arbno_chain_on(void) { return rt_zeta_port_mode() == ZC_PORT_FORTH; }   /* ZB-ITER-1a: nested ARBNO admitted iff the s52 rsp linked-frame-chain is active (ZC_PORT_FORTH); the zcol default (fixed-stride realloc array) cannot nest, so it stays refused */
static const char * sno_pat_collect(const tree_t * pat);
static struct { const char * var; const char * procname; const tree_t * pat; } g_sno_fz[SNO_PAT_MAX];
static int g_sno_nfz = 0;
static int g_sno_fz_unsafe = 0;
static int g_sno_in_patproc = 0;
#define SNO_FZW_MAX 256
static const char * g_sno_fzw_name[SNO_FZW_MAX];
static int g_sno_fzw_cnt[SNO_FZW_MAX];
static int g_sno_nfzw = 0;
static void sno_fz_write(const char * nm) {
    if (!nm || !nm[0]) return;
    int i = 0; for (; i < g_sno_nfzw; i++) if (!strcmp(g_sno_fzw_name[i], nm)) break;
    if (i == g_sno_nfzw) { if (g_sno_nfzw >= SNO_FZW_MAX) { g_sno_fz_unsafe = 1; return; } g_sno_fzw_name[i] = nm; g_sno_fzw_cnt[i] = 0; g_sno_nfzw++; }
    g_sno_fzw_cnt[i]++;
}
static int sno_fz_wrcount(const char * nm) { for (int i = 0; i < g_sno_nfzw; i++) if (!strcmp(g_sno_fzw_name[i], nm)) return g_sno_fzw_cnt[i]; return 0; }
/* s137 OVER-SEAL candidate table (Lon ruling): (name, tree) pairs from every VAR = <supported-pattern-RHS> assignment — WIDER than g_sno_fz (no invariance gate: sealing
 * needs the runtime DT_P to be compiled from exactly this tree, which sno_pat_collect at the assignment site guarantees for any supported RHS; inlining's invariance bar is
 * irrelevant here).  Eligibility is judged LAZILY at lookup: !g_sno_fz_unsafe (EVAL/CODE/CLEAR/untrackable writes void every static claim) && wrcount == 1 (single write
 * program-wide — the INPUT poison above rides the same counter). */
static int g_sno_seal_enabled = 0;   /* s137 MAIN-LOWERING GATE: seal eligibility is a WHOLE-PROGRAM static claim; the runtime EVAL/CODE fragment compiler re-enters this file with fragment-local fz
 * state (fresh statics in an AOT process, fragment-only wrcounts) which proves nothing program-wide — so lower_sno_stage2 (the whole-program entry) grants it once and no fragment
 * path ever does.  m3 fragment-bearing programs are exactly the EVAL/CODE-unsafe ones, so warm-process fragments were already blocked; this makes the m4 runtime process match by construction. */
static struct { const char * name; const tree_t * pat; } g_sno_seal[SNO_PAT_MAX]; static int g_sno_nseal = 0;
static void sno_seal_note(const char * nm, const tree_t * pat) { if (!nm || !pat || g_sno_nseal >= SNO_PAT_MAX) return; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return; g_sno_seal[g_sno_nseal].name = nm; g_sno_seal[g_sno_nseal].pat = pat; g_sno_nseal++; }
static const tree_t * sno_seal_pat(const char * nm) { if (!g_sno_seal_enabled || !nm || g_sno_fz_unsafe || sno_fz_wrcount(nm) != 1) return NULL; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return g_sno_seal[i].pat; return NULL; }
static int sno_pat_right_sealed(const tree_t * t);
static int sno_defer_sealed(const char * nm) { const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_right_sealed(p) : 0; }   /* s137: defer target resolves (eligibly) to a right-sealed tree → IR_t.seal */
static int sno_pat_dfree(const tree_t * t, int spine, int depth) {   /* ZD-5 STATIC-SHAPE (s23i): 1 iff no TT_DEFER is reachable, resolving SPINE-position VAR names through g_sno_seal (eligibility = sno_seal_pat's own gate: single write, fz-safe, main lowering).  spine=0 marks primitive-ARGUMENT subtrees, where a VAR is a value read (LEN(N)'s N), never a pattern name.  Depth cap breaks bare-name chase cycles (A=B;B=A) conservatively.  TT_FNC conservative 0: a build-time call can return a pattern carrying defers; widen only on census evidence.  Unlisted kinds conservative 0 for the same reason -- the classifier's job is a PROOF of non-re-entry, not a guess. */
    if (!t) return 1;
    if (depth > 48) return 0;
    switch (t->t) {
    case TT_DEFER: return 0;
    case TT_QLIT: case TT_ILIT: case TT_FLIT: case TT_CSET: case TT_NUL: return 1;
    case TT_REM: case TT_ARB: case TT_FAIL: case TT_SUCCEED: case TT_ABORT: case TT_BAL: return 1;
    case TT_VAR: { const char * nm = t->v.sval; if (!spine) return 1; if (nm && (!strcmp(nm, "REM") || !strcmp(nm, "ARB") || !strcmp(nm, "FENCE"))) return 1; const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_dfree(p, 1, depth + 1) : 0; }
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX: case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS: { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 0, depth + 1)) return 0; return 1; }
    case TT_SEQ: case TT_CAT: case TT_ALT: case TT_FENCE: case TT_ARBNO: { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 1, depth + 1)) return 0; return 1; }
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: case TT_CAPT_CURSOR: return 0;   /* ⛔ BRACKETED s23i (core.3397, 127_pat_json_keyvalue): a capture INSIDE the referenced blob emits rt_cap_push with the raw cap-slot spelling `lea rdi,[rsp+176]` -- claim-relative, so ENTRY-REGIME-DEPENDENT: the blob is compiled once but an armed statement enters it at a shifted depth, the slot read lands on stale stack residue, and survival depends on absolute stack placement (the env-length flip; s23h flake-ledger disease, same wild-rt_cap_push class as that finding's item 4).  The sound blob spelling is a wire/anchor-carried claim base (the CARRIED-OPEN r9 park-address item), NOT rsp arithmetic -- until that design rung lands, a target bearing ANY capture declines.  127/152 were the deterministic witnesses; 125/146's greens were placement luck on the same defect. */
    default: return 0;
    }
}
static int sno_name_static(const char * nm) { const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_dfree(p, 1, 0) : 0; }   /* the pat_static stamp: eligibly-resolved AND transitively defer-free */
static void sno_fz_scan(const tree_t * t) {
    if (!t) return;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) sno_fz_write(t->c[1]->v.sval);
    if (t->t == TT_CAPT_CURSOR && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) sno_fz_write(t->c[0]->v.sval);
    if (t->t == TT_SWAP || t->t == TT_REVSWAP) for (int i = 0; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR) sno_fz_write(t->c[i]->v.sval);
    if (t->t == TT_FNC && t->v.sval) { const char * fn = t->v.sval;
        if (!strcmp(fn, "EVAL") || !strcmp(fn, "eval") || !strcmp(fn, "CODE") || !strcmp(fn, "code") || !strcmp(fn, "CONVERT") || !strcmp(fn, "convert")) g_sno_fz_unsafe = 1;
        if (!strcmp(fn, "CLEAR") || !strcmp(fn, "clear")) g_sno_fz_unsafe = 1;
        /* s137 FZ REFINEMENT: INPUT(.V, ...) associates exactly ONE named variable — poison THAT name (a double write ⇒ wrcount ≥ 2 excludes it from fz inlining and seal
         * eligibility forever) instead of nuking the whole-program table.  The old blanket unsafe made ANY program with an INPUT association (json-match's raw-mode read)
         * lose every frozen pattern AND every seal.  Name not statically extractable (computed association) ⇒ the blanket unsafe stands, exactly as before. */
        if (!strcmp(fn, "INPUT") || !strcmp(fn, "input")) { const tree_t * a0 = (t->n > 0) ? t->c[0] : NULL; const char * an = NULL;
            if (a0 && a0->t == TT_NAME && a0->n > 0 && a0->c[0] && a0->c[0]->t == TT_VAR) an = a0->c[0]->v.sval;   /* .V — unary dot parses as TT_NAME(TT_VAR) */
            else if (a0 && a0->t == TT_VAR) an = a0->v.sval;
            else if (a0 && a0->t == TT_QLIT) an = a0->v.sval;
            if (an && an[0]) { sno_fz_write(an); sno_fz_write(an); } else g_sno_fz_unsafe = 1; } }
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
/* sno_prologue_* -- PS-3 (s152) ORDER-DOMINANCE license: a pattern name is PROLOGUE-BOUND when its (single, seal==2) assignment sits in the unconditional entry corridor -- the statement run from
 * entry_idx up to the FIRST statement bearing any goto part (named or computed).  Every execution enters at entry and falls through the corridor before anything else can run, so no match statement
 * can ever observe the name unassigned ⇒ the defer site's SLOW path (whose frontier shape differs from the blob contract) is structurally unreachable.  Labels do NOT break the license (a label lets
 * control RETURN to the corridor, not skip its first pass); an in-corridor END exits the program, leaving later statements unreachable from the corridor -- both sound.  Function-graph builds
 * (result_name != NULL) never record: a write inside a DEFINE body dominates nothing. */
static const char * g_sno_pro[128];
static int g_sno_npro = 0;
static void sno_prologue_add(const char * nm) { if (!nm || g_sno_npro >= 128) return; for (int i = 0; i < g_sno_npro; i++) if (!strcmp(g_sno_pro[i], nm)) return; g_sno_pro[g_sno_npro++] = nm; }
int sno_name_prologue_bound(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_sno_npro; i++) if (!strcmp(g_sno_pro[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fz_build_table(const tree_t ** st, int nst) {
    g_sno_nfz = 0; g_sno_fz_unsafe = 0; g_sno_nfzw = 0; g_sno_npro = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i]; if (!s) continue;
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
            if (repl && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) sno_seal_note(subj->v.sval, repl);   /* s137: every supported pattern assignment is a seal CANDIDATE (invariance not required — see the table comment) */
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
    cx->pre[cx->npre].codes = sno_prearg_codes(t->t); cx->npre++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_in_arbno = 0;
static int sno_tree_has_varext(const IR_t * n, int d) { if (!n || d > 12) return 0; if (n->op == IR_MATCH_ARBNO || n->op == IR_MATCH_DEFER) return 1; for (int i = 0; i < n->n_operands; i++) if (sno_tree_has_varext(n->operands[i], d + 1)) return 1; return 0; }   /* SEQ-CELL fence (s21x-l, second falsification): the FIRST fence (body-resident SEQs) held but 066/164/165 persisted -- the mechanism is the SUSPENDED cell, not residency: a top-level SEQ's cell stays pushed across gamma while its ARBNO/DEFER descendants run, shifting every depth-sensitive window slot below it (the s202 defer-window class).  A SEQ over any variable-extent descendant therefore declines wholesale, degrade never die. */   /* ZB-FC-3b: >0 while lowering an ARBNO body; balanced, so it always returns to 0 (EVAL/CODE mint fresh graphs in-process) */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* sno_cap_defer -- R12-EXIT-1 L1b (Lon ruling this session: "all ARBNO needs to know is what size of children it has" -- the capture joins the predetermined-size list).  A capture lowered INSIDE an
 * ARBNO body cannot register its FORTH cell at lowering time: the grant is only sound if the statement takes the ELEMENT path (LIFO fixed-size pushes, uniform depth), and candidacy is decided later;
 * on the anchored/heap-flavor path the old rsp-moves-per-iteration premise still holds and the flat rt_cap array stays correct.  So the walk runs UNCONDITIONALLY here (fp_inner is real), the
 * registration is DEFERRED, and the candidacy site PROMOTES iff the statement converts.  i_end = g->n at defer time = the inner allocation end (nesting detection).  The list is per-statement
 * (cleared at each statement's pattern lower entry); nested-in-arbno captures and captures inside granted-ALT arm extents stay unpromotable (their enclosing fp was baked without the cell) and force
 * the statement to decline to the anchored window (degrade never die). */
static struct { const IR_t * nd; const IR_t * save; int nd_idx; int save_idx; int i_end; int fp_inner; } scd[64];
static int scd_n = 0;
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp);
static void sno_cap_defer_reset(void) { scd_n = 0; }
static void sno_cap_fc(IR_graph_t * g, IR_t * nd, IR_t * save, int before_i) {
    int fp_inner = 0; int walk_ok = fc_walk_range(g, before_i, g->n, 0, &fp_inner);
    if (!walk_ok) return;                                          /* inner not fc-linear: flat rt_cap path verbatim, both paths */
    if (sno_in_arbno == 0) { extern void fc_save_register(const IR_t *); extern void fc_cond_register(const IR_t *, int); fc_save_register(save); fc_cond_register(nd, fp_inner); return; }
    if (scd_n >= 64) return;                                       /* silent defer-table overflow: capture stays ungranted, statement declines at the cap scan */
    scd[scd_n].nd = nd; scd[scd_n].save = save; scd[scd_n].nd_idx = before_i - 2; scd[scd_n].save_idx = before_i - 1; scd[scd_n].i_end = g->n; scd[scd_n].fp_inner = fp_inner; scd_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp) {
    /* ALT-LIFT (the s65 named follow-on): the ONE eligibility+footprint walk all four grant sites share.  A GRANTED ALTERNATE contributes 16 (its own cell) + fpmax (the S10d padded arm -- only one arm
     * is live at yield, padded to max by the sigma stubs) and its arm allocation extent is SKIPPED (the naive range sum counted every arm's leaves = the exact over-count the pre-lift decline existed
     * to avoid).  An UNGRANTED ALTERNATE still declines the whole range.  fc_geom(ALT)=16 for granted ALTs, so the ALT case MUST run before the fc_geom catch or the arm extent is never skipped. */
    extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_extent(const IR_t *, int *, int *);
    int lin = 1;
    for (int k = k0; k < k1; k++) {
        IR_t * x = g->all[k];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) {
            int _b = 0, _e = 0;
            if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > k + 1) k = _e - 1; continue; }   /* ALT-FLAT s202: zero-cell ALT + flat arms contribute 0 to the spine; extent still skipped */
            lin = 0; continue;
        }
        { long fck; if (fc_geom(x, &fck)) { if (fp) *fp += (int)fck; continue; } }
        switch (x->op) {
        case IR_MATCH_LIT: case IR_MATCH_LEN: case IR_MATCH_ANY: case IR_MATCH_NOTANY:
        case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_ATP:
        case IR_MATCH_ASSIGN_SAVE: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_IMM:
        case IR_GOTO: break;
        case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL: if (!lit_ok) lin = 0; break;   /* R12-ERAD s65: constant primitive args allocate inline -- statement+SEQ whitelists only */
        default: lin = 0;
        }
    }
    return lin;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_tail_walk(IR_graph_t * g, int k0, int k1) {
    /* R12-EXIT-1 CARRY-THE-TAIL admission walk (fc_walk_range's shape, ARBNO-statement flavor): every node in the range must be a granted/zero-cell leaf, SEQUENCE, capture pair, wiring, or inline
     * literal.  L1 ALT-IN-BODY LIFT (the s69 named follow-on, landed): a GRANTED ALTERNATE is admissible -- its arms are linear by fc_alt_register's own admission, its footprint enters the element
     * as 16+fpmax in the finalize pass (only one padded arm is live at yield, the S10d law), and its arm extent is SKIPPED here (arm leaves are alternatives, counted per-arm at finalize, not on the
     * spine).  The ALT arm MUST run before the fc_geom catch (fc_geom(granted ALT)=16 -- the exact 163-regression slip the statement grant hit at s66).  An UNGRANTED ALTERNATE still declines
     * wholesale; ARBNO/DEFER/unknowns decline as ever.  Runtime-arg primitives are excluded at the CALLER via cx->npre (their pre-chain operand slots are FLAT, unreachable at element depth without
     * the deleted scratch-load). */
    extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_extent(const IR_t *, int *, int *);
    for (int k = k0; k < k1 && k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x) continue;
        if (x->op == IR_MATCH_ALTERNATE) {
            int _b = 0, _e = 0;
            if (fc_alt_fpmax(x) >= 0 && fc_alt_extent(x, &_b, &_e)) { if (_e > k + 1) k = _e - 1; continue; }
            return 0;
        }
        if (x->op == IR_MATCH_DEFER) {   /* PS-3 s153 DEFER-AS-KNOWN-FOOTPRINT-LEAF: a WRITE-ONCE (seal==2) defer whose name is PROLOGUE-DOMINATED and which carries its PAT$ target literal is ADMISSIBLE
                                          * -- its per-activation retention (SUSP = align16(32+fb)+fp+16, the emit_patzeta ζ size) is a compile-time constant priced at FINALIZE (registry fed by then;
                                          * a non-uniform/unregistered target DECLINES the whole candidate there, chain-arm fallback).  The blob carves at its own α and its ω restores the entry
                                          * frontier ABSOLUTELY (lea rsp,[rbp+K]) so carve/release are exact; β = jmp [rsp+0] finds the γ-record by LIFO.  Same license trio as the s152 DT arm;
                                          * SCRIP_ARBNO_LATCH=1 arms (opt-in until monitor-proven -- default byte-identical, the walk keeps declining). */
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
static IR_t * sno_seq_nary(scx_t * cx, const tree_t ** elems, int ne, IR_t * succ, IR_t * fail) {
    /* SEQ-ERAD SE-5/SE-6 (2026-08-04): IR_MATCH_SEQUENCE is fully deleted.  Elements wire DIRECTLY:
     * σ (rightward success) -> next element's α; φ (leftward fail) -> previous element's β (β-tagged).
     * We use a temporary IR_GOTO sentinel node S to collect σ/φ-tagged edges during lowering, then
     * a GLOBAL fixup pass (over the entire graph by S-pointer identity) re-points every tagged edge
     * at its real neighbour.  Global scan is required: nested sno_seq_nary / ARBNO body calls can
     * allocate nodes beyond hi[i] that also carry σ/φ tags pointing at this S.  Returns ent[0]. */
    IR_graph_t * g = cx->g;
    IR_t * S = lc_build(g, IR_GOTO, succ, NULL);   /* neutral sentinel; unique pointer per call */
    sno_ω_to(S, fail);
    IR_t * ent[128]; IR_t * res[128]; int lo[128];
    for (int i = 0; i < ne && ne < 128; i++) {
        int before = g->n;
        IR_t * ei = sno_pat_node(cx, elems[i], S, S);
        /* SEQ-ERAD s9 ELEMENT-RESUME: g->all[before] is the first-allocated body node.  Under the deleted
         * IR_MATCH_SEQUENCE the construct was minted first (lc_build before recursion), so first-allocated
         * == the construct, and the construct's β was the resume surface a completing element popped into.
         * SE-6 deleted the construct; first-allocated may now be a dead sentinel from a nested sno_seq_nary
         * call (op==IR_GOTO, BOTH ports still on S at this point, zero operands -- that triple is unique to
         * a nested sentinel; a real body GOTO always has exactly one port on S).  Skip it to reach the
         * actual first result-bearing node of the element. */
        int _rb = before;
        while (_rb < g->n && g->all[_rb] && g->all[_rb]->op == IR_GOTO
               && g->all[_rb]->γ.node == S && g->all[_rb]->ω.node == S
               && g->all[_rb]->n_operands == 0) _rb++;
        IR_t * ri = (_rb < g->n) ? g->all[_rb] : ei;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == S) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S) { if (x->op == IR_GOTO && x->ω.node == S) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
        }
        ent[i] = ei; res[i] = ri; lo[i] = before;
    }
    /* GLOBAL fixup: scan ALL nodes by S-pointer identity to catch nested allocations */
    for (int i = 0; i < ne && ne < 128; i++) {
        IR_t * nxt = (i + 1 < ne) ? ent[i + 1] : succ;
        IR_t * prv = (i > 0) ? res[i - 1] : fail;   /* always a β-target */
        int lo_i = lo[i]; int hi_i = (i + 1 < ne) ? lo[i + 1] : g->n;
        for (int k = 0; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x || x == S) continue;
            /* σ/φ tags pointing at S from within this element's range (by allocation order) */
            if (k < lo_i || k >= hi_i) continue;
            if (x->ω.node == S && x->ω.sz[0] == (char)0xcf && (unsigned char)x->ω.sz[1] == 0x86) { x->ω.node = prv; memcpy(x->ω.sz, "β", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x86) { x->γ.node = prv; memcpy(x->γ.sz, "β", 3); x->γ.sz[3] = 0; }
            if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x83) { x->γ.node = nxt; x->γ.sz[0] = 0; }
        }
    }
    /* Second pass: catch any S-tagged edges from nested calls that landed OUTSIDE lo[i]..hi[i] */

    for (int k = 0; k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x || x == S) continue;
        if (x->ω.node == S && x->ω.sz[0] == (char)0xcf && (unsigned char)x->ω.sz[1] == 0x86) { x->ω.node = fail; memcpy(x->ω.sz, "β", 3); x->ω.sz[3] = 0; }
        if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x86) { x->γ.node = fail; memcpy(x->γ.sz, "β", 3); x->γ.sz[3] = 0; }
        if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x83) { x->γ.node = succ; x->γ.sz[0] = 0; }
    }
    S->γ.node = succ; S->γ.sz[0] = 0;
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
static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail) {
    IR_graph_t * g = cx->g;
    if (!t) return succ;
    switch (sno_pat_eff_kind(t)) {                                  /* SN4-BAREKW: bare ABORT/ARB/BAL/FAIL/FENCE/REM/SUCCEED arrive as TT_VAR */
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
    case TT_ABORT:   { IR_t * j = lc_build(g, IR_MATCH_ABORT, NULL, NULL); IR_t * k = cx->pat_seal ? cx->pat_seal : fail; sno_ω_to(j, k); lc_γ_to(j, k); return j; }   /* ABORT-NODE (s193): was a bare IR_GOTO — classifier-invisible, which pinned IR_MATCH_BEGIN in the deep-arrival list (emit.cpp).  The box kills through ω (bb_match_abort = α-label + jmp ω + β trampoline→ω), so a β arrival — the matcher "seeing the ABORT pattern when it is backing up", manual Ch.9's stated reason FENCE(…|ABORT) exists — kills too, MORE faithful than the goto's no-β-surface.  γ wired to the same kill defensively (TT_FAIL precedent).  Kill target unchanged: pat_seal ?: fail, so the anchor-advance bypass (171) is preserved. */
    case TT_SPAN: {
        IR_t * nd = lc_build(g, IR_MATCH_SPAN, succ, NULL);
        sno_ω_to(nd, fail);
        const char * cs = sno_cset_fold((t->n > 0) ? t->c[0] : NULL);
        if (cs) IR_LIT(nd).sval = (char *) cs;
        else sno_pre_req(cx, t, nd);
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
        if (t->c[0]->t == TT_ILIT || t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_POS: case TT_RPOS: {
        IR_t * nd = lc_build(g, (t->t == TT_RPOS) ? IR_MATCH_RPOS : IR_MATCH_POS, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n <= 0 || !t->c[0]) sno_fatal("POS/RPOS requires a position argument", NULL);
        if (t->c[0]->t == TT_ILIT || t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_FENCE:
        /* WHOLE-PATTERN fence (not a spine element — TT_SEQ's splitter owns those): FENCE1 lowers P transparently — nothing exists to its right to seal against, and the match-bracket RELEASE is the
         * sync point that fires the instant P commits, so no IR_MATCH_FENCE1 box is needed; FENCE0 alone matches null = succ.  (Reachability from a TT_ALT arm is a recorded question for the goal file:
         * an arm-nested FENCE1 arrives here transparent, i.e. unsealed — crosscheck-clean at the s131 watermark.) */
        return (t->n > 0 && t->c[0]) ? sno_pat_node(cx, t->c[0], succ, fail) : succ;
    case TT_DEFER: {
        const tree_t * in = (t->n > 0) ? t->c[0] : NULL;
        if (in && in->t == TT_VAR && in->v.sval) { IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = (char *) in->v.sval; sno_fz_mark_defer(g, nd, in->v.sval); nd->seal = sno_defer_sealed(in->v.sval) ? 1 : (sno_seal_pat(in->v.sval) ? 2 : 0);   /* s142: 1 = full right-seal (s137 whack); 2 = WRITE-ONCE only (name eligibly resolves in g_sno_seal: single write, fz-safe) — enables the defer-site entry-cell, NOT the whack.  OP-SPLIT s21x-f: this is the `*X` arm, the only one the manual lets recurse (p.122) — IR_MATCH_DEFER is star-ONLY by construction now; the s199 dstar registration is deleted, the opcode IS the provenance. */ nd->pat_static = sno_name_static(in->v.sval);   /* ZD-5 s23i: a `*X` whose X is transitively defer-free cannot recurse -- the star buys late binding only, and the statement quartet may arm around it (117's *cmd class) */ sno_ω_to(nd, fail); return nd; }
        { const char * bn = sno_expr_collect(in); char pb[40]; snprintf(pb, sizeof pb, "*%s", bn);
          IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = lp_strdup(pb); sno_ω_to(nd, fail); return nd; }
    }
    case TT_VAR: {                                                 /* SN4-BAREKW: the REM/ARB/FENCE strcmp bandages that lived here are now in sno_pat_eff_kind() */
        const char * nm = t->v.sval;
        { IR_t * nd = lc_build(g, IR_MATCH_PATREF, succ, NULL); IR_LIT(nd).sval = (char *) nm; sno_fz_mark_defer(g, nd, nm); nd->seal = sno_defer_sealed(nm) ? 1 : (sno_seal_pat(nm) ? 2 : 0);   /* s142 write-once class; OP-SPLIT s21x-f: the EAGER twin — a bare stored-pattern NAME, built eagerly, cannot self-reference (manual p.122), lowers to IR_MATCH_PATREF; every consumer treats the pair identically this slice */ nd->pat_static = sno_name_static(nm);   /* ZD-5 s23i: static-shape patref -- the named next rung's arming population */ sno_ω_to(nd, fail); return nd; }
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
    case TT_BAL: {                                                 /* SN4-BAL (s34): manual Ch.9 p.124 — shortest non-null paren-balanced string, longer on retry ⇒ a GENERATOR ⇒ it owns runtime state ⇒ it earns a node (s31 rule) */
        IR_t * nd = lc_build(g, IR_MATCH_BAL, succ, NULL);
        sno_ω_to(nd, fail);
        return nd;
    }
    case TT_ARBNO: {
        /* SN4-NARY-ARBNO (Lon directive 2026-07-12, seed-2 idiom; the G/K/F triple is DELETED per the s31
         * node-iff-owns-runtime-state rule).  ARBNO(P) ≡ ε | P·ARBNO(P), shortest-first: R.α anchors entry δ
         * and yields ε; R.β EXTENDS — pushes a per-iteration COLLECTION frame {prev_view, saved_δ} + P's slot
         * window, repoints the ζ view, enters P.α.  Success-glue (σ → nr_s): null-progress guard resumes THIS
         * extension's body β (oracle pin 162), else counts, restores the view, yields one more.  Fail-glue
         * (φ → nr_f): pops, resumes the PREVIOUS iteration's body β; chain empty → restore entry δ → R.ω
         * (seed alt_ω verbatim).  operands = (P.entry, P.resume, last-body-node): flat_drive walks pairs by
         * n_operands/2 so [2] rides free as the zls geometry bracket ([1] = first-allocated is the other end).
         * Same σ/φ inside-edge retag as NARY-ALT; R first-allocated ⇒ R.β IS resume, R.ω IS leftward exhaust —
         * the ARBNO-EXHAUST CHASE died with the triple.  Body admission = any supported pattern except nested
         * ARBNO (zcol elements are realloc-movable; the rsp linked-chain+count flavor lifts this — Lon ruling
         * 2026-07-12, lands at ZB-ITER under ZLS_ARBNO_STACK).  In-body fence seal = R tagged φ: a cut pops to
         * the previous iteration and ARBNO-level backtrack continues, per ARBNO ≡ (ε | P·ARBNO(P)). */
        if (!(t->n > 0) || !t->c[0]) sno_fatal("ARBNO requires a pattern argument", NULL);
        if (sno_pat_contains_arbno(t->c[0]) && !sno_arbno_chain_on()) sno_fatal("nested ARBNO awaits the rsp iteration-frame chain (ZC_PORT_FORTH — GOAL-SNOBOL4-BB ZB-ITER-1a)", NULL);
        IR_t * R = lc_build(g, IR_MATCH_ARBNO, succ, NULL);
        sno_ω_to(R, fail);
        int before = g->n;
        IR_t * prev_seal = cx->pat_seal; cx->pat_seal = R;
        sno_in_arbno++;   /* ZB-FC-3b FENCE: a SEQ lowered INSIDE an ARBNO body must NOT take the FORTH edge re-point -- ARBNO is still HEAP-flavor (Tier D) and moves rsp per iteration (zls2 blocks) between the body's gamma and a later beta, which breaks the static-depth premise the re-point rests on.  Lifts when ARBNO's iteration-frame chain rides rsp (ZB-ITER). */
        IR_t * ei = sno_pat_node(cx, t->c[0], R, R);
        sno_in_arbno--;
        cx->pat_seal = prev_seal;
        if (before >= g->n) sno_fatal("ARBNO body lowered to zero nodes (bare FENCE / null pattern body)", NULL);
        /* SEQ-ERAD s9: g->all[before] is the first-allocated body node, which was the SEQ container
         * under IR_MATCH_SEQUENCE.  SE-6 deleted the container; first-allocated is now a dead sno_seq_nary
         * sentinel (IR_GOTO, γ→succ after fixup, n_operands==0) for multi-element bodies.  Skip it.
         * The sentinel's γ points at succ (=R) after sno_seq_nary's final S->γ.node=succ assignment,
         * distinguishing it from a genuine body GOTO whose γ would point at another body node.
         * NOTE: measured (2026-08-05 s9) — this skip correctly changes operands[1] from the dead
         * sentinel to the first real body node (same as green's resume convention), but does NOT cure
         * the nested-ARBNO crash (H24/H25/X02/X06/X11 still SIGSEGV).  The crash is a separate defect. */
        int _rb = before;
        while (_rb < g->n && g->all[_rb] && g->all[_rb]->op == IR_GOTO
               && g->all[_rb]->γ.node == R /* succ=R for the outer ARBNO body sentinel */
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
        { const tree_t * belems[128]; int bne = 0; sno_seq_flatten_pat(t->c[0], belems, &bne);   /* FENCE-rooted resume surface: a committed iteration's alternatives are invisible on backup (manual ln 4716; the old v1-fenced seal was STRUCTURAL — no resume edge existed; the one-node form makes it an EDGE property: operands[3]=R marks resume≡fail-glue, flat_drive resolves PAIR(1)→na_f, popping cascades to exhaust exactly as SPITBOL cuts left) */
          if (bne > 0 && sno_is_fence(belems[bne - 1])) ir_operand_push(R, R); }
        IR_LIT(R).ival = 1;
        return R;
    }
    case TT_LEN: {
        IR_t * nd = lc_build(g, IR_MATCH_LEN, succ, NULL);
        sno_ω_to(nd, fail);
        long long n = 0;
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT) n = t->c[0]->v.ival;
        else if (t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0)) sno_pre_req(cx, t, nd);
        else sno_fatal("LEN with a deferred or missing count is outside the operand-edge subset", NULL);
        IR_LIT(nd).ival = n;
        return nd;
    }
    case TT_CAPT_COND_ASGN: {
        const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
        if (vn) sno_reg_var(vn);
        if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) { const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL; const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : sno_expr_collect(di); char pb[48]; snprintf(pb, sizeof pb, "*%s", bn); vn = lp_strdup(pb); }
        if (!vn || !(t->n > 0 && t->c[0])) sno_fatal("conditional capture target is not a simple variable (SN4-PAT-2 subset)", NULL);
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
        sno_ω_to(nd, itail); /* COND backtrack-in: resume or pop */
        ir_operand_push(nd, pe);                                   /* [0] inner entry */
        ir_operand_push(nd, save);                                 /* [1] SAVE → COND.op_off = save's slot */
        {   /* ZB-FC-3c (ARCH-ZETA S13 Tier C; plan of record = the s47 COND-CROSS-BOX-READ finding) + ALT-LIFT + L1b: SAVE
             * gets a 16-byte cell (delta at cell+0, rt_cap array retired on the granted path) and COND reads
             * it CROSS-BOX at [rsp + fp(inner)] -- static by S10c (cells pop at omega not gamma, so the whole
             * inner subtree is still suspended at COND.alpha).  fp(inner) via the shared fc_walk_range: a
             * granted ALT inner contributes 16+fpmax (its pad stubs ran at sigma before control reaches
             * COND.alpha, so the padded depth IS the suspended depth); ungranted ALT/ARBNO/ARB/DEFER inside
             * the inner still declines both registrations and the capture keeps today's flat array path
             * verbatim (degrade never die).  Inside an ARBNO body the registration DEFERS to the statement
             * candidacy site (sno_cap_fc): the cell is sound only on the ELEMENT path (Lon's static-size
             * ruling -- the capture is just another predetermined 16 in the sum there), while a declined/
             * anchored statement keeps the flat array (rsp still moves per iteration on that machinery). */
            sno_cap_fc(g, nd, save, before_i);
        }
        { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(nd, g->n); }   /* FLATDISP-LEAF-ORDER: inner allocation ends here; fc_leaf_walk registers COND at the depth AFTER this range */
        return save;                                               /* capture entry is the SAVE node */
    }
    case TT_CAPT_IMMED_ASGN: {
        const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
        if (vn) sno_reg_var(vn);
        if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) { const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL; const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : sno_expr_collect(di); char pb[48]; snprintf(pb, sizeof pb, "*%s", bn); vn = lp_strdup(pb); }
        if (!vn || !(t->n > 0 && t->c[0])) sno_fatal("immediate capture target is not a simple variable (SN4-PAT-2 subset)", NULL);
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
        sno_ω_to(nd, itail);
        ir_operand_push(nd, pe);
        ir_operand_push(nd, save);
        {   /* ZB-FC-3c + ALT-LIFT + L1b: $ is the IDENTICAL topology at op_phase 2 (the s47 finding's C5 -- one mechanism,
             * two phases); same shared walk, same two-directional fence, same registrations, same in-ARBNO deferral. */
            sno_cap_fc(g, nd, save, before_i);
        }
        { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(nd, g->n); }   /* FLATDISP-LEAF-ORDER: twin of the COND arm above */
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
        /* SN4-NARY-SEQ (Lon directive 2026-07-12): a fence-free element list becomes ONE IR_MATCH_SEQUENCE
         * node (sno_seq_nary above; seq_i in ζ replaces the pairwise sno_resume_ω_to seam-stitching, and the
         * per-element direct-to-fail ω was the "controlled optimization" of the transit guarantee — the glue
         * makes the transit structural).  Fences SPLIT the spine into subsequences (goal-file design): each
         * maximal fence-free run of ≥2 elements is its own S node with the run's shared fail target; a run
         * right of the fence fails (S.ω) to the SEAL target, never back across the fence — the old walk's
         * right_sealed non-repoint is preserved verbatim at the fence seams, which stay edge-threaded. */
        const tree_t * elems[128]; int ne = 0;
        sno_seq_flatten_pat(t, elems, &ne);
        int first_fence = ne;
        for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { first_fence = i; break; }
        if (first_fence == ne)
            return ne == 1 ? sno_pat_node(cx, elems[0], succ, fail) : sno_seq_nary(cx, elems, ne, succ, fail);
        IR_t * cur_succ = succ; IR_t * right_tail = NULL; int right_tail_idx = -1; int right_sealed = 0;
        for (int i = ne - 1; i >= 0; ) {
            if (sno_is_fence(elems[i])) {                                           /* FENCE0 or FENCE1: each seals everything to its right; the element to its left cannot resume into it */
                right_sealed = 1;
                const tree_t * inner = sno_is_fence1(elems[i]) ? elems[i]->c[0] : NULL;   /* inner != NULL ⇔ FENCE1; FENCE0 stays node-free (pure rewiring — its box body would be α→γ, β→ω) */
                if (inner) {                                                        /* FENCE1 = FENCE(P): lower P with the pre-seal fail so P retries normally on forward-fail; the seal blocks re-entry after success */
                    /* SYNC-POINT ζ RELEASE (Lon ruling s132, sync point 2 — FENCE(P) success exit).  The old wiring was PURE EDGE
                     * REWIRING: P succeeded straight into cur_succ and every ζ cell P's boxes retained (uniform-β) sat on the stack
                     * until the match bracket died, even though the seal makes them unreachable the instant P commits (its
                     * alternatives are invisible backing up — manual ln 4716).  Measured: json-match.sno's FENCE-per-token ws eats
                     * >32MB ≤64MB of ζ on a 632KB subject (SCRIP_STACK ladder, s132/this rung).  Now the fence is ONE
                     * IR_MATCH_FENCE1 box in the ALT/SEQ σ/φ-glue mold: F.α records the watermark (rsp under FORTH; the zls2 cursor
                     * under the heap ports) into its granted [rbp+off] quad — depth-immune, per-activation, so DEFER recursion
                     * through the same fence is safe — and jmps P's entry; P is lowered succ=F fail=F with the standard inside-edge
                     * retag, so P's commit lands F's na_s glue (bulk-restore to the watermark, jmp F.γ = the old cur_succ) and P's
                     * leftward exhaust lands na_f (same restore — the identity by LIFO — then F.ω = the old fail_p).  Resume-from-
                     * the-right stays STRUCTURALLY absent (right_sealed skips the repoint below, exactly as before); F.β falls into
                     * na_f as the ARBNO-seal "resume ≡ abandon" precedent demands should any future wiring reach it. */
                    IR_t * fail_p = (i > first_fence) ? cx->pat_seal : fail;
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
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx;
                }
                else if (i > 0) {                                                   /* s137 OVER-SEAL (Lon ruling): an INTERIOR FENCE0 gets the operand-free sync box (ival=0) — its α IS the forward
                                                                                     * commit (match null), and the box body is now non-empty: whack the activation's dynamic ζ to the rbp floor
                                                                                     * (bb_match_fence1.cpp ival=0 arm), then γ.  Wiring is the s133 erasure's, verbatim: ω → the pre-seal fail
                                                                                     * (backup ≡ attempt abort), right_sealed already set above so the left run gets no resume repoint and the box
                                                                                     * gets no inbound β.  FIRST-POSITION FENCE0 (i==0, the anchor idiom) keeps the node-free erasure: zero left
                                                                                     * context in this spine, nothing to whack, the s133 reasoning stands there. */
                    IR_t * fail_p = (i > first_fence) ? cx->pat_seal : fail;
                    int f_idx = g->n;
                    IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                    sno_ω_to(F, fail_p);
                    IR_LIT(F).ival = 0;
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx;
                }
                i--;
                continue;
            }
            int j = i; while (j > 0 && !sno_is_fence(elems[j - 1])) j--;             /* the maximal fence-free run [j..i]; a run never spans a fence so one fail target serves it */
            int rn = i - j + 1;
            IR_t * fail_r = (j > first_fence) ? cx->pat_seal : fail;                 /* right of the fence: cut to the SEAL target (== statement-fail at top level, == F/exhaust inside an ARBNO body), never HEAD */
            int before_r = g->n;
            IR_t * re = (rn == 1) ? sno_pat_node(cx, elems[j], cur_succ, fail_r) : sno_seq_nary(cx, elems + j, rn, cur_succ, fail_r);
            IR_t * r_tail = (before_r < g->n) ? g->all[before_r] : re;
            if (right_tail && !right_sealed && before_r < g->n) sno_resume_ω_to(g, right_tail_idx, right_tail, r_tail);
            cur_succ = re; right_tail = r_tail; right_tail_idx = before_r; right_sealed = 0;
            i = j - 1;
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
        /* SN4-NARY-ALT (Lon directive 2026-07-12, seed/test_sno_1.c is the spec): ONE IR_MATCH_ALTERNATE
         * node A with 2N operands = (entry_i, resume_i) pairs — the SAVE+join triple is DELETED.  Runtime
         * "which alternative" state = alt_i in A's own ζ quad (seed's ζ->alt_i), not graph structure.
         * A.α: save δ+dcap, alt_i=0, enter operand-entry 0.  A.β (right context resumes): dispatch on alt_i
         * to resume_i's β — the alternative's OWN inner resume (seed alt_β; fixes the old mark-next-alternative
         * residual that skipped a nested generator's remaining ways).  Shared success-glue (na_s): jmp A.γ.
         * Shared fail-glue (na_f): alt_i++, reload δ+dcap, dispatch to entry_{alt_i}.α or exhaust → A.ω.
         * INSIDE-EDGE TAGS: each alternative is lowered with succ=A, fail=A; afterwards every edge in its
         * allocation range that targets A is re-tagged — γ→A as "σ" (land na_s), ω→A as "φ" (land na_f), and
         * a FAIL-goto's γ→A also "φ" (both its edges mean fail).  Outside edges keep α/β tags and land A's
         * real α/β, so the sno_resume_ω_to generic arm serves the construct with ZERO exhaust-chasing —
         * A IS first-allocated, A.β IS resume, A.ω IS leftward exhaust. */
        IR_t * A = lc_build(g, IR_MATCH_ALTERNATE, succ, NULL);
        sno_ω_to(A, fail);
        (void)0; /* A is first-allocated: the construct tail TT_SEQ resume re-points land A.β */
        int fc_fp[16]; int fc_ab[16]; int fc_ae[16]; int fc_linear = (na <= 10);   /* ZB-FC-3a: N>10 exceeds the 3N+2<=32 pair budget */
        for (int i = 0; i < na; i++) {
            int before = g->n;
            IR_t * ei = sno_pat_node(cx, alts[i], A, A);
            IR_t * ri = (before < g->n) ? g->all[before] : ei;
            int fp_i = 0;
            for (int k = before; k < g->n; k++) {
                IR_t * x = g->all[k];
                if (!x) continue;
                if (x->ω.node == A) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                if (x->γ.node == A) { if (x->op == IR_GOTO && x->ω.node == A) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
            }
            /* ZB-FC-3a (ARCH-ZETA S13 Tier C) + ALT-LIFT: EXACT static arm footprint for the S10d pad-to-max law, via the shared walk -- a nested GRANTED ALT now contributes 16+FPMAX_inner with its arm
             * extent skipped (the pre-lift blanket decline is retired); an UNGRANTED nested ALT or anything unknown still declines the whole outer ALT (stays flat, degrade never die). */
            if (!fc_walk_range(g, before, g->n, 0, &fp_i)) fc_linear = 0;
            if (i < 16) { fc_fp[i] = fp_i; fc_ab[i] = before; fc_ae[i] = g->n; }
            ir_operand_push(A, ei);
            ir_operand_push(A, ri);
        }
        if (fc_linear) { extern void fc_alt_register(const IR_t *, int, const int *, const int *, const int *); extern void fc_arm_member_register(const IR_t *); fc_alt_register(A, (int)na, fc_fp, fc_ab, fc_ae); for (int _j = 0; _j < (int)na; _j++) for (int _k = fc_ab[_j]; _k < fc_ae[_j] && _k < g->n; _k++) if (g->all[_k]) fc_arm_member_register(g->all[_k]); }   /* ALT-FLAT s202: arm residents go flat -- MUST run AFTER the admission walk above (fp math computed pre-decline) */
        IR_LIT(A).ival = (long)na;   /* promoted → _.op_ival = N for the template's dispatch chains (walk_bb_node line ~697); the σ/φ inside-edge tags carry membership, so no extent is needed */
        return A;
    }
    case TT_FNC: {
        /* SN4-PAT-FNC (2026-07-21): a call in pattern position.  Snocone's frontend never runs pat_prim_kind (snobol4.y:37) so a pattern primitive such as LEN(1)/SPAN(cs) arrives here as a generic TT_FNC whose callee name is data; if the name is a primitive, synthesize the matching TT_* node from the call's argument children and recurse into the landed primitive case.  Otherwise the call is a value-returning expression (upr(x), IDENT(...)): SNOBOL4 eager semantics evaluate it once and use the result as a pattern, so materialize it into a fresh PATTMP$n temp via sx_lower and route to IR_MATCH_DEFER on that name — the exact statement-level idiom at the TT_SCAN driver, now reachable at any pattern nesting depth. */
        const char * name = t->v.sval; int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        static const struct { const char * n; tree_e k; } pm[] = { {"ANY",TT_ANY},{"NOTANY",TT_NOTANY},{"SPAN",TT_SPAN},{"BREAK",TT_BREAK},{"BREAKX",TT_BREAKX},{"LEN",TT_LEN},{"POS",TT_POS},{"RPOS",TT_RPOS},{"TAB",TT_TAB},{"RTAB",TT_RTAB},{"ARB",TT_ARB},{"ARBNO",TT_ARBNO},{"REM",TT_REM},{"FAIL",TT_FAIL},{"SUCCEED",TT_SUCCEED},{"FENCE",TT_FENCE},{"ABORT",TT_ABORT},{"BAL",TT_BAL},{NULL,TT_VAR} };
        tree_e pk = TT_VAR;
        if (name) for (int i = 0; pm[i].n; i++) if (!strcmp(name, pm[i].n)) { pk = pm[i].k; break; }
        if (pk != TT_VAR) { extern tree_t * ast_stmt_new(tree_e kind); tree_t * syn = ast_stmt_new(pk); for (int k = argbase; k < t->n; k++) ast_push(syn, (tree_t *) t->c[k]); return sno_pat_node(cx, syn, succ, fail); }
        /* SN4 kill-manufactured-names (2026-07-22): no PATTMP$P global, no IR_ASSIGN, no name.  The eager
         * value-returning call is lowered once (SNOBOL4 eager semantics) into its own value node, which becomes
         * operand[0] of an IR_MATCH_VALUE node; that node reads FR(op_a_slot) at match time — DT_P runs the
         * compiled pattern fn, a scalar is a literal match.  Killing the per-occurrence sno_reg_var(tmpn) is what
         * removes the GLOBAL_MAX/zls flood (PATTMP$P3128) that blocked beauty self-host; each match node's value
         * lives in its own per-node slot, so nested/sequential eager calls no longer clobber a shared name. */
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
    const tree_e k = sno_pat_eff_kind(t);                          /* SN4-BAREKW: bare keywords arrive as TT_VAR */
    if (k == TT_FENCE) return t->n == 0 || sno_pat_supported(t->c[0]);
    if (k == TT_QLIT) return 1;
    if (k == TT_ANY || k == TT_NOTANY) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_SPAN) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_BREAK || k == TT_BREAKX) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_TAB || k == TT_RTAB) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_POS || k == TT_RPOS) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_REM || k == TT_ARB) return 1;
    if (k == TT_ABORT || k == TT_FAIL) return 1;                   /* SN4-BAREKW s34: manual Ch.9 pp.124-125 — both are pure WIRING (own no runtime state) */
    if (k == TT_BAL) return 1;                                     /* SN4-BAL s34: LANDED — IR_MATCH_BAL + bb_match_bal */
    if (k == TT_SUCCEED) return 0;                                 /* SN4-BAREKW s34: NOT YET LOWERED — honest refusal, NOT a silent DEFER(unset). SUCCEED needs a β->γ. */
    if (k == TT_ARBNO) return t->n > 0 && t->c[0] && sno_pat_supported(t->c[0]) && (sno_arbno_chain_on() || !sno_pat_contains_arbno(t->c[0]));
    if (k == TT_VAR) return t->v.sval != NULL;
    if (k == TT_DEFER) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_LEN) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_CAPT_COND_ASGN) return t->n > 1 && t->c[1] && (sno_capt_name(t->c[1]) != NULL || t->c[1]->t == TT_DEFER) && sno_pat_supported(t->c[0]);
    if (k == TT_CAPT_IMMED_ASGN) return t->n > 1 && t->c[1] && (sno_capt_name(t->c[1]) != NULL || t->c[1]->t == TT_DEFER) && sno_pat_supported(t->c[0]);
    if (k == TT_CAPT_CURSOR) return t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval;
    if (k == TT_SEQ) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (k == TT_ALT) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (k == TT_FNC) return 1;                                     /* SN4-PAT-FNC (2026-07-21): a call in pattern position — primitive-name synthesis or value-materialize+DEFER, handled in sno_pat_node's TT_FNC case */
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_pattern_rhs(const tree_t * t) {
    if (!t) return 0;
    switch (sno_pat_eff_kind(t)) {                                 /* SN4-BAREKW: bare ABORT/ARB/BAL/FAIL/FENCE/REM/SUCCEED arrive as TT_VAR */
    case TT_ABORT: case TT_SUCCEED:
    case TT_ALT: case TT_FENCE: case TT_ARBNO:
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX:
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS:
    case TT_ARB: case TT_REM: case TT_BAL:
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN:
        return 1;
    case TT_SEQ: case TT_CAT: { const tree_t * a = (t->n > 0) ? t->c[0] : NULL; const tree_t * b = (t->n > 1) ? t->c[1] : NULL;
        if ((a && a->t == TT_DEFER) || (b && b->t == TT_DEFER)) return 1; return sno_is_pattern_rhs(a) || sno_is_pattern_rhs(b); }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_right_sealed(const tree_t * t) {
    /* NCB-2/SZ-1 β-resume seal (manual Ch.19): a stored blob whose RIGHTMOST element is a fence form must NOT
     * expose a resume surface — FENCE(P)'s alternatives are visible only moving forward, and bare FENCE fails
     * the match on backup; the pre-resume return-once-and-dead behavior WAS the seal (the v1-FENCE admission's
     * "no resume edge is ever stamped into the body from outside").  A mid-pattern fence needs no gate here:
     * the resume cascade reaches its in-blob seal wiring naturally.  Captures are transparent wrappers.
     * s137 OVER-SEAL (Lon ruling): the chase now crosses VAR/DEFER references — a rightmost `X` / `*X` whose name
     * eligibly resolves in g_sno_seal (single write, fz-safe program) recurses into the STORED tree, so sealing is
     * TRANSITIVE (json: ws sealed ⇒ jelement sealed ⇒ jmember sealed).  Depth-guarded against table cycles (X=*Y,
     * Y=*X).  Dual consumers: blob body_root=NULL at PAT$/RT emission (resume-surface removal now widens through
     * refs) and sno_defer_sealed → IR_t.seal (the caller-side fence-demarked whack). */
    if (!t) return 0;
    if (sno_is_fence(t)) return 1;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n > 1) return sno_pat_right_sealed(t->c[1]);
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0) return sno_pat_right_sealed(t->c[0]);
    if (t->t == TT_VAR && t->v.sval) { static int depth = 0; if (depth >= 32) return 0; const tree_t * p = sno_seal_pat(t->v.sval); if (!p) return 0; depth++; int r = sno_pat_right_sealed(p); depth--; return r; }
    if (g_sno_seal_enabled && t->t == TT_DEFER && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) return sno_pat_right_sealed(t->c[0]);   /* defer-of-VAR only: any other deferee is dynamic at match time and its static tree proves nothing */
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_pat_collect(const tree_t * pat) {
    for (int i = 0; i < g_sno_npat; i++) if (sno_expr_eq(g_sno_pats[i].pat, pat)) return g_sno_pats[i].name;
    if (g_sno_npat >= SNO_PAT_MAX) sno_fatal("too many stored patterns in one program", NULL);
    char buf[32]; snprintf(buf, sizeof buf, "PAT$%d", g_sno_npat);
    g_sno_pats[g_sno_npat].name = lp_strdup(buf);
    g_sno_pats[g_sno_npat].pat = pat;
    return g_sno_pats[g_sno_npat++].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land) {
    IR_graph_t * g = cx->g;
    cx->pat_fail = fJ; cx->pat_seal = fJ; cx->npre = 0;
    const tree_t * svt = (subj->n > 0) ? subj->c[0] : NULL;
    const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
    IR_t * land = fJ;
    if (out_land) { land = lc_build(g, IR_GOTO, fJ, NULL); *out_land = land; }   /* R1 STMT-BETA-LAND (06e design, landed this session): fB -- a DEDICATED exhaust-only GOTO carrying ONLY MATCH_BEGIN's ω (anchor exhausted = whole-statement failure).  Untagged it chains fB→fJ→fT, byte-equivalent to the direct wire (zd_chase and the drive chase both thread GOTOs transparently); the zw5 post-loop in sno_build_graph retags fB.γ to STATEMENT_BEGIN with the β port tag, making statement_begin_beta the named failure landing per the STATEMENT-PORT LAWS (Lon 2026-08-06).  fJ is UNTOUCHED: element failures (the scanner retry loop, SPITBOL manual Ch.18 step 6) and every other statement-failure producer keep their edges -- the 06e session measured that retagging the SHARED fJ re-tags every edge chasing through it (3 regressions, two statements fell out of the walk on 067). */
    IR_t * head = lc_build(g, IR_MATCH_BEGIN, NULL, land);
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
    IR_t * release = lc_build(g, IR_MATCH_END, sJ, NULL);
    if (has_repl) IR_LIT(release).dval = 1.0;
    ir_operand_push(release, head);
    int before_pat = g->n;
    sno_cap_defer_reset();   /* L1b: the deferred in-ARBNO capture list is per-statement — entries from a statement whose candidacy never consumed them (npre/REPLACE/multi-ARBNO early-outs) must not leak into the next statement's promotion scan */
    IR_t * pat_entry = sno_pat_node(cx, ptt, release, head);
    lc_γ_to(head, pat_entry);
    {   /* ZB-FC-3d (partition ruling, s49) + ALT-LIFT (the s65 named follow-on, landed): the statement grant.  The shared fc_walk_range over the PATTERN range only ([before_pat, g->n) -- head/release/
         * splice/repl-chain allocate before it, pre-chains and the subject chain after, so the range is pattern-pure by construction).  fp_stmt = the fc_geom range sum PLUS 16+fpmax per granted
         * ALTERNATE (arm extents skipped -- only one padded arm is live at yield, the exact over-count the old blanket decline existed to avoid).  UNGRANTED ALTs and unknowns still decline; either-
         * direction failure keeps the flat path byte-verbatim (degrade never die). */
        int fp_stmt = 0; int fc_lin = (sno_in_arbno == 0) && fc_walk_range(g, before_pat, g->n, 1, &fp_stmt);
        if (fc_lin) { extern void fc_head_register(const IR_t *, int); fc_head_register(head, fp_stmt); }
        /* REG-7 U4 sweep (s86): the s66 ANCHOR-WINDOW decline fallback is DELETED -- a !tail_ok statement now runs the same unconditional-rbp frame as everything else (U1/U2/s79 seeds; FR/FRQ rbp-based
         * post-U3; the ARBNO chain arm's zv() view decoupled s85).  Proof of deadness: the s85 flip probe -- crosscheck watermark-exact, .s byte-identical on 070/117/142/164/165 -- no live emission
         * consulted the window predicate.  Candidacy below is unchanged; the only decline consequence left is the TAIL_DIAG print. */
        if (!fc_lin && rt_zc_frame_live() == ZC_FRAME_RSP) {
            /* R12-EXIT-1 CARRY-THE-TAIL candidacy (tried BEFORE the anchored fallback): exactly ONE spine ARBNO, no REPLACE, no runtime-arg pre-chain (cx->npre), no capture allocated left of the
             * ARBNO (a COND/SAVE preceding it may wrap it -- conservative decline), left/body/right ranges walk clean, ARBNO body-bracket operands resolve.  A candidate registers with zeta_storage's
             * fct (geometry finalizes in the layout pass where zls offsets exist) and its spine SEQ converts to the static-wiring grant (an ungranted SEQ's runtime seq_i is a FLAT slot read at every
             * seam = dynamic depth once elements exist -- the exact class this rung deletes).  Anything short of the full test declines to the anchored window verbatim (degrade never die). */
            int tail_ok = 0;
            const char * tl_why = (cx->npre != 0) ? "npre" : has_repl ? "repl" : "gate";   /* SCRIP_TAIL_DIAG: first-failing-gate name, printed at the decline seam below (the OMEGA-DIAG precedent) */
            if (cx->npre == 0 && !has_repl) {
                int i_arb = -1, n_arb = 0;
                for (int k = before_pat; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->op == IR_MATCH_ARBNO) { n_arb++; i_arb = k; } }
                tl_why = (n_arb == 0) ? "no-arbno" : (n_arb > 1) ? "multi-arbno" : tl_why;
                if (n_arb == 1) {
                    IR_t * R = g->all[i_arb];
                    /* L2 (s71): fence-sealed bodies (operands[3]==R) ADMIT — the seal is an EDGE property (the template's PAIR(1) departures retarget its seal glue via op_tail_seal); element geometry,
                     * walks, and every other v1 gate are identical to the unsealed case, so the range discovery no longer branches on it. */
                    int i_b0 = -1, i_b1 = -1;
                    if (R->n_operands >= 3) for (int j = before_pat; j < g->n; j++) { if (g->all[j] == R->operands[1]) i_b0 = j; if (g->all[j] == R->operands[2]) i_b1 = j; }
                    tl_why = "body-range";
                    if (i_b0 > i_arb && i_b1 >= i_b0) {
                        int cap_left = 0;
                        /* WRAP-CAPTURE LIFT (R12-EXIT-1, Lon ruling this session: the wrap SAVE is one more predetermined 16 in the element sum).  A left-range capture is admissible iff it WRAPS the
                         * ARBNO: its COND/IMM's inner entry (operands[0]) is the ARBNO node R itself (innermost) or the previous wrap's SAVE (a nested wrap -- the capture entry IS the SAVE node), and
                         * its SAVE sits at the very next allocation index with the operand[1] back-edge intact.  Pairs collect INNERMOST-FIRST (zeta_storage flat-cell/slot indexing law).  LEFT-CONTEXT
                         * LIFT (R12-EXIT-1 residue, the 065/066 class): a left-range ASSIGN the chain does not consume is admissible iff it is ALREADY FLAT-GRANTED at lowering (sno_cap_fc with
                         * sno_in_arbno==0: fc_save_active SAVE / fc_cond_fp>=0 COND-IMM) -- its 16 is in fpl via fc_geom, its COND fires and beta-transits at FLAT depth only (elements are born after
                         * the fire and fully popped by exhaust before any beta, oracle-pinned pa2/pb2/pb3/pc2), so nothing else moves.  An UNGRANTED left capture (walk-failed inner) keeps the
                         * conservative decline (tl_why "cap-left"). Cap 4 wrap pairs (element-bloat bound; overflow declines). */
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
                        /* L1b PROMOTION PROTOCOL (Lon ruling this session: the capture is just another predetermined size).  A deferred body capture PROMOTES to its FORTH cell iff the statement takes
                         * the element path — the walk already proved its inner linear and computed fp_inner; on the element path the cell is exactly SPAN's 16, LIFO with the element.  UNPROMOTABLE
                         * residues (their enclosing footprints were baked at lowering time WITHOUT the 16, so granting now would desync depth): (a) a capture inside a granted-ALT arm extent (the fca
                         * arm fp is baked); (b) nested deferred captures (the outer's fp_inner is baked without the inner SAVE's cell).  Any capture in the element ranges that would stay UNGRANTED —
                         * unpromotable residue, walk-failed inner, or a right-range capture that never granted — forces the statement to DECLINE to the anchored window: the flat rt_cap slot is a
                         * PERSISTENT {buf,gen,sp} struct (pattern_match.c) and cannot live in transient element memory (the 163 finding — garbage nonzero buf in fresh element bytes reads/writes wild).
                         * Promotion is applied ONLY after every gate passes: a declined statement must keep every capture ungranted (the anchored ARBNO machinery still moves rsp per iteration —
                         * the sno_in_arbno gate's original and still-valid premise there). */
                        int cap_bad = 0;
                        char promo[64]; for (int d = 0; d < scd_n; d++) promo[d] = 0;
                        { extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_extent(const IR_t *, int *, int *); extern int fc_cond_fp(const IR_t *);
                          for (int d = 0; d < scd_n && !cap_bad; d++) {
                              if (scd[d].save_idx < i_b0 || scd[d].nd_idx >= g->n) { cap_bad = 1; break; }   /* a deferred entry outside the element ranges has no sound home on either path — decline */
                              int in_arm = 0;
                              for (int k = i_b0; k < g->n; k++) { IR_t * x = g->all[k]; if (!x || x->op != IR_MATCH_ALTERNATE || fc_alt_fpmax(x) < 0) continue; int _b = 0, _e = 0; if (fc_alt_extent(x, &_b, &_e) && scd[d].nd_idx >= _b && scd[d].nd_idx < _e) { in_arm = 1; break; } }
                              int nested = 0;
                              for (int e = 0; e < scd_n; e++) { if (e == d) continue; if (scd[e].nd_idx > scd[d].save_idx && scd[e].nd_idx < scd[d].i_end) { nested = 1; break; } }
                              if (in_arm || nested) cap_bad = 1; else promo[d] = 1;
                          }
                          if (!cap_bad) for (int k = i_b0; k < g->n; k++) {   /* any element-range COND/IMM neither granted nor promotable-deferred = the rt_cap path = the 163 mine */
                              IR_t * x = g->all[k];
                              if (!x || (x->op != IR_MATCH_ASSIGN_COND && x->op != IR_MATCH_ASSIGN_IMM)) continue;
                              if (fc_cond_fp(x) >= 0) continue;
                              int def_ok = 0; for (int d = 0; d < scd_n; d++) if (scd[d].nd == x && promo[d]) { def_ok = 1; break; }
                              if (!def_ok) { cap_bad = 1; break; }
                          }
                        }
                        tl_why = cap_left ? "cap-left" : cap_bad ? "cap-bad" : "walk";
                        int dfr_regs = 0;   /* PS-3 s153: a DEFER-bearing candidate can DECLINE AT FINALIZE (target unregistered/non-uniform -- knowable only once the emit_patzeta registry is fed), and
                                             * a finalize-decline must strand NOTHING: the promo/wrap fc_save/fc_cond registrations below grant 16B cells at LOWER assuming the tail path, and a
                                             * declined-to-chain statement with granted captures is the 163-mine class verbatim.  So defer+captures combos decline HERE (degrade never die; slice 2's
                                             * record-peek can widen).  The pat_entry SEQ registration is the one lower-side effect a finalize-decline CAN reach -- zeta_storage's decline path
                                             * fc_seq_unregisters it coherently (emit consults fc_seq_active only after the layout pass). */
                        if (!cap_left && !cap_bad) { int _hd = 0; for (int k = before_pat; k < g->n && !_hd; k++) { IR_t * x = g->all[k]; if (x && x->op == IR_MATCH_DEFER) _hd = 1; }
                                                     if (_hd) { int _ap = 0; for (int d = 0; d < scd_n && !_ap; d++) if (promo[d]) _ap = 1; dfr_regs = (n_wrap > 0 || _ap); if (dfr_regs) tl_why = "defer-caps"; } }
                        if (!cap_left && !cap_bad && !dfr_regs && fc_tail_walk(g, before_pat, i_arb) && fc_tail_walk(g, i_b0, i_b1 + 1) && fc_tail_walk(g, i_b1 + 1, g->n)) {
                            { extern void fc_save_register(const IR_t *); extern void fc_cond_register(const IR_t *, int);
                              for (int d = 0; d < scd_n; d++) if (promo[d]) { fc_save_register(scd[d].save); fc_cond_register(scd[d].nd, scd[d].fp_inner); }
                              for (int w = 0; w < n_wrap; w++) fc_save_register(wsv[w]); }   /* WRAP-CAPTURE: the flat cell (delta at +0) -- its 16 joins fpl via fc_geom before finalize */
                            extern void fc_tail_candidate(const IR_t *, const IR_t *, int, int, int, int, int);
                            fc_tail_candidate(head, R, before_pat, i_arb, i_b0, i_b1, g->n);
                            { extern void fc_tail_wrap(const IR_t *, const IR_t *, const IR_t *);
                              for (int w = 0; w < n_wrap; w++) fc_tail_wrap(R, wsv[w], wcd[w]); }   /* WRAP-CAPTURE: finalize widens opsb by 16*nw and registers each COND's yield-depth read */

                            tail_ok = 1;
                        }
                    }
                }
            }
            if (!tail_ok && getenv("SCRIP_TAIL_DIAG")) fprintf(stderr, "[TAIL-DIAG] decline: %s\n", tl_why);
        }
        /* R12-ERAD s65 + ALT-LIFT: per-leaf flat displacement for ZC_FRAME_RSP via fc_leaf_walk (zeta_storage.c registrar).  Same range, same order (allocation = flow on the linear spine), prefix
         * starts at 32 = HEAD's self-pushed cell; each pattern node's body depth = prefix-before + own granted cell.  Granted ALT arms restart at prefix+16 (alternatives, not concatenation); nodes
         * after a granted ALT see prefix+16+fpmax (the pad stubs' uniform yield depth).  Registered only under the statement grant: a declined statement has no static depth (that is WHY it declined)
         * and its emission stays on the flat path, honestly broken under RSP until its own lift. */
    }
    /* OPERAND-EDGE HOIST (2026-07-10, SEMANTIC PIN 1 — manual p85-86: primitive args are captured at pattern
     * CONSTRUCTION, once per statement execution, BEFORE the scan begins; only *V defers).  Every runtime-arg
     * primitive collected by sno_pre_req gets: arg-expression chain → IR_COERCE_STRING/_INTEGER (SPITBOL error
     * codes on null cset / non-numeric / negative, oracle-pinned) → operand edge into the primitive's node.
     * Chains splice between the subject chain and head; SEQ lowers right-first so forward iteration here yields
     * left-to-right construction order.  The pre-chain runs ONCE per statement entry — never per anchor, never
     * per element α — so LEN(1) $ V BREAK(V) sees the PRE-statement V (oracle probe p5). */
    IR_t * after = head;
    for (int pi = 0; pi < cx->npre; pi++) {
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
/*--- SN4 kill-the-O(n^2) FINAL SHAPE (Lon: "DEFINE is a single statement"): an entry-in-main DEFINE needs a callable
 *--- proc with its OWN exitnd (correct RETURN via the runtime activation record) but NO re-lowered body — the body
 *--- statements already live in the ONE main graph, emitted once.  So mint a TINY 1-node ENTRY-STUB graph: the α is an
 *--- IR_GOTO_DEFERRED to the entry LABEL NAME (rt_goto_transfer resolves name -> the body's already-emitted address at
 *--- runtime, the same wire CODE fragments use to jump back into the body), and the stub carries its own exitnd/failnd
 *--- so RETURN/FRETURN unwind to THIS proc's epilogue, not the program exit.  emit_chain on the 1-node stub is cheap
 *--- and collision-free (no shared node re-walked).  O(1) per DEFINE. ---*/
static IR_graph_t * sno_build_call_stub(const char * entry_label) {
    IR_graph_t * g = IR_alloc(64);
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, exitnd, failnd);
    IR_LIT(gd).sval = lp_strdup(entry_label);
    IR_t * ad = lc_build(g, IR_SAVE_RESTORE, gd, failnd); IR_LIT(ad).ival = 3;   /* SN4-FLAT-PROC (s176) WIRE-ADOPT: copy the prologue-saved γ/ω wires + blob-entry rsp + caller rbp into the open pcall record BEFORE transferring into the body, so the program-wide RETURN/FRETURN floaters can restore machine state and jmp home from ANY depth — the body's exits no longer pass through this stub's exitnd at all */
    g->entry = ad;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name) {
    IR_graph_t * g = IR_alloc(nst * 16 + 256);
    scx_t cx; cx.g = g; cx.loop_exit = NULL; cx.loop_next = NULL; cx.result_name = result_name; cx.pat_fail = NULL; cx.pat_seal = NULL; cx.npre = 0;
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t ** anchor = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    IR_t ** fail_tgt = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));   /* STMT-BETA (cc39c095, Lon ruling 2026-08-06): parallel array saving each statement's fT so the post-loop STATEMENT_BEGIN wiring pass can set its omega port to fT, making the emitter wire statement_begin_beta -> fT as the named failure landing */
    IR_t ** match_land = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));   /* R1 STMT-BETA-LAND: per-statement fB (the dedicated MATCH_BEGIN-exhaust GOTO minted in sno_lower_match) -- the zw5 post-loop below retags each fB.γ to that statement's STATEMENT_BEGIN with the β port tag */
    bb_label_registry_reset();
    for (int i = 0; i < nst; i++) {
        anchor[i] = lc_build(g, IR_GOTO, NULL, NULL);
        { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(anchor[i]).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }   /* ZW-5 O-2: unconditional stno stamp (was MONITOR_BIN-gated) */
        const char * lbl = sfind_str(st[i], ":lbl");
        if (lbl && lbl[0]) bb_label_registry_add(lp_strdup(lbl), anchor[i]);
    }
    bb_label_registry_add(lp_strdup("END"), exitnd);
    /* SN4-FLAT-PROC (s176): in the ONE flat graph (and in EVAL/CODE fragment graphs) the reserved return labels are program-wide FLOATER BBs — manual Ch.8: RETURN yields the fname variable's value,
     * FRETURN signals failure, both restore the pushdown-saved formals/locals/fname; the floaters merely re-route control (peek the open activation's wires, restore rsp/rbp, jmp the γ/ω wire) and the
     * existing epilogue leaves on those wires perform the pop+restore+result protocol verbatim.  ANY goto — direct or $-computed — reaches them through this registry for free.  Level-0 transfer is a
     * runtime error inside rt_flat_ret_snap.  Def-body graphs (result_name != NULL) stay self-contained on their own exitnd/failnd exactly as before: their bodies are not statements of this graph. */
    if (!result_name) {
        IR_t * rf = lc_build(g, IR_SAVE_RESTORE, exitnd, failnd); IR_LIT(rf).ival = 1;
        IR_t * ff = lc_build(g, IR_SAVE_RESTORE, exitnd, failnd); IR_LIT(ff).ival = 2;
        if (!bb_label_landing("RETURN"))  bb_label_registry_add(lp_strdup("RETURN"),  rf);
        if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), ff);
        if (!bb_label_landing("NRETURN")) { IR_t * nrl = lc_build(g, IR_LIT_STRING, NULL, ff); IR_LIT(nrl).sval = (char *) ""; IR_t * nnd = lc_build(g, IR_CALL, rf, ff); IR_LIT(nnd).sval = (char *) "SNO$NRET"; lc_γ_to(nrl, nnd); ir_operand_push(nnd, nrl); bb_label_registry_add(lp_strdup("NRETURN"), nrl); }
    } else {
        if (!bb_label_landing("RETURN"))  bb_label_registry_add(lp_strdup("RETURN"),  exitnd);
        if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), failnd);
        if (!bb_label_landing("NRETURN")) { IR_t * nrl = lc_build(g, IR_LIT_STRING, NULL, failnd); IR_LIT(nrl).sval = (char *) ""; IR_t * nnd = lc_build(g, IR_CALL, exitnd, failnd); IR_LIT(nnd).sval = (char *) "SNO$NRET"; lc_γ_to(nrl, nnd); ir_operand_push(nnd, nrl); bb_label_registry_add(lp_strdup("NRETURN"), nrl); }
    }
    g->entry = (nst > 0) ? anchor[entry_idx] : exitnd;
    int _pro_open = 0, _pro_close = 0;   /* PS-3 (s152) prologue corridor: open at entry_idx, closed AFTER the first goto-bearing statement (its own assignment still executes first, so it records) */
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i];
        if (i == entry_idx) _pro_open = 1;
        if (_pro_close) _pro_open = 0;
        { extern void zls_group_mark_anchor(const IR_graph_t *, const char *, const IR_t *); const char * mlbl = sfind_str(s, ":lbl"); if (mlbl && mlbl[0]) zls_group_mark_anchor(g, lp_strdup(mlbl), anchor[i]); }   /* SN4-FLAT-PROC (s176): carry the anchor pointer so the mode-4 emitter can seed runtime-enterable chains (orphan-proof; see zeta_storage.c) */
        IR_t * next = (i + 1 < nst) ? anchor[i + 1] : exitnd;
        if (sfind(s, ":end")) { lc_γ_to(anchor[i], exitnd); continue; }
        const char * goU = sgoto(s, TT_GOTO_U);
        const char * goS = sgoto(s, TT_GOTO_S);
        const char * goF = sgoto(s, TT_GOTO_F);
        const tree_t * exU = goU ? NULL : sgoto_expr(s, TT_GOTO_U);
        const tree_t * exS = goS ? NULL : sgoto_expr(s, TT_GOTO_S);
        const tree_t * exF = goF ? NULL : sgoto_expr(s, TT_GOTO_F);
        IR_t * sT = goS ? sno_goto_target(g, goS, exitnd) : exS ? sno_goto_computed_target(g, &cx, exS, exitnd) : goU ? sno_goto_target(g, goU, exitnd) : exU ? sno_goto_computed_target(g, &cx, exU, exitnd) : next;
        IR_t * fT = goF ? sno_goto_target(g, goF, exitnd) : exF ? sno_goto_computed_target(g, &cx, exF, exitnd) : goU ? sno_goto_target(g, goU, exitnd) : exU ? sno_goto_computed_target(g, &cx, exU, exitnd) : next;
        IR_t * stb = zw5_on() ? lc_build(g, IR_STATEMENT_END, sT, fT) : (IR_t *) NULL;
        if (stb) { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(stb).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }   /* ZW-5 O-2: stb->ival = stno for per-depth stub label names */   /* ⭐⭐ ZW-5 SLICE 2 (OMEGA O-1): the statement bracket box is minted HERE because these two lines are the ONE derivation point every statement form threads its continuations through -- sT/fT are already resolved (goto field, computed goto, or fallthrough `next`), so one edit reaches every form without touching a single statement arm below.  THE BOX IS A TRAILER, NOT A BRACKET -- measured, not inferred: x86_asm.h:544 x86_alpha() DEFINES the alpha label while x86_asm.h:547 x86_gamma() IS A JMP, so the emitted body `def alpha / jmp gamma / def beta; jmp omega` has exactly ONE entry and control can never return into it; the box is entered once, at alpha, by the statement's SUCCESS wire and falls through to the jmp that carries op_zgpop via the ONE X86H_JMP gamma hook arm (s22k one-authority -- no second whack spelling is created here). */
        IR_t * sJ = lc_build(g, IR_GOTO, stb ? stb : sT, NULL);   /* success -> the box's alpha; the box's own gamma carries sT, so the statement's real continuation is unchanged and the ONLY delta is the release's HOME (WHACK CONTRACT clause 4: BB_END_STATEMENT is op_zgpop's home; the 5,923 fused pops are its absence). */
        fail_tgt[i] = fT;   /* STMT-BETA (this session): save fT at its ONE derivation point for the post-loop STATEMENT_BEGIN omega wiring */
        IR_t * fJ = lc_build(g, IR_GOTO, fT, NULL);   /* ⛔ FAIL EDGE DELIBERATELY UNCHANGED IN SLICE 2.  The rung's admission gate is "all fail edges arrive at depth 0", and a depth-0 arrival needs NO release -- so the box's omega is genuinely dead until slice 3's per-depth stub ladder lands WITH its planner (s22h atomicity).  Checked before assuming: the wire port selector in `.sz` carries alpha (0xce 0xb1) and beta (0xce 0xb2) and the sigma marker read at emit.cpp:2285 -- there is NO omega-ARRIVAL convention, so routing fail edges into the box is not a wiring detail that was skipped, it IS the slice-3 ladder. */
        if (is_def && is_def[i]) { lc_γ_to(anchor[i], sJ); continue; }
        if (_pro_open && (goU || goS || goF || exU || exS || exF)) _pro_close = 1;   /* PS-3 (s152): any goto part ends the unconditional corridor for SUBSEQUENT statements */
        const tree_t * subj = lc_stmt_subj(s);
        const tree_t * pat  = sfind_expr(s, ":pat");
        int has_eq = sfind(s, ":eq") != NULL;
        if (pat) sno_fatal("statement has a separate :pat field (stored-pattern form) — SN4-PAT-2 handles TT_SCAN match subjects only", NULL);
        if (subj && subj->t == TT_SCAN) {
            const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
            if (!sno_pat_supported(ptt)) {
                if (ptt && (ptt->t == TT_FNC || sno_is_pattern_rhs(ptt))) {
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
            IR_t * r = NULL; IR_t * e = sx_lower(&cx, subj, sJ, fJ, &r);
            lc_γ_to(anchor[i], e);
            continue;
        }
        tree_t * repl = sfind_expr(s, ":repl");
        if (subj->t == TT_VAR && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) {
            sno_reg_var(subj->v.sval);
            if (_pro_open && !result_name) sno_prologue_add(subj->v.sval);   /* PS-3 (s152): corridor pattern assignment = order-dominance license for the defer-tail arm */
            const char * bn = NULL;
            for (int fzi = 0; fzi < g_sno_nfz; fzi++) if (g_sno_fz[fzi].pat == repl) { bn = g_sno_fz[fzi].procname; break; }
            if (!bn) bn = sno_pat_collect(repl);
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fJ); IR_LIT(asn).sval = subj->v.sval;
            IR_t * mk = lc_build(g, IR_CALL, asn, fJ); IR_LIT(mk).sval = (char *) "SNO$MKPAT";
            IR_t * nl = lc_build(g, IR_LIT_STRING, mk, fJ); IR_LIT(nl).sval = (char *) bn;
            ir_operand_push(mk, nl);
            ir_operand_push(asn, mk);
            /* PAT-ARG-BIND (s104): args of runtime-arg primitives inside a STORED pattern evaluate HERE, at the
             * MKPAT assignment (manual: pattern-function args evaluate at pattern CONSTRUCTION, not match), into
             * hidden PAT$n$A<i> globals the patproc pre-chain reads.  The scratch sno_pat_node walk harvests the
             * arg trees in the IDENTICAL pre[] order the patproc build will see (same traversal), so index <i>
             * pairs by construction. */
            IR_t * pae = nl;
            {
                IR_graph_t * tg = IR_alloc(256);
                scx_t tx; tx.g = tg; tx.loop_exit = NULL; tx.loop_next = NULL; tx.result_name = NULL; tx.pat_fail = NULL; tx.pat_seal = NULL; tx.npre = 0;
                IR_t * tok = lc_build(tg, IR_SUCCEED, NULL, NULL);
                IR_t * tno = lc_build(tg, IR_FAIL, NULL, NULL);
                tx.pat_fail = tno; tx.pat_seal = tno;
                sno_pat_node(&tx, repl, tok, tno);
                for (int api = 0; api < tx.npre; api++) {
                    char abuf[48]; snprintf(abuf, sizeof abuf, "%s$A%d", bn, api);
                    IR_t * asnA = lc_build(g, IR_ASSIGN, pae, fJ); IR_LIT(asnA).sval = lp_strdup(abuf);
                    IR_t * av = NULL;
                    IR_t * ae = sx_lower(&cx, tx.pre[api].arg, asnA, fJ, &av);
                    ir_operand_push(asnA, av);
                    pae = ae;
                }
            }
            lc_γ_to(anchor[i], pae);
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
    if (zw5_on()) {
        for (int i = 0; i < nst; i++) {
            IR_t * fb = anchor[i] ? anchor[i]->γ.node : NULL;
            if (!fb) continue;
            IR_t * sbeg = lc_build(g, IR_STATEMENT_BEGIN, fb, fail_tgt[i]);   /* STMT-BETA (this session, Lon ruling 2026-08-06): omega = fT makes the emitter's DRIVE_PAIR wire statement_begin_beta -> fT; beta is the named failure landing for the statement scope (always-live per flat_beta_used_scan IR_STATEMENT_BEGIN addition in emit.cpp) */
            { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(sbeg).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }
            lc_γ_to(anchor[i], sbeg);
            if (match_land[i]) lc_γ_tag_β(match_land[i]);   /* R1 STMT-BETA-LAND: tag-only -- fB.γ stays = fJ (chain to fT intact for used-scan and all downstream consumers); β tag makes the emitter's chase propagate oib=1 and route to betas[sbeg_k].  lc_γ_to_β would set fB.γ = sbeg, severing the fJ→fT chain and causing the emitter to miss every statement after the scan (the 175 root cause: node 10@ was the GOTO chain to n11@→n12@→n13@ which held the second STATEMENT_BEGIN; redirecting its γ dropped n12@/n13@ from used[]).  MATCH_BEGIN.ω → fB (β-tagged GOTO → fJ → fT) -- the emitter chases, sees the β tag, routes to sbeg.β, AND the used-scan follows γ to fJ and beyond, keeping the full graph reachable. */
        }
    }   /* s26 BEGIN shim (Lon directive: IR_STATEMENT_BEGIN/END pair): interpose the statement HEAD bracket between each statement's anchor and its first box, AFTER every statement arm has wired -- the anchor is the ONE entry every path (fallthrough, static goto landing, rt_chain_enter via the s26 entry chase) passes through, so one post-loop pass brackets every statement form without touching a single arm, the exact shape of the g_sno_uses_stmtkw hook pass below.  stno stamped into ival same as the END trailer.  Same zw5_on() regime gate as the trailer: SCRIP_ZW5=0 reverts the whole pair. */
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
        if (!ssrc || !ssrc[0]) continue;
        IR_t * t = anchor[i];
        int hops = 0;
        while (t && t->op == IR_GOTO && t->γ.node && hops++ < 64) t = t->γ.node;   /* ⛔⭐⭐ ON-4 (s23e) — THE OBVIOUS FIX HERE IS NOT AN ANNOTATION CHANGE. DO NOT RE-ATTEMPT IT ON AN ANNOTATION RUNG.  The cross-statement pileup is real and diagnosed: convergent GOTO chains land several statements' anchors on ONE node, bb_src_note's append arm bundles them, and roman.s stacks FIVE unrelated source lines above n129 with four of them attributed to the wrong head.  The one-line cure is to stop the chase at a node another statement already owns (`&& !bb_src_of(t->γ.node)`).  MEASURED s23e: it moves EMITTED CODE — 9 of 21 benchmarks and 5 of 122 pattern crosschecks diverge.  ROOT CAUSE: bb_src_of is NOT a comment facility.  emit.cpp's zd_plan roots STATEMENT SEGMENTATION on it ("Runs are already rooted at bb_src_of statement heads"), so which node carries a source note decides where statement runs begin, which decides claims, offsets and depth — relocating a note relocates a statement boundary.  THEREFORE: this fix belongs to the ZD/segmentation ladder WITH a full ON-0 watermark bracket, never to an OBJ-NOTE rung whose whole contract is behaviour-neutrality.  The genuinely inert half of ON-4 landed instead: bb_src_note is now idempotent (exact-segment dedup), which changes the TEXT a node holds and never WHICH node holds one, so segmentation is untouched by construction. */
        if (t) bb_src_note(t, ssrc);
    }
    free(anchor);
    free(fail_tgt);
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
            /* expression-position DEFINE (e.g. DIFFER(DEFINE('f(n)','entry'))) — hoist-register identically to the statement path (last-define-wins, the standing compile-time deviation); sx_lower then folds the call to null */
            const char * entry_opt = NULL;
            if (t->n > argbase + 1 && t->c[argbase + 1]) {
                const tree_t * ea = t->c[argbase + 1];
                if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
                else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
            }
            sno_def_t d; sno_parse_define(t->c[argbase]->v.sval, entry_opt, &d);
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
    g_sno_in_patproc = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pat_count(void) { return g_sno_npat; }
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
        IR_t * pe = sno_pat_node(&px, g_sno_pats[pi2].pat, ok, no);
        {
            extern tree_t *ast_stmt_new(tree_e kind);
            IR_t * paft = pe;
            for (int api = 0; api < px.npre; api++) {
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
        gp->body_root = (gp->n > before_pat && !sno_pat_right_sealed(g_sno_pats[pi2].pat)) ? gp->all[before_pat] : NULL;
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
    g_sno_seal_enabled = 1;   /* s137 MAIN-LOWERING GATE grant: this entry is the whole-program lowering (fragments enter via sno_lower_fragment_at, never here) — see the flag doc at its definition */
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
    /* EVAL/CODE (manual Ch.9): a CODE fragment may goto BACK into a main-program label (its `:S(L)F(DONE)`
     * fields), so every main label must be transfer-reachable at runtime.  Each labelled statement becomes a
     * pseudo-proc "LBL__<name>" — the SAME statement array re-lowered with entry_idx at that label, exactly the
     * entry-label DEFINE shape below — flowing through the existing proc registration in BOTH mode drivers
     * untouched.  rt_goto_transfer resolves fragment-registry first (fragment labels OVERRIDE main's, per the
     * manual), then these.  Use-gated on g_sno_uses_code => byte-zero perturbation for programs without CODE. */
    /* EVAL/CODE (manual Ch.9): a CODE fragment may goto BACK into a main-program label (its `:S(L)F(DONE)`
     * fields), so every main label must be transfer-reachable at runtime.  Each labelled statement becomes a
     * pseudo-proc "LBL__<name>".  SN4 kill-the-O(n^2) (2026-07-22): the old shape re-lowered the ENTIRE
     * statement array once PER LABEL (entry_idx=i), so a P-label program pushed P*P zls group-marks and built
     * P near-identical graphs — beauty self-host (129 labels) overflowed the 8192 mark table before emitting a
     * line.  The graphs differ ONLY in which anchor is the entry, so instead we SHARE the main graph's bb_idx
     * and set proc_entry_node to that label's already-built anchor (bb_label_landing — the registry still holds
     * main's labels here, nothing re-lowered since the main build above).  emit_chain starts at proc_entry_node
     * (bb_proc_entry falls back to g->entry only when it is NULL), so each LBL__ proc emits the main graph from
     * its label onward using main's slot layout — one graph, N entry points, marks/slots no longer squared.
     * Use-gated on g_sno_uses_code => byte-zero perturbation for programs without CODE. */
    /* LBL__ FIXED (O(n), shared-graph): each labelled statement becomes LBL__<name> sharing main's bb_idx and
     * using proc_entry_node to enter the already-built main graph at that label's anchor node.  Zero extra graphs,
     * zero extra ZLS marks.  rt_goto_transfer finds "LBL__<name>" via rt_proc_get_fn → rt_chain_enter.
     * SCRIP_SN4_STUB gate removed: the DEFINE entry-stub is now LAW (SN4-FLAT-PROC s176, extraction deleted);
     * LBL__ is always produced when g_sno_uses_code (same gate as before), plus per-DEFINE-entry below. */
    /* LBL__ O(n) — share main's graph (bb_idx=pi's bb_idx); proc_entry_node points to each label's anchor
     * already in main's bb_label registry.  No sno_reach_body, no sno_build_graph, no extra ZLS marks.
     * bb_proc_entry() falls back to g->entry when proc_entry_node==NULL, so sharing is unconditional safe.
     * emit_chain walks from proc_entry_node through main's node graph using main's slot layout (correct: every
     * LBL__ entry is a slice of main's execution from that label onward, identical frame geometry). */
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
    /* SN4-FLAT-PROC (s176): every DEFINE entry label must be transfer-reachable at runtime — the stub's IR_GOTO_DEFERRED resolves it through rt_goto_transfer, whose arm 4 looks up "LBL__<name>".  When the
     * program uses CODE the block above already minted LBL__ for EVERY label; otherwise mint one per unique entry label here, the same shared-graph shape (main's bb_idx, proc_entry_node = the label's
     * already-built anchor).  This pass runs BEFORE the DEFINE loop because a def-body arm below re-lowers through sno_build_graph, which RESETS the label registry the anchors are read from. */
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
            if (eidx < 0) continue; /* entry label doesn't exist anywhere: this DEFINE is dead (never callable); SPITBOL only resolves an entry at call time, not at DEFINE time, so a program that never calls it must still run (132_pat_fence_eps_recur_shallow) */
            gf = sno_build_call_stub(defs[di].entry);   /* SN4-FLAT-PROC (s176): the extraction regime is retired — the body statements live ONLY in the one main graph; the stub is the callable citizen (jmp-entry prologue carves a fresh MAIN-layout frame via the driver's frame floor, WIRE-ADOPT records the way home, IR_GOTO_DEFERRED transfers to the entry label at CALL time through the same registry rt_goto_transfer serves).  (void)rn: the proc record below still carries result_name for the epilogue leaves. */
        }
        int fpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name = defs[di].fname;
        g_stage2.proc_table[fpi].proc = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].nparams = defs[di].nnames;
        g_stage2.proc_table[fpi].nformals = defs[di].nformals;   /* NPSPLIT (s22w): nparams stays the FULL name set (save/restore + pname + pad bounds, every existing consumer); this new scalar is the arg boundary alone. */
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
    scx_t px; px.g = gp; px.loop_exit = NULL; px.loop_next = NULL; px.result_name = NULL; px.pat_fail = NULL; px.pat_seal = NULL; px.npre = 0;
    IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
    IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
    px.pat_fail = no; px.pat_seal = no;
    int before_pat = gp->n;
    IR_t * pe = sno_pat_node(&px, pat, ok, no);
    if (px.npre > 0) sno_fatal("runtime-operand primitive reached the RT recipe graph builder — recipes must bake literal args (B-RE contract)", NULL);
    gp->entry = pe;
    gp->resumable_callable = 1;
    gp->body_root = (gp->n > before_pat && !sno_pat_right_sealed(pat)) ? gp->all[before_pat] : NULL;
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
