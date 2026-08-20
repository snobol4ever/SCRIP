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
static int sno_kw_static_slot(const char * kw) { return kw ? rt_kw_index(kw) : -1; }   /* ⭐ KW-3b ONE AUTHORITY for "may this `&KW =` take the static-slot template": the block actually names the keyword (KW-4 deleted the armed half). Both TT_KEYWORD assignment sites consult THIS, never the two conditions separately, so the plain-assignment and the pattern-replacement forms cannot disagree about which keywords are retargeted (the s68/s70 spelled-twice law). A miss (-1) keeps the verbatim SNO$KWSET by-name call, which is what preserves today's behaviour for the keywords the block does not name (&ARB/&BAL/&REM/&FAIL and the pattern family) and what makes SCRIP_KW_STATIC unset byte-identical by construction. */
extern void global_register(const char * name);
extern int stage2_proc_grow(stage2_t * s2);
typedef struct { const tree_t * arg; IR_t * prim; int str; long codes; const char * snapg; } sprearg_t;   /* PB-1s: snapg != NULL marks a plain-ref SNAPSHOT entry (VALUE-LEAF class) -- prim is the in-chain IR_VAR whose sval the drain repoints at the hidden stage-2 global; arg is the source TT_VAR tree; str/codes unused */
typedef struct { IR_graph_t * g; IR_t * loop_exit; IR_t * loop_next; const char * result_name; IR_t * pat_fail; IR_t * pat_seal; sprearg_t pre[64]; int npre; } scx_t;
#define SNO_DEF_MAX 128
#define SNO_DEF_NAMES_MAX 64
typedef struct { const char * fname; const char * entry; const char * result_name; const char * names[SNO_DEF_NAMES_MAX]; int nnames; int nformals; } sno_def_t;   /* NPSPLIT (s22w): nformals = the (…) segment count alone; nnames stays the FULL save set (formals then locals, in prototype order — formals-first is load-bearing for arg index -> gk mapping in the slim install). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SNO_EXPR_MAX 4096
static struct { const char * name; const tree_t * expr; int salt; int want_name; } g_sno_exprs[SNO_EXPR_MAX];
static int g_sno_nexpr = 0;
static int g_sno_expr_salt = 0;
void sno_expr_salt_next(void) { g_sno_expr_salt++; }
static const char * sno_expr_collect(const tree_t * expr);
#define SNO_PAT_MAX 256
static struct { const char * name; const tree_t * pat; int salt; } g_sno_pats[SNO_PAT_MAX];   /* PATSALT: the row carries its EVAL salt; the EXISTING table widens, g_sno_seal's `val` precedent */
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
    g_sno_exprs[g_sno_nexpr].want_name = 0;
    return g_sno_exprs[g_sno_nexpr++].name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* WANT-NAME VARIANT (140/141 fix): like sno_expr_collect but marks the thunk want_name=1.  Used when a deferred
 * capture target is a NRETURN-capable FNC call with args (e.g. *inner(c1) in `.` or `$` position).  The thunk
 * emitter prepends SNO$WANTNM so rt_g_want_name=1 is live when the inner FNC call runs its prologue, causing
 * the epilogue's rt_nret_fix to preserve DT_N instead of dereferencing it to the current value. */
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
static const char * sno_t4_target(const char * op, int nops);   /* T4 OPSYN-FOLD resolver -- definition beside sno_fz_build_table */
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
    /* CALL2BB slice 1 (Lon directive s21x: "DEFINE, when CONSTANT FOLDED, emits exactly TWO BBs: IR_DEFINE and IR_CALL") — behind SCRIP_CALL2BB=1 the call site becomes the two-BB pair: role-0
     * IR_DEFINE (carves its OWN slots, saves the fname/formals/locals save-set, opens the pcall residue, installs staged args into the NV globals) chained γ-wise INTO the slimmed IR_CALL (frame
     * dance + transfer + restore landings).  sr0 carries the SAME sval (shared strdup, read-only) and the SAME arg operand list so its drive arm marshals slots exactly like the call family; args chain
     * terminates at sr0 instead of call.  Gate OFF = this function byte-identical to its prior body (sr0 never built, tail == call). */
    IR_t * call = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(call).sval = (char *) lp_strdup(name);
    IR_t * sr0 = NULL; static int c2bb = -1; if (c2bb < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); c2bb = (e2 && *e2 == '1') ? 1 : 0; }
    if (c2bb) { sr0 = lc_build(cx->g, IR_DEFINE, call, ω); IR_LIT(sr0).sval = IR_LIT(call).sval; }   /* UNION-TAG (IR.h: sval/ival alias): writing sval IS the role-0 mark — the slice-1 `ival = 0` here was a dead store the sval write clobbered; the dispatch normalizes pointer-vs-1..3 back to role 0..3 */
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
static const tree_t * sno_const_val(const char * ck);   /* ⭐ SN4-CONSTANTS T1 forward declaration: the seal table and its resolvers are defined with the rest of the constant machinery ~700 lines below, but the FOLD belongs in sx_lower's TT_KEYWORD arm, which is here. Declared rather than moved so the constant machinery stays in one block. */
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = lc_build(cx->g, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; if (res) *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; if (res) *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
    case TT_NUL:  { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    case TT_VAR:  { sno_reg_var(t->v.sval); IR_t * nd = lc_build(cx->g, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; if (res) *res = nd; return nd; }
    case TT_KEYWORD: { if (t->v.sval) { char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval); const tree_t * cv = sno_const_val(cb); if (cv) return sx_lower(cx, cv, γ, ω, res); }   /* ⭐⭐⭐ SN4-CONSTANTS T1 -- THE FOLD. A read of a DECLARED scalar constant lowers as the LITERAL ITSELF: no IR_KEYWORD_SNOBOL4, no by-name string, no rt_keyword_read_snobol4 call, no result cell. Recursing into sx_lower rather than minting a literal node here is the ONE AUTHORITY choice -- TT_ILIT/TT_FLIT/TT_QLIT are lowered by the three arms directly above this one, so T1 adds NO new emission path and inherits their descriptor shapes for free; a hand-built node here would be the same fact spelled twice. Termination is structural: the stored tree is a literal by sno_const_scalar_tree's admission test, so the recursion is exactly one level and cannot cycle the way a `&A = &B` chain could. 342-AT-FOLDED-SITES follows the PT-3/T2 precedent DELIBERATELY (recorded, not overlooked): a read textually preceding the declaration's EXECUTION yields the value instead of raising 342, exactly as an inlined pattern does at a *&Name site; the guarantee is the language's seal, and a name never declared anywhere is absent from the table, so sno_const_val misses and 342 stays live for it -- which is what corpus/probe/cn/cn_read_before.err_sno witnesses. */
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
        { const char * _tg = sno_t4_target(name, t->n); if (_tg && sno_predef_registered(_tg)) return sx_call_named(cx, _tg, t, 0, γ, ω, res); }   /* ⭐⭐⭐ T4 OPSYN-FOLD: the symbol resolves to its constant binding at LOWER TIME -- the use site lowers as the DIRECT DEFINE'd-function call (same sx_call_named, same operands, argbase 0), bypassing the by-name chain that m4 cannot route to SNOBOL-defined targets (B1, FINDING s156).  Table + admission rules at sno_t4_scan above; killswitch SCRIP_OPSYN_FOLD=0 skips this consult. */
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
            /* MODE34-5b-BUGB FIX (this session): `X = subject ? pattern` (a match EXPRESSION as an
             * ordinary assignment RHS, no `= replacement` clause) previously hit sx_lower's TT_SCAN case,
             * which discards its result (*res = NULL, see that case a few lines up) -- nothing upstream
             * ever grants the match a value slot, so bb_assign_global/var/local's guard (needs op_zres OR
             * a legacy op_a_slot+op_off pair) is never satisfied and every such statement hits the
             * x86_bomb "unhandled" stub, unconditionally, both modes, any pattern.  Oracle-confirmed
             * (SPITBOL manual p.72's own Error #212 example establishes `?`'s subject is a name-position
             * construct, not a general value producer; but `X = S ? P . R`-STYLE plain assignment DOES
             * have a defined value -- verified against /home/claude/x64/bin/sbl: `X = 'ABCDEFG' ? 'ABC' . R`
             * gives X='ABC' (the whole matched span, same value a `.` capture would receive), and on match
             * failure X keeps its PRIOR value and the statement fails via the normal goto fields -- exactly
             * SNOBOL4's universal "failed statement, no LHS side effect" rule, needing no new mechanism).
             * FIX: rather than teach sno_lower_match's core IR spine a new value-producing exit (invasive --
             * it is built as pure control flow, no result cell, and re-plumbing it risks the capture/FC/ZD
             * machinery broadly), rewrite the TREE before lowering: `X = subject ? pattern` (no repl) becomes
             * `subject ? (pattern . X)` -- an ordinary whole-pattern `.` capture into X, which is the EXACT
             * value the oracle probe measured and is already a fully-supported, well-tested capture shape.
             * `X` no longer participates in IR_ASSIGN as a value target for this statement (the capture IS
             * the assignment); asn is therefore built but left OPERAND-EMPTY and unreachable garbage --
             * replaced by returning the capture-wrapped scan directly.  Guarded narrowly: only fires when
             * R is a bare TT_SCAN with no replacement clause (R->n==2; a 3rd child would be a `? P = repl`
             * REPLACE form, already handled correctly elsewhere and NOT this defect) and the pattern side
             * exists; anything else keeps the original sx_lower(R) path unchanged. */
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
        }   /* ⭐ KW-3b: the retargeted plain `&KW = value`. Shape follows the SNO$KWSET call it replaces EXACTLY except that the name-literal node is gone -- the value is still lowered first under the same ω, still γ-chained into the writer, and the writer is still the node handed back as the statement result, so the assigned value remains the statement's value (the builtin ended `*out = args[1]`). The keyword NAME rides sval to the template, which resolves it to the block index at EMIT time and seals it; nothing carries a name string into the emitted program. */
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
static const tree_t * sno_const_pat(const char * ck);   /* ⭐ CN-12 forward decls: the fold below chases the DECLARED-CONSTANT table (defined ~50 lines down with the rest of the constant machinery), the bare-var scalar resolver, and the fz single-write table -- three resolvers, ONE fold. */
static const tree_t * sno_fz_tree(const char * var);
static const tree_t * sno_var_val(const char * nm);
static const char * sno_cset_fold(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT) return a->v.sval ? a->v.sval : "";
    if (a->t == TT_ILIT) { char nb[24]; snprintf(nb, sizeof nb, "%lld", (long long) a->v.ival); char * ob = (char *) malloc(strlen(nb) + 1); if (!ob) return NULL; strcpy(ob, nb); return ob; }   /* ⭐ CN-12: an integer in cset position is its decimal string (SPITBOL coercion) -- reachable now that declared scalars fold through the arm below (`&D = 9` · SPAN(&D)). */
    if (a->t == TT_VAR && a->v.sval) { static int _vf = -1; if (_vf < 0) { const char * e = getenv("SCRIP_PAT_INLINE"); _vf = (!e || *e != '0') ? 1 : 0; } if (_vf) { const tree_t * vv = sno_var_val(a->v.sval); if (vv && vv != a) return sno_cset_fold(vv); const tree_t * vt = sno_fz_tree(a->v.sval); if (vt && vt != a) return sno_cset_fold(vt); } return NULL; }   /* ⭐⭐⭐ CN-12 -- A SINGLE-WRITE VAR'S CSET ARGUMENT FOLDS THROUGH THE SAME INFERENCE THE PT-1/PT-3 INLINE ALREADY TRUSTS. sno_fz_tree answers only for names proven invariant (one textual write, fz-safe program, not enclosure-hostile at its own gate), which is exactly the guarantee folding needs; rides the SCRIP_PAT_INLINE killswitch because it is the same trust. This is the arm that unfreezes beauty's real shapes -- SPAN('.' digits &UCASE '_' &LCASE) was refused for the DIGITS leaf alone. */
    if (a->t == TT_KEYWORD && a->v.sval) {
        static const struct { const char * n; const char * v; } kc[] = { { "lcase", "abcdefghijklmnopqrstuvwxyz" }, { "ucase", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" } };
        char lk[16]; size_t li = 0; for (; a->v.sval[li] && li < sizeof lk - 1; li++) lk[li] = (a->v.sval[li] >= 'A' && a->v.sval[li] <= 'Z') ? (char)(a->v.sval[li] - 'A' + 'a') : a->v.sval[li]; lk[li] = 0;
        for (size_t k = 0; k < sizeof kc / sizeof *kc; k++) if (!strcmp(lk, kc[k].n)) return kc[k].v;
        { char cb[130]; snprintf(cb, sizeof cb, "&%s", a->v.sval[0] == '&' ? a->v.sval + 1 : a->v.sval); const tree_t * cv = sno_const_val(cb); if (cv) return sno_cset_fold(cv); const tree_t * cp = sno_const_pat(cb); if (cp && cp != a) return sno_cset_fold(cp); }   /* ⭐⭐⭐ CN-12 -- A DECLARED CONSTANT'S CSET ARGUMENT FOLDS AT COMPILE TIME (Lon's order, s161-2: the declaration is a guarantee and no consumer may refuse it). Scalar table first (T1's literal: `&V = "aeiou"` · SPAN("xyz" &V)), then the pattern table for CAT-of-literal chains the registration classified pattern-shaped. Both resolvers carry their own killswitch gates (STATIC/T1) so the =0 arms restore the old bytes; recursion terminates because stored trees are finite and a cyclic chain bottoms out in the resolvers' own miss (a chain member not yet registered resolves NULL -- statement order, CN-3b). Before this arm the fold's keyword knowledge was a HARDCODED TWO-NAME TABLE (lcase/ucase) -- measured s161: `&T = "\t"` · SPAN(" " &T) refused for exactly that reason. */
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
static int g_sno_pat_match_ctx = 0;   /* PAT-INLINE: 1 only inside sno_lower_match's pattern lowering (the statement's real match-position walk).  The scratch harvest walk (PAT-ARG-BIND, ~2074) and the patproc build walk run with 0 — both must lower bare refs as DEFER so the $V<i>/$A<i> identical-traversal invariant holds and the assignment snapshot chains keep minting. */
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
static struct { const char * name; const tree_t * pat; const tree_t * val; } g_sno_seal[SNO_PAT_MAX]; static int g_sno_nseal = 0;   /* ⭐⭐⭐ SN4-CONSTANTS T1 widens the EXISTING seal table rather than adding a second one: `pat` answers "what pattern graph did the program declare for this name" (CN-3/T2) and `val` answers "what scalar value did it declare", for the SAME name, in the SAME row, found by the SAME scan. A parallel `g_sno_cval[]` array is the shape the NO-NEW-GLOBALS FACT RULE names outright and is also the s68/s70 spelled-twice disease -- two tables that must agree about which names are declared and would silently drift the first time one registration site was edited without the other. Exactly one of the two fields is ever non-NULL for a given row: a declaration is either pattern-shaped or scalar, never both, and the first one noted wins (runtime 341 rejects the second). */
static void sno_seal_note(const char * nm, const tree_t * pat) { if (!nm || !pat || g_sno_nseal >= SNO_PAT_MAX) return; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return; g_sno_seal[g_sno_nseal].name = nm; g_sno_seal[g_sno_nseal].pat = pat; g_sno_nseal++; }
static const tree_t * sno_seal_pat(const char * nm) { if (!g_sno_seal_enabled || !nm || (g_sno_fz_unsafe && !getenv("SCRIP_FZ_FORCE")) || sno_fz_wrcount(nm) != 1) return NULL;   /* ⛔ SCRIP_FZ_FORCE IS A DIAGNOSTIC ONLY, NEVER A FIX (s182, HQ Fable): it overrides the whole-program EVAL/CODE/CONVERT/CLEAR/indirect-assign poison so a seat can MEASURE how much of a program's behaviour the poison alone is responsible for.  It is UNSOUND by construction -- the poison exists because a runtime fragment can rewrite any name, so forcing seals on can fold a pattern that EVAL later changes.  The sound cure for the same programs is the DECLARATION road (sno_const_pat, one screen down, which never consults the poison because CN-2 seals the cell in the language).  No new state: getenv is read at the guard, deliberately, so the diagnostic cannot be left half-armed in a static. */ for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return g_sno_seal[i].pat; return NULL; }
static int sno_pat_right_sealed(const tree_t * t);
static int sno_const_feature(int set_off) { static int _cs = -1; if (set_off) { _cs = 0; return 0; } if (_cs < 0) { const char * e = getenv("SCRIP_CONST_STATIC"); _cs = (e && *e == '0') ? 0 : 1; } return _cs; }   /* ⭐⭐ CN-4b: the declaration and the env killswitch share the ONE cell that already existed -- `_cs` stays a single function-local static, so CN-4 adds NO file-scope variable in this file either (the NO-NEW-GLOBALS FACT RULE is satisfied by construction, not by permission).  set_off=1 is the pre-scan reporting `&USER_DECLARED_CONSTANTS = 0`; it FORCES the cell rather than caching a second input, which makes the two orderings agree: force-then-read never re-consults the env (the `_cs < 0` test fails), and read-then-force overwrites a 1.  There is no path back to 1 by design -- a program that closes the namespace has closed it for the whole compilation, and re-opening it later cannot retroactively make the already-lowered sites legal. */
static int sno_const_static_on(void) { return sno_const_feature(0); }   /* ⭐ SN4-CONSTANTS CN-3 killswitch + CN-4b DECLARATION, ONE AUTHORITY (RULES.md): consulted by ALL THREE CN-3 sites -- the seal-table registration, the sno_pat_dfree `&` arm, and the defer-site stamp.  Every site must be gated or SCRIP_CONST_STATIC=0 is not byte-identical to pre-CN-3 and the A/B is vacuous (the s68 vacuous-gate conviction).  The dfree arm is the one that would bite hardest if left ungated: its `!spine` leg answers 1 for an argument-position keyword (`LEN(&MAXLNGTH)`) where the pre-CN-3 default answered 0, which is reachable from CLASSIC programs that contain no user constant at all.  ⭐⭐ CN-4b TWO INPUTS, ONE FACT: the env killswitch and the program's own `&USER_DECLARED_CONSTANTS = 0` are not two facts needing two homes -- they are two ways of answering the SINGLE question "is the declared-constants feature active for this compilation?", so they resolve in sno_const_feature and nowhere else.  Consulting a second flag at the three call sites would be precisely the spelled-twice disease (s68/s70).  Deliberately asymmetric: only `= 0` is honoured, because the born value is already 1 (the CN-4 runtime cell) and a program that merely re-affirms 1 must not become distinguishable from one that never mentioned the keyword -- that asymmetry is what keeps the default arm byte-identical. */
static const tree_t * sno_const_pat(const char * ck) { if (!sno_const_static_on() || !g_sno_seal_enabled || !ck) return NULL; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, ck)) return g_sno_seal[i].pat; return NULL; }   /* ⭐⭐⭐ SN4-CONSTANTS CN-3: the DECLARATION-backed twin of sno_seal_pat, keyed on canonical "&Name".  THE WHOLE POINT: sno_seal_pat must PROVE single-assignment by inference and therefore carries two gates a declared constant does not need -- sno_fz_wrcount(nm)!=1 (a per-name textual-write count) and g_sno_fz_unsafe (a WHOLE-PROGRAM poison: one EVAL/CODE/CLEAR or one indirect `$(...)` assignment anywhere voids static staging for EVERY name in the program).  A user constant carries the guarantee IN THE LANGUAGE -- CN-2 seals the cell itself (NV_t.is_const, error 341 on re-assignment, enforced at the cell and not the spelling, so OPSYN/indirect/FIELD aliasing cannot bypass it) -- so neither gate applies and neither is consulted here.  g_sno_seal_enabled IS kept: the runtime EVAL/CODE fragment compiler re-enters this file with fragment-local state and must stay conservative (s137 main-lowering gate).  A second textual `&X =` cannot corrupt this: sno_seal_note keeps the FIRST tree, and the runtime raises 341 when the second executes, so the first tree is correct for every reachable state. */
static int sno_const_t1_on(void) { static int _t = -1; if (_t < 0) { const char * e = getenv("SCRIP_CONST_T1"); _t = (e && *e == '0') ? 0 : 1; } return _t; }   /* ⭐ SN4-CONSTANTS T1 killswitch, default ON, exact mirror of SCRIP_CONST_INLINE (T2) one arm below. =0 restores the pre-T1 emission byte-identically, which is what makes the killswitch byte-identity gate meaningful; the FEATURE gate stays sno_const_static_on() so the declaration and the env knob keep resolving in one place. */
static int sno_const_scalar_tree(const tree_t * t) { return t && (t->t == TT_ILIT || t->t == TT_FLIT || t->t == TT_QLIT); }   /* ⭐ T1 admission: the three literal kinds sx_lower already lowers with ZERO runtime help (IR_LIT_INTEGER/REAL/STRING). Deliberately NOT widened to constant-folded expressions (`&N = 2 + 3`) -- that asks this pre-scan to evaluate arithmetic, which is the optimizer's job and a different rung; a miss here costs optimisation, never correctness. */
static void sno_const_note_val(const char * nm, const tree_t * val) { if (!nm || !val || g_sno_nseal >= SNO_PAT_MAX) return; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return; g_sno_seal[g_sno_nseal].name = nm; g_sno_seal[g_sno_nseal].val = val; g_sno_nseal++; }   /* ⭐⭐⭐ T1: FIRST-WINS, identical to sno_seal_note above -- a second textual `&X =` is refused here and raised as 341 when it EXECUTES, so the first tree is correct for every reachable state (the CN-3 argument, unchanged). */
static const tree_t * sno_const_val(const char * ck) { if (!sno_const_static_on() || !sno_const_t1_on() || !g_sno_seal_enabled || !ck) return NULL; if (rt_kw_index(ck) >= 0) return NULL; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, ck)) return g_sno_seal[i].val; return NULL; }   /* ⭐⭐⭐ SN4-CONSTANTS T1 resolver. ⛔ THE rt_kw_index GUARD IS LOAD-BEARING AND IS NOT BELT-AND-BRACES: CN-3's registration comment argues classification is free because "only a tier-3 USER CONSTANT can reach here with a pattern RHS" -- every tier-2 keyword takes an integer or a string, so sno_is_pattern_rhs IS the classifier for T2. T1 admits exactly integers and strings, so that argument does not carry over and the real predicate is required. rt_kw_index is the block's own finder (killswitch-INDEPENDENT: it consults the table, not rt_kw_static_on), returning >=0 for every tier-1 protected and tier-2 unprotected keyword -- so ONE AUTHORITY holds, this file still spells no keyword list, and `&CASE = 1` can never be folded into an immediate. g_sno_seal_enabled is kept for the same reason sno_const_pat keeps it: the EVAL/CODE fragment compiler re-enters this file with fragment-local state and must stay conservative. */
static const tree_t * sno_var_val(const char * nm) { if (!g_sno_seal_enabled || !nm || nm[0] == '&' || g_sno_fz_unsafe || sno_fz_wrcount(nm) != 1) return NULL; for (int i = 0; i < g_sno_nseal; i++) if (!strcmp(g_sno_seal[i].name, nm)) return g_sno_seal[i].val; return NULL; }   /* ⭐⭐⭐ CN-12 -- the BARE-VAR scalar resolver, guard-for-guard the mirror of sno_seal_pat one screen up: same table, same single-write proof (sno_fz_wrcount), same whole-program fz poison, same fragment-compile gate. A bare name carries NO language guarantee, so unlike sno_const_val it must EARN the fold by inference -- which is exactly the division sno_seal_pat's own comment documents for patterns. Returns the stored QLIT for `digits = '0123456789'`-class names; consumed only by sno_cset_fold's TT_VAR arm behind SCRIP_PAT_INLINE. */
static int sno_defer_sealed(const char * nm) { const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_right_sealed(p) : 0; }   /* s137: defer target resolves (eligibly) to a right-sealed tree → IR_t.seal */
static int sno_pat_dfree(const tree_t * t, int spine, int depth) {   /* ZD-5 STATIC-SHAPE (s23i): 1 iff no TT_DEFER is reachable, resolving SPINE-position VAR names through g_sno_seal (eligibility = sno_seal_pat's own gate: single write, fz-safe, main lowering).  spine=0 marks primitive-ARGUMENT subtrees, where a VAR is a value read (LEN(N)'s N), never a pattern name.  Depth cap breaks bare-name chase cycles (A=B;B=A) conservatively.  TT_FNC conservative 0: a build-time call can return a pattern carrying defers; widen only on census evidence.  Unlisted kinds conservative 0 for the same reason -- the classifier's job is a PROOF of non-re-entry, not a guess. */
    if (!t) return 1;
    if (depth > 48) return 0;
    switch (t->t) {
    case TT_DEFER: return 0;
    case TT_QLIT: case TT_ILIT: case TT_FLIT: case TT_CSET: case TT_NUL: return 1;
    case TT_REM: case TT_ARB: case TT_FAIL: case TT_SUCCEED: case TT_ABORT: case TT_BAL: return 1;
    case TT_VAR: { const char * nm = t->v.sval; if (!spine) return 1; if (nm && (!strcmp(nm, "REM") || !strcmp(nm, "ARB") || !strcmp(nm, "FENCE"))) return 1; const tree_t * p = sno_seal_pat(nm); return p ? sno_pat_dfree(p, 1, depth + 1) : 0; }
    case TT_KEYWORD: { if (!sno_const_static_on()) return 0; if (!spine) return 1; if (!t->v.sval) return 0; char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval); const tree_t * p = sno_const_pat(cb); return p ? sno_pat_dfree(p, 1, depth + 1) : 0; }   /* ⭐ SN4-CONSTANTS CN-3: a constant may be BUILT FROM constants (`&Item = &Word | &Num`), so the transitive closure must chase the `&` namespace too, through the declaration-backed resolver.  Conservative on miss exactly as the TT_VAR arm is: a real keyword (`&ARB`, `&BAL`) is never seal-noted -- it cannot take a pattern RHS -- so sno_const_pat returns NULL and this returns 0, which is the pre-CN-3 verdict for this kind.  Depth cap is the shared one; the same `A=B;B=A` cycle guard applies to `&A = &B` chains.  ⛔ MEASURED INERT AT LANDING, recorded so the next seat does not re-derive it: this arm CANNOT FIRE TODAY, because a tree containing a bare TT_KEYWORD never reaches the seal table -- sno_pat_supported() has no TT_KEYWORD arm and returns 0, so the registration in the pre-scan refuses `&Item = &Word | &Num` outright.  Kept, not deleted, because CN-3c needs exactly this arm plus its two siblings; the missing third is the ONLY blocker.  ⛔ AND THE THIRD IS NOT A ONE-LINER: sno_pat_supported asserts THE LOWERER CAN EMIT THIS TREE, so admitting bare `&Word` in pattern position is a claim about the keyword read path (a pattern-valued keyword read is the IR_MATCH_VALUE/dynamic class, not the static one), and asserting it without proving the emission would seal-note a tree the lowerer cannot build -- staging a graph for `*&Item` that has no emitter. */
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX: case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS: { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 0, depth + 1)) return 0; return 1; }
    case TT_SEQ: case TT_CAT: case TT_ALT: case TT_FENCE: case TT_ARBNO: { for (int i = 0; i < t->n; i++) if (!sno_pat_dfree(t->c[i], 1, depth + 1)) return 0; return 1; }
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: case TT_CAPT_CURSOR: return 0;   /* ⛔ BRACKETED s23i (core.3397, 127_pat_json_keyvalue): a capture INSIDE the referenced blob emits rt_cap_push with the raw cap-slot spelling `lea rdi,[rsp+176]` -- claim-relative, so ENTRY-REGIME-DEPENDENT: the blob is compiled once but an armed statement enters it at a shifted depth, the slot read lands on stale stack residue, and survival depends on absolute stack placement (the env-length flip; s23h flake-ledger disease, same wild-rt_cap_push class as that finding's item 4).  The sound blob spelling is a wire/anchor-carried claim base (the CARRIED-OPEN r9 park-address item), NOT rsp arithmetic -- until that design rung lands, a target bearing ANY capture refuses.  127/152 were the deterministic witnesses; 125/146's greens were placement luck on the same defect. */
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
static const char * g_sno_encl[SNO_PAT_MAX]; static int g_sno_nencl = 0;
static void sno_encl_add(const char * nm) { if (!nm) return;
    for (int i = 0; i < g_sno_nencl; i++) if (!strcmp(g_sno_encl[i], nm)) return; if (g_sno_nencl < SNO_PAT_MAX) g_sno_encl[g_sno_nencl++] = nm; }
