/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         RAKU_YYSTYPE
/* Substitute the variable and function names.  */
#define yyparse         raku_yyparse
#define yylex           raku_yylex
#define yyerror         raku_yyerror
#define yydebug         raku_yydebug
#define yynerrs         raku_yynerrs
#define yylval          raku_yylval
#define yychar          raku_yychar

/* First part of user prologue.  */
#line 11 "raku.y"

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
#define RK_ARRNAME_MAX 256
static const char *rk_array_names[RK_ARRNAME_MAX];
static int rk_array_names_n = 0;
static void rk_mark_array_name(const char *bare) {
    if (!bare) return;
    for (int i = 0; i < rk_array_names_n; i++) if (!strcmp(rk_array_names[i], bare)) return;
    if (rk_array_names_n < RK_ARRNAME_MAX) rk_array_names[rk_array_names_n++] = intern(bare);
}
int rk_is_array_name(const char *bare) {
    if (!bare) return 0;
    for (int i = 0; i < rk_array_names_n; i++) if (!strcmp(rk_array_names[i], bare)) return 1;
    return 0;
}
const char *rk_arrlit_scalars[RK_ARRNAME_MAX];
int rk_arrlit_scalars_n = 0;
int rk_is_arrlit_scalar(const char *bare) {
    if (!bare) return 0;
    for (int i = 0; i < rk_arrlit_scalars_n; i++) if (!strcmp(rk_arrlit_scalars[i], bare)) return 1;
    return 0;
}
static void rk_mark_arrlit_scalar(const char *bare, const tree_t *rhs) {
    if (!bare || !rhs || rhs->t != TT_FNC || !rhs->v.sval || strcmp(rhs->v.sval, "__rk_arr_lit")) return;
    if (rk_is_arrlit_scalar(bare) || rk_arrlit_scalars_n >= RK_ARRNAME_MAX) return;
    rk_arrlit_scalars[rk_arrlit_scalars_n++] = intern(bare);
}
static const char *rk_var_ident(const char *s) {
    if (s && (s[0] == '@' || s[0] == '%')) return s;
    return strip_sigil(s);
}
static tree_t *var_node(const char *name) {
    const char *id = rk_var_ident(name);
    if (name && name[0] == '@') rk_mark_array_name(id);
    return leaf_sval(TT_VAR, id);
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
static tree_t *rk_testop_call(const char *name, ExprList *a) {
    tree_t *c = leaf_sval(TT_FNC, name);
    tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name);
    expr_add_child(c, n);
    if (!a) return c;
    if (a->count == 1 && a->items[0] && a->items[0]->t == TT_FNC && a->items[0]->v.sval && !strcmp(a->items[0]->v.sval, "__rk_arr")) {
        tree_t *lst = a->items[0];
        for (int i = 1; i < lst->n; i++) expr_add_child(c, lst->c[i]);
        return c;
    }
    for (int i = 0; i < a->count; i++) expr_add_child(c, a->items[i]);
    return c;
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
static tree_t *rk_byref_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, "@")); return p;
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
    if (hi && hi->t == TT_VAR && rk_is_array_name(hi->v.sval)) {
        tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, hi); ast_push(el, leaf_sval(TT_QLIT, "elems"));
        hi = el;
    }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
}
static tree_t *rk_numeric_ctx(tree_t *e) {
    if (e && e->t == TT_VAR && rk_is_array_name(e->v.sval)) {
        tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, e); ast_push(el, leaf_sval(TT_QLIT, "elems"));
        return el;
    }
    return e;
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
            char vname[256]; int vlen=0; vname[vlen++]=s[i]; i++;
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

