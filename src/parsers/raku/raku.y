%define api.prefix {raku_yy}
%code requires {
#include "ast.h"
#include "../snobol4/scrip_cc.h"
typedef struct ExprList {
    tree_t **items;
    int      count;
    int      cap;
} ExprList;
}
%{
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "raku.tab.h"
#include "raku_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  raku_yylex(void);
extern int  raku_get_lineno(void);
void raku_yyerror(const char *msg) {
    fprintf(stderr, "raku parse error line %d: %s\n", raku_get_lineno(), msg);
}
static ExprList *exprlist_new(void) {
    ExprList *l = calloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
static ExprList *exprlist_append(ExprList *l, tree_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = realloc(l->items, l->cap * sizeof(tree_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
static void exprlist_free(ExprList *l) { if (l) { free(l->items); free(l); } }
static const char *strip_sigil(const char *s) {
    if (s && (s[0]=='$'||s[0]=='@'||s[0]=='%')) return s+1;
    return s;
}
static int rk_tw_priv(const char *s) { return (s && s[0]=='!') ? 1 : 0; }
static const char *rk_tw_bare(const char *s) { return (s && (s[0]=='.'||s[0]=='!')) ? s+1 : s; }
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}
static tree_t *var_node(const char *name) {
    return leaf_sval(TT_VAR, strip_sigil(name));
}
static const char *testop_rt(const char *s) {
    if (!s) return "__rk_test_ok";
    if (!strcmp(s, "plan")) return "__rk_test_plan";
    if (!strcmp(s, "ok")) return "__rk_test_ok";
    if (!strcmp(s, "nok")) return "__rk_test_nok";
    if (!strcmp(s, "is")) return "__rk_test_is";
    if (!strcmp(s, "isnt")) return "__rk_test_isnt";
    if (!strcmp(s, "done-testing")) return "__rk_test_done";
    if (!strcmp(s, "skip-rest")) return "__rk_test_skip_rest";
    if (!strcmp(s, "skip")) return "__rk_test_skip";
    if (!strcmp(s, "todo")) return "__rk_test_todo";
    if (!strcmp(s, "diag")) return "__rk_test_diag";
    if (!strcmp(s, "pass")) return "__rk_test_pass";
    if (!strcmp(s, "flunk")) return "__rk_test_flunk";
    return "__rk_test_ok";
}
static tree_t *make_call(const char *name) {
    tree_t *e = leaf_sval(TT_FNC, name);
    tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name);
    expr_add_child(e, n);
    return e;
}
static tree_t *mk_junction(const char *flav, tree_t *l, tree_t *r) {
    tree_t *e = make_call(flav);
    if (l && l->t == TT_FNC && l->v.sval && strcmp(l->v.sval, flav) == 0) {
        for (int i = 1; i < l->n; i++) expr_add_child(e, l->c[i]);
    } else {
        expr_add_child(e, l);
    }
    expr_add_child(e, r);
    return e;
}
static const char *rk_multi_mangle(const char *base, ExprList *params) {
    static char buf[512]; int np = params ? params->count : 0;
    int pos = snprintf(buf, sizeof buf, "%s$%d", base, np);
    for (int i = 0; i < np; i++) { tree_t *p = params->items[i];
        const char *ty = (p && p->n > 0 && p->c[0] && p->c[0]->v.sval) ? p->c[0]->v.sval : "Any";
        if (!strcmp(ty, "*@") || !strcmp(ty, "**@")) ty = "Slurpy";
        char safe[64]; int j = 0;
        for (const char *c = ty; *c && j < 63; c++, j++) safe[j] = (*c == ':') ? '_' : *c; safe[j] = 0;
        pos += snprintf(buf + pos, sizeof buf - pos, "$%s", safe); }
    return intern(buf);
}
static tree_t *rk_typed_param(const char *type, const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, type)); return p;
}
static tree_t *rk_typed_def_param(const char *type, const char *def, const char *name) {
    char buf[160]; snprintf(buf, sizeof buf, "%s%s", type, def);
    return rk_typed_param(intern(buf), name);
}
static tree_t *make_seq(ExprList *stmts) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmts) {
        for (int i = 0; i < stmts->count; i++) expr_add_child(seq, stmts->items[i]);
        exprlist_free(stmts);
    }
    return seq;
}
static tree_t *seq1(tree_t *stmt) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmt) expr_add_child(seq, stmt);
    return seq;
}
static tree_t *rk_cstyle_loop(tree_t *init, tree_t *cond, tree_t *incr, tree_t *body) {
    tree_t *n = ast_node_new(TT_CLOOP);
    expr_add_child(n, init); expr_add_child(n, cond); expr_add_child(n, incr); expr_add_child(n, body);
    return n;
}
static tree_t *rk_incdec(const char *var, int add) {
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_ASSIGN, var_node(var), expr_binary(add ? TT_ADD : TT_SUB, var_node(var), one));
}
static tree_t *rk_post_incdec(const char *var, int add) {
    static int __post_uid = 0; char tmp[32]; snprintf(tmp, sizeof tmp, "__post_%d", __post_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), var_node(var)));
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    expr_add_child(seq, expr_binary(TT_ASSIGN, var_node(var), expr_binary(add ? TT_ADD : TT_SUB, var_node(var), one)));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
}
static tree_t *rk_destructure(ExprList *targets, tree_t *rhs_arr) {
    static int __destr_uid = 0;
    char tmp[32]; snprintf(tmp, sizeof tmp, "__destr_%d", __destr_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    tree_t *bind = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rhs_arr); expr_add_child(seq, bind);
    int n = targets ? targets->count : 0;
    for (int i = 0; i < n; i++) {
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, tmp));
        tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = i; expr_add_child(get, idx);
        expr_add_child(seq, expr_binary(TT_ASSIGN, targets->items[i], get));
    }
    if (targets) exprlist_free(targets);
    return seq;
}
static tree_t *rk_with_mod(tree_t *stmt, tree_t *cond, int negate) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, cond);
    tree_t *dcall = make_call("__rk_defined"); expr_add_child(dcall, leaf_sval(TT_VAR, "_"));
    tree_t *gate = ast_node_new(negate ? TT_UNLESS : TT_IF); expr_add_child(gate, dcall); expr_add_child(gate, seq1(stmt));
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, gate);
    return seq;
}
static tree_t *rk_given_mod(tree_t *stmt, tree_t *topicval) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, topicval);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, stmt);
    return seq;
}
static tree_t *rk_range_ex(tree_t *lo, tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return expr_binary(TT_TO, lo, d); }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
}
static tree_t *rk_arr_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_TO || rhs->n < 2) return rhs;
    tree_t *call = make_call("__rk_range_arr"); expr_add_child(call, rhs->c[0]); expr_add_child(call, rhs->c[1]);
    return call;
}
static tree_t *rk_arr_index(const char *arr, tree_t *idx) {
    if (idx && idx->t == TT_TO && idx->n >= 2) {
        tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, idx->c[0]); expr_add_child(call, idx->c[1]);
        return call;
    }
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, idx); return c;
}
static tree_t *rk_arr_pick(const char *arr, tree_t *i0, ExprList *rest) {
    tree_t *call = make_call("__rk_arr_pick"); expr_add_child(call, var_node(arr)); expr_add_child(call, i0);
    if (rest) { for (int i = 0; i < rest->count; i++) expr_add_child(call, rest->items[i]); exprlist_free(rest); }
    return call;
}
static tree_t *rk_arr_end_index(const char *arr, tree_t *off, tree_e op) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, expr_binary(op, el, off)); return c;
}
static tree_t *rk_dec(tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return d; }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_SUB, hi, one);
}
static tree_t *rk_arr_all(const char *arr) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *lo = ast_node_new(TT_ILIT); lo->v.ival = 0;
    tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, lo); expr_add_child(call, rk_dec(el));
    return call;
}
static tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
}
static tree_t *rk_slurpy_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("*@"))); return p;
}
static tree_t *rk_slurpy_lol_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("**@"))); return p;
}
static tree_t *rk_slurpy_named_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("*%"))); return p;
}
static tree_t *rk_param_default(tree_t *p, tree_t *dflt) {
    return expr_binary(TT_ASSIGN, p, dflt);
}
static tree_t *rk_scalar_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_XREP || rhs->n < 2) return rhs;
    tree_t *c = make_call("__rk_rep"); expr_add_child(c, rhs->c[0]); expr_add_child(c, rhs->c[1]); return c;
}
static tree_t *rk_named_call(const char *fname, ExprList *pos, ExprList *named) {
    tree_t *c = make_call("__rk_named_call");
    expr_add_child(c, leaf_sval(TT_QLIT, fname));
    tree_t *n = ast_node_new(TT_ILIT); n->v.ival = pos ? pos->count : 0;
    expr_add_child(c, n);
    if (pos) { for (int i = 0; i < pos->count; i++) expr_add_child(c, pos->items[i]); exprlist_free(pos); }
    if (named) { for (int i = 0; i < named->count; i++) expr_add_child(c, named->items[i]); exprlist_free(named); }
    return c;
}
static tree_t *rk_defaults_prologue(ExprList *params, tree_t *body) {
    if (!params) return body;
    ExprList *pro = NULL;
    for (int i = 0; i < params->count; i++) {
        tree_t *p = params->items[i];
        if (!p || p->t != TT_ASSIGN || p->n < 2) continue;
        tree_t *pv = p->c[0]; tree_t *dv = p->c[1];
        params->items[i] = pv;
        tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, rk_tree_clone(pv)); ast_push(mc, leaf_sval(TT_QLIT, "defined"));
        tree_t *un = ast_node_new(TT_UNLESS); ast_push(un, mc); ast_push(un, seq1(expr_binary(TT_ASSIGN, rk_tree_clone(pv), dv)));
        if (!pro) pro = exprlist_new();
        exprlist_append(pro, un);
    }
    if (!pro) return body;
    for (int i = 0; body && i < body->n; i++) exprlist_append(pro, body->c[i]);
    return make_seq(pro);
}
static int rk_is_chain_cmp(tree_e k) {
    return k == TT_LT || k == TT_GT || k == TT_LE || k == TT_GE || k == TT_EQ || k == TT_NE || k == TT_LEQ || k == TT_LNE;
}
static tree_t *rk_chain_last_operand(tree_t *left) {
    if (!left) return NULL;
    if (rk_is_chain_cmp(left->t) && left->n == 2) return expr_right(left);
    if (left->t == TT_SEQ && left->n == 2) return rk_chain_last_operand(expr_right(left));
    return NULL;
}
static tree_t *rk_chain_cmp(tree_t *left, tree_e op, tree_t *right) {
    tree_t *last = rk_chain_last_operand(left);
    if (last) return expr_binary(TT_SEQ, left, expr_binary(op, rk_tree_clone(last), right));
    return expr_binary(op, left, right);
}
/* ⭐ construct (f), raku-frontend-real-world-syntax-gaps: a bounded hand-rolled subscript-expression
   scanner for "@arr[expr]" interpolation, per the task's own catalog note that the alternative (a
   reentrant flex sub-parse) needed its reentrancy risk "not yet assessed" -- this avoids that risk
   entirely by never re-entering the lexer/parser. Deliberately narrow: a $name/@name/integer primary,
   optionally one +-*-/-% operator and a second primary -- measured against every "@name[...]" inside a
   "..." string across all 17 corpus/benchmarks/raku kernels (spinner's "$_ % @spinner" is the ONLY one),
   not guessed at a wider generality nothing in the corpus needs. */
