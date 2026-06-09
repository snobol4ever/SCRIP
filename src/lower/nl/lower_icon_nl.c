#include <string.h>
#include "ast.h"
#include "IR.h"
/*========================================================================================================================*/
typedef struct { IR_graph_t * g; } icx_t;
/*------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
/*------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "α", 3); nd->ω.sz[3] = 0; } }
/*------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = IR_node_alloc(cx->g, op); γ_to(nd, γ); ω_to(nd, ω); return nd; }
/*------------------------------------------------------------------------------------------------------------------------*/
enum { P_K, P_CAP, P_ANY };
typedef struct pat { int op; int tt; const char * sv; int slot; int n; struct pat * sub[6]; } pat;
/*------------------------------------------------------------------------------------------------------------------------*/
static int pmatch(const pat * p, const tree_t * t, const tree_t ** c) { if (p->op == P_ANY) return 1; if (p->op == P_CAP) { if (p->tt >= 0 && (!t || t->t != p->tt)) return 0; c[p->slot] = t; return 1; } if (!t || t->t != p->tt) return 0; if (p->sv && (!t->v.sval || strcmp(t->v.sval, p->sv))) return 0; if (t->n != p->n) return 0; for (int i = 0; i < p->n; i++) if (!pmatch(p->sub[i], t->c[i], c)) return 0; return 1; }
/*------------------------------------------------------------------------------------------------------------------------*/
static int p_bin(const tree_t * t, int k, const tree_t ** c) { static pat a = { P_CAP, -1, 0, 0, 0, {0} }; static pat b = { P_CAP, -1, 0, 1, 0, {0} }; pat root = { P_K, k, 0, 0, 2, { &a, &b } }; return pmatch(&root, t, c); }
/*------------------------------------------------------------------------------------------------------------------------*/
static int p_un(const tree_t * t, int k, const tree_t ** c) { static pat a = { P_CAP, -1, 0, 0, 0, {0} }; pat root = { P_K, k, 0, 0, 1, { &a } }; return pmatch(&root, t, c); }
/*------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { for (int i = 0; i < s->n; i++) { const tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return (a->n > 0) ? a->c[0] : NULL; } return NULL; }
/*------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
static IR_t * lower_decl(icx_t * cx, const tree_t * t);
static IR_t * lower_block(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
static IR_t * lower_call(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
/*------------------------------------------------------------------------------------------------------------------------*/
static void push_kids(icx_t * cx, IR_t * nd, const tree_t * t, int from) { for (int i = from; i < t->n; i++) ir_operand_push(nd, lower(cx, t->c[i], NULL, NULL)); }
/*------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_nary(icx_t * cx, const tree_t * t, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = build(cx, op, γ, ω); push_kids(cx, nd, t, 0); return nd; }
/*------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_binop(icx_t * cx, const tree_t * t, const char * opn, IR_t * γ, IR_t * ω) { const tree_t * c[2]; if (!p_bin(t, t->t, c)) return build(cx, IR_SUCCEED, γ, ω); IR_t * nd = build(cx, IR_BINOP, γ, ω); IR_LIT(nd).sval = opn; ir_operand_push(nd, lower(cx, c[0], NULL, NULL)); ir_operand_push(nd, lower(cx, c[1], NULL, NULL)); return nd; }
/*------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_unop(icx_t * cx, const tree_t * t, const char * opn, IR_t * γ, IR_t * ω) { const tree_t * c[1]; if (!p_un(t, t->t, c)) return build(cx, IR_SUCCEED, γ, ω); IR_t * nd = build(cx, IR_UNOP, γ, ω); IR_LIT(nd).sval = opn; ir_operand_push(nd, lower(cx, c[0], NULL, NULL)); return nd; }
/*========================================================================================================================*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    if (!t) return build(cx, IR_SUCCEED, γ, ω);
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, γ, ω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_CSET: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_NULL: { return build(cx, IR_LIT_NUL, γ, ω); }
    case TT_VAR: { IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_KEYWORD: { IR_t * nd = build(cx, IR_KEYWORD, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval; ir_operand_push(nd, lower(cx, t->c[0], NULL, NULL)); return nd; }
    case TT_ADD: return lower_binop(cx, t, "+", γ, ω);
    case TT_SUB: return lower_binop(cx, t, "-", γ, ω);
    case TT_MUL: return lower_binop(cx, t, "*", γ, ω);
    case TT_DIV: return lower_binop(cx, t, "/", γ, ω);
    case TT_MOD: return lower_binop(cx, t, "%", γ, ω);
    case TT_POW: return lower_binop(cx, t, "^", γ, ω);
    case TT_PLS: return lower_unop(cx, t, "+", γ, ω);
    case TT_MNS: return lower_unop(cx, t, "-", γ, ω);
    case TT_LT: return lower_binop(cx, t, "<", γ, ω);
    case TT_LE: return lower_binop(cx, t, "<=", γ, ω);
    case TT_GT: return lower_binop(cx, t, ">", γ, ω);
    case TT_GE: return lower_binop(cx, t, ">=", γ, ω);
    case TT_EQ: return lower_binop(cx, t, "=", γ, ω);
    case TT_NE: return lower_binop(cx, t, "~=", γ, ω);
    case TT_LLT: return lower_binop(cx, t, "<<", γ, ω);
    case TT_LLE: return lower_binop(cx, t, "<<=", γ, ω);
    case TT_LGT: return lower_binop(cx, t, ">>", γ, ω);
    case TT_LGE: return lower_binop(cx, t, ">>=", γ, ω);
    case TT_LEQ: return lower_binop(cx, t, "==", γ, ω);
    case TT_LNE: return lower_binop(cx, t, "~==", γ, ω);
    case TT_IDENTICAL: return lower_binop(cx, t, "===", γ, ω);
    case TT_NOT: return lower_unop(cx, t, "not", γ, ω);
    case TT_SIZE: return lower_unop(cx, t, "*", γ, ω);
    case TT_NONNULL: return lower_unop(cx, t, "\\", γ, ω);
    case TT_RANDOM: return lower_unop(cx, t, "?", γ, ω);
    case TT_INTERROGATE: return lower_unop(cx, t, "?", γ, ω);
    case TT_MATCH_UNARY: return lower_unop(cx, t, "=", γ, ω);
    case TT_CAT: return lower_binop(cx, t, "||", γ, ω);
    case TT_LCONCAT: return lower_binop(cx, t, "|||", γ, ω);
    case TT_BANG_BINARY: return lower_binop(cx, t, "!", γ, ω);
    case TT_CSET_COMPL: return lower_nary(cx, t, IR_CSET_COMPL, γ, ω);
    case TT_CSET_UNION: return lower_nary(cx, t, IR_CSET_UNION, γ, ω);
    case TT_CSET_DIFF: return lower_nary(cx, t, IR_CSET_DIFF, γ, ω);
    case TT_CSET_INTER: return lower_nary(cx, t, IR_CSET_INTER, γ, ω);
    case TT_FNC: return lower_call(cx, t, γ, ω);
    case TT_IDX: return lower_nary(cx, t, IR_IDX, γ, ω);
    case TT_SECTION: { IR_t * nd = lower_nary(cx, t, IR_SECTION, γ, ω); IR_LIT(nd).ival = 0; return nd; }
    case TT_SECTION_PLUS: { IR_t * nd = lower_nary(cx, t, IR_SECTION, γ, ω); IR_LIT(nd).ival = 1; return nd; }
    case TT_SECTION_MINUS: { IR_t * nd = lower_nary(cx, t, IR_SECTION, γ, ω); IR_LIT(nd).ival = 2; return nd; }
    case TT_MAKELIST: return lower_nary(cx, t, IR_LIST_BANG, γ, ω);
    case TT_VLIST: return lower_nary(cx, t, IR_LIST_BANG, γ, ω);
    case TT_ASSIGN: return lower_nary(cx, t, IR_ASSIGN, γ, ω);
    case TT_AUGOP: { IR_t * nd = lower_nary(cx, t, IR_AUGOP, γ, ω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_SWAP: return lower_nary(cx, t, IR_SWAP, γ, ω);
    case TT_REVASSIGN: { IR_t * nd = lower_nary(cx, t, IR_ASSIGN, γ, ω); IR_LIT(nd).ival = 1; return nd; }
    case TT_REVSWAP: { IR_t * nd = lower_nary(cx, t, IR_SWAP, γ, ω); IR_LIT(nd).ival = 1; return nd; }
    case TT_TO: return lower_nary(cx, t, IR_TO, γ, ω);
    case TT_TO_BY: return lower_nary(cx, t, IR_TO_BY, γ, ω);
    case TT_ALTERNATE: return lower_nary(cx, t, IR_ALT, γ, ω);
    case TT_LIMIT: return lower_nary(cx, t, IR_LIMIT, γ, ω);
    case TT_ITERATE: return lower_nary(cx, t, IR_ITERATE, γ, ω);
    case TT_SEQ_EXPR: return lower_nary(cx, t, IR_SEQ_EXPR, γ, ω);
    case TT_IF: return lower_nary(cx, t, IR_IF, γ, ω);
    case TT_WHILE: return lower_nary(cx, t, IR_WHILE, γ, ω);
    case TT_UNTIL: return lower_nary(cx, t, IR_UNTIL, γ, ω);
    case TT_REPEAT: return lower_nary(cx, t, IR_REPEAT, γ, ω);
    case TT_EVERY: return lower_nary(cx, t, IR_EVERY, γ, ω);
    case TT_CASE: return lower_nary(cx, t, IR_CASE, γ, ω);
    case TT_SUSPEND: return lower_nary(cx, t, IR_SUSPEND, γ, ω);
    case TT_RETURN: return lower_nary(cx, t, IR_RETURN, γ, ω);
    case TT_PROC_FAIL: return build(cx, IR_FAIL, γ, ω);
    case TT_LOOP_BREAK: return build(cx, IR_BREAK, γ, ω);
    case TT_LOOP_NEXT: return build(cx, IR_NEXT, γ, ω);
    case TT_INITIAL: return lower_nary(cx, t, IR_INITIAL, γ, ω);
    case TT_LOCAL: return build(cx, IR_SUCCEED, γ, ω);
    case TT_STATIC_DECL: return build(cx, IR_SUCCEED, γ, ω);
    case TT_SEQ: return lower_block(cx, t, γ, ω);
    case TT_PROGRAM: return lower_block(cx, t, γ, ω);
    case TT_STMT: { const tree_t * sub = stmt_subj(t); return sub ? lower(cx, sub, γ, ω) : build(cx, IR_SUCCEED, γ, ω); }
    case TT_PROC_DECL: return lower_decl(cx, t);
    case TT_RECORD: return lower_decl(cx, t);
    case TT_GLOBAL: return lower_decl(cx, t);
    default: return build(cx, IR_SUCCEED, γ, ω);
    }
}
/*========================================================================================================================*/
static IR_t * lower_block(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
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
/*========================================================================================================================*/
static IR_t * lower_decl(icx_t * cx, const tree_t * t) {
    switch (t->t) {
    case TT_PROC_DECL: { IR_t * nd = IR_node_alloc(cx->g, IR_PROC); IR_LIT(nd).sval = t->v.sval; IR_t * body = (t->n > 2) ? lower_block(cx, t->c[2], NULL, NULL) : build(cx, IR_SUCCEED, NULL, NULL); ir_operand_push(nd, body); return nd; }
    case TT_RECORD: { IR_t * nd = IR_node_alloc(cx->g, IR_RECORD_DEF); IR_LIT(nd).sval = t->v.sval; push_kids(cx, nd, t, 0); return nd; }
    case TT_GLOBAL: { return IR_node_alloc(cx->g, IR_SUCCEED); }
    default: return lower(cx, t, NULL, NULL);
    }
}
/*========================================================================================================================*/
static IR_t * lower_call(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) { IR_t * call = build(cx, IR_CALL, γ, ω); IR_LIT(call).sval = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : 0; IR_LIT(call).ival = t->n - 1; IR_t * succ = call; IR_t * entry = call; for (int i = t->n - 1; i >= 1; i--) { IR_t * arg = lower(cx, t->c[i], succ, ω); succ = arg; entry = arg; } return entry; }
/*------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_proc_body(const tree_t * body) { IR_graph_t * g = IR_alloc(8192, IR_LANG_ICN); icx_t cx; cx.g = g; IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); IR_t * PFAIL = IR_node_alloc(g, IR_FAIL); IR_t * next = PSUCC; int is_last = 1; for (int i = body->n - 1; i >= 0; i--) { const tree_t * s = body->c[i]; if (!s) continue; if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; } IR_t * fw = is_last ? PFAIL : next; next = lower(&cx, s, next, fw); is_last = 0; } g->entry = next; return g; }
/*========================================================================================================================*/
static const tree_t * find_proc(const tree_t * t) { if (!t) return 0; if (t->t == TT_STMT) return find_proc(stmt_subj(t)); if (t->t == TT_PROC_DECL) return t; for (int i = 0; i < t->n; i++) { const tree_t * p = find_proc(t->c[i]); if (p) return p; } return 0; }
/*------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_icon(const tree_t * prog) { const tree_t * pd = find_proc(prog); if (pd && pd->n > 2 && pd->c[2]) return lower_proc_body(pd->c[2]); IR_graph_t * g = IR_alloc(64, IR_LANG_ICN); icx_t cx; cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s; return g; }
