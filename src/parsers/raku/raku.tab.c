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
static tree_t *rk_for_multi(ExprList *vars, tree_t *list, tree_t *body) {
    static int __fm_uid = 0; char av[32], iv[32];
    snprintf(av, sizeof av, "__fm_a_%d", __fm_uid); snprintf(iv, sizeof iv, "__fm_i_%d", __fm_uid); __fm_uid++;
    int k = vars ? vars->count : 0;
    tree_t *hoist = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(av)), list);
    tree_t *z = ast_node_new(TT_ILIT); z->v.ival = 0;
    tree_t *init = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), z);
    tree_t *el = make_call("elems"); expr_add_child(el, leaf_sval(TT_VAR, intern(av)));
    tree_t *cond = expr_binary(TT_LT, leaf_sval(TT_VAR, intern(iv)), el);
    tree_t *kk = ast_node_new(TT_ILIT); kk->v.ival = k;
    tree_t *incr = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), expr_binary(TT_ADD, leaf_sval(TT_VAR, intern(iv)), kk));
    tree_t *seq = ast_node_new(TT_SEQ);
    for (int i = 0; i < k; i++) {
        tree_t *idx = leaf_sval(TT_VAR, intern(iv));
        if (i) { tree_t *off = ast_node_new(TT_ILIT); off->v.ival = i; idx = expr_binary(TT_ADD, idx, off); }
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, intern(av))); expr_add_child(get, idx);
        expr_add_child(seq, expr_binary(TT_ASSIGN, vars->items[i], get));
    }
    expr_add_child(seq, body);
    if (vars) exprlist_free(vars);
    tree_t *outer = ast_node_new(TT_SEQ);
    expr_add_child(outer, hoist); expr_add_child(outer, rk_cstyle_loop(init, cond, incr, seq));
    return outer;
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

