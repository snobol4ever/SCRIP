#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lower.h"
/*====================================================================================================================================================================================================*/
int g_postfix_resume = 0;
static int icn_const_step(const tree_t * s, int64_t * bits, int * isr);
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; IR_t * psucc; IR_t * pfail; const char ** pn; int npn; IR_t * last_gen; IR_t * loop_exit; IR_t * loop_next; IR_t * beta; IR_t * conj_resumable; } icx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*====================================================================================================================================================================================================*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * icn_cset_canon(const char * s) {
    if (!s) return s;
    unsigned char seen[256]; memset(seen, 0, sizeof seen);
    for (const unsigned char * p = (const unsigned char *) s; *p; p++) seen[*p] = 1;
    char buf[257]; int n = 0;
    for (int c = 0; c < 256; c++) if (seen[c]) buf[n++] = (char) c;
    buf[n] = 0; return lp_strdup(buf);
}
static int icn_proc_is_generator(const char * name) { if (!name) return 0; for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_generator; return 0; }
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
    case AUGOP_CONCAT: return TT_CAT; case AUGOP_EQ: return TT_EQ; case AUGOP_LT: return TT_LT; case AUGOP_LE: return TT_LE; case AUGOP_GT: return TT_GT; case AUGOP_GE: return TT_GE; case AUGOP_NE: return TT_NE;
    case AUGOP_SEQ: return TT_LEQ; case AUGOP_SLT: return TT_LLT; case AUGOP_SLE: return TT_LLE; case AUGOP_SGT: return TT_LGT; case AUGOP_SGE: return TT_LGE; case AUGOP_SNE: return TT_LNE;
    case AUGOP_CSET_UNION: return TT_CSET_UNION; case AUGOP_CSET_DIFF: return TT_CSET_DIFF; case AUGOP_CSET_INTER: return TT_CSET_INTER;
    default: return (tree_e) 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_unop_tt(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL: case TT_RANDOM: case TT_INTERROGATE: case TT_MATCH_UNARY: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_resumable(const tree_t * t) {
    if (!t) return 0; if (t->t == TT_STMT) t = stmt_subj(t); if (!t) return 0;
    if (t->t == TT_FNC) { const char * nm = (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) ? t->c[0]->v.sval : NULL; return icn_call_allow_gen(nm); }
    if (lc_is_binop(t->t)) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_ASSIGN) { return (t->n > 1) ? is_resumable(t->c[1]) : 0; }
    switch (t->t) {
    case TT_IF: case TT_SCAN: case TT_EVERY: case TT_TO: case TT_TO_BY: case TT_ALTERNATE: case TT_REPEAT: case TT_WHILE: case TT_UNTIL: return 1;
    default: return 0; }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static int icn_subtree_has_suspend(const tree_t *n);
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*====================================================================================================================================================================================================*/
static IR_t * icn_arg_lower(void * vcx, const tree_t * a, IR_t * F) {
    icx_t * cx = (icx_t *) vcx; IR_t * sps = cx->psucc; IR_t * spf = cx->pfail;
    cx->psucc = NULL; cx->pfail = F;
    IR_t * r = NULL; IR_t * e = lower(cx, a, NULL, F, &r);
    cx->psucc = sps; cx->pfail = spf;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((icx_t *) vcx)->g, IR_LANG_ICN, icn_arg_lower, vcx, a); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_arg_is_scan_fn(const tree_t * a) { if (!a) return 0; if (a->t == TT_STMT) a = stmt_subj(a); if (!a || a->t != TT_FNC) return 0; const char * nm = (a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR) ? a->c[0]->v.sval : NULL; return nm && (!strcmp(nm, "tab") || !strcmp(nm, "move") || !strcmp(nm, "pos") || !strcmp(nm, "any") || !strcmp(nm, "match") || !strcmp(nm, "many") || !strcmp(nm, "upto") || !strcmp(nm, "find") || !strcmp(nm, "bal")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(icx_t * cx, const char * name, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (name && !strcmp(name, "seq")) { IR_t * sq = lower_seq(cx, t, argbase, nargs, γ, ω, res); if (sq) return sq; }
    if (name && !strcmp(name, "key") && nargs == 1) { IR_t * kg = lower_key(cx, t, argbase, nargs, γ, ω, res); if (kg) return kg; }
    IR_t * call = build(cx, IR_CALL, γ, ω); IR_LIT(call).sval = (char *) name; IR_LIT(call).ival = nargs;
    if (res) *res = call;
    int chains = name && (!strcmp(name, "write") || !strcmp(name, "writes"));
    int is_idx_or_list = name && (!strcmp(name, "[]") || !strcmp(name, "MAKELIST"));
    int is_cursor_mover = name && (!strcmp(name, "tab") || !strcmp(name, "move"));
    if (!chains) { for (int k = 0; k < nargs; k++) if (is_resumable(t->c[argbase + k])) { if (is_cursor_mover && icn_arg_is_scan_fn(t->c[argbase + k])) continue; chains = 1; break; } }
    int subgraph = !chains;
    if (subgraph) {
        lc_call_argblks(call, is_idx_or_list ? 2.0 : 3.0, nargs, arg_block, cx, (const tree_t * const *) &t->c[argbase]);
        int any_gen_arg = 0; for (int k = 0; k < nargs; k++) if (is_resumable(t->c[argbase + k])) { any_gen_arg = 1; break; }
        cx->beta = (icn_call_allow_gen(name) || any_gen_arg) ? call : ω;
        return call;
    }
    IR_LIT(call).dval = 1.0;
    IR_t * prev = NULL; IR_t * entry = call; IR_t * aω = ω;
    for (int k = 0; k < nargs; k++) {
        const tree_t * a = t->c[argbase + k]; IR_t * ar = NULL;
        IR_t * ae = lower(cx, a, (k == nargs - 1) ? call : NULL, aω, &ar); aω = cx->beta;
        if (k == 0) entry = ae;
        if (prev) γ_to(prev, ae);
        prev = ar;
        if (is_idx_or_list && ar) ir_operand_push(call, ar);
    }
    cx->beta = g_postfix_resume ? aω : ω;
    return entry;
}
/*====================================================================================================================================================================================================*/
static int icn_scan_kind_for(const char * s) {
    if (!s) return 0;
    if (!strcmp(s, "pos")) return IR_SCAN_POS; if (!strcmp(s, "any")) return IR_SCAN_ANY; if (!strcmp(s, "match")) return IR_SCAN_MATCH; if (!strcmp(s, "many")) return IR_SCAN_MANY; if (!strcmp(s, "tab")) return IR_SCAN_TAB; if (!strcmp(s, "move")) return IR_SCAN_MOVE; if (!strcmp(s, "upto")) return IR_SCAN_UPTO; if (!strcmp(s, "find")) return IR_SCAN_FIND; if (!strcmp(s, "bal")) return IR_SCAN_BAL;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void icn_retag_scan_body(IR_graph_t * g, int depth) {
    if (!g || !g->all || depth > 16) return;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        if (nd->op == IR_CALL && IR_LIT(nd).dval == 3.0 && IR_LIT(nd).sval) { int k = icn_scan_kind_for(IR_LIT(nd).sval); if (k) { nd->op = (IR_e) k; IR_graph_t ** blks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter; if (blks) for (int a = 0; a < (int) IR_LIT(nd).ival; a++) icn_retag_scan_body(blks[a], depth + 1); } }
        else if (nd->op == IR_GEN_SCAN) { icn_retag_scan_body((IR_graph_t *)(intptr_t) IR_EXEC(nd).counter, depth + 1); icn_retag_scan_body((IR_graph_t *)(intptr_t) IR_LIT(nd).ival, depth + 1); }
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    cx->beta = ω;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (lc_is_binop(t->t)) {
        { int64_t fb = 0; int fr = 0; if (icn_const_step(t, &fb, &fr) && fr) { IR_t * nd = build(cx, IR_LIT_F, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; } }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = lc_binop_code(t->t); if (IR_LIT(op).ival >= 5 && IR_LIT(op).ival <= 10) IR_LIT(op).dval = 1.0;
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta; IR_t * eb = lower(cx, t->c[1], op, lβ, &rr);
        IR_t * rβ = cx->beta;
        IR_t * opfail = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : NULL);
        if (IR_LIT(op).dval == 1.0 && opfail) ω_to(op, opfail);
        γ_to(lr, eb); { IR_t * ax[2]; ax[0] = lr; ax[1] = rr; bb_operand_aux_set(cx->g, op, ax, 2); }
        cx->beta = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : ω);
        *res = op; return ea; }
    if (is_unop_tt(t->t)) {
        { int64_t fb = 0; int fr = 0; if (icn_const_step(t, &fb, &fr)) { if (fr) { IR_t * nd = build(cx, IR_LIT_F, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; } IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = fb; *res = nd; return nd; } }
        IR_t * op = build(cx, IR_UNOP, γ, ω); IR_LIT(op).ival = (long long) t->t; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); *res = op; return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_CSET: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = icn_cset_canon(t->v.sval); IR_LIT(nd).ival = 1; *res = nd; return nd; }
    case TT_NULL: { if (t->n > 0 && t->c[0]) { IR_t * op = build(cx, IR_UNOP, γ, ω); IR_LIT(op).ival = (long long) TT_NULL; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); *res = op; return ea; } IR_t * nd = build(cx, IR_LIT_NUL, γ, ω); *res = nd; return nd; }
    case TT_VAR: { IR_t * nd = build(cx, (t->v.sval && t->v.sval[0] == '&') ? IR_KEYWORD : IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: { IR_t * nd = build(cx, IR_KEYWORD, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω);
        IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * ea = lower(cx, t->c[0], nd, ω, &br); ir_operand_push(nd, br); *res = nd; return ea; }
    case TT_FNC: { const tree_t * fn = (t->n > 0) ? t->c[0] : NULL; const char * nm = (fn && fn->t == TT_VAR) ? fn->v.sval : "?"; return lower_call(cx, nm, t, 1, t->n - 1, γ, ω, res); }
    case TT_IDX: return lower_call(cx, "[]", t, 0, t->n, γ, ω, res);
    case TT_MAKELIST: case TT_VLIST: return lower_call(cx, "MAKELIST", t, 0, t->n, γ, ω, res);
    case TT_ASSIGN: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1];
        if (lhs && lhs->t == TT_VAR) { IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = lhs->v.sval;
            IR_t * vr = NULL; IR_t * entry = lower(cx, rhs, asn, ω, &vr); ir_operand_push(asn, vr); *res = asn; return entry; }
        if (lhs && lhs->t == TT_IDX) {
            IR_t * set = build(cx, IR_IDX_SET, γ, ω);
            IR_t * br = NULL; IR_t * entry = lower(cx, lhs->c[0], set, ω, &br); ir_operand_push(set, br);
            for (int k = 1; k < lhs->n; k++) { IR_t * ir = NULL; lower(cx, lhs->c[k], set, ω, &ir); ir_operand_push(set, ir); }
            IR_t * vr = NULL; lower(cx, rhs, set, ω, &vr); ir_operand_push(set, vr);
            *res = set; return entry;
        }
        if (lhs && lhs->t == TT_FIELD) {
            IR_t * set = build(cx, IR_FIELD_SET, γ, ω);
            IR_LIT(set).sval = (lhs->n > 1 && lhs->c[1]) ? lhs->c[1]->v.sval : lhs->v.sval;
            IR_t * vr = NULL; IR_t * rhs_entry = lower(cx, rhs, set, ω, &vr);
            IR_t * br = NULL; IR_t * obj_entry = lower(cx, lhs->c[0], rhs_entry, ω, &br);
            ir_operand_push(set, br); ir_operand_push(set, vr);
            *res = set; return obj_entry;
        }
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
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = bc; IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower(cx, lhs, NULL, ω, &lr); IR_t * eb = lower(cx, rhs, op, ω, &rr); γ_to(lr, eb); *res = op; return ea;
    }
    case TT_RETURN: { IR_t * ret = build(cx, IR_RETURN, cx->psucc ? cx->psucc : γ, ω);
        if (t->n > 0 && t->c[0]) { IR_t * vr = NULL; IR_t * entry = lower(cx, t->c[0], ret, cx->pfail ? cx->pfail : ω, &vr); ir_operand_push(ret, vr); *res = ret; return entry; }
        *res = ret; return ret; }
    case TT_PROC_FAIL: { IR_t * nd = build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOOP_BREAK: { IR_t * lx = cx->loop_exit; IR_t * nd = build(cx, IR_BREAK, lx ? lx : γ, lx ? lx : ω); *res = nd; return nd; }
    case TT_LOOP_NEXT: { IR_t * ln = cx->loop_next; IR_t * nd = build(cx, IR_NEXT, ln ? ln : γ, ln ? ln : ω); *res = nd; return nd; }
    case TT_LOCAL: case TT_STATIC_DECL: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_INITIAL: { IR_t * ini = build(cx, IR_INITIAL, γ, ω);
        if (t->n > 0 && t->c[0]) { IR_t * bsucc = build(cx, IR_SUCCEED, γ, ω); IR_t * br = NULL; IR_t * be = lower(cx, t->c[0], bsucc, ω, &br); ir_operand_push(ini, be); }
        *res = ini; return ini; }
    case TT_SUSPEND: { IR_t * sn = build(cx, IR_SUSPEND, cx->psucc ? cx->psucc : γ, ω); IR_LIT(sn).dval = 1.0;
        IR_t * ev = NULL; IR_t * e_entry = sn;
        if (t->n > 0 && t->c[0]) { e_entry = lower(cx, t->c[0], sn, cx->pfail ? cx->pfail : ω, &ev); }
        ir_operand_push(sn, ev);
        if (t->n > 1 && t->c[1]) { IR_t * dv = NULL; IR_t * d_entry = lower(cx, t->c[1], γ, γ, &dv); ir_operand_push(sn, d_entry); }
        else ir_operand_push(sn, γ);
        *res = sn; return e_entry; }
    case TT_CASE: { if (t->n < 1 || !t->c[0]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        IR_t * cas = build(cx, IR_CASE, γ, ω);
        IR_t * sr = NULL; IR_t * se = lower(cx, t->c[0], cas, ω, &sr); ir_operand_push(cas, sr);
        for (int i = 1; i < t->n; ) {
            int remaining = t->n - i;
            if (remaining == 1) {
                IR_t * dk = build(cx, IR_CASE_ARM, NULL, NULL); IR_t * dr = NULL; (void) lower(cx, t->c[i], γ, ω, &dr); ir_operand_push(dk, dr); ir_operand_push(cas, dk); i++;
            } else {
                IR_t * kn = NULL; (void) lower(cx, t->c[i], NULL, NULL, &kn);
                IR_t * vn = NULL; (void) lower(cx, t->c[i+1], γ, ω, &vn);
                IR_t * arm = build(cx, IR_CASE_ARM, NULL, NULL); ir_operand_push(arm, kn); ir_operand_push(arm, vn); ir_operand_push(cas, arm); i += 2;
            }
        }
        *res = cas; return se; }
    case TT_SEQ:
    case TT_SEQ_EXPR: {
        lc_vec Sv; lc_vec_init(&Sv, (int) sizeof(const tree_t *));
        for (int i = 0; i < t->n; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) s = stmt_subj(s); if (s) lc_vec_push(&Sv, &s); }
        const tree_t ** S = (const tree_t **) Sv.data; int k = Sv.n;
        if (k == 0) { IR_t * su = build(cx, IR_SUCCEED, γ, ω); *res = su; return su; }
        if (k == 1) return lower(cx, S[0], γ, ω, res);
        IR_t * CONJ = build(cx, IR_CONJ, γ, ω);
        IR_t ** val = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t ** ent = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t * succ = CONJ;
        if (t->t == TT_SEQ_EXPR) {
            IR_t * failt = ω; IR_t * last_beta = ω; IR_t * rb = NULL;
            for (int i = k - 1; i >= 0; i--) { val[i] = NULL; ent[i] = lower(cx, S[i], succ, failt, &val[i]); if (i == k - 1) last_beta = cx->beta; if (!rb && is_resumable(S[i])) rb = cx->beta; succ = ent[i]; failt = ent[i]; }
            if (val[k - 1]) ir_operand_push(CONJ, val[k - 1]);
            cx->conj_resumable = rb; cx->beta = last_beta; *res = CONJ; return ent[0];
        }
        IR_t * last_beta = ω; IR_t * rb = NULL;
        for (int i = k - 1; i >= 0; i--) { val[i] = NULL; ent[i] = lower(cx, S[i], succ, ω, &val[i]); if (i == k - 1) last_beta = cx->beta; if (!rb && is_resumable(S[i])) rb = cx->beta; succ = ent[i]; }
        int lr = -1; for (int i = 0; i < k; i++) { if (lr >= 0) ω_to(val[i], val[lr]); if (is_resumable(S[i])) lr = i; }
        if (val[k - 1]) ir_operand_push(CONJ, val[k - 1]);
        cx->conj_resumable = rb; cx->beta = last_beta; *res = CONJ; return ent[0];
    }
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS: {
        if (t->n < 3 || !t->c[0] || !t->c[1] || !t->c[2]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        IR_t * sec = build(cx, IR_SECTION, γ, ω); IR_LIT(sec).ival = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[0], sec, ω, &ar); ir_operand_push(sec, ar);
        IR_t * br = NULL; lower(cx, t->c[1], sec, ω, &br); ir_operand_push(sec, br);
        IR_t * cr = NULL; lower(cx, t->c[2], sec, ω, &cr); ir_operand_push(sec, cr);
        cx->beta = ω; *res = sec; return ae; }
    case TT_NOT: return lower_not(cx, t, γ, ω, res);
    case TT_ALTERNATE: return lower_alt(cx, t, γ, ω, res);
    case TT_ITERATE: { IR_t * bang = build(cx, IR_LIST_BANG, γ, ω);
        IR_t * orr = NULL; (void) lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &orr); ir_operand_push(bang, orr);
        cx->beta = bang; *res = bang; return bang; }
    case TT_IF: return lower_if(cx, t, γ, ω, res);
    case TT_WHILE: return lower_while(cx, t, γ, ω, res);
    case TT_UNTIL: return lower_until(cx, t, γ, ω, res);
    case TT_REPEAT: return lower_repeat(cx, t, γ, ω, res);
    case TT_TO: case TT_TO_BY: return lower_to(cx, t, γ, ω, res);
    case TT_EVERY: return lower_every(cx, t, γ, ω, res);
    case TT_SCAN: { IR_t * gs = build(cx, IR_GEN_SCAN, γ, ω); IR_LIT(gs).dval = 1.0;
        IR_graph_t * ssg = arg_block(cx, (t->n > 0) ? t->c[0] : NULL); IR_graph_t * bsg = arg_block(cx, (t->n > 1) ? t->c[1] : NULL);
        icn_retag_scan_body(bsg, 0);
        IR_EXEC(gs).counter = (int64_t)(intptr_t) ssg; IR_LIT(gs).ival = (long long)(intptr_t) bsg; cx->beta = gs; *res = gs; return gs; }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); if (sub) return lower(cx, sub, γ, ω, res); IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_REPALT: { IR_t * nd = build(cx, IR_REPALT, γ, ω);
        IR_t * er = NULL; (void) lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &er);
        ir_operand_push(nd, er);
        cx->beta = nd; *res = nd; return nd; }
    case TT_LIMIT: { IR_t * nd = build(cx, IR_LIMIT, γ, ω);
        IR_t * er = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, nd, ω, &er);
        IR_t * inner_beta = cx->beta;
        IR_t * lr = NULL; lower(cx, (t->n > 1) ? t->c[1] : NULL, nd, ω, &lr);
        ir_operand_push(nd, er); ir_operand_push(nd, lr); ir_operand_push(nd, ee);
        (void)inner_beta; cx->beta = nd;
        *res = nd; return ee; }
    case TT_LCONCAT: { IR_t * nd = build(cx, IR_LCONCAT, γ, ω);
        IR_t * lr = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &lr); IR_t * lβ = cx->beta;
        IR_t * rr = NULL; lower(cx, (t->n > 1) ? t->c[1] : NULL, nd, lβ, &rr);
        γ_to(lr, (rr && rr != nd) ? rr : nd); { IR_t * ax[2]; ax[0] = lr; ax[1] = rr; bb_operand_aux_set(cx->g, nd, ax, 2); }
        *res = nd; return ee; }
    case TT_SWAP: { IR_t * nd = build(cx, IR_SWAP, γ, ω);
        IR_t * lr = NULL; lower(cx, (t->n > 0) ? t->c[0] : NULL, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, (t->n > 1) ? t->c[1] : NULL, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_REVASSIGN: { IR_t * nd = build(cx, IR_RASGN, γ, ω);
        IR_t * lr = NULL; lower(cx, (t->n > 0) ? t->c[0] : NULL, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, (t->n > 1) ? t->c[1] : NULL, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * W = build(cx, IR_WHILE, γ, ω);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, NULL, W, &cval);
    cx->loop_next = centry;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, centry, centry, &bval);
    γ_to(cval, b_entry); ir_operand_push(W, centry);
    cx->loop_exit = sle; cx->loop_next = sln;
    *res = W; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * U = build(cx, IR_UNTIL, γ, ω);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, U, NULL, &cval);
    cx->loop_next = centry;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, centry, centry, &bval);
    ω_to(cval, b_entry); ir_operand_push(U, centry);
    cx->loop_exit = sle; cx->loop_next = sln;
    *res = U; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
    IR_t * R = build(cx, IR_REPEAT, NULL, ω);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ; cx->loop_next = R;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, R, R, &bval);
    γ_to(R, b_entry); ir_operand_push(R, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln;
    *res = R; return b_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nt = build(cx, IR_NOT, γ, ω);
    IR_t * cr = NULL; IR_t * ce = lower(cx, (t->n > 0) ? t->c[0] : NULL, ω, nt, &cr);
    ir_operand_push(nt, cr);
    cx->beta = ω; *res = nt; return ce;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int n = t->n; if (n < 1) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    IR_t * node = build(cx, IR_ALT, γ, ω);
    IR_t ** entry = (IR_t **) calloc((size_t) n, sizeof(IR_t *)); IR_t ** apply = (IR_t **) calloc((size_t) n, sizeof(IR_t *));
    for (int j = n - 1; j >= 0; j--) {
        IR_t * ωj = (j + 1 < n) ? entry[j + 1] : ω; IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[j], node, ωj, &ar);
        if (ar && !ar->γ.node) γ_to(ar, node); apply[j] = ar ? ar : ae; entry[j] = ae;
    }
    bb_operand_aux_set(cx->g, node, apply, n); cx->beta = node; *res = node; return entry[0];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * T = (t->n > 1) ? t->c[1] : NULL; const tree_t * E = (t->n > 2) ? t->c[2] : NULL;
    IR_t * iff = build(cx, IR_IF, γ, ω);
    IR_t * tval = NULL; IR_t * tentry = lower(cx, T, γ, ω, &tval);
    IR_t * eentry = ω; if (E) { IR_t * eval = NULL; eentry = lower(cx, E, γ, ω, &eval); }
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, tentry, eentry, &cval);
    ir_operand_push(iff, centry);
    *res = iff; return centry;
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
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        rv = pow(la, ra); memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    if ((s->t == TT_ADD || s->t == TT_SUB || s->t == TT_MUL || s->t == TT_DIV || s->t == TT_MOD) && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0; int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            int64_t r; if (s->t == TT_ADD) r = lb + rb; else if (s->t == TT_SUB) r = lb - rb; else if (s->t == TT_MUL) r = lb * rb; else if (s->t == TT_DIV) { if (rb == 0) return 0; r = lb / rb; } else { if (rb == 0) return 0; r = lb % rb; }
            *bits = r; *isr = 0; return 1;
        }
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        if (s->t == TT_ADD) rv = la + ra; else if (s->t == TT_SUB) rv = la - ra; else if (s->t == TT_MUL) rv = la * ra; else if (s->t == TT_DIV) { if (ra == 0.0) return 0; rv = la / ra; } else { if (ra == 0.0) return 0; rv = fmod(la, ra); }
        memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    int64_t by = 1; const tree_t * byt = (nargs > 1) ? t->c[argbase + 1] : NULL;
    if (byt) { int64_t bb = 1; int isr = 0; if (!icn_const_step(byt, &bb, &isr) || isr) return NULL; by = bb; }
    if (by == 0) return NULL;
    IR_t * to = build(cx, IR_TO_BY, γ, ω); IR_LIT(to).sval = (char *) "ag"; IR_LIT(to).ival = by; cx->last_gen = to;
    const tree_t * fromt = (nargs > 0) ? t->c[argbase] : NULL; IR_t * lr; IR_t * ea; IR_t * lβ;
    if (fromt) { ea = lower(cx, fromt, NULL, ω, &lr); lβ = cx->beta; } else { lr = build(cx, IR_LIT_I, NULL, ω); IR_LIT(lr).ival = 1; ea = lr; lβ = ω; }
    IR_t * mr = build(cx, IR_LIT_I, to, lβ); IR_LIT(mr).ival = (by > 0) ? INT64_MAX : INT64_MIN; γ_to(lr, mr);
    ir_operand_push(to, lr); ir_operand_push(to, mr); cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) nargs;
    IR_t * kg = build(cx, IR_KEY_GEN, γ, ω);
    IR_t * orr = NULL; (void) lower(cx, t->c[argbase], NULL, ω, &orr); ir_operand_push(kg, orr);
    cx->beta = kg; *res = kg; return kg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int by = (t->t == TT_TO_BY) ? 1 : 0; int varby = 0;
    IR_t * to = build(cx, by ? IR_TO_BY : IR_TO, γ, ω); IR_LIT(to).sval = (char *) "ag"; cx->last_gen = to;
    if (by && t->n > 2 && t->c[2]) {
        int64_t bits = 1; int isr = 0;
        if (icn_const_step(t->c[2], &bits, &isr)) { IR_LIT(to).ival = bits; if (isr) IR_LIT(to).sval = (char *) "ar"; } else varby = 1;
    }
    IR_t * lr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
    IR_t * mr = NULL; IR_t * em = lower(cx, t->c[1], to, lβ, &mr); γ_to(lr, em);
    ir_operand_push(to, lr); ir_operand_push(to, mr);
    if (varby) { IR_t * mβ = cx->beta; IR_t * br = NULL; IR_t * eb = lower(cx, t->c[2], to, mβ, &br); γ_to(mr, eb); ir_operand_push(to, br); }
    cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * GEN = (t->n > 0) ? t->c[0] : NULL; const tree_t * BODY = (t->n > 1) ? t->c[1] : NULL;
    IR_t * E = build(cx, IR_EVERY, γ, ω);
    IR_t * gen_entry; IR_t * gen_result; IR_t * gen_node; cx->conj_resumable = NULL;
    if (GEN && GEN->t == TT_ASSIGN && GEN->c[0] && GEN->c[0]->t == TT_VAR) {
        IR_t * asn = build(cx, IR_ASSIGN, NULL, E); IR_LIT(asn).sval = GEN->c[0]->v.sval;
        IR_t * rr = NULL; gen_entry = lower(cx, GEN->c[1], asn, E, &rr);
        ir_operand_push(asn, rr); gen_result = asn; gen_node = (cx->beta && cx->beta != E) ? cx->beta : rr;
    } else {
        IR_t * sg = cx->last_gen; cx->last_gen = NULL;
        gen_entry = lower(cx, GEN, NULL, E, &gen_result);
        gen_node = (cx->beta && cx->beta != E) ? cx->beta : (cx->last_gen ? cx->last_gen : gen_result);
        cx->last_gen = sg;
    }
    IR_t * conj_rb = cx->conj_resumable;
    if (!BODY) {
        IR_t * loop_target = (gen_node && gen_node != gen_result && gen_node != ω && gen_node != E) ? gen_node : ((conj_rb && conj_rb != gen_result && conj_rb != ω && conj_rb != E) ? conj_rb : E);
        γ_to(gen_result, loop_target);
        ir_operand_push(E, gen_entry);
        *res = E; return gen_entry;
    }
    IR_t * loop_back = gen_node;
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; cx->loop_exit = γ; cx->loop_next = loop_back;
    IR_t * bval = NULL; IR_t * body_entry = lower(cx, BODY, loop_back, loop_back, &bval);
    cx->loop_exit = sle; cx->loop_next = sln;
    γ_to(gen_result, body_entry);
    ir_operand_push(E, gen_entry);
    *res = E; return gen_entry;
}
/*====================================================================================================================================================================================================*/
static IR_graph_t * lower_proc_body(icx_t * cx, const tree_t * body) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_ICN); cx->g = g;
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->psucc = PSUCC; cx->pfail = PFAIL;
    IR_t * succ = icn_subtree_has_suspend(body) ? PFAIL : PSUCC; IR_t * fail = PFAIL;
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t * s = body->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; } if (!s) continue;
        IR_t * r = NULL; IR_t * entry = lower(cx, s, succ, fail, &r); succ = entry; fail = entry;
    }
    g->entry = succ; return g;
}
/*====================================================================================================================================================================================================*/
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
IR_graph_t * lower_icon_proc(const tree_t * prog, const tree_t * pd) {
    static lc_vec pnv; lc_vec_init(&pnv, (int) sizeof(const char *)); fill_pnames(prog, &pnv);
    icx_t cx; memset(&cx, 0, sizeof cx); cx.pn = (const char **) pnv.data; cx.npn = pnv.n;
    if (pd && pd->n > 2 && pd->c[2]) return lower_proc_body(&cx, pd->c[2]);
    IR_graph_t * g = IR_alloc(64, IR_LANG_ICN); cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s; return g;
}
/*====================================================================================================================================================================================================*/
#include "bb_program.h"
#include "box_state.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_subtree_has_suspend(const tree_t *n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_PROC_DECL || n->t == TT_SUB_DECL) return 0;
    for (int i = 0; i < n->n; i++) if (icn_subtree_has_suspend(n->c[i])) return 1;
    return 0;
}
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_icon_stage2(const tree_t *prog) {
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
        }
    }
}