#line 489 "raku.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "raku.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LIT_INT = 3,                    /* LIT_INT  */
  YYSYMBOL_LIT_FLOAT = 4,                  /* LIT_FLOAT  */
  YYSYMBOL_LIT_STR = 5,                    /* LIT_STR  */
  YYSYMBOL_LIT_INTERP_STR = 6,             /* LIT_INTERP_STR  */
  YYSYMBOL_LIT_REGEX = 7,                  /* LIT_REGEX  */
  YYSYMBOL_LIT_MATCH_GLOBAL = 8,           /* LIT_MATCH_GLOBAL  */
  YYSYMBOL_LIT_SUBST = 9,                  /* LIT_SUBST  */
  YYSYMBOL_VAR_SCALAR = 10,                /* VAR_SCALAR  */
  YYSYMBOL_VAR_ARRAY = 11,                 /* VAR_ARRAY  */
  YYSYMBOL_VAR_HASH = 12,                  /* VAR_HASH  */
  YYSYMBOL_VAR_TWIGIL = 13,                /* VAR_TWIGIL  */
  YYSYMBOL_IDENT = 14,                     /* IDENT  */
  YYSYMBOL_QIDENT = 15,                    /* QIDENT  */
  YYSYMBOL_VAR_ARRAY_TWIGIL = 16,          /* VAR_ARRAY_TWIGIL  */
  YYSYMBOL_VAR_HASH_TWIGIL = 17,           /* VAR_HASH_TWIGIL  */
  YYSYMBOL_CARET = 18,                     /* CARET  */
  YYSYMBOL_DOLLAR_LBRACKET = 19,           /* DOLLAR_LBRACKET  */
  YYSYMBOL_VAR_CAPTURE = 20,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 21,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 22,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_USE = 23,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 24,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 25,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 26,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 27,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 28,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 29,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 30,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 31,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 32,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 33,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 34,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 35,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 36,                 /* KW_RETURN  */
  YYSYMBOL_KW_EXIT = 37,                   /* KW_EXIT  */
  YYSYMBOL_KW_CONSTANT = 38,               /* KW_CONSTANT  */
  YYSYMBOL_KW_ENUM = 39,                   /* KW_ENUM  */
  YYSYMBOL_KW_JOIN = 40,                   /* KW_JOIN  */
  YYSYMBOL_KW_GIVEN = 41,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 42,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 43,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 44,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 45,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 46,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 47,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 48,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 49,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 50,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 51,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 52,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 53,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 54,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 55,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 56,                   /* KW_SORT  */
  YYSYMBOL_KW_REVERSE = 57,                /* KW_REVERSE  */
  YYSYMBOL_KW_TRY = 58,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 59,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 60,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 61,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 62,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 63,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 64,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 65,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 66,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 67,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 68,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 69,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 70,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 71,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 72,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 73,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 74,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 75,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 76,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 77,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 78,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 79,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 80,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 81,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 82,                  /* KW_REGEX  */
  YYSYMBOL_KW_MODULE = 83,                 /* KW_MODULE  */
  YYSYMBOL_OP_FATARROW = 84,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 85,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 86,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 87,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 88,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 89,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 90,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 91,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 92,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 93,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 94,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 95,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 96,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 97,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 98,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 99,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 100,              /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 101,              /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 102,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 103,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 104,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 105,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 106,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 107,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 108,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 109,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 110,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 111,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 112,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 113,                   /* OP_DIV  */
  YYSYMBOL_OP_BAND = 114,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 115,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 116,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 117,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 118,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 119,                   /* OP_POW  */
  YYSYMBOL_120_ = 120,                     /* '='  */
  YYSYMBOL_121_ = 121,                     /* '!'  */
  YYSYMBOL_122_ = 122,                     /* '<'  */
  YYSYMBOL_123_ = 123,                     /* '>'  */
  YYSYMBOL_124_ = 124,                     /* '|'  */
  YYSYMBOL_125_ = 125,                     /* '&'  */
  YYSYMBOL_126_ = 126,                     /* '~'  */
  YYSYMBOL_127_ = 127,                     /* '+'  */
  YYSYMBOL_128_ = 128,                     /* '-'  */
  YYSYMBOL_129_ = 129,                     /* '*'  */
  YYSYMBOL_130_ = 130,                     /* '/'  */
  YYSYMBOL_131_ = 131,                     /* '%'  */
  YYSYMBOL_UMINUS = 132,                   /* UMINUS  */
  YYSYMBOL_133_ = 133,                     /* '.'  */
  YYSYMBOL_134_ = 134,                     /* ';'  */
  YYSYMBOL_135_ = 135,                     /* '('  */
  YYSYMBOL_136_ = 136,                     /* ')'  */
  YYSYMBOL_137_ = 137,                     /* ','  */
  YYSYMBOL_138_ = 138,                     /* '['  */
  YYSYMBOL_139_ = 139,                     /* ']'  */
  YYSYMBOL_140_ = 140,                     /* '{'  */
  YYSYMBOL_141_ = 141,                     /* '}'  */
  YYSYMBOL_142_ = 142,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 143,                 /* $accept  */
  YYSYMBOL_program = 144,                  /* program  */
  YYSYMBOL_stmt_list = 145,                /* stmt_list  */
  YYSYMBOL_stmt = 146,                     /* stmt  */
  YYSYMBOL_if_stmt = 147,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 148,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 149,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 150,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 151,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 152,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 153,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 154,                /* loop_incr  */
  YYSYMBOL_for_stmt = 155,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 156,               /* given_stmt  */
  YYSYMBOL_when_list = 157,                /* when_list  */
  YYSYMBOL_sub_decl = 158,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 159,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 160,                 /* sub_body  */
  YYSYMBOL_method_body = 161,              /* method_body  */
  YYSYMBOL_pkg_name = 162,                 /* pkg_name  */
  YYSYMBOL_class_decl = 163,               /* class_decl  */
  YYSYMBOL_role_decl = 164,                /* role_decl  */
  YYSYMBOL_module_decl = 165,              /* module_decl  */
  YYSYMBOL_is_clauses = 166,               /* is_clauses  */
  YYSYMBOL_class_body_list = 167,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 168,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 169,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 170,           /* named_arg_list  */
  YYSYMBOL_pair_list = 171,                /* pair_list  */
  YYSYMBOL_param_list = 172,               /* param_list  */
  YYSYMBOL_block = 173,                    /* block  */
  YYSYMBOL_closure = 174,                  /* closure  */
  YYSYMBOL_expr = 175,                     /* expr  */
  YYSYMBOL_tern_expr = 176,                /* tern_expr  */
  YYSYMBOL_or_expr = 177,                  /* or_expr  */
  YYSYMBOL_and_expr = 178,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 179,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 180,               /* divis_expr  */
  YYSYMBOL_jct_expr = 181,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 182,                 /* dor_expr  */
  YYSYMBOL_range_expr = 183,               /* range_expr  */
  YYSYMBOL_add_expr = 184,                 /* add_expr  */
  YYSYMBOL_repl_expr = 185,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 186,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 187,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 188,               /* unary_expr  */
  YYSYMBOL_pow_expr = 189,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 190,              /* scalar_list  */
  YYSYMBOL_meth_name = 191,                /* meth_name  */
  YYSYMBOL_postfix_expr = 192,             /* postfix_expr  */
  YYSYMBOL_call_expr = 193,                /* call_expr  */
  YYSYMBOL_arg_list = 194,                 /* arg_list  */
  YYSYMBOL_atom = 195                      /* atom  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined RAKU_YYSTYPE_IS_TRIVIAL && RAKU_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5403

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  143
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  482
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1205

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   375


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   121,     2,     2,     2,   131,   125,     2,
     135,   136,   129,   127,   137,   128,   133,   130,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   142,   134,
     122,   120,   123,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   138,     2,   139,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   140,   124,   141,   126,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   132
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   509,   509,   520,   521,   524,   526,   528,   530,   534,
     536,   538,   540,   542,   546,   550,   552,   556,   560,   562,
     564,   566,   568,   570,   574,   578,   580,   582,   584,   586,
     588,   590,   592,   594,   596,   598,   600,   602,   610,   612,
     614,   616,   618,   620,   622,   625,   628,   630,   633,   636,
     638,   642,   644,   646,   649,   652,   654,   657,   660,   663,
     665,   670,   674,   678,   683,   687,   688,   690,   692,   694,
     696,   699,   701,   703,   705,   708,   711,   714,   717,   720,
     722,   724,   726,   728,   731,   733,   735,   737,   740,   743,
     747,   750,   752,   754,   756,   759,   762,   765,   768,   771,
     774,   777,   778,   779,   780,   781,   782,   783,   785,   787,
     789,   791,   792,   793,   794,   795,   796,   797,   799,   801,
     803,   805,   806,   807,   808,   809,   812,   814,   816,   818,
     820,   822,   824,   826,   830,   832,   834,   836,   838,   840,
     844,   846,   850,   852,   854,   856,   860,   862,   866,   868,
     870,   874,   876,   878,   882,   885,   891,   897,   902,   908,
     913,   918,   926,   937,   938,   944,   952,   958,   964,   972,
     978,   984,   993,  1000,  1009,  1018,  1027,  1034,  1041,  1042,
    1045,  1048,  1050,  1052,  1054,  1056,  1058,  1060,  1062,  1065,
    1067,  1069,  1071,  1074,  1077,  1083,  1084,  1085,  1088,  1091,
    1093,  1095,  1097,  1099,  1101,  1103,  1105,  1108,  1110,  1112,
    1114,  1117,  1120,  1126,  1127,  1130,  1146,  1161,  1176,  1177,
    1189,  1203,  1204,  1207,  1210,  1213,  1216,  1219,  1222,  1225,
    1228,  1231,  1234,  1237,  1240,  1247,  1254,  1261,  1268,  1272,
    1276,  1280,  1284,  1291,  1298,  1305,  1312,  1322,  1330,  1338,
    1347,  1354,  1361,  1371,  1379,  1389,  1404,  1405,  1410,  1415,
    1422,  1426,  1430,  1434,  1440,  1442,  1444,  1446,  1450,  1451,
    1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,
    1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,
    1474,  1475,  1476,  1478,  1480,  1482,  1484,  1486,  1489,  1491,
    1493,  1495,  1497,  1500,  1503,  1507,  1510,  1513,  1516,  1518,
    1522,  1525,  1528,  1531,  1534,  1537,  1539,  1541,  1543,  1545,
    1547,  1549,  1551,  1553,  1557,  1560,  1561,  1562,  1563,  1564,
    1565,  1566,  1567,  1572,  1577,  1579,  1582,  1584,  1587,  1588,
    1591,  1592,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,
    1603,  1604,  1605,  1606,  1607,  1613,  1619,  1625,  1628,  1629,
    1632,  1633,  1634,  1637,  1639,  1642,  1643,  1644,  1647,  1648,
    1651,  1652,  1653,  1656,  1657,  1658,  1661,  1662,  1663,  1664,
    1665,  1667,  1669,  1672,  1673,  1674,  1675,  1676,  1681,  1684,
    1685,  1688,  1689,  1692,  1693,  1694,  1695,  1696,  1697,  1698,
    1699,  1700,  1701,  1702,  1703,  1704,  1706,  1708,  1714,  1719,
    1720,  1722,  1724,  1729,  1731,  1737,  1743,  1747,  1751,  1756,
    1763,  1768,  1774,  1780,  1787,  1794,  1799,  1804,  1811,  1816,
    1821,  1823,  1825,  1827,  1829,  1831,  1833,  1835,  1838,  1839,
    1840,  1843,  1844,  1845,  1846,  1855,  1856,  1857,  1858,  1859,
    1860,  1861,  1862,  1863,  1867,  1871,  1874,  1876,  1878,  1880,
    1882,  1884,  1886,  1888,  1890,  1892,  1893,  1897,  1901,  1905,
    1906,  1908,  1910,  1913,  1914,  1916,  1918,  1921,  1922,  1923,
    1925,  1928,  1929
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if RAKU_YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "LIT_INT", "LIT_FLOAT",
  "LIT_STR", "LIT_INTERP_STR", "LIT_REGEX", "LIT_MATCH_GLOBAL",
  "LIT_SUBST", "VAR_SCALAR", "VAR_ARRAY", "VAR_HASH", "VAR_TWIGIL",
  "IDENT", "QIDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL", "CARET",
  "DOLLAR_LBRACKET", "VAR_CAPTURE", "VAR_FH", "VAR_NAMED_CAPTURE",
  "KW_USE", "TESTOP", "KW_MY", "KW_SAY", "KW_PRINT", "KW_IF", "KW_ELSE",
  "KW_ELSIF", "KW_WHILE", "KW_FOR", "KW_SUB", "KW_GATHER", "KW_TAKE",
  "KW_RETURN", "KW_EXIT", "KW_CONSTANT", "KW_ENUM", "KW_JOIN", "KW_GIVEN",
  "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT", "KW_EXISTS",
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_LOOP", "KW_LAST",
  "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_REVERSE", "KW_TRY",
  "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW",
  "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE",
  "ARR_ALL_SLICE", "SLURPY_POS", "SLURPY_LOL", "SLURPY_NAMED",
  "KW_HANDLES", "WORDLIST", "OP_COLON_D", "OP_COLON_U", "YADA",
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "KW_MODULE",
  "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE",
  "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT", "OP_SLE", "OP_SGT",
  "OP_SGE", "OP_AND", "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND",
  "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ",
  "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_BAND",
  "OP_SHL", "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'",
  "':'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "elsif_tail", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "loop_stmt", "loop_incr", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "scalar_methcall", "sub_body", "method_body", "pkg_name",
  "class_decl", "role_decl", "module_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "or_expr", "and_expr", "cmp_expr", "divis_expr", "jct_expr", "dor_expr",
  "range_expr", "add_expr", "repl_expr", "addsub_expr", "mul_expr",
  "unary_expr", "pow_expr", "scalar_list", "meth_name", "postfix_expr",
  "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-883)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-482)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -883,    51,  2293,  -883,  -883,  -883,  -883,  -883,   617,   -63,
     -50,   -39,     7,  -883,  -883,  5213,  2390,  -883,  -883,  -883,
     269,  2466,   143,  4225,  4301,  4377,  4453,  4529,    -8,   -17,
    4529,  2542,  2618,   107,   105,  4529,  4529,   119,   163,  4605,
    4681,   132,   140,   123,   145,   184,   184,  4757,  4529,   132,
     132,  4529,   387,   387,    41,  5213,  -883,  -883,   387,   387,
     368,   379,  5213,  5213,  5213,  -883,  2694,  2771,   158,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,   743,
    -883,  -883,  -883,  -883,  5270,   749,   333,   518,   325,   493,
     574,   120,   396,  -883,     9,   683,   690,   448,  -883,  -883,
     380,   358,   383,   550,  -883,  -883,  4529,  4529,  4529,  4529,
    4529,  4529,   559,  2847,  4833,  2924,   562,  4529,  4529,   455,
      96,   816,   182,   461,    19,  -883,    34,   132,  3000,  -883,
    -883,   624,   139,  -883,   322,  3077,  3153,  -883,  3229,  -883,
      73,    81,   230,   290,   691,   589,   560,   583,  3000,   665,
    3000,   372,  3000,   132,  3000,   132,   118,   113,   303,  -883,
    -883,  -883,   177,  -883,   226,  -883,   381,   519,   536,   585,
     546,   533,   159,   172,  3000,   132,  3000,   132,   342,  3306,
    -883,  4529,  4529,  -883,  4529,  4529,  -883,  4529,  3382,  4529,
    3458,  4529,  -883,  -883,   681,  -883,  -883,  -883,  -883,  -883,
     611,   610,     5,  -883,   618,   622,  -883,  -883,  -883,  -883,
    -883,   705,  -883,   687,   794,  -883,   340,   627,  1488,  4529,
    4529,  4529,  4529,  4529,  4529,  4529,  4529,  -883,  4529,  4529,
    4529,  4529,  4529,  4529,  4529,  4529,  -883,  4529,  5213,  5213,
    5213,  5213,  5213,  5213,  5213,  5213,  5213,  5213,  5213,  5213,
    5213,  5213,  5213,   873,  5213,  5213,  5213,  5213,  5213,  5213,
    5213,  5213,  5213,  5213,  5213,  5213,  5213,  5213,  5213,  5213,
    5213,  5213,  1050,  1015,   801,   651,   697,   711,   719,   725,
     735,   -73,  -883,   802,  3000,   314,   814,   579,   716,   733,
     750,  -883,   771,   910,   792,    -6,  -883,   939,   810,   812,
    2924,   941,  4529,  4529,  4529,  4529,  4529,  4529,  4529,  4529,
    3534,  -883,  -883,   823,  -883,   834,   847,   687,   829,  -883,
    4529,  4529,  -883,  4909,  -883,  4985,  -883,   310,   312,   313,
     367,   865,   869,  -883,   840,   842,  4529,  4529,  4529,  4529,
    4529,  4529,  4529,  -883,  4529,   858,  4529,  4529,  -883,  4529,
     868,   410,   871,  -883,   981,  4529,  -883,  5213,  5213,    25,
    -883,  -883,  -883,  4529,  4529,  4529,  -883,  4529,  4529,  -883,
    4529,  4529,   862,  4529,  -883,   984,  4529,   986,  4529,   881,
     999,   888,  -883,  4529,  4529,  1001,   875,   904,   906,   915,
     950,   874,  4529,  -883,  -883,  -883,   132,    18,  -883,    30,
     922,   938,  -883,  -883,  -883,  3610,   943,  1083,  3687,  -883,
    -883,   864,   274,   305,   975,  4225,  4301,  4529,  2002,    63,
      72,  -883,    22,   969,  1006,  1012,  1036,  1038,  1058,  1061,
    1062,  1063,  1064,  1065,  1068,  1069,  1071,  1073,  1074,  -883,
     325,  1013,   493,  1009,  1009,  1009,  1009,  1009,  1009,  1009,
    1009,  1009,  1009,  1009,  1009,  -883,  -883,  -883,   120,  -883,
    -883,  -883,  1007,  1007,   683,   690,   690,   448,   448,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  1041,
    1170,  1075,  -883,  3763,  -883,  -883,  -883,  -883,  -883,  -883,
    4529,  3840,  -883,   890,  -883,  4529,  4529,  4529,  4529,  1084,
    1089,  1091,  -883,  3917,  -883,  1131,  4529,  1077,  -883,   -12,
    -883,  1348,   753,  1090,  1076,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,   764,  -883,  -883,  -883,  1080,  -883,  1081,
    3000,   636,  1132,   244,  3994,   709,  1085,  4529,  -883,  5061,
    -883,  4529,  -883,    66,  -883,  4529,  4529,  1098,  3610,  1086,
    1087,  1088,  1092,  1093,  1094,  1096,   710,  3610,  1097,  1104,
     717,   132,   -18,  5137,  -883,   132,   132,   189,   -70,   -44,
      24,  -883,    16,  -883,  -883,  -883,  1099,   895,  1627,   718,
    1106,  1107,  1108,  1109,  1110,  1111,  -883,  1095,    43,  1101,
    1082,  1102,  1105,   132,   132,   132,  1113,  1114,  1129,  4529,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  1236,  1237,  -883,
      21,  1099,   898,   180,   452,   168,  1766,  -883,   907,  4529,
    -883,  -883,   765,  1239,  2924,  4529,  4529,   339,   285,   215,
    -883,   186,  4529,  4529,  -883,  4529,  4529,  -883,  4529,  4529,
    4529,  4529,  4529,  4529,  4529,  4529,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  5213,  4071,  -883,  1271,  1120,   909,  1121,
    -883,   911,  3610,   723,  1117,  1118,   768,  4529,  4529,  4529,
    -883,   916,  -883,   923,   929,  -883,  4529,  1176,  1248,   931,
    -883,  -883,  -883,  -883,  -883,  -883,   942,  -883,  4529,  4529,
    4529,   944,  -883,    74,  -883,  1130,  3000,   724,  1134,  1099,
     946,  1138,  1144,  4529,   951,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,   955,  -883,  -883,  -883,  1070,  -883,  -883,
    3000,   132,  -883,  -883,  1253,  -883,  1260,  1269,  1266,  4529,
    1166,  1284,  1285,  -883,  1099,   458,  4225,  4301,  2078,  -883,
     426,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  4529,   132,
    -883,  -883,  -883,  1162,  1163,  1270,  -883,  -883,  -883,  -883,
    4529,  1164,  -883,  -883,    62,   966,   439,  1238,  -883,  -883,
    1099,  1099,   965,   972,  1287,  1288,  1289,  -883,  -883,  -883,
    1172,  -883,   136,   786,  1167,  -114,  4529,  4529,  4529,  4529,
    4529,  4529,  -883,  -883,  -883,  -883,   217,   300,   359,   362,
     386,   392,   394,   402,   403,   405,   407,   416,  -883,  -883,
     974,  -883,   976,   980,  -883,  1173,  -883,  -883,   982,  -883,
    -883,  -883,  -883,  1175,  1178,  1179,  -883,  -883,  -883,  1174,
    4529,  1180,  -883,  3610,   729,  -883,  -883,  1182,  1230,  1234,
    -883,   987,  -883,  4529,  -883,  -883,  1099,  -883,  -883,   730,
    1185,  1186,   -18,  -883,   991,  1100,   132,   132,   132,  -883,
    -883,  4529,  -883,  -883,  -883,    45,  -883,   135,  -883,  -883,
    -883,   524,   344,  -883,   262,  4529,  4529,  4529,  4529,  4529,
    4529,  4529,  4529,  -883,   132,  1181,  -883,  -883,   132,  1187,
    4529,  -883,   382,   450,    44,    75,   473,  1189,  1190,  1050,
    -883,  -883,  1099,  1099,  1322,  1323,  1325,  -883,  4529,  4148,
    1215,  1216,  -883,   417,   425,   453,   457,   467,   470,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  1203,  -883,  -883,  -883,  -883,
    -883,  4529,   995,  -883,  -883,  4529,  4529,  3610,   731,  -883,
    -883,  4529,  -883,  -883,  -883,  -883,   132,   132,  -883,  -883,
    -883,  -883,  -883,  1324,  4529,  1219,  1332,  1333,  4529,  4529,
    4529,  -883,  -883,  -883,   471,   479,   480,   495,   496,   497,
     501,   523,  -883,  -883,  -883,  4529,  1208,  -883,   259,  1267,
    -883,   275,  -883,  1334,  1335,  1213,  4529,  -883,  1336,  1341,
    1222,  4529,  -883,   223,   301,  1223,  1229,  -883,  -883,   565,
    -883,  -883,  -883,  -883,  -883,   540,  1231,  1002,  4529,  4529,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  1235,  1240,  -883,
    -883,  1026,  -883,   741,  1137,  -883,  -883,  -883,  4529,  -883,
    -883,   543,   548,   554,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  1241,   132,  1233,  1032,  1242,  1905,  1233,  1042,
    1244,  1245,  -883,  1246,  1250,  1251,  -883,  1252,  1373,  1375,
    1256,  4529,  -883,  1377,  1379,  1261,  4529,  -883,  -883,  -883,
     282,  -883,  -883,  -883,  1255,   557,   558,  -883,  -883,  1263,
    -883,   132,  -883,  -883,  -883,  -883,  -883,  4529,  -883,  -883,
    1233,  -883,  4225,  4301,  2154,  -883,   631,  -883,  1233,  -883,
    -883,  -883,  -883,  -883,  -883,  1276,  1278,  -883,  1280,  1281,
    1282,  -883,  1286,  1233,  1044,  -883,  -883,  -883,  -883,  -883,
    1264,  -883,   578,   360,  -883,   377,  4529,  4529,  4529,  4529,
    4529,  4529,  4529,  4529,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  1233,   132,  4529,  4529,  4529,  -883,  -883,
    -883,   570,   602,   605,   607,   608,   623,   626,   642,  -883,
    -883,   645,   648,   662,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   441,   442,   443,   445,   446,   451,
     452,   466,   465,   467,   468,     0,     0,   453,   454,   455,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   460,   444,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     3,     4,
     103,   104,   111,   112,   113,   114,   105,   106,   121,     0,
     122,   123,   125,   124,   110,     0,   335,   337,   339,   341,
     357,   359,   362,   364,   367,   369,   372,   375,   382,   388,
     390,   406,   437,     0,   449,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   446,   451,   452,   466,   465,     0,     0,   481,
     386,   446,   451,   473,     0,     0,     0,    38,     0,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,   482,
     333,   332,     0,    52,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   148,     0,
     151,     0,     0,   115,     0,     0,   116,     0,     0,     0,
       3,     0,   434,   436,   107,   109,   430,   213,   214,   218,
       0,     0,     0,   387,     0,     0,   447,   448,   385,   384,
     383,   446,   477,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,   328,   329,   330,   331,
     325,     0,   413,     0,     0,   326,     0,     0,     0,     0,
       0,    42,   418,     0,   417,   465,   409,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,    29,     0,    31,     0,   477,     0,     0,    41,
     440,     0,     6,     0,    10,     0,    11,     0,     0,     0,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,    46,     0,
       0,   129,     0,   141,     0,     0,   160,     0,     0,     0,
       3,   167,    49,     0,     0,     0,    51,     0,     0,    55,
       0,     0,     0,     0,   163,     0,     0,     0,     0,     0,
     144,     0,   147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,   433,   435,     0,     0,   221,     0,
       0,     0,   256,     3,   478,     0,     0,     0,     0,   470,
     291,   446,   451,   452,   466,     0,     0,     0,     0,     0,
       0,   290,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
     338,     0,   340,   342,   343,   346,   347,   344,   345,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   358,   360,
     361,   363,   365,   366,   368,   370,   371,   373,   374,   379,
     380,   381,   376,   377,   378,   389,   393,   405,   398,   399,
     400,   401,   404,   402,   403,   396,   397,   394,   395,   429,
       0,   426,    62,     0,    96,    97,    98,    99,   100,    59,
       0,     0,   412,     0,    15,     0,     0,     0,     0,   456,
     461,   462,    64,     0,   421,     0,     0,     0,   410,     0,
     408,   440,     0,     0,     0,   327,   328,   329,   330,   331,
     325,   326,   475,     0,    30,    32,    40,     0,   439,     0,
       0,     0,   443,   465,     0,     0,     0,     0,    26,     0,
      27,     0,    28,     0,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   478,     0,     0,   132,   478,     0,     0,   365,   366,
     268,   271,     0,   284,   286,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,   407,     0,     0,
       0,     0,     0,   478,     0,   478,     0,     0,     0,     0,
     117,   118,   119,   120,   324,   432,   108,     0,     0,   221,
       0,     0,     0,     0,     0,     0,     0,   479,     0,     0,
     392,   471,     0,     0,     0,     0,     0,     0,     0,     0,
     317,     0,     0,     0,   318,     0,     0,   319,     0,     0,
       0,     0,     0,     0,     0,     0,   292,    66,    68,    70,
      73,    71,    72,    67,    69,    79,    81,    83,    86,    84,
      85,    80,    82,     0,     0,   422,     0,     0,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     420,     0,   416,     0,     0,   260,     0,     0,     0,     0,
     456,   461,   462,   476,    39,     5,     0,    12,     0,     0,
       0,     0,    19,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    90,    89,    93,    91,
      92,    88,    44,     0,    94,    95,    47,   126,   131,   130,
       0,     0,   140,   157,     0,   159,     0,     0,     0,     0,
     273,     0,     0,   166,     0,     0,     0,     0,     0,   178,
       0,    50,    53,    54,    57,    58,    34,    33,     0,     0,
     161,   463,   464,     0,     0,   142,   145,   146,   149,   150,
       0,     0,   219,   220,     0,     0,     0,     0,   216,   175,
       0,     0,     0,     0,     0,     0,     0,   255,   217,   480,
       0,   472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,   308,   315,   316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   336,   428,
       0,   425,     0,     0,    61,     0,    63,   176,     0,    16,
     459,   458,   457,     0,     0,     0,   419,   414,   415,     0,
       0,     0,   411,     0,     0,   265,   264,     0,     0,     0,
      21,     0,    22,     0,    25,   169,     0,    36,    35,     0,
     480,   480,     0,   133,     0,   134,     0,     0,     0,   269,
     280,     0,   274,   275,   165,   276,   272,     0,   285,   287,
     289,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,     0,     0,    77,    78,     0,     0,
       0,   215,     0,     0,     0,     0,     0,     0,     0,     0,
     174,   172,     0,     0,     0,     0,     0,     9,     0,     0,
     456,   462,   312,     0,     0,     0,     0,     0,     0,   320,
     321,   322,   323,   293,   295,   297,   300,   298,   299,   294,
     296,   427,   424,   423,    60,   480,    74,    75,    76,   261,
     262,     0,     0,    13,    20,     0,     0,     0,     0,   168,
       7,     0,    45,    48,   128,   127,   478,     0,   138,   158,
     155,   156,   282,     0,     0,   277,     0,     0,     0,     0,
       0,   182,   183,   180,     0,     0,     0,     0,     0,     0,
       0,     0,   164,   162,   143,     0,     0,   154,     0,     3,
     251,     0,   248,     0,     0,     0,     0,   227,     0,     0,
       0,     0,   222,     0,     0,     0,     0,   223,   224,     0,
     171,   173,   257,   258,   259,     0,   177,     0,     0,     0,
     302,   304,   307,   305,   306,   303,    17,     0,   480,   267,
     266,     0,    23,     0,   135,   136,   270,   281,     0,   278,
     279,     0,     0,     0,   184,   186,   188,   191,   189,   190,
     185,   187,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,     0,   239,     0,     0,     0,
       0,     0,   229,     0,     0,     0,     0,   228,   225,   226,
       0,   254,   311,   310,   176,     0,     0,   263,    14,   480,
       8,     0,   139,   283,   192,   194,   193,     0,   153,   250,
       0,   196,     0,     0,     0,   195,     0,   247,     0,   236,
     237,   231,   234,   235,   230,     0,     0,   240,     0,     0,
       0,   241,     0,     0,     0,   309,   313,   314,    24,   137,
       0,   249,     0,     0,   199,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   246,   244,   245,   233,   242,
     243,   232,   253,     0,     0,     0,     0,     0,   200,   201,
     198,     0,     0,     0,     0,     0,     0,     0,     0,   252,
     152,     0,     0,     0,   202,   204,   206,   209,   207,   208,
     203,   205,   210,   212,   211
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -883,  -883,     1,  -883,  -564,  -732,  -883,  -883,  -883,  -883,
    -883,   284,  1390,  -883,  -883,  -883,  -883,  -326,  -882,   429,
    -883,  -883,  -883,  -883,   803,  -883,  -883,  -508,   877,  -383,
     357,  1136,    -2,  -236,  -883,  1188,  1184,   668,  1171,  -883,
     632,   -16,  1168,   928,   930,    -3,  -883,  1290,  -264,  -883,
    -883,    87,  -883
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   218,    69,    70,   574,    71,    72,    73,    74,
      75,  1006,    76,    77,   598,    78,    79,   361,  1010,   199,
      80,    81,    82,   397,   620,    83,   625,   298,   545,   587,
     129,   188,   139,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   214,   489,   100,
     101,   140,   102
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      85,     2,   697,   441,   554,   873,   158,   693,   738,   491,
      25,   157,   130,   699,   134,    27,   622,   746,   119,   400,
     512,   149,   151,   153,   155,   156,   750,   932,   162,   164,
     166,  1012,   617,   170,   171,   580,   581,   175,   177,   582,
     580,   581,   618,   747,   582,   192,   193,   500,   748,   196,
     648,     3,   203,   649,   650,   201,   260,   114,  1013,   208,
     209,   210,   501,   651,   213,   216,   652,   653,  1014,   983,
     654,   655,   116,   401,   202,   115,   580,   581,   516,   858,
     582,   118,   260,   785,   786,   768,   769,   787,   859,  1018,
     117,   642,   751,   752,   258,   259,   583,   584,   585,  1019,
     645,   583,   584,   585,   275,   276,   277,   278,   279,   280,
     292,   643,   285,   287,   293,   289,   290,   167,  1015,   169,
     646,   168,    68,    68,   785,   786,   213,   120,   787,   121,
     698,   172,    68,   313,   315,   260,   317,   583,   584,   585,
     120,   301,   121,   978,   749,   985,   335,  1101,   345,  1020,
     350,   181,   352,   141,   142,   143,   236,   144,   619,   302,
     294,   586,   788,   656,  1016,   984,   621,   120,   832,   121,
     720,   182,   379,   184,   381,   173,   145,   386,  1017,   387,
     388,   146,   389,   390,   770,   391,   393,   394,   391,   395,
     580,   581,  1119,   185,   582,  1021,  1127,   183,   357,   358,
     283,   321,   719,   911,   644,   354,   186,   319,   299,  1022,
     320,   986,   987,   647,   364,   322,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   318,   431,   432,   433,   434,
     435,   436,   437,   438,   365,   439,   217,  1088,  1151,   260,
     792,   793,   462,   463,   255,   256,  1165,  1089,   794,   795,
     796,   583,   584,   585,   364,   355,   928,   183,    68,   309,
     753,  1172,   469,   470,   471,   472,   473,   474,   475,   580,
     581,   929,    68,   582,   365,   179,   744,   300,   147,   186,
      68,   375,   503,   135,   136,   580,   581,   104,   105,   582,
     364,  1189,   580,   581,   377,   789,   582,  1090,   522,   376,
     524,   525,   526,   527,   528,   529,   530,   531,   974,   797,
     365,   362,   378,   346,   363,  1093,   791,   113,   538,   539,
     366,   541,  1112,   546,   187,  1094,   320,   815,   710,    68,
     583,   584,   585,   347,   559,   560,   561,   562,   563,   564,
     565,   578,   579,  1091,   568,   569,   583,   584,   585,   362,
     323,   610,   363,   583,   584,   585,   814,  1092,   939,    84,
     366,   588,   590,   591,   324,   592,   593,   806,   594,   595,
     337,   807,   346,   383,   600,  1095,   602,   120,   206,   121,
     808,   606,   607,   809,   810,   159,   161,   811,   346,   207,
     615,   384,   347,   865,   114,  1074,   366,   533,   178,   180,
     346,   197,   198,   993,   626,   364,   194,   195,   347,   367,
     325,  1078,   634,   637,   638,   639,   641,   237,  1143,   348,
     347,  1096,   349,   240,   326,   365,   813,   116,   884,   368,
     547,   566,   549,   551,   611,  1097,   570,   828,   359,   572,
     573,   940,   577,   360,   548,   635,   550,   552,   504,   914,
     589,   505,   915,   916,   895,   917,   918,   896,   897,   310,
     597,   311,   580,   581,   920,   921,   582,   898,   885,   886,
     899,   900,   887,   343,   901,   902,   344,   408,   348,   409,
     812,   349,   200,  1023,   159,   992,  1024,   204,   205,  1025,
    1026,   272,   628,   612,   348,   632,   613,   349,   679,   271,
     941,  1179,   553,   942,   684,   685,   348,   360,   257,   349,
     351,   366,   353,   356,   695,   369,   273,  1008,  1180,   538,
     665,   913,  1009,   583,   584,   585,   666,   943,   667,   888,
     889,   890,   380,   944,   382,   945,   668,   669,   706,   670,
     969,   671,   213,   946,   947,   715,   948,   717,   949,   718,
     672,   725,   988,   721,   722,   337,   989,   950,  1040,   727,
     236,   265,   266,   267,   274,   339,  1041,   903,   340,   341,
     331,   741,   990,   281,   332,    84,   288,   268,   269,   270,
     678,   241,   242,   243,   244,  1011,   760,   728,   681,   291,
    1009,   729,   683,   333,  1042,   686,  1030,  1031,  1043,   300,
     691,   730,   694,   330,   731,   665,  1175,   781,  1044,   337,
    1176,  1045,  1064,   666,   667,   245,   246,   238,   239,   339,
    1065,  1066,   340,   341,    85,  1075,  1177,   800,  1079,   668,
     669,   670,   803,   804,   805,   671,  1067,  1068,  1069,   370,
     816,   817,  1070,   818,   819,   724,   820,   821,   822,   823,
     824,   825,   826,   827,   733,  1029,   371,   672,   343,  1156,
     372,   344,  1157,  1158,  1071,   991,   247,   248,   249,   250,
     251,   252,  1159,   374,   836,  1160,  1161,   725,   253,  1162,
    1163,  1102,   727,   373,  1114,   843,   844,   845,   731,  1115,
     254,   956,   958,   336,   849,  1116,   337,   338,  1146,  1147,
    1100,   327,   328,   329,   665,  1009,   339,   855,   856,   340,
     341,  1194,   343,   342,   861,   344,   508,  1144,   509,  1178,
     103,   869,   104,   105,   106,   107,   108,   109,   110,   104,
     105,   303,   304,   305,   306,   307,   666,   111,   874,   667,
     396,   668,   669,  1195,   308,   399,  1196,   880,  1197,  1198,
     112,   398,   113,   616,   891,   892,   894,   670,   402,   113,
     671,   830,   403,   833,  1199,   236,   904,  1200,   410,   838,
     707,   219,  1164,   708,   220,   221,   672,   228,   909,   725,
     229,   230,   727,  1201,   222,   494,  1202,   223,   224,  1203,
     231,   225,   226,   232,   233,   854,   731,   234,   235,   343,
     261,   262,   344,  1204,   933,   934,   935,   936,   937,   938,
     104,   105,   303,   304,   305,   306,   307,   263,   264,     4,
       5,     6,     7,   404,   405,   308,   131,   132,   124,   125,
     295,   495,    13,    14,    15,    16,    17,    18,    19,   510,
     113,  -391,  -391,   712,   732,   496,   713,   320,   960,   127,
      29,   736,   761,   497,   320,   320,    35,   839,   862,   498,
     320,   863,    37,   963,   970,  1052,   320,   971,   320,   499,
      45,    46,    47,    48,   511,  1110,    51,   227,   320,   982,
     455,   456,   457,   236,   512,    55,    56,   459,   460,   461,
     508,    57,   700,   994,   995,   996,   997,   998,   999,  1000,
    1001,   320,   320,   703,   801,   320,   513,   842,  1007,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,    60,    61,   508,   514,   930,  1035,   515,   737,   739,
     406,   407,   742,   743,   745,   492,   493,    62,   502,   320,
     962,   506,   507,    63,    64,    84,   518,   519,   520,   521,
     968,   128,   296,   517,    67,   523,    68,   534,   297,  1047,
     775,   776,   777,  1049,  1050,   537,   320,   103,   535,   104,
     105,   106,   107,   108,   109,   110,   557,   407,   404,   558,
     476,   536,  1057,    84,   111,   555,  1061,  1062,  1063,   556,
     477,   576,   478,   479,   404,   567,   596,   633,   599,   113,
     601,   480,   481,  1072,   571,   405,   482,   575,   405,   609,
    1077,   608,   483,   484,  1083,   614,  1037,   603,   405,  1087,
     485,   486,   487,   488,   605,   405,   404,   682,   604,   476,
     912,   754,   755,   490,   790,   755,  1105,  1106,   610,   477,
     611,   478,   479,   799,   320,   835,   320,   837,   320,   612,
     480,   481,   846,   320,  1051,   482,  1113,   623,  1053,   847,
     519,   483,   484,   629,   476,   848,   320,   852,   519,   485,
     486,   487,   488,   624,   477,  1126,   478,   479,   404,   853,
     857,   713,   866,   755,   613,   480,   481,   870,   320,  1138,
     482,   871,   320,   630,  1142,   636,   483,   484,   875,   872,
     573,   922,   755,   657,   485,   486,   487,   488,   923,   755,
     951,   320,   952,   519,   673,  1007,   953,   320,   955,   320,
    1152,  1153,  1155,   404,   967,   254,   905,   976,   405,   977,
     573,  1048,   320,   260,     4,     5,     6,     7,  1104,   320,
     658,   131,   132,   124,   125,   295,   659,    13,    14,    15,
      16,    17,    18,    19,  1181,  1182,  1183,  1184,  1185,  1186,
    1187,  1188,  1109,   320,   127,    29,  1111,   573,  1120,   755,
     660,    35,   661,  1191,  1192,  1193,   674,    37,  1128,   755,
    1173,   755,   189,   191,   675,    45,    46,    47,    48,   465,
     466,    51,   662,   467,   468,   663,   664,   665,   666,   667,
      55,    56,   668,   669,   687,   670,    57,   671,   672,   688,
     676,   689,   696,   701,   704,   705,   709,   702,   723,   714,
     725,   726,   727,   772,   771,   773,   728,   729,   730,   975,
     731,   734,   320,   979,   980,   981,    60,    61,   735,   360,
     762,   763,   764,   765,   766,   767,   774,   778,   779,   780,
     782,   783,    62,   802,   834,   836,   840,   841,    63,    64,
     850,  1002,   851,   876,   860,  1004,   128,   692,   864,    67,
     877,    68,   867,   297,     4,     5,     6,     7,   868,   878,
     879,   131,   132,   124,   125,   295,   881,    13,    14,    15,
      16,    17,    18,    19,   882,   883,   906,   907,   910,   908,
     919,   924,   925,   926,   127,    29,   927,   954,   931,   956,
     959,    35,   957,   958,   965,   961,   964,    37,   966,   972,
     973,  1005,  1003,  1027,  1028,    45,    46,    47,    48,  1032,
    1033,    51,  1034,  1054,  1055,  1038,  1039,  1046,  1056,  1058,
      55,    56,  1059,  1060,  1073,  1076,    57,  1082,  1080,  1081,
    1084,     4,     5,     6,     7,  1085,  1086,  1098,   131,   132,
     124,   125,   295,  1099,    13,    14,    15,    16,    17,    18,
      19,  1107,  1103,  1009,  1108,  1117,    60,    61,  1129,  1130,
    1131,   127,    29,  1121,  1132,  1133,  1134,  1135,    35,  1136,
    1137,  1139,    62,  1140,    37,  1141,  1145,  1148,    63,    64,
    1174,  1150,    45,    46,    47,    48,   128,   831,    51,    67,
    1166,    68,  1167,   297,  1168,  1169,  1170,    55,    56,   160,
    1171,   711,   784,    57,   442,   458,   440,     0,   464,     0,
    1118,     0,     0,     0,    84,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1149,    62,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,   128,     0,     0,    67,     0,    68,     0,
     297,     4,     5,     6,     7,     0,     0,     0,   411,   412,
     413,   414,    12,     0,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,   415,   416,    25,     0,     0,    26,
      27,    28,    29,   417,   418,    32,    33,    34,    35,    36,
       0,  1190,     0,     0,    37,    38,    39,    40,    41,    42,
     419,   420,    45,    46,    47,    48,    49,    50,    51,    52,
       0,     0,     0,    53,    54,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,    58,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,    65,    66,     0,     0,    67,     0,    68,   421,
       4,     5,     6,     7,     0,     0,     0,     8,     9,    10,
      11,    12,     0,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   756,   757,    25,     0,     0,    26,    27,
      28,    29,    30,   758,    32,    33,    34,    35,    36,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,     0,
       0,     0,    53,    54,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,    58,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,    63,    64,     0,     0,     0,     0,
       0,    65,    66,     0,     0,    67,     0,    68,   759,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,     0,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,     0,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,     0,     0,
       0,    53,    54,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,    58,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
      65,    66,     0,     0,    67,     0,    68,   798,     4,     5,
       6,     7,     0,     0,     0,     8,     9,    10,    11,    12,
       0,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,  1122,  1123,    25,     0,     0,    26,    27,    28,    29,
      30,  1124,    32,    33,    34,    35,    36,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     0,     0,     0,
      53,    54,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,    58,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
      60,    61,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,    62,     0,     0,     0,
       0,     0,    63,    64,     0,   127,    29,     0,     0,    65,
      66,     0,    35,    67,     0,    68,  1125,     0,    37,     0,
       0,     0,     0,     0,     0,     0,    45,    46,    47,    48,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    62,    37,     0,     0,     0,     0,    63,
      64,     0,    45,    46,    47,    48,   163,   128,    51,     0,
      67,     0,    68,   640,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    62,
      37,     0,     0,     0,     0,    63,    64,     0,    45,    46,
      47,    48,   163,   128,    51,     0,    67,     0,    68,   893,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,   163,   128,
       0,     0,    67,     0,    68,  1154,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,     0,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,     0,     0,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,     0,     0,    53,    54,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,    58,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,    60,    61,
     131,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,    62,     0,     0,     0,     0,     0,
      63,    64,     0,   127,    29,     0,     0,    65,    66,     0,
      35,    67,     0,    68,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,    45,    46,    47,    48,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    62,    37,     0,     0,     0,     0,    63,    64,     0,
      45,    46,    47,    48,     0,   128,    51,     0,    67,   133,
      68,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    62,    37,     0,
       0,     0,     0,    63,    64,     0,    45,    46,    47,    48,
     137,   138,    51,     0,    67,     0,    68,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    62,    37,     0,     0,     0,     0,    63,
      64,     0,    45,    46,    47,    48,   163,   128,    51,     0,
      67,     0,    68,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   211,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    62,
      37,     0,     0,     0,     0,    63,    64,     0,    45,    46,
      47,    48,   165,   128,    51,     0,    67,     0,    68,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    62,     0,    37,     0,     0,
       0,    63,    64,     0,     0,    45,    46,    47,    48,   128,
     212,    51,    67,     0,    68,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   128,    51,     0,    67,
     215,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    62,     0,
      37,     0,     0,     0,    63,    64,     0,     0,    45,    46,
      47,    48,   128,   282,    51,    67,     0,    68,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     131,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    62,    37,     0,     0,     0,
       0,    63,    64,   286,    45,    46,    47,    48,     0,   128,
      51,     0,    67,     0,    68,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    62,     0,    37,     0,     0,     0,    63,    64,     0,
       0,    45,    46,    47,    48,   128,   212,    51,    67,     0,
      68,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,   312,   128,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,   314,   128,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,   385,     0,     0,    60,    61,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
      62,     0,    37,     0,     0,     0,    63,    64,     0,     0,
      45,    46,    47,    48,   128,   316,    51,    67,     0,    68,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    62,    37,     0,
       0,     0,     0,    63,    64,     0,    45,    46,    47,    48,
       0,   128,    51,     0,    67,     0,    68,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    62,    37,     0,     0,     0,     0,    63,
      64,     0,    45,    46,    47,    48,     0,   128,    51,   392,
      67,     0,    68,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,   217,     4,     5,     6,
       7,     0,     0,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    62,
      37,     0,     0,     0,     0,    63,    64,     0,    45,    46,
      47,    48,     0,   128,    51,     0,    67,     0,    68,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     131,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    62,    37,     0,     0,     0,
       0,    63,    64,     0,    45,    46,    47,    48,     0,   128,
      51,     0,    67,   532,    68,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    62,     0,    37,     0,     0,     0,    63,    64,     0,
       0,    45,    46,    47,    48,   128,   627,    51,    67,     0,
      68,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   128,    51,     0,    67,   631,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     131,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,    62,     0,    37,     0,     0,     0,
      63,    64,     0,     0,    45,    46,    47,    48,   128,   677,
      51,    67,     0,    68,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    62,     0,    37,     0,     0,     0,    63,    64,     0,
       0,    45,    46,    47,    48,   128,   680,    51,    67,     0,
      68,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     4,     5,   542,
       7,     0,     0,     0,   131,   132,   124,   125,   543,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    62,     0,
      37,     0,     0,     0,    63,    64,     0,     0,    45,    46,
      47,    48,   128,   690,    51,    67,     0,    68,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    62,     0,    37,     0,     0,
       0,    63,    64,     0,     0,    45,    46,    47,    48,   128,
     212,    51,    67,     0,    68,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    62,     0,    37,     0,     0,     0,    63,    64,
       0,     0,    45,    46,    47,    48,   128,   829,    51,    67,
       0,    68,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    62,
       0,    37,     0,     0,     0,    63,    64,     0,     0,    45,
      46,    47,    48,   128,  1036,    51,    67,     0,    68,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     148,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   150,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   152,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,     0,   154,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   128,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     174,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   176,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   128,    51,     0,    67,     0,   190,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,     0,   284,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
     542,     7,     0,     0,     0,   131,   132,   124,   125,   543,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   540,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     544,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   716,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   122,   123,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,   127,     0,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   740,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -481,  -481,     0,     0,    60,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
      63,    64,     0,     0,     0,     0,     0,     0,   128,     0,
       0,    67,     0,    68,  -481,  -481,  -481,     0,  -481,  -481,
    -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,
    -481,     0,     0,     0,  -481,     0,     0,     0,     0,     0,
       0,     0,  -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,
       0,     0,  -481,  -481,  -481,  -481,  -481,     0,     0,  -481,
    -481,  -481,     0,  -481
};

static const yytype_int16 yycheck[] =
{
       2,     0,    14,   239,   330,   737,    14,   515,   572,   273,
      28,    27,    15,   521,    16,    32,   399,    87,    11,    14,
     134,    23,    24,    25,    26,    27,    10,   141,    30,    31,
      32,   913,    14,    35,    36,    10,    11,    39,    40,    14,
      10,    11,    24,    87,    14,    47,    48,   120,    24,    51,
      28,     0,    55,    31,    32,    14,   126,   120,    14,    62,
      63,    64,   135,    41,    66,    67,    44,    45,    24,    24,
      48,    49,   122,    68,    33,   138,    10,    11,    84,     5,
      14,   120,   126,    62,    63,    42,    43,    66,    14,    14,
     140,    28,    76,    77,    85,    86,    71,    72,    73,    24,
      28,    71,    72,    73,   106,   107,   108,   109,   110,   111,
      14,    48,   114,   115,    18,   117,   118,    10,    74,    14,
      48,    14,   140,   140,    62,    63,   128,   133,    66,   135,
     142,    12,   140,   135,   136,   126,   138,    71,    72,    73,
     133,   122,   135,   875,   120,    10,   148,  1029,   150,    74,
     152,    28,   154,    10,    11,    12,   134,    14,   140,   140,
      64,   136,   141,   141,   120,   120,   136,   133,   676,   135,
     553,    48,   174,    28,   176,    12,    33,   179,   134,   181,
     182,    38,   184,   185,   141,   187,   188,   189,   190,   191,
      10,    11,  1074,    48,    14,   120,  1078,   134,    85,    86,
     113,   120,   136,   141,   141,    87,   134,   134,   121,   134,
     137,    76,    77,   141,    28,   134,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   138,   228,   229,   230,   231,
     232,   233,   234,   235,    48,   237,    78,    14,  1120,   126,
     623,   624,   258,   259,   124,   125,  1128,    24,    80,    81,
      82,    71,    72,    73,    28,   137,   120,   134,   140,   120,
     586,  1143,   265,   266,   267,   268,   269,   270,   271,    10,
      11,   135,   140,    14,    48,   135,    87,   138,   135,   134,
     140,   122,   284,    14,    15,    10,    11,   105,   106,    14,
      28,  1173,    10,    11,   122,   621,    14,    74,   300,   140,
     302,   303,   304,   305,   306,   307,   308,   309,   872,   141,
      48,   134,   140,    28,   137,    14,   136,   135,   320,   321,
     134,   323,  1054,   325,   140,    24,   137,   141,    84,   140,
      71,    72,    73,    48,   336,   337,   338,   339,   340,   341,
     342,   357,   358,   120,   346,   347,    71,    72,    73,   134,
     120,   134,   137,    71,    72,    73,   141,   134,   141,     2,
     134,   360,   364,   365,   134,   367,   368,    28,   370,   371,
      31,    32,    28,    31,   376,    74,   378,   133,    10,   135,
      41,   383,   384,    44,    45,    28,    29,    48,    28,    10,
     392,    49,    48,   719,   120,   136,   134,   310,    41,    42,
      28,    14,    15,   141,   403,    28,    49,    50,    48,    28,
     120,   136,   138,   415,   416,   417,   418,    84,   136,   134,
      48,   120,   137,    98,   134,    48,   141,   122,   754,    48,
     120,   344,   120,   120,   134,   134,   349,   673,   135,    29,
      30,   141,   355,   140,   134,   140,   134,   134,   134,    10,
     363,   137,    13,    14,    28,    16,    17,    31,    32,   137,
     373,   139,    10,    11,   790,   791,    14,    41,    10,    11,
      44,    45,    14,   134,    48,    49,   137,   137,   134,   139,
     141,   137,    53,    10,   127,   141,    13,    58,    59,    16,
      17,   133,   405,   134,   134,   408,   134,   137,   500,   119,
     141,   141,   135,   141,   506,   507,   134,   140,   112,   137,
     153,   134,   155,   156,   516,   134,   133,   135,   141,   521,
     134,   785,   140,    71,    72,    73,   134,   141,   134,    71,
      72,    73,   175,   141,   177,   141,   134,   134,   540,   134,
     866,   134,   544,   141,   141,   547,   141,   549,   141,   551,
     134,   134,    28,   555,   556,    31,    32,   141,   141,   134,
     134,   113,   114,   115,    14,    41,   141,   141,    44,    45,
      10,   573,    48,    14,    14,   218,    14,   129,   130,   131,
     493,    88,    89,    90,    91,   135,   588,   134,   501,   134,
     140,   134,   505,    10,   141,   508,   922,   923,   141,   138,
     513,   134,   515,    14,   134,   134,    28,   609,   141,    31,
      32,   141,   141,   134,   134,   122,   123,    99,   100,    41,
     141,   141,    44,    45,   626,  1008,    48,   629,  1011,   134,
     134,   134,   634,   635,   636,   134,   141,   141,   141,   120,
     642,   643,   141,   645,   646,   558,   648,   649,   650,   651,
     652,   653,   654,   655,   567,   919,   120,   134,   134,    28,
      75,   137,    31,    32,   141,   141,    92,    93,    94,    95,
      96,    97,    41,   140,   134,    44,    45,   134,   104,    48,
      49,   141,   134,   137,   141,   687,   688,   689,   134,   141,
     116,   134,   134,    28,   696,   141,    31,    32,   141,   141,
     135,    10,    11,    12,   134,   140,    41,   709,   710,    44,
      45,   141,   134,    48,   716,   137,   137,  1100,   139,   141,
     103,   723,   105,   106,   107,   108,   109,   110,   111,   105,
     106,   107,   108,   109,   110,   111,   134,   120,   740,   134,
      59,   134,   134,   141,   120,   135,   141,   749,   141,   141,
     133,   140,   135,   396,   756,   757,   758,   134,   140,   135,
     134,   674,   140,   676,   141,   134,   768,   141,   141,   682,
     134,    28,   141,   137,    31,    32,   134,    28,   780,   134,
      31,    32,   134,   141,    41,   134,   141,    44,    45,   141,
      41,    48,    49,    44,    45,   708,   134,    48,    49,   134,
     117,   118,   137,   141,   806,   807,   808,   809,   810,   811,
     105,   106,   107,   108,   109,   110,   111,   127,   128,     3,
       4,     5,     6,   136,   137,   120,    10,    11,    12,    13,
      14,   134,    16,    17,    18,    19,    20,    21,    22,   123,
     135,   136,   137,   134,   134,   134,   137,   137,   850,    33,
      34,   134,   134,   134,   137,   137,    40,   134,   134,   134,
     137,   137,    46,   134,   134,   134,   137,   137,   137,   134,
      54,    55,    56,    57,   141,   134,    60,   134,   137,   881,
       7,     8,     9,   134,   134,    69,    70,   255,   256,   257,
     137,    75,   139,   895,   896,   897,   898,   899,   900,   901,
     902,   137,   137,   139,   139,   137,   135,   139,   910,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   105,   106,   137,    14,   139,   928,   135,   571,   572,
     136,   137,   575,   576,   577,   134,   135,   121,   136,   137,
     853,   127,   128,   127,   128,   588,   136,   137,   136,   137,
     863,   135,   136,    14,   138,    14,   140,   134,   142,   961,
     603,   604,   605,   965,   966,   136,   137,   103,   134,   105,
     106,   107,   108,   109,   110,   111,   136,   137,   136,   137,
      14,   134,   984,   626,   120,   120,   988,   989,   990,   120,
      24,    10,    26,    27,   136,   137,   134,   133,    14,   135,
      14,    35,    36,  1005,   136,   137,    40,   136,   137,   134,
    1009,    10,    46,    47,  1016,   141,   929,   136,   137,  1021,
      54,    55,    56,    57,   136,   137,   136,   137,    29,    14,
      64,   136,   137,    18,   136,   137,  1038,  1039,   134,    24,
     134,    26,    27,   136,   137,   136,   137,   136,   137,   134,
      35,    36,   136,   137,   967,    40,  1058,   135,   971,   136,
     137,    46,    47,   120,    14,   136,   137,   136,   137,    54,
      55,    56,    57,   135,    24,  1077,    26,    27,   136,   137,
     136,   137,   136,   137,   134,    35,    36,   136,   137,  1091,
      40,   136,   137,    10,  1096,   120,    46,    47,   741,    29,
      30,   136,   137,   134,    54,    55,    56,    57,   136,   137,
     136,   137,   136,   137,   101,  1117,   136,   137,   136,   137,
    1122,  1123,  1124,   136,   137,   116,   769,   136,   137,    29,
      30,   136,   137,   126,     3,     4,     5,     6,   136,   137,
     134,    10,    11,    12,    13,    14,   134,    16,    17,    18,
      19,    20,    21,    22,  1156,  1157,  1158,  1159,  1160,  1161,
    1162,  1163,   136,   137,    33,    34,    29,    30,   136,   137,
     134,    40,   134,  1175,  1176,  1177,   135,    46,   136,   137,
     136,   137,    46,    47,    14,    54,    55,    56,    57,   261,
     262,    60,   134,   263,   264,   134,   134,   134,   134,   134,
      69,    70,   134,   134,   120,   134,    75,   134,   134,   120,
     135,   120,   135,   123,   134,   134,    84,   141,   120,   134,
     134,   134,   134,   141,   123,   123,   134,   134,   134,   872,
     134,   134,   137,   876,   877,   878,   105,   106,   134,   140,
     134,   134,   134,   134,   134,   134,   141,   134,   134,   120,
      14,    14,   121,    14,   134,   134,   139,   139,   127,   128,
      84,   904,    14,    10,   134,   908,   135,   136,   134,   138,
      10,   140,   134,   142,     3,     4,     5,     6,   134,    10,
      14,    10,    11,    12,    13,    14,   120,    16,    17,    18,
      19,    20,    21,    22,    10,    10,   134,   134,   134,    29,
      62,    14,    14,    14,    33,    34,   134,   134,   141,   134,
     136,    40,   134,   134,    84,   135,   134,    46,    84,   134,
     134,   134,   141,   134,   134,    54,    55,    56,    57,     7,
       7,    60,     7,   976,   977,   120,   120,   134,    14,   120,
      69,    70,    10,    10,   136,    78,    75,   134,    14,    14,
      14,     3,     4,     5,     6,    14,   134,   134,    10,    11,
      12,    13,    14,   134,    16,    17,    18,    19,    20,    21,
      22,   136,   141,   140,   134,   134,   105,   106,   134,   134,
     134,    33,    34,   141,   134,   134,   134,    14,    40,    14,
     134,    14,   121,    14,    46,   134,   141,   134,   127,   128,
     136,  1117,    54,    55,    56,    57,   135,   136,    60,   138,
     134,   140,   134,   142,   134,   134,   134,    69,    70,    29,
     134,   544,   619,    75,   240,   254,   238,    -1,   260,    -1,
    1073,    -1,    -1,    -1,  1077,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1111,   121,
      -1,    -1,    -1,    -1,    -1,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,   135,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,  1174,    -1,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    -1,    -1,    65,    66,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,   127,   128,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,    -1,   138,    -1,   140,   141,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      -1,    -1,    65,    66,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,   127,   128,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,    -1,   138,    -1,   140,   141,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    -1,
      -1,    65,    66,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,    -1,   138,    -1,   140,   141,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
     105,   106,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,   121,    -1,    -1,    -1,
      -1,    -1,   127,   128,    -1,    33,    34,    -1,    -1,   134,
     135,    -1,    40,   138,    -1,   140,   141,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   121,    46,    -1,    -1,    -1,    -1,   127,
     128,    -1,    54,    55,    56,    57,   134,   135,    60,    -1,
     138,    -1,   140,   141,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,
      46,    -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,
      56,    57,   134,   135,    60,    -1,   138,    -1,   140,   141,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,   127,   128,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,    -1,   138,    -1,   140,   141,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    65,    66,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,   105,   106,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,   121,    -1,    -1,    -1,    -1,    -1,
     127,   128,    -1,    33,    34,    -1,    -1,   134,   135,    -1,
      40,   138,    -1,   140,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   121,    46,    -1,    -1,    -1,    -1,   127,   128,    -1,
      54,    55,    56,    57,    -1,   135,    60,    -1,   138,   139,
     140,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   121,    46,    -1,
      -1,    -1,    -1,   127,   128,    -1,    54,    55,    56,    57,
     134,   135,    60,    -1,   138,    -1,   140,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   121,    46,    -1,    -1,    -1,    -1,   127,
     128,    -1,    54,    55,    56,    57,   134,   135,    60,    -1,
     138,    -1,   140,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,
      46,    -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,
      56,    57,   134,   135,    60,    -1,   138,    -1,   140,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   121,    -1,    46,    -1,    -1,
      -1,   127,   128,    -1,    -1,    54,    55,    56,    57,   135,
     136,    60,   138,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   121,    46,    -1,    -1,    -1,    -1,   127,   128,
      -1,    54,    55,    56,    57,    -1,   135,    60,    -1,   138,
     139,   140,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   121,    -1,
      46,    -1,    -1,    -1,   127,   128,    -1,    -1,    54,    55,
      56,    57,   135,   136,    60,   138,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   121,    46,    -1,    -1,    -1,
      -1,   127,   128,   129,    54,    55,    56,    57,    -1,   135,
      60,    -1,   138,    -1,   140,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   121,    -1,    46,    -1,    -1,    -1,   127,   128,    -1,
      -1,    54,    55,    56,    57,   135,   136,    60,   138,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,    46,
      -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,    56,
      57,   134,   135,    60,    -1,   138,    -1,   140,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   121,    46,    -1,    -1,    -1,    -1,
     127,   128,    -1,    54,    55,    56,    57,   134,   135,    60,
      -1,   138,    -1,   140,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    -1,    -1,   105,   106,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
     121,    -1,    46,    -1,    -1,    -1,   127,   128,    -1,    -1,
      54,    55,    56,    57,   135,   136,    60,   138,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   121,    46,    -1,
      -1,    -1,    -1,   127,   128,    -1,    54,    55,    56,    57,
      -1,   135,    60,    -1,   138,    -1,   140,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   121,    46,    -1,    -1,    -1,    -1,   127,
     128,    -1,    54,    55,    56,    57,    -1,   135,    60,   137,
     138,    -1,   140,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    78,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,
      46,    -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,
      56,    57,    -1,   135,    60,    -1,   138,    -1,   140,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   121,    46,    -1,    -1,    -1,
      -1,   127,   128,    -1,    54,    55,    56,    57,    -1,   135,
      60,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   121,    -1,    46,    -1,    -1,    -1,   127,   128,    -1,
      -1,    54,    55,    56,    57,   135,   136,    60,   138,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,    46,
      -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,    56,
      57,    -1,   135,    60,    -1,   138,   139,   140,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,   121,    -1,    46,    -1,    -1,    -1,
     127,   128,    -1,    -1,    54,    55,    56,    57,   135,   136,
      60,   138,    -1,   140,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   121,    -1,    46,    -1,    -1,    -1,   127,   128,    -1,
      -1,    54,    55,    56,    57,   135,   136,    60,   138,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   121,    -1,
      46,    -1,    -1,    -1,   127,   128,    -1,    -1,    54,    55,
      56,    57,   135,   136,    60,   138,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   121,    -1,    46,    -1,    -1,
      -1,   127,   128,    -1,    -1,    54,    55,    56,    57,   135,
     136,    60,   138,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,   121,    -1,    46,    -1,    -1,    -1,   127,   128,
      -1,    -1,    54,    55,    56,    57,   135,   136,    60,   138,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   121,
      -1,    46,    -1,    -1,    -1,   127,   128,    -1,    -1,    54,
      55,    56,    57,   135,   136,    60,   138,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   121,    46,    -1,    -1,
      -1,    -1,   127,   128,    -1,    54,    55,    56,    57,    -1,
     135,    60,    -1,   138,    -1,   140,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   121,    46,    -1,    -1,    -1,    -1,   127,   128,
      -1,    54,    55,    56,    57,    -1,   135,    60,    -1,   138,
      -1,   140,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,    46,
      -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,    56,
      57,    -1,   135,    60,    -1,   138,    -1,   140,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   121,    46,    -1,    -1,    -1,    -1,
     127,   128,    -1,    54,    55,    56,    57,    -1,   135,    60,
      -1,   138,    -1,   140,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     121,    46,    -1,    -1,    -1,    -1,   127,   128,    -1,    54,
      55,    56,    57,    -1,   135,    60,    -1,   138,    -1,   140,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   121,    46,    -1,    -1,
      -1,    -1,   127,   128,    -1,    54,    55,    56,    57,    -1,
     135,    60,    -1,   138,    -1,   140,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   121,    46,    -1,    -1,    -1,    -1,   127,   128,
      -1,    54,    55,    56,    57,    -1,   135,    60,    -1,   138,
      -1,   140,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,    46,
      -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,    56,
      57,    -1,   135,    60,    -1,   138,    -1,   140,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   121,    46,    -1,    -1,    -1,    -1,
     127,   128,    -1,    54,    55,    56,    57,    -1,   135,    60,
      -1,   138,    -1,   140,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     121,    46,    -1,    -1,    -1,    -1,   127,   128,    -1,    54,
      55,    56,    57,    -1,   135,    60,    -1,   138,    -1,   140,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   121,    46,    -1,    -1,
      -1,    -1,   127,   128,    -1,    54,    55,    56,    57,    -1,
     135,    60,    -1,   138,    -1,   140,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   121,    46,    -1,    -1,    -1,    -1,   127,   128,
      -1,    54,    55,    56,    57,    -1,   135,    60,    -1,   138,
      -1,   140,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   121,    46,
      -1,    -1,    -1,    -1,   127,   128,    -1,    54,    55,    56,
      57,    -1,   135,    60,    -1,   138,    -1,   140,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
      -1,   138,    -1,   140,    84,    85,    86,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,   126,    -1,    -1,   129,
     130,   131,    -1,   133
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   144,   145,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    65,    66,    69,    70,    75,    79,    83,
     105,   106,   121,   127,   128,   134,   135,   138,   140,   146,
     147,   149,   150,   151,   152,   153,   155,   156,   158,   159,
     163,   164,   165,   168,   173,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     192,   193,   195,   103,   105,   106,   107,   108,   109,   110,
     111,   120,   133,   135,   120,   138,   122,   140,   120,    11,
     133,   135,    10,    11,    12,    13,    14,    33,   135,   173,
     188,    10,    11,   139,   175,    14,    15,   134,   135,   175,
     194,    10,    11,    12,    14,    33,    38,   135,   135,   175,
     135,   175,   135,   175,   135,   175,   175,   184,    14,   173,
     155,   173,   175,   134,   175,   134,   175,    10,    14,    14,
     175,   175,    12,    12,   135,   175,   135,   175,   173,   135,
     173,    28,    48,   134,    28,    48,   134,   140,   174,   174,
     140,   174,   175,   175,   173,   173,   175,    14,    15,   162,
     162,    14,    33,   188,   162,   162,    10,    10,   188,   188,
     188,    10,   136,   175,   190,   139,   175,    78,   145,    28,
      31,    32,    41,    44,    45,    48,    49,   134,    28,    31,
      32,    41,    44,    45,    48,    49,   134,    84,    99,   100,
      98,    88,    89,    90,    91,   122,   123,    92,    93,    94,
      95,    96,    97,   104,   116,   124,   125,   112,    85,    86,
     126,   117,   118,   127,   128,   113,   114,   115,   129,   130,
     131,   119,   133,   133,    14,   175,   175,   175,   175,   175,
     175,    14,   136,   194,   135,   175,   129,   175,    14,   175,
     175,   134,    14,    18,    64,    14,   136,   142,   170,   194,
     138,   122,   140,   107,   108,   109,   110,   111,   120,   120,
     137,   139,   134,   175,   134,   175,   136,   175,   194,   134,
     137,   120,   134,   120,   134,   120,   134,    10,    11,    12,
      14,    10,    14,    10,   190,   175,    28,    31,    32,    41,
      44,    45,    48,   134,   137,   175,    28,    48,   134,   137,
     175,   173,   175,   173,    87,   137,   173,    85,    86,   135,
     140,   160,   134,   137,    28,    48,   134,    28,    48,   134,
     120,   120,    75,   137,   140,   122,   140,   122,   140,   175,
     173,   175,   173,    31,    49,    25,   175,   175,   175,   175,
     175,   175,   137,   175,   175,   175,    59,   166,   140,   135,
      14,    68,   140,   140,   136,   137,   136,   137,   137,   139,
     141,    10,    11,    12,    13,    26,    27,    35,    36,    52,
      53,   141,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     178,   176,   179,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,     7,     8,     9,   181,   183,
     183,   183,   184,   184,   185,   186,   186,   187,   187,   188,
     188,   188,   188,   188,   188,   188,    14,    24,    26,    27,
      35,    36,    40,    46,    47,    54,    55,    56,    57,   191,
      18,   191,   134,   135,   134,   134,   134,   134,   134,   134,
     120,   135,   136,   175,   134,   137,   127,   128,   137,   139,
     123,   141,   134,   135,    14,   135,    84,    14,   136,   137,
     136,   137,   175,    14,   175,   175,   175,   175,   175,   175,
     175,   175,   139,   194,   134,   134,   134,   136,   175,   175,
     135,   175,     5,    14,   135,   171,   175,   120,   134,   120,
     134,   120,   134,   135,   160,   120,   120,   136,   137,   175,
     175,   175,   175,   175,   175,   175,   194,   137,   175,   175,
     194,   136,    29,    30,   148,   136,    10,   194,   184,   184,
      10,    11,    14,    71,    72,    73,   136,   172,   145,   194,
     175,   175,   175,   175,   175,   175,   134,   194,   157,    14,
     175,    14,   175,   136,    29,   136,   175,   175,    10,   134,
     134,   134,   134,   134,   141,   175,   173,    14,    24,   140,
     167,   136,   172,   135,   135,   169,   145,   136,   194,   120,
      10,   139,   194,   133,   138,   140,   120,   175,   175,   175,
     141,   175,    28,    48,   141,    28,    48,   141,    28,    31,
      32,    41,    44,    45,    48,    49,   141,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   101,   135,    14,   135,   136,   194,   175,
     136,   194,   137,   194,   175,   175,   194,   120,   120,   120,
     136,   194,   136,   170,   194,   175,   135,    14,   142,   170,
     139,   123,   141,   139,   134,   134,   175,   134,   137,    84,
      84,   171,   134,   137,   134,   175,   135,   175,   175,   136,
     172,   175,   175,   120,   194,   134,   134,   134,   134,   134,
     134,   134,   134,   194,   134,   134,   134,   173,   147,   173,
     135,   175,   173,   173,    87,   173,    87,    87,    24,   120,
      10,    76,    77,   160,   136,   137,    26,    27,    36,   141,
     175,   134,   134,   134,   134,   134,   134,   134,    42,    43,
     141,   123,   141,   123,   141,   173,   173,   173,   134,   134,
     120,   175,    14,    14,   167,    62,    63,    66,   141,   160,
     136,   136,   172,   172,    80,    81,    82,   141,   141,   136,
     175,   139,    14,   175,   175,   175,    28,    32,    41,    44,
      45,    48,   141,   141,   141,   141,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   176,   136,
     194,   136,   170,   194,   134,   136,   134,   136,   194,   134,
     139,   139,   139,   175,   175,   175,   136,   136,   136,   175,
      84,    14,   136,   137,   194,   175,   175,   136,     5,    14,
     134,   175,   134,   137,   134,   160,   136,   134,   134,   175,
     136,   136,    29,   148,   175,   173,    10,    10,    10,    14,
     175,   120,    10,    10,   160,    10,    11,    14,    71,    72,
      73,   175,   175,   141,   175,    28,    31,    32,    41,    44,
      45,    48,    49,   141,   175,   173,   134,   134,    29,   175,
     134,   141,    64,   191,    10,    13,    14,    16,    17,    62,
     160,   160,   136,   136,    14,    14,    14,   134,   120,   135,
     139,   141,   141,   175,   175,   175,   175,   175,   175,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   136,   136,   136,   134,   136,   134,   134,   134,   136,
     175,   135,   194,   134,   134,    84,    84,   137,   194,   160,
     134,   137,   134,   134,   147,   173,   136,    29,   148,   173,
     173,   173,   175,    24,   120,    10,    76,    77,    28,    32,
      48,   141,   141,   141,   175,   175,   175,   175,   175,   175,
     175,   175,   173,   141,   173,   134,   154,   175,   135,   140,
     161,   135,   161,    14,    24,    74,   120,   134,    14,    24,
      74,   120,   134,    10,    13,    16,    17,   134,   134,   191,
     160,   160,     7,     7,     7,   175,   136,   194,   120,   120,
     141,   141,   141,   141,   141,   141,   134,   175,   136,   175,
     175,   194,   134,   194,   173,   173,    14,   175,   120,    10,
      10,   175,   175,   175,   141,   141,   141,   141,   141,   141,
     141,   141,   175,   136,   136,   172,    78,   145,   136,   172,
      14,    14,   134,   175,    14,    14,   134,   175,    14,    24,
      74,   120,   134,    14,    24,    74,   120,   134,   134,   134,
     135,   161,   141,   141,   136,   175,   175,   136,   134,   136,
     134,    29,   148,   175,   141,   141,   141,   134,   173,   161,
     136,   141,    26,    27,    36,   141,   175,   161,   136,   134,
     134,   134,   134,   134,   134,    14,    14,   134,   175,    14,
      14,   134,   175,   136,   172,   141,   141,   141,   134,   173,
     154,   161,   175,   175,   141,   175,    28,    31,    32,    41,
      44,    45,    48,    49,   141,   161,   134,   134,   134,   134,
     134,   134,   161,   136,   136,    28,    32,    48,   141,   141,
     141,   175,   175,   175,   175,   175,   175,   175,   175,   161,
     173,   175,   175,   175,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   143,   144,   145,   145,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   147,   147,   147,   147,
     147,   147,   147,   147,   148,   148,   148,   148,   148,   148,
     149,   149,   150,   150,   150,   150,   151,   151,   152,   152,
     152,   153,   153,   153,   154,   155,   155,   155,   155,   155,
     155,   156,   156,   157,   157,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   159,   159,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   162,   162,   163,   164,   165,   166,   166,
     166,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   168,   169,   169,   169,   169,
     170,   170,   170,   170,   171,   171,   171,   171,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   174,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   176,   176,   177,   177,
     178,   178,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   180,   180,
     181,   181,   181,   182,   182,   183,   183,   183,   184,   184,
     185,   185,   185,   186,   186,   186,   187,   187,   187,   187,
     187,   187,   187,   188,   188,   188,   188,   188,   188,   189,
     189,   190,   190,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   192,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   194,   194,
     194,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     9,     4,     6,     8,     5,     5,
       7,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       4,     3,     4,     5,     5,     6,     6,     4,     2,     5,
       4,     3,     3,     3,     5,     7,     3,     5,     7,     3,
       5,     3,     2,     5,     5,     3,     2,     5,     5,     4,
       7,     6,     4,     6,     4,     2,     4,     4,     4,     4,
       4,     4,     4,     4,     7,     7,     7,     6,     6,     4,
       4,     4,     4,     4,     4,     4,     4,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     4,     4,     4,     4,
       4,     2,     1,     1,     1,     1,     1,     2,     4,     2,
       1,     1,     1,     1,     1,     2,     2,     4,     4,     4,
       4,     1,     1,     1,     1,     1,     5,     7,     7,     3,
       5,     5,     4,     6,     3,     5,     5,     7,     4,     6,
       5,     3,     5,     7,     3,     5,     5,     3,     2,     5,
       5,     2,    12,     9,     1,     7,     7,     5,     7,     5,
       3,     5,     7,     0,     4,     6,     5,     3,     7,     6,
       4,     7,     6,     7,     6,     5,     6,     5,     3,     4,
       5,     4,     5,     5,     6,     6,     6,     6,     6,     6,
       6,     6,     7,     7,     7,     3,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     1,     1,     6,     5,     5,     0,     3,
       3,     0,     4,     4,     4,     5,     5,     4,     5,     5,
       6,     6,     7,     7,     6,     6,     6,     6,     5,     5,
       6,     6,     7,     7,     7,     7,     7,     6,     4,     7,
       6,     4,     8,     7,     5,     5,     0,     4,     4,     4,
       3,     5,     5,     7,     3,     3,     5,     5,     1,     3,
       5,     1,     3,     2,     3,     3,     3,     4,     5,     5,
       3,     5,     4,     6,     1,     3,     1,     3,     1,     3,
       3,     3,     4,     6,     6,     6,     6,     6,     6,     6,
       6,     5,     7,     7,     7,     7,     7,     7,     5,     9,
       8,     8,     6,     9,     9,     5,     5,     4,     4,     4,
       6,     6,     6,     6,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     1,     5,     1,     3,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     1,     2,     2,     2,     2,     2,     1,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     3,
       4,     6,     4,     3,     6,     6,     5,     3,     3,     6,
       5,     4,     4,     6,     6,     5,     3,     6,     5,     3,
       2,     3,     4,     3,     2,     3,     2,     1,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     4,     6,     6,     6,
       1,     4,     4,     5,     5,     1,     1,     1,     1,     2,
       3,     4,     5,     2,     3,     4,     5,     2,     3,     4,
       5,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = RAKU_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == RAKU_YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use RAKU_YYerror or RAKU_YYUNDEF. */