#line 513 "raku.tab.c"

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
  YYSYMBOL_catch_when_list = 159,          /* catch_when_list  */
  YYSYMBOL_when_list = 160,                /* when_list  */
  YYSYMBOL_sub_decl = 161,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 162,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 163,                 /* sub_body  */
  YYSYMBOL_method_body = 164,              /* method_body  */
  YYSYMBOL_pkg_name = 165,                 /* pkg_name  */
  YYSYMBOL_class_decl = 166,               /* class_decl  */
  YYSYMBOL_role_decl = 167,                /* role_decl  */
  YYSYMBOL_module_decl = 168,              /* module_decl  */
  YYSYMBOL_is_clauses = 169,               /* is_clauses  */
  YYSYMBOL_class_body_list = 170,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 171,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 172,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 173,           /* named_arg_list  */
  YYSYMBOL_pair_list = 174,                /* pair_list  */
  YYSYMBOL_param_list = 175,               /* param_list  */
  YYSYMBOL_block = 176,                    /* block  */
  YYSYMBOL_closure = 177,                  /* closure  */
  YYSYMBOL_expr = 178,                     /* expr  */
  YYSYMBOL_tern_expr = 179,                /* tern_expr  */
  YYSYMBOL_or_expr = 180,                  /* or_expr  */
  YYSYMBOL_and_expr = 181,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 182,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 183,               /* divis_expr  */
  YYSYMBOL_jct_expr = 184,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 185,                 /* dor_expr  */
  YYSYMBOL_range_expr = 186,               /* range_expr  */
  YYSYMBOL_add_expr = 187,                 /* add_expr  */
  YYSYMBOL_repl_expr = 188,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 189,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 190,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 191,               /* unary_expr  */
  YYSYMBOL_pow_expr = 192,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 193,              /* scalar_list  */
  YYSYMBOL_meth_name = 194,                /* meth_name  */
  YYSYMBOL_postfix_expr = 195,             /* postfix_expr  */
  YYSYMBOL_call_expr = 196,                /* call_expr  */
  YYSYMBOL_arg_list = 197,                 /* arg_list  */
  YYSYMBOL_atom = 198                      /* atom  */
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
#define YYLAST   5667

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  145
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  491
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1224

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
       0,   534,   534,   545,   546,   549,   551,   553,   555,   559,
     561,   563,   565,   567,   571,   575,   577,   581,   585,   587,
     589,   591,   593,   595,   599,   603,   605,   607,   609,   611,
     613,   615,   617,   619,   621,   623,   625,   627,   635,   637,
     639,   641,   643,   645,   647,   650,   653,   655,   658,   661,
     663,   667,   669,   671,   674,   677,   679,   682,   685,   688,
     690,   695,   699,   703,   708,   712,   713,   715,   717,   719,
     721,   724,   726,   728,   730,   733,   736,   739,   742,   745,
     747,   749,   751,   753,   756,   758,   760,   762,   765,   768,
     772,   775,   777,   779,   781,   784,   787,   790,   793,   796,
     799,   802,   803,   804,   805,   806,   807,   808,   810,   812,
     816,   821,   825,   827,   829,   830,   831,   832,   833,   834,
     835,   837,   839,   841,   843,   844,   845,   846,   847,   850,
     852,   854,   856,   858,   860,   862,   864,   868,   870,   872,
     874,   876,   878,   882,   884,   888,   890,   892,   894,   898,
     900,   904,   906,   908,   912,   914,   916,   920,   923,   929,
     935,   943,   949,   954,   959,   967,   978,   980,   984,   985,
     991,   999,  1005,  1011,  1019,  1025,  1031,  1040,  1047,  1056,
    1065,  1074,  1081,  1088,  1089,  1092,  1095,  1097,  1099,  1101,
    1103,  1105,  1107,  1109,  1112,  1114,  1116,  1118,  1121,  1124,
    1130,  1131,  1132,  1135,  1138,  1140,  1142,  1144,  1146,  1148,
    1150,  1152,  1155,  1157,  1159,  1161,  1164,  1167,  1173,  1174,
    1177,  1193,  1208,  1223,  1224,  1236,  1250,  1251,  1254,  1257,
    1260,  1263,  1266,  1269,  1272,  1275,  1278,  1281,  1284,  1287,
    1294,  1301,  1308,  1315,  1319,  1323,  1327,  1331,  1338,  1345,
    1352,  1359,  1369,  1377,  1385,  1394,  1401,  1408,  1418,  1426,
    1436,  1451,  1452,  1457,  1462,  1469,  1473,  1477,  1481,  1487,
    1489,  1491,  1493,  1497,  1498,  1499,  1500,  1501,  1502,  1503,
    1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,
    1514,  1515,  1516,  1517,  1518,  1521,  1522,  1523,  1525,  1527,
    1529,  1531,  1533,  1536,  1538,  1540,  1542,  1544,  1547,  1550,
    1554,  1557,  1560,  1563,  1565,  1569,  1572,  1575,  1578,  1581,
    1584,  1586,  1588,  1590,  1592,  1594,  1596,  1598,  1600,  1604,
    1607,  1608,  1609,  1610,  1611,  1612,  1613,  1614,  1619,  1624,
    1626,  1629,  1631,  1634,  1635,  1638,  1639,  1642,  1643,  1644,
    1645,  1646,  1647,  1648,  1649,  1650,  1651,  1652,  1653,  1654,
    1660,  1666,  1672,  1675,  1676,  1679,  1680,  1681,  1684,  1686,
    1689,  1690,  1691,  1694,  1695,  1698,  1699,  1700,  1703,  1704,
    1705,  1708,  1709,  1710,  1711,  1713,  1715,  1717,  1720,  1721,
    1722,  1723,  1724,  1729,  1732,  1733,  1736,  1737,  1740,  1741,
    1742,  1743,  1744,  1745,  1746,  1747,  1748,  1749,  1750,  1751,
    1752,  1754,  1756,  1762,  1767,  1768,  1770,  1772,  1777,  1779,
    1785,  1791,  1795,  1799,  1804,  1811,  1816,  1822,  1828,  1835,
    1842,  1847,  1852,  1859,  1864,  1869,  1871,  1873,  1875,  1877,
    1879,  1881,  1883,  1886,  1887,  1888,  1891,  1892,  1893,  1894,
    1903,  1904,  1905,  1906,  1907,  1908,  1909,  1910,  1911,  1915,
    1919,  1922,  1924,  1926,  1928,  1930,  1932,  1934,  1936,  1938,
    1940,  1942,  1944,  1946,  1948,  1949,  1953,  1957,  1961,  1962,
    1964,  1966,  1969,  1970,  1972,  1974,  1977,  1978,  1979,  1981,
    1984,  1985
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
  "catch_when_list", "when_list", "sub_decl", "scalar_methcall",
  "sub_body", "method_body", "pkg_name", "class_decl", "role_decl",
  "module_decl", "is_clauses", "class_body_list", "grammar_decl",
  "grammar_body_list", "named_arg_list", "pair_list", "param_list",
  "block", "closure", "expr", "tern_expr", "or_expr", "and_expr",
  "cmp_expr", "divis_expr", "jct_expr", "dor_expr", "range_expr",
  "add_expr", "repl_expr", "addsub_expr", "mul_expr", "unary_expr",
  "pow_expr", "scalar_list", "meth_name", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-898)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-491)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -898,    95,  2530,  -898,  -898,  -898,  -898,  -898,   798,   -65,
     -51,   -74,     2,  -898,  -898,  5525,  2628,  -898,  -898,  -898,
     819,  2706,   237,  4511,  4589,  4667,  4745,  4823,    13,    26,
    4823,  2784,  2862,   271,   227,  4823,  4823,    73,   198,  4901,
    4979,   130,   -55,    62,    71,   173,   173,  5057,  4823,   130,
     181,  4823,   838,   838,   202,  5525,  -898,  -898,   838,   838,
     361,   374,  5525,  5525,  5525,  -898,  2940,  3019,   332,  -898,
    -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,   556,
    -898,  -898,  -898,  -898,   751,   667,   403,   637,   324,   539,
     578,   747,   320,  -898,     3,   799,   840,   476,  -898,  -898,
     412,   344,   355,   500,  -898,  -898,  4823,  4823,  4823,  4823,
    4823,  4823,   534,  3097,  5135,  3176,   571,  4823,  4823,   430,
     100,  1193,   348,   472,    14,  -898,   530,   130,  3254,  -898,
    -898,   842,     5,  -898,   540,  3333,  3411,  -898,  3489,  -898,
      42,   -62,    -1,   196,   887,   600,   402,   615,  3254,   550,
    3254,    24,  3254,   130,  3254,   130,   210,    85,   333,  -898,
    -898,  -898,   258,  -898,   121,  -898,   299,   526,   532,   594,
     548,   549,   145,   156,  3254,   130,  3254,   130,   282,  3568,
    -898,  4823,  4823,  -898,  4823,  4823,  -898,  4823,  3646,  4823,
    3724,  4823,  -898,  -898,   646,   161,  -898,  -898,  -898,  -898,
    -898,   551,   609,    18,  -898,   659,   678,  -898,  -898,  -898,
    -898,  -898,   905,  -898,   742,   790,  -898,   748,   641,  1712,
    4823,  4823,  4823,  4823,  4823,  4823,  4823,  4823,  -898,  4823,
    4823,  4823,  4823,  4823,  4823,  4823,  4823,  -898,  4823,  5525,
    5525,  5525,  5525,  5525,  5525,  5525,  5525,  5525,  5525,  5525,
    5525,  5525,  5525,  5525,   914,  5525,  5525,  5525,  5525,  5525,
    5525,  5525,  5525,  5525,  5525,  5525,  5525,  5525,  5525,  5525,
    5525,  5525,  5525,   941,   762,   837,   702,   723,   730,   759,
     788,   821,   188,  -898,   846,  3254,   322,   860,   761,   809,
     823,   826,  -898,   834,   966,   867,   -43,  -898,  1007,   853,
     855,  3176,  1014,  4823,  4823,  4823,  4823,  4823,  4823,  4823,
    4823,  3802,  -898,  -898,   916,  -898,   922,   938,   742,   861,
    -898,  4823,  4823,  -898,  5213,  -898,  5291,  -898,   212,   292,
     311,   363,   910,  1010,  -898,   864,   870,  4823,  4823,  4823,
    4823,  4823,  4823,  4823,  -898,  4823,   879,  4823,  4823,  -898,
    4823,   881,   994,   892,  -898,   615,  4823,  -898,  5525,  5525,
       6,  -898,  -898,  -898,  4823,  4823,  4823,  -898,  4823,  4823,
    -898,  4823,  4823,   980,  4823,  -898,  1076,  4823,  1146,  4823,
     896,  1133,   898,  -898,  4823,  4823,  1153,  1028,  1029,  1030,
    1031,  1032,  1026,  4823,  -898,  -898,  -898,   130,  4823,   130,
     169,    12,  -898,    29,  1033,  1034,  -898,  -898,  -898,  3880,
    1050,  1171,  3959,  -898,  -898,   954,   204,   231,  1060,  4511,
    4589,  4823,  2233,    48,   328,  -898,   245,  1047,  1048,  1049,
    1051,  1052,  1053,  1054,  1055,  1059,  1064,  1065,  1066,  1072,
    1080,  1081,  1082,  -898,   324,  1085,   539,  1102,  1102,  1102,
    1102,  1102,  1102,  1102,  1102,  1102,  1102,  1102,  1102,  -898,
    -898,  -898,   747,  -898,  -898,  -898,  1091,  1091,   799,   840,
     840,   476,   476,  -898,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  -898,  -898,  1084,  1208,  1086,  -898,  4037,  -898,  -898,
    -898,  -898,  -898,  -898,  4823,  4116,  -898,   901,  -898,  4823,
    4823,  4823,  4823,  1103,   131,   246,  -898,  4195,  -898,  1345,
    4823,  1087,  -898,    -4,  -898,  1570,   800,  1104,  1088,  -898,
    -898,  -898,  -898,  -898,  -898,  -898,  -898,   801,  -898,  -898,
    -898,  1092,  -898,  1094,  3254,   383,  1148,   -15,  4274,   498,
    1098,  4823,  -898,  5369,  -898,  4823,  -898,    52,  -898,  4823,
    4823,  1113,  3880,  1100,  1105,  1106,  1116,  1118,  1119,  1120,
     565,  3880,  1121,  1122,   691,   130,   -10,  5447,  -898,   130,
     575,   318,   -45,   112,    -5,  -898,    21,  -898,  -898,  -898,
    1095,   907,  1853,   692,  1123,  1124,  1125,  1128,  1129,  1130,
    -898,  1099,   287,  1126,  1127,  1142,  1131,   130,   130,   130,
    1135,  1137,  1147,  4823,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,   130,  1132,  4823,   130,  -898,  1229,  1263,  -898,   216,
    1095,   909,    57,   501,   459,  1994,  -898,   911,  4823,  -898,
    -898,   805,  1264,  3176,  4823,  4823,   358,    22,   338,  -898,
     350,  4823,  4823,  -898,  4823,  4823,  -898,  4823,  4823,  4823,
    4823,  4823,  4823,  4823,  4823,  -898,  -898,  -898,  -898,  -898,
    -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  -898,  5525,  4353,  -898,  1491,  1143,   915,  1144,  -898,
     928,  3880,   693,  1140,  1141,   815,  4823,  -898,  -898,  4823,
    -898,  -898,  4823,  -898,   930,  -898,   932,   934,  -898,  4823,
    1199,  1270,   957,  -898,   978,   983,  -898,  -898,  -898,   961,
    -898,  4823,  4823,  4823,   963,  -898,    39,  -898,  1149,  3254,
     721,  1150,  1095,   965,  1151,  1152,  4823,   968,  -898,  -898,
    -898,  -898,  -898,  -898,  -898,  -898,   971,  -898,  -898,  -898,
     996,  -898,  -898,  3254,   130,  -898,  -898,  1279,  -898,  1280,
    1281,  1278,  4823,  1172,  1283,  1285,  -898,  1095,   545,  4511,
    4589,  2311,  -898,   393,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  4823,   130,  -898,  -898,  -898,  1160,  1161,  1271,  -898,
    -898,  -898,  -898,  4823,  1165,  -898,  -898,   130,  1159,  -898,
    -898,   288,   717,   482,  1241,  -898,  -898,  1095,  1095,   973,
     976,  1290,  1291,  1292,  -898,  -898,  -898,  1173,  -898,   250,
     822,  1164,   308,  4823,  4823,  4823,  4823,  4823,  4823,  -898,
    -898,  -898,  -898,   377,   389,   401,   407,   443,   460,   463,
     477,   483,   488,   502,   503,  -898,  -898,   979,  -898,   982,
     984,  -898,  1174,  -898,  -898,   986,  -898,  -898,  -898,  -898,
    1175,  1176,  1177,  -898,  -898,  -898,  1170,  4823,  1178,  -898,
    3880,   722,  -898,  -898,  1181,  1230,  1234,  -898,   988,  -898,
    4823,  -898,  -898,  1095,  -898,  -898,   749,  1183,  1184,   -10,
    -898,   990,  1101,   130,   130,   130,  -898,  -898,  4823,  -898,
    -898,  -898,    35,  -898,   281,  -898,  -898,  -898,   395,   268,
    -898,   367,  4823,  4823,  4823,  4823,  4823,  4823,  4823,  4823,
    -898,   130,  1182,  -898,  -898,   130,  1185,  4823,  -898,  -898,
    -898,   410,   473,    37,    79,   552,  1188,  1190,   941,  -898,
    -898,  1095,  1095,  1320,  1321,  1322,  -898,  4823,  4432,  1210,
     369,  -898,   504,   508,   541,   542,   566,   567,  -898,  -898,
    -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  -898,  -898,  -898,  1198,  -898,  -898,  -898,  -898,  -898,
    4823,   997,  -898,  -898,  4823,  4823,  3880,   754,  -898,  -898,
    4823,  -898,  -898,  -898,  -898,   130,   130,  -898,  -898,  -898,
    -898,  -898,  1324,  4823,  1214,  1329,  1330,  4823,  4823,  4823,
    -898,  -898,  -898,   570,   582,   583,   586,   587,   592,   596,
     602,  -898,  -898,  -898,  4823,  1203,  -898,    70,  1265,  -898,
     183,  -898,  1328,  1331,  1216,  4823,  -898,  1339,  1340,  1224,
    4823,  -898,   152,   178,  1232,  1233,  -898,  -898,   673,  -898,
    -898,  -898,  -898,  -898,   604,  1201,   999,  4823,  4823,  -898,
    -898,  -898,  -898,  -898,  -898,  -898,  1235,  1236,  -898,  -898,
    1004,  -898,   755,  1115,  -898,  -898,  -898,  4823,  -898,  -898,
     613,   618,   619,  -898,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  1238,   130,  1228,  1008,  1237,  2135,  1228,  1011,  1239,
    1240,  -898,  1245,  1246,  1247,  -898,  1248,  1357,  1363,  1250,
    4823,  -898,  1373,  1374,  1253,  4823,  -898,  -898,  -898,   195,
    -898,  -898,  -898,  1249,   622,   623,  -898,  -898,  1254,  -898,
     130,  -898,  -898,  -898,  -898,  -898,  4823,  -898,  -898,  1228,
    -898,  4511,  4589,  2389,  -898,   418,  -898,  1228,  -898,  -898,
    -898,  -898,  -898,  -898,  1257,  1258,  -898,  1259,  1260,  1261,
    -898,  1262,  1228,  1013,  -898,  -898,  -898,  -898,  -898,  1266,
    -898,   424,   337,  -898,   381,  4823,  4823,  4823,  4823,  4823,
    4823,  4823,  4823,  -898,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  -898,  1228,   130,  4823,  4823,  4823,  -898,  -898,  -898,
     634,   639,   642,   647,   651,   656,   657,   668,  -898,  -898,
     669,   670,   671,  -898,  -898,  -898,  -898,  -898,  -898,  -898,
    -898,  -898,  -898,  -898
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   446,   447,   448,   450,   451,   456,
     457,   475,   474,   476,   477,     0,     0,   458,   459,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   449,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     3,     4,
     103,   104,   114,   115,   116,   117,   105,   106,   124,     0,
     125,   126,   128,   127,   113,     0,   340,   342,   344,   346,
     362,   364,   367,   369,   372,   374,   377,   380,   387,   393,
     395,   411,   442,     0,   454,   455,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   456,   457,   475,   474,     0,     0,   490,
     391,   451,   456,   482,     0,     0,     0,    38,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   491,
     338,   337,     0,    52,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   151,     0,
     154,     0,     0,   118,     0,     0,   119,     0,     0,     0,
       3,     0,   439,   441,   107,     3,   112,   435,   218,   219,
     223,     0,     0,     0,   392,     0,     0,   452,   453,   390,
     389,   388,   451,   486,     0,     0,   478,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,   333,   334,   335,
     336,   330,     0,   418,     0,     0,   331,     0,     0,     0,
       0,     0,    42,   423,     0,   422,   474,   414,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,    29,     0,    31,     0,   486,     0,     0,
      41,   445,     0,     6,     0,    10,     0,    11,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,    46,
       0,     0,   132,     0,   144,     0,     0,   163,     0,     0,
       0,     3,   172,    49,     0,     0,     0,    51,     0,     0,
      55,     0,     0,     0,     0,   168,     0,     0,     0,     0,
       0,   147,     0,   150,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,   438,   440,     0,     0,     0,
       0,     0,   226,     0,     0,     0,   261,     3,   487,     0,
       0,     0,     0,   479,   296,   451,   456,   457,   475,     0,
       0,     0,     0,     0,     0,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   339,   343,     0,   345,   347,   348,   351,
     352,   349,   350,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   363,   365,   366,   368,   370,   371,   373,   375,
     376,   378,   379,   384,   385,   386,   381,   382,   383,   394,
     398,   410,   403,   404,   405,   406,   409,   407,   408,   401,
     402,   399,   400,   434,     0,   431,    62,     0,    96,    97,
      98,    99,   100,    59,     0,     0,   417,     0,    15,     0,
       0,     0,     0,   461,   466,   467,    64,     0,   426,     0,
       0,     0,   415,     0,   413,   445,     0,     0,     0,   332,
     333,   334,   335,   336,   330,   331,   484,     0,    30,    32,
      40,     0,   444,     0,     0,     0,   448,   474,     0,     0,
       0,     0,    26,     0,    27,     0,    28,     0,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,   135,   487,
       0,     0,   370,   371,   273,   276,     0,   289,   291,   293,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,   412,     0,     0,     0,     0,     0,   487,     0,   487,
       0,     0,     0,     0,   120,   121,   122,   123,   329,   437,
     108,     0,     0,     0,     0,   109,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,   488,     0,     0,   397,
     480,     0,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,   323,     0,     0,   324,     0,     0,     0,
       0,     0,     0,     0,     0,   297,    66,    68,    70,    73,
      71,    72,    67,    69,    79,    81,    83,    86,    84,    85,
      80,    82,     0,     0,   427,     0,     0,     0,     0,   182,
       0,     0,     0,     0,     0,     0,     0,   471,   473,     0,
     470,   472,     0,   425,     0,   421,     0,     0,   265,     0,
       0,     0,     0,   461,   466,   467,   485,    39,     5,     0,
      12,     0,     0,     0,     0,    19,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    90,
      89,    93,    91,    92,    88,    44,     0,    94,    95,    47,
     129,   134,   133,     0,     0,   143,   160,     0,   162,     0,
       0,     0,     0,   278,     0,     0,   171,     0,     0,     0,
       0,     0,   183,     0,    50,    53,    54,    57,    58,    34,
      33,     0,     0,   164,   468,   469,     0,     0,   145,   148,
     149,   152,   153,     0,     0,   166,   111,     0,     0,   224,
     225,     0,     0,     0,     0,   221,   180,     0,     0,     0,
       0,     0,     0,     0,   260,   222,   489,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
     313,   320,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,   433,     0,   430,     0,
       0,    61,     0,    63,   181,     0,    16,   464,   463,   462,
       0,     0,     0,   424,   419,   420,     0,     0,     0,   416,
       0,     0,   270,   269,     0,     0,     0,    21,     0,    22,
       0,    25,   174,     0,    36,    35,     0,   489,   489,     0,
     136,     0,   137,     0,     0,     0,   274,   285,     0,   279,
     280,   170,   281,   277,     0,   290,   292,   294,     0,     0,
     186,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,     0,     0,    77,    78,     0,     0,     0,   167,   110,
     220,     0,     0,     0,     0,     0,     0,     0,     0,   179,
     177,     0,     0,     0,     0,     0,     9,     0,     0,   461,
     467,   317,     0,     0,     0,     0,     0,     0,   325,   326,
     327,   328,   298,   300,   302,   305,   303,   304,   299,   301,
     432,   429,   428,    60,   489,    74,    75,    76,   266,   267,
       0,     0,    13,    20,     0,     0,     0,     0,   173,     7,
       0,    45,    48,   131,   130,   487,     0,   141,   161,   158,
     159,   287,     0,     0,   282,     0,     0,     0,     0,     0,
     187,   188,   185,     0,     0,     0,     0,     0,     0,     0,
       0,   169,   165,   146,     0,     0,   157,     0,     3,   256,
       0,   253,     0,     0,     0,     0,   232,     0,     0,     0,
       0,   227,     0,     0,     0,     0,   228,   229,     0,   176,
     178,   262,   263,   264,     0,   182,     0,     0,     0,   307,
     309,   312,   310,   311,   308,    17,     0,   489,   272,   271,
       0,    23,     0,   138,   139,   275,   286,     0,   283,   284,
       0,     0,     0,   189,   191,   193,   196,   194,   195,   190,
     192,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,     0,     0,   244,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,   233,   230,   231,     0,
     259,   316,   315,   181,     0,     0,   268,    14,   489,     8,
       0,   142,   288,   197,   199,   198,     0,   156,   255,     0,
     201,     0,     0,     0,   200,     0,   252,     0,   241,   242,
     236,   239,   240,   235,     0,     0,   245,     0,     0,     0,
     246,     0,     0,     0,   314,   318,   319,    24,   140,     0,
     254,     0,     0,   204,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   202,   251,   249,   250,   238,   247,   248,
     237,   258,     0,     0,     0,     0,     0,   205,   206,   203,
       0,     0,     0,     0,     0,     0,     0,     0,   257,   155,
       0,     0,     0,   207,   209,   211,   214,   212,   213,   208,
     210,   215,   217,   216
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -898,  -898,     1,  -898,  -572,  -745,  -898,  -898,  -898,  -898,
    -898,   267,  1377,  -898,  -898,  -898,  -898,  -898,  -319,  -897,
     518,  -898,  -898,  -898,  -898,   779,  -898,  -898,  -510,   862,
    -395,   351,  1107,    -2,  -237,  -898,  1169,  1168,   835,  1156,
    -898,   680,   -16,  1155,   893,   894,    -8,  -898,  -141,  -272,
    -898,  -898,   171,  -898
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   219,    69,    70,   578,    71,    72,    73,    74,
      75,  1025,    76,    77,   400,   602,    78,    79,   362,  1029,
     200,    80,    81,    82,   401,   629,    83,   634,   299,   549,
     591,   129,   188,   139,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   215,   493,
     100,   101,   140,   102
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      85,     2,   495,   445,   751,   890,   335,   130,   631,   706,
     710,   157,   558,   119,   134,   712,   584,   585,    25,   761,
     586,   149,   151,   153,   155,   156,   626,   158,   162,   164,
     166,   763,   404,   170,   171,  1031,   627,   175,   177,   584,
     585,   520,   759,   586,   875,   192,   193,   204,   118,   197,
     347,  1032,   347,   876,   209,   210,   211,   114,    27,  1002,
     322,  1033,   584,   585,   214,   217,   586,   584,   585,   723,
     348,   586,   348,   116,   323,   115,   651,   587,   588,   589,
     584,   585,   179,   261,   586,   172,   405,    68,   259,   260,
     181,   117,   120,  1037,   121,     3,   652,   764,   765,   184,
     587,   588,   589,  1038,   276,   277,   278,   279,   280,   281,
     182,  1034,   286,   288,   293,   290,   291,   762,   294,   185,
     120,   324,   121,   587,   588,   589,   214,   310,   587,   588,
     589,   261,    68,   314,   316,   325,   318,   120,   302,   121,
     711,   587,   588,   589,   590,   301,   336,   997,   346,   365,
     351,  1120,   353,  1039,   628,    68,   303,  1003,   349,  1035,
     349,   350,   733,   350,   295,   830,  1107,   630,    68,   366,
     358,   359,   380,  1036,   382,   849,  1108,   387,   320,   388,
     389,   321,   390,   391,   183,   392,   394,   395,   392,   396,
     732,   653,  1112,   584,   585,   808,  1138,   586,   183,   760,
    1146,  1040,  1113,   398,   399,   584,   585,   186,  1093,   586,
     173,   623,   624,   261,   580,  1041,   202,   426,   427,   428,
     429,   430,   431,   432,   433,   434,  1109,   435,   436,   437,
     438,   439,   440,   441,   442,   203,   443,   809,   810,   218,
     261,   169,  1170,   466,   467,   697,   698,   141,   142,   143,
    1184,   144,  1114,   699,   587,   588,   589,   367,   473,   474,
     475,   476,   477,   478,   479,  1191,   587,   588,   589,   376,
     145,   766,    68,   657,  1110,   146,   658,   659,   802,   803,
     378,   167,   804,   507,   284,   168,   660,   377,  1111,   661,
     662,  1004,   300,   663,   664,  1208,   347,   355,   379,   526,
    1115,   528,   529,   530,   531,   532,   533,   534,   535,   319,
     504,   806,   625,   384,  1116,   187,   348,   993,   326,   542,
     543,  1097,   545,   195,   550,   505,   114,   368,  1131,   781,
     782,   385,   327,  1162,   551,   563,   564,   565,   566,   567,
     568,   569,   582,   583,   643,   572,   573,   369,   552,   356,
     802,   803,    68,    84,   804,   116,   654,  1005,  1006,   805,
     700,   701,   592,   594,   595,   347,   596,   597,   702,   598,
     599,   207,   947,   644,   147,   604,   655,   606,   365,   159,
     161,   237,   610,   611,   208,   348,   823,   948,   665,   338,
     824,   619,   178,   180,   363,   365,   621,   364,   366,   825,
     194,   196,   826,   827,   349,   757,   828,   350,   635,   365,
     218,  1011,   332,   882,   553,   366,   333,   646,   647,   648,
     650,   912,   241,  1007,   913,   914,   338,  1008,   554,   366,
     783,   930,   258,   555,   915,   370,   340,   916,   917,   341,
     342,   918,   919,  1009,   516,   845,  1175,   556,   901,  1176,
    1177,   951,  1194,   104,   105,   338,  1195,   321,   508,  1178,
      68,   509,  1179,  1180,   186,   340,  1181,  1182,   341,   342,
     360,   656,  1196,   349,   363,   361,   350,   364,   159,   273,
    1198,   831,   537,   700,   701,   113,   367,   238,   939,   940,
     274,  1058,   933,   832,   344,   934,   935,   345,   936,   937,
     557,   829,   688,   367,   352,   361,   354,   357,   693,   694,
    1012,   584,   585,   614,   275,   586,   570,   367,   708,   720,
     958,   574,   721,   542,  1199,   615,   381,   581,   383,   237,
     932,   344,   959,   272,   345,   593,   920,   616,  1010,   811,
     812,   813,   719,   617,   960,   601,   214,  1027,   282,   728,
     961,   730,  1028,   731,   237,   902,   903,   734,   735,   904,
     344,  1183,  1042,   345,   988,  1043,   292,  1197,  1044,  1045,
      84,   201,   587,   588,   589,   754,   205,   206,   337,   674,
     637,   338,   339,   641,   220,   289,   962,   221,   222,   266,
     773,   340,   267,   268,   341,   342,   675,   223,   343,   676,
     224,   225,   814,   963,   226,   227,   964,   269,   270,   271,
    1030,   794,   301,   677,   331,  1028,   905,   906,   907,   678,
     965,   797,  1049,  1050,   679,   334,   966,   242,   243,   244,
     245,   967,  1094,    85,   725,  1098,   817,   726,   680,   681,
     738,   820,   821,   822,   740,   968,   969,  1059,   371,   833,
     834,  1060,   835,   836,   372,   837,   838,   839,   840,   841,
     842,   843,   844,   246,   247,   120,  1048,   121,   687,   373,
     248,   249,   250,   251,   252,   253,   690,   741,   742,   311,
     692,   312,   254,   695,  1061,  1062,   344,   374,   704,   345,
     707,   375,   228,   402,   860,   229,   255,   861,   230,   231,
     862,   745,   743,   744,   321,   397,   674,   866,   232,  1063,
    1064,   233,   234,  1083,   411,   235,   236,    68,   675,   676,
     872,   873,   677,   678,  1163,  1084,  1085,   878,   679,  1086,
    1087,   480,   680,   737,   886,  1088,   239,   240,   681,  1089,
     853,   481,   746,   482,   483,  1090,   403,  1121,   620,   738,
     622,   891,   484,   485,   740,   744,  1133,   486,   975,   977,
     897,  1134,  1135,   487,   488,  1165,  1166,   908,   909,   911,
     674,   489,   490,   491,   492,   675,   480,  1213,   676,   921,
     494,   931,  1214,   677,   414,  1215,   481,   678,   482,   483,
    1216,   926,   679,   680,  1217,  -490,  -490,   484,   485,  1218,
    1219,   406,   486,   237,   681,   738,   740,   744,   487,   488,
    1119,  1220,  1221,  1222,  1223,  1028,   489,   490,   491,   492,
     407,   952,   953,   954,   955,   956,   957,   749,   774,   856,
     321,   321,   321,   135,   136,  -490,  -490,  -490,   498,  -490,
    -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,
    -490,  -490,   198,   199,   847,  -490,   850,   879,   982,   499,
     880,   321,   855,  -490,  -490,   979,   500,  -490,  -490,  -490,
    -490,  -490,  -490,   256,   257,  -490,  -490,  -490,  -490,  -490,
     408,   409,  -490,  -490,  -490,   989,  -490,   412,   990,   413,
    1071,  1129,   871,   321,   321,   501,  1001,   328,   329,   330,
     512,   103,   513,   104,   105,   106,   107,   108,   109,   110,
    1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,   262,   263,
     111,   459,   460,   461,   502,  1026,   750,   752,   410,   411,
     755,   756,   758,   112,   514,   113,   463,   464,   465,   512,
     321,   713,   716,    84,   321,  1054,   818,   104,   105,   304,
     305,   306,   307,   308,   321,   480,   859,   503,   788,   789,
     790,   512,   516,   949,   309,   481,   515,   482,   483,   264,
     265,   517,   795,   496,   497,   798,   484,   485,  1066,   113,
     518,   486,  1068,  1069,   506,   321,    84,   487,   488,   510,
     511,   522,   523,   524,   525,   489,   490,   491,   492,   541,
     321,  1076,   561,   411,   519,  1080,  1081,  1082,   408,   562,
     104,   105,   304,   305,   306,   307,   308,   408,   571,   575,
     409,   521,  1091,   576,   577,   889,   577,   309,   527,  1096,
     579,   409,   559,  1102,   607,   409,   609,   409,  1106,   408,
     691,   981,   113,  -396,  -396,   767,   768,   807,   768,   816,
     321,   987,   538,   852,   321,  1124,  1125,   103,   539,   104,
     105,   106,   107,   108,   109,   110,   854,   321,   863,   321,
     864,   523,   865,   321,   540,  1132,   111,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   642,
     603,   113,   697,   698,  1145,   869,   523,   700,   701,   408,
     870,   874,   726,   883,   768,   892,   887,   321,  1157,   888,
     321,   941,   768,  1161,   942,   768,   600,   970,   321,  1056,
     971,   523,   972,   321,   974,   321,   408,   986,   995,   409,
     996,   577,   560,   922,  1026,  1067,   321,  1123,   321,  1171,
    1172,  1174,  1128,   321,  1130,   577,  1139,   768,   928,  1147,
     768,  1192,   768,   189,   191,   469,   470,  1070,   471,   472,
     605,  1072,   608,   612,   613,   614,   615,   616,   617,   618,
     632,   633,   638,  1200,  1201,  1202,  1203,  1204,  1205,  1206,
    1207,   639,   645,   666,   667,   668,   682,   669,   670,   671,
     672,   673,  1210,  1211,  1212,   674,     4,     5,     6,     7,
     675,   676,   677,   131,   132,   124,   125,   296,   678,    13,
      14,    15,    16,    17,    18,    19,   679,   680,   681,   261,
     255,   683,   684,   685,   709,   696,   127,    29,   717,   714,
     718,   715,   722,    35,   727,   736,   738,   361,   321,    37,
     994,   739,   740,   799,   998,   999,  1000,    45,    46,    47,
      48,   784,   741,    51,   742,   743,   744,   747,   748,   775,
     776,   777,    55,    56,   778,   779,   780,   786,    57,   793,
     785,   791,  1021,   792,   787,   796,  1023,   800,   819,   851,
     853,   857,   858,   867,   868,   877,   881,   884,   885,   893,
     894,   895,   896,   899,   898,   900,   923,   924,    60,    61,
     925,   927,   929,   938,   943,   944,   945,   950,   978,   946,
     973,   975,   976,   977,   984,   980,    62,   983,   985,   991,
     992,  1024,    63,    64,  1046,  1022,  1047,  1051,  1052,  1053,
     128,   297,  1057,    67,  1065,    68,  1077,   298,  1075,  1078,
    1079,  1092,  1099,  1095,  1122,  1100,  1073,  1074,     4,     5,
       6,     7,  1101,  1103,  1104,   131,   132,   124,   125,   296,
    1105,    13,    14,    15,    16,    17,    18,    19,  1117,  1118,
    1028,  1154,  1127,  1126,  1136,  1148,  1149,  1155,   127,    29,
    1140,  1150,  1151,  1152,  1153,    35,  1156,  1158,  1159,  1160,
    1167,    37,  1164,  1185,  1186,  1187,  1188,  1189,  1190,    45,
      46,    47,    48,  1169,  1193,    51,   160,   801,   444,   446,
     724,   462,     0,     0,    55,    56,   468,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1137,     0,     0,     0,    84,     0,     0,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,    63,    64,     0,     0,     0,     0,
       0,  1168,   128,   705,     0,    67,     0,    68,     0,   298,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   296,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,  1209,    45,    46,    47,    48,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     131,   132,   124,   125,   296,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,    60,    61,     0,     0,
       0,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,    62,     0,    37,     0,     0,     0,
      63,    64,     0,     0,    45,    46,    47,    48,   128,   848,
      51,    67,     0,    68,     0,   298,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,   128,     0,     0,
      67,     0,    68,     0,   298,     4,     5,     6,     7,     0,
       0,     0,   415,   416,   417,   418,    12,     0,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,   419,   420,
      25,     0,     0,    26,    27,    28,    29,   421,   422,    32,
      33,    34,    35,    36,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,   423,   424,    45,    46,    47,    48,
      49,    50,    51,    52,     0,     0,     0,    53,    54,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,    58,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,    65,    66,
       0,     0,    67,     0,    68,   425,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,     0,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,   769,
     770,    25,     0,     0,    26,    27,    28,    29,    30,   771,
      32,    33,    34,    35,    36,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,     0,     0,    53,    54,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,    58,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,    63,    64,     0,     0,     0,     0,     0,    65,
      66,     0,     0,    67,     0,    68,   772,     4,     5,     6,
       7,     0,     0,     0,     8,     9,    10,    11,    12,     0,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,     0,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     0,     0,     0,    53,
      54,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,    58,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
      65,    66,     0,     0,    67,     0,    68,   815,     4,     5,
       6,     7,     0,     0,     0,     8,     9,    10,    11,    12,
       0,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,  1141,  1142,    25,     0,     0,    26,    27,    28,    29,
      30,  1143,    32,    33,    34,    35,    36,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     0,     0,     0,
      53,    54,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,    58,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      60,    61,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    62,     0,
       0,     0,     0,     0,    63,    64,   127,    29,     0,     0,
       0,    65,    66,    35,     0,    67,     0,    68,  1144,    37,
       0,     0,     0,     0,     0,     0,     0,    45,    46,    47,
      48,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,   163,
     128,    51,     0,    67,     0,    68,   649,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,   163,   128,    51,
       0,    67,     0,    68,   910,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,    63,    64,
       0,     0,     0,     0,     0,   163,   128,     0,     0,    67,
       0,    68,  1173,     4,     5,     6,     7,     0,     0,     0,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     0,     0,     0,    53,    54,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,    58,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,    60,    61,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    62,     0,     0,     0,     0,     0,    63,
      64,   127,    29,     0,     0,     0,    65,    66,    35,     0,
      67,     0,    68,     0,    37,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    62,    37,     0,     0,     0,     0,    63,    64,     0,
      45,    46,    47,    48,     0,   128,    51,     0,    67,   133,
      68,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    62,
      37,     0,     0,     0,     0,    63,    64,     0,    45,    46,
      47,    48,   137,   138,    51,     0,    67,     0,    68,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    62,    37,     0,
       0,     0,     0,    63,    64,     0,    45,    46,    47,    48,
     163,   128,    51,     0,    67,     0,    68,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     212,   132,   124,   125,   126,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,   127,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    62,    37,     0,     0,     0,
       0,    63,    64,     0,    45,    46,    47,    48,   165,   128,
      51,     0,    67,     0,    68,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,    60,    61,     0,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    62,     0,    37,     0,     0,     0,    63,
      64,     0,     0,    45,    46,    47,    48,   128,   213,    51,
      67,     0,    68,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   128,    51,     0,    67,
     216,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,    60,    61,     0,     0,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
      62,     0,    37,     0,     0,     0,    63,    64,     0,     0,
      45,    46,    47,    48,   128,   283,    51,    67,     0,    68,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    62,
      37,     0,     0,     0,     0,    63,    64,   287,    45,    46,
      47,    48,     0,   128,    51,     0,    67,     0,    68,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,    60,
      61,     0,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,    62,     0,    37,
       0,     0,     0,    63,    64,     0,     0,    45,    46,    47,
      48,   128,   213,    51,    67,     0,    68,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,   313,
     128,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,   315,   128,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   131,   132,
     124,   125,   126,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,   386,    60,    61,     0,     0,     0,     0,
       0,   127,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    62,     0,    37,     0,     0,     0,    63,    64,
       0,     0,    45,    46,    47,    48,   128,   317,    51,    67,
       0,    68,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    62,    37,     0,     0,     0,     0,    63,    64,     0,
      45,    46,    47,    48,     0,   128,    51,     0,    67,     0,
      68,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   131,   132,   124,   125,   126,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    62,
      37,     0,     0,     0,     0,    63,    64,     0,    45,    46,
      47,    48,     0,   128,    51,   393,    67,     0,    68,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,   218,     0,     0,     4,     5,     6,     7,     0,
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
      51,     0,    67,   536,    68,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,    60,    61,     0,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    62,     0,    37,     0,     0,     0,    63,
      64,     0,     0,    45,    46,    47,    48,   128,   636,    51,
      67,     0,    68,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   128,    51,     0,    67,
     640,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   131,   132,   124,   125,
     126,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,    60,    61,     0,     0,     0,     0,     0,   127,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
      62,     0,    37,     0,     0,     0,    63,    64,     0,     0,
      45,    46,    47,    48,   128,   686,    51,    67,     0,    68,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,    60,    61,     0,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    62,
       0,    37,     0,     0,     0,    63,    64,     0,     0,    45,
      46,    47,    48,   128,   689,    51,    67,     0,    68,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     4,     5,   546,
       7,     0,     0,     0,   131,   132,   124,   125,   547,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
      60,    61,     0,     0,     0,     0,     0,   127,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    62,     0,
      37,     0,     0,     0,    63,    64,     0,     0,    45,    46,
      47,    48,   128,   703,    51,    67,     0,    68,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,    60,
      61,     0,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,    62,     0,    37,
       0,     0,     0,    63,    64,     0,     0,    45,    46,    47,
      48,   128,   213,    51,    67,     0,    68,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   131,   132,   124,   125,   126,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,    60,    61,
       0,     0,     0,     0,     0,   127,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    62,     0,    37,     0,
       0,     0,    63,    64,     0,     0,    45,    46,    47,    48,
     128,   846,    51,    67,     0,    68,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,    60,    61,     0,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    62,     0,    37,     0,     0,
       0,    63,    64,     0,     0,    45,    46,    47,    48,   128,
    1055,    51,    67,     0,    68,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,     0,   148,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   150,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   152,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   154,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   131,   132,   124,   125,   126,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     128,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,     0,   174,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   176,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   131,   132,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   128,    51,     0,    67,     0,   190,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   131,   132,   124,   125,   126,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,   127,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    62,    37,
       0,     0,     0,     0,    63,    64,     0,    45,    46,    47,
      48,     0,   285,    51,     0,    67,     0,    68,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     4,     5,   546,     7,     0,     0,
       0,   131,   132,   124,   125,   547,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,   127,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    62,    37,     0,     0,
       0,     0,    63,    64,     0,    45,    46,    47,    48,     0,
     544,    51,     0,    67,     0,    68,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   131,
     132,   124,   125,   126,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,   127,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    62,    37,     0,     0,     0,     0,
      63,    64,     0,    45,    46,    47,    48,     0,   548,    51,
       0,    67,     0,    68,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   131,   132,   124,
     125,   126,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
     127,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    62,    37,     0,     0,     0,     0,    63,    64,
       0,    45,    46,    47,    48,     0,   729,    51,     0,    67,
       0,    68,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   124,   125,   126,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      62,    37,     0,     0,     0,     0,    63,    64,     0,    45,
      46,    47,    48,     0,   753,    51,     0,    67,     0,    68,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,    63,    64,     0,     0,     0,     0,
       0,     0,   128,     0,     0,    67,     0,    68
};

