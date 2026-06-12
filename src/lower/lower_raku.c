#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; } rcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(rcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
static IR_t * lower_decl(rcx_t * cx, const tree_t * t);
static IR_t * lower_block(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void push_kids(rcx_t * cx, IR_t * nd, const tree_t * t, int from) { for (int i = from; i < t->n; i++) ir_operand_push(nd, lower(cx, t->c[i], NULL, NULL)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_nary(rcx_t * cx, const tree_t * t, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = build(cx, op, γ, ω); push_kids(cx, nd, t, 0); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_binop(rcx_t * cx, const tree_t * t, const char * opn, IR_t * γ, IR_t * ω) {
    IR_t * nd = build(cx, IR_BINOP, γ, ω); IR_LIT(nd).sval = opn;
    ir_operand_push(nd, lower(cx, t->c[0], NULL, NULL));
    ir_operand_push(nd, lower(cx, t->c[1], NULL, NULL)); return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_unop(rcx_t * cx, const tree_t * t, const char * opn, IR_t * γ, IR_t * ω) {
    IR_t * nd = build(cx, IR_UNOP, γ, ω); IR_LIT(nd).sval = opn;
    ir_operand_push(nd, lower(cx, t->c[0], NULL, NULL)); return nd;
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    if (!t) return build(cx, IR_SUCCEED, γ, ω);
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, γ, ω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_NUL: return build(cx, IR_LIT_NUL, γ, ω);
    case TT_VAR: { IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval; ir_operand_push(nd, lower(cx, t->c[0], NULL, NULL)); return nd; }
    case TT_TWIGIL_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ADD: return lower_binop(cx, t, "+", γ, ω);
    case TT_SUB: return lower_binop(cx, t, "-", γ, ω);
    case TT_MUL: return lower_binop(cx, t, "*", γ, ω);
    case TT_DIV: return lower_binop(cx, t, "/", γ, ω);
    case TT_MOD: return lower_binop(cx, t, "%", γ, ω);
    case TT_MNS: return lower_unop(cx, t, "-", γ, ω);
    case TT_LT: return lower_binop(cx, t, "<", γ, ω);
    case TT_LE: return lower_binop(cx, t, "<=", γ, ω);
    case TT_GT: return lower_binop(cx, t, ">", γ, ω);
    case TT_GE: return lower_binop(cx, t, ">=", γ, ω);
    case TT_EQ: return lower_binop(cx, t, "==", γ, ω);
    case TT_NE: return lower_binop(cx, t, "!=", γ, ω);
    case TT_LEQ: return lower_binop(cx, t, "eq", γ, ω);
    case TT_LNE: return lower_binop(cx, t, "ne", γ, ω);
    case TT_NOT: return lower_unop(cx, t, "not", γ, ω);
    case TT_CAT: return lower_binop(cx, t, "~", γ, ω);
    case TT_FNC: return lower_nary(cx, t, IR_CALL, γ, ω);
    case TT_ARR_GET: return lower_nary(cx, t, IR_IDX, γ, ω);
    case TT_ARR_SET: return lower_nary(cx, t, IR_IDX_SET, γ, ω);
    case TT_HASH_GET: return lower_nary(cx, t, IR_IDX, γ, ω);
    case TT_HASH_SET: return lower_nary(cx, t, IR_IDX_SET, γ, ω);
    case TT_HASH_EXISTS: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "exists"; return nd; }
    case TT_HASH_DELETE: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "delete"; return nd; }
    case TT_METHCALL: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_NEW: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "new"; return nd; }
    case TT_ASSIGN: return lower_nary(cx, t, IR_ASSIGN, γ, ω);
    case TT_TO: return lower_nary(cx, t, IR_TO, γ, ω);
    case TT_ITERATE: return lower_nary(cx, t, IR_ITERATE, γ, ω);
    case TT_EVERY: return lower_nary(cx, t, IR_EVERY, γ, ω);
    case TT_MAP: return lower_nary(cx, t, IR_MAP, γ, ω);
    case TT_GREP: return lower_nary(cx, t, IR_GREP, γ, ω);
    case TT_SORT: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "sort"; return nd; }
    case TT_GATHER: return lower_nary(cx, t, IR_GATHER, γ, ω);
    case TT_ALT: return lower_nary(cx, t, IR_ALT, γ, ω);
    case TT_SEQ_EXPR: return lower_nary(cx, t, IR_SEQ_EXPR, γ, ω);
    case TT_SAY: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "say"; return nd; }
    case TT_SAY_FH: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "say"; return nd; }
    case TT_PRINT: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "print"; return nd; }
    case TT_PRINT_FH: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "print"; return nd; }
    case TT_IF: return lower_nary(cx, t, IR_IF, γ, ω);
    case TT_UNLESS: return lower_nary(cx, t, IR_IF, γ, ω);
    case TT_WHILE: return lower_nary(cx, t, IR_WHILE, γ, ω);
    case TT_UNTIL: return lower_nary(cx, t, IR_UNTIL, γ, ω);
    case TT_REPEAT: return lower_nary(cx, t, IR_REPEAT, γ, ω);
    case TT_FOR_RANGE: return lower_nary(cx, t, IR_EVERY, γ, ω);
    case TT_CASE: return lower_nary(cx, t, IR_CASE, γ, ω);
    case TT_SUSPEND: return lower_nary(cx, t, IR_SUSPEND, γ, ω);
    case TT_RETURN: return lower_nary(cx, t, IR_RETURN, γ, ω);
    case TT_TRY: return lower_nary(cx, t, IR_CATCH, γ, ω);
    case TT_DIE: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "die"; return nd; }
    case TT_SMATCH: return lower_nary(cx, t, IR_PAT_MATCH, γ, ω);
    case TT_CAPTURE: return lower_nary(cx, t, IR_PATTERN_CAPTURE, γ, ω);
    case TT_NAMED_CAPTURE: return lower_nary(cx, t, IR_PATTERN_CAPTURE, γ, ω);
    case TT_FH_CAPTURE: return lower_nary(cx, t, IR_PATTERN_CAPTURE, γ, ω);
    case TT_REGEX_DECL: { IR_t * nd = build(cx, IR_SUCCEED, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_GRAMMAR_DECL: { IR_t * nd = build(cx, IR_SUCCEED, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_DECL: return build(cx, IR_SUCCEED, γ, ω);
    case TT_SEQ: return lower_block(cx, t, γ, ω);
    case TT_PROGRAM: return lower_block(cx, t, γ, ω);
    case TT_STMT: { const tree_t * sub = stmt_subj(t); return sub ? lower(cx, sub, γ, ω) : build(cx, IR_SUCCEED, γ, ω); }
    case TT_SUB_DECL: return lower_decl(cx, t);
    case TT_CLASS_DECL: return lower_decl(cx, t);
    default: return build(cx, IR_SUCCEED, γ, ω);
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_block(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    IR_t * head = NULL; IR_t * prev = NULL;
    for (int i = 0; i < t->n; i++) {
        const tree_t * s = t->c[i];
        if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        IR_t * nd = lower(cx, s, NULL, ω);
        if (!head) head = nd;
        if (prev) γ_to(prev, nd);
        prev = nd;
    }
    if (prev) γ_to(prev, γ);
    if (!head) head = build(cx, IR_SUCCEED, γ, ω);
    return head;
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_decl(rcx_t * cx, const tree_t * t) {
    switch (t->t) {
    case TT_SUB_DECL: { IR_t * nd = IR_node_alloc(cx->g, IR_PROC); IR_LIT(nd).sval = t->v.sval;
        IR_t * body = (t->n > 2) ? lower_block(cx, t->c[2], NULL, NULL) : build(cx, IR_SUCCEED, NULL, NULL);
        ir_operand_push(nd, body); return nd; }
    case TT_CLASS_DECL: { IR_t * nd = IR_node_alloc(cx->g, IR_RECORD_DEF); IR_LIT(nd).sval = t->v.sval; push_kids(cx, nd, t, 0); return nd; }
    default: return lower(cx, t, NULL, NULL);
    }
}
/*====================================================================================================================================================================================================*/
static int rk_is_binop(tree_e tt) {
    switch (tt) { case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_CAT: case TT_LEQ: case TT_LNE: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_arg_lower(void * vcx, const tree_t * a, IR_t * F) { IR_t * r = NULL; return lower_rv((rcx_t *) vcx, a, NULL, F, &r); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * rk_arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((rcx_t *) vcx)->g, IR_LANG_RKU, rk_arg_lower, vcx, a); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rcall(rcx_t * cx, const tree_t * t, const char * nm, int from, int visible, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = t->n - from;
    if (visible) {
        IR_LIT(nd).dval = 1.0;
        IR_t * succ = nd; IR_t * entry = nd;
        for (int i = t->n - 1; i >= from; i--) { IR_t * r = NULL; IR_t * e = lower_rv(cx, t->c[i], succ, ω, &r); succ = e; entry = e; }
        *res = nd; return (t->n > from) ? entry : nd; }
    lc_call_argblks(nd, 2.0, t->n - from, rk_arg_block, cx, (const tree_t * const *) &t->c[from]);
    *res = nd; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rblock(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    if (!t || t->n == 0) return build(cx, IR_SUCCEED, γ, ω);
    IR_t * succ = γ; IR_t * entry = γ;
    for (int i = t->n - 1; i >= 0; i--) {
        const tree_t * s = t->c[i];
        if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        IR_t * r = NULL; IR_t * e = lower_rv(cx, s, succ, ω, &r);
        if (e) { entry = e; succ = e; }
    }
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (rk_is_binop(t->t)) {
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower_rv(cx, t->c[0], NULL, ω, &lr); IR_t * eb = lower_rv(cx, t->c[1], op, ω, &rr);
        γ_to(lr, eb); { IR_t * ax[2]; ax[0] = lr; ax[1] = rr; bb_operand_aux_set(cx->g, op, ax, 2); } *res = op; return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_NUL: { IR_t * nd = build(cx, IR_LIT_NUL, γ, ω); *res = nd; return nd; }
    case TT_VAR: { IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_ASSIGN: if (t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR) {
        const tree_t * rhs = t->c[1];
        if (rhs && rhs->t == TT_FNC && rhs->n > 1 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "pop") && rhs->c[1] && rhs->c[1]->t == TT_VAR) {
            IR_t * asA = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asA).sval = rhs->c[1]->v.sval;
            IR_t * r2 = NULL; IR_t * einit = lower_rcall(cx, rhs, "arr_init", 1, 0, asA, ω, &r2);
            IR_t * asP = build(cx, IR_ASSIGN, einit, ω); IR_LIT(asP).sval = t->c[0]->v.sval;
            IR_t * r3 = NULL; IR_t * elast = lower_rcall(cx, rhs, "arr_last", 1, 0, asP, ω, &r3); *res = asA; return elast; }
        IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = t->c[0]->v.sval;
        IR_t * rr = NULL; IR_t * e = lower_rv(cx, t->c[1], nd, ω, &rr); *res = nd; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_ARR_SET: if (t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR) {
        IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = t->c[0]->v.sval;
        IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "arr_set_pure", 0, 0, as, ω, &r2); *res = as; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_SAY: case TT_SAY_FH: return lower_rcall(cx, t, "write", 0, 1, γ, ω, res);
    case TT_PRINT: case TT_PRINT_FH: return lower_rcall(cx, t, "print", 0, 1, γ, ω, res);
    case TT_FNC: { const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?";
        if (nm && !strcmp(nm, "any")) nm = "__rk_jct_any"; else if (nm && !strcmp(nm, "all")) nm = "__rk_jct_all";
        else if (nm && !strcmp(nm, "one")) nm = "__rk_jct_one"; else if (nm && !strcmp(nm, "none")) nm = "__rk_jct_none";
        if (nm && !strcmp(nm, "push") && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) {
            IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = t->c[1]->v.sval;
            IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "push_pure", 1, 0, as, ω, &r2); *res = as; return e; }
        if (nm && !strcmp(nm, "hash_set") && t->n > 2 && t->c[1] && (t->c[1]->t == TT_VAR || t->c[1]->t == TT_TWIGIL_FIELD)) {
            const char * vn = t->c[1]->v.sval;
            IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
            IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_set_pure", 1, 0, as, ω, &r2); *res = as; return e; }
        if (nm && !strcmp(nm, "hash_delete") && t->n > 1 && t->c[1] && (t->c[1]->t == TT_VAR || t->c[1]->t == TT_TWIGIL_FIELD)) {
            const char * vn = t->c[1]->v.sval;
            IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
            IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_delete_pure", 1, 0, as, ω, &r2); *res = as; return e; }
        return lower_rcall(cx, t, nm, 1, 0, γ, ω, res); }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); return sub ? lower_rv(cx, sub, γ, ω, res) : (build(cx, IR_SUCCEED, γ, ω)); }
    case TT_IF: {
        IR_t * nd = build(cx, IR_IF, γ, ω);
        IR_t * tconj = build(cx, IR_CONJ, γ, ω);
        IR_t * tentry = (t->n > 1) ? lower_rblock(cx, t->c[1], tconj, ω) : tconj;
        IR_t * eentry = NULL;
        if (t->n > 2 && t->c[2]) { IR_t * econj = build(cx, IR_CONJ, γ, ω); eentry = lower_rblock(cx, t->c[2], econj, ω); }
        IR_t * r = NULL; IR_t * centry = lower_rv(cx, t->c[0], tentry, eentry ? eentry : γ, &r);
        ir_operand_push(nd, centry); *res = nd; return centry; }
    case TT_EVERY: if (t->n > 1 && t->c[0] && t->c[0]->t == TT_ITERATE && t->c[0]->n > 0) {
        const tree_t * src = t->c[0]->c[0];
        if (src && src->t == TT_GATHER) {
            IR_t * va = build(cx, IR_ASSIGN, NULL, ω); IR_LIT(va).sval = t->c[0]->v.sval;
            const tree_t * gb = (src->n > 0) ? src->c[0] : NULL;
            int ntk = 0;
            if (gb && gb->t == TT_SEQ_EXPR) { for (int i = 0; i < gb->n; i++) if (gb->c[i] && gb->c[i]->t == TT_SUSPEND) ntk++; }
            else if (gb && gb->t == TT_SUSPEND) ntk = 1;
            IR_t * ga = build(cx, IR_GATHER, va, γ); IR_LIT(ga).ival = ntk ? ntk : ((gb) ? gb->n : 0);
            if (ntk > 0) { IR_graph_t ** subs = (IR_graph_t **) calloc((size_t) ntk, sizeof(IR_graph_t *));
                if (subs) { int k = 0;
                    if (gb->t == TT_SEQ_EXPR) { for (int i = 0; i < gb->n; i++) { const tree_t * s = gb->c[i];
                        if (!s || s->t != TT_SUSPEND || s->n < 1 || !s->c[0]) continue; subs[k++] = rk_arg_block(cx, s->c[0]); } }
                    else if (gb->n >= 1 && gb->c[0]) subs[k++] = rk_arg_block(cx, gb->c[0]);
                    IR_EXEC(ga).counter = (int64_t)(intptr_t) subs; } }
            IR_t * gconj = build(cx, IR_CONJ, ga, ga);
            IR_t * gbentry = lower_rblock(cx, t->c[1], gconj, ga);
            γ_to(va, gbentry); *res = ga; return ga; }
        IR_t * src_nd = lower_rv(cx, t->c[0]->c[0], NULL, γ, NULL);
        const char * lbvar = t->c[0]->v.sval;
        IR_t * va = (lbvar && lbvar[0]) ? build(cx, IR_ASSIGN, NULL, γ) : NULL;
        if (va) IR_LIT(va).sval = lbvar;
        IR_t * gen = src_nd;
        γ_to(gen, va ? va : NULL);
        IR_t * gconj = build(cx, IR_CONJ, gen, gen);
        IR_t * bentry = lower_rblock(cx, t->c[1], gconj, gen);
        if (va) γ_to(va, bentry);
        *res = gen; return gen; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_WHILE: {
        IR_t * nd = build(cx, IR_WHILE, γ, ω);
        IR_t * r = NULL; IR_t * centry = lower_rv(cx, t->c[0], NULL, nd, &r);
        IR_t * conj = build(cx, IR_CONJ, centry, centry);
        IR_t * bentry = (t->n > 1) ? lower_rblock(cx, t->c[1], conj, centry) : conj;
        γ_to(r, bentry); ir_operand_push(nd, centry); *res = nd; return centry; }
    case TT_FOR_RANGE: if (t->n > 3 && t->c[0] && t->c[0]->t == TT_VAR) {
        IR_t * va = build(cx, IR_ASSIGN, NULL, ω); IR_LIT(va).sval = t->c[0]->v.sval;
        IR_t * to = build(cx, IR_TO, va, γ); IR_LIT(to).sval = "ag";
        IR_t * rlo = NULL, * rhi = NULL;
        IR_t * elo = lower_rv(cx, t->c[1], NULL, γ, &rlo);
        IR_t * ehi = lower_rv(cx, t->c[2], to, γ, &rhi);
        γ_to(rlo, ehi); ir_operand_push(to, elo); ir_operand_push(to, ehi);
        IR_t * conj = build(cx, IR_CONJ, to, to);
        IR_t * bentry = lower_rblock(cx, t->c[3], conj, to);
        γ_to(va, bentry); *res = to; return elo; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_SMATCH: if (t->n > 1) {
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "re_match"; IR_LIT(nd).ival = 2;
        lc_call_argblks(nd, 2.0, 2, rk_arg_block, cx, (const tree_t * const *) t->c);
        *res = nd; return nd; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_ARR_GET: return lower_rcall(cx, t, "arr_get", 0, 1, γ, ω, res);
    case TT_HASH_GET: return lower_rcall(cx, t, "hash_get", 0, 1, γ, ω, res);
    case TT_HASH_SET: if (t->n > 2 && t->c[0] && (t->c[0]->t == TT_VAR || t->c[0]->t == TT_TWIGIL_FIELD)) {
        const char * vn = t->c[0]->t == TT_TWIGIL_FIELD ? t->c[0]->v.sval : (t->c[0]->n > 0 && t->c[0]->c[0] ? t->c[0]->c[0]->v.sval : t->c[0]->v.sval);
        IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
        IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_set_pure", 0, 0, as, ω, &r2); *res = as; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_HASH_EXISTS: return lower_rcall(cx, t, "hash_exists", 0, 1, γ, ω, res);
    case TT_HASH_DELETE: if (t->n > 0 && t->c[0] && (t->c[0]->t == TT_VAR || t->c[0]->t == TT_TWIGIL_FIELD)) {
        const char * vn = t->c[0]->t == TT_TWIGIL_FIELD ? t->c[0]->v.sval : (t->c[0]->n > 0 && t->c[0]->c[0] ? t->c[0]->c[0]->v.sval : t->c[0]->v.sval);
        IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
        IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_delete_pure", 0, 0, as, ω, &r2); *res = as; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_TO: if (t->n > 1) { IR_t * to = build(cx, IR_TO, NULL, ω);
        IR_t * rlo = NULL, * rhi = NULL;
        IR_t * elo = lower_rv(cx, t->c[0], NULL, ω, &rlo);
        IR_t * ehi = lower_rv(cx, t->c[1], to, ω, &rhi);
        γ_to(rlo, ehi); ir_operand_push(to, elo); ir_operand_push(to, ehi);
        *res = to; return elo; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_GATHER: { const tree_t * gb = (t->n > 0) ? t->c[0] : NULL;
        int ntk = 0;
        if (gb && gb->t == TT_SEQ_EXPR) { for (int i = 0; i < gb->n; i++) if (gb->c[i] && gb->c[i]->t == TT_SUSPEND) ntk++; }
        else if (gb && gb->t == TT_SUSPEND) ntk = 1;
        IR_t * ga = build(cx, IR_GATHER, NULL, ω); IR_LIT(ga).ival = ntk ? ntk : (gb ? gb->n : 0);
        if (ntk > 0) { IR_graph_t ** subs = (IR_graph_t **) calloc((size_t) ntk, sizeof(IR_graph_t *));
            if (subs) { int k = 0;
                if (gb->t == TT_SEQ_EXPR) { for (int i = 0; i < gb->n; i++) { const tree_t * s = gb->c[i];
                    if (!s || s->t != TT_SUSPEND || s->n < 1 || !s->c[0]) continue; subs[k++] = rk_arg_block(cx, s->c[0]); } }
                else if (gb->n >= 1 && gb->c[0]) subs[k++] = rk_arg_block(cx, gb->c[0]);
                IR_EXEC(ga).counter = (int64_t)(intptr_t) subs; } }
        *res = ga; return ga; }
    case TT_SORT: return lower_rcall(cx, t, "array_sort", 0, 0, γ, ω, res);
    case TT_MAP: if (t->n > 1) { IR_t * nd = build(cx, IR_MAP, γ, ω);
        IR_graph_t * bg = rk_arg_block(cx, t->c[0]); IR_LIT(nd).ival = (long long)(intptr_t) bg;
        IR_graph_t * sg = rk_arg_block(cx, t->c[1]); IR_EXEC(nd).counter = (int64_t)(intptr_t) sg;
        *res = nd; return nd; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_GREP: if (t->n > 1) { IR_t * nd = build(cx, IR_GREP, γ, ω);
        IR_graph_t * bg = rk_arg_block(cx, t->c[0]); IR_LIT(nd).ival = (long long)(intptr_t) bg;
        IR_graph_t * sg = rk_arg_block(cx, t->c[1]); IR_EXEC(nd).counter = (int64_t)(intptr_t) sg;
        *res = nd; return nd; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_CAPTURE: return lower_rcall(cx, t, "re_capture", 0, 0, γ, ω, res);
    case TT_NAMED_CAPTURE: return lower_rcall(cx, t, "re_named_capture", 0, 0, γ, ω, res);
    case TT_SEQ: case TT_PROGRAM: { IR_t * b = lower_rblock(cx, t, γ, ω); *res = b; return b; }
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_raku_enum(const tree_t * prog, const tree_t ** out, int max) {
    int n = 0;
    if (!prog) return 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (d && d->t == TT_SUB_DECL) { if (out && n < max) out[n] = d; n++; }
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_raku_proc(const tree_t * prog, const tree_t * pd) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_RKU); rcx_t cx; cx.g = g;
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_t * fail = IR_node_alloc(g, IR_FAIL);
    IR_t * sentry = succ; IR_t * entry = succ;
    for (int i = (pd ? pd->n : 0) - 1; i >= 1; i--) {
        const tree_t * s = pd->c[i];
        if (!s) continue;
        if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        if (s->t == TT_VAR) continue;
        IR_t * r = NULL; IR_t * e = lower_rv(&cx, s, sentry, fail, &r);
        if (e) { entry = e; sentry = e; }
    }
    g->entry = entry; return g;
}
/*====================================================================================================================================================================================================*/
IR_graph_t * lower_raku(const tree_t * prog) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_RKU);
    rcx_t cx; cx.g = g;
    IR_t * top = IR_node_alloc(g, IR_PROG);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        ir_operand_push(top, lower_decl(&cx, d));
    }
    g->entry = top;
    return g;
}

/*====================================================================================================================*/
/* stage2 entry — relocated from lower_program.c (lower_common rung)                                                  */
/*====================================================================================================================*/
#include "stage2.h"
#include "bb_program.h"
static int lower_raku_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_raku_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_raku_stage2(const tree_t *prog) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_SUB_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_raku_body(prog, proc);
        if (bb_idx >= 0) g_stage2.proc_table[pi].bb_idx = bb_idx;
    }
}