static tree_t *rk_interp_primary(const char *s, int *ip, int len) {
    int i = *ip;
    while (i<len && s[i]==' ') i++;
    if (i<len && (s[i]=='$'||s[i]=='@')) {
        i++;
        char nm[256]; int nl=0;
        while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
            { if(nl<255) nm[nl++]=s[i]; i++; }
        nm[nl]='\0'; *ip=i;
        return leaf_sval(TT_VAR,nm);
    }
    if (i<len && s[i]>='0' && s[i]<='9') {
        long v=0;
        while (i<len && s[i]>='0' && s[i]<='9') { v=v*10+(s[i]-'0'); i++; }
        *ip=i;
        tree_t *lit=ast_node_new(TT_ILIT); lit->v.ival=v; return lit;
    }
    *ip=i; return NULL;
}
static tree_t *rk_interp_subexpr(const char *s, int *ip, int len) {
    tree_t *left = rk_interp_primary(s,ip,len);
    if (!left) return NULL;
    int i = *ip;
    while (i<len && s[i]==' ') i++;
    if (i<len && (s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='%')) {
        char op = s[i]; i++; *ip=i;
        tree_t *right = rk_interp_primary(s,ip,len);
        if (!right) return left;
        tree_e k = op=='+'?TT_ADD:op=='-'?TT_SUB:op=='*'?TT_MUL:op=='/'?TT_DIV:TT_MOD;
        return expr_binary(k,left,right);
    }
    *ip=i; return left;
}
static tree_t *lower_interp_str(const char *s) {
    int len = s ? (int)strlen(s) : 0;
    tree_t *result = NULL;
    char litbuf[4096]; int litpos = 0, i = 0;
    while (i < len) {
        if (s[i]=='$' && i+1<len &&
            (s[i+1]=='_'||(s[i+1]>='A'&&s[i+1]<='Z')||(s[i+1]>='a'&&s[i+1]<='z'))) {
            if (litpos>0) { litbuf[litpos]='\0';
                tree_t *lit=leaf_sval(TT_QLIT,litbuf);
                result=result?expr_binary(TT_CAT,result,lit):lit; litpos=0; }
            i++;
            char vname[256]; int vlen=0;
            while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
                { if(vlen<255) vname[vlen++]=s[i]; i++; }
            vname[vlen]='\0';
            tree_t *var=leaf_sval(TT_VAR,vname);
            result=result?expr_binary(TT_CAT,result,var):var;
        } else if (s[i]=='@' && i+1<len &&
            (s[i+1]=='_'||(s[i+1]>='A'&&s[i+1]<='Z')||(s[i+1]>='a'&&s[i+1]<='z'))) {
            if (litpos>0) { litbuf[litpos]='\0';
                tree_t *lit=leaf_sval(TT_QLIT,litbuf);
                result=result?expr_binary(TT_CAT,result,lit):lit; litpos=0; }
            i++;
            char vname[256]; int vlen=0;
            while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
                { if(vlen<255) vname[vlen++]=s[i]; i++; }
            vname[vlen]='\0';
            tree_t *arrpart;
            if (i<len && s[i]=='[') {
                i++;
                tree_t *idx = rk_interp_subexpr(s,&i,len);
                while (i<len && s[i]!=']') i++;
                if (i<len && s[i]==']') i++;
                arrpart = idx ? rk_arr_index(vname,idx) : leaf_sval(TT_VAR,vname);
            } else { arrpart = leaf_sval(TT_VAR,vname); }
            result=result?expr_binary(TT_CAT,result,arrpart):arrpart;
        } else { if(litpos<4095) litbuf[litpos++]=s[i]; i++; }
    }
    if (litpos>0) { litbuf[litpos]='\0';
        tree_t *lit=leaf_sval(TT_QLIT,litbuf);
        result=result?expr_binary(TT_CAT,result,lit):lit; }
    return result ? result : leaf_sval(TT_QLIT,"");
}
tree_t *raku_prog_result = NULL;
static void add_proc(tree_t *e) {
    if (!e) return;
    if (!raku_prog_result) raku_prog_result = ast_stmt_new(TT_PROGRAM);
    tree_t *st = ast_stmt_new(TT_STMT);
    expr_add_child(st, ast_attr_int(":line", 0));
    expr_add_child(st, ast_attr_int(":stno", 0));
    expr_add_child(st, ast_attr_expr(":subj", e));
    expr_add_child(raku_prog_result, st);
}
#define RAKU_METH_MAX 256
typedef struct { char key[128]; char procname[128]; } RakuMethEntry;
static RakuMethEntry raku_meth_table[RAKU_METH_MAX];
static int           raku_meth_ntypes = 0;
static void raku_meth_register(const char *classname, const char *methname, const char *procname) {
    if (raku_meth_ntypes >= RAKU_METH_MAX) return;
    RakuMethEntry *e = &raku_meth_table[raku_meth_ntypes++];
    snprintf(e->key,      sizeof e->key,      "%s::%s", classname, methname);
    snprintf(e->procname, sizeof e->procname,  "%s",     procname);
}
const char *raku_meth_lookup(const char *classname, const char *methname) {
    char key[128];
    snprintf(key, sizeof key, "%s::%s", classname, methname);
    for (int i = 0; i < raku_meth_ntypes; i++)
        if (strcmp(raku_meth_table[i].key, key) == 0)
            return raku_meth_table[i].procname;
    return NULL;
}
%}
%union {
    long      ival;
    double    dval;
    char     *sval;
    tree_t  *node;
    ExprList *list;
}
%token <ival> LIT_INT
%token <dval> LIT_FLOAT
%token <sval> LIT_STR LIT_INTERP_STR LIT_REGEX LIT_MATCH_GLOBAL LIT_SUBST
%token <sval> VAR_SCALAR VAR_ARRAY VAR_HASH VAR_TWIGIL IDENT
%token <sval> VAR_ARRAY_TWIGIL VAR_HASH_TWIGIL
%token CARET
%token <ival> VAR_CAPTURE
%token <ival> VAR_FH
%token <sval> VAR_NAMED_CAPTURE
%token KW_USE
%token <sval> TESTOP
%token KW_MY KW_SAY KW_PRINT KW_IF KW_ELSE KW_ELSIF KW_WHILE KW_FOR
%token KW_SUB KW_GATHER KW_TAKE KW_RETURN KW_EXIT
%token KW_CONSTANT
%token KW_ENUM
%token KW_JOIN
%token KW_GIVEN KW_WHEN KW_DEFAULT KW_WITH KW_WITHOUT
%token KW_EXISTS KW_DELETE KW_UNLESS KW_UNTIL KW_REPEAT
%token KW_LOOP KW_LAST KW_NEXT
%token KW_MAP KW_GREP KW_SORT
%token KW_TRY KW_CATCH KW_DIE
%token KW_CLASS KW_METHOD KW_HAS KW_NEW
%token KW_ROLE
%token KW_MULTI KW_PROTO
%token <sval> OP_NAME
%token <sval> OP_REDUCE
%token <sval> ARR_ALL_SLICE
%token <sval> SLURPY_POS
%token <sval> SLURPY_LOL
%token <sval> SLURPY_NAMED
%token <sval> KW_HANDLES
%token <sval> WORDLIST
%token OP_COLON_D OP_COLON_U
%token YADA
%token KW_GRAMMAR KW_TOKEN KW_RULE KW_REGEX
%token OP_FATARROW
%token OP_RANGE OP_RANGE_EX
%token OP_ARROW
%token OP_EQ OP_NE OP_LE OP_GE
%token OP_SEQ OP_SNE OP_SLT OP_SLE OP_SGT OP_SGE
%token OP_AND OP_OR OP_TERNARY1 OP_TERNARY2
%token OP_BIND
%token OP_DOTEQ
%token OP_SMATCH
%token OP_INC OP_DEC
%token OP_ADD_EQ OP_SUB_EQ OP_MUL_EQ OP_DIV_EQ OP_CAT_EQ
%token OP_DOR
%token OP_DIV
%token OP_BAND OP_SHL
%token OP_DIVIS
%token OP_REP_X OP_REP_XX
%token OP_POW
%type <node> stmt expr atom range_expr cmp_expr tern_expr jct_expr dor_expr add_expr closure
%type <node> mul_expr unary_expr pow_expr postfix_expr call_expr block
%type <node> repl_expr addsub_expr divis_expr
%type <node> if_stmt while_stmt for_stmt sub_decl given_stmt sub_body method_body elsif_tail scalar_methcall
%type <node> unless_stmt until_stmt repeat_stmt loop_stmt loop_incr class_decl grammar_decl role_decl
%type <node> pair_list
%type <list> scalar_list
%type <sval> is_clauses meth_name
%type <list> stmt_list arg_list param_list when_list named_arg_list class_body_list grammar_body_list
%right '=' OP_BIND
%right OP_TERNARY1 OP_TERNARY2
%left  OP_OR
%left  OP_AND
%left  '!'
%left  OP_EQ OP_NE '<' '>' OP_LE OP_GE OP_SEQ OP_SNE OP_SLT OP_SLE OP_SGT OP_SGE OP_SMATCH
%left  OP_DIVIS
%left  '|' '&'
%left  OP_RANGE OP_RANGE_EX
%left  '~'
%left  OP_REP_X OP_REP_XX
%left  '+' '-'
%left  '*' '/' '%' OP_DIV OP_BAND OP_SHL
%right UMINUS
%right OP_POW
%left  '.'
%%
program
    : stmt_list
        {
            ExprList *all = $1;
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
    ;
stmt_list
    :    { $$ = exprlist_new(); }
    | stmt_list stmt { $$ = exprlist_append($1, $2); }
    ;
stmt
    : KW_MY VAR_SCALAR '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), rk_scalar_rhs($4)); }
    | KW_MY VAR_SCALAR ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), ast_node_new(TT_NUL)); }
    | KW_MY '(' scalar_list ')' '=' expr ';'
        { $$ = rk_destructure($3, $6); }
    | KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$6);
          ExprList *args=$8; if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          $$ = rk_destructure($3, call); }
    | '(' scalar_list ')' '=' expr ';'
        { $$ = rk_destructure($2, $5); }
    | KW_MY VAR_ARRAY ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), make_call("__rk_undef")); }
    | KW_MY VAR_HASH ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), make_call("__rk_undef")); }
    | KW_MY VAR_ARRAY '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), rk_arr_rhs($4)); }
    | KW_MY VAR_ARRAY '=' expr ',' arg_list ';'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$4);
          ExprList *args=$6; if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          $$ = expr_binary(TT_ASSIGN, var_node($2), call); }
    | KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$5);
          ExprList *args=$7; if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          $$ = expr_binary(TT_ASSIGN, var_node($2), call); }
    | VAR_ARRAY '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($1), rk_arr_rhs($3)); }
    | VAR_ARRAY '=' expr ',' arg_list ';'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$3);
          ExprList *args=$5; if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          $$ = expr_binary(TT_ASSIGN, var_node($1), call); }
    | VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$4);
          ExprList *args=$6; if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          $$ = expr_binary(TT_ASSIGN, var_node($1), call); }
    | KW_MY VAR_HASH '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), $4); }
    | KW_MY VAR_HASH '=' pair_list ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), $4); }
    | KW_MY VAR_HASH '=' '(' pair_list ')' ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), $5); }
    | KW_MY IDENT VAR_SCALAR '=' expr ';'
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); ast_push(e,$5); $$=e; }
    | KW_MY IDENT VAR_ARRAY '=' expr ';'
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); ast_push(e,rk_arr_rhs($5)); $$=e; }
    | KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$5);
          ExprList *args=$7; if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); ast_push(e,call); $$=e; }
    | KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$6);
          ExprList *args=$8; if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); ast_push(e,call); $$=e; }
    | KW_MY IDENT VAR_HASH '=' expr ';'
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); ast_push(e,$5); $$=e; }
    | KW_MY IDENT VAR_SCALAR ';'
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); $$=e; }
    | KW_MY IDENT VAR_ARRAY ';'
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); $$=e; }
    | KW_MY IDENT VAR_HASH ';'
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,$2)); free($2); ast_push(e,var_node($3)); $$=e; }
    | KW_USE IDENT ';'
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern($2); free($2); $$=u; }
    | KW_CONSTANT IDENT '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), $4); free($2); }
    | KW_CONSTANT VAR_SCALAR '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($2), $4); free($2); }
    | KW_MY KW_CONSTANT IDENT '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($3), $5); free($3); }
    | KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'
        { $$ = expr_binary(TT_ASSIGN, var_node($3), $5); free($3); }
    | KW_ENUM IDENT WORDLIST ';'
        { ExprList *l=exprlist_new(); char *s=$3; int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free($2); free($3); $$ = make_seq(l); }
    | TESTOP ';'
        { $$=make_call(testop_rt($1)); free($1); }
    | TESTOP '(' arg_list ')' ';'
        { tree_t *c=make_call(testop_rt($1)); free($1); ExprList *a=$3; if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } $$=c; }
    | TESTOP '(' ')' ';'
        { $$=make_call(testop_rt($1)); free($1); }
    | TESTOP arg_list ';'
        { tree_t *c=make_call(testop_rt($1)); free($1); ExprList *a=$2; if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } $$=c; }
    | IDENT VAR_ARRAY ';'
        { tree_t *c=make_call($1); free($1); expr_add_child(c,var_node($2)); $$=c; }
    | KW_SAY expr ';'
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,$2); $$=c; }
    | KW_SAY expr ',' arg_list ';'
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,$2);
          ExprList *a=$4; if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } $$=c; }
    | KW_SAY '(' expr ',' arg_list ')' ';'
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,$3);
          ExprList *a=$5; if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } $$=c; }
    | KW_PRINT expr ';'
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,$2); $$=c; }
    | KW_PRINT expr ',' arg_list ';'
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,$2);
          ExprList *a=$4; if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } $$=c; }
    | KW_PRINT '(' expr ',' arg_list ')' ';'
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,$3);
          ExprList *a=$5; if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } $$=c; }
    | KW_TAKE expr ';'
        { $$=expr_unary(TT_SUSPEND,$2); }
    | KW_RETURN expr ';'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$2); $$=r; }
    | KW_RETURN ';'
        { $$=ast_node_new(TT_RETURN); }
    | KW_RETURN expr KW_IF expr ';'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$2);
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$4); expr_add_child(e,seq1(r)); $$=e; }
    | KW_RETURN expr KW_UNLESS expr ';'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$2);
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$4); ast_push(e,seq1(r)); $$=e; }
    | KW_EXIT expr ';'
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,$2); $$=c; }
    | KW_EXIT ';'
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); $$=c; }
    | KW_EXIT expr KW_IF expr ';'
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,$2);
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$4); expr_add_child(e,seq1(c)); $$=e; }
    | KW_EXIT expr KW_UNLESS expr ';'
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,$2);
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$4); ast_push(e,seq1(c)); $$=e; }
    | VAR_SCALAR '=' expr ';'
        { $$=expr_binary(TT_ASSIGN,var_node($1),rk_scalar_rhs($3)); }
    | VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node($1)); ast_push(mc,leaf_sval(TT_QLIT,$3)); free($3);
          ExprList *args=$5; if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          $$=expr_binary(TT_ASSIGN,var_node($1),mc); }
    | VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node($1)); ast_push(mc,leaf_sval(TT_QLIT,$3)); free($3);
          $$=expr_binary(TT_ASSIGN,var_node($1),mc); }
    | VAR_SCALAR OP_DOTEQ IDENT ';'
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node($1)); ast_push(mc,leaf_sval(TT_QLIT,$3)); free($3);
          $$=expr_binary(TT_ASSIGN,var_node($1),mc); }
    | VAR_SCALAR '.' IDENT '=' expr ';'
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern($3); free($3);
          expr_add_child(fe,var_node($1));
          $$=expr_binary(TT_ASSIGN,fe,$5); }
    | VAR_TWIGIL '=' expr ';'
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare($1)); free($1);
          $$=expr_binary(TT_ASSIGN,fe,$3); }
    | scalar_methcall ';'                        { $$=$1; }
    | scalar_methcall KW_IF expr ';'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,seq1($1)); $$=e; }
    | scalar_methcall KW_UNLESS expr ';'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$3); ast_push(e,seq1($1)); $$=e; }
    | scalar_methcall KW_WHILE expr ';'
        { $$=expr_binary(TT_WHILE,$3,seq1($1)); }
    | scalar_methcall KW_UNTIL expr ';'
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,$3); expr_add_child(e,seq1($1)); $$=e; }
    | scalar_methcall KW_FOR expr ';'
        { tree_t *gen=expr_unary(TT_ITERATE,$3); gen->v.sval=(char*)intern("_");
          $$=expr_binary(TT_EVERY, gen, seq1($1)); }
    | scalar_methcall KW_WITH expr ';'
        { $$=rk_with_mod($1,$3,0); }
    | scalar_methcall KW_WITHOUT expr ';'
        { $$=rk_with_mod($1,$3,1); }
    | scalar_methcall KW_GIVEN expr ';'
        { $$=rk_given_mod($1,$3); }
    | VAR_ARRAY '[' expr ']' '=' expr ';'
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node($1)); ast_push(c,$3); ast_push(c,$6); $$=c; }
    | VAR_HASH '<' IDENT '>' '=' expr ';'
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node($1)); ast_push(c,leaf_sval(TT_QLIT,$3)); ast_push(c,$6); $$=c; }
    | VAR_HASH '{' expr '}' '=' expr ';'
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node($1)); ast_push(c,$3); ast_push(c,$6); $$=c; }
    | KW_DELETE VAR_HASH '<' IDENT '>' ';'
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node($2)); ast_push(c,leaf_sval(TT_QLIT,$4)); $$=c; }
    | KW_DELETE VAR_HASH '{' expr '}' ';'
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node($2)); ast_push(c,$4); $$=c; }
    | expr KW_IF expr ';'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,seq1($1)); $$=e; }
    | expr KW_UNLESS expr ';'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$3); ast_push(e,seq1($1)); $$=e; }
    | expr KW_WHILE expr ';'
        { $$=expr_binary(TT_WHILE,$3,seq1($1)); }
    | expr KW_UNTIL expr ';'
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,$3); expr_add_child(e,seq1($1)); $$=e; }
    | expr KW_FOR expr ';'
        { tree_t *gen=expr_unary(TT_ITERATE,$3); gen->v.sval=(char*)intern("_");
          $$=expr_binary(TT_EVERY, gen, seq1($1)); }
    | expr KW_WITH expr ';'
        { $$=rk_with_mod($1,$3,0); }
    | expr KW_WITHOUT expr ';'
        { $$=rk_with_mod($1,$3,1); }
    | expr KW_GIVEN expr ';'
        { $$=rk_given_mod($1,$3); }
    | KW_SAY expr KW_IF expr ';'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$2);
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$4); expr_add_child(e,seq1(s)); $$=e; }
    | KW_SAY expr KW_UNLESS expr ';'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$2);
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$4); ast_push(e,seq1(s)); $$=e; }
    | KW_SAY expr KW_FOR expr ';'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$2);
          tree_t *gen=expr_unary(TT_ITERATE,$4); gen->v.sval=(char*)intern("_");
          $$=expr_binary(TT_EVERY, gen, seq1(s)); }
    | KW_SAY expr KW_WHILE expr ';'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$2);
          $$=expr_binary(TT_WHILE,$4,seq1(s)); }
    | KW_SAY expr KW_WITH expr ';'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$2); $$=rk_with_mod(s,$4,0); }
    | KW_SAY expr KW_WITHOUT expr ';'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$2); $$=rk_with_mod(s,$4,1); }
    | KW_SAY expr KW_GIVEN expr ';'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$2); $$=rk_given_mod(s,$4); }
    | KW_PRINT expr KW_IF expr ';'
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,$2);
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$4); expr_add_child(e,seq1(p)); $$=e; }
    | KW_PRINT expr KW_UNLESS expr ';'
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,$2);
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$4); ast_push(e,seq1(p)); $$=e; }
    | VAR_SCALAR OP_ADD_EQ expr ';'
        { tree_t *v=var_node($1);
          $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_ADD,v,$3)); }
    | VAR_SCALAR OP_SUB_EQ expr ';'
        { tree_t *v=var_node($1);
          $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_SUB,v,$3)); }
    | VAR_SCALAR OP_MUL_EQ expr ';'
        { tree_t *v=var_node($1);
          $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_MUL,v,$3)); }
    | VAR_SCALAR OP_DIV_EQ expr ';'
        { tree_t *v=var_node($1);
          $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_DIV,v,$3)); }
    | VAR_SCALAR OP_CAT_EQ expr ';'
        { tree_t *v=var_node($1);
          $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_CAT,v,$3)); }
    | expr ';' { $$=$1; }
    | ';' { $$=make_seq(exprlist_new()); }
    | if_stmt           { $$=$1; }
    | while_stmt        { $$=$1; }
    | for_stmt          { $$=$1; }
    | given_stmt        { $$=$1; }
    | KW_TRY block
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,$2); $$=e; }
    | KW_TRY block KW_CATCH block
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,$2); ast_push(e,$4); $$=e; }
    | KW_CATCH block
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,$2); $$=e; }
    | block
        { $$=$1; }
    | unless_stmt       { $$=$1; }
    | until_stmt        { $$=$1; }
    | repeat_stmt       { $$=$1; }
    | loop_stmt         { $$=$1; }
    | KW_LAST ';'       { $$=ast_node_new(TT_LOOP_BREAK); }
    | KW_NEXT ';'       { $$=ast_node_new(TT_LOOP_NEXT); }
    | KW_LAST KW_IF expr ';'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); $$=e; }
    | KW_LAST KW_UNLESS expr ';'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$3); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); $$=e; }
    | KW_NEXT KW_IF expr ';'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); $$=e; }
    | KW_NEXT KW_UNLESS expr ';'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$3); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); $$=e; }
    | sub_decl          { $$=$1; }
    | class_decl        { $$=$1; }
    | role_decl         { $$=$1; }
    | grammar_decl      { $$=$1; }
    ;