static const yytype_int16 yycheck[] =
{
       2,     0,   274,   240,   576,   750,   147,    15,   403,   519,
      14,    27,   331,    11,    16,   525,    10,    11,    28,    24,
      14,    23,    24,    25,    26,    27,    14,    14,    30,    31,
      32,    10,    14,    35,    36,   932,    24,    39,    40,    10,
      11,    84,    87,    14,     5,    47,    48,    55,   122,    51,
      28,    14,    28,    14,    62,    63,    64,   122,    32,    24,
     122,    24,    10,    11,    66,    67,    14,    10,    11,    84,
      48,    14,    48,   124,   136,   140,    28,    71,    72,    73,
      10,    11,   137,   128,    14,    12,    68,   142,    85,    86,
      28,   142,   135,    14,   137,     0,    48,    76,    77,    28,
      71,    72,    73,    24,   106,   107,   108,   109,   110,   111,
      48,    74,   114,   115,    14,   117,   118,   122,    18,    48,
     135,   122,   137,    71,    72,    73,   128,   122,    71,    72,
      73,   128,   142,   135,   136,   136,   138,   135,   124,   137,
     144,    71,    72,    73,   138,   140,   148,   892,   150,    28,
     152,  1048,   154,    74,   142,   142,   142,   122,   136,   122,
     136,   139,   557,   139,    64,   143,    14,   138,   142,    48,
      85,    86,   174,   136,   176,   685,    24,   179,   136,   181,
     182,   139,   184,   185,   136,   187,   188,   189,   190,   191,
     138,   143,    14,    10,    11,   138,  1093,    14,   136,    87,
    1097,   122,    24,    42,    43,    10,    11,   136,   138,    14,
      12,    42,    43,   128,   355,   136,    14,   219,   220,   221,
     222,   223,   224,   225,   226,   227,    74,   229,   230,   231,
     232,   233,   234,   235,   236,    33,   238,   632,   633,    78,
     128,    14,  1139,   259,   260,   114,   115,    10,    11,    12,
    1147,    14,    74,   122,    71,    72,    73,   136,   266,   267,
     268,   269,   270,   271,   272,  1162,    71,    72,    73,   124,
      33,   590,   142,    28,   122,    38,    31,    32,    62,    63,
     124,    10,    66,   285,   113,    14,    41,   142,   136,    44,
      45,    10,   121,    48,    49,  1192,    28,    87,   142,   301,
     122,   303,   304,   305,   306,   307,   308,   309,   310,   138,
     122,   630,   143,    31,   136,   142,    48,   889,   122,   321,
     322,   138,   324,   142,   326,   137,   122,    28,  1073,    42,
      43,    49,   136,   138,   122,   337,   338,   339,   340,   341,
     342,   343,   358,   359,   140,   347,   348,    48,   136,   139,
      62,    63,   142,     2,    66,   124,    28,    76,    77,   143,
     114,   115,   361,   365,   366,    28,   368,   369,   122,   371,
     372,    10,   122,   142,   137,   377,    48,   379,    28,    28,
      29,   136,   384,   385,    10,    48,    28,   137,   143,    31,
      32,   393,    41,    42,   136,    28,   398,   139,    48,    41,
      49,    50,    44,    45,   136,    87,    48,   139,   407,    28,
      78,   143,    10,   732,   122,    48,    14,   419,   420,   421,
     422,    28,    98,    28,    31,    32,    31,    32,   136,    48,
     143,   143,   112,   122,    41,   136,    41,    44,    45,    44,
      45,    48,    49,    48,   136,   682,    28,   136,   767,    31,
      32,   143,    28,   105,   106,    31,    32,   139,   136,    41,
     142,   139,    44,    45,   136,    41,    48,    49,    44,    45,
     137,   143,    48,   136,   136,   142,   139,   139,   127,   135,
     143,   143,   311,   114,   115,   137,   136,    84,   807,   808,
     135,   122,    10,   143,   136,    13,    14,   139,    16,    17,
     137,   143,   504,   136,   153,   142,   155,   156,   510,   511,
     143,    10,    11,   136,    14,    14,   345,   136,   520,   136,
     143,   350,   139,   525,   143,   136,   175,   356,   177,   136,
     802,   136,   143,   121,   139,   364,   143,   136,   143,    80,
      81,    82,   544,   136,   143,   374,   548,   137,    14,   551,
     143,   553,   142,   555,   136,    10,    11,   559,   560,    14,
     136,   143,    10,   139,   883,    13,   136,   143,    16,    17,
     219,    53,    71,    72,    73,   577,    58,    59,    28,   136,
     409,    31,    32,   412,    28,    14,   143,    31,    32,   113,
     592,    41,   116,   117,    44,    45,   136,    41,    48,   136,
      44,    45,   143,   143,    48,    49,   143,   131,   132,   133,
     137,   613,   140,   136,    14,   142,    71,    72,    73,   136,
     143,   623,   941,   942,   136,    10,   143,    88,    89,    90,
      91,   143,  1027,   635,   136,  1030,   638,   139,   136,   136,
     136,   643,   644,   645,   136,   143,   143,   143,   122,   651,
     652,   143,   654,   655,   122,   657,   658,   659,   660,   661,
     662,   663,   664,   124,   125,   135,   938,   137,   497,    75,
      92,    93,    94,    95,    96,    97,   505,   136,   136,   139,
     509,   141,   104,   512,   143,   143,   136,   139,   517,   139,
     519,   142,   136,   142,   696,    28,   118,   699,    31,    32,
     702,   136,   136,   136,   139,    59,   136,   709,    41,   143,
     143,    44,    45,   143,   139,    48,    49,   142,   136,   136,
     722,   723,   136,   136,  1119,   143,   143,   729,   136,   143,
     143,    14,   136,   562,   736,   143,    99,   100,   136,   143,
     136,    24,   571,    26,    27,   143,   137,   143,   397,   136,
     399,   753,    35,    36,   136,   136,   143,    40,   136,   136,
     762,   143,   143,    46,    47,   143,   143,   769,   770,   771,
     136,    54,    55,    56,    57,   136,    14,   143,   136,   781,
      18,    64,   143,   136,   143,   143,    24,   136,    26,    27,
     143,   793,   136,   136,   143,    44,    45,    35,    36,   143,
     143,   142,    40,   136,   136,   136,   136,   136,    46,    47,
     137,   143,   143,   143,   143,   142,    54,    55,    56,    57,
     142,   823,   824,   825,   826,   827,   828,   136,   136,   136,
     139,   139,   139,    14,    15,    84,    85,    86,   136,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,    14,    15,   683,   104,   685,   136,   136,   136,
     139,   139,   691,   112,   113,   867,   136,   116,   117,   118,
     119,   120,   121,   126,   127,   124,   125,   126,   127,   128,
     138,   139,   131,   132,   133,   136,   135,   139,   139,   141,
     136,   136,   721,   139,   139,   136,   898,    10,    11,    12,
     139,   103,   141,   105,   106,   107,   108,   109,   110,   111,
     912,   913,   914,   915,   916,   917,   918,   919,   119,   120,
     122,     7,     8,     9,   136,   927,   575,   576,   138,   139,
     579,   580,   581,   135,   125,   137,   256,   257,   258,   139,
     139,   141,   141,   592,   139,   947,   141,   105,   106,   107,
     108,   109,   110,   111,   139,    14,   141,   136,   607,   608,
     609,   139,   136,   141,   122,    24,   143,    26,    27,   129,
     130,   137,   621,   136,   137,   624,    35,    36,   980,   137,
      14,    40,   984,   985,   138,   139,   635,    46,    47,   129,
     130,   138,   139,   138,   139,    54,    55,    56,    57,   138,
     139,  1003,   138,   139,   137,  1007,  1008,  1009,   138,   139,
     105,   106,   107,   108,   109,   110,   111,   138,   139,   138,
     139,    14,  1024,    29,    30,    29,    30,   122,    14,  1028,
     138,   139,   122,  1035,   138,   139,   138,   139,  1040,   138,
     139,   870,   137,   138,   139,   138,   139,   138,   139,   138,
     139,   880,   136,   138,   139,  1057,  1058,   103,   136,   105,
     106,   107,   108,   109,   110,   111,   138,   139,   138,   139,
     138,   139,   138,   139,   136,  1077,   122,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   135,
      14,   137,   114,   115,  1096,   138,   139,   114,   115,   138,
     139,   138,   139,   138,   139,   754,   138,   139,  1110,   138,
     139,   138,   139,  1115,   138,   139,   136,   138,   139,   948,
     138,   139,   138,   139,   138,   139,   138,   139,   138,   139,
      29,    30,   122,   782,  1136,   138,   139,   138,   139,  1141,
    1142,  1143,   138,   139,    29,    30,   138,   139,   797,   138,
     139,   138,   139,    46,    47,   262,   263,   986,   264,   265,
      14,   990,    29,    10,   136,   136,   136,   136,   136,   143,
     137,   137,   122,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,    10,   122,   136,   136,   136,   101,   136,   136,   136,
     136,   136,  1194,  1195,  1196,   136,     3,     4,     5,     6,
     136,   136,   136,    10,    11,    12,    13,    14,   136,    16,
      17,    18,    19,    20,    21,    22,   136,   136,   136,   128,
     118,   137,    14,   137,   137,   122,    33,    34,   136,   125,
     136,   143,    84,    40,   136,   122,   136,   142,   139,    46,
     889,   136,   136,    14,   893,   894,   895,    54,    55,    56,
      57,   125,   136,    60,   136,   136,   136,   136,   136,   136,
     136,   136,    69,    70,   136,   136,   136,   125,    75,   122,
     143,   136,   921,   136,   143,   143,   925,    14,    14,   136,
     136,   141,   141,    84,    14,   136,   136,   136,   136,    10,
      10,    10,    14,    10,   122,    10,   136,   136,   105,   106,
      29,   136,   143,    62,    14,    14,    14,   143,   138,   136,
     136,   136,   136,   136,    84,   137,   123,   136,    84,   136,
     136,   136,   129,   130,   136,   143,   136,     7,     7,     7,
     137,   138,   122,   140,   136,   142,   122,   144,    14,    10,
      10,   138,    14,    78,   143,    14,   995,   996,     3,     4,
       5,     6,   136,    14,    14,    10,    11,    12,    13,    14,
     136,    16,    17,    18,    19,    20,    21,    22,   136,   136,
     142,    14,   136,   138,   136,   136,   136,    14,    33,    34,
     143,   136,   136,   136,   136,    40,   136,    14,    14,   136,
     136,    46,   143,   136,   136,   136,   136,   136,   136,    54,
      55,    56,    57,  1136,   138,    60,    29,   628,   239,   241,
     548,   255,    -1,    -1,    69,    70,   261,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1092,    -1,    -1,    -1,  1096,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,    -1,    -1,    -1,
      -1,  1130,   137,   138,    -1,   140,    -1,   142,    -1,   144,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,  1193,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,   123,    -1,    46,    -1,    -1,    -1,
     129,   130,    -1,    -1,    54,    55,    56,    57,   137,   138,
      60,   140,    -1,   142,    -1,   144,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
     140,    -1,   142,    -1,   144,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    -1,    -1,    65,    66,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,    -1,   140,    -1,   142,   143,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    65,    66,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,    -1,    -1,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,   142,   143,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    -1,    65,
      66,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,    -1,    -1,    -1,    -1,
     136,   137,    -1,    -1,   140,    -1,   142,   143,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
     105,   106,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,   129,   130,    33,    34,    -1,    -1,
      -1,   136,   137,    40,    -1,   140,    -1,   142,   143,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,
      -1,    -1,   129,   130,    -1,    54,    55,    56,    57,   136,
     137,    60,    -1,   140,    -1,   142,   143,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,    -1,
     129,   130,    -1,    54,    55,    56,    57,   136,   137,    60,
      -1,   140,    -1,   142,   143,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,     3,     4,     5,     6,   105,   106,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,   129,
     130,    33,    34,    -1,    -1,    -1,   136,   137,    40,    -1,
     140,    -1,   142,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,
      54,    55,    56,    57,    -1,   137,    60,    -1,   140,   141,
     142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,
      46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,    55,
      56,    57,   136,   137,    60,    -1,   140,    -1,   142,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,
      -1,    -1,    -1,   129,   130,    -1,    54,    55,    56,    57,
     136,   137,    60,    -1,   140,    -1,   142,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,
      -1,   129,   130,    -1,    54,    55,    56,    57,   136,   137,
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
      -1,    54,    55,    56,    57,    -1,   137,    60,    -1,   140,
     141,   142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
     123,    -1,    46,    -1,    -1,    -1,   129,   130,    -1,    -1,
      54,    55,    56,    57,   137,   138,    60,   140,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,
      46,    -1,    -1,    -1,    -1,   129,   130,   131,    54,    55,
      56,    57,    -1,   137,    60,    -1,   140,    -1,   142,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,   123,    -1,    46,
      -1,    -1,    -1,   129,   130,    -1,    -1,    54,    55,    56,
      57,   137,   138,    60,   140,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   123,    46,    -1,    -1,
      -1,    -1,   129,   130,    -1,    54,    55,    56,    57,   136,
     137,    60,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   123,    46,    -1,    -1,    -1,    -1,
     129,   130,    -1,    54,    55,    56,    57,   136,   137,    60,
      -1,   140,    -1,   142,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    25,   105,   106,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,   123,    -1,    46,    -1,    -1,    -1,   129,   130,
      -1,    -1,    54,    55,    56,    57,   137,   138,    60,   140,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,
      54,    55,    56,    57,    -1,   137,    60,    -1,   140,    -1,
     142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   123,
      46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,    55,
      56,    57,    -1,   137,    60,   139,   140,    -1,   142,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    78,    -1,    -1,     3,     4,     5,     6,    -1,
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
      60,    -1,   140,   141,   142,    -1,    -1,    -1,    -1,    69,
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
      -1,    54,    55,    56,    57,    -1,   137,    60,    -1,   140,
     141,   142,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
     123,    -1,    46,    -1,    -1,    -1,   129,   130,    -1,    -1,
      54,    55,    56,    57,   137,   138,    60,   140,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   123,
      -1,    46,    -1,    -1,    -1,   129,   130,    -1,    -1,    54,
      55,    56,    57,   137,   138,    60,   140,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   123,    -1,
      46,    -1,    -1,    -1,   129,   130,    -1,    -1,    54,    55,
      56,    57,   137,   138,    60,   140,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,   123,    -1,    46,
      -1,    -1,    -1,   129,   130,    -1,    -1,    54,    55,    56,
      57,   137,   138,    60,   140,    -1,   142,    -1,    -1,    -1,
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
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     123,    46,    -1,    -1,    -1,    -1,   129,   130,    -1,    54,
      55,    56,    57,    -1,   137,    60,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,   140,    -1,   142
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
     149,   151,   152,   153,   154,   155,   157,   158,   161,   162,
     166,   167,   168,   171,   176,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     195,   196,   198,   103,   105,   106,   107,   108,   109,   110,
     111,   122,   135,   137,   122,   140,   124,   142,   122,    11,
     135,   137,    10,    11,    12,    13,    14,    33,   137,   176,
     191,    10,    11,   141,   178,    14,    15,   136,   137,   178,
     197,    10,    11,    12,    14,    33,    38,   137,   137,   178,
     137,   178,   137,   178,   137,   178,   178,   187,    14,   176,
     157,   176,   178,   136,   178,   136,   178,    10,    14,    14,
     178,   178,    12,    12,   137,   178,   137,   178,   176,   137,
     176,    28,    48,   136,    28,    48,   136,   142,   177,   177,
     142,   177,   178,   178,   176,   142,   176,   178,    14,    15,
     165,   165,    14,    33,   191,   165,   165,    10,    10,   191,
     191,   191,    10,   138,   178,   193,   141,   178,    78,   147,
      28,    31,    32,    41,    44,    45,    48,    49,   136,    28,
      31,    32,    41,    44,    45,    48,    49,   136,    84,    99,
     100,    98,    88,    89,    90,    91,   124,   125,    92,    93,
      94,    95,    96,    97,   104,   118,   126,   127,   112,    85,
      86,   128,   119,   120,   129,   130,   113,   116,   117,   131,
     132,   133,   121,   135,   135,    14,   178,   178,   178,   178,
     178,   178,    14,   138,   197,   137,   178,   131,   178,    14,
     178,   178,   136,    14,    18,    64,    14,   138,   144,   173,
     197,   140,   124,   142,   107,   108,   109,   110,   111,   122,
     122,   139,   141,   136,   178,   136,   178,   138,   178,   197,
     136,   139,   122,   136,   122,   136,   122,   136,    10,    11,
      12,    14,    10,    14,    10,   193,   178,    28,    31,    32,
      41,    44,    45,    48,   136,   139,   178,    28,    48,   136,
     139,   178,   176,   178,   176,    87,   139,   176,    85,    86,
     137,   142,   163,   136,   139,    28,    48,   136,    28,    48,
     136,   122,   122,    75,   139,   142,   124,   142,   124,   142,
     178,   176,   178,   176,    31,    49,    25,   178,   178,   178,
     178,   178,   178,   139,   178,   178,   178,    59,    42,    43,
     159,   169,   142,   137,    14,    68,   142,   142,   138,   139,
     138,   139,   139,   141,   143,    10,    11,    12,    13,    26,
      27,    35,    36,    52,    53,   143,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   181,   179,   182,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,     7,
       8,     9,   184,   186,   186,   186,   187,   187,   188,   189,
     189,   190,   190,   191,   191,   191,   191,   191,   191,   191,
      14,    24,    26,    27,    35,    36,    40,    46,    47,    54,
      55,    56,    57,   194,    18,   194,   136,   137,   136,   136,
     136,   136,   136,   136,   122,   137,   138,   178,   136,   139,
     129,   130,   139,   141,   125,   143,   136,   137,    14,   137,
      84,    14,   138,   139,   138,   139,   178,    14,   178,   178,
     178,   178,   178,   178,   178,   178,   141,   197,   136,   136,
     136,   138,   178,   178,   137,   178,     5,    14,   137,   174,
     178,   122,   136,   122,   136,   122,   136,   137,   163,   122,
     122,   138,   139,   178,   178,   178,   178,   178,   178,   178,
     197,   139,   178,   178,   197,   138,    29,    30,   150,   138,
     193,   197,   187,   187,    10,    11,    14,    71,    72,    73,
     138,   175,   147,   197,   178,   178,   178,   178,   178,   178,
     136,   197,   160,    14,   178,    14,   178,   138,    29,   138,
     178,   178,    10,   136,   136,   136,   136,   136,   143,   178,
     176,   178,   176,    42,    43,   143,    14,    24,   142,   170,
     138,   175,   137,   137,   172,   147,   138,   197,   122,    10,
     141,   197,   135,   140,   142,   122,   178,   178,   178,   143,
     178,    28,    48,   143,    28,    48,   143,    28,    31,    32,
      41,    44,    45,    48,    49,   143,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   101,   137,    14,   137,   138,   197,   178,   138,
     197,   139,   197,   178,   178,   197,   122,   114,   115,   122,
     114,   115,   122,   138,   197,   138,   173,   197,   178,   137,
      14,   144,   173,   141,   125,   143,   141,   136,   136,   178,
     136,   139,    84,    84,   174,   136,   139,   136,   178,   137,
     178,   178,   138,   175,   178,   178,   122,   197,   136,   136,
     136,   136,   136,   136,   136,   136,   197,   136,   136,   136,
     176,   149,   176,   137,   178,   176,   176,    87,   176,    87,
      87,    24,   122,    10,    76,    77,   163,   138,   139,    26,
      27,    36,   143,   178,   136,   136,   136,   136,   136,   136,
     136,    42,    43,   143,   125,   143,   125,   143,   176,   176,
     176,   136,   136,   122,   178,   176,   143,   178,   176,    14,
      14,   170,    62,    63,    66,   143,   163,   138,   138,   175,
     175,    80,    81,    82,   143,   143,   138,   178,   141,    14,
     178,   178,   178,    28,    32,    41,    44,    45,    48,   143,
     143,   143,   143,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   179,   138,   197,   138,   173,
     197,   136,   138,   136,   138,   197,   136,   141,   141,   141,
     178,   178,   178,   138,   138,   138,   178,    84,    14,   138,
     139,   197,   178,   178,   138,     5,    14,   136,   178,   136,
     139,   136,   163,   138,   136,   136,   178,   138,   138,    29,
     150,   178,   176,    10,    10,    10,    14,   178,   122,    10,
      10,   163,    10,    11,    14,    71,    72,    73,   178,   178,
     143,   178,    28,    31,    32,    41,    44,    45,    48,    49,
     143,   178,   176,   136,   136,    29,   178,   136,   176,   143,
     143,    64,   194,    10,    13,    14,    16,    17,    62,   163,
     163,   138,   138,    14,    14,    14,   136,   122,   137,   141,
     143,   143,   178,   178,   178,   178,   178,   178,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     138,   138,   138,   136,   138,   136,   136,   136,   138,   178,
     137,   197,   136,   136,    84,    84,   139,   197,   163,   136,
     139,   136,   136,   149,   176,   138,    29,   150,   176,   176,
     176,   178,    24,   122,    10,    76,    77,    28,    32,    48,
     143,   143,   143,   178,   178,   178,   178,   178,   178,   178,
     178,   176,   143,   176,   136,   156,   178,   137,   142,   164,
     137,   164,    14,    24,    74,   122,   136,    14,    24,    74,
     122,   136,    10,    13,    16,    17,   136,   136,   194,   163,
     163,     7,     7,     7,   178,   138,   197,   122,   122,   143,
     143,   143,   143,   143,   143,   136,   178,   138,   178,   178,
     197,   136,   197,   176,   176,    14,   178,   122,    10,    10,
     178,   178,   178,   143,   143,   143,   143,   143,   143,   143,
     143,   178,   138,   138,   175,    78,   147,   138,   175,    14,
      14,   136,   178,    14,    14,   136,   178,    14,    24,    74,
     122,   136,    14,    24,    74,   122,   136,   136,   136,   137,
     164,   143,   143,   138,   178,   178,   138,   136,   138,   136,
      29,   150,   178,   143,   143,   143,   136,   176,   164,   138,
     143,    26,    27,    36,   143,   178,   164,   138,   136,   136,
     136,   136,   136,   136,    14,    14,   136,   178,    14,    14,
     136,   178,   138,   175,   143,   143,   143,   136,   176,   156,
     164,   178,   178,   143,   178,    28,    31,    32,    41,    44,
      45,    48,    49,   143,   164,   136,   136,   136,   136,   136,
     136,   164,   138,   138,    28,    32,    48,   143,   143,   143,
     178,   178,   178,   178,   178,   178,   178,   178,   164,   176,
     178,   178,   178,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143
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
     148,   148,   148,   148,   148,   148,   148,   148,   148,   149,
     149,   149,   149,   149,   149,   149,   149,   150,   150,   150,
     150,   150,   150,   151,   151,   152,   152,   152,   152,   153,
     153,   154,   154,   154,   155,   155,   155,   156,   157,   157,
     157,   157,   157,   157,   158,   158,   159,   159,   160,   160,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   162,   162,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   165,   165,
     166,   167,   168,   169,   169,   169,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     171,   172,   172,   172,   172,   173,   173,   173,   173,   174,
     174,   174,   174,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   177,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   179,   179,   180,   180,   181,   181,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   183,   183,   184,   184,   184,   185,   185,
     186,   186,   186,   187,   187,   188,   188,   188,   189,   189,
     189,   190,   190,   190,   190,   190,   190,   190,   191,   191,
     191,   191,   191,   191,   192,   192,   193,   193,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   195,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   197,   197,   197,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198
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
       4,     2,     1,     1,     1,     1,     1,     2,     4,     4,
       6,     5,     2,     1,     1,     1,     1,     1,     2,     2,
       4,     4,     4,     4,     1,     1,     1,     1,     1,     5,
       7,     7,     3,     5,     5,     4,     6,     3,     5,     5,
       7,     4,     6,     5,     3,     5,     7,     3,     5,     5,
       3,     2,     5,     5,     2,    12,     9,     1,     7,     7,
       5,     7,     5,     3,     5,     7,     3,     4,     0,     4,
       6,     5,     3,     7,     6,     4,     7,     6,     7,     6,
       5,     6,     5,     3,     4,     5,     4,     5,     5,     6,
       6,     6,     6,     6,     6,     6,     6,     7,     7,     7,
       3,     3,     4,     5,     4,     5,     5,     6,     6,     6,
       6,     6,     6,     6,     6,     7,     7,     7,     1,     1,
       6,     5,     5,     0,     3,     3,     0,     4,     4,     4,
       5,     5,     4,     5,     5,     6,     6,     7,     7,     6,
       6,     6,     6,     5,     5,     6,     6,     7,     7,     7,
       7,     7,     6,     4,     7,     6,     4,     8,     7,     5,
       5,     0,     4,     4,     4,     3,     5,     5,     7,     3,
       3,     5,     5,     1,     3,     5,     1,     3,     2,     3,
       3,     3,     4,     5,     5,     3,     5,     4,     6,     1,
       3,     1,     3,     1,     3,     3,     3,     4,     6,     6,
       6,     6,     6,     6,     6,     6,     5,     7,     7,     7,
       7,     7,     7,     5,     9,     8,     8,     6,     9,     9,
       5,     5,     4,     4,     4,     6,     6,     6,     6,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     3,
       1,     5,     1,     3,     1,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     3,     3,     1,     2,     2,
       2,     2,     2,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     3,     4,     6,     4,     3,     6,
       6,     5,     3,     3,     6,     5,     4,     4,     6,     6,
       5,     3,     6,     5,     3,     2,     3,     4,     3,     2,
       3,     2,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     4,     6,     6,     6,     1,     4,     4,     5,     5,
       5,     5,     5,     5,     1,     1,     1,     1,     2,     3,
       4,     5,     2,     3,     4,     5,     2,     3,     4,     5,
       1,     2
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
#line 535 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3392 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 545 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3398 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 546 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3404 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 550 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3410 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 552 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3416 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 554 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3422 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 556 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3430 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 560 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3436 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 562 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3442 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 564 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3448 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 566 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3454 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 568 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3462 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 572 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3470 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 576 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3476 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 578 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3484 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 582 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3492 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 586 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3498 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 588 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3504 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 590 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3510 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 592 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3516 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 594 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3522 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 596 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3530 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 600 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3538 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3544 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3550 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 608 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3556 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 610 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3562 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 612 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3568 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 614 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3574 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 616 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3580 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 618 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3586 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 620 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3592 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 622 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3598 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 624 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3604 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 626 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3610 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 628 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3622 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 636 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3628 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 638 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); free((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3634 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 640 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3640 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 642 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); free((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3646 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 644 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3652 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 646 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3658 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 648 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3665 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 651 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3672 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 654 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3678 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 656 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3685 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 659 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3692 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 662 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3698 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 664 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3706 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 668 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3712 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 670 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3718 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 672 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3725 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 675 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3732 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 678 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3738 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 680 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3745 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 683 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3752 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 686 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3759 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 689 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3765 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 691 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3774 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 696 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3782 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 700 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3790 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 704 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3799 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 709 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3807 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 712 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3813 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3819 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 716 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3825 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 718 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3831 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3837 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 722 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3844 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 725 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3850 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 727 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3856 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 729 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3862 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 731 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3869 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 734 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3876 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 737 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3883 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 740 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3890 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 743 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3897 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 746 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3903 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 748 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3909 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 750 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3915 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 752 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3921 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 754 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3928 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 757 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3934 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 759 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3940 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 761 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3946 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 763 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3953 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 766 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3960 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 769 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3968 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 773 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3975 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 776 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3981 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 778 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3987 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 780 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3993 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 782 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4000 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 785 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4007 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 788 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4014 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 791 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4021 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 794 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4028 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 797 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4035 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 800 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4042 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 802 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4048 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 803 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4054 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 804 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4060 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 805 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4066 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 806 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4072 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 807 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4078 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 809 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4084 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 811 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4090 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 813 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4098 "raku.tab.c"
    break;

  case 110: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 817 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4107 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 822 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4115 "raku.tab.c"
    break;

  case 112: /* stmt: KW_CATCH block  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4121 "raku.tab.c"
    break;

  case 113: /* stmt: block  */
#line 828 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4127 "raku.tab.c"
    break;

  case 114: /* stmt: unless_stmt  */
#line 829 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4133 "raku.tab.c"
    break;

  case 115: /* stmt: until_stmt  */
#line 830 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4139 "raku.tab.c"
    break;

  case 116: /* stmt: repeat_stmt  */
#line 831 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4145 "raku.tab.c"
    break;

  case 117: /* stmt: loop_stmt  */
#line 832 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4151 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST ';'  */
#line 833 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4157 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT ';'  */
#line 834 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4163 "raku.tab.c"
    break;

  case 120: /* stmt: KW_LAST KW_IF expr ';'  */
#line 836 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4169 "raku.tab.c"
    break;

  case 121: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 838 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4175 "raku.tab.c"
    break;

  case 122: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4181 "raku.tab.c"
    break;

  case 123: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4187 "raku.tab.c"
    break;

  case 124: /* stmt: sub_decl  */
#line 843 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4193 "raku.tab.c"
    break;

  case 125: /* stmt: class_decl  */
#line 844 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4199 "raku.tab.c"
    break;

  case 126: /* stmt: role_decl  */
#line 845 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4205 "raku.tab.c"
    break;

  case 127: /* stmt: grammar_decl  */
#line 846 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4211 "raku.tab.c"
    break;

  case 128: /* stmt: module_decl  */
#line 847 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4217 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF '(' expr ')' block  */
#line 851 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4223 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4229 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 855 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4235 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF expr block  */
#line 857 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4241 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 859 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4247 "raku.tab.c"
    break;

  case 134: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 861 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4253 "raku.tab.c"
    break;

  case 135: /* if_stmt: KW_IF expr block elsif_tail  */
#line 863 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4259 "raku.tab.c"
    break;

  case 136: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 865 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4265 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF expr block  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4271 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 871 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4277 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 873 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4283 "raku.tab.c"
    break;

  case 140: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 875 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4289 "raku.tab.c"
    break;

  case 141: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 877 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4295 "raku.tab.c"
    break;

  case 142: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 879 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4301 "raku.tab.c"
    break;

  case 143: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 883 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4307 "raku.tab.c"
    break;

  case 144: /* while_stmt: KW_WHILE expr block  */
#line 885 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4313 "raku.tab.c"
    break;

  case 145: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 889 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4319 "raku.tab.c"
    break;

  case 146: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 891 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4325 "raku.tab.c"
    break;

  case 147: /* unless_stmt: KW_UNLESS expr block  */
#line 893 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4331 "raku.tab.c"
    break;

  case 148: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 895 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4337 "raku.tab.c"
    break;

  case 149: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 899 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4343 "raku.tab.c"
    break;

  case 150: /* until_stmt: KW_UNTIL expr block  */
#line 901 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4349 "raku.tab.c"
    break;

  case 151: /* repeat_stmt: KW_REPEAT block  */
#line 905 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4355 "raku.tab.c"
    break;

  case 152: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 907 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4361 "raku.tab.c"
    break;

  case 153: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 909 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4367 "raku.tab.c"
    break;

  case 154: /* loop_stmt: KW_LOOP block  */
#line 913 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4373 "raku.tab.c"
    break;

  case 155: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 915 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4379 "raku.tab.c"
    break;

  case 156: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 917 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4385 "raku.tab.c"
    break;

  case 157: /* loop_incr: expr  */
#line 920 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4391 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 924 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4401 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 930 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4411 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 936 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node));
          } else (yyval.node) = rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)); }