static void sno_encl_mark_all(const tree_t * t) { if (!t) return; if (t->t == TT_VAR && t->v.sval) sno_encl_add(t->v.sval); for (int i = 0; i < t->n; i++) sno_encl_mark_all(t->c[i]); }
static void sno_encl_scan(const tree_t * t) { if (!t) return; if (t->t == TT_ARBNO || sno_is_fence(t)) { sno_encl_mark_all(t); return; } for (int i = 0; i < t->n; i++) sno_encl_scan(t->c[i]); }
static int sno_encl_hostile(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_sno_nencl; i++) if (!strcmp(g_sno_encl[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐⭐ T4 OPSYN-FOLD (s161; Lon's ruling 2026-08-19: "constant folding should get us home to beauty self host") — a LITERAL OPSYN('op','Target',arity) is a compile-time-visible CONSTANT OPERATOR
 * BINDING, the operator-namespace sibling of T1's scalar fold: resolve the symbol to its target AT LOWER TIME so the use-site lowers as a DIRECT call to the DEFINE'd function instead of a by-name
 * call on the symbol — the by-name chain is exactly B1 (FINDING s156: core_call_registered_fn cannot reach SNOBOL-defined targets in m4; beauty's grammar is OPSYN('&','reduce',2)/('~','shift',2)
 * and every rule evaluation silently nulls).  TEXTUAL-FOLD-IS-THE-SPEC per the T1/PT-3 precedent (routed, ARCH-SN4-CONSTANTS §T1-FOLD-SEMANTICS).  ADMISSION IS CONSERVATIVE and mirrors T1's:
 * operator-shaped symbol (non-identifier lead char), all three args literal, arity 1 or 2, SAME op re-bound differently => that op poisoned, ANY OPSYN we cannot read exactly (variable args,
 * .NAME operands, operator-shaped with missing arity) => the WHOLE table poisoned — we cannot know which operator moved.  Function-alias form OPSYN('NEW','OLD') is NOT ours and NOT poisonous:
 * it moves no operator.  Fold fires only when the target is a prescan-registered DEFINE (sno_predef_registered) — builtin targets keep today's path (their by-name dispatch WORKS; no reason to
 * risk arg-convention drift).  Killswitch SCRIP_OPSYN_FOLD=0 restores the by-name lowering byte-identically (the fold is a lowering-time substitution; OFF emits the pre-T4 bytes by construction). */
typedef struct { const char * op; const char * tgt; int arity; int poisoned; } sno_t4_t;
static sno_t4_t g_sno_t4[16];
static int g_sno_nt4 = 0, g_sno_t4_unsafe = 0;
static int sno_t4_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_OPSYN_FOLD"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int sno_t4_opchar(char c) { return !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '.'); }
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
          g_sno_t4_unsafe = 1;   /* operator-shaped OPSYN we could not read exactly */
          return; }
      if (op && tg) return;      /* function-alias form, fully literal: moves no operator */
      g_sno_t4_unsafe = 1; }     /* non-literal OPSYN anywhere: unknown operator moved */
}
static const char * sno_t4_target(const char * op, int nops) {
    if (!sno_t4_on() || g_sno_t4_unsafe || !op) return NULL;
    for (int i = 0; i < g_sno_nt4; i++)
        if (!g_sno_t4[i].poisoned && !strcmp(g_sno_t4[i].op, op) && g_sno_t4[i].arity == nops) return g_sno_t4[i].tgt;
    return NULL;
}
static void sno_fz_build_table(const tree_t ** st, int nst) {
    g_sno_nfz = 0; g_sno_fz_unsafe = 0; g_sno_nfzw = 0; g_sno_npro = 0; g_sno_nencl = 0;
    g_sno_nt4 = 0; g_sno_t4_unsafe = 0;   /* T4: rebuilt per lowering invocation, exactly like the fz table */
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i]; if (!s) continue;
        sno_encl_scan(s);
        sno_t4_scan(s);   /* T4: deep-walk for OPSYN calls -- registration or poison, per the admission rules above */   /* PT-2b: whole statement — inline_ok gates the STORED tree, the hazard is the USE-SITE enclosure (070: DIGIT=ANY() inline-ok, ARBNO(*DIGIT) not; 128 FENCE in an RHS) */
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
            if (repl && repl->t == TT_QLIT && subj->v.sval && subj->v.sval[0] != '&') sno_const_note_val(subj->v.sval, repl);   /* ⭐⭐⭐ CN-12 -- BARE-VAR SCALAR STRINGS ENTER THE SAME TABLE (the T1 widening precedent: never a second table). A plain `digits = '0123456789'` is a seal CANDIDATE exactly as the pattern line above -- candidacy is free and the GUARDS LIVE IN THE RESOLVER (sno_var_val: wrcount==1, !g_sno_fz_unsafe, killswitches), mirroring sno_seal_pat's own division of labor. This is the arm that lets sno_cset_fold see through beauty's SPAN('.' digits &UCASE '_' &LCASE) -- measured s161-2: the DIGITS leaf alone was the whole reason that shape refused. Key is the bare name (no '&'), so it can never collide with a canonical "&Name" constant lookup. */
            if (repl && g_sno_nfz < SNO_PAT_MAX && sno_is_pattern_rhs(repl) && sno_pat_supported(repl) && sno_pat_invariant(repl)) {
                g_sno_fz[g_sno_nfz].var = subj->v.sval; g_sno_fz[g_sno_nfz].pat = repl; g_sno_fz[g_sno_nfz].procname = NULL; g_sno_nfz++; }
            continue; }
        if (subj && subj->t == TT_KEYWORD) { if (subj->v.sval && repl) { const char * kn = subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval; if (!strcasecmp(kn, "USER_DECLARED_CONSTANTS") && repl->t == TT_ILIT && repl->v.ival == 0) sno_const_feature(1); }   /* ⭐⭐⭐ CN-4b -- THE DECLARATION REACHES THE LOWERER.  The runtime cell (keywords.c kwb_own[7]) governs the tier-3 namespace at RUN time; static staging happens at LOWER time and cannot read it, so the pre-scan reads the declaration TEXTUALLY -- the same discipline CN-3b already established for constant chains, where a fact must be visible earlier in program text to be usable.  Only a LITERAL 0 counts: `&USER_DECLARED_CONSTANTS = X` is a runtime value this pass cannot evaluate, and guessing would be worse than staying open (the runtime gate still fires and raises 251, so a missed compile-time close costs optimisation, never correctness -- the right failure direction).  strcasecmp, not strcmp, because SNOBOL4 case-folds keyword names under &CASE=1 while SCRIP itself is case-sensitive; the runtime read path lower-cases before comparing, so matching case-insensitively HERE is what keeps the two halves agreeing on which statement is the declaration. */
            if (sno_const_static_on() && subj->v.sval && repl && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) { char cb[130]; snprintf(cb, sizeof cb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval); sno_seal_note(lp_strdup(cb), repl); }   /* ⭐⭐⭐ SN4-CONSTANTS CN-3 -- THE REGISTRATION GAP.  Pre-CN-3 this arm skipped EVERY `&Name =` statement, so a constant's defining tree never entered g_sno_seal and `*&Name` at the defer site below could resolve nothing: that is exactly why CN-2 had to leave pat_static at the conservative 0.  CLASSIFICATION IS FREE AND NEEDS NO SECOND COPY OF THE KEYWORD LIST (ONE AUTHORITY, RULES.md): only a tier-3 USER CONSTANT can reach here with a pattern RHS, because every assignable tier-2 keyword (&ANCHOR &TRIM &STLIMIT &MAXLNGTH &FULLSCAN &DUMP &ERRLIMIT &CODE &CASE &FTRACE &TRACE &ABEND &COMPARE &PROFILE &ERRTEXT &ERRTYPE) takes an integer or string, and every tier-1 protected keyword refuses assignment outright (oracle error 209, measured this session) -- so the sno_is_pattern_rhs test IS the classifier.  sno_seal_note stores the pointer without copying, hence lp_strdup.  sno_snapref_scan mirrors the TT_VAR arm above so a constant's member stores are not dead-eliminated out from under a blob that will snapshot them (the PT-2 census hole, s6). */
            if (sno_const_static_on() && sno_const_t1_on() && subj->v.sval && sno_const_scalar_tree(repl)) { char vb[130]; snprintf(vb, sizeof vb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval); if (rt_kw_index(vb) < 0) sno_const_note_val(lp_strdup(vb), repl); } continue; }   /* ⭐⭐⭐ SN4-CONSTANTS T1 -- THE SCALAR HALF OF THE SAME REGISTRATION GAP CN-3 CLOSED FOR PATTERNS. A declared `&N = 42` is a compile-time-known integer sealed by the language (CN-2's NV_t.is_const, error 341 on re-assignment, enforced at the CELL so OPSYN/indirect/FIELD aliasing cannot bypass it), yet every read of it emitted a by-name string, a spine carve, three register spills, a call to rt_keyword_read_snobol4 and four reloads -- twelve-odd instructions and a PLT call to reproduce a value this pass is holding. ⛔ THE rt_kw_index TEST IS THE CLASSIFIER AND MUST STAY: the arm above may lean on sno_is_pattern_rhs because no tier-2 keyword takes a pattern, but this arm admits integers and strings, which is precisely what &ANCHOR/&TRIM/&CASE/&ERRTEXT take, so without it `&CASE = 1` would be registered as a user constant and every later read folded to the immediate 1 -- silently freezing a keyword the manual p.189 says outright "may be set to integer values to modify SPITBOL's behavior". lp_strdup because the table stores the pointer without copying (sno_seal_note's contract, shared). */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_in_arbno = 0;