if_stmt
    : KW_IF '(' expr ')' block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,$5); $$=e; }
    | KW_IF '(' expr ')' block KW_ELSE block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,$5); expr_add_child(e,$7); $$=e; }
    | KW_IF '(' expr ')' block KW_ELSE if_stmt
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,$5); expr_add_child(e,$7); $$=e; }
    | KW_IF expr block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$2); expr_add_child(e,$3); $$=e; }
    | KW_IF expr block KW_ELSE block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$2); expr_add_child(e,$3); expr_add_child(e,$5); $$=e; }
    | KW_IF expr block KW_ELSE if_stmt
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$2); expr_add_child(e,$3); expr_add_child(e,$5); $$=e; }
    | KW_IF expr block elsif_tail
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$2); expr_add_child(e,$3); expr_add_child(e,$4); $$=e; }
    | KW_IF '(' expr ')' block elsif_tail
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,$5); expr_add_child(e,$6); $$=e; }
    ;
elsif_tail
    : KW_ELSIF expr block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$2); expr_add_child(e,$3); $$=e; }
    | KW_ELSIF '(' expr ')' block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,$5); $$=e; }
    | KW_ELSIF expr block KW_ELSE block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$2); expr_add_child(e,$3); expr_add_child(e,$5); $$=e; }
    | KW_ELSIF '(' expr ')' block KW_ELSE block
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,$5); expr_add_child(e,$7); $$=e; }
    | KW_ELSIF expr block elsif_tail
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$2); expr_add_child(e,$3); expr_add_child(e,$4); $$=e; }
    | KW_ELSIF '(' expr ')' block elsif_tail
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$3); expr_add_child(e,$5); expr_add_child(e,$6); $$=e; }
    ;