#define YYERRCODE RAKU_YYUNDEF


/* Enable debugging if requested.  */
#if RAKU_YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !RAKU_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !RAKU_YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = RAKU_YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == RAKU_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= RAKU_YYEOF)
    {
      yychar = RAKU_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == RAKU_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = RAKU_YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = RAKU_YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt_list  */
#line 510 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3303 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 520 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3309 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 521 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3315 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 525 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3321 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 527 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3327 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 529 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3333 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 531 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3341 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 535 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3347 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 537 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3353 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 539 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3359 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 541 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3365 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 543 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3373 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 547 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3381 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 551 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3387 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 553 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3395 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 557 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3403 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 561 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3409 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 563 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3415 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 565 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3421 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 567 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3427 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 569 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3433 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 571 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3441 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 575 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3449 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3455 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3461 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3467 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3473 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 587 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3479 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 589 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3485 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 591 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3491 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 593 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3497 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 595 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3503 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 597 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3509 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 599 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3515 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 601 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3521 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 603 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3533 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 611 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3539 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 613 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); free((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3545 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 615 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3551 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 617 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); free((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3557 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 619 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3563 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 621 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3569 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 623 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3576 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 626 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3583 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 629 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3589 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 631 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3596 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 634 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3603 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 637 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3609 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 639 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3617 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 643 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3623 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 645 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3629 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 647 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3636 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 650 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3643 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 653 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3649 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 655 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3656 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 658 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3663 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 661 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 664 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3676 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 666 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3685 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 671 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3693 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 675 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3701 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 679 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3710 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 684 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3718 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 687 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3724 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 689 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3730 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 691 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 693 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3742 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3748 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 697 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3755 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 700 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3761 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 702 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3767 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 704 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3773 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 706 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3780 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 709 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3787 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 712 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3794 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 715 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3801 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 718 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3808 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 721 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3814 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 723 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3820 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 725 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3826 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 727 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3832 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 729 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3839 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 732 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3845 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 734 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3851 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 736 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3857 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 738 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3864 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 741 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3871 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 744 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3879 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 748 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3886 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 751 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3892 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 753 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3898 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 755 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3904 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 757 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3911 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 760 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3918 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 763 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3925 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 766 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3932 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 769 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3939 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 772 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3946 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 775 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3953 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 777 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3959 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 778 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3965 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 779 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3971 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 780 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3977 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 781 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3983 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 782 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3989 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 784 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3995 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 786 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4001 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH block  */
#line 788 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4007 "raku.tab.c"
    break;

  case 110: /* stmt: block  */
#line 790 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4013 "raku.tab.c"
    break;

  case 111: /* stmt: unless_stmt  */
#line 791 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4019 "raku.tab.c"
    break;

  case 112: /* stmt: until_stmt  */
#line 792 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4025 "raku.tab.c"
    break;

  case 113: /* stmt: repeat_stmt  */
#line 793 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4031 "raku.tab.c"
    break;

  case 114: /* stmt: loop_stmt  */
#line 794 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4037 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST ';'  */
#line 795 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4043 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT ';'  */
#line 796 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4049 "raku.tab.c"
    break;

  case 117: /* stmt: KW_LAST KW_IF expr ';'  */
#line 798 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4055 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 800 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4061 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4067 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4073 "raku.tab.c"
    break;

  case 121: /* stmt: sub_decl  */
#line 805 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4079 "raku.tab.c"
    break;

  case 122: /* stmt: class_decl  */
#line 806 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4085 "raku.tab.c"
    break;

  case 123: /* stmt: role_decl  */
#line 807 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4091 "raku.tab.c"
    break;

  case 124: /* stmt: grammar_decl  */
#line 808 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4097 "raku.tab.c"
    break;

  case 125: /* stmt: module_decl  */
#line 809 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4103 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF '(' expr ')' block  */
#line 813 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4109 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 815 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4115 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 817 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4121 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block  */
#line 819 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4127 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 821 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4133 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 823 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4139 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF expr block elsif_tail  */
#line 825 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4145 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 827 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4151 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block  */
#line 831 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4157 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 833 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4163 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 835 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4169 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 837 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4175 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 839 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4181 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 841 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4187 "raku.tab.c"
    break;

  case 140: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 845 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4193 "raku.tab.c"
    break;

  case 141: /* while_stmt: KW_WHILE expr block  */
#line 847 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4199 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 851 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4205 "raku.tab.c"
    break;

  case 143: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4211 "raku.tab.c"
    break;

  case 144: /* unless_stmt: KW_UNLESS expr block  */
#line 855 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4217 "raku.tab.c"
    break;

  case 145: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 857 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4223 "raku.tab.c"
    break;

  case 146: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 861 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4229 "raku.tab.c"
    break;

  case 147: /* until_stmt: KW_UNTIL expr block  */
#line 863 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4235 "raku.tab.c"
    break;

  case 148: /* repeat_stmt: KW_REPEAT block  */
#line 867 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4241 "raku.tab.c"
    break;

  case 149: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4247 "raku.tab.c"
    break;

  case 150: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 871 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4253 "raku.tab.c"
    break;

  case 151: /* loop_stmt: KW_LOOP block  */
#line 875 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4259 "raku.tab.c"
    break;

  case 152: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 877 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4265 "raku.tab.c"
    break;

  case 153: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 879 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4271 "raku.tab.c"
    break;

  case 154: /* loop_incr: expr  */
#line 882 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4277 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 886 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4287 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 892 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4297 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 898 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4306 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 903 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4316 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 909 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4325 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR expr block  */
#line 914 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4332 "raku.tab.c"
    break;

  case 161: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 919 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4344 "raku.tab.c"
    break;

  case 162: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 927 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4357 "raku.tab.c"
    break;

  case 163: /* when_list: %empty  */
#line 937 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4363 "raku.tab.c"
    break;

  case 164: /* when_list: when_list KW_WHEN expr block  */
#line 939 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4371 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 945 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4383 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 953 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4393 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_SUB IDENT sub_body  */
#line 959 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4403 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 965 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4415 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 973 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4425 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 979 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4435 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 985 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4448 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 994 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4459 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1001 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4472 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1010 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4485 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1019 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4496 "raku.tab.c"
    break;

  case 176: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1028 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4507 "raku.tab.c"
    break;

  case 177: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1035 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4516 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list '}'  */
#line 1041 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4522 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr '}'  */
#line 1043 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4529 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1046 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4536 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1049 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4542 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1051 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4548 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1053 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4554 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1055 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4560 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1057 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4566 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1059 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4572 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1061 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4578 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1063 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4585 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1066 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4591 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1068 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4597 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1070 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4603 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1072 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4610 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1075 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4617 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1078 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4625 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list '}'  */
#line 1083 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4631 "raku.tab.c"
    break;

  case 196: /* method_body: '{' YADA '}'  */
#line 1084 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4637 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr '}'  */
#line 1086 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4644 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1089 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4651 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1092 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4657 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1094 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4663 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1096 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4669 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1098 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4675 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1100 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4681 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1102 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4687 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1104 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4693 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1106 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4700 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1109 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4706 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1111 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4712 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1113 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4718 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1115 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4725 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1118 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4732 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1121 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4740 "raku.tab.c"
    break;

  case 213: /* pkg_name: IDENT  */
#line 1126 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4746 "raku.tab.c"
    break;

  case 214: /* pkg_name: QIDENT  */
#line 1127 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4752 "raku.tab.c"
    break;

  case 215: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1131 "raku.y"
        {
            const char *cname = intern((yyvsp[-4].sval)); free((yyvsp[-4].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *cd = ast_node_new(TT_CLASS_DECL);
            if ((yyvsp[-3].sval)) cd->v.sval = (yyvsp[-3].sval);
            ast_push(cd, leaf_sval(TT_VAR, cname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(cd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = cd;
        }
#line 4770 "raku.tab.c"
    break;

  case 216: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1147 "raku.y"
        {
            const char *rname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *rd = ast_node_new(TT_ROLE_DECL);
            ast_push(rd, leaf_sval(TT_VAR, rname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(rd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = rd;
        }
#line 4787 "raku.tab.c"
    break;

  case 217: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1162 "raku.y"
        {
            const char *mname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *md = ast_node_new(TT_MODULE_DECL);
            ast_push(md, leaf_sval(TT_VAR, mname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(md, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = md;
        }
#line 4804 "raku.tab.c"
    break;

  case 218: /* is_clauses: %empty  */
#line 1176 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4810 "raku.tab.c"
    break;

  case 219: /* is_clauses: is_clauses IDENT IDENT  */
#line 1178 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)malloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)malloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); free((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 4826 "raku.tab.c"
    break;

  case 220: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1190 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)malloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)malloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); free((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 4842 "raku.tab.c"
    break;

  case 221: /* class_body_list: %empty  */
#line 1203 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4848 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1205 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4855 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1208 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4862 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1211 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4869 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1214 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4876 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1217 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4883 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1220 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4890 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1223 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4897 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1226 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4904 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1229 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4911 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1232 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4918 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1235 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4925 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1238 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4932 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1241 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4943 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1248 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4954 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1255 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4965 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1262 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4976 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1269 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4984 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1273 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4992 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1277 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5000 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1281 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5008 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1285 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5019 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1292 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5030 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1299 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5041 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1306 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5052 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1313 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5066 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1323 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5078 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1331 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5090 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1339 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5103 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1348 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5114 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1355 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5125 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1362 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5139 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1372 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5151 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1380 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5163 "raku.tab.c"
    break;

  case 255: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1390 "raku.y"
        {
            const char *gname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *gd = ast_node_new(TT_GRAMMAR_DECL);
            ast_push(gd, leaf_sval(TT_VAR, gname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(gd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = gd;
        }
#line 5180 "raku.tab.c"
    break;

  case 256: /* grammar_body_list: %empty  */
#line 1404 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5186 "raku.tab.c"
    break;

  case 257: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1406 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5195 "raku.tab.c"
    break;

  case 258: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1411 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5204 "raku.tab.c"
    break;

  case 259: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1416 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5213 "raku.tab.c"
    break;

  case 260: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1423 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5221 "raku.tab.c"
    break;

  case 261: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1427 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5229 "raku.tab.c"
    break;

  case 262: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1431 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5237 "raku.tab.c"
    break;

  case 263: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1435 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5245 "raku.tab.c"
    break;

  case 264: /* pair_list: IDENT OP_FATARROW expr  */
#line 1441 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5251 "raku.tab.c"
    break;

  case 265: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1443 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5257 "raku.tab.c"
    break;

  case 266: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1445 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5263 "raku.tab.c"
    break;

  case 267: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1447 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5269 "raku.tab.c"
    break;

  case 268: /* param_list: VAR_SCALAR  */
#line 1450 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5275 "raku.tab.c"
    break;

  case 269: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1451 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5281 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1452 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5287 "raku.tab.c"
    break;

  case 271: /* param_list: VAR_ARRAY  */
#line 1453 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5293 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' VAR_ARRAY  */
#line 1454 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5299 "raku.tab.c"
    break;

  case 273: /* param_list: IDENT VAR_SCALAR  */
#line 1455 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5305 "raku.tab.c"
    break;

  case 274: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1456 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5311 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1457 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5317 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' VAR_SCALAR  */
#line 1458 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5323 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1459 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5329 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1460 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5335 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1461 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5341 "raku.tab.c"
    break;

  case 280: /* param_list: VAR_SCALAR '=' expr  */
#line 1462 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5347 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1463 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5353 "raku.tab.c"
    break;

  case 282: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1464 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5359 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1465 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5365 "raku.tab.c"
    break;

  case 284: /* param_list: SLURPY_POS  */
#line 1466 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5371 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' SLURPY_POS  */
#line 1467 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5377 "raku.tab.c"
    break;

  case 286: /* param_list: SLURPY_LOL  */
#line 1468 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5383 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' SLURPY_LOL  */
#line 1469 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5389 "raku.tab.c"
    break;

  case 288: /* param_list: SLURPY_NAMED  */
#line 1470 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5395 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1471 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5401 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list '}'  */
#line 1474 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5407 "raku.tab.c"
    break;

  case 291: /* block: '{' YADA '}'  */
#line 1475 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5413 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr '}'  */
#line 1477 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5419 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1479 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5425 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1481 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5431 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1483 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5437 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1485 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5443 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1487 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5450 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1490 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5456 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1492 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5462 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1494 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5468 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1496 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5474 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1498 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5481 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1501 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5488 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1504 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5496 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1508 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5503 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1511 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5510 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1514 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5517 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1517 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5523 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1519 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5531 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1523 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5538 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1526 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5545 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1529 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5552 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1532 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5559 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1535 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5566 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1538 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5572 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1540 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5578 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1542 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5584 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_LAST '}'  */
#line 1544 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5590 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1546 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5596 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1548 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5602 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1550 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5608 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1552 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5614 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1554 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5620 "raku.tab.c"
    break;

  case 324: /* closure: '{' expr '}'  */
#line 1557 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5626 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR '=' expr  */
#line 1560 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5632 "raku.tab.c"
    break;

  case 326: /* expr: VAR_ARRAY '=' expr  */
#line 1561 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5638 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1562 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5644 "raku.tab.c"
    break;

  case 328: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1563 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5650 "raku.tab.c"
    break;

  case 329: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1564 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5656 "raku.tab.c"
    break;

  case 330: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1565 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5662 "raku.tab.c"
    break;

  case 331: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1566 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5668 "raku.tab.c"
    break;

  case 332: /* expr: KW_GATHER block  */
#line 1567 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5678 "raku.tab.c"
    break;

  case 333: /* expr: KW_GATHER for_stmt  */
#line 1572 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5688 "raku.tab.c"
    break;

  case 334: /* expr: tern_expr OP_FATARROW expr  */
#line 1578 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5694 "raku.tab.c"
    break;

  case 335: /* expr: tern_expr  */
#line 1579 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5700 "raku.tab.c"
    break;

  case 336: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1583 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5706 "raku.tab.c"
    break;

  case 337: /* tern_expr: or_expr  */
#line 1584 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5712 "raku.tab.c"
    break;

  case 338: /* or_expr: or_expr OP_OR and_expr  */
#line 1587 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5718 "raku.tab.c"
    break;

  case 339: /* or_expr: and_expr  */
#line 1588 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5724 "raku.tab.c"
    break;

  case 340: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1591 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5730 "raku.tab.c"
    break;

  case 341: /* and_expr: cmp_expr  */
#line 1592 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5736 "raku.tab.c"
    break;

  case 342: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1595 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5742 "raku.tab.c"
    break;

  case 343: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1596 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5748 "raku.tab.c"
    break;

  case 344: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1597 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5754 "raku.tab.c"
    break;

  case 345: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1598 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5760 "raku.tab.c"
    break;

  case 346: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1599 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5766 "raku.tab.c"
    break;

  case 347: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1600 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5772 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1601 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5778 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1602 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5784 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1603 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5790 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1604 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5796 "raku.tab.c"
    break;

  case 352: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1605 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5802 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1606 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5808 "raku.tab.c"
    break;

  case 354: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1608 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5818 "raku.tab.c"
    break;

  case 355: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1614 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5828 "raku.tab.c"
    break;

  case 356: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1620 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5838 "raku.tab.c"
    break;

  case 357: /* cmp_expr: divis_expr  */
#line 1625 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5844 "raku.tab.c"
    break;

  case 358: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1628 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5850 "raku.tab.c"
    break;

  case 359: /* divis_expr: jct_expr  */
#line 1629 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5856 "raku.tab.c"
    break;

  case 360: /* jct_expr: jct_expr '|' range_expr  */
#line 1632 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5862 "raku.tab.c"
    break;

  case 361: /* jct_expr: jct_expr '&' range_expr  */
#line 1633 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5868 "raku.tab.c"
    break;

  case 362: /* jct_expr: dor_expr  */
#line 1634 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5874 "raku.tab.c"
    break;

  case 363: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1638 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5880 "raku.tab.c"
    break;

  case 364: /* dor_expr: range_expr  */
#line 1639 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5886 "raku.tab.c"
    break;

  case 365: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1642 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5892 "raku.tab.c"
    break;

  case 366: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1643 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5898 "raku.tab.c"
    break;

  case 367: /* range_expr: add_expr  */
#line 1644 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5904 "raku.tab.c"
    break;

  case 368: /* add_expr: add_expr '~' repl_expr  */
#line 1647 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5910 "raku.tab.c"
    break;

  case 369: /* add_expr: repl_expr  */
#line 1648 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5916 "raku.tab.c"
    break;

  case 370: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1651 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5922 "raku.tab.c"
    break;

  case 371: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1652 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5928 "raku.tab.c"
    break;

  case 372: /* repl_expr: addsub_expr  */
#line 1653 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5934 "raku.tab.c"
    break;

  case 373: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1656 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5940 "raku.tab.c"
    break;

  case 374: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1657 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5946 "raku.tab.c"
    break;

  case 375: /* addsub_expr: mul_expr  */
#line 1658 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5952 "raku.tab.c"
    break;

  case 376: /* mul_expr: mul_expr '*' unary_expr  */
#line 1661 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5958 "raku.tab.c"
    break;

  case 377: /* mul_expr: mul_expr '/' unary_expr  */
#line 1662 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5964 "raku.tab.c"
    break;

  case 378: /* mul_expr: mul_expr '%' unary_expr  */
#line 1663 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5970 "raku.tab.c"
    break;

  case 379: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1664 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5976 "raku.tab.c"
    break;

  case 380: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1666 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5982 "raku.tab.c"
    break;

  case 381: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1668 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5988 "raku.tab.c"
    break;

  case 382: /* mul_expr: unary_expr  */
#line 1669 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5994 "raku.tab.c"
    break;

  case 383: /* unary_expr: '-' unary_expr  */
#line 1672 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6000 "raku.tab.c"
    break;

  case 384: /* unary_expr: '+' unary_expr  */
#line 1673 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6006 "raku.tab.c"
    break;

  case 385: /* unary_expr: '!' unary_expr  */
#line 1674 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6012 "raku.tab.c"
    break;

  case 386: /* unary_expr: CARET unary_expr  */
#line 1675 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6018 "raku.tab.c"
    break;

  case 387: /* unary_expr: OP_REDUCE unary_expr  */
#line 1677 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6027 "raku.tab.c"
    break;

  case 388: /* unary_expr: pow_expr  */
#line 1681 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6033 "raku.tab.c"
    break;

  case 389: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1684 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6039 "raku.tab.c"
    break;

  case 390: /* pow_expr: postfix_expr  */
#line 1685 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6045 "raku.tab.c"
    break;

  case 391: /* scalar_list: VAR_SCALAR  */
#line 1688 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6051 "raku.tab.c"
    break;

  case 392: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1689 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6057 "raku.tab.c"
    break;

  case 393: /* meth_name: IDENT  */
#line 1692 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6063 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_SORT  */
#line 1693 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6069 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_REVERSE  */
#line 1694 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6075 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_MAP  */
#line 1695 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6081 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_GREP  */
#line 1696 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6087 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_SAY  */
#line 1697 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6093 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_PRINT  */
#line 1698 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6099 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_TAKE  */
#line 1699 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6105 "raku.tab.c"
    break;

  case 401: /* meth_name: KW_RETURN  */
#line 1700 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6111 "raku.tab.c"
    break;

  case 402: /* meth_name: KW_EXISTS  */
#line 1701 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6117 "raku.tab.c"
    break;

  case 403: /* meth_name: KW_DELETE  */
#line 1702 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6123 "raku.tab.c"
    break;

  case 404: /* meth_name: KW_JOIN  */
#line 1703 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6129 "raku.tab.c"
    break;

  case 405: /* meth_name: TESTOP  */
#line 1704 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6135 "raku.tab.c"
    break;

  case 406: /* postfix_expr: call_expr  */
#line 1706 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6141 "raku.tab.c"
    break;

  case 407: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1709 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6151 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '(' arg_list ')'  */
#line 1715 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6160 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '(' ')'  */
#line 1719 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6166 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1721 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6172 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1723 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6178 "raku.tab.c"
    break;

  case 412: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1725 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6187 "raku.tab.c"
    break;

  case 413: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1730 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6193 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1732 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6203 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1738 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6213 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1744 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6221 "raku.tab.c"
    break;

  case 417: /* call_expr: IDENT '.' KW_NEW  */
#line 1748 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6229 "raku.tab.c"
    break;

  case 418: /* call_expr: IDENT '.' IDENT  */
#line 1752 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6238 "raku.tab.c"
    break;

  case 419: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1757 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6249 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1764 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6258 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '.' CARET IDENT  */
#line 1769 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6268 "raku.tab.c"
    break;

  case 422: /* call_expr: atom '.' CARET IDENT  */
#line 1775 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6278 "raku.tab.c"
    break;

  case 423: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1781 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6289 "raku.tab.c"
    break;

  case 424: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1788 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6300 "raku.tab.c"
    break;

  case 425: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1795 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6309 "raku.tab.c"
    break;

  case 426: /* call_expr: atom '.' meth_name  */
#line 1800 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6318 "raku.tab.c"
    break;

  case 427: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1805 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6329 "raku.tab.c"
    break;

  case 428: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1812 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6338 "raku.tab.c"
    break;

  case 429: /* call_expr: call_expr '.' meth_name  */
#line 1817 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6347 "raku.tab.c"
    break;

  case 430: /* call_expr: KW_DIE expr  */
#line 1822 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6353 "raku.tab.c"
    break;

  case 431: /* call_expr: KW_MAP closure expr  */
#line 1824 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6359 "raku.tab.c"
    break;

  case 432: /* call_expr: KW_MAP closure ',' expr  */
#line 1826 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6365 "raku.tab.c"
    break;

  case 433: /* call_expr: KW_GREP closure expr  */
#line 1828 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6371 "raku.tab.c"
    break;

  case 434: /* call_expr: KW_SORT expr  */
#line 1830 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6377 "raku.tab.c"
    break;

  case 435: /* call_expr: KW_SORT closure expr  */
#line 1832 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6383 "raku.tab.c"
    break;

  case 436: /* call_expr: KW_REVERSE expr  */
#line 1834 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6389 "raku.tab.c"
    break;

  case 437: /* call_expr: atom  */
#line 1835 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6395 "raku.tab.c"
    break;

  case 438: /* arg_list: expr  */
#line 1838 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6401 "raku.tab.c"
    break;

  case 439: /* arg_list: arg_list ',' expr  */
#line 1839 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6407 "raku.tab.c"
    break;

  case 440: /* arg_list: arg_list ','  */
#line 1840 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6413 "raku.tab.c"
    break;

  case 441: /* atom: LIT_INT  */
#line 1843 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6419 "raku.tab.c"
    break;

  case 442: /* atom: LIT_FLOAT  */
#line 1844 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6425 "raku.tab.c"
    break;

  case 443: /* atom: LIT_STR  */
#line 1845 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6431 "raku.tab.c"
    break;

  case 444: /* atom: WORDLIST  */
#line 1847 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6444 "raku.tab.c"
    break;

  case 445: /* atom: LIT_INTERP_STR  */
#line 1855 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6450 "raku.tab.c"
    break;

  case 446: /* atom: VAR_SCALAR  */
#line 1856 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6456 "raku.tab.c"
    break;

  case 447: /* atom: OP_INC VAR_SCALAR  */
#line 1857 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6462 "raku.tab.c"
    break;

  case 448: /* atom: OP_DEC VAR_SCALAR  */
#line 1858 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6468 "raku.tab.c"
    break;

  case 449: /* atom: VAR_SCALAR OP_INC  */
#line 1859 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6474 "raku.tab.c"
    break;

  case 450: /* atom: VAR_SCALAR OP_DEC  */
#line 1860 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6480 "raku.tab.c"
    break;

  case 451: /* atom: VAR_ARRAY  */
#line 1861 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6486 "raku.tab.c"
    break;

  case 452: /* atom: VAR_HASH  */
#line 1862 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6492 "raku.tab.c"
    break;

  case 453: /* atom: VAR_CAPTURE  */
#line 1864 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6500 "raku.tab.c"
    break;

  case 454: /* atom: VAR_FH  */
#line 1868 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6508 "raku.tab.c"
    break;

  case 455: /* atom: VAR_NAMED_CAPTURE  */
#line 1872 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6515 "raku.tab.c"
    break;

  case 456: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1875 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6521 "raku.tab.c"
    break;

  case 457: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1877 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6527 "raku.tab.c"
    break;

  case 458: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1879 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6533 "raku.tab.c"
    break;

  case 459: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1881 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6539 "raku.tab.c"
    break;

  case 460: /* atom: ARR_ALL_SLICE  */
#line 1883 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6545 "raku.tab.c"
    break;

  case 461: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1885 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6551 "raku.tab.c"
    break;

  case 462: /* atom: VAR_HASH '{' expr '}'  */
#line 1887 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6557 "raku.tab.c"
    break;

  case 463: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1889 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6563 "raku.tab.c"
    break;

  case 464: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1891 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6569 "raku.tab.c"
    break;

  case 465: /* atom: IDENT  */
#line 1892 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6575 "raku.tab.c"
    break;

  case 466: /* atom: VAR_TWIGIL  */
#line 1894 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6583 "raku.tab.c"
    break;

  case 467: /* atom: VAR_ARRAY_TWIGIL  */
#line 1898 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6591 "raku.tab.c"
    break;

  case 468: /* atom: VAR_HASH_TWIGIL  */
#line 1902 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6599 "raku.tab.c"
    break;

  case 469: /* atom: '[' ']'  */
#line 1905 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6605 "raku.tab.c"
    break;

  case 470: /* atom: '[' expr ']'  */
#line 1907 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6611 "raku.tab.c"
    break;

  case 471: /* atom: '[' expr ',' ']'  */
#line 1909 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6617 "raku.tab.c"
    break;

  case 472: /* atom: '[' expr ',' arg_list ']'  */
#line 1911 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6624 "raku.tab.c"
    break;

  case 473: /* atom: DOLLAR_LBRACKET ']'  */
#line 1913 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 6630 "raku.tab.c"
    break;

  case 474: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1915 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6636 "raku.tab.c"
    break;

  case 475: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1917 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6642 "raku.tab.c"
    break;

  case 476: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1919 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6649 "raku.tab.c"
    break;

  case 477: /* atom: '(' ')'  */
#line 1921 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6655 "raku.tab.c"
    break;

  case 478: /* atom: '(' expr ')'  */
#line 1922 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6661 "raku.tab.c"
    break;

  case 479: /* atom: '(' expr ',' ')'  */
#line 1924 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6667 "raku.tab.c"
    break;

  case 480: /* atom: '(' expr ',' arg_list ')'  */
#line 1926 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6674 "raku.tab.c"
    break;

  case 481: /* atom: block  */
#line 1928 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6680 "raku.tab.c"
    break;

  case 482: /* atom: KW_SUB block  */
#line 1929 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6686 "raku.tab.c"
    break;


#line 6690 "raku.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == RAKU_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= RAKU_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == RAKU_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = RAKU_YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != RAKU_YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1931 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