static int sno_tree_has_varext(const IR_t * n, int d) { if (!n || d > 12) return 0; if (n->op == IR_MATCH_ARBNO || n->op == IR_MATCH_DEFER) return 1; for (int i = 0; i < n->n_operands; i++) if (sno_tree_has_varext(n->operands[i], d + 1)) return 1; return 0; }   /* SEQ-CELL fence (s21x-l, second falsification): the FIRST fence (body-resident SEQs) held but 066/164/165 persisted -- the mechanism is the SUSPENDED cell, not residency: a top-level SEQ's cell stays pushed across gamma while its ARBNO/DEFER descendants run, shifting every depth-sensitive window slot below it (the s202 defer-window class).  A SEQ over any variable-extent descendant therefore refuses wholesale, degrade never die. */   /* ZB-FC-3b: >0 while lowering an ARBNO body; balanced, so it always returns to 0 (EVAL/CODE mint fresh graphs in-process) */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* sno_cap_defer -- R12-EXIT-1 L1b (Lon ruling this session: "all ARBNO needs to know is what size of children it has" -- the capture joins the predetermined-size list).  A capture lowered INSIDE an
 * ARBNO body cannot register its FORTH cell at lowering time: the grant is only sound if the statement takes the ELEMENT path (LIFO fixed-size pushes, uniform depth), and candidacy is decided later;
 * on the anchored/heap-flavor path the old rsp-moves-per-iteration premise still holds and the flat rt_cap array stays correct.  So the walk runs UNCONDITIONALLY here (fp_inner is real), the
 * registration is DEFERRED, and the candidacy site PROMOTES iff the statement converts.  i_end = g->n at defer time = the inner allocation end (nesting detection).  The list is per-statement
 * (cleared at each statement's pattern lower entry); nested-in-arbno captures and captures inside granted-ALT arm extents stay unpromotable (their enclosing fp was baked without the cell) and force
 * the statement to refuse to the anchored window (degrade never die). */
static struct { const IR_t * nd; const IR_t * save; int nd_idx; int save_idx; int i_end; int fp_inner; } scd[64];
static int scd_n = 0;
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp);
static void sno_cap_defer_reset(void) { scd_n = 0; }
static void sno_cap_fc(IR_graph_t * g, IR_t * nd, IR_t * save, int before_i) {
    int fp_inner = 0; int walk_ok = fc_walk_range(g, before_i, g->n, 0, &fp_inner);
    if (!walk_ok) return;                                          /* inner not fc-linear: flat rt_cap path verbatim, both paths */
    if (sno_in_arbno == 0) { extern void fc_save_register(const IR_t *); extern void fc_cond_register_with_save(const IR_t *, const IR_t *, int); fc_save_register(save); fc_cond_register_with_save(nd, save, fp_inner); return; }
    if (scd_n >= 64) return;                                       /* silent defer-table overflow: capture stays ungranted, statement refuses at the cap scan */
    scd[scd_n].nd = nd; scd[scd_n].save = save; scd[scd_n].nd_idx = before_i - 2; scd[scd_n].save_idx = before_i - 1; scd[scd_n].i_end = g->n; scd[scd_n].fp_inner = fp_inner; scd_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_walk_range(IR_graph_t * g, int k0, int k1, int lit_ok, int * fp) {
    /* ALT-LIFT (the s65 named follow-on): the ONE eligibility+footprint walk all four grant sites share.  A GRANTED ALTERNATE contributes 16 (its own cell) + fpmax (the S10d padded arm -- only one arm
     * is live at yield, padded to max by the sigma stubs) and its arm allocation extent is SKIPPED (the naive range sum counted every arm's leaves = the exact over-count the pre-lift refuse existed
     * to avoid).  An UNGRANTED ALTERNATE still refuses the whole range.  fc_geom(ALT)=16 for granted ALTs, so the ALT case MUST run before the fc_geom catch or the arm extent is never skipped. */
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
        case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL: if (!lit_ok) lin = 0; else if (fp) *fp += 16; break;   /* R12-ERAD s65: constant primitive args allocate inline -- statement+SEQ op-filters only.  ⭐ L-3b CARVING-CLASS FIX (this session): this arm CONTRIBUTED 0 TO fp, but these three kinds are NOT in zd_k's K=0 exception list (emit.cpp), so the universal ZD arm (g_zd_arm, emit.cpp ~841: op_zres=1, op_fc_bytes=g_zd_k=16) unconditionally carves 16 bytes for them whenever admitted -- fc_geom's OWN grant (line above, fc_vlit_active-gated) is a DIFFERENT, narrower authority that this node may miss (e.g. a LIT_INTEGER used as a pattern-primitive's constant argument, TAB(14)'s '14') while still being admitted (lit_ok) and still carving via the ZD arm regardless.  Two authorities for the SAME carve decision (fc_geom's grant vs zd_k's universal K) is exactly the s22k \"spelled twice\" class this codebase's own law was written to forbid, and they disagreed here: fc_walk_range (this function) silently zeroed a carve that zd_k+the ZD arm actually emit, corrupting op_fc_disp for the WHOLE containing statement by exactly 16 bytes per uncounted literal argument, not just this node's own read.  CONFIRMED (l3_spl_tab_nonterm, TAB(14)): MATCH_END's splice-end write landed at a fixed offset from the writer-side RSP; the reader (bb_match_replace.cpp, now correctly using the ARB-fix regime-2 addressing) computed its own offset assuming op_fc_disp counted every un-popped carve between MATCH_BEGIN and MATCH_END -- LIT_INTEGER's uncounted 16 bytes shifted writer and reader RSP by a different amount, landing the read 16+ bytes from the written value (gdb-confirmed: the correct end cursor 14 sat 96 bytes from where the reader looked, consistent with more than one uncounted 16B cell stacking across the statement's LEN/LIT_INTEGER/TAB chain).  Matching zd_k's own K=16 default here is the ONE-AUTHORITY fix: fc_walk_range must count exactly what the ZD arm actually carves, not re-derive it from fc_geom's narrower eligibility. */
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
     * spine).  The ALT arm MUST run before the fc_geom catch (fc_geom(granted ALT)=16 -- the exact 163-regression slip the statement grant hit at s66).  An UNGRANTED ALTERNATE still refuses
     * wholesale; ARBNO/DEFER/unknowns refuse as ever.  Runtime-arg primitives are excluded at the CALLER via cx->npre (their pre-chain operand slots are FLAT, unreachable at element depth without
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
                                          * a non-uniform/unregistered target REFUSES the whole candidate there, chain-arm fallback).  The blob carves at its own α and its ω restores the entry
                                          * frontier ABSOLUTELY (lea rsp,[___+K]) so carve/release are exact; β = jmp [rsp+0] finds the γ-record by LIFO.  Same license trio as the s152 DT arm;
                                          * SCRIP_ARBNO_LATCH=1 arms (opt-in until monitor-proven -- default byte-identical, the walk keeps refusing). */
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
static int sno_cap_name_strict(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_CAP_NAME_STRICT"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* ⭐ SN4-CAP-NAME-STRICT (s170, row b1c-retreat, FINDING-2026-08-19-s170): DEFAULT ON since the s178 flip (Lon greenlight; =0 reverts byte-identically).  Oracle law, measured: the target of `.`/`$` is a NAME CONTEXT — a deferred `*VAR` names the VARIABLE ITSELF (never its value), and a deferred `*F()` supplies a name only via NRETURN; a plain RETURN yields a VALUE, is not a name, and the pattern node RETREATS (sbl 4.0f: `A . *F()` prints F's output then nomatch, ZZ unassigned).  SCRIP instead read `. *E` as `. $E` — evaluate, use the VALUE as an indirect name, always succeed — so three witnesses answer match where the oracle retreats.  =1 arms both halves: the *VAR target lowers to the plain variable, and a *CALL target that did not NRETURN fails the match through IR_MATCH_END's new ω.  =0 restores the pre-s170 graph and asm byte-identically (no ω wired, no test/branch emitted). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_rtseq_resume(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_RTSEQ_RESUME"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* ⭐⭐⭐ RTSEQ-RESUME killswitch (=0 restores the pre-fix emission byte-identically: sno_pat_node route + the raw gp->all[before_pat] publication).  Function-local static, so this adds NO file-scope variable -- the same construction sno_defer_resume and sno_const_feature already use to satisfy the NO-NEW-GLOBALS FACT RULE. */
static int sno_defer_resume(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_RESUME"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* ⭐ s124 DEFAULT INVERTED ON: half A of the SAME switch as emit.cpp's sn4_defer_resume -- s121's law that the two halves LAND TOGETHER binds their DEFAULTS too, and inverting only half B was measured this seat to be strictly WORSE than either uniform arm (seam 1/4 -> 0/4), which is exactly the "B without A is dead code" failure s121 named.  Both halves flip in lockstep or neither does. */   /* ⭐ SN4-DEFER-RESUME (s121, FINDING-2026-08-16-s120-arbno-stored-no-resume): ONE switch, TWO coupled halves that must land together — (A) the SEQ-RESUME-GATE below re-admits IR_MATCH_DEFER as a resume-bearing left neighbour, and (B) the PAT$ blob publishes a real resume carrier in body_root which emit.cpp's β dispatch lands.  A without B recreates the gate's measured rc=124 replay hang; B without A is dead code (no fail edge ever reaches the site β).  =0 restores the pre-s121 wiring byte-identically. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_contains_fence(const tree_t * t, int depth) { if (!t || depth > 64) return 0; if (sno_is_fence(t)) return 1; for (int i = 0; i < t->n; i++) if (sno_pat_contains_fence(t->c[i], depth + 1)) return 1; return 0; }   /* ⭐ SN4-DEFER-RESUME FENCE REFUSE (s121, board-convicted same session): the first cut published a resume carrier for EVERY unsealed blob and broke exactly the fence-bearing stored-pattern class, 7 movers one shape (114/119/129/130/148/149/150 — a fence NESTED in an ALT/ARBNO body slips both the top-level flatten check and sno_pat_right_sealed's rightmost-only chase, and the resume re-entry crosses a fence that backup must not re-run: FENCE0 aborts, FENCE1 passes through once, manual pp.203/222).  A blob whose TREE contains any fence anywhere therefore publishes NO carrier and keeps =0 behaviour verbatim; per-blob locality is compositional — a referenced inner pattern's blob applies its own guard, so VAR members are deliberately not chased (also the A=B;B=A cycle guard for free).  Witness set: the 7 named programs restored + arbnostore/earn0/180/181/183 kept. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_seq_nary(scx_t * cx, const tree_t ** elems, int ne, IR_t * succ, IR_t * fail, IR_t ** out_rtail) {   /* ⭐ FENCE-PASS-THROUGH: out_rtail (nullable) receives res[ne-1], the LAST element's resume carrier — the run's EXTERNAL right-side resume surface.  Under the deleted IR_MATCH_SEQUENCE the construct's own β dispatched this internally; SEQ-ERAD removed the construct and no plain caller needed the surface until the fence pass-through seam (a righter construct's exhaust must resume the run's rightmost generator, not element 0). */
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
        IR_t * prv = (i > 0 && (res[i - 1]->op != IR_MATCH_DEFER || (sno_defer_resume() && res[i - 1]->seal != 1))) ? res[i - 1] : fail;   /* ⭐ SN4-DEFER-RESUME (s121) LIFTS THE GATE — FOR UNSEALED DEFERS ONLY (seal!=1).  A seal==1 defer's β is the s137 fence-demarked WHACK (`mov rsp,rbp; pop rbp; ω`), an ε-cascade exhaust whose own frame is already popped at its γ: β-targeting it from a right neighbour whacks the ENCLOSING activation instead — measured this session as the 7-mover fence-via-var class (114 et al., rc0→139; the *cmd = FENCE(…) shape).  seal 0/2 β is `jmp [rsp+0]` through the CLASS D record — the resume this rung exists for.  The historical hang the old blanket exclusion cured is structurally impossible once emit.cpp's β dispatch (half B, same switch) lands the blob's resume carrier's β: a deterministic carrier's β fail-throughs home through the absolute unwind, so the site β can never replay a success; a generator's β EXTENDS (manual p.121 ARBNO retry, p.122 *P recursion).  witnesses: probe/arbnostore/ 3 RED + earn0_stored_varref hang repro green under =1, 114/119/129/130/148/149/150 green under =1 post-narrowing.  ORIGINAL GATE TEXT (kept, it is the =0 arm's law): */   /* SEQ-RESUME-GATE (s—): was unconditionally res[i-1] for ANY left neighbour -- correct for a true generator (ARB/ARBNO/ALTERNATE: β genuinely advances retry state; manual's 'O' ARB . X 'A' depends on exactly this wiring, SIGSEGVs if removed wholesale, measured) but wrong for IR_MATCH_DEFER, whose bb_match_defer is a jmp-entry TRANSFER box (bb_glue_pass_wires into the deferred pattern's own code), not a generator with retained state: "resuming" it re-transfers into the target from scratch and can succeed identically on replay, an infinite loop with no progress (MEASURED: earn0_stored_varref.sno / minimal `Q = P LEN(2)` on 'abc', rc=124 at HEAD).  Manual's OWN legitimate resumable-defer case, `*P` with P itself a generator (p.122), is ALSO broken today the same way (MEASURED: `Q = *P 'X'` with P=ARB hangs identically) -- excluding MATCH_DEFER here regresses nothing working; true nested-generator resume through a defer is unimplemented and out of scope for this fix.  Board-verified zero regression: crosscheck/patterns BY-SET diff REPAIRED=0 BROKEN=0 (76/122 both builds); 3 already-broken recursive-defer programs (178/179/182, manual p.122 shape) move SIG11->HANG, still wrong either way, diagnostic-only shift, not a correctness regression. */
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
    if (out_rtail) *out_rtail = (ne > 0 && ne < 128) ? res[ne - 1] : NULL;   /* FENCE-PASS-THROUGH: the run's external right-side resume surface */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_kw_chase(const char * nm, int op) { static const char * stk[24]; static int top = 0; if (op == 1) { if (nm && top < 24) { stk[top++] = nm; return 1; } return 0; } if (op == 2) { if (top > 0) top--; return 1; } if (op == 3) return top != 0; if (!nm) return 0; for (int i = 0; i < top; i++) if (!strcmp(stk[i], nm)) return 1; return 0; }   /* ⭐⭐⭐ CN-12 -- THE SUBSTITUTION CYCLE GUARD, the one piece that makes trusting recursive constants safe. ops: 0 = is nm being inlined somewhere up-stack, 1 = push (returns 0 when full = treat as on-stack, no inline), 2 = pop. A recursive grammar (`&R = 'q' *&R | 'z'`, beauty's &Expr14) inlines its FIRST level; the nested occurrence finds itself on the stack, refuses the substitution, and emits the ordinary defer -- dynamic re-entry at match time, which IS recursion's meaning (manual p.122: `*X` is the sole recursion form). Function-local statics per the house pattern (kdepth, _cs, g_snapctr) -- no file-scope state. Names pushed are lp_strdup'd canonical "&Name" keys, compared by strcmp. */   /* PAT-INLINE license = MEMBERSHIP IN THE POST-SWEEP FZ TABLE, the ONE AUTHORITY: a name is there iff its assignment passed the sno_pat_invariant gate (993 — literal leaves, constant primitive args) AND the single-write/fz-safe sweep (1001) — exactly the set whose refs reach a PAT$ blob today, so inline-set ⊆ blob-linkage-set BY CONSTRUCTION (no second predicate to drift; the sno_fz_mark_defer:962 precedent, same table, same !g_sno_in_patproc guard).  fz trees are CLOSED over plain names (the gate's TT_VAR arm admits only REM/ARB/FENCE), so inlining one can never trigger a nested inline — no lower-time cycle exists. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_kw_nest_ok(const char * nm) { static int _nn = -1; if (_nn < 0) { const char * e = getenv("SCRIP_CONST_NEST"); _nn = (e && *e == '1') ? 1 : 0; } return _nn ? !sno_kw_chase(nm, 0) : !sno_kw_chase((const char *)0, 3); }   /* ⭐⭐⭐ CN-15 -- THE SUBSTITUTION-DEPTH SELECTOR, default byte-identical.  OFF (default) = the s161 TOP-LEVEL-ONLY limit verbatim: substitute only when the chase stack is EMPTY.  ON (SCRIP_CONST_NEST=1) = the honest cycle test the stack was built for (op 0), so a keyword member INSIDE an already-substituted tree substitutes too and only a genuine self-reference refuses.  ⛔ ARMING IS BLOCKED ON SCRIP_SPAN_FRAME, NOT ON THIS SWITCH: lifting the limit turns an ALT arm's IR_MATCH_DEFER into a scratch-cell leaf, which re-arms the s130/s131 LEAF-SUSPENSION class (a leaf on an ALT arm addresses its ζ cell at a RAW FLAT ZLS COORDINATE off rsp and writes above the live frame once earlier statements push that coordinate past it) -- measured, both media, witness family corpus/probe/cn/cn_nest_alt_defer + probe/cn/cn_alt_leaf_flat_*.  With SCRIP_SPAN_FRAME=1 the same programs are green in both media. */
static int sno_pat_inline_ok(const tree_t * t) {   /* ⛔ PAT-INLINE SLICE-1 SHAPE GATE (exclusions are statement-regime frontiers the blob context happens to survive; inline may not route a passing program onto an open class, per may-only-add-passes): CAPTURES (`.`/`$`/`@`) + ARB → word1/word2/word3 regressed onto the already-failing 157_pat_cap_arb_alt_keep / 061-065_capture_* classes; FENCE → 116/145/151-family open; BAL → 174-176 open.  ADMITTED = literals, cset primitives with FOLDABLE args, integer primitives with ILIT args, REM, SEQ/CAT/ALT composition, and ARBNO over an admitted argument.  EACH EXCLUSION DELETES when its statement rung lands and the named witnesses pass inline.  ⭐ ARBNO EXCLUSION DELETED (SN4-ARBNO-STORE s120): its stated justification — "any second iteration SEGVs; z4_arbno rc=139" — is FALSIFIED AT HEAD.  Both named witnesses were re-measured PASSING before this edit: `'aa' POS(0) ARBNO('a') RPOS(0)` MATCHes in m3, and z4_arbno completes its 150000-iteration backtrack ladder at `arbno 150000` == oracle ref.  The statement-ARBNO rung landed and nobody deleted the gate, so every stored ARBNO kept falling to the PAT$ blob path, whose MATCH_ARBNO terminates in a closed SUCCEED/FAIL pair with NO resumable entry — the outer MATCH_RPOS ω then points at MATCH_BEGIN instead of back into the ARBNO, so it takes its shy null match (manual p.121) and is never retried.  That is the beauty.sno `Parse Error` (`Parse = ... ARBNO(*Command) ...` at :225 matched under POS(0)…RPOS(0) at :608).  Witnesses: corpus/probe/arbnostore/ (3 RED / 4 GREEN, oracle-anchored).  Killswitch SCRIP_PAT_INLINE_ARBNO=0 restores the exclusion. */
    if (!t) return 1;
    switch (t->t) {
    case TT_QLIT: return 1;
    case TT_REM: return 1;
    case TT_VAR: return t->v.sval != NULL;   /* ⭐⭐⭐ CN-12 (was: REM only) -- a named-var LEAF inside an inlined tree lowers through the PB-1s snapshot-defer arm, the SAME statement-regime mechanism every bare ref in pattern position already uses, so admitting it opens no excluded construct class. SEMANTICS (the T2 twin of the T1 §T1-FOLD-SEMANTICS amendment, adopted under Lon's s161-2 order): the leaf snapshots at the USE statement's stage-2, not at the stored pattern's MKPAT -- the arms diverge only for a program that MUTATES a var referenced by a constant's definition between assignment and use, the same degenerate class the T1 ruling accepted (declaration is the truth, no dominance analysis). fz-path safety is structural, not asserted: fz trees are CLOSED over plain names (their gate admits only REM/ARB/FENCE vars -- sno_fz_tree's own doc), so this widening cannot change what the PT-1/PT-3 inference path inlines. */
    case TT_KEYWORD: return t->v.sval != NULL;   /* ⭐⭐⭐ CN-12 -- a `&Name` leaf is TOTAL at emission (the new pat_node TT_KEYWORD arm: substitute the declared tree / fold the declared scalar / PB-1s snapshot-defer as last resort), so admission is unconditional; cycles are the EMITTER's job (sno_kw_chase) and bottom out in the dynamic defer, which is correct recursion semantics, never a bomb. */
    case TT_DEFER: return t->n > 0 && t->c[0] != NULL;   /* ⭐ CN-12 -- a defer INSIDE an inlined constant (`&Gray = *&White | epsilon`) emits the ordinary IR_MATCH_DEFER the statement regime carries everywhere; the deferee gets its own CN-3 treatment (inline/pat_static/seal) recursively at that site. */
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX: return (t->n > 0) && sno_cset_fold(t->c[0]) != NULL;
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS: return (t->n > 0) && t->c[0] && t->c[0]->t == TT_ILIT;
    case TT_SEQ: case TT_CAT: case TT_ALT: { for (int i = 0; i < t->n; i++) if (!sno_pat_inline_ok(t->c[i])) return 0; return 1; }
    case TT_ARBNO: { static int _ia = -1; if (_ia < 0) { const char * e = getenv("SCRIP_PAT_INLINE_ARBNO"); _ia = (!e || *e != '0') ? 1 : 0; } return _ia && t->n > 0 && sno_pat_inline_ok(t->c[0]); }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PT-2 DEAD-BUILD ELISION DELETED (Lon 2026-08-21 in-chat, HQ s177: "remove the eliding"): sno_fz_is_dead_build/sno_fz_procname_is_dead suppressed the MKPAT chain, the GVA store, and the proc_PAT
 * graph whenever a census (defer_cnt + snapref + encl) read zero consumers -- and the census was structurally blind to *X consumers inside kept patprocs (mark_defer's in_patproc guard) and inside
 * deferred expressions (MKEXPR fragments lower the read as a plain GVA-cell VAR, no IR_MATCH_DEFER ever minted), so every hole was a silent null-string zero-width match at the defer site (the whole
 * s175/s176 pass-thru wall census: pt0_any_before_seam and family).  Every stored pattern now ALWAYS builds and stores; a truly-unused build costs dead bytes, never correctness -- the RESULT-law 3a
 * stabilization trade.  The snapref set (the s6 partial patch for this same class) died with the verdicts it fed. */
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
        if (cs) { IR_LIT(nd).sval = (char *) cs; return nd; }
        /* MODE34-5b FIX: a bare-variable charset argument (SPAN(WS), not SPAN('literal')) cannot fold
         * to a compile-time constant, so it fell to sno_pre_req's pre-chain operand slot -- read at
         * match time via the legacy op_sa/FRQ(off+8) accessor, which is depth-blind to the enclosing
         * MATCH_BEGIN/MATCH_ASSIGN_SAVE nesting (gdb-measured: 240B off the real DESCR, see rt_pat_prim_str
         * in rt.c).  For the plain-TT_VAR case only, mirror LEN(*var)'s validated by-NAME fetch: stash
         * "*varname" in sval so the template calls rt_pat_prim_str at match time instead, never touching
         * op_sa/FRQ.  Anything else (arithmetic sub-expression, indirection, ...) keeps the pre-chain
         * path unchanged -- this fix targets only the shape 5b's repro and gdb trace confirmed broken. */
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
        if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }   /* CONST-AT-LOWER (Lon 2026-08-13): the folded value RIDES THE NODE (LEN's TT_ILIT shape) — no IR_LIT_INTEGER chain operand is minted, so no dead 16B cell is ever emitted or released.  n_operands==0 == folded; an operand present == supplied by an IR_LIT chain or pre-chain node. */
        if (t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_POS: case TT_RPOS: {
        IR_t * nd = lc_build(g, (t->t == TT_RPOS) ? IR_MATCH_RPOS : IR_MATCH_POS, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n <= 0 || !t->c[0]) sno_fatal("POS/RPOS requires a position argument", NULL);
        if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }   /* CONST-AT-LOWER (Lon 2026-08-13): twin of the TAB/RTAB arm above — value rides the node, n_operands==0 == folded.  This retires the n3_lit_integer dead-cell shape (roman.s RPOS(1)) and with it the CONST-WPOP orphan-release the emitter carried for it. */
        if (t->c[0]->t == TT_DEFER) { IR_t * argval = NULL; IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval); ir_operand_push(nd, argval); return arg_entry; }
        sno_pre_req(cx, t, nd);
        return nd;
    }
    case TT_FENCE:
        /* WHOLE-PATTERN / NESTED fence (not a spine element — TT_SEQ's splitter owns those).  ⭐ FENCE-NONSPINE (this rung): the old transparent lowering ("no IR_MATCH_FENCE1 box is needed") was FALSIFIED
         * by the oracle — the s131 "recorded question" answered.  A FENCE1 reached here (under `$`/`.`, as an ALT arm, as a whole pattern) still owes the interior cut: on ANY backup into it, alternatives
         * within P are invisible (manual p.222).  MEASURED: the manual's own example P = FENCE(BREAK(',') | REM) $ STR *DIFFER(STR) on subject ',' — SPITBOL FAILS (REM never retried on backup), the
         * transparent lowering MATCHED (T6 witness).  Build the same F box as the splitter: P lowered succ=F fail=F (interior fully live forward), inside-edge σ/φ retag, F.γ=succ, F.ω=fail (β-aware:
         * pass-through — the enclosing context's own seam/alternation machinery resumes leftward exactly as for any failing element).  FENCE0 alone (n==0) still matches null = succ. */
        if (t->n > 0 && t->c[0] && !sno_in_arbno && !g_sno_in_patproc) {   /* ⭐ FENCE-NONSPINE: gate on !sno_in_arbno AND !g_sno_in_patproc — PAT$ blob geometry predates cells-above-claim; a FENCE1 inside a blob must use the transparent path (old behavior).  Inside ARBNO bodies same Tier-D premise applies. */
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
        return (t->n > 0 && t->c[0]) ? sno_pat_node(cx, t->c[0], succ, fail) : succ;   /* inside ARBNO body or FENCE0: transparent lowering */
    case TT_DEFER: {
        const tree_t * in = (t->n > 0) ? t->c[0] : NULL;
        if (in && in->t == TT_VAR && in->v.sval) {
            { static int _p3 = -1; if (_p3 < 0) { const char * e = getenv("SCRIP_PAT_INLINE"); _p3 = (!e || *e != '0') ? 1 : 0; }   /* PT-3: *name-of-invariant inline — same killswitch as PT-1 */
              if (_p3 && !sno_encl_hostile(in->v.sval)) { const tree_t * p = sno_fz_tree(in->v.sval); if (p && sno_pat_inline_ok(p)) return sno_pat_node(cx, p, succ, fail); } }   /* inline before mark_defer so defer_cnt stays 0 */
            IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = (char *) in->v.sval; sno_fz_mark_defer(g, nd, in->v.sval); nd->seal = sno_defer_sealed(in->v.sval) ? 1 : (sno_seal_pat(in->v.sval) ? 2 : 0);   /* s142: 1 = full right-seal (s137 whack); 2 = WRITE-ONCE only (name eligibly resolves in g_sno_seal: single write, fz-safe) — enables the defer-site entry-cell, NOT the whack.  OP-SPLIT s21x-f: this is the `*X` arm, the only one the manual lets recurse (p.122) — IR_MATCH_DEFER is star-ONLY by construction now; the s199 dstar registration is deleted, the opcode IS the provenance. */ nd->pat_static = sno_name_static(in->v.sval);   /* ZD-5 s23i: a `*X` whose X is transitively defer-free cannot recurse -- the star buys late binding only, and the statement quartet may arm around it (117's *cmd class) */ sno_ω_to(nd, fail); return nd; }
        if (in && in->t == TT_KEYWORD && in->v.sval) { static int _cn = -1; if (_cn < 0) { const char * e = getenv("SCRIP_CONST"); _cn = (e && *e == '0') ? 0 : 1; }   /* ⭐ SN4-CONSTANTS CN-2 (s145, default ON; =0 restores the EXPR$ thunk arm = the measured Error-22 dead end): *&Name in pattern position is a DEFERRED CONSTANT READ -- same IR_MATCH_DEFER by-name machinery as *X (manual p.122), NV-keyed "&Name" to match the keyword read/write canonicalization in keywords.c.  seal 0 (plain resume record) and pat_static 0 are the conservative MVP; CN-3 upgrades declared constants to static staging. */
          if (_cn) { char cb[130]; snprintf(cb, sizeof cb, "&%s", in->v.sval[0] == '&' ? in->v.sval + 1 : in->v.sval);
            { static int _ci = -1; if (_ci < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); _ci = (e && *e == '0') ? 0 : 1; }   /* ⭐⭐⭐ SN4-CONSTANTS T2 — THE INLINE GRAPH (Lon's ruling 2026-08-19 in-chat, verbatim in substance: "I love the INLINE graph idea. Make it happen. Just like SNOBOL4 does. It builds patterns at runtime. We will build them at compile time."): a `*&Name` whose declaration-backed tree passes the SAME shape gate PT-3 uses for invariant variables lowers the TREE ITSELF at the defer site — compile-time pattern build, no IR_MATCH_DEFER, no NV read, no resume record.  PT-3 is the exact precedent (Lon's PAT-INLINE directive 2026-08-09, guard-free); a declared constant is STRONGER than an inferred invariant (cell sealed, error 341).  342-at-inlined-sites follows the PT-3 precedent — no runtime guard; 342 stays live at every non-inlined site; disposition recorded as the T2 RULING in GOAL-SNOBOL4-100.  Recursive grammar constants SELF-REFUSE: sno_pat_inline_ok admits no TT_DEFER, so trees carrying `*&X`/`*Y` fall through to the defer arm — leaves inline, spines defer, by construction.  SCRIP_CONST_INLINE=0 restores CN-3 byte-identically. */
              if (_ci) { const tree_t * cp0 = sno_const_pat(cb); if (cp0 && g_sno_pat_match_ctx && !g_sno_in_patproc && sno_kw_nest_ok(cb) && sno_pat_inline_ok(cp0)) { char * ky0 = lp_strdup(cb); if (sno_kw_chase(ky0, 1)) { IR_t * r0 = sno_pat_node(cx, cp0, succ, fail); sno_kw_chase(NULL, 2); return r0; } } } }   /* ⭐ CN-12: the guard became LOAD-BEARING the moment inline_ok started admitting KEYWORD/DEFER interiors -- `*&R` with `&R = 'q' *&R | 'z'` substitutes one level, meets itself on the chase stack at the nested site, and emits the defer that IS the recursion (before CN-12 this site could not recurse because inline_ok refused any tree containing a defer, which is also why beauty's 149 constant-defer sites never inlined). */
            IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = lp_strdup(cb);
            { const tree_t * cp = sno_const_pat(cb); if (cp) nd->pat_static = sno_pat_dfree(cp, 1, 0); if (cp && !g_sno_fz_unsafe) nd->seal = 2; }   /* ⭐⭐⭐ CN-3 pat_static + ⭐⭐⭐ CN-5 seal=2 -- see s148 FINDING for full accounting of both. */
            sno_ω_to(nd, fail); return nd; } }
        { const char * bn = sno_expr_collect(in); char pb[40]; snprintf(pb, sizeof pb, "*%s", bn);
          IR_t * nd = lc_build(g, IR_MATCH_DEFER, succ, NULL); IR_LIT(nd).sval = lp_strdup(pb); sno_ω_to(nd, fail); return nd; }
    }
    case TT_KEYWORD: {   /* ⭐⭐⭐ CN-12 / T2b -- BARE `&Name` IN PATTERN POSITION, TOTAL (Lon's order, s161-2: "there are no unsupported kinds; you are not allowed to refuse a single one"). Before this arm a bare keyword in pattern position fell to the default sno_fatal -- a COMPILE BOMB, so totality here can only add passes. The ladder, best case first: (1) declared SCALAR -- a QLIT folds to the literal-match box (T1's worldview in pattern position; ILIT/FLIT deliberately take the snapshot road instead, where sx_lower's T1 arm folds them and the runtime's own numeric-to-string coercion happens at build, not via a hand-rolled itoa here); (2) declared PATTERN -- substitute the staged tree COMPILE-TIME (the T2b inline the s151 cursor priced beauty's payoff on), guarded by sno_kw_chase so a recursive constant inlines one level and recurses dynamically below it; (3) EVERYTHING ELSE -- tier-2 keywords, forward refs, cyclic tails, unresolvable names -- the PB-1s snapshot-defer, the exact TT_VAR mechanism one arm below: the pre-chain sx_lower's the KEYWORD TREE itself (T1 folds declared scalars there; the runtime read answers the rest, with 342/251 semantics intact at BUILD time) into a PATV$ hidden global born pat_static=1. No refusal, no bomb; killswitch story inherited -- (1)/(2) die with STATIC/T1/INLINE off, (3) is reachable only where the old code bombed. */
        if (!t->v.sval) return succ;
        char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
        { const tree_t * cv = sno_const_val(cb); if (cv && cv->t == TT_QLIT) return sno_pat_node(cx, cv, succ, fail); }
        { static int _ck = -1; if (_ck < 0) { const char * e = getenv("SCRIP_CONST_INLINE"); _ck = (e && *e == '0') ? 0 : 1; }
          if (_ck) { const tree_t * cp = sno_const_pat(cb); if (cp && g_sno_pat_match_ctx && !g_sno_in_patproc && sno_kw_nest_ok(cb) && sno_pat_inline_ok(cp)) { char * ky = lp_strdup(cb); if (sno_kw_chase(ky, 1)) { IR_t * r = sno_pat_node(cx, cp, succ, fail); sno_kw_chase(NULL, 2); return r; } } } }
        IR_t * mv = lc_build(g, IR_MATCH_DEFER, succ, NULL); sno_ω_to(mv, fail);
        if (cx->npre >= 0 && cx->npre < 64) { cx->pre[cx->npre].arg = t; cx->pre[cx->npre].prim = mv; cx->pre[cx->npre].str = 0; cx->pre[cx->npre].codes = 0; cx->pre[cx->npre].snapg = lp_strdup(cb); cx->npre++; }
        return mv;
    }
    case TT_VAR: {                                                 /* SN4-BAREKW: the REM/ARB/FENCE strcmp bandages that lived here are now in sno_pat_eff_kind() */
        /* PB-5 (Lon 5-stage ruling 2026-08-07, deletion 2026-08-07): plain ref in pattern position is
         * VARIANT — evaluated ONCE at stage-2 (pattern build) via pre-chain PATV$ snapshot; IR_MATCH_DEFER
         * on the frozen global reads it at match time.  IR_MATCH_PATREF DELETED (PB-5): by-name per-anchor fetch
         * was neither variant (snapshot) nor invariant (compile-time); invalid under Lon's 5-stage model.
         * *X (TT_DEFER above) stays IR_MATCH_DEFER: sole recursion form (manual p.122).
         * PB-1s SNAPSHOT (Lon 5-stage ruling 2026-08-07): plain ref in pattern position emits IR_MATCH_DEFER on a hidden
         * PATV$k global frozen by a stage-2 VAR→ASSIGN pre-chain in cx->pre[].  Anchor retries and mid-match $-assignments
         * see the frozen stage-2 value (manual pp.87-88: 'A' $ X X uses the BUILD snapshot for the second X).
         * IR_MATCH_DEFER owns the NV acquisition path (GVA / rt_defer_get_pat_fn) and is correct for by-name global reads.
         * SCRIP_PB_SNAP killswitch deleted (PB-5): the live-name VAR→MATCH_VALUE fallback is the PATREF-era incorrect path. */
        { static int _pi = -1; if (_pi < 0) { const char * e = getenv("SCRIP_PAT_INLINE"); _pi = (!e || *e != '0') ? 1 : 0; }   /* ⭐ PAT-INLINE (Lon directive 2026-08-09): a bare ref whose name is TOTALLY INVARIANT lowers the STORED TREE INLINE into the statement spine — the reference IS pass-thru glue (pure wiring, zero nodes of its own), and the DEFER→PAT$-blob linkage (BLOB-GRANT whole-graph ___ frame, CLASS D suspend protocol) never exists for it.  The elements ride the statement regime — the licensed frame census {STATEMENT·FUNCTION·MATCH_BEGIN·FENCE1} and the modern per-box mechanism — byte-for-byte as if written inline.  SEMANTICS: sound exactly because invariance proves the PB-5 stage-2 snapshot equals the compile-time tree at every execution (single write, fz-safe, constant args) — the snapshot pre-chain is not skipped, it is PROVEN REDUNDANT.  GUARDS: !g_sno_in_patproc (blob-interior lowering; also breaks lower-time cycles); shape gate sno_pat_inline_ok (ARBNO/FENCE/captures excluded — their statement rungs delete the exclusion when they land); inline cannot fire in the PAT-ARG-BIND scratch walk or patproc build walk (g_sno_pat_match_ctx=0 / g_sno_in_patproc=1 respectively, both confirmed by code read and gate sweep).  `*X` (TT_DEFER arm above) is UNTOUCHED — sole recursion form, manual p.122.  ✅ DEFAULT ON (PT-1 exit gate 2026-08-09): w_pinline1/hand/full hit=A both modes; claws5-match m3 legacy==inline, xc318 120/122 same (2 pre-existing ASLR/ARBNO flakes, neither inline-caused); killswitch SCRIP_PAT_INLINE=0 reverts; corpus/probe/pt_inline_1{,_hand,_full}.sno promoted. */
          if (_pi && !sno_encl_hostile(t->v.sval)) { const tree_t * p = sno_fz_tree(t->v.sval); if (p && sno_pat_inline_ok(p)) return sno_pat_node(cx, p, succ, fail); } }   /* shape gate = sno_pat_inline_ok, exclusions + deletion conditions documented there */
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
        if (t->n <= 0 || !t->c[0]) sno_fatal("LEN requires a count argument", NULL);
        if (t->c[0]->t == TT_ILIT) { IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
        if (t->c[0]->t == TT_DEFER && t->c[0]->n > 0 && t->c[0]->c[0] && t->c[0]->c[0]->v.sval) {
            /* D08 FIX (s22 CLIMB): LEN(*var) deferred-integer — fetch var at MATCH TIME, not pre-chain.
             * pre-chain coerce runs before MATCH_BEGIN; under ζ-cells-on-RSP the coerce result lives on
             * the FORTH spine, whose depth at LEN read time differs from coerce-write time by exactly the
             * intervening sub-rsp carves (MATCH_BEGIN +32, SAVE +16, SPAN +16, …). FRQ(slot+8) reads the
             * wrong address. Fix: store "*varname" in sval; template calls rt_pat_prim_int at match time. */
            const tree_t * inner = t->c[0]->c[0];
            const char * vn = inner->v.sval;
            char pb[128]; snprintf(pb, sizeof pb, "*%s", vn ? vn : "");
            IR_LIT(nd).sval = lp_strdup(pb);
            return nd;
        }
        sno_pre_req(cx, t, nd);                                    /* TT_DEFER (*var or *(arith)): sno_pre_req unwraps one * level (line 1006) → runtime integer fetch; SNO$MKEXPR route (RTX 55c045eb) wrongly built a PAT DESCR instead */
        return nd;
    }
    case TT_CAPT_COND_ASGN: {
        const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
        if (vn) sno_reg_var(vn);
        if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) { const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL; if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }   /* SN4-CAP-NAME-STRICT half A: `pat . *VAR` ≡ `pat . VAR` — the name of a deferred variable IS that variable (oracle p14/p15: `Q = 'q0'; 'A' . *Q` leaves q0 null and sets Q=A; the pre-s170 "*Q" spelling reached the pump's indirect arm and assigned q0 instead). */
        if (!vn) { const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : (di && di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect(di); char pb[48]; snprintf(pb, sizeof pb, "*%s", bn); vn = lp_strdup(pb); } }
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
             * the inner still refuses both registrations and the capture keeps today's flat array path
             * verbatim (degrade never die).  Inside an ARBNO body the registration DEFERS to the statement
             * candidacy site (sno_cap_fc): the cell is sound only on the ELEMENT path (Lon's static-size
             * ruling -- the capture is just another predetermined 16 in the sum there), while a refused/
             * anchored statement keeps the flat array (rsp still moves per iteration on that machinery). */
            sno_cap_fc(g, nd, save, before_i);
        }
        { extern void fc_pair_extent_register(const IR_t *, int); fc_pair_extent_register(nd, g->n); }   /* FLATDISP-LEAF-ORDER: inner allocation ends here; fc_leaf_walk registers COND at the depth AFTER this range */
        return save;                                               /* capture entry is the SAVE node */
    }
    case TT_CAPT_IMMED_ASGN: {
        const char * vn = (t->n > 1) ? sno_capt_name(t->c[1]) : NULL;
        if (vn) sno_reg_var(vn);
        if (!vn && t->n > 1 && t->c[1] && t->c[1]->t == TT_DEFER) { const tree_t * di = (t->c[1]->n > 0) ? t->c[1]->c[0] : NULL; if (sno_cap_name_strict() && di && di->t == TT_VAR && di->v.sval && di->v.sval[0]) { vn = lp_strdup(di->v.sval); sno_reg_var(vn); }   /* SN4-CAP-NAME-STRICT half A: `pat . *VAR` ≡ `pat . VAR` — the name of a deferred variable IS that variable (oracle p14/p15: `Q = 'q0'; 'A' . *Q` leaves q0 null and sets Q=A; the pre-s170 "*Q" spelling reached the pump's indirect arm and assigned q0 instead). */
        if (!vn) { const char * bn = (di && di->t == TT_FNC && di->v.sval && di->n == 0) ? di->v.sval : (di && di->t == TT_FNC && di->n > 0) ? sno_expr_collect_wn(di) : sno_expr_collect(di); char pb[48]; snprintf(pb, sizeof pb, "*%s", bn); vn = lp_strdup(pb); } }
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
        int first_fence = ne; int first_f0 = ne;
        for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { first_fence = i; break; }
        for (int i = 0; i < ne; i++) if (sno_is_fence0(elems[i])) { first_f0 = i; break; }   /* ⭐ FENCE-PASS-THROUGH (this rung): the pat_seal abort cut belongs to FENCE0 ALONE (manual p.203: backup through &FENCE aborts the attempt).  FENCE1 (manual p.222, verbatim): "Pattern backup will always pass through FENCE().  Note that backup through FENCE() does not cause the match to abort" — so runs right of a FENCE1 exhaust into the F box (pass-through), never to pat_seal, and the leftward seam chain carries on.  first_f0 replaces first_fence in BOTH fail-target formulas below; first_fence keeps only the splitter early-out.  ORACLE WITNESSES: H01 (=S vs =F at HEAD, parked in XFAIL.run) and the T4/T5 probes ('AA'|'A' FENCE('') 'AB' on AAB · 'XA'|'X' FENCE('A') 'Y' on XAY): SPITBOL matches both, HEAD failed both. */
        if (first_fence == ne)
            return ne == 1 ? sno_pat_node(cx, elems[0], succ, fail) : sno_seq_nary(cx, elems, ne, succ, fail, NULL);
        IR_t * cur_succ = succ; IR_t * right_tail = NULL; int right_tail_idx = -1; int right_sealed = 0;
        for (int i = ne - 1; i >= 0; ) {
            if (sno_is_fence(elems[i])) {                                           /* FENCE0 seals everything to its right (abort-on-backup); FENCE1 does NOT — it is a normal seam element whose box β IS the pass-through (⭐ FENCE-PASS-THROUGH, this rung; the old umbrella "each seals its right" applied FENCE0's abort to FENCE1 and was falsified by the oracle: H01/T4/T5) */
                const tree_t * inner = sno_is_fence1(elems[i]) ? elems[i]->c[0] : NULL;   /* inner != NULL ⇔ FENCE1; FENCE0 stays node-free (pure rewiring — its box body would be α→γ, β→ω) */
                if (!inner || sno_in_arbno) right_sealed = 1;                      /* ⭐ FENCE-PASS-THROUGH: FENCE0 always seals; FENCE1 seals inside an ARBNO body (sno_in_arbno>0): the pass-through seam repoint rests on a static-depth rsp premise that ARBNO violates (rsp moves per iteration, Tier D).  Outside ARBNO bodies FENCE1 participates in the standard leftward seam chain. */
                if (inner && sno_in_arbno) {                                        /* FENCE1 inside ARBNO body: ival=2 = watermark+P but NO U-2 ___ frame (ARBNO owns ___; nested push/pop corrupts its frame) */
                    IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;
                    int f_idx = g->n;
                    IR_t * F = lc_build(g, IR_MATCH_FENCE1, cur_succ, NULL);
                    sno_ω_to(F, fail_p);
                    IR_LIT(F).ival = 2;   /* ⭐ ival=2: FENCE1-in-ARBNO — suppresses fence_u2_frame() in template */
                    int before_p = g->n;
                    IR_t * pe = sno_pat_node(cx, inner, F, F);
                    IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
                    for (int q = before_p; q < g->n; q++) { IR_t * x = g->all[q];
                        if (!x) continue;
                        if (x->ω.node == F) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
                        if (x->γ.node == F) { if (x->op == IR_GOTO && x->ω.node == F) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; } }
                    ir_operand_push(F, pe);
                    ir_operand_push(F, p_tail);
                    /* NO sno_resume_ω_to: right_sealed=1 prevents the seam repoint (Tier-D premise) */
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx;
                }
                else if (inner && !sno_in_arbno) {                                  /* FENCE1 = FENCE(P) outside an ARBNO body: lower P with the pre-seal fail so P retries normally on forward-fail; the seal blocks re-entry after success */
                    /* SYNC-POINT ζ RELEASE (Lon ruling s132, sync point 2 — FENCE(P) success exit).  The old wiring was PURE EDGE
                     * REWIRING: P succeeded straight into cur_succ and every ζ cell P's boxes retained (uniform-β) sat on the stack
                     * until the match bracket died, even though the seal makes them unreachable the instant P commits (its
                     * alternatives are invisible backing up — manual ln 4716).  Measured: json-match.sno's FENCE-per-token ws eats
                     * >32MB ≤64MB of ζ on a 632KB subject (SCRIP_STACK ladder, s132/this rung).  Now the fence is ONE
                     * IR_MATCH_FENCE1 box in the ALT/SEQ σ/φ-glue mold: F.α records the watermark (rsp under FORTH; the zls2 cursor
                     * under the heap ports) into its granted [___+off] quad — depth-immune, per-activation, so DEFER recursion
                     * through the same fence is safe — and jmps P's entry; P is lowered succ=F fail=F with the standard inside-edge
                     * retag, so P's commit lands F's na_s glue (bulk-restore to the watermark, jmp F.γ = the old cur_succ) and P's
                     * leftward exhaust lands na_f (same restore — the identity by LIFO — then F.ω = the old fail_p).  Resume-from-
                     * the-right stays STRUCTURALLY absent (right_sealed skips the repoint below, exactly as before); F.β falls into
                     * na_f as the ARBNO-seal "resume ≡ abandon" precedent demands should any future wiring reach it. */
                    IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;           /* ⭐ FENCE-PASS-THROUGH: F.ω routes past FENCE0s only — pat_seal iff a FENCE0 lies to F's left; otherwise the spine fail (HEAD).  The generic seam below then repoints this ω at the left run's tail-β (generator resume) exactly as for any element, restoring the manual's "elements LEFT of the fence stay live". */
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
                    if (right_tail && !right_sealed) sno_resume_ω_to(g, right_tail_idx, right_tail, F);   /* ⭐ FENCE-PASS-THROUGH: the right run's leftward exhaust lands F's β; only reached when !sno_in_arbno (the arbno gate above ensured right_sealed=1 inside bodies). */
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx; right_sealed = 0;   /* F is now an ordinary left-neighbor; next (left) run repoints F.ω at its tail-β */
                }
                else if (i > 0) {                                                   /* s137 OVER-SEAL (Lon ruling): an INTERIOR FENCE0 gets the operand-free sync box (ival=0) — its α IS the forward
                                                                                     * commit (match null), and the box body is now non-empty: whack the activation's dynamic ζ to the ___ floor
                                                                                     * (bb_match_fence1.cpp ival=0 arm), then γ.  Wiring is the s133 erasure's, verbatim: ω → the pre-seal fail
                                                                                     * (backup ≡ attempt abort), right_sealed already set above so the left run gets no resume repoint and the box
                                                                                     * gets no inbound β.  FIRST-POSITION FENCE0 (i==0, the anchor idiom) keeps the node-free erasure: zero left
                                                                                     * context in this spine, nothing to whack, the s133 reasoning stands there. */
                    IR_t * fail_p = (i > first_f0) ? cx->pat_seal : fail;   /* FENCE-PASS-THROUGH: FENCE0-relative */
                    int f_idx = g->n;
                    IR_t * F = lc_build(g, IR_MATCH_FENCE0, cur_succ, NULL);   /* ⭐ FENCE-SPLIT: FENCE0 is NOT a FENCE1 with ival=0 -- the two constructs have OPPOSITE beta semantics (manual Ch.19: backup through FENCE0 ABORTS the attempt; backup through FENCE1 PASSES THROUGH and does not abort), so they carry separate kinds.  ival stays 0 for the legacy readers that still consult it. */
                    sno_ω_to(F, fail_p);
                    IR_LIT(F).ival = 0;
                    cur_succ = F; right_tail = F; right_tail_idx = f_idx;
                }
                i--;
                continue;
            }
            int j = i; while (j > 0 && !sno_is_fence(elems[j - 1])) j--;             /* the maximal fence-free run [j..i]; a run never spans a fence so one fail target serves it */
            int rn = i - j + 1;
            IR_t * fail_r = (j > first_f0) ? cx->pat_seal : fail;                 /* right of the FENCE0: cut to the SEAL target (== statement-fail at top level, == F/exhaust inside an ARBNO body), never HEAD.  ⭐ FENCE-PASS-THROUGH: FENCE0-relative (first_f0) — a run whose nearest left fence is a FENCE1 keeps the plain fail and its tail is seam-repointed at that F box (pass-through), per the manual p.222. */
            int before_r = g->n;
            IR_t * n_rt = NULL; IR_t * re = (rn == 1) ? sno_pat_node(cx, elems[j], cur_succ, fail_r) : sno_seq_nary(cx, elems + j, rn, cur_succ, fail_r, &n_rt);
            int _rb2 = before_r; while (_rb2 < g->n && g->all[_rb2] && g->all[_rb2]->op == IR_GOTO && g->all[_rb2]->n_operands == 0) _rb2++;   /* ⭐ FENCE-PASS-THROUGH: a multi-element run lowers via sno_seq_nary whose FIRST-ALLOCATED node is its sentinel GOTO (SEQ-ERAD SE-6's own warning, applied here) — the seam repoint must land the first REAL resume-bearing node, not the relay.  Pre-rung this r_tail was never consumed across a fence (right_sealed skipped every such seam), so the sentinel landing was latent; the F.ω left-resume is its first consumer (MEASURED: H01 exited silently with zero output when F.ω β-landed the sentinel). */
            IR_t * r_tail = n_rt ? n_rt : ((_rb2 < g->n) ? g->all[_rb2] : re);   /* ⭐ FENCE-PASS-THROUGH: a multi-element run's right-side resume surface is its LAST element's resume carrier (n_rt from sno_seq_nary), so a righter fence's ω resumes the rightmost generator; single-element runs keep the sentinel-skipped first-real node. */
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
             * extent skipped (the pre-lift blanket refuse is retired); an UNGRANTED nested ALT or anything unknown still refuses the whole outer ALT (stays flat, degrade never die). */
            if (!fc_walk_range(g, before, g->n, 0, &fp_i)) fc_linear = 0;
            if (i < 16) { fc_fp[i] = fp_i; fc_ab[i] = before; fc_ae[i] = g->n; }
            ir_operand_push(A, ei);
            ir_operand_push(A, ri);
        }
        if (fc_linear) { extern void fc_alt_register(const IR_t *, int, const int *, const int *, const int *); extern void fc_arm_member_register(const IR_t *); fc_alt_register(A, (int)na, fc_fp, fc_ab, fc_ae); for (int _j = 0; _j < (int)na; _j++) for (int _k = fc_ab[_j]; _k < fc_ae[_j] && _k < g->n; _k++) if (g->all[_k]) fc_arm_member_register(g->all[_k]); }   /* ALT-FLAT s202: arm residents go flat -- MUST run AFTER the admission walk above (fp math computed pre-refuse) */
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
        /* SN4-PAT-EAGER-CALL (2026-08-15 s106): the MATCH_VALUE arm below splices the call's evaluation chain INTO THE PATTERN GRAPH, so for a pattern that is BUILT (stored) rather
         * than matched in place, the call lands inside the PAT$ blob and runs at MATCH time.  The manual gives the opposite law: a bare call is evaluated when the pattern is
         * CONSTRUCTED -- p.86 "NPAT captures the value of variable N at the time of pattern construction"; p.134 "Without it, PUSH() is called when the pattern is first constructed".
         * Deferral is exactly what the unary * operator is FOR, and a bare call must not receive it.  Witnesses corpus/probe/mv/: mv_alt_call_build_order is the pure ordering
         * divergence with NO crash (top level, side-effecting callee); mv_alt_builtin_call_infn and mv_alt_call_infn_matched_infn are the SIG11 face of the same root inside a
         * DEFINE body.  FIX rides the EXISTING PB-1s stage-2 snapshot: register the whole call tree as a cx->pre[] entry, whose drain (~2005) already lowers cx->pre[].arg
         * GENERICALLY via sx_lower -- so the call is evaluated ONCE at the BUILD site into the hidden PATV$k global and the blob DEFERs a by-name read of the frozen result.
         * ONE mechanism, no second path, no new global (env read via function-static).  Killswitch SCRIP_PAT_EAGER_CALL=0 restores the MATCH_VALUE arm verbatim. */
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
    if (k == TT_KEYWORD) return t->v.sval != NULL;   /* ⭐⭐⭐ CN-12 -- THE MISSING THIRD ARM (CN-3c), the single line this file's own s148 notes named as "the ONLY blocker". Honest only NOW: pat_supported asserts THE LOWERER CAN EMIT THIS TREE, and the claim is true as of the pat_node TT_KEYWORD arm landed in the same commit (substitute / scalar-fold / PB-1s snapshot-defer -- total, no default-arm bomb reachable). This is what lets constant CHAINS register (`&Item = &Word | &Num`) and activates the two arms CN-3 left dormant-by-measurement (sno_pat_dfree and sno_is_pattern_rhs TT_KEYWORD chase). */
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
    case TT_VAR: { static int depth = 0; if (depth >= 32 || !t->v.sval) return 0; const tree_t * p = sno_seal_pat(t->v.sval); if (!p) return 0; depth++; int r = sno_is_pattern_rhs(p); depth--; return r; }   /* ___-EARN s45 DEFECT A: a bare name that eligibly resolves to a stored pattern IS a pattern-RHS -- without this arm, Q = P / Q = P P is never classified, sno_snapref_scan never runs, and P's build is dead-eliminated while Q's use-site reads null (earn0_varref_bare_dropped / _cat_dropped).  Same seal-table resolution + depth cap as sno_pat_right_sealed's own TT_VAR arm (A=B;B=A cycle guard); bare keywords never reach here (sno_pat_eff_kind remaps them first). */
    case TT_KEYWORD: { static int kdepth = 0; if (!sno_const_static_on() || kdepth >= 32 || !t->v.sval) return 0; char cb[130]; snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval); const tree_t * p = sno_const_pat(cb); if (!p) return 0; kdepth++; int r = sno_is_pattern_rhs(p); kdepth--; return r; }   /* ⭐⭐ SN4-CONSTANTS CN-3b: the `&`-namespace twin of the TT_VAR arm above, and the arm that makes CONSTANT CHAINS work -- `&Item = &Word | &Num` is a pattern RHS iff its constant members are, and without it the chain is never classified, so &Item never registers and `*&Item` falls back to the conservative pat_static=0 (measured before this arm: the chain witness stamped 1 node in BOTH killswitch arms).  ORDER IS THE CORRECTNESS CONDITION: sno_const_pat resolves against g_sno_seal, which the pre-scan fills IN STATEMENT ORDER, so a chain classifies only when its members are DEFINED EARLIER IN THE PROGRAM TEXT -- a forward reference stays conservative rather than wrong, which is the right failure direction.  Own depth counter (not the TT_VAR one) so a mixed `&A = B` / `B = &A` cycle is still capped.  Conservative on miss: a real keyword is never seal-noted (it cannot take a pattern RHS), so this returns 0 for `&ALPHABET` exactly as the pre-CN-3 default did.  ⛔ ALSO MEASURED INERT AT LANDING for the same single reason as the sno_pat_dfree arm above -- sno_pat_supported() gates registration and has no TT_KEYWORD arm, so no chain reaches either classifier.  The chain witness cn_const_chain stamps 1 node in BOTH killswitch arms today; when CN-3c lands the sno_pat_supported arm, THAT is the number that must move to 2, and it is the gate for the rung. */
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
/* PATSALT (s184): sno_expr_collect (line ~86) has carried a `salt == g_sno_expr_salt` guard on its memo since the EXPR$<n>F<salt> naming landed; sno_pat_collect below NEVER RECEIVED IT.  Without it a
 * stored row's `pat` is a DANGLING pointer into an EVAL fragment AST that ast_tree_free_dyn already released: the arena hands the 5th fragment the 1st fragment's addresses, and sno_expr_eq's `a == b`
 * short-circuit then declares two unrelated patterns identical without reading a single field.  Measured on probe/passthru/ptw_min_epsshift8: fragments 5..8 arrive at 0x43e180/0x43dd70/0x43e0b0/
 * 0x43f300 -- byte-for-byte the four pointers rows 0..3 hold -- so npat freezes at 4 and elements 5..8 are wired into elements 1..4's blobs.  Every static pattern is collected at salt 0, so the guard
 * is inert for a program with no EVAL and the static emission is byte-identical. */
static int sno_patsalt_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATSALT"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* SCRIP_PATSALT=0 = unguarded memo */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_pat_collect(const tree_t * pat) {
    for (int i = 0; i < g_sno_npat; i++) if ((!sno_patsalt_on() || g_sno_pats[i].salt == g_sno_expr_salt) && sno_expr_eq(g_sno_pats[i].pat, pat)) return g_sno_pats[i].name;
    if (g_sno_npat >= SNO_PAT_MAX) sno_fatal("too many stored patterns in one program", NULL);
    char buf[32]; snprintf(buf, sizeof buf, "PAT$%d", g_sno_npat);
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
    if (out_land) { land = lc_build(g, IR_GOTO, fJ, NULL); *out_land = land; }   /* R1 STMT-BETA-LAND (06e design, landed this session): fB -- a DEDICATED exhaust-only GOTO carrying ONLY MATCH_BEGIN's ω (anchor exhausted = whole-statement failure).  Untagged it chains fB→fJ→fT, byte-equivalent to the direct wire (zd_chase and the drive chase both thread GOTOs transparently); the zw5 post-loop in sno_build_graph retags fB.γ to STATEMENT_BEGIN with the β port tag, making statement_begin_beta the named failure landing per the STATEMENT-PORT LAWS (Lon 2026-08-06).  fJ is UNTOUCHED: element failures (the scanner retry loop, SPITBOL manual Ch.18 step 6) and every other statement-failure producer keep their edges -- the 06e session measured that retagging the SHARED fJ re-tags every edge chasing through it (3 regressions, two statements fell out of the walk on 067). */
    IR_t * head = lc_build(g, IR_MATCH_BEGIN, NULL, land);
    { IR_t * sealJ = lc_build(g, IR_GOTO, head, NULL); memcpy(sealJ->γ.sz, "φ", 3); sealJ->γ.sz[3] = 0; cx->pat_seal = sealJ; }   /* ⭐ SEAL-UNWIND (this rung): pat_seal re-routed THROUGH the head.  sealJ is a φ-tagged GOTO into IR_MATCH_BEGIN — the emitter's φ-chase resolves it to BEGIN's na_f = the L(1) anchor-exhaust unwind (CAS pop, rsp restore, PATCTX restore, claim release) whose ω tail is the statement-fail, anchor NOT advanced — exactly the abort semantics with the bookkeeping the raw-fJ route skipped (the zeta_storage head.sigma_save KNOWN BYPASS, now closed).  fJ itself is untouched: it stays BEGIN's own ω/land path, so no ω→na_f→ω cycle exists.  cx->pat_fail stays fJ.  EVAL/CODE and pattern-blob graphs keep their own tno/no seal targets (their unwind is the blob ω-glue).  WITNESSES: t1m (SEGV→0), t1x2 (silent→f1/f2), fence_probe T6 contamination. */
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
    IR_t * release = lc_build(g, IR_MATCH_END, sJ, sno_cap_name_strict() ? cx->pat_seal : NULL);   /* SN4-CAP-NAME-STRICT half B: the terminus gains the failure edge it never had.  A deferred capture target that resolves to a VALUE instead of a NAME is discovered in the commit-time pump (rt_dcap_pump), which today runs DOWNSTREAM of every decision point and can only warn — the measured "deferred-call failure not propagating as pattern retreat".  ω → cx->pat_seal, the SAME φ-tagged unwind TT_ABORT uses: BEGIN's na_f (CAS pop, rsp restore, PATCTX restore, claim release) whose ω tail is the statement-fail with the ANCHOR NOT ADVANCED.  Measured why it is not `head`: sbl calls the target EXACTLY ONCE and abandons the statement (`P = EVAL('LEN(0) . *F()'); 'abc' ? P` prints one "F called", then OK) -- a not-a-name target aborts the match, it does not retreat into a rescan.  Wiring ω to head instead re-entered the scanner with the outer Σ/δ/Δ un-restored and looped forever, leaking a frame per lap until the stack died (witness probe/eval/ev_beauty_shape, caught by the BY-SET corpus diff, never by the pass COUNTS). */
    if (has_repl) IR_LIT(release).dval = 1.0;
    ir_operand_push(release, head);
    int before_pat = g->n;
    sno_cap_defer_reset();   /* L1b: the deferred in-ARBNO capture list is per-statement — entries from a statement whose candidacy never consumed them (npre/REPLACE/multi-ARBNO early-outs) must not leak into the next statement's promotion scan */
    IR_t * pat_entry; { int _mcsv = g_sno_pat_match_ctx; g_sno_pat_match_ctx = 1; pat_entry = sno_pat_node(cx, ptt, release, head); g_sno_pat_match_ctx = _mcsv; }   /* PAT-INLINE: the ONE extent where bare-ref inline is licensed */
    lc_γ_to(head, pat_entry);
    {   /* ZB-FC-3d (partition ruling, s49) + ALT-LIFT (the s65 named follow-on, landed): the statement grant.  The shared fc_walk_range over the PATTERN range only ([before_pat, g->n) -- head/release/
         * splice/repl-chain allocate before it, pre-chains and the subject chain after, so the range is pattern-pure by construction).  fp_stmt = the fc_geom range sum PLUS 16+fpmax per granted
         * ALTERNATE (arm extents skipped -- only one padded arm is live at yield, the exact over-count the old blanket refuse existed to avoid).  UNGRANTED ALTs and unknowns still refuse; either-
         * direction failure keeps the flat path byte-verbatim (degrade never die). */
        int fp_stmt = 0; int fc_lin = (sno_in_arbno == 0) && fc_walk_range(g, before_pat, g->n, 1, &fp_stmt);
        if (fc_lin) { extern void fc_head_register(const IR_t *, int); fc_head_register(head, fp_stmt); }
        /* REG-7 U4 sweep (s86): the s66 ANCHOR-WINDOW refuse fallback is DELETED -- a !tail_ok statement now runs the same unconditional-___ frame as everything else (U1/U2/s79 seeds; FR/FRQ ___-based
         * post-U3; the ARBNO chain arm's zv() view decoupled s85).  Proof of deadness: the s85 flip probe -- crosscheck watermark-exact, .s byte-identical on 070/117/142/164/165 -- no live emission
         * consulted the window predicate.  Candidacy below is unchanged; the only refuse consequence left is the TAIL_DIAG print. */
        if (!fc_lin && rt_zc_frame_live() == ZC_FRAME_RSP) {
            /* R12-EXIT-1 CARRY-THE-TAIL candidacy (tried BEFORE the anchored fallback): exactly ONE spine ARBNO, no REPLACE, no runtime-arg pre-chain (cx->npre), no capture allocated left of the
             * ARBNO (a COND/SAVE preceding it may wrap it -- conservative refuse), left/body/right ranges walk clean, ARBNO body-bracket operands resolve.  A candidate registers with zeta_storage's
             * fct (geometry finalizes in the layout pass where zls offsets exist) and its spine SEQ converts to the static-wiring grant (an ungranted SEQ's runtime seq_i is a FLAT slot read at every
             * seam = dynamic depth once elements exist -- the exact class this rung deletes).  Anything short of the full test refuses to the anchored window verbatim (degrade never die). */
            int tail_ok = 0;
            const char * tl_why = (cx->npre != 0) ? "npre" : has_repl ? "repl" : "gate";   /* SCRIP_TAIL_DIAG: first-failing-gate name, printed at the refuse seam below (the OMEGA-DIAG precedent) */
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
                         * conservative refuse (tl_why "cap-left"). Cap 4 wrap pairs (element-bloat bound; overflow refuses). */
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
                         * unpromotable residue, walk-failed inner, or a right-range capture that never granted — forces the statement to REFUSE to the anchored window: the flat rt_cap slot is a
                         * PERSISTENT {buf,gen,sp} struct (pattern_match.c) and cannot live in transient element memory (the 163 finding — garbage nonzero buf in fresh element bytes reads/writes wild).
                         * Promotion is applied ONLY after every gate passes: a refused statement must keep every capture ungranted (the anchored ARBNO machinery still moves rsp per iteration —
                         * the sno_in_arbno gate's original and still-valid premise there). */
                        int cap_bad = 0;
                        char promo[64]; for (int d = 0; d < scd_n; d++) promo[d] = 0;
                        { extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_extent(const IR_t *, int *, int *); extern int fc_cond_fp(const IR_t *);
                          for (int d = 0; d < scd_n && !cap_bad; d++) {
                              if (scd[d].save_idx < i_b0 || scd[d].nd_idx >= g->n) { cap_bad = 1; break; }   /* a deferred entry outside the element ranges has no sound home on either path — refuse */
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
                        int dfr_regs = 0;   /* PS-3 s153: a DEFER-bearing candidate can REFUSE AT FINALIZE (target unregistered/non-uniform -- knowable only once the emit_patzeta registry is fed), and
                                             * a finalize-refuse must strand NOTHING: the promo/wrap fc_save/fc_cond registrations below grant 16B cells at LOWER assuming the tail path, and a
                                             * refused-to-chain statement with granted captures is the 163-mine class verbatim.  So defer+captures combos refuse HERE (degrade never die; slice 2's
                                             * record-peek can widen).  The pat_entry SEQ registration is the one lower-side effect a finalize-refuse CAN reach -- zeta_storage's refuse path
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
            if (!tail_ok && getenv("SCRIP_TAIL_DIAG")) fprintf(stderr, "[TAIL-DIAG] refuse: %s\n", tl_why);
        }
        /* R12-ERAD s65 + ALT-LIFT: per-leaf flat displacement for ZC_FRAME_RSP via fc_leaf_walk (zeta_storage.c registrar).  Same range, same order (allocation = flow on the linear spine), prefix
         * starts at 32 = HEAD's self-pushed cell; each pattern node's body depth = prefix-before + own granted cell.  Granted ALT arms restart at prefix+16 (alternatives, not concatenation); nodes
         * after a granted ALT see prefix+16+fpmax (the pad stubs' uniform yield depth).  Registered only under the statement grant: a refused statement has no static depth (that is WHY it refused)
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
        if (cx->pre[pi].snapg) {   /* PB-1s: stage-2 snapshot -- VAR(live name) -> ASSIGN(hidden global) in the pre-chain; the IR_MATCH_DEFER (prim) fetches the frozen global at scan time by name */
            static int g_snapctr = 0; char nb[32]; snprintf(nb, sizeof nb, "PATV$%d", g_snapctr++);
            char * gname = lp_strdup(nb); sno_reg_var(gname);
            IR_LIT(cx->pre[pi].prim).sval = gname;   /* wire DEFER to the hidden global */
            cx->pre[pi].prim->pat_static = 1;   /* PB-1s STATIC STAMP (W-2a, FINDING-2026-08-07e): PATV$ hidden globals are write-once-frozen by construction (one ASSIGN in the pre-chain, runs once at statement entry, chain frozen before MATCH_BEGIN); the zdyn veto at emit.cpp:2014 reads pat_static=0 and falsely classifies them as dynamic, refusing the quartet.  sno_name_static() cannot reach PATV$ names (they are not in the AST seal table); the PATV$ prefix IS the proof -- stamp directly.  Killswitch: SCRIP_ZD_DYNARM=0 still forces st=0 and overrides. */
            sno_fz_mark_defer(g, cx->pre[pi].prim, gname);   /* DEFER FZ registration so zeta_storage sees it */
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
/*--- SN4 kill-the-O(n^2) FINAL SHAPE (Lon: "DEFINE is a single statement"): an entry-in-main DEFINE needs a callable
 *--- proc with its OWN exitnd (correct RETURN via the runtime activation record) but NO re-lowered body — the body
 *--- statements already live in the ONE main graph, emitted once.  So mint a TINY 1-node ENTRY-STUB graph: the α is an
 *--- IR_GOTO_DEFERRED to the entry LABEL NAME (rt_goto_transfer resolves name -> the body's already-emitted address at
 *--- runtime, the same wire CODE fragments use to jump back into the body), and the stub carries its own exitnd/failnd
 *--- so RETURN/FRETURN unwind to THIS proc's epilogue, not the program exit.  emit_chain on the 1-node stub is cheap
 *--- and collision-free (no shared node re-walked).  O(1) per DEFINE. ---*/
static IR_graph_t * sno_build_call_stub(const char * entry_label, const char * fname) {
    IR_graph_t * g = IR_alloc(64);
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    /* TINY-REAL s58 (Lon): the per-DEFINE role-4 SHIM (<fn>_alpha swap/extend, <fn>_gamma/<fn>_omega restore) rides THIS chain, γ-linked after the fold box — MEASURED: the emission walker is
     * reachability-based (a port-orphan node minted into the main graph emitted 0 definitions against 6 site references), and the fold box ends in an unconditional `jmp rax`, so the shim is emitted
     * yet control-dead inline; sites enter it by NAME only.  Placement at the DEFINE statement's comment (Lon) rides the stub-suppression rung — this lands the label the sites need first. */
    IR_t * sh4 = fname ? lc_build(g, IR_DEFINE, exitnd, failnd) : (IR_t *)0;
    if (sh4) { IR_LIT(sh4).ival = 4;
        IR_t * s41 = lc_build(g, IR_LIT_STRING, NULL, NULL); IR_LIT(s41).sval = (char *) fname;
        IR_t * s42 = lc_build(g, IR_LIT_STRING, NULL, NULL); IR_LIT(s42).sval = (char *) entry_label;
        ir_operand_push(sh4, s41); ir_operand_push(sh4, s42); }
    IR_t * gd = lc_build(g, IR_GOTO_DEFERRED, sh4 ? sh4 : exitnd, failnd);
    IR_LIT(gd).sval = lp_strdup(entry_label);
    gd->seal = 1;   /* ⭐ DEFINE-FOLD (s53): fold-eligible mark -- this goto's target is the CONSTANT entry label of a DEFINE, whose LBL__ body proc exists in this same compilation.  bb_goto_deferred's fold arm reads it (staged as op_ival at the dispatch) and emits the DIRECT transfer `lea rdi,[rip+proc_LBL__<name>_α]; call rt_chain_enter`, deleting the per-call rt_goto_transfer string lookup while preserving the chain protocol (callee-save pushes + fall-off landing) verbatim.  seal is free on this kind (no other consumer). */
    g->entry = gd;   /* ROLE-3 DELETE (Lon s114 in-chat: "remove the IR_DEFINE since it DOES NOTHING... The FUNCTION wiring is already done via the SHIM, so this must be some OLD code that never materialized. Clean it up."): the WIRE-ADOPT box has been an empty label + relay since s54/s55 moved wire seating to the sites (r10/r11) — the stub graph now ENTERS at the IR_GOTO_DEFERRED itself; proc_role3_kind reads that entry shape as kind 1.  EXPR$/PAT$ thunks (kind 2) keep their role-3 entry: it is their kind discriminator, and their registration/startup path is unchanged. */
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * g_sno_prescan_top       = NULL;
static int            g_sno_expr_define_seen  = 0;
static IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name) {
    IR_graph_t * g = IR_alloc(nst * 16 + 256);
    scx_t cx; cx.g = g; cx.loop_exit = NULL; cx.loop_next = NULL; cx.result_name = result_name; cx.pat_fail = NULL; cx.pat_seal = NULL; cx.npre = 0;
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t ** anchor = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    IR_t ** fail_tgt = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));   /* STMT-BETA (cc39c095, Lon ruling 2026-08-06): parallel array saving each statement's fT so the post-loop STATEMENT_BEGIN wiring pass can set its omega port to fT, making the emitter wire statement_begin_beta -> fT as the named failure landing */
    IR_t ** match_land = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    IR_t ** asgn_land = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));   /* fA (this session, finishing STMT-BETA for the ASSIGNMENT class): per-statement dedicated exhaust-only GOTO carrying ONLY value-spine statement failures; the zw5 post-loop beta-tags it exactly as it does fB.  fJ stays SHARED and UNTAGGED (the 067 conviction at the fB comment). */   /* R1 STMT-BETA-LAND: per-statement fB (the dedicated MATCH_BEGIN-exhaust GOTO minted in sno_lower_match) -- the zw5 post-loop below retags each fB.γ to that statement's STATEMENT_BEGIN with the β port tag */
    bb_label_registry_reset();
    for (int i = 0; i < nst; i++) {
        anchor[i] = lc_build(g, IR_GOTO, NULL, NULL);
        { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(anchor[i]).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }   /* ZW-5 O-2: unconditional stno stamp (was MONITOR_BIN-gated) */
        const char * lbl = sfind_str(st[i], ":lbl");
        if (lbl && lbl[0]) bb_label_registry_add(lp_strdup(lbl), anchor[i]);
    }
    bb_label_registry_add(lp_strdup("END"), exitnd);
    /* SN4-FLAT-PROC (s176): in the ONE flat graph (and in EVAL/CODE fragment graphs) the reserved return labels are program-wide FLOATER BBs — manual Ch.8: RETURN yields the fname variable's value,
     * FRETURN signals failure, both restore the pushdown-saved formals/locals/fname; the floaters merely re-route control (peek the open activation's wires, restore rsp/___, jmp the γ/ω wire) and the
     * existing epilogue leaves on those wires perform the pop+restore+result protocol verbatim.  ANY goto — direct or $-computed — reaches them through this registry for free.  Level-0 transfer is a
     * runtime error inside rt_flat_ret_snap.  Def-body graphs (result_name != NULL) stay self-contained on their own exitnd/failnd exactly as before: their bodies are not statements of this graph. */
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
        if (fT == next && !goF && !exF && !goU && !exU && sfind(s, ":nofail")) { IR_t *nf = lc_build(g, IR_CALL, exitnd, exitnd); IR_LIT(nf).sval = (char *)"SNO$NOFAIL"; fT = nf; }   /* -NOFAIL: statement failure without a goto fires error 35 (manual ch.14 p.173) */
        IR_t * stb = zw5_on() ? lc_build(g, IR_STATEMENT_END, sT, fT) : (IR_t *) NULL;
        if (stb) { const tree_t * _sa = sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(stb).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }   /* ZW-5 O-2: stb->ival = stno for per-depth stub label names */   /* ⭐⭐ ZW-5 SLICE 2 (OMEGA O-1): the statement bracket box is minted HERE because these two lines are the ONE derivation point every statement form threads its continuations through -- sT/fT are already resolved (goto field, computed goto, or fallthrough `next`), so one edit reaches every form without touching a single statement arm below.  THE BOX IS A TRAILER, NOT A BRACKET -- measured, not inferred: x86_asm.h:544 x86_alpha() DEFINES the alpha label while x86_asm.h:547 x86_gamma() IS A JMP, so the emitted body `def alpha / jmp gamma / def beta; jmp omega` has exactly ONE entry and control can never return into it; the box is entered once, at alpha, by the statement's SUCCESS wire and falls through to the jmp that carries op_zgpop via the ONE X86H_JMP gamma hook arm (s22k one-authority -- no second whack spelling is created here). */
        IR_t * sJ = lc_build(g, IR_GOTO, stb ? stb : sT, NULL);   /* success -> the box's alpha; the box's own gamma carries sT, so the statement's real continuation is unchanged and the ONLY delta is the release's HOME (WHACK CONTRACT clause 4: BB_END_STATEMENT is op_zgpop's home; the 5,923 fused pops are its absence). */
        fail_tgt[i] = fT;   /* STMT-BETA (this session): save fT at its ONE derivation point for the post-loop STATEMENT_BEGIN omega wiring */
        IR_t * fJ = lc_build(g, IR_GOTO, fT, NULL);
        IR_t * fA = lc_build(g, IR_GOTO, fJ, NULL); asgn_land[i] = fA;   /* fA: untagged it chains fA->fJ->fT, byte-equivalent to the direct wire by the same chase-transparency the fB comment records; the post-loop beta-tag makes STATEMENT_BEGIN.beta the named failure landing for the assignment class (STATEMENT-PORT LAWS).  UNWIND clause 2 guarantees claim-base arrival, so the slice-3 per-depth ladder the 1980 comment awaited is DELETED, not built. */   /* ⛔ FAIL EDGE DELIBERATELY UNCHANGED IN SLICE 2.  The rung's admission gate is "all fail edges arrive at depth 0", and a depth-0 arrival needs NO release -- so the box's omega is genuinely dead until slice 3's per-depth stub ladder lands WITH its planner (s22h atomicity).  Checked before assuming: the wire port selector in `.sz` carries alpha (0xce 0xb1) and beta (0xce 0xb2) and the sigma marker read at emit.cpp:2285 -- there is NO omega-ARRIVAL convention, so routing fail edges into the box is not a wiring detail that was skipped, it IS the slice-3 ladder. */
        if (is_def && is_def[i]) {
            /* LADDER AB (2026-08-08): mint the per-DEFINE ACTIVATION BLOCK node.  The block is jump-target-only dead code at this
             * position -- nothing in the graph wires to it yet (AB-3 flips the call sites).  We build an IR_DEFINE node
             * γ-wired to exitnd (the block's β is its own domain; exitnd is a safe dead sink here) and insert it off the side of
             * the anchor so the emitter lays it out as unreachable .text after the DEFINE skip.  The node carries op_sval=fname
             * and IR_LIT_STRING operands for each save-set member name so the emit-time drive can call gva_index_of on them.
             * SCRIP_AB=0 restores byte-identical pre-AB emission. */
            static int _ab = -1; if (_ab < 0) { const char * _e = getenv("SCRIP_AB"); _ab = (_e && *_e == '1') ? 1 : 0; }   /* RTX-FUNC-0: default OFF (opt-in SCRIP_AB=1) until bug#2 (nformals=0 at block emit nulls formal N) lands — transfer path proven live end-to-end m3 this seat; default-on flip owed with the bug#2 fix */
            if (_ab && !g_sno_expr_define_seen) {   /* RTX-FUNC-8 hatch: any nested DEFINE in this program => mint NO blocks => ab_n==0 => call sites take the legacy fallback */
                int _argbase = 0; const tree_t * dsub = sno_stmt_define(s, &_argbase);
                const tree_t * pnode = (dsub && dsub->n > _argbase) ? dsub->c[_argbase] : NULL;
                if (pnode && sno_qlit_fold(pnode)) {
                    sno_def_t d; sno_parse_define(sno_qlit_fold(pnode), NULL, &d);
                    int nsave = 1 + d.nnames;   /* fname + formals + locals; encoded as n_operands by ir_operand_push below */
                    IR_t * ab = lc_build(g, IR_DEFINE, exitnd, failnd);
                    IR_LIT(ab).sval = lp_strdup(d.fname);   /* fname in the union; n_operands carries nsave via ir_operand_push */
                    ab->seal = d.nformals;   /* AB-3b: nformals in seal field (never set on IR_DEFINE nodes — seal is IR_MATCH_DEFER-specific); surfaced as op_ab_nformals by emitter dispatch */
                    /* operands: IR_LIT_STRING nodes carrying the variable names; emit drive calls gva_index_of at emit time */
                    { IR_t * nm = lc_build(g, IR_LIT_STRING, ab, failnd); IR_LIT(nm).sval = lp_strdup(d.fname); ir_operand_push(ab, nm); }
                    for (int _k = 0; _k < d.nnames; _k++) { IR_t * nm = lc_build(g, IR_LIT_STRING, ab, failnd); IR_LIT(nm).sval = lp_strdup(d.names[_k]); ir_operand_push(ab, nm); }
                    /* register in g->ab_nodes[] for post-main-chain emission by the driver (jump-target-only dead code until AB-3) */
                    if (g->ab_n < (int)(sizeof g->ab_nodes / sizeof *g->ab_nodes)) g->ab_nodes[g->ab_n++] = ab;
                    else fprintf(stderr, "WARN AB-1: ab_nodes[] full (>32 DEFINEs in one graph); activation block for '%s' will be missing from .s\n", d.fname);
                    { IR_t * bind = lc_build(g, IR_DEFINE, sJ, fA); IR_LIT(bind).sval = lp_strdup(d.fname); /* ROLE = n_operands==0 (bind) vs >=1 (block): IR_LIT is a union, ival and sval share storage — measured this session. */ lc_γ_to(anchor[i], bind); continue; }   /* AB-3a (this session): the DEFINE residual bind joins the LIVE chain — anchor -> bind -> sJ (γ), ω = fA per the statement fail convention (unused: DEFINE errors are fatal, not fail).  ival=2 selects the bind arm in the emit dispatch; sval carries fname for the fname-derived α label and the shared cell registry.  This replaces the bare skip ONLY on the folded-prototype + SCRIP_AB path; the shared legacy skip below still serves _ab=0 and unfolded shapes, keeping SCRIP_AB=0 byte-identical. */
                }
            }
            /* ⭐⭐⭐ DEFINE-SITE s57 (Lon in-chat: "the code for DEFINE comes directly after the statement comment ... move it there. This is shared code"): the folded DEFINE statement ALWAYS carries the
             * bind box in the shared chain (anchor -> bind -> sJ; sJ carries the :(...) goto), independent of SCRIP_AB — the box's registration half (bb_define_bind) is the DEFINE code AT the statement,
             * replacing the m4 startup hoist for dyn_scope procs.  The AB=1 path above continues before reaching here (bind already minted with the cell store); this arm serves AB=0.  Non-literal
             * prototypes fall through to the bare skip (they are outside the landed fold subset and fatal upstream at :776 anyway). */
            { int _argbase = 0; const tree_t * dsub = sno_stmt_define(s, &_argbase); const tree_t * pnode = (dsub && dsub->n > _argbase) ? dsub->c[_argbase] : NULL;
              if (pnode && sno_qlit_fold(pnode)) { sno_def_t d; sno_parse_define(sno_qlit_fold(pnode), NULL, &d);
                IR_t * bind = lc_build(g, IR_DEFINE, sJ, fA); IR_LIT(bind).sval = lp_strdup(d.fname); lc_γ_to(anchor[i], bind); continue; } }
            lc_γ_to(anchor[i], sJ); continue;
        }
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
            if (_pro_open && !result_name) sno_prologue_add(subj->v.sval);   /* PS-3 (s152): corridor pattern assignment = order-dominance license for the defer-tail arm */
            const char * bn = NULL;
            for (int fzi = 0; fzi < g_sno_nfz; fzi++) if (g_sno_fz[fzi].pat == repl) { bn = g_sno_fz[fzi].procname; break; }
            if (!bn) bn = sno_pat_collect(repl);
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fA); IR_LIT(asn).sval = subj->v.sval;
            IR_t * mk = lc_build(g, IR_CALL, asn, fA); IR_LIT(mk).sval = (char *) "SNO$MKPAT";
            IR_t * nl = lc_build(g, IR_LIT_STRING, mk, fA); IR_LIT(nl).sval = (char *) bn;
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
                IR_t * pahead = NULL; IR_t * palast = NULL; const char * pbao = getenv("SCRIP_PB_ARGORDER");   /* ⭐⭐⭐ PB-ARGORDER (s182, HQ Fable; SCRIP_PB_ARGORDER=0 reverts byte-identically): the loop below PREPENDED each arg to the chain, so N pre-args EVALUATED RIGHT-TO-LEFT while pre[] is harvested left-to-right -- SPITBOL evaluates expression operands LEFT TO RIGHT, and the divergence is observable the moment an operand has a side effect.  MEASURED by the IPC 2-way monitor (Lon's call) on beauty + a one-newline input: FIRST DIVERGENCE of the whole run, step 1499, case.inc:23 `icase = icase (upr(letter) | lwr(letter))` -- oracle CALL upr, scrip CALL lwr.  Standalone witness probe/cn/cn_alt_eval_order.sno: `P = (f('a') | g('a'))` prints call-f/call-g on sbl and call-g/call-f here, while the CONCAT twin `Q = f('a') g('a')` is correct in both (sx_binop chains forward), which is what convicts this loop and not the TT_ALT arm. */
                for (int api = 0; api < tx.npre; api++) {
                    char abuf[48]; snprintf(abuf, sizeof abuf, tx.pre[api].snapg ? "%s$V%d" : "%s$A%d", bn, api);   /* PB-1s: $V<i> = plain-ref VALUE-LEAF snapshot (stage-2 fetch at THIS assignment); $A<i> = runtime-arg primitive (PAT-ARG-BIND, unchanged); same api index both walks by the identical-traversal invariant */
                    IR_t * asnA = lc_build(g, IR_ASSIGN, pae, fA); IR_LIT(asnA).sval = lp_strdup(abuf);
                    IR_t * av = NULL;
                    IR_t * ae = sx_lower(&cx, tx.pre[api].arg, asnA, fA, &av);
                    ir_operand_push(asnA, av);
                    if (pbao && *pbao == '0') { pae = ae; continue; }   /* legacy reversed chain, kept as the exact revert arm */
                    if (!pahead) pahead = ae; else lc_γ_to(palast, ae);   /* forward: arg i's ASSIGN gamma-chains to arg i+1's ENTRY; the LAST asnA keeps the gamma it was built with (nl), so the tail splice below is untouched */
                    palast = asnA;
                }
                if (pahead) pae = pahead;
                if (tx.npre > 0) { IR_t * ncnt = lc_build(g, IR_LIT_STRING, mk, fA); char cb[16]; snprintf(cb, sizeof cb, "%d", tx.npre); IR_LIT(ncnt).sval = lp_strdup(cb); ir_operand_push(mk, ncnt); lc_γ_to(nl, ncnt); }   /* PB-1s (s108): args[1] = pre[] count -- SNO$MKPAT freezes the $V<i> stage-2 stores above into the fresh DTP's per-construction snap vector (manual p.85-86: each construction freezes ITS OWN values); spliced nl->ncnt->mk so the count literal rides the existing evaluation chain; zero-pre sites stay byte-identical */
            }
            lc_γ_to(anchor[i], pae);
            continue;
        }
        if (subj->t == TT_VAR) {
            sno_reg_var(subj->v.sval);
            /* MODE34-5b-BUGB FIX (this session): the REAL site for `X = subject ? pattern` (plain top-level
             * assignment whose replacement field is a match expression) -- confirmed via debug instrumentation
             * that the sx_lower-level TT_ASSIGN case (a sub-expression path, reached only for NESTED
             * assignments) never runs for a top-level statement: sno_build_graph's OWN statement loop lowers
             * `X = ...` by hand-unpacking :subj/:eq/:repl (opt_subject/opt_repl grammar, snobol4.y — the `=`
             * is parsed as opt_repl's leading token, not a general TT_ASSIGN tree), and THIS branch, not
             * sx_lower's, is what actually builds the IR_ASSIGN node for it.  Same underlying defect as
             * documented at the sx_lower TT_ASSIGN site above (see that comment for the oracle-grounded value
             * semantics and the mechanism): `repl` reaching here as a bare TT_SCAN means sx_lower's own
             * TT_SCAN case discards its result, so ir_operand_push(asn, vr) below pushes vr=NULL and
             * bb_assign_global/var/local's slot guard is never satisfied -- the x86_bomb "unhandled" stub.
             * Same fix, same place it actually needs to live: rewrite `X = subject ? pattern` (no repl-of-
             * REPLACE, i.e. subj?pattern is not itself inside a further `= REPLACEMENT` -- has_eq/:repl
             * already consumed that outer position, so `repl` here can only be a VALUE expression, never a
             * second replacement clause) into `subject ? (pattern . X)`, an ordinary whole-pattern capture,
             * before it ever reaches sx_lower. */
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
        }   /* ⭐ KW-3b: the statement-level twin of the site above. The ONLY structural difference from that one is whose γ the statement anchor lands on -- here the anchor chains to the value expression's ENTRY (ke) exactly as the SNO$KWSET arm chained it to the name literal, because the name literal was the first node of that arm and the value expression is the first node of this one. Getting that wrong would strand the statement's entry edge on a node the graph never reaches. */
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
            IR_t * sbeg = lc_build(g, IR_STATEMENT_BEGIN, fb, fail_tgt[i]);   /* STMT-BETA (this session, Lon ruling 2026-08-06): omega = fT makes the emitter's DRIVE_PAIR wire statement_begin_beta -> fT; beta is the named failure landing for the statement scope (always-live per flat_beta_used_scan IR_STATEMENT_BEGIN addition in emit.cpp) */
            { int _null_stmt = !lc_stmt_subj(st[i]) && !sfind_str(st[i],":lbl") && !sgoto(st[i],TT_GOTO_U) && !sgoto_expr(st[i],TT_GOTO_U) && !sgoto(st[i],TT_GOTO_S) && !sgoto_expr(st[i],TT_GOTO_S) && !sgoto(st[i],TT_GOTO_F) && !sgoto_expr(st[i],TT_GOTO_F) && !sfind(st[i],":eq"); const tree_t * _sa = _null_stmt ? NULL : sfind(st[i], ":stno"); if (_sa && _sa->n > 0 && _sa->c[0]) { const tree_t * _c = _sa->c[0]; IR_LIT(sbeg).ival = (_c->t == TT_ILIT) ? _c->v.ival : (_c->v.sval ? (int64_t)atoll(_c->v.sval) : 0); } }   /* MON-NULL-STMT: null stmt (no subj/LABEL/goto/eq) gets stno=0 → emitter tap skipped (op_stno>0 guard) — matches SPITBOL/CSNOBOL4 which emit no LABEL for blank-line pass-throughs.  B-13 FIX (BOARD, this session): the predicate previously checked subj/goto/eq but NOT ":lbl" -- a bare-label-only statement (label present, nothing else, e.g. "EMIT_x" on its own source line) was misclassified as a null pass-through and silently lost its STNO stamp, so scrip's monitor bridge never emitted a LABEL event for it, while SPITBOL's bridge counts a label line as its own null statement (manual Ch.4 p.28) and does emit one.  This was the exact desync MONITOR_SKIP_BARE_LABEL_STNO=1 (SCRIP 5ec6e607) worked around controller-side; that workaround stays in place for the moment but the native emission gap it papers over is now closed at the source. */
            lc_γ_to(anchor[i], sbeg);
            if (match_land[i]) lc_γ_tag_β(match_land[i]);
            if (asgn_land[i]) { lc_γ_to(asgn_land[i], sbeg); lc_γ_tag_β(asgn_land[i]); }   /* fA REDIRECT+TAG (this session, measured): fA.γ -> OWN sbeg, single hop, β-tagged.  NOT the fB tag-only shape: bc_chase (optimizer/branch_chain.c:30) propagates tags LAST-HOP-WINS, so a chain fA(β)→fJ(α)→fT arrives at the emitter as sz=α — tag clobbered, arm dead (measured via SCRIP_OPT_TRACE on w_fa: ".ω 0x…(op=28) -> 0x…(op=125) sz=α").  Single-hop sidesteps the clobber AND lands on the OWN statement's begin, so the β label selection (emit.cpp:1888 meaning (a)) picks THIS statement's β — which forwards to fT — instead of the chased-through TARGET statement's β, which would skip the target's body outright.  fT stays reachable via sbeg.ω (minted with fail_tgt[i] above); fJ keeps its match-arm element-retry references.  ⛔ fB (match_land) is left tag-only and is, by the same bc_chase measurement, a DEAD ARM at this HEAD — recorded in the session FINDING; lighting it takes the same redirect treatment and its own gates. */   /* fA joins the beta-tag: same tag-only discipline as fB (gamma stays = fJ; chain to fT intact for the used-scan). */   /* R1 STMT-BETA-LAND: tag-only -- fB.γ stays = fJ (chain to fT intact for used-scan and all downstream consumers); β tag makes the emitter's chase propagate oib=1 and route to betas[sbeg_k].  lc_γ_to_β would set fB.γ = sbeg, severing the fJ→fT chain and causing the emitter to miss every statement after the scan (the 175 root cause: node 10@ was the GOTO chain to n11@→n12@→n13@ which held the second STATEMENT_BEGIN; redirecting its γ dropped n12@/n13@ from used[]).  MATCH_BEGIN.ω → fB (β-tagged GOTO → fJ → fT) -- the emitter chases, sees the β tag, routes to sbeg.β, AND the used-scan follows γ to fJ and beyond, keeping the full graph reachable. */
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
        if (!ssrc) continue;   /* EMPTY-STMT src (Lon s116, the dup-banner fix): a blank-line statement now carries an EMPTY note by construction (stmt_ast.c) -- register it so bb_src_of still roots the zd run exactly as before (measured: dropping it merged the empty pair into the prior run and moved its add-rsp release); the banner block prints nothing for an empty note. */
        IR_t * t = anchor[i];
        int hops = 0;
        while (t && t->op == IR_GOTO && t->γ.node && hops++ < 64) t = t->γ.node;   /* ⛔⭐⭐ ON-4 (s23e) — THE OBVIOUS FIX HERE IS NOT AN ANNOTATION CHANGE. DO NOT RE-ATTEMPT IT ON AN ANNOTATION RUNG.  The cross-statement pileup is real and diagnosed: convergent GOTO chains land several statements' anchors on ONE node, bb_src_note's append arm bundles them, and roman.s stacks FIVE unrelated source lines above n129 with four of them attributed to the wrong head.  The one-line cure is to stop the chase at a node another statement already owns (`&& !bb_src_of(t->γ.node)`).  MEASURED s23e: it moves EMITTED CODE — 9 of 21 benchmarks and 5 of 122 pattern crosschecks diverge.  ROOT CAUSE: bb_src_of is NOT a comment facility.  emit.cpp's zd_plan roots STATEMENT SEGMENTATION on it ("Runs are already rooted at bb_src_of statement heads"), so which node carries a source note decides where statement runs begin, which decides claims, offsets and depth — relocating a note relocates a statement boundary.  THEREFORE: this fix belongs to the ZD/segmentation ladder WITH a full ON-0 watermark bracket, never to an OBJ-NOTE rung whose whole contract is behaviour-neutrality.  The genuinely inert half of ON-4 landed instead: bb_src_note is now idempotent (exact-segment dedup), which changes the TEXT a node holds and never WHICH node holds one, so segmentation is untouched by construction. */
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
/* RTX-FUNC-8 (2026-08-10): AB SAFETY HATCH for EXPRESSION-POSITION DEFINE.  sno_prescan_expr registers BOTH statement-position and
 * nested DEFINEs, but the AB activation block + bind are minted ONLY on the statement path (sno_build_graph, the sno_stmt_define arm),
 * so a nested DEFINE leaves fn_cell$<FN> unwired (new name -> rt_ab_undef_fn_stub) or STALE (redefine -> the PREVIOUS body, entered under
 * the new prototype's frame shape -> SIGSEGV; crosscheck/rung10/1011_func_redefine).  Until RTX-FUNC-8 proper lands -- which needs the
 * compile-time vs runtime DEFINE binding question routed, since the block must match the LIVE definition and not merely exist -- a program
 * containing ANY nested DEFINE takes NO activation blocks at all, so ab_n stays 0 and every call site falls through the already-tested
 * SCRIP_AB=0 hatch to the classic open_slim body.  Correctness-only: AB can refuse to fire, it can never fire on a stale block.
 * g_sno_prescan_top is the statement SUBJECT currently being prescanned; a DEFINE node that IS the subject is statement-position and does
 * NOT trip the hatch -- without that test every DEFINE-bearing program would trip it and AB would be dead everywhere. */
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
            if (t != g_sno_prescan_top) g_sno_expr_define_seen = 1;   /* RTX-FUNC-8 hatch: nested DEFINE gets no activation block, so this program takes none at all (see the note at sno_prescan_expr) */
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
        scx_t ex; ex.g = gx; ex.loop_exit = NULL; ex.loop_next = NULL; ex.result_name = g_sno_exprs[xi].name; ex.pat_fail = NULL; ex.pat_seal = NULL; ex.npre = 0;   /* PB-1s hygiene: npre was uninitialized in this context -- any sno_pat_node reached under an expr thunk read garbage */
        IR_t * ok = lc_build(gx, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gx, IR_FAIL, NULL, NULL);
        IR_t * sJ = lc_build(gx, IR_GOTO, ok, NULL);
        IR_t * fJ = lc_build(gx, IR_GOTO, no, NULL);
        sno_reg_var(g_sno_exprs[xi].name);
        IR_t * asn = lc_build(gx, IR_ASSIGN, sJ, fJ); IR_LIT(asn).sval = (char *) g_sno_exprs[xi].name;
        IR_t * vr = NULL; IR_t * e = sx_lower(&ex, g_sno_exprs[xi].expr, asn, fJ, &vr);
        ir_operand_push(asn, vr);
        if (g_sno_exprs[xi].want_name) { IR_t * wn_lit = lc_build(gx, IR_LIT_STRING, NULL, fJ); IR_LIT(wn_lit).sval = (char *) ""; IR_t * wn_call = lc_build(gx, IR_CALL, NULL, fJ); IR_LIT(wn_call).sval = (char *) "SNO$WANTNM"; lc_γ_to(wn_lit, wn_call); lc_γ_to(wn_call, e); ir_operand_push(wn_call, wn_lit); gx->entry = wn_lit; } else { gx->entry = e; }
        { IR_t * ad = lc_build(gx, IR_DEFINE, gx->entry, fJ); IR_LIT(ad).ival = 3; gx->entry = ad; }   /* ⭐ EXPR-CLASSP (2026-08-09, the h2 silent-success-drop root): every live EXPR$ caller is the WIRE loop (dcap pump / rt_defer_open both do rt_proc_call_open then lea rcx,γ; lea rdx,ω; jmp rax — a pcall record is ALWAYS open), yet the graph exited CLASS C (bb_glue_outer_γ: mov rsp,___; pop ___; mov eax,DT; ret) against an ___ NO prologue established — the GLUE-SYM disease verbatim, a fossil of the deleted C-side rt_dcap_flush.  Prepending the role-3 WIRE-ADOPT makes the graph satisfy the driver's EXISTING CLASS P floor predicate (scrip.c ×4: entry==IR_DEFINE && ival==3), so exits become bb_glue_wire_γ/ω → rt_flat_ret_snap → the adopted wires — the same one authority DEFINE stubs use; zero driver/pump edits. */
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
static IR_t * sno_pat_carrier_build(scx_t * px, const tree_t * pat, IR_t * ok, IR_t * no, IR_t ** out_brt, int * out_pfenced) {   /* ⭐⭐⭐ THE ONE AUTHORITY for "build a stored-pattern body and capture its rightmost resume carrier" (s183, HQ Fable — extracted VERBATIM from sno_pat_thunks_build so the two roads cannot drift; the extraction is the whole point, see sno_pat_tree_graph_rt).  Half B1 of SN4-DEFER-RESUME (s121): a fence-free multi-element top is built through the IDENTICAL flatten→nary path sno_pat_node's TT_SEQ arm takes (same flatten, same args, byte-identical emission) — one level lower ONLY to capture out_rtail, the run's rightmost resume carrier (beauty's `Parse` is a 4-element SEQ; resume must land its rightmost generator, never element 0).  Fenced or single-element tops keep the sno_pat_node route verbatim. */
    IR_t * pe; *out_brt = NULL;
    int pfenced = sno_pat_contains_fence(pat, 0);   /* ⛔ SCRIP_FENCE_IGNORE=1 IS A DIAGNOSTIC ONLY, NEVER A FIX (s182): zeroes the fence verdict outright so a seat can MEASURE how much of a program's behaviour the s121 fence refusal alone is responsible for.  It is UNSOUND by construction -- the nary diversion and the carrier publication both key off pfenced, and a fence really does refuse a backward entry -- so a green run under it is a MEASUREMENT, not a cure. */
    if (getenv("SCRIP_FENCE_IGNORE")) pfenced = 0;   /* ⭐ SN4-DEFER-RESUME FENCE REFUSE: one verdict feeds both the nary diversion and the carrier publication */
    { const tree_t * fel[128]; int fne = 0; sno_seq_flatten_pat(pat, fel, &fne);
      pe = (sno_defer_resume() && fne > 1 && !pfenced) ? sno_seq_nary(px, fel, fne, ok, no, out_brt) : sno_pat_node(px, pat, ok, no); }
    *out_pfenced = pfenced;
    return pe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_pat_publish_body_root(IR_graph_t * gp, int before_pat, const tree_t * pat, IR_t * brt, int pfenced, const char * dbgname) {   /* ⭐⭐⭐ THE ONE AUTHORITY for "which node is this blob's published resume surface" (s183, HQ Fable — extracted VERBATIM from sno_pat_thunks_build; sno_pat_tree_graph_rt spelled this fact a SECOND time and got it wrong, which was THE M1 WALL).  body_root IS the resume surface: fence-free SEQ top → nary's out_rtail (rightmost element's carrier); else the first REAL body node — "R first-allocated ⇒ R.β IS resume" per the TT_ARB/TT_ARBNO arms' own doc, and a deterministic carrier's β is a harmless fail-through, so this is uniform.  Leading argument-less GOTO relays (nested-nary sentinels) are skipped exactly as SEQ-ERAD SE-6's own warning prescribes.  right-sealed blobs keep body_root=NULL — a sealed blob exposes NO resume surface (manual Ch.19, backup through the rightmost fence must not re-enter). */
    IR_t * rn = NULL;
    if (sno_defer_resume() && !pfenced) {
        rn = brt;
        if (!rn) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { rn = x2; break; } }
    }
    else if (sno_defer_resume() && pfenced) { extern int zdp_seam_tier(const IR_t *); const char * _fre = getenv("SCRIP_FENCE_RESUME");   /* ⭐⭐⭐ FENCE-RESUME (s182, HQ Fable; SCRIP_FENCE_RESUME=0 reverts byte-identically).  THE s121 FENCE REFUSE IS A SHAPE REFUSAL AND LAW 0d FORBIDS IT: a fence ANYWHERE published body_root=NULL for the WHOLE blob, so a stored `ARBNO(FENCE(X))` emitted `PAT$N_β: jmp PAT$N_ω` and CONCEDED instead of extending.  THE MANUAL IS THE AUTHORITY (v3.7 Ch.9/18): FENCE "fails if the scanner has to BACK UP THROUGH it", while ARBNO's β does not back through anything -- it EXTENDS FORWARD with one more instance at the current cursor.  NARROWED, NOT DELETED (the refusal owns a named 7-mover class 114/119/129/130/148/149/150): the carrier is published ONLY when the FIRST real body node is itself a TIER-1 GENERATOR by the zdp lattice.  The fence template's own β still concedes if it is ever reached, so the cut semantics are untouched. */
        if (!(_fre && *_fre == '0')) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { if (zdp_seam_tier(x2) == 1) rn = x2; break; } }
    }
    int rs = sno_pat_right_sealed(pat) ? 1 : 0;
    gp->body_root = (gp->n > before_pat && !rs) ? ((sno_defer_resume() && pfenced && !rn) ? NULL : (rn ? rn : gp->all[before_pat])) : NULL;
    if (getenv("SCRIP_RESUME_WHY") && !gp->body_root)   /* ⛔ DIAGNOSTIC ONLY (s182): a stored pattern with body_root==NULL concedes WHOLESALE at beta, so naming WHICH of the three refusals fired -- empty body, right-sealed, or fenced-without-a-tier-1-carrier -- turns 'N blobs refuse' into N named rungs. */
        { extern int zdp_seam_tier(const IR_t *); const IR_t * _fb = NULL; for (int k3 = before_pat; k3 < gp->n; k3++) { IR_t * x3 = gp->all[k3]; if (x3 && !(x3->op == IR_GOTO && x3->n_operands == 0)) { _fb = x3; break; } }   /* ⛔ DIAGNOSTIC ONLY (s183, seat3): fb/fbtier/chain name WHAT THE CARRIER SCAN IS LOOKING AT, which is the whole of row blob-resume-refusals -- all five refusing beauty blobs answer fb=IR_MATCH_ALTERNATE or fb=IR_MATCH_FENCE1, both seam-tier 0, and the allocation-order chain shows allocation runs RIGHT-TO-LEFT, so "the first real body node" is the RIGHTMOST element and a tier-1 generator further left is never looked at.  Same getenv, same read-only shape, 0 .s movers over 272 programs. */
            fprintf(stderr, "[RESUME-NIL] pat=%s empty=%d right_sealed=%d pfenced=%d rn=%d brt=%d fb=%s fbtier=%d chain=%s|%s|%s|%s\n", dbgname ? dbgname : "?", !(gp->n > before_pat), rs, pfenced, rn ? 1 : 0, brt ? 1 : 0, _fb ? bb_op_name(_fb->op) : "-", zdp_seam_tier(_fb), (before_pat + 0 < gp->n) ? bb_op_name(gp->all[before_pat + 0]->op) : "-", (before_pat + 1 < gp->n) ? bb_op_name(gp->all[before_pat + 1]->op) : "-", (before_pat + 2 < gp->n) ? bb_op_name(gp->all[before_pat + 2]->op) : "-", (before_pat + 3 < gp->n) ? bb_op_name(gp->all[before_pat + 3]->op) : "-"); }
    if (getenv("SCRIP_RESUME_WHY"))   /* ⛔ DIAGNOSTIC ONLY (s183, seat1's [RTGRAPH] probe grafted into the shared authority at the merge so it names the carrier for BOTH roads, not just the JIT one -- a diagnostic that answers for one of two callers is how the second spelling stayed invisible for 60 sessions). */
        fprintf(stderr, "[RTGRAPH] pat=%s pfenced=%d brt=%d rn=%d body_root_op=%d\n", dbgname ? dbgname : "?", pfenced, brt ? 1 : 0, rn ? 1 : 0, gp->body_root ? (int) gp->body_root->op : -1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
        IR_t * pe = sno_pat_carrier_build(&px, g_sno_pats[pi2].pat, ok, no, &brt, &pfenced);   /* ⭐ s183: the build+capture is now THE ONE AUTHORITY shared with sno_pat_tree_graph_rt (the JIT road) — this site's behaviour is unchanged by construction, the extraction is verbatim. */
        {
            extern tree_t *ast_stmt_new(tree_e kind);
            IR_t * paft = pe;
            for (int api = 0; api < px.npre; api++) {
                if (px.pre[api].snapg) { char vbuf[48]; snprintf(vbuf, sizeof vbuf, "%s$V%d", g_sno_pats[pi2].name, api); char * vg = lp_strdup(vbuf); sno_reg_var(vg); IR_LIT(px.pre[api].prim).sval = vg; sno_fz_mark_defer(gp, px.pre[api].prim, vg); continue; }   /* PB-1s: DEFER reads the $V<i> global (frozen at MKPAT = stage-2); no COERCE, no operand edge */
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
        sno_pat_publish_body_root(gp, before_pat, g_sno_pats[pi2].pat, brt, pfenced, g_sno_pats[pi2].name);   /* ⭐ s183: the publish is now THE ONE AUTHORITY shared with sno_pat_tree_graph_rt (the JIT road), which spelled this fact a SECOND time and got it wrong — THE M1 WALL. */
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
    g_sno_expr_define_seen = 0; g_sno_prescan_top = NULL;   /* RTX-FUNC-8 hatch: per-program state, reset at the whole-program entry */
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
        g_sno_prescan_top = lc_stmt_subj(st[i]);   /* RTX-FUNC-8 hatch: mark statement position so only NESTED DEFINEs trip it */
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
            gf = sno_build_call_stub(defs[di].entry, defs[di].fname);   /* SN4-FLAT-PROC (s176): the extraction regime is retired — the body statements live ONLY in the one main graph; the stub is the callable citizen (jmp-entry prologue carves a fresh MAIN-layout frame via the driver's frame floor, WIRE-ADOPT records the way home, IR_GOTO_DEFERRED transfers to the entry label at CALL time through the same registry rt_goto_transfer serves).  (void)rn: the proc record below still carries result_name for the epilogue leaves. */
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
    int rtc = sno_rtseq_resume();   /* ⭐⭐⭐ RTSEQ-RESUME / RT-CARRIER (s183 -- seat1 and HQ Fable found this INDEPENDENTLY and landed within minutes of each other; MERGED here, seat1's killswitch name kept because it landed first, HQ's EXTRACTION kept because the defect WAS a second spelling and a third copy would re-arm it).  ⛔ THIS WAS THE M1 WALL.  A RUNTIME-COMPOSED pattern (`P = mk() ''` -- a concat whose elements are FUNCTION-CALL RESULTS, i.e. beauty's `Parse` verbatim) is not a PAT$ blob: pat_cat builds a TT_SEQ recipe and dtp_fn_of JITs it through HERE.  This builder published `gp->all[before_pat]` RAW, which for a multi-element SEQ is a leading argument-less GOTO relay; branch_chain.c:59 then chased that relay to the IR_SUCCEED terminator, which is not even in the emitted node set, so emit.cpp's β dispatch found no resume target and defaulted to lbl_ω.  MEASURED: body_root_op=116 (IR_SUCCEED) tier=0 in_nodes=0 on the JIT road vs op=72 (IR_MATCH_LIT) tier=2 in_nodes=1 on the statically-lowered twin -- so the composed pattern's β could never re-enter its LEFT element to extend it, and the retreat unwound to MATCH_BEGIN's restart loop instead.  Witness pair: probe/passthru/ptw_min_fncat_arbno (ARBNO ports α+γ only, never β) vs ptw_min_varcat (α+β+γ).  THE CURE IS NOT A NEW RULE -- it is deleting the second spelling: both roads now call the ONE AUTHORITY, so the JIT road inherits the nary carrier capture, the FENCE-RESUME tier-1 narrowing (which the copy-paste cut omitted -- beauty's own `Command = nInc() FENCE(3-arm ALT)` is exactly a fenced composed pattern), the right-sealed refusal and the GOTO-relay skip.  SCRIP_RTSEQ_RESUME=0 restores the two pre-rung lines verbatim. */
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
    g_sno_nfz = 0; g_sno_fz_unsafe = 1; g_sno_nencl = 0;
    g_sno_nt4 = 0; g_sno_t4_unsafe = 1;   /* T4: fragments never fold (the g-cn2 boundary precedent -- fragment-local rescans stay conservative) */
    int seal_sv = g_sno_seal_enabled; g_sno_seal_enabled = 0;   /* ⭐⭐⭐ g-cn2 -- THE EVAL/CODE BOUNDARY, ENFORCED. g_sno_seal_enabled's own doc says the runtime fragment compiler "must stay conservative", but NOTHING enforced it: lower_sno_stage2 grants the flag once and no path ever cleared it, so a fragment re-entered sx_lower with the WHOLE PROGRAM's seal table live and T1 folded `&N` against a tree_t owned by the main compile. Measured (s153, pristine b69c63a5): m3 `EVAL('&N')` printed EMPTY with T1 on and 42 with SCRIP_CONST_T1=0, while m4 printed 42 in BOTH arms -- because a mode-4 binary compiles its fragments in a DIFFERENT PROCESS whose table is empty, i.e. m4 was accidentally correct for the reason this line now makes deliberate. That is a MODE34-IDENTICAL violation on the DEFAULT arm and a silent wrong answer, not an error. Clearing here is the one-line statement of ARCH-SN4-CONSTANTS's own ruling ("no fold in the runtime-compile path is the safe default") and it closes T1 and T2 together: sno_const_val and sno_const_pat are both gated on this flag alone, and an inlined T2 graph carries the same cross-compile pointer hazard. Fragments keep reading the SEALED CELL by name -- CN-2's binding is process-global and is what makes 341/342 answerable inside a thunk. Saved and restored rather than left 0 because mode 3 keeps lowering state alive for the life of the process. */
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