while_stmt
    : KW_WHILE '(' expr ')' block
        { $$=expr_binary(TT_WHILE,$3,$5); }
    | KW_WHILE expr block
        { $$=expr_binary(TT_WHILE,$2,$3); }
    ;
unless_stmt
    : KW_UNLESS '(' expr ')' block
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$3); ast_push(e,$5); $$=e; }
    | KW_UNLESS '(' expr ')' block KW_ELSE block
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$3); ast_push(e,$5); ast_push(e,$7); $$=e; }
    | KW_UNLESS expr block
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$2); ast_push(e,$3); $$=e; }
    | KW_UNLESS expr block KW_ELSE block
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$2); ast_push(e,$3); ast_push(e,$5); $$=e; }
    ;
until_stmt
    : KW_UNTIL '(' expr ')' block
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,$3); expr_add_child(e,$5); $$=e; }
    | KW_UNTIL expr block
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,$2); expr_add_child(e,$3); $$=e; }
    ;
repeat_stmt
    : KW_REPEAT block
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,$2); e->v.ival=0; $$=e; }
    | KW_REPEAT block KW_WHILE expr ';'
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,$2); expr_add_child(e,$4); e->v.ival=1; $$=e; }
    | KW_REPEAT block KW_UNTIL expr ';'
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,$2); expr_add_child(e,$4); e->v.ival=2; $$=e; }
    ;
