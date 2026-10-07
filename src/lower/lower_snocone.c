#include "ct_arena.h"
#include <string.h>
#include <stdio.h>
#include "lower.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_e lsc_prim_kind(const char *s) {
    if (!s) return TT_VAR;
    static const struct { const char *n; tree_e k; } m[] = {
        {"ANY",TT_ANY},{"NOTANY",TT_NOTANY},{"SPAN",TT_SPAN},{"BREAK",TT_BREAK},{"BREAKX",TT_BREAKX},
        {"LEN",TT_LEN},{"POS",TT_POS},{"RPOS",TT_RPOS},{"TAB",TT_TAB},{"RTAB",TT_RTAB},
        {"ARB",TT_ARB},{"ARBNO",TT_ARBNO},{"REM",TT_REM},{"FAIL",TT_FAIL},{"SUCCEED",TT_SUCCEED},
        {"FENCE",TT_FENCE},{"FLUSH",TT_FLUSH},{"ABORT",TT_ABORT},{"BAL",TT_BAL},{NULL,TT_VAR}
    };
    for (int i = 0; m[i].n; i++) if (strcmp(s, m[i].n) == 0) return m[i].k;
    return TT_VAR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lsc_qlit(const char *txt) {
    tree_t *q = ast_node_new(TT_QLIT);
    q->v.sval = ct_strdup(txt);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *lsc_join_vars(const tree_t *l) {
    size_t len = 1;
    for (int i = 0; i < l->n; i++) len += strlen(l->c[i]->v.sval) + 1;
    char *out = ct_alloc(len);
    out[0] = 0;
    for (int i = 0; i < l->n; i++) { if (i) strcat(out, ","); strcat(out, l->c[i]->v.sval); }
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lsc_expr(tree_t *e)
{
    if (!e) return e;
    for (int i = 0; i < e->n; i++) e->c[i] = lsc_expr(e->c[i]);
    if (e->t == TT_FNC && e->n == 2 && e->c[0]->t == TT_QLIT && e->c[1]->t == TT_ARGS) {
        char   *nm = e->c[0]->v.sval;
        tree_e  k  = lsc_prim_kind(nm);
        tree_t *f  = ast_node_new(k == TT_VAR ? TT_FNC : k);
        if (k == TT_VAR || k == TT_ARB || k == TT_BAL || k == TT_REM || k == TT_FAIL || k == TT_SUCCEED || k == TT_ABORT) f->v.sval = nm;
        for (int i = 0; i < e->c[1]->n; i++) ast_push(f, e->c[1]->c[i]);
        f->line = e->line;
        return f;
    }
    if (e->t == TT_IDX && e->n == 2 && e->c[1]->t == TT_VLIST) {
        tree_t *f = ast_node_new(TT_IDX);
        ast_push(f, e->c[0]);
        for (int i = 0; i < e->c[1]->n; i++) ast_push(f, e->c[1]->c[i]);
        f->line = e->line;
        return f;
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static STMT_t *lsc_row(CODE_t *code, tree_t *node);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lsc_body(CODE_t *code, tree_t *prog)
{
    STMT_t *head = NULL, *tail = NULL;
    for (int i = 0; i < prog->n; i++) {
        STMT_t *s = lsc_row(code, prog->c[i]);
        if (!head) head = s;
        else       tail->next = s;
        tail = s;
    }
    int i = 0;
    for (STMT_t *s = head, *nxt; s; s = nxt) {
        nxt = s->next;
        prog->c[i++] = stmt_to_ast(s);
        ct_drop(s);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static STMT_t *lsc_row(CODE_t *code, tree_t *node)
{
    STMT_t *s = stmt_new();
    s->lineno = node->line;
    if (node->t == TT_LABEL) {
        s->label = ct_strdup(node->c[0]->v.sval);
        s->stno  = ++code->nstmts;
        return s;
    }
    if (node->t == TT_STRUCT) {
        char    *fl  = lsc_join_vars(node->c[1]);
        size_t   len = strlen(node->c[0]->v.sval) + strlen(fl) + 3;
        char    *spec = ct_alloc(len);
        snprintf(spec, len, "%s(%s)", node->c[0]->v.sval, fl);
        tree_t  *call = ast_node_new(TT_FNC);
        call->v.sval = ct_strdup("DATA");
        ast_push(call, lsc_qlit(spec));
        call->line = node->line;
        node = call;
    } else if (node->t == TT_DEFINE) {
        char *al = lsc_join_vars(node->c[1]);
        char *ll = lsc_join_vars(node->c[2]);
        size_t len = strlen(node->c[0]->v.sval) + strlen(al) + strlen(ll) + 3;
        char  *sig = ct_alloc(len);
        snprintf(sig, len, "%s(%s)%s", node->c[0]->v.sval, al, ll);
        lsc_body(code, node->c[3]);
        tree_t *def = ast_node_new(TT_DEFINE);
        ast_push(def, node->c[0]);
        ast_push(def, lsc_qlit(sig));
        ast_push(def, node->c[3]);
        def->line = node->line;
        node = def;
    } else if (node->t == TT_GOTO_U && node->n == 1) {
        node->v.sval = node->c[0]->v.sval;
        node->n = 0;
    } else if (node->t == TT_IF || node->t == TT_WHILE || node->t == TT_DO_WHILE || node->t == TT_FOR || node->t == TT_CASE) {
        for (int i = 0; i < node->n; i++) {
            if (node->c[i]->t == TT_PROGRAM) lsc_body(code, node->c[i]);
            else node->c[i] = lsc_expr(node->c[i]);
        }
    } else {
        node = lsc_expr(node);
    }
    s->subject = node;
    if (node->t == TT_ASSIGN && node->n == 2 && node->c[0] && node->c[1]) {
        tree_t *lhs = node->c[0];
        if (lhs->t == TT_INDIRECT && lhs->n == 1 && lhs->c[0] && lhs->c[0]->t == TT_QLIT && lhs->c[0]->v.sval && lhs->c[0]->v.sval[0] && lhs->c[0]->v.sval[0] != '&') { tree_t *v = ast_node_new(TT_VAR); v->v.sval = ct_strdup(lhs->c[0]->v.sval); lhs = v; }
        if (lhs->t == TT_VAR && lhs->v.sval) { s->subject = lhs; s->replacement = node->c[1]; s->has_eq = 1; }
    }
    s->stno = ++code->nstmts;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static CODE_t *lsc_desugar(tree_t *prog) {
    CODE_t *code = ct_zalloc(1, sizeof *code);
    for (int i = 0; i < prog->n; i++) {
        STMT_t *s = lsc_row(code, prog->c[i]);
        if (!code->head) code->head = s;
        else             code->tail->next = s;
        code->tail = s;
    }
    return code;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *lower_snocone_tree(tree_t *pruned) {
    if (!pruned) return NULL;
    return code_to_ast(lsc_desugar(pruned));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