#line 4423 "raku.tab.c"
    break;

  case 161: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 944 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4433 "raku.tab.c"
    break;

  case 162: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 950 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4442 "raku.tab.c"
    break;

  case 163: /* for_stmt: KW_FOR expr block  */
#line 955 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4449 "raku.tab.c"
    break;

  case 164: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 960 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4461 "raku.tab.c"
    break;

  case 165: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 968 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4474 "raku.tab.c"
    break;

  case 166: /* catch_when_list: KW_WHEN expr block  */
#line 979 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,(yyvsp[-1].node)); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 4480 "raku.tab.c"
    break;

  case 167: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 981 "raku.y"
        { exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 4486 "raku.tab.c"
    break;

  case 168: /* when_list: %empty  */
#line 984 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4492 "raku.tab.c"
    break;

  case 169: /* when_list: when_list KW_WHEN expr block  */
#line 986 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4500 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 992 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4512 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1000 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4522 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1006 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4532 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1012 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4544 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1020 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4554 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1026 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4564 "raku.tab.c"
    break;

  case 176: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1032 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4577 "raku.tab.c"
    break;

  case 177: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1041 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4588 "raku.tab.c"
    break;

  case 178: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1048 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4601 "raku.tab.c"
    break;

  case 179: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1057 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4614 "raku.tab.c"
    break;

  case 180: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1066 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4625 "raku.tab.c"
    break;

  case 181: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1075 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4636 "raku.tab.c"
    break;

  case 182: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1082 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4645 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list '}'  */
#line 1088 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4651 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr '}'  */
#line 1090 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4658 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1093 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4665 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1096 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4671 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1098 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4677 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1100 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4683 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1102 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4689 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1104 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4695 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1106 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4701 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1108 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4707 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1110 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4714 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1113 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4720 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1115 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4726 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1117 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4732 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1119 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4739 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1122 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4746 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1125 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4754 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list '}'  */
#line 1130 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4760 "raku.tab.c"
    break;

  case 201: /* method_body: '{' YADA '}'  */
#line 1131 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4766 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr '}'  */
#line 1133 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4773 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1136 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4780 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1139 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4786 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1141 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4792 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1143 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4798 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1145 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4804 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1147 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4810 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1149 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4816 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1151 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4822 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1153 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4829 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1156 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4835 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1158 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4841 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1160 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4847 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1162 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4854 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1165 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4861 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1168 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4869 "raku.tab.c"
    break;

  case 218: /* pkg_name: IDENT  */
#line 1173 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4875 "raku.tab.c"
    break;

  case 219: /* pkg_name: QIDENT  */
#line 1174 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4881 "raku.tab.c"
    break;

  case 220: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1178 "raku.y"
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
#line 4899 "raku.tab.c"
    break;

  case 221: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1194 "raku.y"
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
#line 4916 "raku.tab.c"
    break;

  case 222: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1209 "raku.y"
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
#line 4933 "raku.tab.c"
    break;

  case 223: /* is_clauses: %empty  */
#line 1223 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4939 "raku.tab.c"
    break;

  case 224: /* is_clauses: is_clauses IDENT IDENT  */
#line 1225 "raku.y"
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
#line 4955 "raku.tab.c"
    break;

  case 225: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1237 "raku.y"
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
#line 4971 "raku.tab.c"
    break;

  case 226: /* class_body_list: %empty  */
#line 1250 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4977 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1252 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4984 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1255 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4991 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1258 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4998 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1261 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5005 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1264 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5012 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1267 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5019 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1270 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5026 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1273 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5033 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1276 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5040 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1279 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5047 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1282 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5054 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1285 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5061 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1288 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5072 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1295 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5083 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1302 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5094 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1309 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5105 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1316 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5113 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1320 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5121 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1324 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5129 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1328 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5137 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1332 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5148 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1339 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5159 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1346 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5170 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1353 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5181 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1360 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5195 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1370 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5207 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1378 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5219 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1386 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5232 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1395 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5243 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1402 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5254 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1409 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5268 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1419 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5280 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1427 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5292 "raku.tab.c"
    break;

  case 260: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1437 "raku.y"
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
#line 5309 "raku.tab.c"
    break;

  case 261: /* grammar_body_list: %empty  */
#line 1451 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5315 "raku.tab.c"
    break;

  case 262: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1453 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5324 "raku.tab.c"
    break;

  case 263: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1458 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5333 "raku.tab.c"
    break;

  case 264: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1463 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5342 "raku.tab.c"
    break;

  case 265: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1470 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5350 "raku.tab.c"
    break;

  case 266: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1474 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5358 "raku.tab.c"
    break;

  case 267: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1478 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5366 "raku.tab.c"
    break;

  case 268: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1482 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5374 "raku.tab.c"
    break;

  case 269: /* pair_list: IDENT OP_FATARROW expr  */
#line 1488 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5380 "raku.tab.c"
    break;

  case 270: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1490 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5386 "raku.tab.c"
    break;

  case 271: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1492 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5392 "raku.tab.c"
    break;

  case 272: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1494 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5398 "raku.tab.c"
    break;

  case 273: /* param_list: VAR_SCALAR  */
#line 1497 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5404 "raku.tab.c"
    break;

  case 274: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1498 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5410 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1499 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5416 "raku.tab.c"
    break;

  case 276: /* param_list: VAR_ARRAY  */
#line 1500 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5422 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' VAR_ARRAY  */
#line 1501 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5428 "raku.tab.c"
    break;

  case 278: /* param_list: IDENT VAR_SCALAR  */
#line 1502 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5434 "raku.tab.c"
    break;

  case 279: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1503 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5440 "raku.tab.c"
    break;

  case 280: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1504 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5446 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' VAR_SCALAR  */
#line 1505 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5452 "raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1506 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5458 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1507 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5464 "raku.tab.c"
    break;

  case 284: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1508 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5470 "raku.tab.c"
    break;

  case 285: /* param_list: VAR_SCALAR '=' expr  */
#line 1509 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5476 "raku.tab.c"
    break;

  case 286: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1510 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5482 "raku.tab.c"
    break;

  case 287: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1511 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5488 "raku.tab.c"
    break;

  case 288: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1512 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5494 "raku.tab.c"
    break;

  case 289: /* param_list: SLURPY_POS  */
#line 1513 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5500 "raku.tab.c"
    break;

  case 290: /* param_list: param_list ',' SLURPY_POS  */
#line 1514 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5506 "raku.tab.c"
    break;

  case 291: /* param_list: SLURPY_LOL  */
#line 1515 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5512 "raku.tab.c"
    break;

  case 292: /* param_list: param_list ',' SLURPY_LOL  */
#line 1516 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5518 "raku.tab.c"
    break;

  case 293: /* param_list: SLURPY_NAMED  */
#line 1517 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5524 "raku.tab.c"
    break;

  case 294: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1518 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5530 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list '}'  */
#line 1521 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5536 "raku.tab.c"
    break;

  case 296: /* block: '{' YADA '}'  */
#line 1522 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5542 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr '}'  */
#line 1524 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5548 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1526 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5554 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1528 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5560 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1530 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5566 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1532 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5572 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1534 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5579 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1537 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5585 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1539 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5591 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1541 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5597 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1543 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5603 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1545 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5610 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1548 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5617 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1551 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5625 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1555 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5632 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1558 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5639 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1561 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5646 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1564 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5652 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1566 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5660 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1570 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5667 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1573 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5674 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1576 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5681 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1579 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5688 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1582 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5695 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1585 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5701 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1587 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5707 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1589 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5713 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_LAST '}'  */
#line 1591 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5719 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1593 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5725 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1595 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5731 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1597 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5737 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1599 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5743 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1601 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5749 "raku.tab.c"
    break;

  case 329: /* closure: '{' expr '}'  */
#line 1604 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5755 "raku.tab.c"
    break;

  case 330: /* expr: VAR_SCALAR '=' expr  */
#line 1607 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5761 "raku.tab.c"
    break;

  case 331: /* expr: VAR_ARRAY '=' expr  */
#line 1608 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5767 "raku.tab.c"
    break;

  case 332: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1609 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5773 "raku.tab.c"
    break;

  case 333: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1610 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5779 "raku.tab.c"
    break;

  case 334: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1611 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5785 "raku.tab.c"
    break;

  case 335: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1612 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5791 "raku.tab.c"
    break;

  case 336: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1613 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5797 "raku.tab.c"
    break;

  case 337: /* expr: KW_GATHER block  */
#line 1614 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5807 "raku.tab.c"
    break;

  case 338: /* expr: KW_GATHER for_stmt  */
#line 1619 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5817 "raku.tab.c"
    break;

  case 339: /* expr: tern_expr OP_FATARROW expr  */
#line 1625 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5823 "raku.tab.c"
    break;

  case 340: /* expr: tern_expr  */
#line 1626 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5829 "raku.tab.c"
    break;

  case 341: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1630 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5835 "raku.tab.c"
    break;

  case 342: /* tern_expr: or_expr  */
#line 1631 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5841 "raku.tab.c"
    break;

  case 343: /* or_expr: or_expr OP_OR and_expr  */
#line 1634 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5847 "raku.tab.c"
    break;

  case 344: /* or_expr: and_expr  */
#line 1635 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5853 "raku.tab.c"
    break;

  case 345: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1638 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5859 "raku.tab.c"
    break;

  case 346: /* and_expr: cmp_expr  */
#line 1639 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5865 "raku.tab.c"
    break;

  case 347: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1642 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5871 "raku.tab.c"
    break;

  case 348: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1643 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5877 "raku.tab.c"
    break;

  case 349: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1644 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5883 "raku.tab.c"
    break;

  case 350: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1645 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5889 "raku.tab.c"
    break;

  case 351: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1646 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5895 "raku.tab.c"
    break;

  case 352: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1647 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5901 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1648 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5907 "raku.tab.c"
    break;

  case 354: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1649 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5913 "raku.tab.c"
    break;

  case 355: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1650 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5919 "raku.tab.c"
    break;

  case 356: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1651 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5925 "raku.tab.c"
    break;

  case 357: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1652 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5931 "raku.tab.c"
    break;

  case 358: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1653 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5937 "raku.tab.c"
    break;

  case 359: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1655 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5947 "raku.tab.c"
    break;

  case 360: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1661 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5957 "raku.tab.c"
    break;

  case 361: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1667 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5967 "raku.tab.c"
    break;

  case 362: /* cmp_expr: divis_expr  */
#line 1672 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5973 "raku.tab.c"
    break;

  case 363: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1675 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5979 "raku.tab.c"
    break;

  case 364: /* divis_expr: jct_expr  */
#line 1676 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5985 "raku.tab.c"
    break;

  case 365: /* jct_expr: jct_expr '|' range_expr  */
#line 1679 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5991 "raku.tab.c"
    break;

  case 366: /* jct_expr: jct_expr '&' range_expr  */
#line 1680 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5997 "raku.tab.c"
    break;

  case 367: /* jct_expr: dor_expr  */
#line 1681 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6003 "raku.tab.c"
    break;

  case 368: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1685 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6009 "raku.tab.c"
    break;

  case 369: /* dor_expr: range_expr  */
#line 1686 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6015 "raku.tab.c"
    break;

  case 370: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1689 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6021 "raku.tab.c"
    break;

  case 371: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1690 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6027 "raku.tab.c"
    break;

  case 372: /* range_expr: add_expr  */
#line 1691 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6033 "raku.tab.c"
    break;

  case 373: /* add_expr: add_expr '~' repl_expr  */
#line 1694 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6039 "raku.tab.c"
    break;

  case 374: /* add_expr: repl_expr  */
#line 1695 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6045 "raku.tab.c"
    break;

  case 375: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1698 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6051 "raku.tab.c"
    break;

  case 376: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1699 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6057 "raku.tab.c"
    break;

  case 377: /* repl_expr: addsub_expr  */
#line 1700 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6063 "raku.tab.c"
    break;

  case 378: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1703 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6069 "raku.tab.c"
    break;

  case 379: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1704 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6075 "raku.tab.c"
    break;

  case 380: /* addsub_expr: mul_expr  */
#line 1705 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6081 "raku.tab.c"
    break;

  case 381: /* mul_expr: mul_expr '*' unary_expr  */
#line 1708 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6087 "raku.tab.c"
    break;

  case 382: /* mul_expr: mul_expr '/' unary_expr  */
#line 1709 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6093 "raku.tab.c"
    break;

  case 383: /* mul_expr: mul_expr '%' unary_expr  */
#line 1710 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6099 "raku.tab.c"
    break;

  case 384: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1712 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6105 "raku.tab.c"
    break;

  case 385: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1714 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6111 "raku.tab.c"
    break;

  case 386: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1716 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6117 "raku.tab.c"
    break;

  case 387: /* mul_expr: unary_expr  */
#line 1717 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6123 "raku.tab.c"
    break;

  case 388: /* unary_expr: '-' unary_expr  */
#line 1720 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6129 "raku.tab.c"
    break;

  case 389: /* unary_expr: '+' unary_expr  */
#line 1721 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6135 "raku.tab.c"
    break;

  case 390: /* unary_expr: '!' unary_expr  */
#line 1722 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6141 "raku.tab.c"
    break;

  case 391: /* unary_expr: CARET unary_expr  */
#line 1723 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6147 "raku.tab.c"
    break;

  case 392: /* unary_expr: OP_REDUCE unary_expr  */
#line 1725 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6156 "raku.tab.c"
    break;

  case 393: /* unary_expr: pow_expr  */
#line 1729 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6162 "raku.tab.c"
    break;

  case 394: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1732 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6168 "raku.tab.c"
    break;

  case 395: /* pow_expr: postfix_expr  */
#line 1733 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6174 "raku.tab.c"
    break;

  case 396: /* scalar_list: VAR_SCALAR  */
#line 1736 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6180 "raku.tab.c"
    break;

  case 397: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1737 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6186 "raku.tab.c"
    break;

  case 398: /* meth_name: IDENT  */
#line 1740 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6192 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_SORT  */
#line 1741 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6198 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_REVERSE  */
#line 1742 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6204 "raku.tab.c"
    break;

  case 401: /* meth_name: KW_MAP  */
#line 1743 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6210 "raku.tab.c"
    break;

  case 402: /* meth_name: KW_GREP  */
#line 1744 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6216 "raku.tab.c"
    break;

  case 403: /* meth_name: KW_SAY  */
#line 1745 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6222 "raku.tab.c"
    break;

  case 404: /* meth_name: KW_PRINT  */
#line 1746 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6228 "raku.tab.c"
    break;

  case 405: /* meth_name: KW_TAKE  */
#line 1747 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6234 "raku.tab.c"
    break;

  case 406: /* meth_name: KW_RETURN  */
#line 1748 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6240 "raku.tab.c"
    break;

  case 407: /* meth_name: KW_EXISTS  */
#line 1749 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6246 "raku.tab.c"
    break;

  case 408: /* meth_name: KW_DELETE  */
#line 1750 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6252 "raku.tab.c"
    break;

  case 409: /* meth_name: KW_JOIN  */
#line 1751 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6258 "raku.tab.c"
    break;

  case 410: /* meth_name: TESTOP  */
#line 1752 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6264 "raku.tab.c"
    break;

  case 411: /* postfix_expr: call_expr  */
#line 1754 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6270 "raku.tab.c"
    break;

  case 412: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1757 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6280 "raku.tab.c"
    break;

  case 413: /* call_expr: IDENT '(' arg_list ')'  */
#line 1763 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6289 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '(' ')'  */
#line 1767 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6295 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1769 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6301 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1771 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6307 "raku.tab.c"
    break;

  case 417: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1773 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6316 "raku.tab.c"
    break;

  case 418: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1778 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6322 "raku.tab.c"
    break;

  case 419: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1780 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6332 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1786 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6342 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1792 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6350 "raku.tab.c"
    break;

  case 422: /* call_expr: IDENT '.' KW_NEW  */
#line 1796 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6358 "raku.tab.c"
    break;

  case 423: /* call_expr: IDENT '.' IDENT  */
#line 1800 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6367 "raku.tab.c"
    break;

  case 424: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1805 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6378 "raku.tab.c"
    break;

  case 425: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1812 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6387 "raku.tab.c"
    break;

  case 426: /* call_expr: IDENT '.' CARET IDENT  */
#line 1817 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6397 "raku.tab.c"
    break;

  case 427: /* call_expr: atom '.' CARET IDENT  */
#line 1823 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6407 "raku.tab.c"
    break;

  case 428: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1829 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6418 "raku.tab.c"
    break;

  case 429: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1836 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6429 "raku.tab.c"
    break;

  case 430: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1843 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6438 "raku.tab.c"
    break;

  case 431: /* call_expr: atom '.' meth_name  */
#line 1848 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6447 "raku.tab.c"
    break;

  case 432: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1853 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6458 "raku.tab.c"
    break;

  case 433: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1860 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6467 "raku.tab.c"
    break;

  case 434: /* call_expr: call_expr '.' meth_name  */
#line 1865 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6476 "raku.tab.c"
    break;

  case 435: /* call_expr: KW_DIE expr  */
#line 1870 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6482 "raku.tab.c"
    break;

  case 436: /* call_expr: KW_MAP closure expr  */
#line 1872 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6488 "raku.tab.c"
    break;

  case 437: /* call_expr: KW_MAP closure ',' expr  */
#line 1874 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6494 "raku.tab.c"
    break;

  case 438: /* call_expr: KW_GREP closure expr  */
#line 1876 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6500 "raku.tab.c"
    break;

  case 439: /* call_expr: KW_SORT expr  */
#line 1878 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6506 "raku.tab.c"
    break;

  case 440: /* call_expr: KW_SORT closure expr  */
#line 1880 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6512 "raku.tab.c"
    break;

  case 441: /* call_expr: KW_REVERSE expr  */
#line 1882 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6518 "raku.tab.c"
    break;

  case 442: /* call_expr: atom  */
#line 1883 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6524 "raku.tab.c"
    break;

  case 443: /* arg_list: expr  */
#line 1886 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6530 "raku.tab.c"
    break;

  case 444: /* arg_list: arg_list ',' expr  */
#line 1887 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6536 "raku.tab.c"
    break;

  case 445: /* arg_list: arg_list ','  */
#line 1888 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6542 "raku.tab.c"
    break;

  case 446: /* atom: LIT_INT  */
#line 1891 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6548 "raku.tab.c"
    break;

  case 447: /* atom: LIT_FLOAT  */
#line 1892 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6554 "raku.tab.c"
    break;

  case 448: /* atom: LIT_STR  */
#line 1893 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6560 "raku.tab.c"
    break;

  case 449: /* atom: WORDLIST  */
#line 1895 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6573 "raku.tab.c"
    break;

  case 450: /* atom: LIT_INTERP_STR  */
#line 1903 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6579 "raku.tab.c"
    break;

  case 451: /* atom: VAR_SCALAR  */
#line 1904 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6585 "raku.tab.c"
    break;

  case 452: /* atom: OP_INC VAR_SCALAR  */
#line 1905 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6591 "raku.tab.c"
    break;

  case 453: /* atom: OP_DEC VAR_SCALAR  */
#line 1906 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6597 "raku.tab.c"
    break;

  case 454: /* atom: VAR_SCALAR OP_INC  */
#line 1907 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6603 "raku.tab.c"
    break;

  case 455: /* atom: VAR_SCALAR OP_DEC  */
#line 1908 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6609 "raku.tab.c"
    break;

  case 456: /* atom: VAR_ARRAY  */
#line 1909 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6615 "raku.tab.c"
    break;

  case 457: /* atom: VAR_HASH  */
#line 1910 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6621 "raku.tab.c"
    break;

  case 458: /* atom: VAR_CAPTURE  */
#line 1912 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6629 "raku.tab.c"
    break;

  case 459: /* atom: VAR_FH  */
#line 1916 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6637 "raku.tab.c"
    break;

  case 460: /* atom: VAR_NAMED_CAPTURE  */
#line 1920 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6644 "raku.tab.c"
    break;

  case 461: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1923 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6650 "raku.tab.c"
    break;

  case 462: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1925 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6656 "raku.tab.c"
    break;

  case 463: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1927 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6662 "raku.tab.c"
    break;

  case 464: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1929 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6668 "raku.tab.c"
    break;

  case 465: /* atom: ARR_ALL_SLICE  */
#line 1931 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6674 "raku.tab.c"
    break;

  case 466: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1933 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6680 "raku.tab.c"
    break;

  case 467: /* atom: VAR_HASH '{' expr '}'  */
#line 1935 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6686 "raku.tab.c"
    break;

  case 468: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1937 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6692 "raku.tab.c"
    break;

  case 469: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1939 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6698 "raku.tab.c"
    break;

  case 470: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 1941 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 6704 "raku.tab.c"
    break;

  case 471: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 1943 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 6710 "raku.tab.c"
    break;

  case 472: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 1945 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 6716 "raku.tab.c"
    break;

  case 473: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 1947 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 6722 "raku.tab.c"
    break;

  case 474: /* atom: IDENT  */
#line 1948 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6728 "raku.tab.c"
    break;

  case 475: /* atom: VAR_TWIGIL  */
#line 1950 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6736 "raku.tab.c"
    break;

  case 476: /* atom: VAR_ARRAY_TWIGIL  */
#line 1954 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6744 "raku.tab.c"
    break;

  case 477: /* atom: VAR_HASH_TWIGIL  */
#line 1958 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6752 "raku.tab.c"
    break;

  case 478: /* atom: '[' ']'  */
#line 1961 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6758 "raku.tab.c"
    break;

  case 479: /* atom: '[' expr ']'  */
#line 1963 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6764 "raku.tab.c"
    break;

  case 480: /* atom: '[' expr ',' ']'  */
#line 1965 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6770 "raku.tab.c"
    break;

  case 481: /* atom: '[' expr ',' arg_list ']'  */
#line 1967 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6777 "raku.tab.c"
    break;

  case 482: /* atom: DOLLAR_LBRACKET ']'  */
#line 1969 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 6783 "raku.tab.c"
    break;

  case 483: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1971 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6789 "raku.tab.c"
    break;

  case 484: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1973 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6795 "raku.tab.c"
    break;

  case 485: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1975 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6802 "raku.tab.c"
    break;

  case 486: /* atom: '(' ')'  */
#line 1977 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6808 "raku.tab.c"
    break;

  case 487: /* atom: '(' expr ')'  */
#line 1978 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6814 "raku.tab.c"
    break;

  case 488: /* atom: '(' expr ',' ')'  */
#line 1980 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6820 "raku.tab.c"
    break;

  case 489: /* atom: '(' expr ',' arg_list ')'  */
#line 1982 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6827 "raku.tab.c"
    break;

  case 490: /* atom: block  */
#line 1984 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6833 "raku.tab.c"
    break;

  case 491: /* atom: KW_SUB block  */
#line 1985 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6839 "raku.tab.c"
    break;


#line 6843 "raku.tab.c"

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

#line 1987 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