loop_stmt
    : KW_LOOP block
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; $$=expr_binary(TT_WHILE,one,$2); }
    | KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block
        { $$=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node($4),$6),$8,$10,$12); }
    | KW_LOOP '(' expr ';' expr ';' loop_incr ')' block
        { $$=rk_cstyle_loop($3,$5,$7,$9); }
    ;
loop_incr
    : expr                { $$=$1; }
    ;
for_stmt
    : KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block
        { const char *vn = intern(strip_sigil($6)); free($6);
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, $2); ast_push(r, $4); ast_push(r, $7);
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          $$ = r; }
    | KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block
        { const char *vn = intern(strip_sigil($6)); free($6);
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, $2); ast_push(r, rk_dec($4)); ast_push(r, $7);
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          $$ = r; }
    | KW_FOR expr OP_ARROW VAR_SCALAR block
        { const char *vn = intern(strip_sigil($4)); free($4);
          tree_t *gen = expr_unary(TT_ITERATE, $2);
          gen->v.sval = (char *)vn;
          $$ = expr_binary(TT_EVERY, gen, $5); }
    | KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block
        { const char *vn = intern(strip_sigil($6)); free($6);
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,$2);
          ExprList *a=$4; if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          $$ = expr_binary(TT_EVERY, gen, $7); }
    | KW_FOR expr ',' arg_list block
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,$2);
          ExprList *a=$4; if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          $$ = expr_binary(TT_EVERY, gen, $5); }
    | KW_FOR expr block
        { tree_t *gen = expr_unary(TT_ITERATE, $2);
          $$ = expr_binary(TT_EVERY, gen, $3); }
    ;
given_stmt
    : KW_GIVEN expr '{' when_list '}'
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,$2);
          ExprList *whens=$4;
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          $$=ec; }
    | KW_GIVEN expr '{' when_list KW_DEFAULT block '}'
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,$2);
          ExprList *whens=$4;
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,$6);
          $$=ec; }
    ;
when_list
    :  { $$=exprlist_new(); }
    | when_list KW_WHEN expr block
        {
          exprlist_append($1,$3); exprlist_append($1,$4);
          $$=$1; }
    ;
