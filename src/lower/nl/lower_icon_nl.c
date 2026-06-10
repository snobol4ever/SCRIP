#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "IR.h"
extern int bb_operand_aux_set(IR_graph_t * bbg, IR_t * bb, IR_t * const * src, int n);
/*====================================================================================================================================================================================================*/
int g_icn_postfix_resume = 0;
int g_icn_globals_nv     = 1;
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; IR_t * psucc; IR_t * pfail; const char ** pn; int npn; IR_t * last_gen; IR_t * loop_exit; IR_t * beta; } icx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "α", 3); nd->ω.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = IR_node_alloc(cx->g, op); γ_to(nd, γ); ω_to(nd, ω); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int binop_code(tree_e tt) {
    switch (tt) {
    case TT_ADD: return 0; case TT_SUB: return 1; case TT_MUL: return 2; case TT_DIV: return 3; case TT_MOD: return 4;
    case TT_LT: return 5; case TT_LE: return 6; case TT_GT: return 7; case TT_GE: return 8;
    case TT_EQ: return 9; case TT_NE: return 10; case TT_CAT: return 11;
    case TT_LLT: return 12; case TT_LLE: return 13; case TT_LGT: return 14; case TT_LGE: return 15;
    case TT_LEQ: return 16; case TT_LNE: return 17; case TT_POW: return 18; default: return 0; }
}
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
static int is_user_proc(icx_t * cx, const char * nm) { if (!nm) return 0; for (int i = 0; i < cx->npn; i++) if (cx->pn[i] && !strcmp(cx->pn[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_binop_tt(tree_e tt) {
    switch (tt) {
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW: case TT_LT: case TT_LE: case TT_GT: case TT_GE:
    case TT_EQ: case TT_NE: case TT_CAT: case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_unop_tt(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL: case TT_RANDOM: case TT_NOT: case TT_INTERROGATE: case TT_MATCH_UNARY: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_resumable(const tree_t * t) {
    if (!t) return 0; if (t->t == TT_STMT) t = stmt_subj(t); if (!t) return 0;
    switch (t->t) {
    case TT_IF: case TT_SCAN: case TT_EVERY: case TT_TO: case TT_TO_BY: case TT_ALTERNATE: case TT_REPEAT: case TT_WHILE: case TT_UNTIL: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * arg_block(icx_t * cx, const tree_t * a) {
    IR_graph_t * saved = cx->g; IR_t * sps = cx->psucc; IR_t * spf = cx->pfail;
    IR_graph_t * g2 = IR_alloc(256, IR_LANG_ICN); cx->g = g2;
    IR_t * F = IR_node_alloc(g2, IR_FAIL);
    cx->psucc = NULL; cx->pfail = F;
    IR_t * r = NULL; IR_t * e = lower(cx, a, NULL, F, &r);
    g2->entry = e;
    cx->g = saved; cx->psucc = sps; cx->pfail = spf;
    return g2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(icx_t * cx, const char * name, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * call = build(cx, IR_CALL, γ, ω); IR_LIT(call).sval = (char *) name; IR_LIT(call).ival = nargs;
    if (res) *res = call;
    int chains = name && (!strcmp(name, "write") || !strcmp(name, "writes"));
    int subgraph = !chains;
    if (subgraph) {
        IR_LIT(call).dval = (name && (!strcmp(name, "[]") || !strcmp(name, "MAKELIST"))) ? 2.0 : 3.0;
        if (nargs > 0) {
            IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
            if (blks) { for (int k = 0; k < nargs; k++) blks[k] = arg_block(cx, t->c[argbase + k]); IR_EXEC(call).counter = (int64_t)(intptr_t) blks; }
        }
        cx->beta = ω;
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
    }
    cx->beta = g_icn_postfix_resume ? aω : ω;
    return entry;
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    cx->beta = ω;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (is_binop_tt(t->t)) {
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = binop_code(t->t); if (IR_LIT(op).ival >= 5 && IR_LIT(op).ival <= 10) IR_LIT(op).dval = 1.0;
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta; IR_t * eb = lower(cx, t->c[1], op, lβ, &rr);
        γ_to(lr, eb); { IR_t * ax[2]; ax[0] = lr; ax[1] = rr; bb_operand_aux_set(cx->g, op, ax, 2); } *res = op; return ea; }
    if (is_unop_tt(t->t)) { IR_t * op = build(cx, IR_UNOP, γ, ω); IR_LIT(op).ival = (long long) t->t; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); *res = op; return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: case TT_CSET: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_NULL: { IR_t * nd = build(cx, IR_LIT_NUL, γ, ω); *res = nd; return nd; }
    case TT_VAR: { IR_t * nd = build(cx, (t->v.sval && t->v.sval[0] == '&') ? IR_KEYWORD : IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: { IR_t * nd = build(cx, IR_KEYWORD, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω);
        IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * ea = lower(cx, t->c[0], nd, ω, &br); ir_operand_push(nd, br); *res = nd; return ea; }
    case TT_FNC: { const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?"; return lower_call(cx, nm, t, 1, t->n - 1, γ, ω, res); }
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
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_t * lr = NULL, * rr = NULL;
        IR_t * eb = lower(cx, rhs, asn, ω, &rr); IR_t * ea = lower(cx, lhs, eb, ω, &lr);
        ir_operand_push(asn, rr); ir_operand_push(asn, lr); *res = asn; return ea;
    }
    case TT_AUGOP: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1]; int bc = augop_code((int) t->v.ival);
        if (lhs && lhs->t == TT_VAR) { IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = lhs->v.sval;
            IR_t * op = build(cx, IR_BINOP, asn, ω); IR_LIT(op).ival = bc; ir_operand_push(asn, op);
            IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower(cx, lhs, NULL, ω, &lr); IR_t * eb = lower(cx, rhs, op, ω, &rr);
            γ_to(lr, eb); *res = asn; return ea; }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = bc; IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower(cx, lhs, NULL, ω, &lr); IR_t * eb = lower(cx, rhs, op, ω, &rr); γ_to(lr, eb); *res = op; return ea;
    }
    case TT_RETURN: { IR_t * ret = build(cx, IR_RETURN, γ, ω);
        if (t->n > 0 && t->c[0]) { IR_t * vr = NULL; IR_t * entry = lower(cx, t->c[0], ret, ω, &vr); ir_operand_push(ret, vr); *res = ret; return entry; }
        *res = ret; return ret; }
    case TT_PROC_FAIL: { IR_t * nd = build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOOP_BREAK: { IR_t * lx = cx->loop_exit; IR_t * nd = build(cx, IR_BREAK, lx ? lx : γ, lx ? lx : ω); *res = nd; return nd; }
    case TT_LOOP_NEXT: { IR_t * nd = build(cx, IR_NEXT, γ, ω); *res = nd; return nd; }
    case TT_LOCAL: case TT_STATIC_DECL: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_INITIAL: { IR_t * ini = build(cx, IR_INITIAL, γ, ω);
        if (t->n > 0 && t->c[0]) { IR_t * br = NULL; IR_t * be = lower(cx, t->c[0], γ, ω, &br); ir_operand_push(ini, be); }
        *res = ini; return ini; }
    case TT_SUSPEND: { IR_t * sn = build(cx, IR_SUSPEND, γ, ω); IR_LIT(sn).dval = 1.0;
        if (t->n > 0 && t->c[0]) { IR_graph_t * eg = arg_block(cx, t->c[0]); IR_EXEC(sn).counter = (int64_t)(intptr_t) eg; }
        if (t->n > 1 && t->c[1]) { IR_graph_t * bg = arg_block(cx, t->c[1]); IR_LIT(sn).ival  = (long long)(intptr_t) bg; }
        *res = sn; return sn; }
    case TT_CASE: { if (t->n < 1 || !t->c[0]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        IR_t * cas = build(cx, IR_CASE, γ, ω);
        IR_t * sr = NULL; IR_t * se = lower(cx, t->c[0], cas, ω, &sr); ir_operand_push(cas, sr);
        for (int i = 1; i < t->n; ) {
            int remaining = t->n - i;
            if (remaining == 1) {
                IR_t * dk = build(cx, IR_LIT_NUL, NULL, NULL); IR_t * dr = NULL; IR_t * de = lower(cx, t->c[i], γ, ω, &dr); ir_operand_push(dk, de); ir_operand_push(cas, dk); i++;
            } else {
                IR_t * kn = NULL; lower(cx, t->c[i], NULL, NULL, &kn);
                IR_t * vn = NULL; lower(cx, t->c[i+1], γ, ω, &vn);
                IR_t * arm = build(cx, IR_LIT_NUL, NULL, NULL); ir_operand_push(arm, kn); ir_operand_push(arm, vn); ir_operand_push(cas, arm); i += 2;
            }
        }
        *res = cas; return se; }
    case TT_SEQ: {
        IR_t * succ = γ; IR_t * fail = ω; IR_t * entry = γ;
        for (int i = t->n - 1; i >= 0; i--) {
            const tree_t * s = t->c[i];
            if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
            if (!s) continue; IR_t * r = NULL; entry = lower(cx, s, succ, fail, &r); succ = entry; fail = entry; }
        *res = entry; return entry;
    }
    case TT_SEQ_EXPR: {
        const tree_t * S[128]; int k = 0;
        for (int i = 0; i < t->n && k < 128; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) s = stmt_subj(s); if (s) S[k++] = s; }
        if (k == 0) { IR_t * su = build(cx, IR_SUCCEED, γ, ω); *res = su; return su; }
        if (k == 1) return lower(cx, S[0], γ, ω, res);
        IR_t * CONJ = build(cx, IR_CONJ, γ, ω);
        IR_t * val[128]; IR_t * ent[128]; IR_t * succ = CONJ;
        for (int i = k - 1; i >= 0; i--) { val[i] = NULL; ent[i] = lower(cx, S[i], succ, ω, &val[i]); succ = ent[i]; }
        int lr = -1; for (int i = 0; i < k; i++) { if (lr >= 0) ω_to(val[i], val[lr]); if (is_resumable(S[i])) lr = i; }
        *res = CONJ; return ent[0];
    }
    case TT_IF: return lower_if(cx, t, γ, ω, res);
    case TT_WHILE: return lower_while(cx, t, γ, ω, res);
    case TT_UNTIL: return lower_until(cx, t, γ, ω, res);
    case TT_REPEAT: return lower_repeat(cx, t, γ, ω, res);
    case TT_TO: case TT_TO_BY: return lower_to(cx, t, γ, ω, res);
    case TT_EVERY: return lower_every(cx, t, γ, ω, res);
    case TT_SCAN: { IR_t * gs = build(cx, IR_GEN_SCAN, γ, ω); IR_LIT(gs).dval = 1.0;
        IR_graph_t * ssg = arg_block(cx, (t->n > 0) ? t->c[0] : NULL); IR_graph_t * bsg = arg_block(cx, (t->n > 1) ? t->c[1] : NULL);
        IR_EXEC(gs).counter = (int64_t)(intptr_t) ssg; IR_LIT(gs).ival = (long long)(intptr_t) bsg; *res = gs; return gs; }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); if (sub) return lower(cx, sub, γ, ω, res); IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * W = build(cx, IR_WHILE, γ, ω);
    IR_t * sle = cx->loop_exit; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, NULL, W, &cval);
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, centry, centry, &bval);
    γ_to(cval, b_entry); ir_operand_push(W, centry);
    cx->loop_exit = sle;
    *res = W; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * U = build(cx, IR_UNTIL, γ, ω);
    IR_t * sle = cx->loop_exit; cx->loop_exit = γ;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, U, NULL, &cval);
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, centry, centry, &bval);
    ω_to(cval, b_entry); ir_operand_push(U, centry);
    cx->loop_exit = sle;
    *res = U; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
    IR_t * R = build(cx, IR_REPEAT, NULL, ω);
    IR_t * sle = cx->loop_exit; cx->loop_exit = γ;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, R, R, &bval);
    γ_to(R, b_entry); ir_operand_push(R, b_entry);
    cx->loop_exit = sle;
    *res = R; return b_entry;
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
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int by = (t->t == TT_TO_BY) ? 1 : 0;
    IR_t * to = build(cx, by ? IR_TO_BY : IR_TO, γ, ω); IR_LIT(to).sval = (char *) "ag"; cx->last_gen = to;
    if (by && t->n > 2 && t->c[2]) {
        int64_t bits = 1; int isr = 0;
        if (icn_const_step(t->c[2], &bits, &isr)) { IR_LIT(to).ival = bits; if (isr) IR_LIT(to).sval = (char *) "ar"; }
    }
    IR_t * lr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
    IR_t * mr = NULL; IR_t * em = lower(cx, t->c[1], to, lβ, &mr); γ_to(lr, em);
    ir_operand_push(to, lr); ir_operand_push(to, mr);
    cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * GEN = (t->n > 0) ? t->c[0] : NULL; const tree_t * BODY = (t->n > 1) ? t->c[1] : NULL;
    IR_t * E = build(cx, IR_EVERY, γ, ω);
    IR_t * gen_entry; IR_t * gen_result; IR_t * gen_node;
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
    if (!BODY) {
        γ_to(gen_result, gen_node == gen_result ? E : gen_node);
        ir_operand_push(E, gen_entry);
        *res = E; return gen_entry;
    }
    IR_t * loop_back = gen_node;
    IR_t * bval = NULL; IR_t * body_entry = lower(cx, BODY, loop_back, loop_back, &bval);
    γ_to(gen_result, body_entry);
    ir_operand_push(E, gen_entry);
    *res = E; return gen_entry;
}
/*====================================================================================================================================================================================================*/
static IR_graph_t * lower_proc_body(icx_t * cx, const tree_t * body) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_ICN); cx->g = g;
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->psucc = PSUCC; cx->pfail = PFAIL;
    IR_t * succ = PSUCC; IR_t * fail = PFAIL;
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t * s = body->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; } if (!s) continue;
        IR_t * r = NULL; IR_t * entry = lower(cx, s, succ, fail, &r); succ = entry; fail = entry;
    }
    g->entry = succ; return g;
}
/*====================================================================================================================================================================================================*/
static int collect_procs(const tree_t * t, const tree_t ** out, int max, int n) {
    if (!t || n >= max) return n;
    if (t->t == TT_STMT) return collect_procs(stmt_subj(t), out, max, n);
    if (t->t == TT_PROC_DECL) { out[n++] = t; return n; }
    for (int i = 0; i < t->n; i++) n = collect_procs(t->c[i], out, max, n);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_icon_enum(const tree_t * prog, const tree_t ** out, int max) { return collect_procs(prog, out, max, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fill_pnames(const tree_t * prog, const char ** pn, int * npn, int max) {
    const tree_t * ps[256]; int k = collect_procs(prog, ps, 256, 0); int c = 0;
    for (int i = 0; i < k && c < max; i++) if (ps[i]->v.sval) pn[c++] = ps[i]->v.sval;
    *npn = c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_icon_proc(const tree_t * prog, const tree_t * pd) {
    static const char * pn[256]; int npn = 0; fill_pnames(prog, pn, &npn, 256);
    icx_t cx; memset(&cx, 0, sizeof cx); cx.pn = pn; cx.npn = npn;
    if (pd && pd->n > 2 && pd->c[2]) return lower_proc_body(&cx, pd->c[2]);
    IR_graph_t * g = IR_alloc(64, IR_LANG_ICN); cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s; return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_icon(const tree_t * prog) {
    const tree_t * ps[256]; int k = collect_procs(prog, ps, 256, 0);
    if (k > 0) return lower_icon_proc(prog, ps[0]);
    IR_graph_t * g = IR_alloc(64, IR_LANG_ICN); icx_t cx; memset(&cx, 0, sizeof cx); cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s; return g;
}
