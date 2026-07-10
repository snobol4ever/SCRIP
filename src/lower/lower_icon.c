#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lower.h"
extern int icn_builtin_is_known(const char *);
extern int icn_builtin_is_generator(const char *);
int g_postfix_resume = 0;
static int icn_const_step(const tree_t * s, int64_t * bits, int * isr);
typedef struct {
    IR_graph_t * g; IR_t * psucc; IR_t * pfail; const char ** pn; int npn; const char ** ln; int nln;
    IR_t * last_gen; IR_t * loop_exit; IR_t * loop_next; IR_t * beta; IR_t * conj_resumable;
} icx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_is_local(const icx_t * cx, const char * nm) { if (!nm) return 0; for (int i = 0; i < cx->nln; i++) if (cx->ln[i] && !strcmp(cx->ln[i], nm)) return 1; return 0; }
static void γ_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_γ_to_β(nd, t); else lc_γ_to(nd, t); }
static void ω_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = lc_build(cx->g, op, γ, ω);
    if (γ && ir_is_generator_kind(γ->op)) lc_γ_to_β(nd, γ);
    if (ω && ir_is_generator_kind(ω->op)) lc_ω_to_β(nd, ω);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * icn_cset_canon(const char * s) {
    if (!s) return s;
    unsigned char seen[256]; memset(seen, 0, sizeof seen);
    for (const unsigned char * p = (const unsigned char *) s; *p; p++) seen[*p] = 1;
    char buf[257]; int n = 0;
    for (int c = 0; c < 256; c++) if (seen[c]) buf[n++] = (char) c;
    buf[n] = 0; return lp_strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_proc_is_generator(const char * name) {
    if (!name) return 0; for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_generator;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_call_allow_gen(const char * name) { return name && (icn_proc_is_generator(name) || !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "key")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int augop_code(int aop) {
    switch (aop) {
    case AUGOP_ADD: return 0; case AUGOP_SUB: return 1; case AUGOP_MUL: return 2; case AUGOP_DIV: return 3; case AUGOP_MOD: return 4;
    case AUGOP_POW: return 18; case AUGOP_CONCAT: return 11; case AUGOP_EQ: return 9; case AUGOP_SEQ: return 16;
    case AUGOP_LT: return 5; case AUGOP_LE: return 6; case AUGOP_GT: return 7; case AUGOP_GE: return 8; case AUGOP_NE: return 10;
    case AUGOP_SLT: return 12; case AUGOP_SLE: return 13; case AUGOP_SGT: return 14; case AUGOP_SGE: return 15; case AUGOP_SNE: return 17;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_e icn_augop_binop_tt(int a) {
    switch (a) {
    case AUGOP_ADD: return TT_ADD; case AUGOP_SUB: return TT_SUB; case AUGOP_MUL: return TT_MUL; case AUGOP_DIV: return TT_DIV; case AUGOP_MOD: return TT_MOD; case AUGOP_POW: return TT_POW;
    case AUGOP_CONCAT: return TT_CAT; case AUGOP_EQ: return TT_EQ; case AUGOP_LT: return TT_LT; case AUGOP_LE: return TT_LE;
    case AUGOP_GT: return TT_GT; case AUGOP_GE: return TT_GE; case AUGOP_NE: return TT_NE;
    case AUGOP_SEQ: return TT_LEQ; case AUGOP_SLT: return TT_LLT; case AUGOP_SLE: return TT_LLE; case AUGOP_SGT: return TT_LGT; case AUGOP_SGE: return TT_LGE; case AUGOP_SNE: return TT_LNE;
    case AUGOP_CSET_UNION: return TT_CSET_UNION; case AUGOP_CSET_DIFF: return TT_CSET_DIFF; case AUGOP_CSET_INTER: return TT_CSET_INTER;
    default: return (tree_e) 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_unop_tt(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL: case TT_CSET_COMPL: case TT_INTERROGATE: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_resumable(const tree_t * t) {
    if (!t) return 0; if (t->t == TT_STMT) t = stmt_subj(t); if (!t) return 0;
    if (t->t == TT_FNC) { const char * nm = (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) ? t->c[0]->v.sval : NULL; return icn_call_allow_gen(nm); }
    if (lc_is_binop(t->t)) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_IDX) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_ASSIGN) { if (t->n > 0 && t->c[0] && t->c[0]->t == TT_ITERATE) return 1; return (t->n > 1) ? is_resumable(t->c[1]) : 0; }
    if (t->t == TT_SWAP) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    switch (t->t) {
    case TT_IF: case TT_SCAN: case TT_EVERY: case TT_TO: case TT_TO_BY: case TT_ALTERNATE: case TT_REPEAT: case TT_WHILE: case TT_UNTIL: case TT_REVASSIGN: case TT_ITERATE: return 1;
    default: return 0; }
}
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static int icn_subtree_has_suspend(const tree_t *n);
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_arg_lower(void * vcx, const tree_t * a, IR_t * F) {
    icx_t * cx = (icx_t *) vcx; IR_t * sps = cx->psucc; IR_t * spf = cx->pfail;
    cx->psucc = NULL; cx->pfail = F;
    IR_t * r = NULL; IR_t * e = lower(cx, a, NULL, F, &r);
    cx->psucc = sps; cx->pfail = spf;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((icx_t *) vcx)->g, icn_arg_lower, vcx, a); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_arg_is_scan_fn(const tree_t * a) {
    if (!a) return 0; if (a->t == TT_STMT) a = stmt_subj(a); if (!a || a->t != TT_FNC) return 0; const char * nm = (a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR) ? a->c[0]->v.sval : NULL;
    return nm && (!strcmp(nm, "tab") || !strcmp(nm, "move") || !strcmp(nm, "pos") || !strcmp(nm, "any") || !strcmp(nm, "match")
                  || !strcmp(nm, "many") || !strcmp(nm, "upto") || !strcmp(nm, "find") || !strcmp(nm, "bal"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(icx_t * cx, const char * name, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (name && !strcmp(name, "seq")) { IR_t * sq = lower_seq(cx, t, argbase, nargs, γ, ω, res); if (sq) return sq; }
    if (name && !strcmp(name, "key") && nargs == 1) { IR_t * kg = lower_key(cx, t, argbase, nargs, γ, ω, res); if (kg) return kg; }
    int gb = name && nargs == 2 && (!strcmp(name, "find") || !strcmp(name, "upto"));
    IR_t * call = build(cx, icn_proc_is_generator(name) ? IR_PROC_GEN : (gb ? IR_CALL_BUILTIN_GEN : IR_CALL), γ, ω); IR_LIT(call).sval = (char *) name;
    if (res) *res = call;
    int chains = name && (!strcmp(name, "write") || !strcmp(name, "writes"));
    int is_cursor_mover = name && (!strcmp(name, "tab") || !strcmp(name, "move"));
    if (!chains) { for (int k = 0; k < nargs; k++) if (is_resumable(t->c[argbase + k])) { if (is_cursor_mover && icn_arg_is_scan_fn(t->c[argbase + k])) continue; chains = 1; break; } }
    IR_t * prev = NULL; IR_t * entry = call; IR_t * aω = ω; IR_t * last_ar = NULL;
    for (int k = 0; k < nargs; k++) {
        const tree_t * a = t->c[argbase + k]; IR_t * ar = NULL;
        IR_t * ae = lower(cx, a, (k == nargs - 1) ? call : NULL, aω, &ar); aω = cx->beta;
        if (k == 0) entry = ae;
        if (prev) lc_γ_to(prev, ae); /* ARG-BOUNDARY α-FORCE: forward success edge into the next arg is a fresh entry, never a resume; auto-β (γ_to) here made a keyword/generator-entry arg swallow the whole call. Resume direction flows via aω/cx->beta, untouched. */
        prev = ar;
        if (ar) { ir_operand_push(call, ar); last_ar = ar; }
    }
    if ((icn_proc_is_generator(name) || gb) && last_ar) lc_γ_to(last_ar, call);
    cx->beta = (icn_proc_is_generator(name) || gb) ? call : (g_postfix_resume ? aω : ω);
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_idx_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    if (t->n < 2 || !t->c[0]) { IR_t * su = build(cx, IR_SUCCEED, NULL, ω); *var_res = su; return su; }
    IR_t * br = NULL; IR_t * entry;
    const tree_t * b0 = t->c[0];
    if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') {
        IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; br = vr; entry = vr;
    } else if (b0->t == TT_IDX) {
        entry = lower_idx_var(cx, b0, ω, &br);
    } else entry = lower(cx, b0, NULL, ω, &br);
    IR_t * cur = br; IR_t * hook = br; IR_t * prevβ = (b0->t == TT_VAR || b0->t == TT_IDX) ? NULL : cx->beta;
    for (int k = 1; k < t->n; k++) {
        IR_t * ir = NULL; IR_t * ie = lower(cx, t->c[k], NULL, prevβ ? prevβ : ω, &ir); prevβ = cx->beta;
        γ_to(hook, ie);
        IR_t * sub = build(cx, IR_SUBSCRIPT, NULL, prevβ ? prevβ : ω);
        γ_to(ir, sub);
        ir_operand_push(sub, cur); ir_operand_push(sub, ir);
        cur = sub; hook = sub;
    }
    *var_res = cur; return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_lvalue_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    if (!t) return NULL;
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] != '&') {
        IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = t->v.sval; *var_res = vr; return vr;
    }
    if (t->t == TT_IDX) return lower_idx_var(cx, t, ω, var_res);
    if (t->t == TT_ITERATE && t->n > 0 && t->c[0]) {
        IR_t * it = build(cx, IR_ITERATE, NULL, ω); IR_LIT(it).ival = 0; IR_LIT(it).sval = "lv";
        const tree_t * b0 = t->c[0]; IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, it); ir_operand_push(it, ar);
        cx->beta = it;
        *var_res = it; return ae;
    }
    if (t->t == TT_RANDOM && t->n > 0 && t->c[0]) {
        IR_t * rn = build(cx, IR_RANDOM, NULL, ω);
        const tree_t * b0 = t->c[0]; IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, rn); ir_operand_push(rn, ar);
        *var_res = rn; return ae;
    }
    if ((t->t == TT_SECTION || t->t == TT_SECTION_PLUS || t->t == TT_SECTION_MINUS) && t->n >= 3 && t->c[0] && t->c[1] && t->c[2]) {
        int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * sec = build(cx, IR_SUBSCRIPT, NULL, ω); IR_LIT(sec).ival = 0; IR_LIT(sec).sval = "lv";
        IR_t * ar = NULL; IR_t * ae; const tree_t * b0 = t->c[0];
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else if (b0->t == TT_SECTION || b0->t == TT_SECTION_PLUS || b0->t == TT_SECTION_MINUS || b0->t == TT_IDX) ae = lower_lvalue_var(cx, b0, ω, &ar);
        else ae = lower(cx, b0, NULL, ω, &ar);
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[1], NULL, ω, &br); γ_to(ar, be);
        IR_t * i1β = cx->beta;
        IR_t * cr = NULL; IR_t * ce = lower(cx, t->c[2], sec_variant ? NULL : sec, i1β, &cr); γ_to(br, ce);
        if (sec_variant) {
            IR_t * op = build(cx, IR_BINOP, sec, i1β); IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB; ir_operand_push(op, br); ir_operand_push(op, cr); γ_to(cr, op); cr = op;
        }
        ir_operand_push(sec, ar); ir_operand_push(sec, br); ir_operand_push(sec, cr);
        lc_γ_to(cr, sec);
        *var_res = sec; return ae;
    }
    if (t->t == TT_FIELD && t->n > 0 && t->c[0]) {
        IR_t * fg = build(cx, IR_FIELD_VAR, NULL, ω);
        IR_LIT(fg).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[0], NULL, ω, &br);
        lc_γ_to(br, fg); ir_operand_push(fg, br);
        *var_res = fg; return be;
    }
    if ((t->t == TT_NULL || t->t == TT_NONNULL) && t->n > 0 && t->c[0]) {
        IR_t * clv = NULL; IR_t * ce = lower_lvalue_var(cx, t->c[0], ω, &clv);
        if (!ce || !clv) return NULL;
        IR_t * ut = build(cx, IR_NULLTEST_VAR, NULL, ω); IR_LIT(ut).sval = (t->t == TT_NONNULL) ? "nonnull" : "null";
        ir_operand_push(ut, clv); lc_γ_to(clv, ut);
        *var_res = ut; return ce;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_scan_kind_for(const char * s) {
    if (!s) return 0;
    if (!strcmp(s,"tab"))   return (int)IR_SCAN_TAB;
    if (!strcmp(s,"move"))  return (int)IR_SCAN_MOVE;
    if (!strcmp(s,"upto"))  return (int)IR_SCAN_UPTO;
    if (!strcmp(s,"any"))   return (int)IR_SCAN_ANY;
    if (!strcmp(s,"many"))  return (int)IR_SCAN_MANY;
    if (!strcmp(s,"find"))  return (int)IR_SCAN_FIND;
    if (!strcmp(s,"match")) return (int)IR_SCAN_MATCH;
    if (!strcmp(s,"pos"))   return (int)IR_SCAN_POS;
    if (!strcmp(s,"bal"))   return (int)IR_SCAN_BAL;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_retag_scan_body(IR_graph_t * g, int depth) {
    if (!g || !g->all || depth > 16) return;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        if ((nd->op == IR_CALL || nd->op == IR_CALL_BUILTIN || nd->op == IR_CALL_BUILTIN_ICON) && IR_LIT(nd).sval && nd->n_operands == 1) { int k = icn_scan_kind_for(IR_LIT(nd).sval); if (k) nd->op = (IR_e) k; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lc_key(icx_t * cx, const tree_t * t, const char * kw, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * id = (kw && kw[0] == '&') ? kw + 1 : kw;
    if (id && !strcmp(id, "line")) { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = (t && t->line > 0) ? t->line : 0; *res = nd; return nd; }
    if (id && !strcmp(id, "file")) { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; *res = nd; return nd; }
    if (id) {
        const char * cs = !strcmp(id, "ucase") ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : !strcmp(id, "lcase") ? "abcdefghijklmnopqrstuvwxyz" : !strcmp(id, "digits") ? "0123456789" : NULL;
        if (!cs && !strcmp(id, "letters")) cs = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        if (cs) { IR_t * nd = build(cx, IR_LIT_CHARSET, γ, ω); IR_LIT(nd).sval = icn_cset_canon(cs); *res = nd; return nd; }
    }
    int is_gen_kw = id && (!strcmp(id, "features") || !strcmp(id, "regions") || !strcmp(id, "storage") || !strcmp(id, "collections"));
    IR_t * nd = build(cx, is_gen_kw ? IR_KEYWORD_ICON_GEN : IR_KEYWORD_ICON, γ, ω); IR_LIT(nd).sval = (char *) kw;
    if (is_gen_kw) {
        IR_t * seed = build(cx, IR_GOTO, ω, ω); lc_γ_to(seed, nd); cx->beta = nd; *res = nd; return seed;
    }
    *res = nd; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    cx->beta = ω;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (lc_is_binop(t->t)) {
        { int64_t fb = 0; int fr = 0; if (icn_const_step(t, &fb, &fr) && fr) { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; } }
        int64_t bcode = lc_binop_code(t->t); int is_relop = (bcode >= BINOP_LT && bcode <= BINOP_NE) || (bcode >= BINOP_SLT && bcode <= BINOP_SNE) || bcode == BINOP_EQV || bcode == BINOP_NEQV;
        IR_t * op = build(cx, is_relop ? IR_BINOP_TEST : IR_BINOP, γ, ω); IR_LIT(op).ival = bcode;
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta; IR_t * eb = lower(cx, t->c[1], op, lβ, &rr);
        IR_t * rβ = cx->beta;
        IR_t * opfail = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : NULL);
        if (is_relop && opfail) ω_to(op, opfail);
        lc_γ_to(lr, eb); ir_operand_push(op, lr); ir_operand_push(op, rr);
        cx->beta = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : ω);
        *res = op; return ea; }
    if (is_unop_tt(t->t)) {
        {
            int64_t fb = 0; int fr = 0;
            if (icn_const_step(t, &fb, &fr)) {
                if (fr) {
                    IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd;
                }
                IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = fb; *res = nd; return nd;
            }
        }
        IR_e uop_kind = (t->t == TT_NONNULL) ? IR_UNOP_TEST : IR_UNOP;
        IR_t * op = build(cx, uop_kind, γ, ω); IR_LIT(op).ival = (long long) t->t; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); ir_operand_push(op, orr); *res = op; return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_CSET: { IR_t * nd = build(cx, IR_LIT_CHARSET, γ, ω); IR_LIT(nd).sval = icn_cset_canon(t->v.sval); *res = nd; return nd; }
    case TT_NULL: {
        if (t->n > 0 && t->c[0]) {
            IR_t * op = build(cx, IR_UNOP_TEST, γ, ω); IR_LIT(op).ival = (long long) TT_NULL; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); ir_operand_push(op, orr); *res = op;
            return ea;
        } IR_t * nd = build(cx, IR_FAIL, γ, ω);
        *res = nd; return nd;
    }
    case TT_BANG_BINARY: {
        const tree_t * lhs = (t->n > 0) ? t->c[0] : NULL;
        const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
        tree_t * callee;
        if (lhs && lhs->t == TT_VAR && lhs->v.sval && lhs->v.sval[0] != '&' && !icn_is_local(cx, lhs->v.sval)) {
            callee = ast_node_new(TT_QLIT); callee->v.sval = lhs->v.sval;
        } else callee = (tree_t *) lhs;
        tree_t * fn = ast_node_new(TT_VAR); fn->v.sval = (char *) "__apply__";
        tree_t * call = ast_node_new(TT_FNC);
        ast_push(call, fn); ast_push(call, callee); if (rhs) ast_push(call, (tree_t *) rhs);
        return lower(cx, call, γ, ω, res);
    }
    case TT_VAR: { if (t->v.sval && t->v.sval[0] == '&') return lc_key(cx, t, t->v.sval, γ, ω, res); IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: return lc_key(cx, t, t->v.sval, γ, ω, res);
    case TT_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω);
        IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * ea = lower(cx, t->c[0], nd, ω, &br); ir_operand_push(nd, br); *res = nd; return ea; }
    case TT_FNC: { const tree_t * fn = (t->n > 0) ? t->c[0] : NULL;
        if (!fn || (fn->t == TT_VAR && fn->v.sval && fn->v.sval[0] != '&' && !icn_is_local(cx, fn->v.sval))) {
            const char * nm = (fn && fn->t == TT_VAR) ? fn->v.sval : "?";
            return lower_call(cx, nm, t, 1, t->n - 1, γ, ω, res);
        }
        IR_t * cr = NULL; IR_t * ce = lower(cx, fn, NULL, ω, &cr);
        IR_t * prevβ = cx->beta;
        IR_t * nd = build(cx, IR_CALL_VALUE, γ, NULL);
        IR_t * prev = cr;
        ir_operand_push(nd, cr);
        for (int i = 1; i < t->n; i++) {
            IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[i], NULL, prevβ ? prevβ : ω, &ar); prevβ = cx->beta;
            lc_γ_to(prev, ae); prev = ar;
            ir_operand_push(nd, ar);
        }
        ω_to(nd, prevβ ? prevβ : ω);
        lc_γ_to(prev, nd);
        cx->beta = nd;
        *res = nd; return ce; }
    case TT_RANDOM: {
        IR_t * rn = build(cx, IR_RANDOM, NULL, ω);
        const tree_t * b0 = (t->n > 0) ? t->c[0] : NULL; if (!b0) { IR_t * f = build(cx, IR_FAIL, γ, ω); *res = f; return f; }
        IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, rn); ir_operand_push(rn, ar);
        IR_t * drf = build(cx, IR_DEREF, γ, ω); lc_γ_to(rn, drf); ir_operand_push(drf, rn);
        *res = drf; return ae; }
    case TT_IDX: {
        IR_t * vr = NULL; IR_t * e = lower_idx_var(cx, t, ω, &vr);
        IR_t * idxβ = cx->beta;
        IR_t * drf = build(cx, IR_DEREF, γ, idxβ ? idxβ : ω); lc_γ_to(vr, drf); ir_operand_push(drf, vr);
        *res = drf; return e; }
    case TT_MAKELIST: case TT_VLIST: return lower_make_list(cx, t, γ, ω, res);
    case TT_ASSIGN: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1];
        if (lhs && (lhs->t == TT_VAR || lhs->t == TT_KEYWORD) && lhs->v.sval && lhs->v.sval[0] == '&') {
            IR_t * ka = build(cx, IR_KEYWORD_ASSIGN, γ, ω); IR_LIT(ka).sval = lhs->v.sval;
            IR_t * vr = NULL; IR_t * entry = lower(cx, rhs, ka, ω, &vr); ir_operand_push(ka, vr); *res = ka; return entry; }
        if (lhs && lhs->t == TT_VAR) { IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = lhs->v.sval;
            IR_t * vr = NULL; IR_t * entry = lower(cx, rhs, asn, ω, &vr); ir_operand_push(asn, vr); *res = asn; return entry; }
        { IR_t * b4 = cx->beta;
          IR_t * lv = NULL; IR_t * lve = lhs ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
          if (lve && lv) {
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, asn, lvbeta ? lvbeta : ω, &rr);
            lc_γ_to(lv, re);
            ir_operand_push(asn, lv); ir_operand_push(asn, rr);
            *res = asn; return lve;
          } }
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_t * lr = NULL, * rr = NULL;
        IR_t * eb = lower(cx, rhs, asn, ω, &rr); IR_t * ea = lower(cx, lhs, eb, ω, &lr);
        ir_operand_push(asn, rr); ir_operand_push(asn, lr); *res = asn; return ea;
    }
    case TT_AUGOP: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1]; int bc = augop_code((int) t->v.ival);
        if (lhs && lhs->t == TT_VAR && rhs && icn_augop_binop_tt((int) t->v.ival) != (tree_e) 0) {
            tree_t * bo = ast_node_new(icn_augop_binop_tt((int) t->v.ival)); ast_push(bo, (tree_t *) lhs); ast_push(bo, (tree_t *) rhs);
            tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, (tree_t *) lhs); ast_push(as, bo);
            return lower(cx, as, γ, ω, res);
        }
        { IR_t * b4 = cx->beta;
          IR_t * lv = NULL; IR_t * lve = (lhs && rhs) ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
          if (lve && lv) {
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * op = build(cx, IR_BINOP, asn, ω); IR_LIT(op).ival = bc;
            IR_t * dr = build(cx, IR_DEREF, NULL, ω);
            ir_operand_push(dr, lv);
            lc_γ_to(lv, dr);
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, op, lvbeta ? lvbeta : ω, &rr);
            lc_γ_to(dr, re);
            ir_operand_push(op, dr); ir_operand_push(op, rr);
            ir_operand_push(asn, lv); ir_operand_push(asn, op);
            *res = asn; return lve;
          }
        }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = bc; IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower(cx, lhs, NULL, ω, &lr); IR_t * eb = lower(cx, rhs, op, ω, &rr); γ_to(lr, eb); *res = op; return ea;
    }
    case TT_RETURN: { IR_t * ret = build(cx, IR_RETURN, cx->psucc ? cx->psucc : γ, ω);
        if (t->n > 0 && t->c[0]) { IR_t * vr = NULL; IR_t * entry = lower(cx, t->c[0], ret, cx->pfail ? cx->pfail : ω, &vr); ir_operand_push(ret, vr); *res = ret; return entry; }
        *res = ret; return ret; }
    case TT_PROC_FAIL: { IR_t * nd = build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOOP_BREAK: { IR_t * lx = cx->loop_exit; IR_t * nd = lx ? build(cx, IR_GOTO, lx, lx) : build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOOP_NEXT: { IR_t * ln = cx->loop_next; IR_t * nd = ln ? build(cx, IR_GOTO, ln, ln) : build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOCAL: case TT_STATIC_DECL: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_INITIAL: {
        IR_t * ini = build(cx, IR_INITIAL, NULL, γ);
        if (t->n > 0 && t->c[0]) {
            IR_t * br = NULL;
            IR_t * be = lower(cx, t->c[0], γ, γ, &br);
            lc_γ_to(ini, be);
            ir_operand_push(ini, br);
        } else {
            lc_γ_to(ini, γ);
        }
        *res = ini; return ini; }
    case TT_SUSPEND: { IR_t * sn = build(cx, IR_SUSPEND, cx->psucc ? cx->psucc : γ, ω); IR_LIT(sn).dval = 1.0;
        IR_t * ev = NULL; IR_t * e_entry = sn; IR_t * eβ = NULL;
        if (t->n > 0 && t->c[0]) { e_entry = lower(cx, t->c[0], sn, cx->pfail ? cx->pfail : ω, &ev); if (is_resumable(t->c[0])) eβ = cx->beta; }
        ir_operand_push(sn, ev);
        if (t->n > 1 && t->c[1]) { IR_t * dv = NULL; IR_t * d_entry = lower(cx, t->c[1], eβ ? eβ : γ, eβ ? eβ : γ, &dv); ir_operand_push(sn, d_entry); }
        else ir_operand_push(sn, eβ ? eβ : γ);
        *res = sn; return e_entry; }
    case TT_CASE: {
        if (t->n < 1 || !t->c[0]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        static const char * CVAR = "__case_result";
        IR_t * cvar = build(cx, IR_VAR, γ, ω); IR_LIT(cvar).sval = (char *) CVAR;
        IR_t * sr = NULL; IR_t * se = lower(cx, t->c[0], NULL, ω, &sr);
        int nc = t->n - 1;
        if (nc <= 0) { cx->beta = ω; *res = cvar; return se; }
        int npairs   = nc / 2;
        int has_dflt = (nc % 2 == 1);
        IR_t * chain_next = ω;
        if (has_dflt) {
            IR_t * dv = NULL; IR_t * de = lower(cx, t->c[t->n - 1], NULL, ω, &dv);
            IR_t * dasn = build(cx, IR_ASSIGN, cvar, ω); IR_LIT(dasn).sval = (char *) CVAR;
            if (dv) ir_operand_push(dasn, dv);
            γ_to(dv ? dv : de, dasn);
            chain_next = de;
        }
        for (int i = npairs - 1; i >= 0; i--) {
            int ki = 1 + i * 2; int bi = ki + 1;
            IR_t * kn = NULL; IR_t * ke = lower(cx, t->c[ki], NULL, ω, &kn);
            IR_t * bv = NULL; IR_t * be = lower(cx, t->c[bi], NULL, ω, &bv);
            IR_t * asn = build(cx, IR_ASSIGN, cvar, ω); IR_LIT(asn).sval = (char *) CVAR;
            if (bv) ir_operand_push(asn, bv);
            γ_to(bv ? bv : be, asn);
            IR_t * idc = build(cx, IR_CALL_BUILTIN, be, chain_next);
            IR_LIT(idc).sval = (char *) "IDENTICAL";
            ir_operand_push(idc, sr);
            ir_operand_push(idc, kn);
            γ_to(kn, idc);
            chain_next = ke ? ke : idc;
        }
        γ_to(sr, chain_next);
        cx->beta = ω; *res = cvar; return se; }
    case TT_CONJ:
    case TT_SEQ_EXPR: {
        lc_vec Sv; lc_vec_init(&Sv, (int) sizeof(const tree_t *));
        for (int i = 0; i < t->n; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) s = stmt_subj(s); if (s) lc_vec_push(&Sv, &s); }
        const tree_t ** S = (const tree_t **) Sv.data; int k = Sv.n;
        if (k == 0) { IR_t * su = build(cx, IR_SUCCEED, γ, ω); *res = su; return su; }
        if (k == 1) return lower(cx, S[0], γ, ω, res);
        IR_t * SEQX = build(cx, IR_CONJUNCTION, γ, ω);
        IR_t ** val = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t ** ent = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t * succ = SEQX;
        if (t->t == TT_SEQ_EXPR) {
            IR_t * failt = ω; IR_t * last_beta = ω; IR_t * rb = NULL;
            for (int i = k - 1; i >= 0; i--) {
                val[i] = NULL; ent[i] = lower(cx, S[i], succ, failt, &val[i]); if (i == k - 1) last_beta = cx->beta; if (!rb && is_resumable(S[i])) rb = cx->beta;
                if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ); succ = ent[i]; failt = ent[i];
                if (i > 0 && ent[i] && ir_is_generator_kind(ent[i]->op)) {
                    IR_t * SENT = build(cx, IR_GOTO, NULL, NULL);
                    lc_γ_to(SENT, ent[i]); lc_ω_to(SENT, ent[i]);
                    succ = SENT; failt = SENT;
                }
            }
            if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
            cx->conj_resumable = rb; cx->beta = last_beta; *res = SEQX; return ent[0];
        }
        IR_t * last_beta = ω; IR_t * rb = NULL;
        IR_t ** bet = (IR_t **) calloc((size_t) k, sizeof(IR_t *));
        IR_t ** jn = (IR_t **) calloc((size_t) k, sizeof(IR_t *));
        for (int i = k - 1; i >= 0; i--) { val[i] = NULL; cx->beta = ω;
            IR_t * failt = ω; if (i > 0) { jn[i] = build(cx, IR_GOTO, ω, ω); failt = jn[i]; }
            ent[i] = lower(cx, S[i], succ, failt, &val[i]); bet[i] = cx->beta;
            if (i == k - 1) last_beta = cx->beta;
            if (!rb && is_resumable(S[i])) rb = cx->beta;
            if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ);
            succ = ent[i];
        }
        int lr = -1;
        for (int i = 0; i < k; i++) {
            if (i > 0 && jn[i]) {
                IR_t * tgt = ω; if (lr >= 0) tgt = (bet[lr] && bet[lr] != ω) ? bet[lr] : val[lr];
                γ_to(jn[i], tgt); ω_to(jn[i], tgt);
            }
            if (is_resumable(S[i])) lr = i;
        }
        if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
        cx->conj_resumable = rb; cx->beta = last_beta; *res = SEQX; return ent[0];
    }
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS: {
        if (t->n < 3 || !t->c[0] || !t->c[1] || !t->c[2]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * sec = build(cx, IR_SUBSCRIPT, γ, ω);
        if (sec_variant == 1) IR_LIT(sec).sval = "+";
        else if (sec_variant == 2) IR_LIT(sec).sval = "-";
        IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[0], NULL, ω, &ar); IR_t * aβ = cx->beta;
        IR_t * ωa = (aβ && aβ != ω && aβ != sec) ? aβ : ω;
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[1], NULL, ωa, &br); γ_to(ar, be); IR_t * bβ = cx->beta;
        IR_t * ωb = (bβ && bβ != ωa && bβ != sec) ? bβ : ωa;
        IR_t * cr = NULL; IR_t * ce = lower(cx, t->c[2], sec_variant ? NULL : sec, ωb, &cr); γ_to(br, ce); IR_t * cβ = cx->beta;
        IR_t * ωc = (cβ && cβ != ωb && cβ != sec) ? cβ : ωb;
        if (sec_variant) {
            IR_t * op = build(cx, IR_BINOP, sec, ωc); IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB;
            ir_operand_push(op, br); ir_operand_push(op, cr);
            γ_to(cr, op); cr = op;
        }
        ir_operand_push(sec, ar);
        ir_operand_push(sec, br);
        ir_operand_push(sec, cr);
        if (ωc != ω) ω_to(sec, ωc);
        cx->beta = ωc; *res = sec; return ae; }
    case TT_NOT: return lower_not(cx, t, γ, ω, res);
    case TT_ALTERNATE: return lower_alt(cx, t, γ, ω, res);
    case TT_ITERATE: {
        IR_t * nd = build(cx, IR_ITERATE, γ, ω);
        IR_t * orr = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &orr);
        ir_operand_push(nd, orr);
        lc_γ_to(orr, nd);
        cx->beta = nd; *res = nd; return ee; }
    case TT_IF: return lower_if(cx, t, γ, ω, res);
    case TT_WHILE: return lower_while(cx, t, γ, ω, res);
    case TT_UNTIL: return lower_until(cx, t, γ, ω, res);
    case TT_REPEAT: return lower_repeat(cx, t, γ, ω, res);
    case TT_TO: case TT_TO_BY: return lower_to(cx, t, γ, ω, res);
    case TT_EVERY: return lower_every(cx, t, γ, ω, res);
    case TT_SCAN: {
        if (t->n < 2 || !t->c[0] || !t->c[1]) {
            IR_t * gs = build(cx, IR_FAIL, γ, ω); *res = gs; return gs;
        }
        IR_t * enter = build(cx, IR_SCAN_ENTER, NULL, ω);
        IR_t * leave_succ = build(cx, IR_SCAN, γ, ω);
        IR_t * leave_fail = build(cx, IR_SCAN, ω, ω);
        ir_operand_push(leave_succ, enter);
        ir_operand_push(leave_fail, enter);
        IR_t * bv = NULL; IR_t * b_entry = lower(cx, t->c[1], leave_succ, leave_fail, &bv);
        if (bv) ir_operand_push(leave_succ, bv);
        icn_retag_scan_body(cx->g, 0);
        lc_γ_to(enter, b_entry);
        cx->beta = ω;
        IR_t * sr = NULL; IR_t * s_entry = lower(cx, t->c[0], enter, ω, &sr);
        ir_operand_push(enter, sr);
        IR_t * subj_beta = cx->beta;
        if (subj_beta && subj_beta != ω) { γ_to(leave_fail, subj_beta); ω_to(leave_fail, subj_beta); }
        cx->beta = (subj_beta && subj_beta != ω) ? subj_beta : ω; *res = leave_succ; return s_entry; }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); if (sub) return lower(cx, sub, γ, ω, res); IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_CREATE: {
        IR_t * nd = build(cx, IR_CREATE, γ, ω);
        IR_t * coret = build(cx, IR_CORET, NULL, NULL);
        IR_t * cofail = build(cx, IR_COFAIL, NULL, NULL);
        IR_t * bv = NULL; IR_t * b_entry = lower(cx, (t->n > 0) ? t->c[0] : NULL, coret, cofail, &bv);
        IR_t * body_beta = cx->beta;
        if (body_beta) γ_to(coret, body_beta);
        ir_operand_push(coret, bv);
        ir_operand_push(nd, b_entry);
        cx->beta = ω; *res = nd; return nd; }
    case TT_ACTIVATE: {
        IR_t * nd = build(cx, IR_ACTIVATE, γ, ω);
        const tree_t * xt = (t->n > 1) ? t->c[0] : NULL;
        const tree_t * ct = (t->n > 1) ? t->c[1] : t->c[0];
        IR_t * cr = NULL; IR_t * c_entry = lower(cx, ct, nd, ω, &cr);
        IR_t * entry = c_entry;
        ir_operand_push(nd, cr);
        if (xt) { IR_t * xr = NULL; entry = lower(cx, xt, c_entry, ω, &xr); ir_operand_push(nd, xr); }
        cx->beta = ω; *res = nd; return entry; }
    case TT_REPALT: {
        IR_t * nd = build(cx, IR_REPALT, γ, ω);
        lc_γ_to(nd, γ);
        IR_t * er = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &er);
        ir_operand_push(nd, er);
        ir_operand_push(nd, ee);
        cx->beta = nd; *res = nd; return nd; }
    case TT_LIMIT: {
        IR_t * lim = build(cx, IR_LIMIT, γ, ω);
        IR_t * lr = NULL; IR_t * ee = lower(cx, (t->n > 1) ? t->c[1] : NULL, lim, ω, &lr);
        IR_t * inner_beta = cx->beta;
        IR_t * er = NULL; IR_t * ge = lower(cx, (t->n > 0) ? t->c[0] : NULL, lim, ω, &er);
        IR_t * gen_beta = cx->beta;
        ir_operand_push(lim, er);
        ir_operand_push(lim, lr);
        γ_to(lr, ge);
        if (lr && lr->γ.node == ge) lc_γ_to(lr, ge);
        if (er) lc_γ_to(er, lim);
        (void)inner_beta; cx->beta = gen_beta;
        *res = lim; return ee; }
    case TT_LCONCAT: {
        if (t->n < 2 || !t->c[0] || !t->c[1]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = BINOP_CONCAT;
        IR_t * lr = NULL; IR_t * ee = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
        IR_t * rr = NULL; IR_t * re = lower(cx, t->c[1], op, lβ, &rr); γ_to(lr, re);
        ir_operand_push(op, lr); ir_operand_push(op, rr);
        *res = op; return ee; }
    case TT_SWAP: {
        const tree_t * lt = (t->n > 0) ? t->c[0] : NULL; const tree_t * rt2 = (t->n > 1) ? t->c[1] : NULL;
        int plain_l = lt && lt->t == TT_VAR && lt->v.sval && lt->v.sval[0] != '&';
        int plain_r = rt2 && rt2->t == TT_VAR && rt2->v.sval && rt2->v.sval[0] != '&';
        int kw_l = lt && (lt->t == TT_VAR || lt->t == TT_KEYWORD) && lt->v.sval && lt->v.sval[0] == '&';
        int kw_r = rt2 && (rt2->t == TT_VAR || rt2->t == TT_KEYWORD) && rt2->v.sval && rt2->v.sval[0] == '&';
        if (kw_l && kw_r) {
            /* kw <-> kw (&pos :=: &subject): both reads via IR_KEYWORD_ICON, both writes via
             * IR_KEYWORD_ASSIGN, canonical oasgn.r order lhs := rhs_old first, then rhs := lhs_old. */
            IR_t * lv_old = build(cx, IR_KEYWORD_ICON, NULL, ω); IR_LIT(lv_old).sval = (char *) lt->v.sval;
            IR_t * rv_old = build(cx, IR_KEYWORD_ICON, NULL, ω); IR_LIT(rv_old).sval = (char *) rt2->v.sval;
            lc_γ_to(lv_old, rv_old);
            IR_t * write_l = build(cx, IR_KEYWORD_ASSIGN, NULL, ω); IR_LIT(write_l).sval = (char *) lt->v.sval;  ir_operand_push(write_l, rv_old);
            IR_t * write_r = build(cx, IR_KEYWORD_ASSIGN, γ,    ω); IR_LIT(write_r).sval = (char *) rt2->v.sval; ir_operand_push(write_r, lv_old);
            lc_γ_to(rv_old, write_l); lc_γ_to(write_l, write_r);
            *res = write_r; return lv_old;
        }
        if (kw_l || kw_r) {
            /* Keyword operand: emit sequential read-old/write-new per canonical oasgn.r :=: swap.
             * Canonical order from oasgn.r: lhs := rhs_old FIRST, then rhs := lhs_old.
             * kw_l (&pos :=: x): &pos := x_old first (fails OOB -> both unchanged); then x := &pos_old.
             * kw_r (x :=: &pos): x := &pos_old first (always succeeds); then &pos := x_old (fails OOB -> x updated, &pos not). */
            IR_t * kv_old = build(cx, IR_KEYWORD_ICON, NULL, ω);
            IR_t * pv_old = build(cx, IR_VAR, NULL, ω);
            const tree_t * kw_tree = kw_l ? lt : rt2;
            const tree_t * pl_tree = kw_l ? rt2 : lt;
            IR_LIT(kv_old).sval = (char *) kw_tree->v.sval;
            IR_LIT(pv_old).sval = (char *) pl_tree->v.sval;
            lc_γ_to(kv_old, pv_old);
            if (kw_l) {
                /* lhs=kw: write kw := plain_old first (can fail), then write plain := kw_old */
                IR_t * write_kw    = build(cx, IR_KEYWORD_ASSIGN, NULL, ω); IR_LIT(write_kw).sval    = kw_tree->v.sval; ir_operand_push(write_kw, pv_old);
                IR_t * write_plain = build(cx, IR_ASSIGN,          γ,    ω); IR_LIT(write_plain).sval = pl_tree->v.sval; ir_operand_push(write_plain, kv_old);
                lc_γ_to(pv_old, write_kw); lc_γ_to(write_kw, write_plain);
                *res = write_plain; return kv_old;
            } else {
                /* lhs=plain: write plain := kw_old first (always ok), then write kw := plain_old (can fail) */
                IR_t * write_plain = build(cx, IR_ASSIGN,          NULL, ω); IR_LIT(write_plain).sval = pl_tree->v.sval; ir_operand_push(write_plain, kv_old);
                IR_t * write_kw    = build(cx, IR_KEYWORD_ASSIGN,  γ,    ω); IR_LIT(write_kw).sval    = kw_tree->v.sval; ir_operand_push(write_kw, pv_old);
                lc_γ_to(pv_old, write_plain); lc_γ_to(write_plain, write_kw);
                *res = write_kw; return kv_old;
            } }
        if (!(plain_l && plain_r)) {
            IR_t * xr = NULL; IR_t * xe = lower_lvalue_var(cx, lt, ω, &xr);
            IR_t * yr = NULL; IR_t * ye = xe ? lower_lvalue_var(cx, rt2, ω, &yr) : NULL;
            if (xe && ye) {
                lc_γ_to(xr, ye);
                IR_t * nd = build(cx, IR_SWAP_VAR, γ, ω);
                lc_γ_to(yr, nd);
                ir_operand_push(nd, xr); ir_operand_push(nd, yr);
                *res = nd; return xe;
            }
        }
        IR_t * nd = build(cx, IR_SWAP, γ, ω);
        IR_t * lr = NULL; lower(cx, lt, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rt2, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_REVASSIGN: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
        if (lhs && (lhs->t == TT_IDX || lhs->t == TT_ITERATE || lhs->t == TT_SECTION || lhs->t == TT_SECTION_PLUS || lhs->t == TT_SECTION_MINUS || lhs->t == TT_FIELD || lhs->t == TT_RANDOM)) {
            IR_t * b4 = cx->beta;
            IR_t * vr = NULL; IR_t * entry = lower_lvalue_var(cx, lhs, ω, &vr);
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * nd = build(cx, IR_REV_ASSIGN_VAR, γ, lvbeta ? lvbeta : ω);
            IR_t * b5 = cx->beta;
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, NULL, lvbeta ? lvbeta : ω, &rr);
            IR_t * rbeta = (cx->beta != b5) ? cx->beta : NULL;
            if (rbeta) ω_to(nd, rbeta);
            lc_γ_to(vr, re);
            lc_γ_to(rr, nd);
            ir_operand_push(nd, vr);
            ir_operand_push(nd, rr);
            cx->beta = nd; *res = nd; return entry;
        }
        if (lhs && lhs->t == TT_VAR) {
            IR_t * nd = build(cx, IR_REV_ASSIGN, γ, ω);
            IR_t * lr = NULL; IR_t * le = lower(cx, lhs, NULL, ω, &lr);
            IR_t * b4 = cx->beta;
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, NULL, ω, &rr);
            IR_t * rbeta = (cx->beta != b4) ? cx->beta : NULL;
            if (rbeta) ω_to(nd, rbeta);
            γ_to(lr, re);
            lc_γ_to(rr, nd);
            ir_operand_push(nd, rr);
            ir_operand_push(nd, lr);
            cx->beta = nd; *res = nd; return le;
        }
        IR_t * nd = build(cx, IR_FAIL, γ, ω);
        IR_t * lr = NULL; lower(cx, lhs, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rhs, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_REVSWAP: {
        const tree_t * lt = (t->n > 0) ? t->c[0] : NULL; const tree_t * rt2 = (t->n > 1) ? t->c[1] : NULL;
        int name_l = lt && (lt->t == TT_VAR || lt->t == TT_KEYWORD) && lt->v.sval;
        int name_r = rt2 && (rt2->t == TT_VAR || rt2->t == TT_KEYWORD) && rt2->v.sval;
        if (name_l && name_r) {
            /* x <-> y (oasgn.r rswap): ONE box; alpha = save both olds + forward swap in canonical order
             * (lhs := rhs_old first, fail -> omega with rhs untouched; then rhs := lhs_old, fail -> omega
             * with lhs committed); beta = restore lhs first (fail -> omega skipping rhs), then rhs, omega.
             * rhs name rides a dangling IR_LIT_STRING carrier (operands[0], control-unreachable, data only). */
            IR_t * nd = build(cx, IR_REV_SWAP, γ, ω); IR_LIT(nd).sval = (char *) lt->v.sval;
            IR_t * rc = build(cx, IR_LIT_STRING, NULL, NULL); IR_LIT(rc).sval = (char *) rt2->v.sval;
            ir_operand_push(nd, rc);
            cx->beta = nd; *res = nd; return nd;
        }
        IR_t * nd = build(cx, IR_FAIL, γ, ω);
        IR_t * lr = NULL; lower(cx, lt, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rt2, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_RECORD: {
        extern void record_register(const char *spec);
        const char * rname = t->v.sval;
        if (rname) {
            char spec[2048]; int pos = 0;
            pos += snprintf(spec + pos, sizeof spec - pos, "%s(", rname);
            for (int k = 0; k < t->n; k++) {
                if (k) pos += snprintf(spec + pos, sizeof spec - pos, ",");
                if (t->c[k] && t->c[k]->v.sval) pos += snprintf(spec + pos, sizeof spec - pos, "%s", t->c[k]->v.sval);
            }
            snprintf(spec + pos, sizeof spec - pos, ")");
            record_register(spec);
        }
        IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_INVOCABLE:
    case TT_LINK:
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * W = build(cx, IR_GOTO, γ, ω); γ_to(W, γ); ω_to(W, γ);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, NULL, W, &cval);
    cx->loop_next = centry;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, centry, centry, &bval);
    lc_γ_to(cval, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln;
    *res = W; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * U = build(cx, IR_GOTO, γ, ω); γ_to(U, γ); ω_to(U, γ);
    IR_t * BENT = build(cx, IR_GOTO, γ, ω);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, U, BENT, &cval);
    cx->loop_next = centry;
    IR_t * b_entry; if (B) { IR_t * bval = NULL; b_entry = lower(cx, B, centry, centry, &bval); } else b_entry = centry;
    lc_γ_to(BENT, b_entry); lc_ω_to(BENT, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln;
    *res = U; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
    IR_t * H = build(cx, IR_GOTO, NULL, ω);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ; cx->loop_next = H;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, H, H, &bval);
    lc_γ_to(H, b_entry); lc_ω_to(H, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln;
    cx->beta = γ; *res = H; return H;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nullv = build(cx, IR_VAR, γ, ω); IR_LIT(nullv).sval = (char *) "&null";
    IR_t * cr = NULL; IR_t * ce = lower(cx, (t->n > 0) ? t->c[0] : NULL, ω, nullv, &cr);
    cx->beta = ω; *res = nullv; return ce;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int n = t->n; if (n < 1) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    IR_t * dj = build(cx, IR_DISJUNCTION, γ, ω);
    IR_t ** entry = (IR_t **) calloc((size_t) n, sizeof(IR_t *));
    for (int j = n - 1; j >= 0; j--) {
        IR_t * ωj = (j + 1 < n) ? entry[j + 1] : ω;
        IR_t * ml = build(cx, IR_MOVE_LABEL, γ, ω);
        cx->beta = ωj;
        IR_t * ar = NULL; entry[j] = lower(cx, t->c[j], ml, ωj, &ar);
        IR_t * ab = cx->beta ? cx->beta : ωj;
        IR_LIT(ml).ival = (ab && ir_is_generator_kind(ab->op)) ? 1 : 0;
        ir_operand_push(ml, ab); ir_operand_push(ml, dj); ir_operand_push(ml, ar);
    }
    cx->beta = dj; *res = dj; return entry[0];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * TH = (t->n > 1) ? t->c[1] : NULL; const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
    IR_t * ig = build(cx, IR_INDIRECT_GOTO, γ, ω);
    IR_t * ml_th = build(cx, IR_MOVE_LABEL, γ, ω);
    cx->beta = ω;
    IR_t * then_val = NULL; IR_t * then_entry = lower(cx, TH, ml_th, ω, &then_val);
    IR_t * ab_th = cx->beta ? cx->beta : ω;
    IR_LIT(ml_th).ival = (ab_th && ir_is_generator_kind(ab_th->op)) ? 1 : 0;
    ir_operand_push(ml_th, ab_th); ir_operand_push(ml_th, ig); ir_operand_push(ml_th, then_val);
    IR_t * else_entry;
    if (EL) {
        IR_t * ml_el = build(cx, IR_MOVE_LABEL, γ, ω);
        cx->beta = ω;
        IR_t * else_val = NULL; else_entry = lower(cx, EL, ml_el, ω, &else_val);
        IR_t * ab_el = cx->beta ? cx->beta : ω;
        IR_LIT(ml_el).ival = (ab_el && ir_is_generator_kind(ab_el->op)) ? 1 : 0;
        ir_operand_push(ml_el, ab_el); ir_operand_push(ml_el, ig); ir_operand_push(ml_el, else_val);
    } else { else_entry = ω; }
    IR_t * cond_val = NULL; IR_t * cond_entry = lower(cx, C, then_entry, else_entry, &cond_val); (void) cond_val;
    cx->beta = ig; *res = ig; return cond_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_const_step(const tree_t * s, int64_t * bits, int * isr) {
    if (!s) return 0;
    if (s->t == TT_ILIT) { *bits = s->v.ival; *isr = 0; return 1; }
    if (s->t == TT_FLIT) { double d = s->v.dval; memcpy(bits, &d, 8); *isr = 1; return 1; }
    if ((s->t == TT_MNS || s->t == TT_PLS) && s->n >= 1 && s->c[0]) {
        if (!icn_const_step(s->c[0], bits, isr)) return 0;
        if (s->t == TT_MNS) { if (*isr) { double d; memcpy(&d, bits, 8); d = -d; memcpy(bits, &d, 8); } else *bits = -(*bits); }
        return 1;
    }
    if (s->t == TT_POW && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0; int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            if (rb >= 0) { int64_t acc = 1; for (int64_t k = 0; k < rb; k++) acc *= lb; *bits = acc; *isr = 0; return 1; }
            if (lb == 1)  { *bits = 1; *isr = 0; return 1; }
            if (lb == -1) { *bits = (rb & 1) ? -1 : 1; *isr = 0; return 1; }
            if (lb == 0)  return 0;
            *bits = 0; *isr = 0; return 1;
        }
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        rv = pow(la, ra); memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    if ((s->t == TT_ADD || s->t == TT_SUB || s->t == TT_MUL || s->t == TT_DIV || s->t == TT_MOD) && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0; int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            int64_t r;
            if (s->t == TT_ADD) r = lb + rb;
            else if (s->t == TT_SUB) r = lb - rb;
            else if (s->t == TT_MUL) r = lb * rb;
            else if (s->t == TT_DIV) { if (rb == 0) return 0; r = lb / rb; }
            else { if (rb == 0) return 0; r = lb % rb; }
            *bits = r; *isr = 0; return 1;
        }
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        if (s->t == TT_ADD) rv = la + ra; else if (s->t == TT_SUB) rv = la - ra; else if (s->t == TT_MUL) rv = la * ra; else if (s->t == TT_DIV) {
            if (ra == 0.0) return 0; rv = la / ra;
        } else { if (ra == 0.0) return 0; rv = fmod(la, ra); }
        memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    int64_t by = 1; const tree_t * byt = (nargs > 1) ? t->c[argbase + 1] : NULL;
    if (byt) { int64_t bb = 1; int isr = 0; if (!icn_const_step(byt, &bb, &isr) || isr) return NULL; by = bb; }
    if (by != 1) return NULL;
    IR_t * to = build(cx, IR_TO, γ, ω); IR_LIT(to).sval = (char *) "ag"; cx->last_gen = to;
    const tree_t * fromt = (nargs > 0) ? t->c[argbase] : NULL; IR_t * lr; IR_t * ea; IR_t * lβ;
    if (fromt) { ea = lower(cx, fromt, NULL, ω, &lr); lβ = cx->beta; } else { lr = build(cx, IR_LIT_INTEGER, NULL, ω); IR_LIT(lr).ival = 1; ea = lr; lβ = ω; }
    IR_t * mr = build(cx, IR_LIT_INTEGER, to, lβ); IR_LIT(mr).ival = (by > 0) ? INT64_MAX : INT64_MIN; γ_to(lr, mr);
    ir_operand_push(to, lr); ir_operand_push(to, mr); cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) nargs;
    /* key(t) is a generator: yield each key of table t in turn.  Reuse the
     * proven IR_ITERATE (unary-bang) Byrd box, tagged with the "key" variant
     * so the box calls rt_list_bang_key_at (keys) instead of rt_list_bang_at
     * (values).  The operand's entry (ee) is the box entry; wire operand-γ
     * into the generator, and leave cx->beta at the generator so backtracking
     * resumes it for the next key.  (Previously this was a bare IR_FAIL stub,
     * so key() always failed and every table-key iteration produced nothing —
     * silently breaking tgrlink/ipxref/rsg/geddump output.) */
    IR_t * kg = build(cx, IR_ITERATE, γ, ω);
    IR_LIT(kg).sval = "key";
    IR_t * orr = NULL; IR_t * ee = lower(cx, t->c[argbase], NULL, ω, &orr);
    ir_operand_push(kg, orr);
    lc_γ_to(orr, kg);
    cx->beta = kg; *res = kg; return ee;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * ml = build(cx, IR_MAKE_LIST, γ, ω);
    if (res) *res = ml;
    IR_t * prev = NULL; IR_t * entry = ml; IR_t * aω = ω;
    for (int k = 0; k < t->n; k++) {
        const tree_t * a = t->c[k]; IR_t * ar = NULL;
        IR_t * ae = lower(cx, a, (k == t->n - 1) ? ml : NULL, aω, &ar); aω = cx->beta;
        if (k == 0) entry = ae;
        if (prev) γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(ml, ar);
    }
    cx->beta = g_postfix_resume ? aω : ω;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int by = (t->t == TT_TO_BY && t->n > 2 && t->c[2]) ? 1 : 0;
    IR_t * to = build(cx, by ? IR_TO_BY : IR_TO, γ, ω); IR_LIT(to).sval = (char *) "ag"; cx->last_gen = to;
    IR_t * lr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
    IR_t * mr = NULL; IR_t * em = lower(cx, t->c[1], by ? NULL : to, lβ, &mr); γ_to(lr, em);
    ir_operand_push(to, lr); ir_operand_push(to, mr); IR_t * last_op = mr;
    if (by) {
        IR_t * mβ = cx->beta; IR_t * br = NULL; IR_t * eb = lower(cx, t->c[2], to, mβ, &br); γ_to(mr, eb); (void) eb;
        if (br && ir_is_generator_kind(to->op)) lc_γ_to(br, to);
        ir_operand_push(to, br); last_op = br;
        if ((lr && lr->op == IR_LIT_REAL) || (mr && mr->op == IR_LIT_REAL) || (br && br->op == IR_LIT_REAL)) IR_LIT(to).sval = (char *) "ar";
    } else if (mr && ir_is_generator_kind(to->op)) lc_γ_to(mr, to);
    { IR_t * resume_op = NULL;
      if (by && last_op && ir_is_generator_kind(last_op->op)) resume_op = last_op;
      else if (mr && ir_is_generator_kind(mr->op)) resume_op = mr;
      else if (lr && ir_is_generator_kind(lr->op)) resume_op = lr;
      if (resume_op) lc_ω_to_β(to, resume_op); } /* range-exhausted resumes the RIGHTMOST GENERATOR operand (right-to-left over by/hi/lo), re-pumping it for a fresh bound; operand-fail edges already cascade leftward (mid ω=lβ, by ω=mβ), so resuming any operand replays the full cross-product; all-literal bounds leave ω at the threaded caller edge — extends the prior rightmost-only wiring to generator LOWER bounds ((1 to 2) to 3) without disturbing the literal-bound every-loop exit */
    cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) γ;
    const tree_t * E = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * eval = NULL; IR_t * e_entry = lower(cx, E, NULL, ω, &eval); IR_t * gen_beta = cx->beta;
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = ω; cx->loop_next = gen_beta;
    IR_t * bval = NULL; (void) bval; IR_t * b_entry;
    if (B) { b_entry = lower(cx, B, gen_beta, gen_beta, &bval); }
    else { b_entry = build(cx, IR_GOTO, gen_beta, gen_beta); }
    cx->loop_exit = sle; cx->loop_next = sln;
    γ_to(eval, b_entry);
    cx->beta = ω; *res = eval ? eval : e_entry; return e_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_proc_body(icx_t * cx, const tree_t * body) {
    IR_graph_t * g = IR_alloc(8192); cx->g = g;
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->psucc = PSUCC; cx->pfail = PFAIL;
    IR_t * succ = PFAIL; IR_t * fail = PFAIL;
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t * s = body->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; } if (!s) continue;
        IR_t * r = NULL; IR_t * entry = lower(cx, s, succ, fail, &r); if (r && r->γ.node == succ) lc_γ_to(r, succ);
        /* STMT-BOUNDARY α-FORCE: the next statement (source order) reaches this one's entry as a
         * fresh evaluation, never a resume. If that entry is generator-kind, build()'s auto-β stamp
         * (lc_ω_to_β/lc_γ_to_β) would make a subsequent statement's fail/success edge land on its β
         * (resume-and-fail) label, skipping the statement body. Interpose an α-stamped GOTO so the
         * cross-statement edge enters at α. (Same trampoline idiom as lower_while/until/repeat.) */
        if (entry && ir_is_generator_kind(entry->op)) {
            IR_t * tramp = IR_node_alloc(g, IR_GOTO); lc_γ_to(tramp, entry); lc_ω_to(tramp, entry);
            entry = tramp;
        }
        succ = entry; fail = entry;
    }
    g->entry = succ; return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_procs_vec(const tree_t * t, lc_vec * out) {
    if (!t) return;
    if (t->t == TT_STMT) { collect_procs_vec(stmt_subj(t), out); return; }
    if (t->t == TT_PROC_DECL) { lc_vec_push(out, &t); return; }
    for (int i = 0; i < t->n; i++) collect_procs_vec(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fill_pnames(const tree_t * prog, lc_vec * pn) {
    lc_vec ps; lc_vec_init(&ps, (int) sizeof(const tree_t *));
    collect_procs_vec(prog, &ps);
    for (int i = 0; i < ps.n; i++) if (LC_AT(&ps, const tree_t *, i)->v.sval) lc_vec_push(pn, &LC_AT(&ps, const tree_t *, i)->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_rename_statics_walk(tree_t * n, const char ** names, char ** mangled, int cnt) {
    if (!n) return;
    if (n->t == TT_PROC_DECL || n->t == TT_STATIC_DECL) return;
    if (n->t == TT_VAR && n->v.sval) { for (int k = 0; k < cnt; k++) if (!strcmp(n->v.sval, names[k])) { n->v.sval = mangled[k]; return; } }
    for (int i = 0; i < n->n; i++) icn_rename_statics_walk(n->c[i], names, mangled, cnt);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_statics_prepass(tree_t * body, const char * pname) {
    if (!body) return;
    const char * names[64]; char * mangled[64]; int cnt = 0; int inits = 0;
    for (int i = 0; i < body->n; i++) {
        tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = (tree_t *) stmt_subj(st);
        if (st && st->t == TT_STATIC_DECL)
            for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval && cnt < 64) {
                names[cnt] = st->c[k]->v.sval;
                char * m = malloc(strlen(pname) + strlen(names[cnt]) + 12); sprintf(m, "%s__STATIC__%s", pname, names[cnt]);
                { extern void global_register(const char *); global_register(m); }
                mangled[cnt] = m; cnt++;
            }
    }
    if (cnt) icn_rename_statics_walk(body, names, mangled, cnt);
    for (int i = 0; i < body->n; i++) {
        tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = (tree_t *) stmt_subj(st);
        if (st && st->t == TT_INITIAL) {
            char * f = malloc(strlen(pname) + 20); sprintf(f, "%s__INITFLAG__%d", pname, inits++);
            { extern void global_register(const char *); global_register(f); }
            tree_t * fv = ast_node_new(TT_VAR); fv->v.sval = f;
            tree_t * nz = ast_node_new(TT_NULL); ast_push(nz, fv);
            tree_t * one = ast_node_new(TT_ILIT); one->v.ival = 1;
            tree_t * asn = ast_node_new(TT_ASSIGN); ast_push(asn, nz); ast_push(asn, one);
            tree_t * child = (st->n > 0) ? st->c[0] : NULL;
            st->t = TT_IF; st->n = 0;
            ast_push(st, asn);
            if (child) ast_push(st, child); else { tree_t * s1 = ast_node_new(TT_ILIT); s1->v.ival = 1; ast_push(st, s1); }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_icon_proc(const tree_t * prog, const tree_t * pd) {
    static lc_vec pnv; lc_vec_init(&pnv, (int) sizeof(const char *)); fill_pnames(prog, &pnv);
    icx_t cx; memset(&cx, 0, sizeof cx); cx.pn = (const char **) pnv.data; cx.npn = pnv.n;
    static lc_vec lnv; lc_vec_init(&lnv, (int) sizeof(const char *)); lnv.n = 0;
    if (pd) {
        const tree_t * plist = (pd->n > 1) ? pd->c[1] : NULL;
        for (int i = 0; plist && i < plist->n; i++) if (plist->c[i] && plist->c[i]->v.sval) lc_vec_push(&lnv, &plist->c[i]->v.sval);
        const tree_t * body = (pd->n > 2) ? pd->c[2] : NULL;
        icn_statics_prepass((tree_t *) body, pd->v.sval ? pd->v.sval : "anon");
        for (int i = 0; body && i < body->n; i++) {
            const tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = stmt_subj(st);
            if (st && st->t == TT_LOCAL)
                for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) lc_vec_push(&lnv, &st->c[k]->v.sval);
        }
    }
    cx.ln = (const char **) lnv.data; cx.nln = lnv.n;
    if (pd && pd->n > 2 && pd->c[2]) return lower_proc_body(&cx, pd->c[2]);
    IR_graph_t * g = IR_alloc(64); cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s; return g;
}
#include "bb_program.h"
#include "emit.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_subtree_has_suspend(const tree_t *n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_PROC_DECL || n->t == TT_SUB_DECL) return 0;
    for (int i = 0; i < n->n; i++) if (icn_subtree_has_suspend(n->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_body_has_suspend(const tree_t *proc) {
    if (!proc) return 0;
    for (int i = 0; i < proc->n; i++) if (icn_subtree_has_suspend(proc->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_icon_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_icon_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
void lower_icon_resolve_call_kinds(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icon_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    extern void record_register(const char * spec);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * proc = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!proc) continue;
        if (proc->t == TT_GLOBAL) {
            for (int _gi = 0; _gi < proc->n; _gi++)
                if (proc->c[_gi] && proc->c[_gi]->v.sval)
                    global_register(proc->c[_gi]->v.sval);
        }
        if (proc->t == TT_RECORD && proc->v.sval && *proc->v.sval) {
            char spec[256]; int pos = 0;
            pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", proc->v.sval);
            for (int _ri = 0; _ri < proc->n && pos < (int)sizeof(spec)-2; _ri++) {
                if (_ri > 0) spec[pos++] = ',';
                const char *fn2 = (proc->c[_ri] && proc->c[_ri]->v.sval) ? proc->c[_ri]->v.sval : "";
                pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
            }
            if (pos < (int)sizeof(spec)-1) spec[pos++] = ')';
            spec[pos] = '\0';
            record_register(spec);
        }
        if (proc->t == TT_FNC || proc->t == TT_PROC_DECL || proc->t == TT_SUB_DECL) {
            const char *name = NULL;
            if (proc->t == TT_SUB_DECL) {
                if (proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) name = proc->c[0]->v.sval;
            } else {
                name = (proc->v.sval && *proc->v.sval) ? proc->v.sval : ((proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) ? proc->c[0]->v.sval : NULL);
            }
            if (name) {
                int _pi = stage2_proc_grow(s2);
                s2->proc_table[_pi].name     = name;
                s2->proc_table[_pi].proc     = proc;
                s2->proc_table[_pi].entry_pc = -1;
                s2->proc_table[_pi].bb_idx   = -1;
                s2->proc_table[_pi].nparams  = (proc->t == TT_PROC_DECL && proc->n >= 2 ? proc->c[1]->n : 0);
                s2->proc_table[_pi].is_variadic = (proc->t == TT_PROC_DECL && proc->n >= 2 && proc->c[1] ? (int)proc->c[1]->v.ival : 0);
                s2->proc_table[_pi].byref_mask = 0;
                if (mod_idx >= 0) s2->module_registry.mods[mod_idx].nprocs++;
                if (strcmp(name, "main") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_icon_stage2(const tree_t *prog) {
    icon_register_program(&g_stage2, prog);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        g_stage2.proc_table[pi].is_generator = icn_body_has_suspend(proc);
    }
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_icon_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            g_stage2.bbp.table[bb_idx]->nparams = g_stage2.proc_table[pi].nparams;
            const tree_t *plist = (proc->n >= 2) ? proc->c[1] : NULL;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            for (int k = 0; k < np && plist && k < plist->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                const tree_t *pv = plist->c[k];
                if (!pv || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n;
                sc->n++;
            }
            if (np > 0) {
                const char ** _pn = (const char **)calloc((size_t)np, sizeof(const char *));
                if (_pn) { for (int k = 0; k < np && k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
            { const tree_t *body = (proc->n > 2) ? proc->c[2] : NULL; int nl = 0;
              for (int i = 0; body && i < body->n; i++) { const tree_t *st = body->c[i]; if (st && st->t == TT_STMT) st = stmt_subj(st);
                  if (st && st->t == TT_LOCAL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) nl++; }
              if (nl > 0) { const char ** _ln = (const char **)calloc((size_t)nl, sizeof(const char *)); int w = 0;
                  for (int i = 0; body && i < body->n && _ln; i++) { const tree_t *st = body->c[i]; if (st && st->t == TT_STMT) st = stmt_subj(st);
                      if (st && st->t == TT_LOCAL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) _ln[w++] = lp_strdup(st->c[k]->v.sval); }
                  if (_ln) { g_stage2.bbp.table[bb_idx]->lnames = _ln; g_stage2.bbp.table[bb_idx]->nlocals = w; } } }
        }
    }
    lower_icon_resolve_call_kinds();
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_callable_proc_index(const char * fn) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const char * pname = g_stage2.proc_table[pi].name;
        if (!pname || strcmp(pname, fn) != 0) continue;
        if (strcmp(pname, "main") == 0) return -1;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) return -1;
        return pi;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int g_icon_write_reassignable = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_scan_write_reassignable(void) {
    g_icon_write_reassignable = 0;
    for (int gi = 0; gi < g_stage2.bbp.count && !g_icon_write_reassignable; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd) continue;
            const char * tgt = 0;
            if (nd->op == IR_ASSIGN) tgt = IR_LIT(nd).sval;
            else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) tgt = IR_LIT(nd->operands[1]).sval;
            if (tgt && (!strcmp(tgt, "write") || !strcmp(tgt, "writes"))) { g_icon_write_reassignable = 1; break; }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_icon_resolve_call_kinds(void) {
    extern int rt_builtin_is_generator(const char *);
    extern int rt_builtin_is_known(const char *);
    extern int is_global(const char *);
    icn_scan_write_reassignable();
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_CALL) continue;
            const char * fn = IR_LIT(nd).sval;
            if (!fn || !fn[0]) continue;
            int pi = icn_callable_proc_index(fn);
            if (pi >= 0 && g_stage2.proc_table[pi].is_generator) nd->op = IR_PROC_GEN;
            else if (pi >= 0) nd->op = IR_CALL_PROC_STAGED;
            else if (icn_builtin_is_generator(fn)) nd->op = IR_CALL_BUILTIN_ICON;
            else if (icn_builtin_is_known(fn)) nd->op = IR_CALL_BUILTIN_ICON;
        }
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_VAR) continue;
            const char * vn = IR_LIT(nd).sval;
            if (!vn || !vn[0] || vn[0] == '&') continue;
            if (is_global(vn)) continue;
            int skip = 0;
            for (int k = 0; !skip && g->pnames && k < g->nparams; k++) if (g->pnames[k] && !strcmp(g->pnames[k], vn)) skip = 1;
            for (int k = 0; !skip && k < g->n; k++) {
                IR_t * a = g->all[k]; if (!a) continue;
                if (a->op == IR_ASSIGN && IR_LIT(a).sval && !strcmp(IR_LIT(a).sval, vn)) skip = 1;
                if (a->op == IR_REV_ASSIGN && a->n_operands > 1 && a->operands[1] && IR_LIT(a->operands[1]).sval && !strcmp(IR_LIT(a->operands[1]).sval, vn)) skip = 1;
            }
            if (skip) continue;
            int isproc = 0;
            for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, vn)) { isproc = 1; break; }
            if (isproc || rt_builtin_is_known(vn) || rt_builtin_is_generator(vn) || !strcmp(vn, "push") || !strcmp(vn, "put")) nd->op = IR_PROC_VALUE;
        }
    }
}