sub_decl
    : KW_SUB IDENT '(' param_list ')' sub_body
        { ExprList *params=$4; tree_t *rkbody=rk_defaults_prologue(params,$6); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,$2); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern($2); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          $$=e; }
    | KW_SUB IDENT '(' ')' sub_body
        { tree_t *e=leaf_sval(TT_SUB_DECL,$2); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern($2); expr_add_child(e,nn);
          tree_t *body=$5;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          $$=e; }
    | KW_SUB IDENT sub_body
        { tree_t *e=leaf_sval(TT_SUB_DECL,$2); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern($2); expr_add_child(e,nn);
          tree_t *body=$3;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          $$=e; }
    | KW_MY KW_SUB IDENT '(' param_list ')' sub_body
        { ExprList *params=$5; tree_t *rkbody=rk_defaults_prologue(params,$7); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,$3); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern($3); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          $$=e; }
    | KW_MY KW_SUB IDENT '(' ')' sub_body
        { tree_t *e=leaf_sval(TT_SUB_DECL,$3); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern($3); expr_add_child(e,nn);
          tree_t *body=$6;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          $$=e; }
    | KW_MY KW_SUB IDENT sub_body
        { tree_t *e=leaf_sval(TT_SUB_DECL,$3); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern($3); expr_add_child(e,nn);
          tree_t *body=$4;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          $$=e; }
    | KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body
        { ExprList *params=$5; tree_t *rkbody=rk_defaults_prologue(params,$7); int np=params?params->count:0;
          const char *mname=rk_multi_mangle($3,params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free($3); $$=e; }
    | KW_MULTI KW_SUB IDENT '(' ')' sub_body
        { const char *mname=rk_multi_mangle($3,NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=$6;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free($3); $$=e; }
    | KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body
        { ExprList *params=$5; tree_t *rkbody=rk_defaults_prologue(params,$7); int np=params?params->count:0;
          const char *mname=rk_multi_mangle($3,params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free($3); $$=e; }
    | KW_MULTI IDENT '(' param_list ')' sub_body
        { ExprList *params=$4; tree_t *rkbody=rk_defaults_prologue(params,$6); int np=params?params->count:0;
          const char *mname=rk_multi_mangle($2,params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free($2); $$=e; }
    | KW_MULTI IDENT '(' ')' sub_body
        { const char *mname=rk_multi_mangle($2,NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=$5;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free($2); $$=e; }
    ;
scalar_methcall
    : VAR_SCALAR '.' IDENT '(' arg_list ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node($1));
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          ExprList *args = $5;
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          $$ = c; }
    | VAR_SCALAR '.' IDENT '(' ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node($1));
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          $$ = c; }
    ;
sub_body
    : '{' stmt_list '}'          { $$=make_seq($2); }
    | '{' stmt_list expr '}'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$3);
          ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_RETURN expr '}'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$4);
          ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_RETURN '}'
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4); ExprList *l=$2; exprlist_append(l,s); $$=make_seq(l); }
    | '{' stmt_list KW_PRINT expr '}'
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,$4); ExprList *l=$2; exprlist_append(l,p); $$=make_seq(l); }
    | '{' stmt_list expr KW_IF expr '}'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$5); expr_add_child(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_UNLESS expr '}'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$5); ast_push(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WHILE expr '}'
        { tree_t *e=expr_binary(TT_WHILE,$5,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_UNTIL expr '}'
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,$5); expr_add_child(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_FOR expr '}'
        { tree_t *gen=expr_unary(TT_ITERATE,$5); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WITH expr '}'
        { tree_t *e=rk_with_mod($3,$5,0); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WITHOUT expr '}'
        { tree_t *e=rk_with_mod($3,$5,1); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_GIVEN expr '}'
        { tree_t *e=rk_given_mod($3,$5); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_IF expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$6); expr_add_child(e,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_UNLESS expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$6); ast_push(e,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_FOR expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *gen=expr_unary(TT_ITERATE,$6); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    ;
method_body
    : '{' stmt_list '}'          { $$=make_seq($2); }
    | '{' YADA '}'               { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); $$=make_seq(l); }
    | '{' stmt_list expr '}'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$3);
          ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_RETURN expr '}'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$4);
          ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_RETURN '}'
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4); ExprList *l=$2; exprlist_append(l,s); $$=make_seq(l); }
    | '{' stmt_list KW_PRINT expr '}'
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,$4); ExprList *l=$2; exprlist_append(l,p); $$=make_seq(l); }
    | '{' stmt_list expr KW_IF expr '}'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$5); expr_add_child(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_UNLESS expr '}'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$5); ast_push(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WHILE expr '}'
        { tree_t *e=expr_binary(TT_WHILE,$5,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_UNTIL expr '}'
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,$5); expr_add_child(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_FOR expr '}'
        { tree_t *gen=expr_unary(TT_ITERATE,$5); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WITH expr '}'
        { tree_t *e=rk_with_mod($3,$5,0); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WITHOUT expr '}'
        { tree_t *e=rk_with_mod($3,$5,1); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_GIVEN expr '}'
        { tree_t *e=rk_given_mod($3,$5); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_IF expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$6); expr_add_child(e,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_UNLESS expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$6); ast_push(e,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_FOR expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *gen=expr_unary(TT_ITERATE,$6); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    ;
class_decl
    : KW_CLASS IDENT is_clauses '{' class_body_list '}'
        {
            const char *cname = intern($2); free($2);
            ExprList *body = $5;
            tree_t *cd = ast_node_new(TT_CLASS_DECL);
            if ($3) cd->v.sval = $3;
            ast_push(cd, leaf_sval(TT_VAR, cname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(cd, body->items[i]);
                exprlist_free(body);
            }
            $$ = cd;
        }
    ;
role_decl
    : KW_ROLE IDENT '{' class_body_list '}'
        {
            const char *rname = intern($2); free($2);
            ExprList *body = $4;
            tree_t *rd = ast_node_new(TT_ROLE_DECL);
            ast_push(rd, leaf_sval(TT_VAR, rname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(rd, body->items[i]);
                exprlist_free(body);
            }
            $$ = rd;
        }
    ;
is_clauses
    :  { $$ = (char *)0; }
    | is_clauses IDENT IDENT
        {
            char tag = 0;
            if ($2 && !strcmp($2, "is")) tag = 'i';
            else if ($2 && !strcmp($2, "does")) tag = 'd';
            if (tag && $3) {
                size_t l2 = strlen($3);
                if (!$1) { char *m = (char *)malloc(l2 + 2); m[0] = tag; memcpy(m + 1, $3, l2 + 1); $$ = m; }
                else { size_t l1 = strlen($1); char *m = (char *)malloc(l1 + l2 + 3); memcpy(m, $1, l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, $3, l2 + 1); free($1); $$ = m; }
            } else { $$ = $1; }
            free($2); free($3);
        }
    | is_clauses TESTOP IDENT
        {
            char tag = 0;
            if ($2 && !strcmp($2, "is")) tag = 'i';
            else if ($2 && !strcmp($2, "does")) tag = 'd';
            if (tag && $3) {
                size_t l2 = strlen($3);
                if (!$1) { char *m = (char *)malloc(l2 + 2); m[0] = tag; memcpy(m + 1, $3, l2 + 1); $$ = m; }
                else { size_t l1 = strlen($1); char *m = (char *)malloc(l1 + l2 + 3); memcpy(m, $1, l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, $3, l2 + 1); free($1); $$ = m; }
            } else { $$ = $1; }
            free($2); free($3);
        }
    ;
class_body_list
    :  { $$ = exprlist_new(); }
    | class_body_list KW_HAS VAR_TWIGIL ';'
        { tree_t *fv = leaf_sval(TT_VAR, $3); free($3);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern($3); free($3);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_HASH_TWIGIL ';'
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern($3); free($3);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'
        { free($3); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern($4); free($4);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'
        { free($3); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern($4); free($4);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_SCALAR ';'
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil($3)); free($3);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_TWIGIL ';'
        { free($3); tree_t *fv = leaf_sval(TT_VAR, $4); free($4);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_SCALAR ';'
        { free($3); tree_t *fv = leaf_sval(TT_VAR, strip_sigil($4)); free($4);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_TWIGIL '=' expr ';'
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern($3); free($3); expr_add_child(fv, $5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_SCALAR '=' expr ';'
        { const char *fn = strip_sigil($3); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free($3); expr_add_child(fv, $5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'
        { free($3); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern($4); free($4); expr_add_child(fv, $6);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'
        { free($3); const char *fn = strip_sigil($4); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free($4); expr_add_child(fv, $6);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'
        { tree_t *fv;
          if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern($3); }
          else if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern($3); }
          else fv = leaf_sval(TT_VAR, $3);
          free($3); free($4); free($5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'
        { tree_t *fv;
          if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern($3); }
          else if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern($3); }
          else fv = leaf_sval(TT_VAR, $3);
          free($3); free($4); free($5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'
        { tree_t *fv; const char *fn = strip_sigil($3);
          if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free($3); free($4); free($5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'
        { tree_t *fv; const char *fn = strip_sigil($3);
          if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ($4 && !strcmp($4, "is") && $5 && !strcmp($5, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free($3); free($4); free($5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'
        { const char *fn = strip_sigil($3); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, $4)); free($3); free($4);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern($3);
          expr_add_child(fv, leaf_sval(TT_QLIT, $4)); free($3); free($4);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'
        { const char *fn = strip_sigil($4); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, $5)); free($3); free($4); free($5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern($4);
          expr_add_child(fv, leaf_sval(TT_QLIT, $5)); free($3); free($4); free($5);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'
        { tree_t *fv;
          if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern($4); }
          else if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern($4); }
          else fv = leaf_sval(TT_VAR, $4);
          free($3); free($4); free($5); free($6);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'
        { tree_t *fv;
          if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern($4); }
          else if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern($4); }
          else fv = leaf_sval(TT_VAR, $4);
          free($3); free($4); free($5); free($6);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'
        { tree_t *fv; const char *fn = strip_sigil($4);
          if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free($3); free($4); free($5); free($6);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'
        { tree_t *fv; const char *fn = strip_sigil($4);
          if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ($5 && !strcmp($5, "is") && $6 && !strcmp($6, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free($3); free($4); free($5); free($6);
          $$ = exprlist_append($1, fv); }
    | class_body_list KW_METHOD meth_name '(' param_list ')' method_body
        { ExprList *params = $5; tree_t *rkbody=rk_defaults_prologue(params,$7); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern($3); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free($3);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_METHOD meth_name '(' ')' method_body
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern($3); expr_add_child(e, nn);
          tree_t *body = $6;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free($3);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_METHOD meth_name method_body
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern($3); expr_add_child(e, nn);
          tree_t *body = $4;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free($3);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body
        { ExprList *params = $5; tree_t *rkbody=rk_defaults_prologue(params,$7); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_METHOD KW_NEW '(' ')' method_body
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = $6;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_METHOD KW_NEW method_body
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = $4;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body
        { ExprList *params = $6; tree_t *rkbody=rk_defaults_prologue(params,$8); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle($4, params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free($4);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body
        { const char *mname = rk_multi_mangle($4, NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = $7;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free($4);
          $$ = exprlist_append($1, e); }
    | class_body_list KW_MULTI KW_METHOD meth_name method_body
        { const char *mname = rk_multi_mangle($4, NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = $5;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free($4);
          $$ = exprlist_append($1, e); }
    ;
grammar_decl
    : KW_GRAMMAR IDENT '{' grammar_body_list '}'
        {
            const char *gname = intern($2); free($2);
            ExprList *body = $4;
            tree_t *gd = ast_node_new(TT_GRAMMAR_DECL);
            ast_push(gd, leaf_sval(TT_VAR, gname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(gd, body->items[i]);
                exprlist_free(body);
            }
            $$ = gd;
        }
    ;
grammar_body_list
    :  { $$ = exprlist_new(); }
    | grammar_body_list KW_TOKEN IDENT LIT_REGEX
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern($3))); free($3);
          ast_push(rd, leaf_sval(TT_QLIT, $4));
          $$ = exprlist_append($1, rd); }
    | grammar_body_list KW_RULE IDENT LIT_REGEX
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern($3))); free($3);
          ast_push(rd, leaf_sval(TT_QLIT, $4));
          $$ = exprlist_append($1, rd); }
    | grammar_body_list KW_REGEX IDENT LIT_REGEX
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern($3))); free($3);
          ast_push(rd, leaf_sval(TT_QLIT, $4));
          $$ = exprlist_append($1, rd); }
    ;
named_arg_list
    : IDENT OP_FATARROW expr
        { $$ = exprlist_new();
          exprlist_append($$, leaf_sval(TT_QLIT, $1)); free($1);
          exprlist_append($$, $3); }
    | ':' IDENT '(' expr ')'
        { $$ = exprlist_new();
          exprlist_append($$, leaf_sval(TT_QLIT, $2)); free($2);
          exprlist_append($$, $4); }
    | named_arg_list ',' IDENT OP_FATARROW expr
        { exprlist_append($1, leaf_sval(TT_QLIT, $3)); free($3);
          exprlist_append($1, $5);
          $$ = $1; }
    | named_arg_list ',' ':' IDENT '(' expr ')'
        { exprlist_append($1, leaf_sval(TT_QLIT, $4)); free($4);
          exprlist_append($1, $6);
          $$ = $1; }
    ;
pair_list
    : IDENT OP_FATARROW expr
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,$1)); free($1); expr_add_child(c,$3); $$=c; }
    | LIT_STR OP_FATARROW expr
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,$1)); expr_add_child(c,$3); $$=c; }
    | pair_list ',' IDENT OP_FATARROW expr
        { expr_add_child($1,leaf_sval(TT_QLIT,$3)); free($3); expr_add_child($1,$5); $$=$1; }
    | pair_list ',' LIT_STR OP_FATARROW expr
        { expr_add_child($1,leaf_sval(TT_QLIT,$3)); expr_add_child($1,$5); $$=$1; }
    ;
param_list
    : VAR_SCALAR             { $$=exprlist_append(exprlist_new(),var_node($1)); }
    | VAR_SCALAR TESTOP IDENT { free($2); free($3); $$=exprlist_append(exprlist_new(),var_node($1)); }
    | param_list ',' VAR_SCALAR TESTOP IDENT { free($4); free($5); $$=exprlist_append($1,var_node($3)); }
    | VAR_ARRAY               { $$=exprlist_append(exprlist_new(),var_node($1)); }
    | param_list ',' VAR_ARRAY { $$=exprlist_append($1,var_node($3)); }
    | IDENT VAR_SCALAR       { $$=exprlist_append(exprlist_new(),rk_typed_param($1,$2)); free($1); }
    | IDENT OP_COLON_D VAR_SCALAR { $$=exprlist_append(exprlist_new(),rk_typed_def_param($1,":D",$3)); free($1); }
    | IDENT OP_COLON_U VAR_SCALAR { $$=exprlist_append(exprlist_new(),rk_typed_def_param($1,":U",$3)); free($1); }
    | param_list ',' VAR_SCALAR { $$=exprlist_append($1,var_node($3)); }
    | param_list ',' IDENT VAR_SCALAR { $$=exprlist_append($1,rk_typed_param($3,$4)); free($3); }
    | param_list ',' IDENT OP_COLON_D VAR_SCALAR { $$=exprlist_append($1,rk_typed_def_param($3,":D",$5)); free($3); }
    | param_list ',' IDENT OP_COLON_U VAR_SCALAR { $$=exprlist_append($1,rk_typed_def_param($3,":U",$5)); free($3); }
    | VAR_SCALAR '=' expr    { $$=exprlist_append(exprlist_new(),rk_param_default(var_node($1),$3)); }
    | param_list ',' VAR_SCALAR '=' expr { $$=exprlist_append($1,rk_param_default(var_node($3),$5)); }
    | IDENT VAR_SCALAR '=' expr { $$=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param($1,$2),$4)); free($1); }
    | param_list ',' IDENT VAR_SCALAR '=' expr { $$=exprlist_append($1,rk_param_default(rk_typed_param($3,$4),$6)); free($3); }
    | SLURPY_POS             { $$=exprlist_append(exprlist_new(),rk_slurpy_param($1)); }
    | param_list ',' SLURPY_POS { $$=exprlist_append($1,rk_slurpy_param($3)); }
    | SLURPY_LOL             { $$=exprlist_append(exprlist_new(),rk_slurpy_lol_param($1)); }
    | param_list ',' SLURPY_LOL { $$=exprlist_append($1,rk_slurpy_lol_param($3)); }
    | SLURPY_NAMED             { $$=exprlist_append(exprlist_new(),rk_slurpy_named_param($1)); }
    | param_list ',' SLURPY_NAMED { $$=exprlist_append($1,rk_slurpy_named_param($3)); }
    ;
block
    : '{' stmt_list '}'  { $$=make_seq($2); }
    | '{' YADA '}'       { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); $$=make_seq(l); }
    | '{' stmt_list expr '}'
        { ExprList *l=$2; exprlist_append(l,$3); $$=make_seq(l); }
    | '{' stmt_list expr KW_IF expr '}'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$5); expr_add_child(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_UNLESS expr '}'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$5); ast_push(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WHILE expr '}'
        { tree_t *e=expr_binary(TT_WHILE,$5,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_UNTIL expr '}'
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,$5); expr_add_child(e,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_FOR expr '}'
        { tree_t *gen=expr_unary(TT_ITERATE,$5); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1($3)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WITH expr '}'
        { tree_t *e=rk_with_mod($3,$5,0); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_WITHOUT expr '}'
        { tree_t *e=rk_with_mod($3,$5,1); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list expr KW_GIVEN expr '}'
        { tree_t *e=rk_given_mod($3,$5); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4); ExprList *l=$2; exprlist_append(l,s); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_IF expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$6); expr_add_child(e,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_UNLESS expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$6); ast_push(e,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_FOR expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *gen=expr_unary(TT_ITERATE,$6); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_WITH expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=rk_with_mod(s,$6,0); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=rk_with_mod(s,$6,1); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_SAY expr KW_GIVEN expr '}'
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,$4);
          tree_t *e=rk_given_mod(s,$6); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_PRINT expr '}'
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,$4); ExprList *l=$2; exprlist_append(l,p); $$=make_seq(l); }
    | '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node($3)); ast_push(c,leaf_sval(TT_QLIT,$5)); free($5);
          ExprList *args=$7; if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=$2; exprlist_append(l,c); $$=make_seq(l); }
    | '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node($3)); ast_push(c,leaf_sval(TT_QLIT,$5)); free($5);
          ExprList *l=$2; exprlist_append(l,c); $$=make_seq(l); }
    | '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern($5); free($5); expr_add_child(fe,var_node($3));
          tree_t *a=expr_binary(TT_ASSIGN,fe,$7); ExprList *l=$2; exprlist_append(l,a); $$=make_seq(l); }
    | '{' stmt_list VAR_TWIGIL '=' expr '}'
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare($3)); free($3);
          tree_t *a=expr_binary(TT_ASSIGN,fe,$5); ExprList *l=$2; exprlist_append(l,a); $$=make_seq(l); }
    | '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node($3)); ast_push(c,$5); ast_push(c,$8);
          ExprList *l=$2; exprlist_append(l,c); $$=make_seq(l); }
    | '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node($3)); ast_push(c,$5); ast_push(c,$8);
          ExprList *l=$2; exprlist_append(l,c); $$=make_seq(l); }
    | '{' stmt_list KW_TAKE expr '}'
        { tree_t *t=expr_unary(TT_SUSPEND,$4); ExprList *l=$2; exprlist_append(l,t); $$=make_seq(l); }
    | '{' stmt_list KW_RETURN expr '}'
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,$4); ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_RETURN '}'
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=$2; exprlist_append(l,r); $$=make_seq(l); }
    | '{' stmt_list KW_LAST '}'
        { ExprList *l=$2; exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); $$=make_seq(l); }
    | '{' stmt_list KW_NEXT '}'
        { ExprList *l=$2; exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); $$=make_seq(l); }
    | '{' stmt_list KW_LAST KW_IF expr '}'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$5); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_LAST KW_UNLESS expr '}'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$5); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_NEXT KW_IF expr '}'
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,$5); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    | '{' stmt_list KW_NEXT KW_UNLESS expr '}'
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,$5); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=$2; exprlist_append(l,e); $$=make_seq(l); }
    ;
