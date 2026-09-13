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
  YYSYMBOL_ADV_EXISTS = 114,               /* ADV_EXISTS  */
  YYSYMBOL_ADV_DELETE = 115,               /* ADV_DELETE  */
  YYSYMBOL_OP_BAND = 116,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 117,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 118,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 119,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 120,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 121,                   /* OP_POW  */
  YYSYMBOL_122_ = 122,                     /* '='  */
  YYSYMBOL_123_ = 123,                     /* '!'  */
  YYSYMBOL_124_ = 124,                     /* '<'  */
  YYSYMBOL_125_ = 125,                     /* '>'  */
  YYSYMBOL_126_ = 126,                     /* '|'  */
  YYSYMBOL_127_ = 127,                     /* '&'  */
  YYSYMBOL_128_ = 128,                     /* '~'  */
  YYSYMBOL_129_ = 129,                     /* '+'  */
  YYSYMBOL_130_ = 130,                     /* '-'  */
  YYSYMBOL_131_ = 131,                     /* '*'  */
  YYSYMBOL_132_ = 132,                     /* '/'  */
  YYSYMBOL_133_ = 133,                     /* '%'  */
  YYSYMBOL_UMINUS = 134,                   /* UMINUS  */
  YYSYMBOL_135_ = 135,                     /* '.'  */
  YYSYMBOL_136_ = 136,                     /* ';'  */
  YYSYMBOL_137_ = 137,                     /* '('  */
  YYSYMBOL_138_ = 138,                     /* ')'  */
  YYSYMBOL_139_ = 139,                     /* ','  */
  YYSYMBOL_140_ = 140,                     /* '['  */
  YYSYMBOL_141_ = 141,                     /* ']'  */
  YYSYMBOL_142_ = 142,                     /* '{'  */
  YYSYMBOL_143_ = 143,                     /* '}'  */
  YYSYMBOL_144_ = 144,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 145,                 /* $accept  */
  YYSYMBOL_program = 146,                  /* program  */
  YYSYMBOL_stmt_list = 147,                /* stmt_list  */
  YYSYMBOL_stmt = 148,                     /* stmt  */
  YYSYMBOL_if_stmt = 149,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 150,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 151,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 152,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 153,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 154,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 155,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 156,                /* loop_incr  */
  YYSYMBOL_for_stmt = 157,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 158,               /* given_stmt  */
  YYSYMBOL_when_list = 159,                /* when_list  */
  YYSYMBOL_sub_decl = 160,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 161,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 162,                 /* sub_body  */
  YYSYMBOL_method_body = 163,              /* method_body  */
  YYSYMBOL_pkg_name = 164,                 /* pkg_name  */
  YYSYMBOL_class_decl = 165,               /* class_decl  */
  YYSYMBOL_role_decl = 166,                /* role_decl  */
  YYSYMBOL_module_decl = 167,              /* module_decl  */
  YYSYMBOL_is_clauses = 168,               /* is_clauses  */
  YYSYMBOL_class_body_list = 169,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 170,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 171,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 172,           /* named_arg_list  */
  YYSYMBOL_pair_list = 173,                /* pair_list  */
  YYSYMBOL_param_list = 174,               /* param_list  */
  YYSYMBOL_block = 175,                    /* block  */
  YYSYMBOL_closure = 176,                  /* closure  */
  YYSYMBOL_expr = 177,                     /* expr  */
  YYSYMBOL_tern_expr = 178,                /* tern_expr  */
  YYSYMBOL_or_expr = 179,                  /* or_expr  */
  YYSYMBOL_and_expr = 180,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 181,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 182,               /* divis_expr  */
  YYSYMBOL_jct_expr = 183,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 184,                 /* dor_expr  */
  YYSYMBOL_range_expr = 185,               /* range_expr  */
  YYSYMBOL_add_expr = 186,                 /* add_expr  */
  YYSYMBOL_repl_expr = 187,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 188,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 189,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 190,               /* unary_expr  */
  YYSYMBOL_pow_expr = 191,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 192,              /* scalar_list  */
  YYSYMBOL_meth_name = 193,                /* meth_name  */
  YYSYMBOL_postfix_expr = 194,             /* postfix_expr  */
  YYSYMBOL_call_expr = 195,                /* call_expr  */
  YYSYMBOL_arg_list = 196,                 /* arg_list  */
  YYSYMBOL_atom = 197                      /* atom  */
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
#define YYLAST   5605

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  145
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  486
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1209

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   377


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
       2,     2,     2,   123,     2,     2,     2,   133,   127,     2,
     137,   138,   131,   129,   139,   130,   135,   132,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   144,   136,
     124,   122,   125,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   140,     2,   141,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   142,   126,   143,   128,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   121,   134
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   510,   510,   521,   522,   525,   527,   529,   531,   535,
     537,   539,   541,   543,   547,   551,   553,   557,   561,   563,
     565,   567,   569,   571,   575,   579,   581,   583,   585,   587,
     589,   591,   593,   595,   597,   599,   601,   603,   611,   613,
     615,   617,   619,   621,   623,   626,   629,   631,   634,   637,
     639,   643,   645,   647,   650,   653,   655,   658,   661,   664,
     666,   671,   675,   679,   684,   688,   689,   691,   693,   695,
     697,   700,   702,   704,   706,   709,   712,   715,   718,   721,
     723,   725,   727,   729,   732,   734,   736,   738,   741,   744,
     748,   751,   753,   755,   757,   760,   763,   766,   769,   772,
     775,   778,   779,   780,   781,   782,   783,   784,   786,   788,
     790,   792,   793,   794,   795,   796,   797,   798,   800,   802,
     804,   806,   807,   808,   809,   810,   813,   815,   817,   819,
     821,   823,   825,   827,   831,   833,   835,   837,   839,   841,
     845,   847,   851,   853,   855,   857,   861,   863,   867,   869,
     871,   875,   877,   879,   883,   886,   892,   898,   903,   909,
     914,   919,   927,   938,   939,   945,   953,   959,   965,   973,
     979,   985,   994,  1001,  1010,  1019,  1028,  1035,  1042,  1043,
    1046,  1049,  1051,  1053,  1055,  1057,  1059,  1061,  1063,  1066,
    1068,  1070,  1072,  1075,  1078,  1084,  1085,  1086,  1089,  1092,
    1094,  1096,  1098,  1100,  1102,  1104,  1106,  1109,  1111,  1113,
    1115,  1118,  1121,  1127,  1128,  1131,  1147,  1162,  1177,  1178,
    1190,  1204,  1205,  1208,  1211,  1214,  1217,  1220,  1223,  1226,
    1229,  1232,  1235,  1238,  1241,  1248,  1255,  1262,  1269,  1273,
    1277,  1281,  1285,  1292,  1299,  1306,  1313,  1323,  1331,  1339,
    1348,  1355,  1362,  1372,  1380,  1390,  1405,  1406,  1411,  1416,
    1423,  1427,  1431,  1435,  1441,  1443,  1445,  1447,  1451,  1452,
    1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,
    1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,
    1475,  1476,  1477,  1479,  1481,  1483,  1485,  1487,  1490,  1492,
    1494,  1496,  1498,  1501,  1504,  1508,  1511,  1514,  1517,  1519,
    1523,  1526,  1529,  1532,  1535,  1538,  1540,  1542,  1544,  1546,
    1548,  1550,  1552,  1554,  1558,  1561,  1562,  1563,  1564,  1565,
    1566,  1567,  1568,  1573,  1578,  1580,  1583,  1585,  1588,  1589,
    1592,  1593,  1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,
    1604,  1605,  1606,  1607,  1608,  1614,  1620,  1626,  1629,  1630,
    1633,  1634,  1635,  1638,  1640,  1643,  1644,  1645,  1648,  1649,
    1652,  1653,  1654,  1657,  1658,  1659,  1662,  1663,  1664,  1665,
    1667,  1669,  1671,  1674,  1675,  1676,  1677,  1678,  1683,  1686,
    1687,  1690,  1691,  1694,  1695,  1696,  1697,  1698,  1699,  1700,
    1701,  1702,  1703,  1704,  1705,  1706,  1708,  1710,  1716,  1721,
    1722,  1724,  1726,  1731,  1733,  1739,  1745,  1749,  1753,  1758,
    1765,  1770,  1776,  1782,  1789,  1796,  1801,  1806,  1813,  1818,
    1823,  1825,  1827,  1829,  1831,  1833,  1835,  1837,  1840,  1841,
    1842,  1845,  1846,  1847,  1848,  1857,  1858,  1859,  1860,  1861,
    1862,  1863,  1864,  1865,  1869,  1873,  1876,  1878,  1880,  1882,
    1884,  1886,  1888,  1890,  1892,  1894,  1896,  1898,  1900,  1902,
    1903,  1907,  1911,  1915,  1916,  1918,  1920,  1923,  1924,  1926,
    1928,  1931,  1932,  1933,  1935,  1938,  1939
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
  "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "ADV_EXISTS",
  "ADV_DELETE", "OP_BAND", "OP_SHL", "OP_DIVIS", "OP_REP_X", "OP_REP_XX",
  "OP_POW", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['",
  "']'", "'{'", "'}'", "':'", "$accept", "program", "stmt_list", "stmt",
  "if_stmt", "elsif_tail", "while_stmt", "unless_stmt", "until_stmt",
  "repeat_stmt", "loop_stmt", "loop_incr", "for_stmt", "given_stmt",
  "when_list", "sub_decl", "scalar_methcall", "sub_body", "method_body",
  "pkg_name", "class_decl", "role_decl", "module_decl", "is_clauses",
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

#define YYPACT_NINF (-887)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-486)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -887,    63,  2416,  -887,  -887,  -887,  -887,  -887,   753,   -46,
     115,  -104,    30,  -887,  -887,  5411,  2514,  -887,  -887,  -887,
     125,  2592,   143,  4397,  4475,  4553,  4631,  4709,    -5,   -17,
    4709,  2670,  2748,   492,    73,  4709,  4709,    98,   111,  4787,
    4865,   -52,   -87,    -1,    74,   -15,   -15,  4943,  4709,   -52,
     -52,  4709,   624,   624,   192,  5411,  -887,  -887,   624,   624,
     161,   199,  5411,  5411,  5411,  -887,  2826,  2905,   169,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,   565,
    -887,  -887,  -887,  -887,  5470,   630,   208,   824,   210,   403,
     636,   597,   202,  -887,   -42,   718,   796,   408,  -887,  -887,
     233,   185,   214,   350,  -887,  -887,  4709,  4709,  4709,  4709,
    4709,  4709,   423,  2983,  5021,  3062,   440,  4709,  4709,   263,
      57,   811,   -53,   270,   154,  -887,   399,   -52,  3140,  -887,
    -887,   558,   187,  -887,   405,  3219,  3297,  -887,  3375,  -887,
     396,    78,   119,   131,   843,   470,   512,   542,  3140,   557,
    3140,   147,  3140,   -52,  3140,   -52,   228,   231,   -49,  -887,
    -887,  -887,   428,  -887,   130,  -887,   220,   375,   434,   483,
     431,   439,   249,   276,  3140,   -52,  3140,   -52,   370,  3454,
    -887,  4709,  4709,  -887,  4709,  4709,  -887,  4709,  3532,  4709,
    3610,  4709,  -887,  -887,   532,  -887,  -887,  -887,  -887,  -887,
     453,   466,    12,  -887,   475,   530,  -887,  -887,  -887,  -887,
    -887,   800,  -887,   797,   804,  -887,   606,   465,  1598,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  4709,  -887,  4709,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  -887,  4709,  5411,  5411,
    5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,
    5411,  5411,  5411,   884,  5411,  5411,  5411,  5411,  5411,  5411,
    5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,
    5411,  5411,  1010,   975,   809,   550,   577,   617,   627,   667,
     712,   -23,  -887,   827,  3140,   520,   854,   679,   754,   595,
     778,  -887,   752,   905,   784,   150,  -887,   933,   849,   857,
    3062,   936,  4709,  4709,  4709,  4709,  4709,  4709,  4709,  4709,
    3688,  -887,  -887,   793,  -887,   816,   818,   797,   859,  -887,
    4709,  4709,  -887,  5099,  -887,  5177,  -887,   211,   221,   311,
     265,   835,   840,  -887,   865,   867,  4709,  4709,  4709,  4709,
    4709,  4709,  4709,  -887,  4709,   870,  4709,  4709,  -887,  4709,
     874,   987,   881,  -887,   971,  4709,  -887,  5411,  5411,     6,
    -887,  -887,  -887,  4709,  4709,  4709,  -887,  4709,  4709,  -887,
    4709,  4709,   864,  4709,  -887,  1019,  4709,  1021,  4709,   887,
    1015,   889,  -887,  4709,  4709,  1039,   919,   927,   950,   963,
     967,   842,  4709,  -887,  -887,  -887,   -52,    18,  -887,    25,
     932,   955,  -887,  -887,  -887,  3766,  1016,  1108,  3845,  -887,
    -887,   965,   287,   306,  1026,  4397,  4475,  4709,  2119,    26,
     212,  -887,   380,  1020,  1032,  1033,  1034,  1035,  1036,  1049,
    1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,  -887,
     210,  1056,   403,  1084,  1084,  1084,  1084,  1084,  1084,  1084,
    1084,  1084,  1084,  1084,  1084,  -887,  -887,  -887,   597,  -887,
    -887,  -887,  1077,  1077,   718,   796,   796,   408,   408,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  1069,
    1193,  1071,  -887,  3923,  -887,  -887,  -887,  -887,  -887,  -887,
    4709,  4002,  -887,   900,  -887,  4709,  4709,  4709,  4709,  1087,
     365,   385,  -887,  4081,  -887,  1170,  4709,  1074,  -887,    -6,
    -887,  1456,   737,  1088,  1075,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,   743,  -887,  -887,  -887,  1076,  -887,  1078,
    3140,   521,  1133,   160,  4160,   545,  1085,  4709,  -887,  5255,
    -887,  4709,  -887,    46,  -887,  4709,  4709,  1097,  3766,  1086,
    1092,  1093,  1095,  1098,  1099,  1105,   561,  3766,  1107,  1110,
     569,   -52,   -18,  5333,  -887,   -52,   -52,   241,   -47,   -39,
      34,  -887,    52,  -887,  -887,  -887,  1081,   904,  1739,   583,
    1111,  1112,  1114,  1115,  1116,  1118,  -887,  1094,   234,  1117,
    1101,  1124,  1119,   -52,   -52,   -52,  1120,  1122,  1137,  4709,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  1206,  1241,  -887,
     309,  1081,   909,    59,   378,   250,  1880,  -887,   913,  4709,
    -887,  -887,   746,  1246,  3062,  4709,  4709,   412,    55,   145,
    -887,   217,  4709,  4709,  -887,  4709,  4709,  -887,  4709,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  5411,  4239,  -887,  1312,  1125,   915,  1127,
    -887,   920,  3766,   666,  1126,  1128,   774,  4709,  -887,  -887,
    4709,  -887,  -887,  4709,  -887,   923,  -887,   939,   942,  -887,
    4709,  1181,  1252,   944,  -887,   970,   974,  -887,  -887,  -887,
     952,  -887,  4709,  4709,  4709,   956,  -887,   198,  -887,  1134,
    3140,   668,  1135,  1081,   958,  1136,  1138,  4709,   966,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,   968,  -887,  -887,
    -887,  1079,  -887,  -887,  3140,   -52,  -887,  -887,  1263,  -887,
    1267,  1268,  1265,  4709,  1158,  1271,  1272,  -887,  1081,   549,
    4397,  4475,  2197,  -887,   414,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  4709,   -52,  -887,  -887,  -887,  1147,  1148,  1256,
    -887,  -887,  -887,  -887,  4709,  1150,  -887,  -887,   360,   680,
     602,  1225,  -887,  -887,  1081,  1081,   972,   976,  1274,  1275,
    1276,  -887,  -887,  -887,  1155,  -887,    27,   779,  1149,   243,
    4709,  4709,  4709,  4709,  4709,  4709,  -887,  -887,  -887,  -887,
     352,   372,   374,   425,   437,   447,   451,   468,   487,   493,
     499,   540,  -887,  -887,   978,  -887,   982,   984,  -887,  1159,
    -887,  -887,   989,  -887,  -887,  -887,  -887,  1160,  1161,  1162,
    -887,  -887,  -887,  1156,  4709,  1164,  -887,  3766,   711,  -887,
    -887,  1166,  1219,  1220,  -887,   992,  -887,  4709,  -887,  -887,
    1081,  -887,  -887,   733,  1169,  1173,   -18,  -887,   994,  1083,
     -52,   -52,   -52,  -887,  -887,  4709,  -887,  -887,  -887,    48,
    -887,   248,  -887,  -887,  -887,   433,    72,  -887,   251,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  4709,  -887,   -52,  1163,
    -887,  -887,   -52,  1175,  4709,  -887,   329,   359,    37,    68,
     422,  1177,  1183,  1010,  -887,  -887,  1081,  1081,  1313,  1314,
    1320,  -887,  4709,  4318,  1213,   415,  -887,   546,   556,   566,
     567,   578,   603,  -887,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  1202,
    -887,  -887,  -887,  -887,  -887,  4709,   996,  -887,  -887,  4709,
    4709,  3766,   734,  -887,  -887,  4709,  -887,  -887,  -887,  -887,
     -52,   -52,  -887,  -887,  -887,  -887,  -887,  1325,  4709,  1218,
    1331,  1332,  4709,  4709,  4709,  -887,  -887,  -887,   605,   607,
     613,   619,   621,   625,   629,   631,  -887,  -887,  -887,  4709,
    1205,  -887,   191,  1266,  -887,   240,  -887,  1333,  1334,  1214,
    4709,  -887,  1335,  1337,  1217,  4709,  -887,    71,    77,  1221,
    1223,  -887,  -887,   376,  -887,  -887,  -887,  -887,  -887,   633,
    1211,   998,  4709,  4709,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  1222,  1226,  -887,  -887,  1002,  -887,   738,  1113,  -887,
    -887,  -887,  4709,  -887,  -887,   635,   637,   641,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  1227,   -52,  1228,  1006,
    1212,  2021,  1228,  1008,  1229,  1235,  -887,  1237,  1238,  1239,
    -887,  1240,  1342,  1347,  1242,  4709,  -887,  1350,  1363,  1243,
    4709,  -887,  -887,  -887,   279,  -887,  -887,  -887,  1245,   643,
     645,  -887,  -887,  1244,  -887,   -52,  -887,  -887,  -887,  -887,
    -887,  4709,  -887,  -887,  1228,  -887,  4397,  4475,  2275,  -887,
     441,  -887,  1228,  -887,  -887,  -887,  -887,  -887,  -887,  1247,
    1248,  -887,  1249,  1250,  1253,  -887,  1254,  1228,  1012,  -887,
    -887,  -887,  -887,  -887,  1255,  -887,   534,   113,  -887,   298,
    4709,  4709,  4709,  4709,  4709,  4709,  4709,  4709,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  1228,   -52,  4709,
    4709,  4709,  -887,  -887,  -887,   649,   653,   655,   657,   658,
     663,   683,   698,  -887,  -887,   699,   700,   703,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   441,   442,   443,   445,   446,   451,
     452,   470,   469,   471,   472,     0,     0,   453,   454,   455,
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
       0,     0,   446,   451,   452,   470,   469,     0,     0,   485,
     386,   446,   451,   477,     0,     0,     0,    38,     0,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,   486,
     333,   332,     0,    52,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   148,     0,
     151,     0,     0,   115,     0,     0,   116,     0,     0,     0,
       3,     0,   434,   436,   107,   109,   430,   213,   214,   218,
       0,     0,     0,   387,     0,     0,   447,   448,   385,   384,
     383,   446,   481,     0,     0,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,   328,   329,   330,   331,
     325,     0,   413,     0,     0,   326,     0,     0,     0,     0,
       0,    42,   418,     0,   417,   469,   409,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   478,    29,     0,    31,     0,   481,     0,     0,    41,
     440,     0,     6,     0,    10,     0,    11,     0,     0,     0,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,    46,     0,
       0,   129,     0,   141,     0,     0,   160,     0,     0,     0,
       3,   167,    49,     0,     0,     0,    51,     0,     0,    55,
       0,     0,     0,     0,   163,     0,     0,     0,     0,     0,
     144,     0,   147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,   433,   435,     0,     0,   221,     0,
       0,     0,   256,     3,   482,     0,     0,     0,     0,   474,
     291,   446,   451,   452,   470,     0,     0,     0,     0,     0,
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
     325,   326,   479,     0,    30,    32,    40,     0,   439,     0,
       0,     0,   443,   469,     0,     0,     0,     0,    26,     0,
      27,     0,    28,     0,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   482,     0,     0,   132,   482,     0,     0,   365,   366,
     268,   271,     0,   284,   286,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,   407,     0,     0,
       0,     0,     0,   482,     0,   482,     0,     0,     0,     0,
     117,   118,   119,   120,   324,   432,   108,     0,     0,   221,
       0,     0,     0,     0,     0,     0,     0,   483,     0,     0,
     392,   475,     0,     0,     0,     0,     0,     0,     0,     0,
     317,     0,     0,     0,   318,     0,     0,   319,     0,     0,
       0,     0,     0,     0,     0,     0,   292,    66,    68,    70,
      73,    71,    72,    67,    69,    79,    81,    83,    86,    84,
      85,    80,    82,     0,     0,   422,     0,     0,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,   466,   468,
       0,   465,   467,     0,   420,     0,   416,     0,     0,   260,
       0,     0,     0,     0,   456,   461,   462,   480,    39,     5,
       0,    12,     0,     0,     0,     0,    19,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      90,    89,    93,    91,    92,    88,    44,     0,    94,    95,
      47,   126,   131,   130,     0,     0,   140,   157,     0,   159,
       0,     0,     0,     0,   273,     0,     0,   166,     0,     0,
       0,     0,     0,   178,     0,    50,    53,    54,    57,    58,
      34,    33,     0,     0,   161,   463,   464,     0,     0,   142,
     145,   146,   149,   150,     0,     0,   219,   220,     0,     0,
       0,     0,   216,   175,     0,     0,     0,     0,     0,     0,
       0,   255,   217,   484,     0,   476,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,   308,   315,   316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   336,   428,     0,   425,     0,     0,    61,     0,
      63,   176,     0,    16,   459,   458,   457,     0,     0,     0,
     419,   414,   415,     0,     0,     0,   411,     0,     0,   265,
     264,     0,     0,     0,    21,     0,    22,     0,    25,   169,
       0,    36,    35,     0,   484,   484,     0,   133,     0,   134,
       0,     0,     0,   269,   280,     0,   274,   275,   165,   276,
     272,     0,   285,   287,   289,     0,     0,   181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,     0,     0,
      77,    78,     0,     0,     0,   215,     0,     0,     0,     0,
       0,     0,     0,     0,   174,   172,     0,     0,     0,     0,
       0,     9,     0,     0,   456,   462,   312,     0,     0,     0,
       0,     0,     0,   320,   321,   322,   323,   293,   295,   297,
     300,   298,   299,   294,   296,   427,   424,   423,    60,   484,
      74,    75,    76,   261,   262,     0,     0,    13,    20,     0,
       0,     0,     0,   168,     7,     0,    45,    48,   128,   127,
     482,     0,   138,   158,   155,   156,   282,     0,     0,   277,
       0,     0,     0,     0,     0,   182,   183,   180,     0,     0,
       0,     0,     0,     0,     0,     0,   164,   162,   143,     0,
       0,   154,     0,     3,   251,     0,   248,     0,     0,     0,
       0,   227,     0,     0,     0,     0,   222,     0,     0,     0,
       0,   223,   224,     0,   171,   173,   257,   258,   259,     0,
     177,     0,     0,     0,   302,   304,   307,   305,   306,   303,
      17,     0,   484,   267,   266,     0,    23,     0,   135,   136,
     270,   281,     0,   278,   279,     0,     0,     0,   184,   186,
     188,   191,   189,   190,   185,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,     0,
     239,     0,     0,     0,     0,     0,   229,     0,     0,     0,
       0,   228,   225,   226,     0,   254,   311,   310,   176,     0,
       0,   263,    14,   484,     8,     0,   139,   283,   192,   194,
     193,     0,   153,   250,     0,   196,     0,     0,     0,   195,
       0,   247,     0,   236,   237,   231,   234,   235,   230,     0,
       0,   240,     0,     0,     0,   241,     0,     0,     0,   309,
     313,   314,    24,   137,     0,   249,     0,     0,   199,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   246,
     244,   245,   233,   242,   243,   232,   253,     0,     0,     0,
       0,     0,   200,   201,   198,     0,     0,     0,     0,     0,
       0,     0,     0,   252,   152,     0,     0,     0,   202,   204,
     206,   209,   207,   208,   203,   205,   210,   212,   211
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -887,  -887,     1,  -887,  -566,  -736,  -887,  -887,  -887,  -887,
    -887,   271,  1362,  -887,  -887,  -887,  -887,  -327,  -886,   417,
    -887,  -887,  -887,  -887,   775,  -887,  -887,  -508,   851,  -387,
     356,  1106,    -2,  -237,  -887,  1165,  1157,   728,  1142,  -887,
     639,   -16,  1139,   893,   903,     4,  -887,  1251,  -269,  -887,
    -887,   282,  -887
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   218,    69,    70,   574,    71,    72,    73,    74,
      75,  1010,    76,    77,   598,    78,    79,   361,  1014,   199,
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
      85,     2,   441,   554,   491,   877,   742,   697,   701,   158,
      25,   157,   622,   703,   134,    27,   580,   581,   118,   130,
     582,   149,   151,   153,   155,   156,   400,   181,   162,   164,
     166,  1016,   617,   170,   171,   580,   581,   175,   177,   582,
     750,   119,   618,   258,   259,   192,   193,   182,   751,   196,
     179,  1017,   104,   105,   642,    68,   580,   581,   752,   203,
     582,  1018,   754,     3,   213,   216,   208,   209,   210,   580,
     581,   292,   987,   582,   643,   293,   114,   583,   584,   585,
     401,   260,  1022,   346,   113,  1092,   260,   169,   359,   260,
      68,  1097,  1023,   360,   115,  1093,   583,   584,   585,   500,
     346,  1098,   184,   347,   275,   276,   277,   278,   279,   280,
     172,  1019,   285,   287,   501,   289,   290,   583,   584,   585,
     347,   294,   185,   173,    68,    68,   213,   187,   755,   756,
     583,   584,   585,   313,   315,   183,   317,    68,   702,   135,
     136,   346,  1024,   982,   586,  1094,   335,  1105,   345,   932,
     350,  1099,   352,   141,   142,   143,   753,   144,   364,  1020,
     619,   347,   183,   621,   933,   120,   724,   121,   836,   644,
     988,   206,   379,  1021,   381,   346,   145,   386,   365,   387,
     388,   146,   389,   390,   723,   391,   393,   394,   391,   395,
    1025,   348,  1123,  1095,   349,   347,  1131,   795,   817,  1100,
     321,   580,   581,   862,  1026,   582,   201,  1096,   348,   207,
     186,   349,   863,  1101,   322,   996,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   202,   431,   432,   433,   434,
     435,   436,   437,   438,   516,   439,   796,   797,  1155,   116,
     645,   323,   462,   463,   714,   364,  1169,   217,   367,   348,
     580,   581,   349,   325,   582,   324,  1183,   117,   989,   757,
     646,  1176,   583,   584,   585,   365,   366,   326,   368,   469,
     470,   471,   472,   473,   474,   475,   772,   773,   301,   364,
     147,   362,   503,   348,   363,   120,   349,   121,   818,   580,
     581,  1193,   237,   582,   793,   120,   302,   121,   522,   365,
     524,   525,   526,   527,   528,   529,   530,   531,   240,   309,
     978,   583,   584,   585,   257,   354,   357,   358,   538,   539,
     272,   541,  1116,   546,   990,   991,   364,   300,   748,  1078,
     798,   799,   800,   547,   559,   560,   561,   562,   563,   564,
     565,   578,   579,   549,   568,   569,   365,   548,   186,   273,
     583,   584,   585,   366,   271,   647,   369,   550,    84,   260,
     819,   588,   590,   591,   274,   592,   593,   355,   594,   595,
      68,   789,   790,   375,   600,   791,   602,   774,  1082,   512,
     320,   606,   607,    68,   159,   161,   936,   366,   580,   581,
     615,   376,   582,   801,   997,   283,   869,   178,   180,   291,
     377,   383,   553,   299,   626,   194,   195,   360,   648,   114,
     300,   649,   650,   637,   638,   639,   641,  1147,   378,   384,
     318,   651,   789,   790,   652,   653,   791,   634,   654,   655,
     116,   888,  1027,   551,   366,  1028,   832,   281,  1029,  1030,
     810,  1184,   899,   337,   811,   900,   901,   552,   635,   583,
     584,   585,   792,   812,   288,   902,   813,   814,   903,   904,
     815,   992,   905,   906,   337,   993,  1012,   924,   925,  1160,
     200,  1013,  1161,  1162,   339,   204,   205,   340,   341,   688,
     689,   994,  1163,   159,   330,  1164,  1165,   690,   610,  1166,
    1167,   241,   242,   243,   244,   943,  1015,   370,   679,   691,
     692,  1013,   167,   915,   684,   685,   168,   693,   611,   351,
     612,   353,   356,  1104,   699,   944,   236,   945,  1013,   538,
     917,   265,   331,   656,   266,   267,   332,   245,   246,   691,
     692,   380,   319,   382,   120,   320,   121,  1043,   710,   268,
     269,   270,   213,   973,   310,   719,   311,   721,   343,   722,
     236,   344,   333,   725,   726,   816,   371,   907,   372,   889,
     890,   613,  1179,   891,   362,   337,  1180,   363,   946,   343,
     373,   745,   344,   665,    84,   339,   995,   236,   340,   341,
     947,   374,  1181,   666,  1168,   336,   764,   667,   337,   338,
     948,   396,   533,   219,   949,   398,   220,   221,   339,  1034,
    1035,   340,   341,   399,   668,   342,   222,   785,   410,   223,
     224,   950,   918,   225,   226,   919,   920,   402,   921,   922,
     892,   893,   894,   669,    85,  1079,   566,   804,  1083,   670,
     951,   570,   807,   808,   809,   671,   952,   577,   197,   198,
     820,   821,   953,   822,   823,   589,   824,   825,   826,   827,
     828,   829,   830,   831,  1033,   597,   504,   711,   228,   505,
     712,   229,   230,   104,   105,   303,   304,   305,   306,   307,
     343,   231,   403,   344,   232,   233,   672,  1182,   234,   235,
     308,   716,   729,   954,   717,   847,   494,   628,   848,  1044,
     632,   849,   731,   343,   476,   113,   344,   736,   853,  1045,
     320,   227,   732,   733,   477,   740,   478,   479,   320,  1046,
    1047,   859,   860,   495,   734,   480,   481,  1148,   865,   765,
     482,  1048,   320,   255,   256,   873,   483,   484,   247,   248,
     249,   250,   251,   252,   485,   486,   487,   488,   511,   735,
     253,   665,   878,   666,   916,   408,  1049,   409,  1068,   667,
    1069,   884,   616,   496,   254,   668,  1070,   669,   895,   896,
     898,   670,  1071,   497,  1072,   671,   236,   672,  1073,   840,
     908,   729,  1074,   731,  1075,   678,  1106,   735,  1118,   960,
    1119,   962,   913,   681,  1120,   665,  1150,   683,  1151,   666,
     686,   667,  1198,   668,   669,   695,  1199,   698,  1200,   670,
    1201,  1202,   843,   498,   866,   320,  1203,   867,   937,   938,
     939,   940,   941,   942,     4,     5,     6,     7,   508,   671,
     509,   131,   132,   124,   125,   295,  1204,    13,    14,    15,
      16,    17,    18,    19,   672,   729,   731,   261,   262,   735,
     728,  1205,  1206,  1207,   127,    29,  1208,   967,   499,   737,
     320,    35,   964,   327,   328,   329,   103,    37,   104,   105,
     106,   107,   108,   109,   110,    45,    46,    47,    48,   974,
    1056,    51,   975,   320,  1114,   111,   508,   320,   704,   510,
      55,    56,   320,   986,   707,   320,    57,   805,   112,   513,
     113,   455,   456,   457,   459,   460,   461,   998,   999,  1000,
    1001,  1002,  1003,  1004,  1005,   104,   105,   303,   304,   305,
     306,   307,  1011,   320,   512,   846,    60,    61,   508,   514,
     934,   515,   308,   238,   239,   263,   264,   741,   743,   534,
    1039,   746,   747,   749,    62,   404,   405,   113,  -391,  -391,
      63,    64,   406,   407,    84,   492,   493,   517,   128,   296,
     523,    67,   535,    68,   536,   297,   834,   555,   837,   779,
     780,   781,   556,  1051,   842,   502,   320,  1053,  1054,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   576,    84,   506,   507,   614,  1061,   518,   519,   476,
    1065,  1066,  1067,   490,   858,   520,   521,   537,   320,   477,
     596,   478,   479,   557,   407,   404,   558,  1076,   404,   567,
     480,   481,   571,   405,  1081,   482,   572,   573,  1087,   575,
     405,   483,   484,  1091,   476,   603,   405,   605,   405,   485,
     486,   487,   488,   599,   477,   601,   478,   479,   404,   682,
    1109,  1110,   758,   759,   604,   480,   481,   794,   759,   608,
     482,   803,   320,   839,   320,   609,   483,   484,   841,   320,
    1117,   850,   320,   610,   485,   486,   487,   488,   103,   623,
     104,   105,   106,   107,   108,   109,   110,   851,   519,  1130,
     852,   320,   856,   519,   688,   689,   611,   111,   691,   692,
     404,   857,   624,  1142,   861,   717,   870,   759,  1146,   612,
     633,   879,   113,   613,   874,   320,   875,   320,   876,   573,
     926,   759,   981,   573,   927,   759,   955,   320,   630,  1011,
     956,   519,   957,   320,  1156,  1157,  1159,   959,   320,   909,
     404,   971,   980,   405,  1052,   320,  1108,   320,   629,   966,
    1113,   320,  1115,   573,  1124,   759,  1132,   759,   636,   972,
    1177,   759,   189,   191,   465,   466,   657,   673,  1185,  1186,
    1187,  1188,  1189,  1190,  1191,  1192,   467,   468,   658,   659,
     660,   661,   662,     4,     5,     6,     7,  1195,  1196,  1197,
     131,   132,   124,   125,   295,   663,    13,    14,    15,    16,
      17,    18,    19,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   254,   127,    29,   260,   674,   675,   676,   687,
      35,   700,   708,   705,   709,  1041,    37,   713,   706,   727,
     786,   718,   729,   360,    45,    46,    47,    48,   730,   731,
      51,   732,   979,   320,   733,   734,   983,   984,   985,    55,
      56,   735,   775,   738,   776,    57,   739,   766,   767,   777,
     768,   769,   770,  1055,   771,   787,   782,  1057,   783,   784,
     806,   838,   778,   840,  1006,   854,   855,   844,  1008,   845,
     864,   868,   871,   880,   872,    60,    61,   881,   882,   883,
     885,   886,   887,   910,   911,   912,   914,   923,   928,   929,
     930,   931,   935,    62,   963,   958,   960,   961,   962,    63,
      64,   965,   968,   969,   970,   976,  1007,   128,   696,   977,
      67,  1009,    68,  1031,   297,     4,     5,     6,     7,  1032,
    1036,  1037,   131,   132,   124,   125,   295,  1038,    13,    14,
      15,    16,    17,    18,    19,  1042,  1058,  1059,  1050,  1060,
    1062,  1063,  1064,  1077,  1080,   127,    29,  1084,  1085,  1088,
    1086,  1089,    35,  1090,  1107,  1125,  1139,  1102,    37,  1103,
    1111,  1140,  1112,  1121,  1143,  1133,    45,    46,    47,    48,
    1013,  1134,    51,  1135,  1136,  1137,  1138,  1144,  1141,  1145,
    1152,    55,    56,  1170,  1171,  1172,  1173,    57,  1149,  1174,
    1175,   160,  1154,  1178,   788,   715,   458,   442,   334,   464,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1122,     0,    62,     0,    84,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,   128,
     835,     0,    67,     0,    68,     0,   297,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     295,  1153,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,  1194,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,   128,     0,     0,    67,     0,    68,     0,
     297,     4,     5,     6,     7,     0,     0,     0,   411,   412,
     413,   414,    12,     0,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,   415,   416,    25,     0,     0,    26,
      27,    28,    29,   417,   418,    32,    33,    34,    35,    36,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
     419,   420,    45,    46,    47,    48,    49,    50,    51,    52,
       0,     0,     0,    53,    54,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,    58,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,    65,    66,     0,     0,    67,     0,
      68,   421,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,     0,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   760,   761,    25,     0,     0,
      26,    27,    28,    29,    30,   762,    32,    33,    34,    35,
      36,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,     0,     0,    53,    54,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,    58,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,    63,    64,
       0,     0,     0,     0,     0,    65,    66,     0,     0,    67,
       0,    68,   763,     4,     5,     6,     7,     0,     0,     0,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     0,     0,     0,    53,    54,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,    58,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,    65,    66,     0,     0,
      67,     0,    68,   802,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,  1126,  1127,    25,
       0,     0,    26,    27,    28,    29,    30,  1128,    32,    33,
      34,    35,    36,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,     0,     0,    53,    54,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
      58,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    60,    61,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    62,     0,     0,     0,     0,     0,
      63,    64,   127,    29,     0,     0,     0,    65,    66,    35,
       0,    67,     0,    68,  1129,    37,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,   163,   128,    51,     0,    67,
       0,    68,   640,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,   163,   128,    51,     0,    67,     0,    68,
     897,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,    63,    64,     0,     0,     0,     0,
       0,   163,   128,     0,     0,    67,     0,    68,  1158,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,     0,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,     0,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,     0,     0,
       0,    53,    54,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,    58,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,    60,    61,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    62,
       0,     0,     0,     0,     0,    63,    64,   127,    29,     0,
       0,     0,    65,    66,    35,     0,    67,     0,    68,     0,
      37,     0,     0,     0,     0,     0,     0,     0,    45,    46,
      47,    48,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    62,    37,     0,
       0,     0,     0,    63,    64,     0,    45,    46,    47,    48,
       0,   128,    51,     0,    67,   133,    68,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     131,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    62,    37,     0,     0,     0,
       0,    63,    64,     0,    45,    46,    47,    48,   137,   138,
      51,     0,    67,     0,    68,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    62,    37,     0,     0,     0,     0,    63,
      64,     0,    45,    46,    47,    48,   163,   128,    51,     0,
      67,     0,    68,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   211,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    62,    37,     0,     0,     0,     0,    63,    64,     0,
      45,    46,    47,    48,   165,   128,    51,     0,    67,     0,
      68,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,    60,    61,     0,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    62,
       0,    37,     0,     0,     0,    63,    64,     0,     0,    45,
      46,    47,    48,   128,   212,    51,    67,     0,    68,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   128,    51,     0,    67,   215,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,    60,    61,
       0,     0,     0,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    62,     0,    37,     0,
       0,     0,    63,    64,     0,     0,    45,    46,    47,    48,
     128,   282,    51,    67,     0,    68,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     131,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    62,    37,     0,     0,     0,
       0,    63,    64,   286,    45,    46,    47,    48,     0,   128,
      51,     0,    67,     0,    68,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,    60,    61,     0,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    62,     0,    37,     0,     0,     0,    63,
      64,     0,     0,    45,    46,    47,    48,   128,   212,    51,
      67,     0,    68,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,   312,   128,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,   314,   128,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,   385,
      60,    61,     0,     0,     0,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    62,     0,
      37,     0,     0,     0,    63,    64,     0,     0,    45,    46,
      47,    48,   128,   316,    51,    67,     0,    68,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    62,    37,     0,
       0,     0,     0,    63,    64,     0,    45,    46,    47,    48,
       0,   128,    51,     0,    67,     0,    68,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     131,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    62,    37,     0,     0,     0,
       0,    63,    64,     0,    45,    46,    47,    48,     0,   128,
      51,   392,    67,     0,    68,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,   217,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    62,    37,     0,     0,     0,     0,    63,
      64,     0,    45,    46,    47,    48,     0,   128,    51,     0,
      67,     0,    68,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    62,    37,     0,     0,     0,     0,    63,    64,     0,
      45,    46,    47,    48,     0,   128,    51,     0,    67,   532,
      68,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,    60,    61,     0,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    62,
       0,    37,     0,     0,     0,    63,    64,     0,     0,    45,
      46,    47,    48,   128,   627,    51,    67,     0,    68,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   128,    51,     0,    67,   631,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,    60,    61,
       0,     0,     0,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    62,     0,    37,     0,
       0,     0,    63,    64,     0,     0,    45,    46,    47,    48,
     128,   677,    51,    67,     0,    68,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,    60,    61,     0,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    62,     0,    37,     0,     0,
       0,    63,    64,     0,     0,    45,    46,    47,    48,   128,
     680,    51,    67,     0,    68,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     4,     5,   542,     7,     0,     0,     0,
     131,   132,   124,   125,   543,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,    60,    61,     0,     0,
       0,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,    62,     0,    37,     0,     0,     0,
      63,    64,     0,     0,    45,    46,    47,    48,   128,   694,
      51,    67,     0,    68,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,    60,    61,     0,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    62,     0,    37,     0,     0,     0,    63,
      64,     0,     0,    45,    46,    47,    48,   128,   212,    51,
      67,     0,    68,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,    60,    61,     0,     0,     0,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    62,     0,    37,     0,     0,     0,    63,    64,
       0,     0,    45,    46,    47,    48,   128,   833,    51,    67,
       0,    68,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,    60,    61,     0,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    62,     0,    37,     0,     0,     0,    63,    64,     0,
       0,    45,    46,    47,    48,   128,  1040,    51,    67,     0,
      68,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   148,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   150,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     152,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,     0,   154,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   128,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   174,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   176,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     128,    51,     0,    67,     0,   190,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,     0,   284,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,   542,     7,     0,     0,     0,   131,   132,   124,
     125,   543,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   540,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   544,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   720,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   122,   123,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     744,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -485,  -485,    60,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
      63,    64,     0,     0,     0,     0,     0,     0,   128,     0,
       0,    67,     0,    68,  -485,  -485,  -485,     0,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,     0,     0,     0,  -485,     0,     0,     0,     0,     0,
       0,     0,  -485,  -485,     0,     0,  -485,  -485,  -485,  -485,
    -485,  -485,     0,     0,  -485,  -485,  -485,  -485,  -485,     0,
       0,  -485,  -485,  -485,     0,  -485
};

