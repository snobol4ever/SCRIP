#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
/*====================================================================================================================================================================================================*/
typedef struct { IR_graph_t * g; } rcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RK_GRAM_MAX 64
static const char * g_rk_gram_names[RK_GRAM_MAX];
static int          g_rk_gram_n = 0;
static const char * g_rk_class_names[RK_GRAM_MAX];
static int          g_rk_class_n = 0;
static char         g_rk_multi_names[RK_GRAM_MAX][128];
static int          g_rk_multi_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_multi_name(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_rk_multi_n; i++) if (!strcmp(g_rk_multi_names[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_multi_name_add(const char * base) { if (!base || rk_is_multi_name(base) || g_rk_multi_n >= RK_GRAM_MAX) return; snprintf(g_rk_multi_names[g_rk_multi_n++], 128, "%s", base); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_grammar_name(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_rk_gram_n; i++) if (!strcmp(g_rk_gram_names[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_class_name(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_rk_class_n; i++) if (!strcmp(g_rk_class_names[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_modeled_type(const char * ty) { if (!ty) return 0; static const char * k[] = { "Int", "Num", "Rat", "Str", "Numeric", "Real", "Cool", "Bool", 0 }; for (int i = 0; k[i]; i++) if (!strcmp(ty, k[i])) return 1; return rk_is_class_name(ty); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_method_is_stub(const tree_t * m) { if (!m || m->t != TT_SUB_DECL) return 0; int bs = (int) m->v.ival; if (bs < 1) bs = 1; if (m->n - bs != 1) return 0; const tree_t * b = m->c[bs]; return b && b->t == TT_YADA; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_fld_bare(const char * s) { return (s && (s[0] == '.' || s[0] == '!')) ? s + 1 : s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_fld_priv(const char * s) { return (s && s[0] == '!') ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_find_type_decl(const tree_t * prog, const char * name) {
    if (!prog || !name) return NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * tn = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (tn && !strcmp(tn, name)) return d;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_type_provides_real_method(const tree_t * decl, const char * mname) {
    if (!decl || !mname) return 0;
    for (int j = 1; j < decl->n; j++) {
        const tree_t * ch = decl->c[j];
        if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
        const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
        if (nm && !strcmp(nm, mname)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(rcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*====================================================================================================================================================================================================*/
static int rk_is_binop(tree_e tt) {
    switch (tt) { case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_CAT: case TT_LEQ: case TT_LNE: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_relop(tree_e tt) {
    switch (tt) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_LEQ: case TT_LNE: return 1; default: return 0; }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
static IR_t * lower_decl(rcx_t * cx, const tree_t * t);
static IR_t * lower_block(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static int rk_proc_known(const char * name);
/*====================================================================================================================================================================================================*/
static void push_kids(rcx_t * cx, IR_t * nd, const tree_t * t, int from) { for (int i = from; i < t->n; i++) ir_operand_push(nd, lower(cx, t->c[i], NULL, NULL)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_cond_wrap(rcx_t * cx, IR_t * r, IR_t * on_true, IR_t * on_false) {
    IR_t * bk = IR_node_alloc(cx->g, IR_CALL); IR_LIT(bk).sval = "__rk_bool"; IR_LIT(bk).ival = 1; IR_LIT(bk).dval = 0.0;
    bk->γ.node = on_true; bk->ω.node = on_false; ir_operand_push(bk, r); return bk;
}
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
/*====================================================================================================================================================================================================*/
static IR_t * lower(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    if (!t) return build(cx, IR_SUCCEED, γ, ω);
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_NUL: return build(cx, IR_LIT_NUL, γ, ω);
    case TT_VAR: { if (rk_is_grammar_name(t->v.sval) || rk_is_class_name(t->v.sval)) { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; } IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_FIELD: { const char * fname = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "field_get_pub"; IR_LIT(nd).ival = 2; IR_LIT(nd).dval = 1.0;
        IR_t * r = NULL; IR_t * nl = build(cx, IR_LIT_STRING, nd, ω); IR_LIT(nl).sval = fname; IR_t * eo = lower_rv(cx, t->c[0], nl, ω, &r); return eo; }
    case TT_TWIGIL_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = t->v.sval; IR_t * sv = IR_node_alloc(cx->g, IR_VAR); IR_LIT(sv).sval = "self"; ir_operand_push(nd, sv); return nd; }
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
    case TT_METHCALL: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "meth_call"; return nd; }
    case TT_NEW: { IR_t * nd = lower_nary(cx, t, IR_CALL, γ, ω); IR_LIT(nd).sval = "obj_new"; return nd; }
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
    case TT_SMATCH: return lower_nary(cx, t, IR_MATCH, γ, ω);
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
    case TT_CLASS_DECL: { IR_t * nd = IR_node_alloc(cx->g, IR_RECORD_DEF);
        const char * cname = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : "";
        char spec[512]; int pos = 0;
        pos += snprintf(spec + pos, sizeof(spec) - pos, "%s(", cname);
        int first_field = 1;
        for (int i = 1; i < t->n; i++) {
            const tree_t * ch = t->c[i];
            if (!ch || ch->t == TT_SUB_DECL) continue;
            if (!first_field) { if (pos < (int)sizeof(spec) - 2) spec[pos++] = ','; }
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            pos += snprintf(spec + pos, sizeof(spec) - pos, "%s", fn);
            first_field = 0;
        }
        if (pos < (int)sizeof(spec) - 1) spec[pos++] = ')';
        spec[pos] = '\0';
        IR_LIT(nd).sval = lp_strdup(spec);
        return nd; }
    default: return lower(cx, t, NULL, NULL);
    }
}
/*====================================================================================================================================================================================================*/
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (rk_is_binop(t->t)) {
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower_rv(cx, t->c[0], NULL, ω, &lr); IR_t * eb = lower_rv(cx, t->c[1], op, ω, &rr);
        γ_to(lr, eb); { IR_t * ax[2]; ax[0] = lr; ax[1] = rr; bb_operand_aux_set(cx->g, op, ax, 2); } *res = op; return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_NUL: { IR_t * nd = build(cx, IR_LIT_NUL, γ, ω); *res = nd; return nd; }
    case TT_VAR: { if (rk_is_grammar_name(t->v.sval) || rk_is_class_name(t->v.sval)) { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; } IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_ASSIGN: if (t->n > 1 && t->c[0] && (t->c[0]->t == TT_FIELD || t->c[0]->t == TT_TWIGIL_FIELD)) {
        const tree_t * lhs = t->c[0];
        const char * fname = (lhs->t == TT_TWIGIL_FIELD) ? lhs->v.sval : ((lhs->n > 1 && lhs->c[1]) ? lhs->c[1]->v.sval : lhs->v.sval);
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = (lhs->t == TT_TWIGIL_FIELD) ? "field_set" : "field_set_pub"; IR_LIT(nd).ival = 3; IR_LIT(nd).dval = 1.0;
        IR_t * succ = nd; IR_t * entry = nd; IR_t * r = NULL;
        IR_t * ev = lower_rv(cx, t->c[1], succ, ω, &r); succ = ev; entry = ev;
        IR_t * nl = build(cx, IR_LIT_STRING, succ, ω); IR_LIT(nl).sval = fname; succ = nl; entry = nl;
        if (lhs->t == TT_TWIGIL_FIELD) { IR_t * sv = build(cx, IR_VAR, succ, ω); IR_LIT(sv).sval = "self"; succ = sv; entry = sv; }
        else { IR_t * eo = lower_rv(cx, lhs->c[0], succ, ω, &r); succ = eo; entry = eo; }
        *res = nd; return entry; }
        else if (t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR) {
        const tree_t * rhs = t->c[1];
        if (rhs && rhs->t == TT_FNC && rhs->n > 1 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "pop") && rhs->c[1] && rhs->c[1]->t == TT_VAR) {
            IR_t * asA = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asA).sval = rhs->c[1]->v.sval;
            IR_t * r2 = NULL; IR_t * einit = lower_rcall(cx, rhs, "arr_init", 1, 0, asA, ω, &r2);
            IR_t * asP = build(cx, IR_ASSIGN, einit, ω); IR_LIT(asP).sval = t->c[0]->v.sval;
            IR_t * r3 = NULL; IR_t * elast = lower_rcall(cx, rhs, "arr_last", 1, 0, asP, ω, &r3); *res = asA; return elast; }
        IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = t->c[0]->v.sval;
        if (t->c[1] && rk_is_relop(t->c[1]->t)) {
            IR_t * lit1 = IR_node_alloc(cx->g, IR_LIT_INTEGER); IR_LIT(lit1).ival = 1; lit1->γ.node = nd; lit1->ω.node = ω;
            IR_t * lit0 = IR_node_alloc(cx->g, IR_LIT_INTEGER); IR_LIT(lit0).ival = 0; lit0->γ.node = nd; lit0->ω.node = ω;
            IR_t * bk = IR_node_alloc(cx->g, IR_CALL); IR_LIT(bk).sval = "__rk_bool"; IR_LIT(bk).ival = 1; IR_LIT(bk).dval = 2.0;
            bk->γ.node = lit1; bk->ω.node = lit0;
            IR_graph_t * cblk = rk_arg_block(cx, t->c[1]);
            IR_graph_t ** blks = (IR_graph_t **) calloc(1, sizeof(IR_graph_t *)); blks[0] = cblk;
            (void)(blks);
            *res = nd; return bk; }
        IR_t * rr = NULL; IR_t * e = lower_rv(cx, t->c[1], nd, ω, &rr); *res = nd; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_ARR_SET: if (t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR) {
        IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = t->c[0]->v.sval;
        IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "arr_set_pure", 0, 0, as, ω, &r2); *res = as; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_SAY: case TT_SAY_FH: return lower_rcall(cx, t, "write", 0, 1, γ, ω, res);
    case TT_PRINT: case TT_PRINT_FH: return lower_rcall(cx, t, "print", 0, 1, γ, ω, res);
    case TT_DIE: return lower_rcall(cx, t, "die", 0, 1, γ, ω, res);
    case TT_TRY: { int has_catch = (t->n > 1 && t->c[1] != NULL);
        int na = has_catch ? 2 : 1;
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_try"; IR_LIT(nd).ival = na;
        IR_LIT(nd).dval = 2.0;
        IR_graph_t ** blks2 = (IR_graph_t **) calloc((size_t) na, sizeof(IR_graph_t *));
        if (blks2) {
            IR_graph_t * saved = cx->g;
            IR_graph_t * g0 = IR_alloc(256, IR_LANG_RKU); cx->g = g0;
            IR_t * F0 = IR_node_alloc(g0, IR_FAIL); IR_t * S0 = IR_node_alloc(g0, IR_SUCCEED);
            IR_t * e0 = (t->n > 0 && t->c[0]) ? lower_rblock(cx, t->c[0], S0, F0) : S0;
            g0->entry = e0; blks2[0] = g0;
            if (has_catch) {
                IR_graph_t * g1 = IR_alloc(256, IR_LANG_RKU); cx->g = g1;
                IR_t * F1 = IR_node_alloc(g1, IR_FAIL); IR_t * S1 = IR_node_alloc(g1, IR_SUCCEED);
                IR_t * e1 = lower_rblock(cx, t->c[1], S1, F1);
                g1->entry = e1; blks2[1] = g1;
            }
            cx->g = saved;
            (void)(blks2);
        }
        *res = nd; return nd; }
    case TT_FNC: { const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?";
        if (nm && rk_is_multi_name(nm)) {
            tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *)"__multi_call";
            tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *)"__multi_call"; ast_push(mc, nmv);
            tree_t * basq = ast_node_new(TT_QLIT); basq->v.sval = (char *)nm; ast_push(mc, basq);
            for (int i = 1; i < t->n; i++) ast_push(mc, t->c[i]);
            return lower_rcall(cx, mc, "__multi_call", 1, 1, γ, ω, res); }
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
        IR_t * e = lower_rcall(cx, t, nm, 1, 0, γ, ω, res);
        if (res && *res && rk_proc_known(nm)) IR_LIT(*res).dval = 3.0;
        return e; }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); return sub ? lower_rv(cx, sub, γ, ω, res) : (build(cx, IR_SUCCEED, γ, ω)); }
    case TT_IF: {
        IR_t * tconj = build(cx, IR_CONJ, γ, ω);
        IR_t * tentry = (t->n > 1) ? lower_rblock(cx, t->c[1], tconj, ω) : tconj;
        IR_t * eentry = γ;
        if (t->n > 2 && t->c[2]) { IR_t * econj = build(cx, IR_CONJ, γ, ω); eentry = lower_rblock(cx, t->c[2], econj, ω); }
        IR_t * bk = IR_node_alloc(cx->g, IR_CALL); IR_LIT(bk).sval = "__rk_bool"; IR_LIT(bk).ival = 1; IR_LIT(bk).dval = 2.0;
        bk->γ.node = tentry; bk->ω.node = eentry;
        IR_graph_t * cblk = rk_arg_block(cx, t->c[0]);
        IR_graph_t ** blks = (IR_graph_t **) calloc(1, sizeof(IR_graph_t *)); blks[0] = cblk;
        (void)(blks);
        *res = bk; return bk; }
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
                    (void)(subs); } }
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
        IR_t * r = NULL; IR_t * centry = lower_rv(cx, t->c[0], NULL, NULL, &r);
        IR_t * conj = build(cx, IR_CONJ, centry, centry);
        IR_t * bentry = (t->n > 1) ? lower_rblock(cx, t->c[1], conj, centry) : conj;
        IR_t * bk = rk_cond_wrap(cx, r, bentry, nd);
        γ_to(r, bk); ir_operand_push(nd, centry); *res = nd; return centry; }
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
                (void)(subs); } }
        *res = ga; return ga; }
    case TT_SORT: return lower_rcall(cx, t, "array_sort", 0, 0, γ, ω, res);
    case TT_MAP: if (t->n > 1) { IR_t * nd = build(cx, IR_MAP, γ, ω);
        IR_graph_t * bg = rk_arg_block(cx, t->c[0]); (void)(bg);
        IR_graph_t * sg = rk_arg_block(cx, t->c[1]); (void)(sg);
        *res = nd; return nd; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_GREP: if (t->n > 1) { IR_t * nd = build(cx, IR_GREP, γ, ω);
        IR_graph_t * bg = rk_arg_block(cx, t->c[0]); (void)(bg);
        IR_graph_t * sg = rk_arg_block(cx, t->c[1]); (void)(sg);
        *res = nd; return nd; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_CAPTURE: return lower_rcall(cx, t, "re_capture", 0, 0, γ, ω, res);
    case TT_NAMED_CAPTURE: return lower_rcall(cx, t, "re_named_capture", 0, 0, γ, ω, res);
    case TT_SEQ: case TT_PROGRAM: { IR_t * b = lower_rblock(cx, t, γ, ω); *res = b; return b; }
    case TT_METHCALL: return lower_rcall(cx, t, "meth_call", 0, 1, γ, ω, res);
    case TT_NEW: return lower_rcall(cx, t, "obj_new", 0, 1, γ, ω, res);
    case TT_TWIGIL_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = t->v.sval; IR_t * sv = build(cx, IR_VAR, nd, ω); IR_LIT(sv).sval = "self"; ir_operand_push(nd, sv); *res = nd; return sv; }
    case TT_FIELD: { const char * fname = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "field_get_pub"; IR_LIT(nd).ival = 2; IR_LIT(nd).dval = 1.0;
        IR_t * r = NULL; IR_t * nl = build(cx, IR_LIT_STRING, nd, ω); IR_LIT(nl).sval = fname; IR_t * eo = lower_rv(cx, t->c[0], nl, ω, &r); *res = nd; return eo; }
    case TT_RETURN: { if (t->n > 0 && t->c[0]) { IR_t * nd = build(cx, IR_RETURN, γ, ω); IR_t * r = NULL; IR_t * e = lower_rv(cx, t->c[0], nd, ω, &r); ir_operand_push(nd, r ? r : e); *res = nd; return e; } IR_t * nd = build(cx, IR_RETURN, γ, ω); *res = nd; return nd; }
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_proc(const tree_t * proc, const char * name, int nparams) {
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name     = lp_strdup(name);
    g_stage2.proc_table[pi].proc     = (tree_t *)(intptr_t) proc;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx   = -1;
    g_stage2.proc_table[pi].nparams  = nparams;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_proc_known(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++)
        if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_grammars(const tree_t * prog) {
    extern void rt_grammar_register(const char *qname, const char *body, int flavor);
    g_rk_gram_n = 0;
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_GRAMMAR_DECL) continue;
        const char * gname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!gname || !*gname) continue;
        if (g_rk_gram_n < RK_GRAM_MAX && !rk_is_grammar_name(gname)) g_rk_gram_names[g_rk_gram_n++] = gname;
        for (int j = 1; j < d->n; j++) {
            const tree_t * rd = d->c[j];
            if (!rd || rd->t != TT_REGEX_DECL) continue;
            const char * rname = (rd->n > 0 && rd->c[0] && rd->c[0]->v.sval) ? rd->c[0]->v.sval : NULL;
            const char * body  = (rd->n > 1 && rd->c[1] && rd->c[1]->v.sval) ? rd->c[1]->v.sval : NULL;
            if (!rname || !body) continue;
            char qn[256]; snprintf(qn, sizeof qn, "%s::%s", gname, rname);
            rt_grammar_register(qn, body, (int) rd->v.ival);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_classes(const tree_t * prog) {
    extern void record_register(const char *spec);
    if (!prog) return;
    g_rk_class_n = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!cname || !*cname) continue;
        if (g_rk_class_n < RK_GRAM_MAX && !rk_is_class_name(cname)) g_rk_class_names[g_rk_class_n++] = cname;
        char spec[512]; int pos = 0;
        pos += snprintf(spec + pos, sizeof(spec) - pos, "%s(", cname);
        int first_field = 1;
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t == TT_SUB_DECL) continue;
            if (!first_field) { if (pos < (int)sizeof(spec) - 2) spec[pos++] = ','; }
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            pos += snprintf(spec + pos, sizeof(spec) - pos, "%s", fn);
            first_field = 0;
        }
        if (pos < (int)sizeof(spec) - 1) spec[pos++] = ')';
        spec[pos] = '\0';
        record_register(spec);
        extern void dat_add_method(const char *type, const char *mname);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
            const char * mname = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
            if (!mname) continue;
            const char * dollar = strchr(mname, '$');
            if (dollar) { char base[128]; int bl = (int)(dollar - mname); if (bl > 127) bl = 127; memcpy(base, mname, bl); base[bl] = '\0'; dat_add_method(cname, base); }
            else dat_add_method(cname, mname);
            if (!strcmp(mname, "BUILD")) {
                extern void dat_set_build_key(const char *cls, const char *key);
                int bs = (int)ch->v.ival; if (bs < 1) bs = 1; dat_set_build_key(cname, NULL);
                for (int p = 1; p < bs; p++) { const tree_t * pp = ch->c[p]; const char * pk = (pp && pp->v.sval) ? pp->v.sval : NULL; if (pk && *pk) dat_set_build_key(cname, pk); }
            }
        }
        extern void dat_set_field_default_i(const char *cls, const char *field, int64_t v);
        extern void dat_set_field_default_s(const char *cls, const char *field, const char *v);
        extern void dat_set_field_default_r(const char *cls, const char *field, double v);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HAS_DECL || ch->n < 1) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); const tree_t * dv = ch->c[0]; if (!dv) continue;
            if (dv->t == TT_ILIT) dat_set_field_default_i(cname, fn, dv->v.ival);
            else if (dv->t == TT_QLIT) dat_set_field_default_s(cname, fn, dv->v.sval);
            else if (dv->t == TT_FLIT) dat_set_field_default_r(cname, fn, dv->v.dval);
        }
        extern void dat_set_field_required(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HAS_DECL || ch->n != 0) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_required(cname, fn);
        }
        extern void dat_set_field_rw(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_RW_DECL) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_rw(cname, fn);
        }
        extern void dat_add_handles(const char *cls, const char *meth, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HANDLES_DECL) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (!*fn) continue;
            const char * words = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : "";
            char wbuf[256]; snprintf(wbuf, sizeof wbuf, "%s", words); char * sp = wbuf; char * tok;
            while ((tok = strtok(sp, " \t")) != (char *)0) { sp = (char *)0; if (*tok) dat_add_handles(cname, tok, fn); }
        }
        extern void dat_set_field_sigil(const char *cls, const char *field, int sig);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || (ch->t != TT_ARR_DECL && ch->t != TT_HASH_DECL)) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_sigil(cname, fn, ch->t == TT_ARR_DECL ? '@' : '%');
        }
        extern void dat_set_field_priv(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t == TT_SUB_DECL) continue;
            if (!rk_fld_priv(ch->v.sval)) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_priv(cname, fn);
        }
    }
    extern void class_inherit_multi(const char *child, const char **parents, int nparents);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_CLASS_DECL) continue;
        const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        const char * pname = d->v.sval;
        if (cname && pname && *pname) {
            char pbuf[8][64]; const char * pl[8]; int np = 0; char rbuf[8][64]; int nr = 0; const char * s = pname;
            while (*s) { const char * nx = strchr(s, '\x01'); size_t L = nx ? (size_t)(nx - s) : strlen(s);
                if (L >= 1) { char tag = s[0]; const char * nm = s + 1; size_t NL = L - 1; if (NL > 63) NL = 63;
                    if (tag == 'i' && np < 8) { memcpy(pbuf[np], nm, NL); pbuf[np][NL] = '\0'; pl[np] = pbuf[np]; np++; }
                    else if (tag == 'd' && nr < 8) { memcpy(rbuf[nr], nm, NL); rbuf[nr][NL] = '\0'; nr++; } }
                if (!nx) break; s = nx + 1; }
            if (np > 0) class_inherit_multi(cname, pl, np);
            extern void class_compose_role(const char *child, const char *role);
            for (int ri = 0; ri < nr; ri++) class_compose_role(cname, rbuf[ri]);
            extern void rt_script_die_surface(const char *msg);
            const tree_t * cdecl = rk_find_type_decl(prog, cname);
            for (int ri = 0; ri < nr; ri++) {
                const tree_t * rdecl = rk_find_type_decl(prog, rbuf[ri]); if (!rdecl) continue;
                for (int j = 1; j < rdecl->n; j++) {
                    const tree_t * ch = rdecl->c[j];
                    if (!ch || ch->t != TT_SUB_DECL || !rk_method_is_stub(ch)) continue;
                    const char * rm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                    if (!rm) continue;
                    int sat = rk_type_provides_real_method(cdecl, rm);
                    for (int rj = 0; rj < nr && !sat; rj++) { const tree_t * od = rk_find_type_decl(prog, rbuf[rj]); if (rk_type_provides_real_method(od, rm)) sat = 1; }
                    if (!sat) { char _m[256]; snprintf(_m, sizeof _m, "Method '%s' must be implemented by class %s because it is required by role %s", rm, cname, rbuf[ri]); rt_script_die_surface(_m); }
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_procs(const tree_t * prog) {
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d) continue;
        if (d->t == TT_SUB_DECL) {
            const char * nm = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
            if (!nm) continue;
            int np = (int) d->v.ival;
            rk_register_proc(d, nm, np);
        } else if (d->t == TT_CLASS_DECL || d->t == TT_ROLE_DECL) {
            const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
            if (!cname || !*cname) continue;
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
                const char * mname = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                if (!mname) continue;
                char qname[256]; snprintf(qname, sizeof qname, "%s__%s", cname, mname);
                int np = (int) ch->v.ival;
                rk_register_proc(ch, qname, np);
            }
        }
    }
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
    if (pd && !((pd->n > 0 && pd->c[0] && pd->c[0]->v.sval) && strchr(pd->c[0]->v.sval, '$'))) {
        for (int k = pd->n - 1; k >= 1; k--) {
            const tree_t * pv = pd->c[k];
            if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
            if (pv->n < 1 || !pv->c[0] || !pv->c[0]->v.sval) continue;
            const char * ty = pv->c[0]->v.sval;
            if (!strstr(ty, ":D") && !strstr(ty, ":U") && !rk_is_modeled_type(ty)) continue;
            const char * pn = pv->v.sval;
            tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *) "__param_check";
            tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *) "__param_check"; ast_push(mc, nmv);
            tree_t * tyq = ast_node_new(TT_QLIT); tyq->v.sval = (char *) ty; ast_push(mc, tyq);
            tree_t * pvr = ast_node_new(TT_VAR); pvr->v.sval = (char *) pn; ast_push(mc, pvr);
            tree_t * pnq = ast_node_new(TT_QLIT); pnq->v.sval = (char *) pn; ast_push(mc, pnq);
            IR_t * r = NULL; IR_t * e = lower_rcall(&cx, mc, "__param_check", 1, 1, sentry, fail, &r);
            if (e) { entry = e; sentry = e; }
        }
    }
    g->entry = entry; return g;
}
/*====================================================================================================================================================================================================*/
#include "stage2.h"
#include "bb_program.h"
static int lower_raku_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_raku_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_raku_stage2(const tree_t *prog) {
    rk_discover_grammars(prog);
    rk_register_classes(prog);
    g_rk_multi_n = 0;
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_SUB_DECL) continue;
        const char * nm = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!nm) continue;
        const char * soh = strchr(nm, '$'); if (!soh) continue;
        char base[128]; int bl = (int)(soh - nm); if (bl > 127) bl = 127; memcpy(base, nm, bl); base[bl] = 0;
        rk_multi_name_add(base);
    }
    rk_discover_procs(prog);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_SUB_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_raku_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            const char * pname = g_stage2.proc_table[pi].name;
            int is_method = (pname && strchr(pname, '_') && strchr(pname, '_')[1] == '_');
            int param_start = is_method ? 0 : 1;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            if (is_method) {
                sc->e[sc->n].name = lp_strdup("self");
                sc->e[sc->n].slot = sc->n; sc->n++;
                param_start = 1;
            }
            for (int k = 0; k < np && (k + param_start) < proc->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                const tree_t *pv = proc->c[k + param_start];
                if (!pv || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n; sc->n++;
            }
        }
    }
    int has_main = 0;
    for (int pi = 0; pi < g_stage2.proc_count; pi++)
        if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "main") == 0) { has_main = 1; break; }
    if (!has_main) {
        IR_graph_t * tg = IR_alloc(8192, IR_LANG_RKU); rcx_t tcx; tcx.g = tg;
        IR_t * succ = IR_node_alloc(tg, IR_SUCCEED); IR_t * fail = IR_node_alloc(tg, IR_FAIL);
        IR_t * sentry = succ; IR_t * entry = succ;
        for (int i = prog->n - 1; i >= 0; i--) {
            const tree_t * s = prog->c[i];
            if (!s) continue;
            if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
            if (s->t == TT_SUB_DECL || s->t == TT_CLASS_DECL || s->t == TT_ROLE_DECL || s->t == TT_GRAMMAR_DECL) continue;
            IR_t * r = NULL; IR_t * e = lower_rv(&tcx, s, sentry, fail, &r);
            if (e) { entry = e; sentry = e; }
        }
        tg->entry = entry;
        int bb_idx = bb_program_add(&g_stage2.bbp, tg);
        if (bb_idx >= 0) {
            int pi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[pi].name     = lp_strdup("main");
            g_stage2.proc_table[pi].proc     = NULL;
            g_stage2.proc_table[pi].entry_pc = -1;
            g_stage2.proc_table[pi].bb_idx   = bb_idx;
            g_stage2.proc_table[pi].nparams  = 0;
        }
    }
}