closure
    : '{' expr '}'  { $$=$2; }
    ;
expr
    : VAR_SCALAR '=' expr  { $$=expr_binary(TT_ASSIGN,var_node($1),$3); }
    | VAR_ARRAY '=' expr   { $$=expr_binary(TT_ASSIGN,var_node($1),rk_arr_rhs($3)); }
    | VAR_SCALAR OP_ADD_EQ expr { tree_t *v=var_node($1); $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_ADD,v,$3)); }
    | VAR_SCALAR OP_SUB_EQ expr { tree_t *v=var_node($1); $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_SUB,v,$3)); }
    | VAR_SCALAR OP_MUL_EQ expr { tree_t *v=var_node($1); $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_MUL,v,$3)); }
    | VAR_SCALAR OP_DIV_EQ expr { tree_t *v=var_node($1); $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_DIV,v,$3)); }
    | VAR_SCALAR OP_CAT_EQ expr { tree_t *v=var_node($1); $$=expr_binary(TT_ASSIGN,var_node($1),expr_binary(TT_CAT,v,$3)); }
    | KW_GATHER block      {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, $2);
          $$ = g;
      }
    | KW_GATHER for_stmt   {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, $2);
          $$ = g;
      }
    | tern_expr            { $$=$1; }
    ;
tern_expr
    : cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, $1); ast_push(c, $3); ast_push(c, $5); $$ = c; }
    | cmp_expr             { $$=$1; }
    ;
cmp_expr
    : cmp_expr OP_AND divis_expr  { $$=expr_binary(TT_SEQ,$1,$3); }
    | cmp_expr OP_OR  divis_expr  { $$=expr_binary(TT_ALT,$1,$3); }
    | cmp_expr OP_EQ  divis_expr  { $$=rk_chain_cmp($1,TT_EQ,$3); }
    | cmp_expr OP_NE  divis_expr  { $$=rk_chain_cmp($1,TT_NE,$3); }
    | cmp_expr '<'    divis_expr  { $$=rk_chain_cmp($1,TT_LT,$3); }
    | cmp_expr '>'    divis_expr  { $$=rk_chain_cmp($1,TT_GT,$3); }
    | cmp_expr OP_LE  divis_expr  { $$=rk_chain_cmp($1,TT_LE,$3); }
    | cmp_expr OP_GE  divis_expr  { $$=rk_chain_cmp($1,TT_GE,$3); }
    | divis_expr OP_SEQ divis_expr  { $$=expr_binary(TT_LEQ,$1,$3); }
    | divis_expr OP_SNE divis_expr  { $$=expr_binary(TT_LNE,$1,$3); }
    | divis_expr OP_SLT divis_expr  { $$=expr_binary(TT_LLT,$1,$3); }
    | divis_expr OP_SLE divis_expr  { $$=expr_binary(TT_LLE,$1,$3); }
    | divis_expr OP_SGT divis_expr  { $$=expr_binary(TT_LGT,$1,$3); }
    | divis_expr OP_SGE divis_expr  { $$=expr_binary(TT_LGE,$1,$3); }
    | divis_expr OP_SMATCH LIT_REGEX
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          $$ = c; }
    | divis_expr OP_SMATCH LIT_MATCH_GLOBAL
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          $$ = c; }
    | divis_expr OP_SMATCH LIT_SUBST
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          $$ = c; }
    | divis_expr                 { $$=$1; }
    ;
divis_expr
    : divis_expr OP_DIVIS jct_expr  { $$=expr_binary(TT_DIVIS,$1,$3); }
    | jct_expr                 { $$=$1; }
    ;
jct_expr
    : jct_expr '|' range_expr  { $$=mk_junction("any",$1,$3); }
    | jct_expr '&' range_expr  { $$=mk_junction("all",$1,$3); }
    | dor_expr                 { $$=$1; }
    ;
dor_expr
    : dor_expr OP_DOR range_expr
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,$1); expr_add_child(c,$3); $$=c; }
    | range_expr               { $$=$1; }
    ;
range_expr
    : add_expr OP_RANGE    add_expr { $$=expr_binary(TT_TO,$1,$3); }
    | add_expr OP_RANGE_EX add_expr { $$=rk_range_ex($1,$3); }
    | add_expr                      { $$=$1; }
    ;
add_expr
    : add_expr '~' repl_expr  { $$=expr_binary(TT_CAT,$1,$3); }
    | repl_expr               { $$=$1; }
    ;
repl_expr
    : repl_expr OP_REP_X addsub_expr  { $$=expr_binary(TT_XREP,$1,$3); }
    | repl_expr OP_REP_XX addsub_expr { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,$1); expr_add_child(call,$3); $$=call; }
    | addsub_expr                     { $$=$1; }
    ;
addsub_expr
    : addsub_expr '+' mul_expr  { $$=expr_binary(TT_ADD,$1,$3); }
    | addsub_expr '-' mul_expr  { $$=expr_binary(TT_SUB,$1,$3); }
    | mul_expr                  { $$=$1; }
    ;