static const yytype_int16 yycheck[] =
{
       2,     0,   239,   330,   273,   741,   572,   515,    14,    14,
      28,    27,   399,   521,    16,    32,    10,    11,   122,    15,
      14,    23,    24,    25,    26,    27,    14,    28,    30,    31,
      32,   917,    14,    35,    36,    10,    11,    39,    40,    14,
      87,    11,    24,    85,    86,    47,    48,    48,    87,    51,
     137,    14,   105,   106,    28,   142,    10,    11,    24,    55,
      14,    24,    10,     0,    66,    67,    62,    63,    64,    10,
      11,    14,    24,    14,    48,    18,   122,    71,    72,    73,
      68,   128,    14,    28,   137,    14,   128,    14,   137,   128,
     142,    14,    24,   142,   140,    24,    71,    72,    73,   122,
      28,    24,    28,    48,   106,   107,   108,   109,   110,   111,
      12,    74,   114,   115,   137,   117,   118,    71,    72,    73,
      48,    64,    48,    12,   142,   142,   128,   142,    76,    77,
      71,    72,    73,   135,   136,   136,   138,   142,   144,    14,
      15,    28,    74,   879,   138,    74,   148,  1033,   150,   122,
     152,    74,   154,    10,    11,    12,   122,    14,    28,   122,
     142,    48,   136,   138,   137,   135,   553,   137,   676,   143,
     122,    10,   174,   136,   176,    28,    33,   179,    48,   181,
     182,    38,   184,   185,   138,   187,   188,   189,   190,   191,
     122,   136,  1078,   122,   139,    48,  1082,   138,   143,   122,
     122,    10,    11,     5,   136,    14,    14,   136,   136,    10,
     136,   139,    14,   136,   136,   143,   218,   219,   220,   221,
     222,   223,   224,   225,   226,    33,   228,   229,   230,   231,
     232,   233,   234,   235,    84,   237,   623,   624,  1124,   124,
      28,   122,   258,   259,    84,    28,  1132,    78,    28,   136,
      10,    11,   139,   122,    14,   136,   143,   142,    10,   586,
      48,  1147,    71,    72,    73,    48,   136,   136,    48,   265,
     266,   267,   268,   269,   270,   271,    42,    43,   124,    28,
     137,   136,   284,   136,   139,   135,   139,   137,   143,    10,
      11,  1177,    84,    14,   621,   135,   142,   137,   300,    48,
     302,   303,   304,   305,   306,   307,   308,   309,    98,   122,
     876,    71,    72,    73,   112,    87,    85,    86,   320,   321,
     135,   323,  1058,   325,    76,    77,    28,   140,    87,   138,
      80,    81,    82,   122,   336,   337,   338,   339,   340,   341,
     342,   357,   358,   122,   346,   347,    48,   136,   136,   135,
      71,    72,    73,   136,   121,   143,   136,   136,     2,   128,
     143,   360,   364,   365,    14,   367,   368,   139,   370,   371,
     142,    62,    63,   124,   376,    66,   378,   143,   138,   136,
     139,   383,   384,   142,    28,    29,   143,   136,    10,    11,
     392,   142,    14,   143,   143,   113,   723,    41,    42,   136,
     124,    31,   137,   121,   403,    49,    50,   142,    28,   122,
     140,    31,    32,   415,   416,   417,   418,   138,   142,    49,
     138,    41,    62,    63,    44,    45,    66,   140,    48,    49,
     124,   758,    10,   122,   136,    13,   673,    14,    16,    17,
      28,   143,    28,    31,    32,    31,    32,   136,   142,    71,
      72,    73,   143,    41,    14,    41,    44,    45,    44,    45,
      48,    28,    48,    49,    31,    32,   137,   794,   795,    28,
      53,   142,    31,    32,    41,    58,    59,    44,    45,   114,
     115,    48,    41,   127,    14,    44,    45,   122,   136,    48,
      49,    88,    89,    90,    91,   143,   137,   122,   500,   114,
     115,   142,    10,   143,   506,   507,    14,   122,   136,   153,
     136,   155,   156,   137,   516,   143,   136,   143,   142,   521,
     789,   113,    10,   143,   116,   117,    14,   124,   125,   114,
     115,   175,   136,   177,   135,   139,   137,   122,   540,   131,
     132,   133,   544,   870,   139,   547,   141,   549,   136,   551,
     136,   139,    10,   555,   556,   143,   122,   143,    75,    10,
      11,   136,    28,    14,   136,    31,    32,   139,   143,   136,
     139,   573,   139,   136,   218,    41,   143,   136,    44,    45,
     143,   142,    48,   136,   143,    28,   588,   136,    31,    32,
     143,    59,   310,    28,   143,   142,    31,    32,    41,   926,
     927,    44,    45,   137,   136,    48,    41,   609,   143,    44,
      45,   143,    10,    48,    49,    13,    14,   142,    16,    17,
      71,    72,    73,   136,   626,  1012,   344,   629,  1015,   136,
     143,   349,   634,   635,   636,   136,   143,   355,    14,    15,
     642,   643,   143,   645,   646,   363,   648,   649,   650,   651,
     652,   653,   654,   655,   923,   373,   136,   136,    28,   139,
     139,    31,    32,   105,   106,   107,   108,   109,   110,   111,
     136,    41,   142,   139,    44,    45,   136,   143,    48,    49,
     122,   136,   136,   143,   139,   687,   136,   405,   690,   143,
     408,   693,   136,   136,    14,   137,   139,   136,   700,   143,
     139,   136,   136,   136,    24,   136,    26,    27,   139,   143,
     143,   713,   714,   136,   136,    35,    36,  1104,   720,   136,
      40,   143,   139,   126,   127,   727,    46,    47,    92,    93,
      94,    95,    96,    97,    54,    55,    56,    57,   143,   136,
     104,   136,   744,   136,    64,   139,   143,   141,   143,   136,
     143,   753,   396,   136,   118,   136,   143,   136,   760,   761,
     762,   136,   143,   136,   143,   136,   136,   136,   143,   136,
     772,   136,   143,   136,   143,   493,   143,   136,   143,   136,
     143,   136,   784,   501,   143,   136,   143,   505,   143,   136,
     508,   136,   143,   136,   136,   513,   143,   515,   143,   136,
     143,   143,   136,   136,   136,   139,   143,   139,   810,   811,
     812,   813,   814,   815,     3,     4,     5,     6,   139,   136,
     141,    10,    11,    12,    13,    14,   143,    16,    17,    18,
      19,    20,    21,    22,   136,   136,   136,   119,   120,   136,
     558,   143,   143,   143,    33,    34,   143,   136,   136,   567,
     139,    40,   854,    10,    11,    12,   103,    46,   105,   106,
     107,   108,   109,   110,   111,    54,    55,    56,    57,   136,
     136,    60,   139,   139,   136,   122,   139,   139,   141,   125,
      69,    70,   139,   885,   141,   139,    75,   141,   135,   137,
     137,     7,     8,     9,   255,   256,   257,   899,   900,   901,
     902,   903,   904,   905,   906,   105,   106,   107,   108,   109,
     110,   111,   914,   139,   136,   141,   105,   106,   139,    14,
     141,   137,   122,    99,   100,   129,   130,   571,   572,   136,
     932,   575,   576,   577,   123,   138,   139,   137,   138,   139,
     129,   130,   138,   139,   588,   136,   137,    14,   137,   138,
      14,   140,   136,   142,   136,   144,   674,   122,   676,   603,
     604,   605,   122,   965,   682,   138,   139,   969,   970,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,    10,   626,   129,   130,   143,   988,   138,   139,    14,
     992,   993,   994,    18,   712,   138,   139,   138,   139,    24,
     136,    26,    27,   138,   139,   138,   139,  1009,   138,   139,
      35,    36,   138,   139,  1013,    40,    29,    30,  1020,   138,
     139,    46,    47,  1025,    14,   138,   139,   138,   139,    54,
      55,    56,    57,    14,    24,    14,    26,    27,   138,   139,
    1042,  1043,   138,   139,    29,    35,    36,   138,   139,    10,
      40,   138,   139,   138,   139,   136,    46,    47,   138,   139,
    1062,   138,   139,   136,    54,    55,    56,    57,   103,   137,
     105,   106,   107,   108,   109,   110,   111,   138,   139,  1081,
     138,   139,   138,   139,   114,   115,   136,   122,   114,   115,
     138,   139,   137,  1095,   138,   139,   138,   139,  1100,   136,
     135,   745,   137,   136,   138,   139,   138,   139,    29,    30,
     138,   139,    29,    30,   138,   139,   138,   139,    10,  1121,
     138,   139,   138,   139,  1126,  1127,  1128,   138,   139,   773,
     138,   139,   138,   139,   138,   139,   138,   139,   122,   857,
     138,   139,    29,    30,   138,   139,   138,   139,   122,   867,
     138,   139,    46,    47,   261,   262,   136,   101,  1160,  1161,
    1162,  1163,  1164,  1165,  1166,  1167,   263,   264,   136,   136,
     136,   136,   136,     3,     4,     5,     6,  1179,  1180,  1181,
      10,    11,    12,    13,    14,   136,    16,    17,    18,    19,
      20,    21,    22,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   118,    33,    34,   128,   137,    14,   137,   122,
      40,   137,   136,   125,   136,   933,    46,    84,   143,   122,
      14,   136,   136,   142,    54,    55,    56,    57,   136,   136,
      60,   136,   876,   139,   136,   136,   880,   881,   882,    69,
      70,   136,   125,   136,   143,    75,   136,   136,   136,   125,
     136,   136,   136,   971,   136,    14,   136,   975,   136,   122,
      14,   136,   143,   136,   908,    84,    14,   141,   912,   141,
     136,   136,   136,    10,   136,   105,   106,    10,    10,    14,
     122,    10,    10,   136,   136,    29,   136,    62,    14,    14,
      14,   136,   143,   123,   138,   136,   136,   136,   136,   129,
     130,   137,   136,    84,    84,   136,   143,   137,   138,   136,
     140,   136,   142,   136,   144,     3,     4,     5,     6,   136,
       7,     7,    10,    11,    12,    13,    14,     7,    16,    17,
      18,    19,    20,    21,    22,   122,   980,   981,   136,    14,
     122,    10,    10,   138,    78,    33,    34,    14,    14,    14,
     136,    14,    40,   136,   143,   143,    14,   136,    46,   136,
     138,    14,   136,   136,    14,   136,    54,    55,    56,    57,
     142,   136,    60,   136,   136,   136,   136,    14,   136,   136,
     136,    69,    70,   136,   136,   136,   136,    75,   143,   136,
     136,    29,  1121,   138,   619,   544,   254,   240,   147,   260,
      -1,    -1,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1077,    -1,   123,    -1,  1081,    -1,    -1,
      -1,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,    -1,   142,    -1,   144,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,  1115,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,  1178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,   140,    -1,   142,    -1,
     144,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    -1,    -1,    65,    66,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,    -1,   140,    -1,
     142,   143,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,    -1,    -1,    -1,    -1,   136,   137,    -1,    -1,   140,
      -1,   142,   143,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,   129,
     130,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,   142,   143,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,   105,   106,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
     129,   130,    33,    34,    -1,    -1,    -1,   136,   137,    40,
      -1,   140,    -1,   142,   143,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,
      -1,    54,    55,    56,    57,   136,   137,    60,    -1,   140,
      -1,   142,   143,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,
      55,    56,    57,   136,   137,    60,    -1,   140,    -1,   142,
     143,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,    -1,   140,    -1,   142,   143,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    -1,
      -1,    65,    66,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,   105,   106,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,   129,   130,    33,    34,    -1,
      -1,    -1,   136,   137,    40,    -1,   140,    -1,   142,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,
      -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,    57,
      -1,   137,    60,    -1,   140,   141,   142,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,
      -1,   129,   130,    -1,    54,    55,    56,    57,   136,   137,
      60,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   123,    46,    -1,    -1,    -1,    -1,   129,
     130,    -1,    54,    55,    56,    57,   136,   137,    60,    -1,
     140,    -1,   142,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,
      54,    55,    56,    57,   136,   137,    60,    -1,   140,    -1,
     142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   123,
      -1,    46,    -1,    -1,    -1,   129,   130,    -1,    -1,    54,
      55,    56,    57,   137,   138,    60,   140,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,
      -1,    -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,
      57,    -1,   137,    60,    -1,   140,   141,   142,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,   123,    -1,    46,    -1,
      -1,    -1,   129,   130,    -1,    -1,    54,    55,    56,    57,
     137,   138,    60,   140,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,
      -1,   129,   130,   131,    54,    55,    56,    57,    -1,   137,
      60,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,   123,    -1,    46,    -1,    -1,    -1,   129,
     130,    -1,    -1,    54,    55,    56,    57,   137,   138,    60,
     140,    -1,   142,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,
      -1,    54,    55,    56,    57,   136,   137,    60,    -1,   140,
      -1,   142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,
      55,    56,    57,   136,   137,    60,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
     105,   106,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   123,    -1,
      46,    -1,    -1,    -1,   129,   130,    -1,    -1,    54,    55,
      56,    57,   137,   138,    60,   140,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,
      -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,    57,
      -1,   137,    60,    -1,   140,    -1,   142,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,
      -1,   129,   130,    -1,    54,    55,    56,    57,    -1,   137,
      60,   139,   140,    -1,   142,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    78,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   123,    46,    -1,    -1,    -1,    -1,   129,
     130,    -1,    54,    55,    56,    57,    -1,   137,    60,    -1,
     140,    -1,   142,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,
      54,    55,    56,    57,    -1,   137,    60,    -1,   140,   141,
     142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   123,
      -1,    46,    -1,    -1,    -1,   129,   130,    -1,    -1,    54,
      55,    56,    57,   137,   138,    60,   140,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,
      -1,    -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,
      57,    -1,   137,    60,    -1,   140,   141,   142,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,   123,    -1,    46,    -1,
      -1,    -1,   129,   130,    -1,    -1,    54,    55,    56,    57,
     137,   138,    60,   140,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   123,    -1,    46,    -1,    -1,
      -1,   129,   130,    -1,    -1,    54,    55,    56,    57,   137,
     138,    60,   140,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,   123,    -1,    46,    -1,    -1,    -1,
     129,   130,    -1,    -1,    54,    55,    56,    57,   137,   138,
      60,   140,    -1,   142,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,   123,    -1,    46,    -1,    -1,    -1,   129,
     130,    -1,    -1,    54,    55,    56,    57,   137,   138,    60,
     140,    -1,   142,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,   123,    -1,    46,    -1,    -1,    -1,   129,   130,
      -1,    -1,    54,    55,    56,    57,   137,   138,    60,   140,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   123,    -1,    46,    -1,    -1,    -1,   129,   130,    -1,
      -1,    54,    55,    56,    57,   137,   138,    60,   140,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,
      55,    56,    57,    -1,   137,    60,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,
      -1,    -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,
      57,    -1,   137,    60,    -1,   140,    -1,   142,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,
      -1,    -1,   129,   130,    -1,    54,    55,    56,    57,    -1,
     137,    60,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,    -1,
     129,   130,    -1,    54,    55,    56,    57,    -1,   137,    60,
      -1,   140,    -1,   142,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,
      -1,    54,    55,    56,    57,    -1,   137,    60,    -1,   140,
      -1,   142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,
      55,    56,    57,    -1,   137,    60,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,
      -1,    -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,
      57,    -1,   137,    60,    -1,   140,    -1,   142,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,
      -1,    -1,   129,   130,    -1,    54,    55,    56,    57,    -1,
     137,    60,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,    -1,
     129,   130,    -1,    54,    55,    56,    57,    -1,   137,    60,
      -1,   140,    -1,   142,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,
      -1,    54,    55,    56,    57,    -1,   137,    60,    -1,   140,
      -1,   142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,
      55,    56,    57,    -1,   137,    60,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,
      -1,    -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,
      57,    -1,   137,    60,    -1,   140,    -1,   142,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,
      -1,    -1,   129,   130,    -1,    54,    55,    56,    57,    -1,
     137,    60,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,   140,    -1,   142,    84,    85,    86,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,    -1,    -1,   116,   117,   118,   119,
     120,   121,    -1,    -1,   124,   125,   126,   127,   128,    -1,
      -1,   131,   132,   133,    -1,   135
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   146,   147,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    65,    66,    69,    70,    75,    79,    83,
     105,   106,   123,   129,   130,   136,   137,   140,   142,   148,
     149,   151,   152,   153,   154,   155,   157,   158,   160,   161,
     165,   166,   167,   170,   175,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     194,   195,   197,   103,   105,   106,   107,   108,   109,   110,
     111,   122,   135,   137,   122,   140,   124,   142,   122,    11,
     135,   137,    10,    11,    12,    13,    14,    33,   137,   175,
     190,    10,    11,   141,   177,    14,    15,   136,   137,   177,
     196,    10,    11,    12,    14,    33,    38,   137,   137,   177,
     137,   177,   137,   177,   137,   177,   177,   186,    14,   175,
     157,   175,   177,   136,   177,   136,   177,    10,    14,    14,
     177,   177,    12,    12,   137,   177,   137,   177,   175,   137,
     175,    28,    48,   136,    28,    48,   136,   142,   176,   176,
     142,   176,   177,   177,   175,   175,   177,    14,    15,   164,
     164,    14,    33,   190,   164,   164,    10,    10,   190,   190,
     190,    10,   138,   177,   192,   141,   177,    78,   147,    28,
      31,    32,    41,    44,    45,    48,    49,   136,    28,    31,
      32,    41,    44,    45,    48,    49,   136,    84,    99,   100,
      98,    88,    89,    90,    91,   124,   125,    92,    93,    94,
      95,    96,    97,   104,   118,   126,   127,   112,    85,    86,
     128,   119,   120,   129,   130,   113,   116,   117,   131,   132,
     133,   121,   135,   135,    14,   177,   177,   177,   177,   177,
     177,    14,   138,   196,   137,   177,   131,   177,    14,   177,
     177,   136,    14,    18,    64,    14,   138,   144,   172,   196,
     140,   124,   142,   107,   108,   109,   110,   111,   122,   122,
     139,   141,   136,   177,   136,   177,   138,   177,   196,   136,
     139,   122,   136,   122,   136,   122,   136,    10,    11,    12,
      14,    10,    14,    10,   192,   177,    28,    31,    32,    41,
      44,    45,    48,   136,   139,   177,    28,    48,   136,   139,
     177,   175,   177,   175,    87,   139,   175,    85,    86,   137,
     142,   162,   136,   139,    28,    48,   136,    28,    48,   136,
     122,   122,    75,   139,   142,   124,   142,   124,   142,   177,
     175,   177,   175,    31,    49,    25,   177,   177,   177,   177,
     177,   177,   139,   177,   177,   177,    59,   168,   142,   137,
      14,    68,   142,   142,   138,   139,   138,   139,   139,   141,
     143,    10,    11,    12,    13,    26,    27,    35,    36,    52,
      53,   143,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     180,   178,   181,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,     7,     8,     9,   183,   185,
     185,   185,   186,   186,   187,   188,   188,   189,   189,   190,
     190,   190,   190,   190,   190,   190,    14,    24,    26,    27,
      35,    36,    40,    46,    47,    54,    55,    56,    57,   193,
      18,   193,   136,   137,   136,   136,   136,   136,   136,   136,
     122,   137,   138,   177,   136,   139,   129,   130,   139,   141,
     125,   143,   136,   137,    14,   137,    84,    14,   138,   139,
     138,   139,   177,    14,   177,   177,   177,   177,   177,   177,
     177,   177,   141,   196,   136,   136,   136,   138,   177,   177,
     137,   177,     5,    14,   137,   173,   177,   122,   136,   122,
     136,   122,   136,   137,   162,   122,   122,   138,   139,   177,
     177,   177,   177,   177,   177,   177,   196,   139,   177,   177,
     196,   138,    29,    30,   150,   138,    10,   196,   186,   186,
      10,    11,    14,    71,    72,    73,   138,   174,   147,   196,
     177,   177,   177,   177,   177,   177,   136,   196,   159,    14,
     177,    14,   177,   138,    29,   138,   177,   177,    10,   136,
     136,   136,   136,   136,   143,   177,   175,    14,    24,   142,
     169,   138,   174,   137,   137,   171,   147,   138,   196,   122,
      10,   141,   196,   135,   140,   142,   122,   177,   177,   177,
     143,   177,    28,    48,   143,    28,    48,   143,    28,    31,
      32,    41,    44,    45,    48,    49,   143,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   101,   137,    14,   137,   138,   196,   177,
     138,   196,   139,   196,   177,   177,   196,   122,   114,   115,
     122,   114,   115,   122,   138,   196,   138,   172,   196,   177,
     137,    14,   144,   172,   141,   125,   143,   141,   136,   136,
     177,   136,   139,    84,    84,   173,   136,   139,   136,   177,
     137,   177,   177,   138,   174,   177,   177,   122,   196,   136,
     136,   136,   136,   136,   136,   136,   136,   196,   136,   136,
     136,   175,   149,   175,   137,   177,   175,   175,    87,   175,
      87,    87,    24,   122,    10,    76,    77,   162,   138,   139,
      26,    27,    36,   143,   177,   136,   136,   136,   136,   136,
     136,   136,    42,    43,   143,   125,   143,   125,   143,   175,
     175,   175,   136,   136,   122,   177,    14,    14,   169,    62,
      63,    66,   143,   162,   138,   138,   174,   174,    80,    81,
      82,   143,   143,   138,   177,   141,    14,   177,   177,   177,
      28,    32,    41,    44,    45,    48,   143,   143,   143,   143,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   178,   138,   196,   138,   172,   196,   136,   138,
     136,   138,   196,   136,   141,   141,   141,   177,   177,   177,
     138,   138,   138,   177,    84,    14,   138,   139,   196,   177,
     177,   138,     5,    14,   136,   177,   136,   139,   136,   162,
     138,   136,   136,   177,   138,   138,    29,   150,   177,   175,
      10,    10,    10,    14,   177,   122,    10,    10,   162,    10,
      11,    14,    71,    72,    73,   177,   177,   143,   177,    28,
      31,    32,    41,    44,    45,    48,    49,   143,   177,   175,
     136,   136,    29,   177,   136,   143,    64,   193,    10,    13,
      14,    16,    17,    62,   162,   162,   138,   138,    14,    14,
      14,   136,   122,   137,   141,   143,   143,   177,   177,   177,
     177,   177,   177,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   138,   138,   138,   136,   138,
     136,   136,   136,   138,   177,   137,   196,   136,   136,    84,
      84,   139,   196,   162,   136,   139,   136,   136,   149,   175,
     138,    29,   150,   175,   175,   175,   177,    24,   122,    10,
      76,    77,    28,    32,    48,   143,   143,   143,   177,   177,
     177,   177,   177,   177,   177,   177,   175,   143,   175,   136,
     156,   177,   137,   142,   163,   137,   163,    14,    24,    74,
     122,   136,    14,    24,    74,   122,   136,    10,    13,    16,
      17,   136,   136,   193,   162,   162,     7,     7,     7,   177,
     138,   196,   122,   122,   143,   143,   143,   143,   143,   143,
     136,   177,   138,   177,   177,   196,   136,   196,   175,   175,
      14,   177,   122,    10,    10,   177,   177,   177,   143,   143,
     143,   143,   143,   143,   143,   143,   177,   138,   138,   174,
      78,   147,   138,   174,    14,    14,   136,   177,    14,    14,
     136,   177,    14,    24,    74,   122,   136,    14,    24,    74,
     122,   136,   136,   136,   137,   163,   143,   143,   138,   177,
     177,   138,   136,   138,   136,    29,   150,   177,   143,   143,
     143,   136,   175,   163,   138,   143,    26,    27,    36,   143,
     177,   163,   138,   136,   136,   136,   136,   136,   136,    14,
      14,   136,   177,    14,    14,   136,   177,   138,   174,   143,
     143,   143,   136,   175,   156,   163,   177,   177,   143,   177,
      28,    31,    32,    41,    44,    45,    48,    49,   143,   163,
     136,   136,   136,   136,   136,   136,   163,   138,   138,    28,
      32,    48,   143,   143,   143,   177,   177,   177,   177,   177,
     177,   177,   177,   163,   175,   177,   177,   177,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   145,   146,   147,   147,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   149,   149,   149,   149,
     149,   149,   149,   149,   150,   150,   150,   150,   150,   150,
     151,   151,   152,   152,   152,   152,   153,   153,   154,   154,
     154,   155,   155,   155,   156,   157,   157,   157,   157,   157,
     157,   158,   158,   159,   159,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   161,   161,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   164,   164,   165,   166,   167,   168,   168,
     168,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   170,   171,   171,   171,   171,
     172,   172,   172,   172,   173,   173,   173,   173,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   176,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   178,   178,   179,   179,
     180,   180,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   182,   182,
     183,   183,   183,   184,   184,   185,   185,   185,   186,   186,
     187,   187,   187,   188,   188,   188,   189,   189,   189,   189,
     189,   189,   189,   190,   190,   190,   190,   190,   190,   191,
     191,   192,   192,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   194,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   196,   196,
     196,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197
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
       1,     4,     4,     5,     5,     5,     5,     5,     5,     1,
       1,     1,     1,     2,     3,     4,     5,     2,     3,     4,
       5,     2,     3,     4,     5,     1,     2
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
#line 511 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3345 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 521 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3351 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 522 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3357 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 526 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3363 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 528 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3369 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 530 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3375 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 532 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3383 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 536 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3389 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 538 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3395 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 540 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3401 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 542 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3407 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 544 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3415 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 548 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3423 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 552 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3429 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 554 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3437 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 558 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3445 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 562 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3451 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 564 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3457 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 566 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3463 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 568 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3469 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 570 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3475 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 572 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3483 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 576 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3491 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 580 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3497 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 582 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3503 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 584 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3509 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 586 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3515 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 588 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3521 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 590 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3527 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 592 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3533 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 594 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3539 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 596 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3545 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 598 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3551 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 600 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3557 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 602 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3563 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 604 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3575 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 612 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3581 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 614 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); free((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3587 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 616 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3593 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 618 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); free((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3599 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 620 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3605 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 622 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3611 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 624 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3618 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 627 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3625 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 630 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3631 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 632 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3638 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 635 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3645 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 638 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3651 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 640 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3659 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 644 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3665 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 646 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3671 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 648 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3678 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 651 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3685 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 654 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3691 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 656 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3698 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 659 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3705 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 662 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3712 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 665 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3718 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 667 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3727 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 672 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3735 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 676 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3743 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 680 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3752 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 685 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3760 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 688 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3766 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3772 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 692 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3778 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 694 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3784 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 696 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3790 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 698 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3797 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 701 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3803 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 703 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3809 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 705 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3815 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 707 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3822 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 710 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3829 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 713 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3836 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 716 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3843 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 719 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3850 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 722 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3856 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 724 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3862 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 726 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3868 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 728 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3874 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 730 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3881 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 733 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3887 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 735 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3893 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 737 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3899 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 739 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3906 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 742 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3913 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 745 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3921 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 749 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3928 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 752 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3934 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 754 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3940 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 756 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3946 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 758 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3953 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 761 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3960 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 764 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3967 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 767 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3974 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 770 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3981 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 773 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3988 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 776 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3995 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 778 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4001 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 779 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4007 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 780 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4013 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 781 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4019 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 782 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4025 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 783 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4031 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 785 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4037 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 787 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4043 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH block  */
#line 789 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4049 "raku.tab.c"
    break;

  case 110: /* stmt: block  */
#line 791 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4055 "raku.tab.c"
    break;

  case 111: /* stmt: unless_stmt  */
#line 792 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4061 "raku.tab.c"
    break;

  case 112: /* stmt: until_stmt  */
#line 793 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4067 "raku.tab.c"
    break;

  case 113: /* stmt: repeat_stmt  */
#line 794 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4073 "raku.tab.c"
    break;

  case 114: /* stmt: loop_stmt  */
#line 795 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4079 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST ';'  */
#line 796 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4085 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT ';'  */
#line 797 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4091 "raku.tab.c"
    break;

  case 117: /* stmt: KW_LAST KW_IF expr ';'  */
#line 799 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4097 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 801 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4103 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 803 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4109 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 805 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4115 "raku.tab.c"
    break;

  case 121: /* stmt: sub_decl  */
#line 806 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4121 "raku.tab.c"
    break;

  case 122: /* stmt: class_decl  */
#line 807 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4127 "raku.tab.c"
    break;

  case 123: /* stmt: role_decl  */
#line 808 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4133 "raku.tab.c"
    break;

  case 124: /* stmt: grammar_decl  */
#line 809 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4139 "raku.tab.c"
    break;

  case 125: /* stmt: module_decl  */
#line 810 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4145 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF '(' expr ')' block  */
#line 814 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4151 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 816 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4157 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 818 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4163 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block  */
#line 820 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4169 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 822 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4175 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 824 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4181 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF expr block elsif_tail  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4187 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4193 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block  */
#line 832 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4199 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 834 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4205 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 836 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4211 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 838 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4217 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4223 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4229 "raku.tab.c"
    break;

  case 140: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 846 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4235 "raku.tab.c"
    break;

  case 141: /* while_stmt: KW_WHILE expr block  */
#line 848 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4241 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 852 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4247 "raku.tab.c"
    break;

  case 143: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 854 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4253 "raku.tab.c"
    break;

  case 144: /* unless_stmt: KW_UNLESS expr block  */
#line 856 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4259 "raku.tab.c"
    break;

  case 145: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 858 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4265 "raku.tab.c"
    break;

  case 146: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 862 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4271 "raku.tab.c"
    break;

  case 147: /* until_stmt: KW_UNTIL expr block  */
#line 864 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4277 "raku.tab.c"
    break;

  case 148: /* repeat_stmt: KW_REPEAT block  */
#line 868 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4283 "raku.tab.c"
    break;

  case 149: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 870 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4289 "raku.tab.c"
    break;

  case 150: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 872 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4295 "raku.tab.c"
    break;

  case 151: /* loop_stmt: KW_LOOP block  */
#line 876 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4301 "raku.tab.c"
    break;

  case 152: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 878 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4307 "raku.tab.c"
    break;

  case 153: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 880 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4313 "raku.tab.c"
    break;

  case 154: /* loop_incr: expr  */
#line 883 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4319 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 887 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4329 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 893 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4339 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 899 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4348 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 904 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4358 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 910 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4367 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR expr block  */
#line 915 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4374 "raku.tab.c"
    break;

  case 161: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 920 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4386 "raku.tab.c"
    break;

  case 162: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 928 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4399 "raku.tab.c"
    break;

  case 163: /* when_list: %empty  */
#line 938 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4405 "raku.tab.c"
    break;

  case 164: /* when_list: when_list KW_WHEN expr block  */
#line 940 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4413 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 946 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4425 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 954 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4435 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_SUB IDENT sub_body  */
#line 960 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4445 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 966 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4457 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 974 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4467 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 980 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4477 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 986 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4490 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 995 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4501 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1002 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4514 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1011 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4527 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1020 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4538 "raku.tab.c"
    break;

  case 176: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1029 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4549 "raku.tab.c"
    break;

  case 177: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1036 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4558 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list '}'  */
#line 1042 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4564 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr '}'  */
#line 1044 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4571 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1047 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4578 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1050 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4584 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1052 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4590 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1054 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4596 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1056 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4602 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1058 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4608 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1060 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4614 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1062 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4620 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1064 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4627 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1067 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4633 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1069 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4639 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1071 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4645 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1073 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4652 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1076 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4659 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1079 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4667 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list '}'  */
#line 1084 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4673 "raku.tab.c"
    break;

  case 196: /* method_body: '{' YADA '}'  */
#line 1085 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4679 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr '}'  */
#line 1087 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4686 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1090 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4693 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1093 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4699 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1095 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4705 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1097 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4711 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1099 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4717 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1101 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4723 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1103 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4729 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1105 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4735 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1107 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4742 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1110 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4748 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1112 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4754 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1114 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4760 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1116 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4767 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1119 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4774 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1122 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4782 "raku.tab.c"
    break;

  case 213: /* pkg_name: IDENT  */
#line 1127 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4788 "raku.tab.c"
    break;

  case 214: /* pkg_name: QIDENT  */
#line 1128 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4794 "raku.tab.c"
    break;

  case 215: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1132 "raku.y"
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
#line 4812 "raku.tab.c"
    break;

  case 216: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1148 "raku.y"
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
#line 4829 "raku.tab.c"
    break;

  case 217: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1163 "raku.y"
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
#line 4846 "raku.tab.c"
    break;

  case 218: /* is_clauses: %empty  */
#line 1177 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4852 "raku.tab.c"
    break;

  case 219: /* is_clauses: is_clauses IDENT IDENT  */
#line 1179 "raku.y"
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
#line 4868 "raku.tab.c"
    break;

  case 220: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1191 "raku.y"
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
#line 4884 "raku.tab.c"
    break;

  case 221: /* class_body_list: %empty  */
#line 1204 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4890 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1206 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4897 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1209 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4904 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1212 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4911 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1215 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4918 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1218 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4925 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1221 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4932 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1224 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4939 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1227 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4946 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1230 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4953 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1233 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4960 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1236 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4967 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1239 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4974 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1242 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4985 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1249 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4996 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1256 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5007 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1263 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5018 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1270 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5026 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1274 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5034 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1278 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5042 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1282 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5050 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1286 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5061 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1293 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5072 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1300 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5083 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1307 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5094 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1314 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5108 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1324 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5120 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1332 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5132 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1340 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5145 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1349 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5156 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1356 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5167 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1363 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5181 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1373 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5193 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1381 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5205 "raku.tab.c"
    break;

  case 255: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1391 "raku.y"
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
#line 5222 "raku.tab.c"
    break;

  case 256: /* grammar_body_list: %empty  */
#line 1405 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5228 "raku.tab.c"
    break;

  case 257: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1407 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5237 "raku.tab.c"
    break;

  case 258: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1412 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5246 "raku.tab.c"
    break;

  case 259: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1417 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5255 "raku.tab.c"
    break;

  case 260: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1424 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5263 "raku.tab.c"
    break;

  case 261: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1428 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5271 "raku.tab.c"
    break;

  case 262: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1432 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5279 "raku.tab.c"
    break;

  case 263: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1436 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5287 "raku.tab.c"
    break;

  case 264: /* pair_list: IDENT OP_FATARROW expr  */
#line 1442 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5293 "raku.tab.c"
    break;

  case 265: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1444 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5299 "raku.tab.c"
    break;

  case 266: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1446 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5305 "raku.tab.c"
    break;

  case 267: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1448 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5311 "raku.tab.c"
    break;

  case 268: /* param_list: VAR_SCALAR  */
#line 1451 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5317 "raku.tab.c"
    break;

  case 269: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1452 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5323 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1453 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5329 "raku.tab.c"
    break;

  case 271: /* param_list: VAR_ARRAY  */
#line 1454 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5335 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' VAR_ARRAY  */
#line 1455 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5341 "raku.tab.c"
    break;

  case 273: /* param_list: IDENT VAR_SCALAR  */
#line 1456 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5347 "raku.tab.c"
    break;

  case 274: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1457 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5353 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1458 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5359 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' VAR_SCALAR  */
#line 1459 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5365 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1460 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5371 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1461 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5377 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1462 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5383 "raku.tab.c"
    break;

  case 280: /* param_list: VAR_SCALAR '=' expr  */
#line 1463 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5389 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1464 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5395 "raku.tab.c"
    break;

  case 282: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1465 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5401 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1466 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5407 "raku.tab.c"
    break;

  case 284: /* param_list: SLURPY_POS  */
#line 1467 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5413 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' SLURPY_POS  */
#line 1468 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5419 "raku.tab.c"
    break;

  case 286: /* param_list: SLURPY_LOL  */
#line 1469 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5425 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' SLURPY_LOL  */
#line 1470 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5431 "raku.tab.c"
    break;

  case 288: /* param_list: SLURPY_NAMED  */
#line 1471 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5437 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1472 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5443 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list '}'  */
#line 1475 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5449 "raku.tab.c"
    break;

  case 291: /* block: '{' YADA '}'  */
#line 1476 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5455 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr '}'  */
#line 1478 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5461 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1480 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5467 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1482 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5473 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1484 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5479 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1486 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5485 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1488 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5492 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1491 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5498 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1493 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5504 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1495 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5510 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1497 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5516 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1499 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5523 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1502 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5530 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1505 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5538 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1509 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5545 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1512 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5552 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1515 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5559 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1518 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5565 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1520 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5573 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1524 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5580 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1527 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5587 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1530 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5594 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1533 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5601 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1536 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5608 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1539 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5614 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1541 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5620 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1543 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5626 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_LAST '}'  */
#line 1545 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5632 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1547 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5638 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1549 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5644 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1551 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5650 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1553 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5656 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1555 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5662 "raku.tab.c"
    break;

  case 324: /* closure: '{' expr '}'  */
#line 1558 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5668 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR '=' expr  */
#line 1561 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5674 "raku.tab.c"
    break;

  case 326: /* expr: VAR_ARRAY '=' expr  */
#line 1562 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5680 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1563 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5686 "raku.tab.c"
    break;

  case 328: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1564 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5692 "raku.tab.c"
    break;

  case 329: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1565 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5698 "raku.tab.c"
    break;

  case 330: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1566 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5704 "raku.tab.c"
    break;

  case 331: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1567 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5710 "raku.tab.c"
    break;

  case 332: /* expr: KW_GATHER block  */
#line 1568 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5720 "raku.tab.c"
    break;

  case 333: /* expr: KW_GATHER for_stmt  */
#line 1573 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5730 "raku.tab.c"
    break;

  case 334: /* expr: tern_expr OP_FATARROW expr  */
#line 1579 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5736 "raku.tab.c"
    break;

  case 335: /* expr: tern_expr  */
#line 1580 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5742 "raku.tab.c"
    break;

  case 336: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1584 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5748 "raku.tab.c"
    break;

  case 337: /* tern_expr: or_expr  */
#line 1585 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5754 "raku.tab.c"
    break;

  case 338: /* or_expr: or_expr OP_OR and_expr  */
#line 1588 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5760 "raku.tab.c"
    break;

  case 339: /* or_expr: and_expr  */
#line 1589 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5766 "raku.tab.c"
    break;

  case 340: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1592 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5772 "raku.tab.c"
    break;

  case 341: /* and_expr: cmp_expr  */
#line 1593 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5778 "raku.tab.c"
    break;

  case 342: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1596 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5784 "raku.tab.c"
    break;

  case 343: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1597 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5790 "raku.tab.c"
    break;

  case 344: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1598 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5796 "raku.tab.c"
    break;

  case 345: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1599 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5802 "raku.tab.c"
    break;

  case 346: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1600 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5808 "raku.tab.c"
    break;

  case 347: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1601 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5814 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1602 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5820 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1603 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5826 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1604 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5832 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1605 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5838 "raku.tab.c"
    break;

  case 352: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1606 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5844 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1607 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5850 "raku.tab.c"
    break;

  case 354: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1609 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5860 "raku.tab.c"
    break;

  case 355: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1615 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5870 "raku.tab.c"
    break;

  case 356: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1621 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5880 "raku.tab.c"
    break;

  case 357: /* cmp_expr: divis_expr  */
#line 1626 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5886 "raku.tab.c"
    break;

  case 358: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1629 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5892 "raku.tab.c"
    break;

  case 359: /* divis_expr: jct_expr  */
#line 1630 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5898 "raku.tab.c"
    break;

  case 360: /* jct_expr: jct_expr '|' range_expr  */
#line 1633 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5904 "raku.tab.c"
    break;

  case 361: /* jct_expr: jct_expr '&' range_expr  */
#line 1634 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5910 "raku.tab.c"
    break;

  case 362: /* jct_expr: dor_expr  */
#line 1635 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5916 "raku.tab.c"
    break;

  case 363: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1639 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5922 "raku.tab.c"
    break;

  case 364: /* dor_expr: range_expr  */
#line 1640 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5928 "raku.tab.c"
    break;

  case 365: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1643 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5934 "raku.tab.c"
    break;

  case 366: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1644 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5940 "raku.tab.c"
    break;

  case 367: /* range_expr: add_expr  */
#line 1645 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5946 "raku.tab.c"
    break;

  case 368: /* add_expr: add_expr '~' repl_expr  */
#line 1648 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5952 "raku.tab.c"
    break;

  case 369: /* add_expr: repl_expr  */
#line 1649 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5958 "raku.tab.c"
    break;

  case 370: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1652 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5964 "raku.tab.c"
    break;

  case 371: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1653 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5970 "raku.tab.c"
    break;

  case 372: /* repl_expr: addsub_expr  */
#line 1654 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5976 "raku.tab.c"
    break;

  case 373: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1657 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5982 "raku.tab.c"
    break;

  case 374: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1658 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5988 "raku.tab.c"
    break;

  case 375: /* addsub_expr: mul_expr  */
#line 1659 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5994 "raku.tab.c"
    break;

  case 376: /* mul_expr: mul_expr '*' unary_expr  */
#line 1662 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6000 "raku.tab.c"
    break;

  case 377: /* mul_expr: mul_expr '/' unary_expr  */
#line 1663 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6006 "raku.tab.c"
    break;

  case 378: /* mul_expr: mul_expr '%' unary_expr  */
#line 1664 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6012 "raku.tab.c"
    break;

  case 379: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1666 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6018 "raku.tab.c"
    break;

  case 380: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1668 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6024 "raku.tab.c"
    break;

  case 381: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1670 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6030 "raku.tab.c"
    break;

  case 382: /* mul_expr: unary_expr  */
#line 1671 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6036 "raku.tab.c"
    break;

  case 383: /* unary_expr: '-' unary_expr  */
#line 1674 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6042 "raku.tab.c"
    break;

  case 384: /* unary_expr: '+' unary_expr  */
#line 1675 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6048 "raku.tab.c"
    break;

  case 385: /* unary_expr: '!' unary_expr  */
#line 1676 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6054 "raku.tab.c"
    break;

  case 386: /* unary_expr: CARET unary_expr  */
#line 1677 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6060 "raku.tab.c"
    break;

  case 387: /* unary_expr: OP_REDUCE unary_expr  */
#line 1679 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6069 "raku.tab.c"
    break;

  case 388: /* unary_expr: pow_expr  */
#line 1683 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6075 "raku.tab.c"
    break;

  case 389: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1686 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6081 "raku.tab.c"
    break;

  case 390: /* pow_expr: postfix_expr  */
#line 1687 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6087 "raku.tab.c"
    break;

  case 391: /* scalar_list: VAR_SCALAR  */
#line 1690 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6093 "raku.tab.c"
    break;

  case 392: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1691 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6099 "raku.tab.c"
    break;

  case 393: /* meth_name: IDENT  */
#line 1694 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6105 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_SORT  */
#line 1695 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6111 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_REVERSE  */
#line 1696 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6117 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_MAP  */
#line 1697 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6123 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_GREP  */
#line 1698 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6129 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_SAY  */
#line 1699 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6135 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_PRINT  */
#line 1700 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6141 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_TAKE  */
#line 1701 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6147 "raku.tab.c"
    break;

  case 401: /* meth_name: KW_RETURN  */
#line 1702 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6153 "raku.tab.c"
    break;

  case 402: /* meth_name: KW_EXISTS  */
#line 1703 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6159 "raku.tab.c"
    break;

  case 403: /* meth_name: KW_DELETE  */
#line 1704 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6165 "raku.tab.c"
    break;

  case 404: /* meth_name: KW_JOIN  */
#line 1705 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6171 "raku.tab.c"
    break;

  case 405: /* meth_name: TESTOP  */
#line 1706 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6177 "raku.tab.c"
    break;

  case 406: /* postfix_expr: call_expr  */
#line 1708 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6183 "raku.tab.c"
    break;

  case 407: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1711 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6193 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '(' arg_list ')'  */
#line 1717 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6202 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '(' ')'  */
#line 1721 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6208 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1723 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6214 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1725 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6220 "raku.tab.c"
    break;

  case 412: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1727 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6229 "raku.tab.c"
    break;

  case 413: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1732 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6235 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1734 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6245 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1740 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6255 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1746 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6263 "raku.tab.c"
    break;

  case 417: /* call_expr: IDENT '.' KW_NEW  */
#line 1750 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6271 "raku.tab.c"
    break;

  case 418: /* call_expr: IDENT '.' IDENT  */
#line 1754 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6280 "raku.tab.c"
    break;

  case 419: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1759 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6291 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1766 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6300 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '.' CARET IDENT  */
#line 1771 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6310 "raku.tab.c"
    break;

  case 422: /* call_expr: atom '.' CARET IDENT  */
#line 1777 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6320 "raku.tab.c"
    break;

  case 423: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1783 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6331 "raku.tab.c"
    break;

  case 424: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1790 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6342 "raku.tab.c"
    break;

  case 425: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1797 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6351 "raku.tab.c"
    break;

  case 426: /* call_expr: atom '.' meth_name  */
#line 1802 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6360 "raku.tab.c"
    break;

  case 427: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1807 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6371 "raku.tab.c"
    break;

  case 428: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1814 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6380 "raku.tab.c"
    break;

  case 429: /* call_expr: call_expr '.' meth_name  */
#line 1819 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6389 "raku.tab.c"
    break;

  case 430: /* call_expr: KW_DIE expr  */
#line 1824 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6395 "raku.tab.c"
    break;

  case 431: /* call_expr: KW_MAP closure expr  */
#line 1826 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6401 "raku.tab.c"
    break;

  case 432: /* call_expr: KW_MAP closure ',' expr  */
#line 1828 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6407 "raku.tab.c"
    break;

  case 433: /* call_expr: KW_GREP closure expr  */
#line 1830 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6413 "raku.tab.c"
    break;

  case 434: /* call_expr: KW_SORT expr  */
#line 1832 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6419 "raku.tab.c"
    break;

  case 435: /* call_expr: KW_SORT closure expr  */
#line 1834 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6425 "raku.tab.c"
    break;

  case 436: /* call_expr: KW_REVERSE expr  */
#line 1836 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6431 "raku.tab.c"
    break;

  case 437: /* call_expr: atom  */
#line 1837 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6437 "raku.tab.c"
    break;

  case 438: /* arg_list: expr  */
#line 1840 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6443 "raku.tab.c"
    break;

  case 439: /* arg_list: arg_list ',' expr  */
#line 1841 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6449 "raku.tab.c"
    break;

  case 440: /* arg_list: arg_list ','  */
#line 1842 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6455 "raku.tab.c"
    break;

  case 441: /* atom: LIT_INT  */
#line 1845 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6461 "raku.tab.c"
    break;

  case 442: /* atom: LIT_FLOAT  */
#line 1846 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6467 "raku.tab.c"
    break;

  case 443: /* atom: LIT_STR  */
#line 1847 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6473 "raku.tab.c"
    break;

  case 444: /* atom: WORDLIST  */
#line 1849 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6486 "raku.tab.c"
    break;

  case 445: /* atom: LIT_INTERP_STR  */
#line 1857 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6492 "raku.tab.c"
    break;

  case 446: /* atom: VAR_SCALAR  */
#line 1858 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6498 "raku.tab.c"
    break;

  case 447: /* atom: OP_INC VAR_SCALAR  */
#line 1859 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6504 "raku.tab.c"
    break;

  case 448: /* atom: OP_DEC VAR_SCALAR  */
#line 1860 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6510 "raku.tab.c"
    break;

  case 449: /* atom: VAR_SCALAR OP_INC  */
#line 1861 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6516 "raku.tab.c"
    break;

  case 450: /* atom: VAR_SCALAR OP_DEC  */
#line 1862 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6522 "raku.tab.c"
    break;

  case 451: /* atom: VAR_ARRAY  */
#line 1863 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6528 "raku.tab.c"
    break;

  case 452: /* atom: VAR_HASH  */
#line 1864 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6534 "raku.tab.c"
    break;

  case 453: /* atom: VAR_CAPTURE  */
#line 1866 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6542 "raku.tab.c"
    break;

  case 454: /* atom: VAR_FH  */
#line 1870 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6550 "raku.tab.c"
    break;

  case 455: /* atom: VAR_NAMED_CAPTURE  */
#line 1874 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6557 "raku.tab.c"
    break;

  case 456: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1877 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6563 "raku.tab.c"
    break;

  case 457: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1879 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6569 "raku.tab.c"
    break;

  case 458: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1881 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6575 "raku.tab.c"
    break;

  case 459: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1883 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6581 "raku.tab.c"
    break;

  case 460: /* atom: ARR_ALL_SLICE  */
#line 1885 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6587 "raku.tab.c"
    break;

  case 461: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1887 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6593 "raku.tab.c"
    break;

  case 462: /* atom: VAR_HASH '{' expr '}'  */
#line 1889 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6599 "raku.tab.c"
    break;

  case 463: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1891 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6605 "raku.tab.c"
    break;

  case 464: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1893 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6611 "raku.tab.c"
    break;

  case 465: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 1895 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 6617 "raku.tab.c"
    break;

  case 466: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 1897 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 6623 "raku.tab.c"
    break;

  case 467: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 1899 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 6629 "raku.tab.c"
    break;

  case 468: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 1901 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 6635 "raku.tab.c"
    break;

  case 469: /* atom: IDENT  */
#line 1902 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6641 "raku.tab.c"
    break;

  case 470: /* atom: VAR_TWIGIL  */
#line 1904 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6649 "raku.tab.c"
    break;

  case 471: /* atom: VAR_ARRAY_TWIGIL  */
#line 1908 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6657 "raku.tab.c"
    break;

  case 472: /* atom: VAR_HASH_TWIGIL  */
#line 1912 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6665 "raku.tab.c"
    break;

  case 473: /* atom: '[' ']'  */
#line 1915 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6671 "raku.tab.c"
    break;

  case 474: /* atom: '[' expr ']'  */
#line 1917 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6677 "raku.tab.c"
    break;

  case 475: /* atom: '[' expr ',' ']'  */
#line 1919 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6683 "raku.tab.c"
    break;

  case 476: /* atom: '[' expr ',' arg_list ']'  */
#line 1921 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6690 "raku.tab.c"
    break;

  case 477: /* atom: DOLLAR_LBRACKET ']'  */
#line 1923 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 6696 "raku.tab.c"
    break;

  case 478: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1925 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6702 "raku.tab.c"
    break;

  case 479: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1927 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6708 "raku.tab.c"
    break;

  case 480: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1929 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6715 "raku.tab.c"
    break;

  case 481: /* atom: '(' ')'  */
#line 1931 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6721 "raku.tab.c"
    break;

  case 482: /* atom: '(' expr ')'  */
#line 1932 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6727 "raku.tab.c"
    break;

  case 483: /* atom: '(' expr ',' ')'  */
#line 1934 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6733 "raku.tab.c"
    break;

  case 484: /* atom: '(' expr ',' arg_list ')'  */
#line 1936 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6740 "raku.tab.c"
    break;

  case 485: /* atom: block  */
#line 1938 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6746 "raku.tab.c"
    break;

  case 486: /* atom: KW_SUB block  */
#line 1939 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6752 "raku.tab.c"
    break;


#line 6756 "raku.tab.c"

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

#line 1941 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