mul_expr
    : mul_expr '*'     unary_expr  { $$=expr_binary(TT_MUL,$1,$3); }
    | mul_expr '/'     unary_expr  { $$=expr_binary(TT_DIV,$1,$3); }
    | mul_expr '%'     unary_expr  { $$=expr_binary(TT_MOD,$1,$3); }
    | mul_expr OP_DIV  unary_expr  { $$=expr_binary(TT_DIV,$1,$3); }
    | mul_expr OP_BAND unary_expr
        { tree_t *c=make_call("iand"); expr_add_child(c,$1); expr_add_child(c,$3); $$=c; }
    | mul_expr OP_SHL  unary_expr
        { tree_t *c=make_call("ishift"); expr_add_child(c,$1); expr_add_child(c,$3); $$=c; }
    | unary_expr                   { $$=$1; }
    ;
unary_expr
    : '-' unary_expr %prec UMINUS  { $$=expr_unary(TT_MNS,$2); }
    | '!' unary_expr               { $$=expr_unary(TT_NOT,$2); }
    | CARET unary_expr             { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; $$=rk_range_ex(z,$2); }
    | OP_REDUCE unary_expr
        { const char *rop = !strcmp($1,"+") ? "__rk_reduce_add" : !strcmp($1,"-") ? "__rk_reduce_sub"
                          : !strcmp($1,"*") ? "__rk_reduce_mul" : !strcmp($1,"~") ? "__rk_reduce_cat"
                          : !strcmp($1,"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,$2); free($1); $$=e; }
    | pow_expr                     { $$=$1; }
    ;
pow_expr
    : postfix_expr OP_POW unary_expr  { $$=expr_binary(TT_POW,$1,$3); }
    | postfix_expr                    { $$=$1; }
    ;
scalar_list
    : VAR_SCALAR                    { $$ = exprlist_append(exprlist_new(), var_node($1)); free($1); }
    | scalar_list ',' VAR_SCALAR    { $$ = exprlist_append($1, var_node($3)); free($3); }
    ;
meth_name
    : IDENT      { $$=$1; }
    | KW_SORT    { $$=strdup("sort"); }
    | KW_MAP     { $$=strdup("map"); }
    | KW_GREP    { $$=strdup("grep"); }
    | KW_SAY     { $$=strdup("say"); }
    | KW_PRINT   { $$=strdup("print"); }
    | KW_TAKE    { $$=strdup("take"); }
    | KW_RETURN  { $$=strdup("return"); }
    | KW_EXISTS  { $$=strdup("exists"); }
    | KW_DELETE  { $$=strdup("delete"); }
    | KW_JOIN    { $$=strdup("join"); }
    | TESTOP     { $$=$1; }
    ;
postfix_expr : call_expr { $$=$1; } ;
call_expr
    : KW_JOIN expr ',' arg_list
        { tree_t *e=make_call("join");
          expr_add_child(e, $2);
          ExprList *args=$4;
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          $$=e; }
    | IDENT '(' arg_list ')'
        { tree_t *e=make_call($1);
          ExprList *args=$3;
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          $$=e; }
    | IDENT '(' ')'  { $$=make_call($1); }
    | IDENT '(' named_arg_list ')'
        { $$ = rk_named_call($1, NULL, $3); free($1); }
    | IDENT '(' arg_list ',' named_arg_list ')'
        { $$ = rk_named_call($1, $3, $5); free($1); }
    | VAR_SCALAR '(' arg_list ')'
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node($1));
          ExprList *args=$3;
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          $$=e; }
    | VAR_SCALAR '(' ')'
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node($1)); $$=e; }
    | IDENT '.' KW_NEW '(' named_arg_list ')'
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, $1)); free($1);
          ExprList *nargs = $5;
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          $$ = c; }
    | IDENT '.' KW_NEW '(' arg_list ')'
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, $1)); free($1);
          ExprList *args = $5;
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          $$ = c; }
    | IDENT '.' KW_NEW '(' ')'
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, $1)); free($1);
          $$ = c; }
    | IDENT '.' KW_NEW
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, $1)); free($1);
          $$ = c; }
    | IDENT '.' IDENT
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node($1)); free($1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          $$ = c; }
    | IDENT '.' IDENT '(' arg_list ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node($1)); free($1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          ExprList *args = $5;
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          $$ = c; }
    | IDENT '.' IDENT '(' ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node($1)); free($1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          $$ = c; }
    | IDENT '.' CARET IDENT
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node($1)); free($1);
          { size_t _l = strlen($4); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,$4,_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free($4);
          $$ = c; }
    | atom '.' CARET IDENT
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          { size_t _l = strlen($4); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,$4,_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free($4);
          $$ = c; }
    | atom '.' meth_name '(' arg_list ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          ExprList *args = $5;
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          $$ = c; }
    | atom '.' meth_name '(' named_arg_list ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          ExprList *nargs = $5;
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          $$ = c; }
    | atom '.' meth_name '(' ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          $$ = c; }
    | atom '.' meth_name
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          $$ = c; }
    | call_expr '.' meth_name '(' arg_list ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          ExprList *args = $5;
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          $$ = c; }
    | call_expr '.' meth_name '(' ')'
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          $$ = c; }
    | call_expr '.' meth_name
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, $1);
          ast_push(c, leaf_sval(TT_QLIT, $3)); free($3);
          $$ = c; }
    | KW_DIE expr
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,$2); $$=d; }
    | KW_MAP closure expr
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, $2); ast_push(c, $3); $$ = c; }
    | KW_GREP closure expr
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, $2); ast_push(c, $3); $$ = c; }
    | KW_SORT expr
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, $2); $$ = c; }
    | KW_SORT closure expr
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, $2); ast_push(c, $3); $$ = c; }
    | atom           { $$=$1; }
    ;
arg_list
    : expr              { $$=exprlist_append(exprlist_new(),$1); }
    | arg_list ',' expr { $$=exprlist_append($1,$3); }
    | arg_list ','      { $$=$1; }
    ;
atom
    : LIT_INT         { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=$1; $$=e; }
    | LIT_FLOAT       { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=$1; $$=e; }
    | LIT_STR         { $$=leaf_sval(TT_QLIT,$1); }
    | WORDLIST
        { tree_t *call=make_call("__rk_arr"); char *s=$1; int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free($1);
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; $$=only; }
          else { $$=call; } }
    | LIT_INTERP_STR  { $$=lower_interp_str($1); }
    | VAR_SCALAR      { $$=var_node($1); }
    | OP_INC VAR_SCALAR { $$=rk_incdec($2,1); }
    | OP_DEC VAR_SCALAR { $$=rk_incdec($2,0); }
    | VAR_SCALAR OP_INC { $$=rk_post_incdec($1,1); }
    | VAR_SCALAR OP_DEC { $$=rk_post_incdec($1,0); }
    | VAR_ARRAY       { $$=var_node($1); }
    | VAR_HASH        { $$=var_node($1); }
    | VAR_CAPTURE
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = $1;
          ast_push(c, idx); $$ = c; }
    | VAR_FH
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = $1;
          ast_push(c, idx); $$ = c; }
    | VAR_NAMED_CAPTURE
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, $1)); $$ = c; }
    | VAR_ARRAY '[' expr ']'
        { $$ = rk_arr_index($1, $3); }
    | VAR_ARRAY '[' expr ',' arg_list ']'
        { $$ = rk_arr_pick($1, $3, $5); }
    | VAR_ARRAY '[' '*' '-' expr ']'
        { $$ = rk_arr_end_index($1, $5, TT_SUB); }
    | VAR_ARRAY '[' '*' '+' expr ']'
        { $$ = rk_arr_end_index($1, $5, TT_ADD); }
    | ARR_ALL_SLICE
        { $$ = rk_arr_all($1); }
    | VAR_HASH '<' IDENT '>'
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node($1)); ast_push(c,leaf_sval(TT_QLIT,$3)); $$=c; }
    | VAR_HASH '{' expr '}'
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node($1)); ast_push(c,$3); $$=c; }
    | KW_EXISTS VAR_HASH '<' IDENT '>'
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node($2)); ast_push(c,leaf_sval(TT_QLIT,$4)); $$=c; }
    | KW_EXISTS VAR_HASH '{' expr '}'
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node($2)); ast_push(c,$4); $$=c; }
    | IDENT           { $$=var_node($1); }
    | VAR_TWIGIL
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare($1)); free($1);
          $$ = fe; }
    | VAR_ARRAY_TWIGIL
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare($1)); free($1);
          $$ = fe; }
    | VAR_HASH_TWIGIL
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare($1)); free($1);
          $$ = fe; }
    | '[' ']'         { $$=make_call("__rk_arr"); }
    | '[' expr ']'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$2); $$=call; }
    | '[' expr ',' ']'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$2); $$=call; }
    | '[' expr ',' arg_list ']'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$2);
          ExprList *a=$4; if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } $$=call; }
    | '(' ')'         { $$=make_call("__rk_arr"); }
    | '(' expr ')'    { $$=$2; }
    | '(' expr ',' ')'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$2); $$=call; }
    | '(' expr ',' arg_list ')'
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,$2);
          ExprList *a=$4; if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } $$=call; }
    | block           { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,$1); $$=b; }
    | KW_SUB block    { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,$2); $$=b; }
    ;
%%
extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
