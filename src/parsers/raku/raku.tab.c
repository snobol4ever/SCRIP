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
static tree_t *rk_tw_field(const char *name) {
    tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *)intern(rk_tw_bare(name)); return fe;
}
static tree_t *rk_tw_post_incdec(const char *var, int add) {
    static int __twpost_uid = 0; char tmp[32]; snprintf(tmp, sizeof tmp, "__twpost_%d", __twpost_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rk_tw_field(var)));
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    expr_add_child(seq, expr_binary(TT_ASSIGN, rk_tw_field(var), expr_binary(add ? TT_ADD : TT_SUB, rk_tw_field(var), one)));
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

#line 525 "raku.tab.c"

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
  YYSYMBOL_LIT_BOOL = 4,                   /* LIT_BOOL  */
  YYSYMBOL_LIT_FLOAT = 5,                  /* LIT_FLOAT  */
  YYSYMBOL_LIT_STR = 6,                    /* LIT_STR  */
  YYSYMBOL_LIT_INTERP_STR = 7,             /* LIT_INTERP_STR  */
  YYSYMBOL_LIT_REGEX = 8,                  /* LIT_REGEX  */
  YYSYMBOL_LIT_MATCH_GLOBAL = 9,           /* LIT_MATCH_GLOBAL  */
  YYSYMBOL_LIT_SUBST = 10,                 /* LIT_SUBST  */
  YYSYMBOL_VAR_SCALAR = 11,                /* VAR_SCALAR  */
  YYSYMBOL_VAR_ARRAY = 12,                 /* VAR_ARRAY  */
  YYSYMBOL_VAR_HASH = 13,                  /* VAR_HASH  */
  YYSYMBOL_VAR_TWIGIL = 14,                /* VAR_TWIGIL  */
  YYSYMBOL_IDENT = 15,                     /* IDENT  */
  YYSYMBOL_QIDENT = 16,                    /* QIDENT  */
  YYSYMBOL_VAR_ARRAY_TWIGIL = 17,          /* VAR_ARRAY_TWIGIL  */
  YYSYMBOL_VAR_HASH_TWIGIL = 18,           /* VAR_HASH_TWIGIL  */
  YYSYMBOL_CARET = 19,                     /* CARET  */
  YYSYMBOL_DOLLAR_LBRACKET = 20,           /* DOLLAR_LBRACKET  */
  YYSYMBOL_VAR_CAPTURE = 21,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 22,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 23,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_USE = 24,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 25,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 26,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 27,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 28,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 29,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 30,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 31,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 32,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 33,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 34,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 35,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 36,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 37,                 /* KW_RETURN  */
  YYSYMBOL_KW_EXIT = 38,                   /* KW_EXIT  */
  YYSYMBOL_KW_CONSTANT = 39,               /* KW_CONSTANT  */
  YYSYMBOL_KW_ENUM = 40,                   /* KW_ENUM  */
  YYSYMBOL_KW_JOIN = 41,                   /* KW_JOIN  */
  YYSYMBOL_KW_GIVEN = 42,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 43,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 44,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 45,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 46,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 47,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 48,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 49,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 50,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 51,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 52,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 53,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 54,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 55,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 56,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 57,                   /* KW_SORT  */
  YYSYMBOL_KW_REVERSE = 58,                /* KW_REVERSE  */
  YYSYMBOL_KW_TRY = 59,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 60,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 61,                    /* KW_DIE  */
  YYSYMBOL_KW_FAIL = 62,                   /* KW_FAIL  */
  YYSYMBOL_KW_CLASS = 63,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 64,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 65,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 66,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 67,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 68,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 69,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 70,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 71,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 72,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 73,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 74,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 75,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 76,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 77,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 78,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 79,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 80,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 81,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 82,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 83,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 84,                  /* KW_REGEX  */
  YYSYMBOL_KW_MODULE = 85,                 /* KW_MODULE  */
  YYSYMBOL_OP_FATARROW = 86,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 87,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 88,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 89,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 90,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 91,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 92,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 93,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 94,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 95,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 96,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 97,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 98,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 99,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 100,                   /* OP_AND  */
  YYSYMBOL_OP_OR = 101,                    /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 102,              /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 103,              /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 104,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 105,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 106,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 107,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 108,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 109,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 110,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 111,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 112,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 113,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 114,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 115,                   /* OP_DIV  */
  YYSYMBOL_ADV_EXISTS = 116,               /* ADV_EXISTS  */
  YYSYMBOL_ADV_DELETE = 117,               /* ADV_DELETE  */
  YYSYMBOL_OP_BAND = 118,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 119,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 120,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 121,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 122,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 123,                   /* OP_POW  */
  YYSYMBOL_124_ = 124,                     /* '='  */
  YYSYMBOL_125_ = 125,                     /* '!'  */
  YYSYMBOL_126_ = 126,                     /* '<'  */
  YYSYMBOL_127_ = 127,                     /* '>'  */
  YYSYMBOL_128_ = 128,                     /* '|'  */
  YYSYMBOL_129_ = 129,                     /* '&'  */
  YYSYMBOL_130_ = 130,                     /* '~'  */
  YYSYMBOL_131_ = 131,                     /* '+'  */
  YYSYMBOL_132_ = 132,                     /* '-'  */
  YYSYMBOL_133_ = 133,                     /* '*'  */
  YYSYMBOL_134_ = 134,                     /* '/'  */
  YYSYMBOL_135_ = 135,                     /* '%'  */
  YYSYMBOL_UMINUS = 136,                   /* UMINUS  */
  YYSYMBOL_137_ = 137,                     /* '.'  */
  YYSYMBOL_138_ = 138,                     /* ';'  */
  YYSYMBOL_139_ = 139,                     /* '('  */
  YYSYMBOL_140_ = 140,                     /* ')'  */
  YYSYMBOL_141_ = 141,                     /* ','  */
  YYSYMBOL_142_ = 142,                     /* '['  */
  YYSYMBOL_143_ = 143,                     /* ']'  */
  YYSYMBOL_144_ = 144,                     /* '{'  */
  YYSYMBOL_145_ = 145,                     /* '}'  */
  YYSYMBOL_146_ = 146,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 147,                 /* $accept  */
  YYSYMBOL_program = 148,                  /* program  */
  YYSYMBOL_stmt_list = 149,                /* stmt_list  */
  YYSYMBOL_stmt = 150,                     /* stmt  */
  YYSYMBOL_if_stmt = 151,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 152,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 153,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 154,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 155,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 156,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 157,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 158,                /* loop_incr  */
  YYSYMBOL_for_stmt = 159,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 160,               /* given_stmt  */
  YYSYMBOL_catch_when_list = 161,          /* catch_when_list  */
  YYSYMBOL_when_list = 162,                /* when_list  */
  YYSYMBOL_sub_decl = 163,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 164,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 165,                 /* sub_body  */
  YYSYMBOL_method_body = 166,              /* method_body  */
  YYSYMBOL_pkg_name = 167,                 /* pkg_name  */
  YYSYMBOL_class_decl = 168,               /* class_decl  */
  YYSYMBOL_role_decl = 169,                /* role_decl  */
  YYSYMBOL_module_decl = 170,              /* module_decl  */
  YYSYMBOL_is_clauses = 171,               /* is_clauses  */
  YYSYMBOL_class_body_list = 172,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 173,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 174,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 175,           /* named_arg_list  */
  YYSYMBOL_pair_list = 176,                /* pair_list  */
  YYSYMBOL_param_list = 177,               /* param_list  */
  YYSYMBOL_block = 178,                    /* block  */
  YYSYMBOL_closure = 179,                  /* closure  */
  YYSYMBOL_expr = 180,                     /* expr  */
  YYSYMBOL_tern_expr = 181,                /* tern_expr  */
  YYSYMBOL_or_expr = 182,                  /* or_expr  */
  YYSYMBOL_and_expr = 183,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 184,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 185,               /* divis_expr  */
  YYSYMBOL_jct_expr = 186,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 187,                 /* dor_expr  */
  YYSYMBOL_range_expr = 188,               /* range_expr  */
  YYSYMBOL_add_expr = 189,                 /* add_expr  */
  YYSYMBOL_repl_expr = 190,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 191,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 192,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 193,               /* unary_expr  */
  YYSYMBOL_pow_expr = 194,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 195,              /* scalar_list  */
  YYSYMBOL_meth_name = 196,                /* meth_name  */
  YYSYMBOL_postfix_expr = 197,             /* postfix_expr  */
  YYSYMBOL_call_expr = 198,                /* call_expr  */
  YYSYMBOL_arg_list = 199,                 /* arg_list  */
  YYSYMBOL_atom = 200                      /* atom  */
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
#define YYLAST   7394

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  504
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1248

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   379


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
       2,     2,     2,   125,     2,     2,     2,   135,   129,     2,
     139,   140,   133,   131,   141,   132,   137,   134,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   146,   138,
     126,   124,   127,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   142,     2,   143,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   144,   128,   145,   130,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   121,   122,   123,   136
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   546,   546,   557,   558,   561,   563,   565,   567,   571,
     573,   575,   577,   579,   583,   587,   589,   593,   597,   599,
     601,   603,   605,   607,   611,   615,   617,   619,   621,   623,
     625,   627,   629,   631,   633,   635,   637,   639,   647,   649,
     651,   653,   655,   657,   659,   662,   665,   667,   670,   673,
     675,   679,   681,   683,   685,   687,   690,   693,   695,   698,
     701,   704,   706,   711,   715,   719,   724,   728,   729,   731,
     733,   735,   737,   740,   742,   744,   746,   749,   752,   755,
     758,   761,   763,   765,   767,   769,   772,   774,   776,   778,
     781,   784,   788,   791,   793,   795,   797,   800,   803,   806,
     809,   812,   815,   818,   819,   820,   821,   822,   823,   824,
     826,   828,   832,   837,   841,   843,   845,   846,   847,   848,
     849,   850,   851,   853,   855,   857,   859,   860,   861,   862,
     863,   866,   868,   870,   872,   874,   876,   878,   880,   884,
     886,   888,   890,   892,   894,   898,   900,   904,   906,   908,
     910,   914,   916,   920,   922,   924,   928,   930,   932,   936,
     939,   945,   951,   959,   965,   970,   975,   983,   994,   996,
    1000,  1001,  1007,  1015,  1021,  1027,  1035,  1041,  1047,  1056,
    1063,  1072,  1081,  1090,  1097,  1102,  1109,  1110,  1113,  1116,
    1118,  1120,  1122,  1124,  1126,  1128,  1130,  1133,  1135,  1137,
    1139,  1142,  1145,  1151,  1152,  1153,  1156,  1159,  1162,  1164,
    1166,  1168,  1170,  1172,  1174,  1176,  1179,  1181,  1183,  1185,
    1188,  1191,  1197,  1198,  1201,  1217,  1232,  1247,  1248,  1260,
    1274,  1275,  1278,  1281,  1284,  1287,  1290,  1293,  1296,  1299,
    1302,  1305,  1308,  1311,  1318,  1324,  1330,  1337,  1344,  1351,
    1355,  1359,  1363,  1367,  1374,  1381,  1388,  1395,  1405,  1413,
    1421,  1430,  1437,  1444,  1454,  1462,  1472,  1487,  1488,  1493,
    1498,  1505,  1509,  1513,  1517,  1523,  1525,  1527,  1529,  1533,
    1534,  1535,  1536,  1537,  1538,  1539,  1540,  1541,  1542,  1543,
    1544,  1545,  1546,  1547,  1548,  1549,  1550,  1551,  1552,  1553,
    1554,  1557,  1558,  1559,  1561,  1563,  1565,  1567,  1569,  1572,
    1574,  1576,  1578,  1580,  1583,  1586,  1590,  1593,  1596,  1599,
    1601,  1605,  1608,  1611,  1614,  1617,  1620,  1622,  1624,  1626,
    1628,  1630,  1632,  1634,  1636,  1640,  1643,  1644,  1645,  1646,
    1647,  1648,  1649,  1650,  1655,  1660,  1662,  1665,  1667,  1670,
    1671,  1674,  1675,  1678,  1679,  1680,  1681,  1682,  1683,  1684,
    1685,  1686,  1687,  1688,  1689,  1690,  1696,  1702,  1708,  1714,
    1717,  1718,  1721,  1722,  1723,  1726,  1728,  1731,  1732,  1733,
    1736,  1737,  1740,  1741,  1742,  1745,  1746,  1747,  1750,  1751,
    1752,  1753,  1755,  1757,  1759,  1762,  1763,  1764,  1765,  1766,
    1771,  1774,  1775,  1778,  1779,  1782,  1783,  1784,  1785,  1786,
    1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,  1796,  1798,
    1804,  1809,  1810,  1812,  1814,  1819,  1821,  1827,  1833,  1837,
    1841,  1846,  1853,  1858,  1864,  1870,  1877,  1884,  1889,  1894,
    1901,  1906,  1911,  1918,  1923,  1928,  1930,  1932,  1934,  1936,
    1938,  1940,  1942,  1945,  1946,  1947,  1950,  1951,  1955,  1956,
    1957,  1966,  1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,
    1975,  1976,  1980,  1984,  1987,  1989,  1991,  1993,  1995,  1997,
    1999,  2001,  2003,  2005,  2007,  2009,  2011,  2013,  2014,  2018,
    2022,  2026,  2027,  2029,  2031,  2034,  2035,  2037,  2039,  2042,
    2043,  2044,  2046,  2049,  2050
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
  "\"end of file\"", "error", "\"invalid token\"", "LIT_INT", "LIT_BOOL",
  "LIT_FLOAT", "LIT_STR", "LIT_INTERP_STR", "LIT_REGEX",
  "LIT_MATCH_GLOBAL", "LIT_SUBST", "VAR_SCALAR", "VAR_ARRAY", "VAR_HASH",
  "VAR_TWIGIL", "IDENT", "QIDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL",
  "CARET", "DOLLAR_LBRACKET", "VAR_CAPTURE", "VAR_FH", "VAR_NAMED_CAPTURE",
  "KW_USE", "TESTOP", "KW_MY", "KW_SAY", "KW_PRINT", "KW_IF", "KW_ELSE",
  "KW_ELSIF", "KW_WHILE", "KW_FOR", "KW_SUB", "KW_GATHER", "KW_TAKE",
  "KW_RETURN", "KW_EXIT", "KW_CONSTANT", "KW_ENUM", "KW_JOIN", "KW_GIVEN",
  "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT", "KW_EXISTS",
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_LOOP", "KW_LAST",
  "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_REVERSE", "KW_TRY",
  "KW_CATCH", "KW_DIE", "KW_FAIL", "KW_CLASS", "KW_METHOD", "KW_HAS",
  "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE",
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

#define YYPACT_NINF (-894)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-504)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -894,    13,  2681,  -894,  -894,  -894,  -894,  -894,  -894,   796,
     -59,   132,   414,    28,  -894,  -894,  7200,  2780,  -894,  -894,
    -894,    86,  2879,   163,  5913,  6012,  6111,  6210,  6309,     2,
     -15,  6309,  2978,  3077,   263,    54,  6309,  6309,    64,   122,
    6408,  6507,     4,   470,    -1,    43,   162,   162,  6606,  6309,
       4,   283,  6309,  3176,   493,   493,    46,  7200,  -894,  -894,
     493,   493,   219,   300,  7200,  7200,  7200,   834,  -894,  3275,
    3417,   357,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,   584,  -894,  -894,  -894,  -894,  7259,   648,   364,
     643,   371,   455,   656,   814,   417,  -894,   117,   824,   818,
     459,  -894,  -894,   440,   430,   471,   564,  -894,  -894,  6309,
    6309,  6309,  6309,  6309,  6309,   616,  3516,  6705,  3658,   632,
    6309,  -894,  -894,  6309,   551,    47,  1221,   247,   573,   177,
     845,    82,     4,  3800,  -894,  -894,   528,    93,  -894,   203,
    3942,  4041,  -894,  4140,  -894,    90,   -48,   -10,   277,   369,
     691,   394,   716,  3800,   558,  3800,    57,  3800,     4,  3800,
       4,   198,   291,   548,  -894,  -894,  -894,   348,  -894,   171,
    -894,   319,   595,   607,   658,   668,   622,   205,   221,  3800,
       4,  3800,     4,   342,  4282,  -894,  6309,  6309,  -894,  6309,
    6309,  -894,  6309,  4381,  6309,  4480,  6309,  -894,  -894,   768,
     218,  -894,  -894,  -894,   651,  -894,  -894,  -894,   700,   725,
      49,  -894,   750,   779,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,   730,   854,  -894,   839,   841,  -894,   318,   685,
    1812,  6309,  6309,  6309,  6309,  6309,  6309,  6309,  6309,  -894,
    6309,  6309,  6309,  6309,  6309,  6309,  6309,  6309,  -894,  6309,
    7200,  7200,  7200,  7200,  7200,  7200,  7200,  7200,  7200,  7200,
    7200,  7200,  7200,  7200,  7200,   693,  7200,  7200,  7200,  7200,
    7200,  7200,  7200,  7200,  7200,  7200,  7200,  7200,  7200,  7200,
    7200,  7200,  7200,  7200,   834,   760,   816,   710,   800,   803,
     809,   819,   822,   -64,  -894,   843,  3800,   507,   857,   398,
     842,   832,   865,  -894,   879,  1020,   907,   309,  -894,  1044,
     850,   858,  3658,  1073,  6309,  6309,  6309,  6309,  6309,  6309,
    6309,  6309,  4579,  -894,  -894,   983,  -894,   985,   987,   839,
     864,  -894,  6309,  6309,  -894,  6804,  -894,  6903,  -894,   374,
     377,   378,   561,  1009,  1012,  -894,   866,   868,  6309,  6309,
    6309,  6309,  6309,  6309,  6309,  -894,  6309,   871,  6309,  6309,
    -894,  6309,   873,   955,   876,  -894,   716,  6309,  -894,  7200,
    7200,    21,  -894,  -894,  -894,  6309,  6309,  6309,  -894,  6309,
    6309,  -894,  6309,  6309,  1001,  6309,  -894,  1125,  6309,  1126,
    6309,   882,  1112,   884,  -894,  6309,  6309,  1132,  1006,  1007,
    1008,  1011,  1013,  1002,  6309,  -894,  -894,  -894,     4,  6309,
       4,    35,  -894,    29,  -894,    30,  1014,  1018,  -894,  -894,
    4678,  -894,  4820,  1026,  1141,  4962,  -894,  -894,   863,   251,
     250,   419,  5913,  6012,  6309,  2340,    68,   154,  -894,   268,
    1021,  1022,  1023,  1025,  1027,  1029,  1031,  1032,  1033,  1037,
    1038,  1039,  1040,  1041,  1042,  1045,  -894,   371,  1055,   455,
    1061,  1061,  1061,  1061,  1061,  1061,  1061,  1061,  1061,  1061,
    1061,  1061,  -894,  -894,  -894,  -894,   814,  -894,  -894,  -894,
    1052,  1052,   824,   818,   818,   459,   459,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  1046,  1169,  1048,  -894,  5061,  -894,
    -894,  -894,  -894,  -894,  -894,  6309,  5203,  -894,   886,  -894,
    6309,  6309,  6309,  6309,  1064,   339,   481,  -894,  5345,  -894,
    1380,  6309,  1050,  -894,    -4,  -894,  1668,   442,  1063,  1054,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,   518,  -894,
    -894,  -894,  1065,  -894,  1067,  3800,   704,  1115,   321,  5487,
     739,  1068,  6309,  -894,  7002,  -894,  6309,  -894,    59,  -894,
    6309,  6309,  1078,  4820,  1069,  1070,  1071,  1072,  1074,  1079,
    1080,   755,  4820,  1081,  1082,   757,     4,   -19,  7101,  -894,
       4,   194,   310,   -46,   -37,    12,  -894,    44,  -894,  -894,
    -894,  1077,   888,  1955,   761,  1084,  1091,  1092,  1093,  1099,
    1107,  -894,  1075,    56,  1096,  1066,  1119,  1102,     4,     4,
       4,  1110,  1111,  1127,  6309,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,     4,  1105,  6309,     4,  -894,  1237,  1238,  -894,
      23,  1077,   890,    70,   399,   289,  2098,  -894,   893,  -894,
     897,  6309,  -894,  -894,   535,  1239,  3658,  6309,  6309,   387,
      91,   541,  -894,   167,  6309,  6309,  -894,  6309,  6309,  -894,
    6309,  6309,  6309,  6309,  6309,  6309,  6309,  6309,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  7200,  5629,  -894,  1524,  1120,
     899,  1121,  -894,   901,  4820,   774,  1114,  1117,   735,  6309,
    -894,  -894,  6309,  -894,  -894,  6309,  -894,   904,  -894,   908,
     910,  -894,  6309,  1175,  1248,   913,  -894,   939,   941,  -894,
    -894,  -894,   921,  -894,  6309,  6309,  6309,   923,  -894,    83,
    -894,  1128,  3800,   775,  1129,  1077,   925,  1131,  1133,  6309,
     927,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,   931,
    -894,  -894,  -894,  1043,  -894,  -894,  3800,     4,  -894,  -894,
    1253,  -894,  1254,  1259,  1257,  6309,  1149,  1269,  1270,  -894,
    1077,   476,  5913,  6012,  2439,  -894,   420,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  6309,     4,  -894,  -894,  -894,  1145,
    1146,  1255,  -894,  -894,  -894,  -894,  6309,  1148,  -894,  -894,
       4,  1142,  -894,  -894,    62,   701,   610,  1224,  -894,  -894,
    1077,  1077,   950,   952,  1274,  1275,  1276,  -894,  -894,  -894,
    -894,  1156,  -894,    32,   791,  1150,    14,  6309,  6309,  6309,
    6309,  6309,  6309,  -894,  -894,  -894,  -894,   130,   232,   293,
     359,   392,   397,   421,   451,   508,   513,   519,   546,  -894,
    -894,   954,  -894,   956,   958,  -894,  1158,  -894,  -894,   960,
    -894,  -894,  -894,  -894,  1159,  1161,  1162,  -894,  -894,  -894,
    1163,  6309,  1165,  -894,  4820,   776,  -894,  -894,  1164,  1215,
    1219,  -894,   962,  -894,  6309,  -894,  -894,  1077,  -894,  -894,
     780,  1168,  1170,   -19,  -894,   964,  1076,     4,     4,     4,
    -894,  -894,  6309,  -894,  -894,  -894,    34,  -894,   255,  -894,
    -894,  -894,   435,   373,  -894,   340,  6309,  6309,  6309,  6309,
    6309,  6309,  6309,  6309,  -894,     4,  1166,  -894,  -894,     4,
    1171,  6309,  -894,  -894,  -894,   602,   630,    48,   135,   601,
    1172,  1176,   834,  -894,  -894,  1077,  1077,  1299,  1305,  1307,
    -894,  6309,  5771,  1192,   526,  -894,   550,   566,   576,   579,
     580,   585,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  1179,  -894,
    -894,  -894,  -894,  -894,  6309,   969,  -894,  -894,  6309,  6309,
    4820,   781,  -894,  -894,  6309,  -894,  -894,  -894,  -894,     4,
       4,  -894,  -894,  -894,  -894,  -894,  1303,  6309,  1195,  1309,
    1310,  6309,  6309,  6309,  -894,  -894,  -894,   594,   598,   623,
     625,   627,   633,   639,   645,  -894,  -894,  -894,  6309,  1183,
    -894,   151,  1244,  -894,   197,  -894,  1311,  1312,  1187,  6309,
    -894,  1315,  1316,  1194,  6309,  -894,   139,   212,  1196,  1197,
    -894,  -894,   667,  -894,  -894,  -894,  -894,  -894,   653,  1188,
     971,  6309,  6309,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    1198,  1199,  -894,  -894,   973,  -894,   799,  1085,  -894,  -894,
    -894,  6309,  -894,  -894,   655,   657,   665,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  1201,     4,  1200,   977,  1191,
    2241,  1200,   979,  1202,  1203,  -894,  1204,   437,   446,  -894,
    1205,  1330,  1332,  1210,  6309,  -894,  1334,  1335,  1213,  6309,
    -894,  -894,  -894,   278,  -894,  -894,  -894,  1209,   674,   676,
    -894,  -894,  1217,  -894,     4,  -894,  -894,  -894,  -894,  -894,
    6309,  -894,  -894,  1200,  -894,   448,  5913,  6012,  2538,  -894,
     450,  -894,  1200,  -894,  -894,  -894,  6309,  -894,  6309,  -894,
    -894,  1218,  1226,  -894,  1228,  1230,  1231,  -894,  1232,  1200,
     988,  -894,  -894,  -894,  -894,  -894,  1222,  -894,  6309,   461,
     395,  -894,   379,  6309,  6309,  6309,  6309,  6309,  6309,  6309,
    6309,  -894,  -894,  1234,  1235,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  1200,     4,   682,  6309,  6309,  6309,  -894,  -894,
    -894,   684,   686,   687,   688,   696,   705,   708,   709,  -894,
    -894,  -894,  -894,  -894,   727,   728,   729,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   456,   457,   458,   459,   461,   462,
     469,   470,   488,   487,   489,   490,     0,     0,   471,   472,
     473,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   478,   460,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     3,     4,   105,   106,   116,   117,   118,   119,   107,
     108,   126,     0,   127,   128,   130,   129,   115,     0,   346,
     348,   350,   352,   369,   371,   374,   376,   379,   381,   384,
     387,   394,   400,   402,   418,   452,     0,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   467,   468,     0,     0,     0,     0,   462,   469,   470,
     488,   487,     0,     0,   503,   398,   462,   469,   495,     0,
       0,     0,    38,     0,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,     0,   504,   344,   343,     0,    52,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   153,     0,   156,     0,     0,   120,     0,
       0,   121,     0,     0,     0,     3,     0,   449,   451,   109,
       3,   114,   445,    54,     0,   222,   223,   227,     0,     0,
       0,   399,     0,     0,   463,   464,   397,   396,   395,   405,
     417,   410,   411,   412,   413,   416,   414,   415,   408,   409,
     406,   407,   444,   462,   499,     0,     0,   491,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,   103,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   339,   340,
     341,   342,   336,   185,   425,     0,     0,   337,     0,     0,
       0,     0,     0,    42,   430,     0,   429,   487,   421,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,    29,     0,    31,     0,   499,     0,
       0,    41,   455,     0,     6,     0,    10,     0,    11,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
      46,     0,     0,   134,     0,   146,     0,     0,   165,     0,
       0,     0,     3,   174,    49,     0,     0,     0,    51,     0,
       0,    57,     0,     0,     0,     0,   170,     0,     0,     0,
       0,     0,   149,     0,   152,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,   448,   450,     0,     0,
       0,     0,    53,     0,   230,     0,     0,     0,   267,     3,
       0,   500,     0,     0,     0,     0,   492,   302,   462,   469,
     470,   488,     0,     0,     0,     0,     0,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,   349,     0,   351,
     353,   354,   357,   358,   355,   356,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   370,   372,   373,   375,
     377,   378,   380,   382,   383,   385,   386,   391,   392,   393,
     388,   389,   390,   401,   441,     0,   438,    64,     0,    98,
      99,   100,   101,   102,    61,     0,     0,   424,     0,    15,
       0,     0,     0,     0,   474,   479,   480,    66,     0,   433,
       0,     0,     0,   422,     0,   420,   455,     0,     0,     0,
     338,   339,   340,   341,   342,   336,   337,   497,     0,    30,
      32,    40,     0,   454,     0,     0,     0,   459,   487,     0,
       0,     0,     0,    26,     0,    27,     0,    28,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   500,     0,     0,   137,
     500,     0,     0,   377,   378,   279,   282,     0,   295,   297,
     299,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,   419,     0,     0,     0,     0,     0,   500,     0,
     500,     0,     0,     0,     0,   122,   123,   124,   125,   335,
     447,   110,     0,     0,     0,     0,   111,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,   443,     0,   501,
       0,     0,   404,   493,     0,     0,     0,     0,     0,     0,
       0,     0,   328,     0,     0,     0,   329,     0,     0,   330,
       0,     0,     0,     0,     0,     0,     0,     0,   303,    68,
      70,    72,    75,    73,    74,    69,    71,    81,    83,    85,
      88,    86,    87,    82,    84,     0,     0,   434,     0,     0,
       0,     0,   184,     0,     0,     0,     0,     0,     0,     0,
     484,   486,     0,   483,   485,     0,   432,     0,   428,     0,
       0,   271,     0,     0,     0,     0,   474,   479,   480,   498,
      39,     5,     0,    12,     0,     0,     0,     0,    19,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    92,    91,    95,    93,    94,    90,    44,     0,
      96,    97,    47,   131,   136,   135,     0,     0,   145,   162,
       0,   164,     0,     0,     0,     0,   284,     0,     0,   173,
       0,     0,     0,     0,     0,   186,     0,    50,    55,    56,
      59,    60,    34,    33,     0,     0,   166,   481,   482,     0,
       0,   147,   150,   151,   154,   155,     0,     0,   168,   113,
       0,     0,   228,   229,     0,     0,     0,     0,   225,   182,
       0,     0,     0,     0,     0,     0,     0,   266,   226,   442,
     502,     0,   494,   185,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,   319,   326,   327,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     440,     0,   437,     0,     0,    63,     0,    65,   183,     0,
      16,   477,   476,   475,     0,     0,     0,   431,   426,   427,
       0,     0,     0,   423,     0,     0,   276,   275,     0,     0,
       0,    21,     0,    22,     0,    25,   176,     0,    36,    35,
       0,   502,   502,     0,   138,     0,   139,     0,     0,     0,
     280,   291,     0,   285,   286,   172,   287,   283,     0,   296,
     298,   300,     0,     0,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   187,     0,     0,    79,    80,     0,
       0,     0,   169,   112,   224,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   179,     0,     0,     0,     0,     0,
       9,     0,     0,   474,   480,   323,     0,     0,     0,     0,
       0,     0,   331,   332,   333,   334,   304,   306,   308,   311,
     309,   310,   305,   307,   439,   436,   435,    62,   502,    76,
      77,    78,   272,   273,     0,     0,    13,    20,     0,     0,
       0,     0,   175,     7,     0,    45,    48,   133,   132,   500,
       0,   143,   163,   160,   161,   293,     0,     0,   288,     0,
       0,     0,     0,     0,   190,   191,   188,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   167,   148,     0,     0,
     159,     0,     3,   262,     0,   259,     0,     0,     0,     0,
     236,     0,     0,     0,     0,   231,     0,     0,     0,     0,
     232,   233,     0,   178,   180,   268,   269,   270,     0,   184,
       0,     0,     0,   313,   315,   318,   316,   317,   314,    17,
       0,   502,   278,   277,     0,    23,     0,   140,   141,   281,
     292,     0,   289,   290,     0,     0,     0,   192,   194,   196,
     199,   197,   198,   193,   195,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   249,     0,     0,     0,   250,
       0,     0,     0,     0,     0,   238,     0,     0,     0,     0,
     237,   234,   235,     0,   265,   322,   321,   183,     0,     0,
     274,    14,   502,     8,     0,   144,   294,   200,   202,   201,
       0,   158,   261,     0,   204,   488,     0,     0,     0,   203,
       0,   258,     0,   247,   248,   240,     0,   243,     0,   246,
     239,     0,     0,   251,     0,     0,     0,   252,     0,     0,
       0,   320,   324,   325,    24,   142,     0,   260,     0,     0,
       0,   208,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   206,   257,     0,     0,   255,   256,   242,   253,   254,
     241,   264,     0,     0,     0,     0,     0,     0,   209,   210,
     207,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,   263,   157,   205,     0,     0,     0,   211,   213,   215,
     218,   216,   217,   212,   214,   219,   221,   220
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -894,  -894,     1,  -894,  -571,  -757,  -894,  -894,  -894,  -894,
    -894,   207,  1329,  -894,  -894,  -894,  -894,  -894,  -345,  -893,
     -41,  -894,  -894,  -894,  -894,   737,  -894,  -894,  -509,   815,
    -421,   265,  1083,    -2,  -256,  -894,  1118,  1134,   813,  1101,
    -894,   609,   -16,  1097,   851,   852,    -8,  -894,  -143,  -292,
    -894,  -894,   428,  -894
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   240,    72,    73,   589,    74,    75,    76,    77,
      78,  1039,    79,    80,   421,   613,    81,    82,   383,  1043,
     207,    83,    84,    85,   423,   640,    86,   645,   320,   560,
     602,   134,   193,   144,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   236,   232,
     103,   104,   145,   105
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      88,     2,   504,   506,   642,   468,   904,   569,   135,   356,
      26,   723,   162,     3,   208,   139,   764,   163,    28,   212,
     213,   719,   154,   156,   158,   160,   161,   725,   186,   167,
     169,   171,   595,   596,   175,   176,   597,   774,   180,   182,
     124,   595,   596,   772,   637,   597,   197,   198,   187,   211,
     202,   204,   773,  1045,   638,   776,   216,   217,   218,  1016,
     515,   209,   314,  1046,   426,   117,   315,   235,   238,   174,
     595,   596,   189,  1047,   597,   516,   343,   177,   634,   635,
     210,   595,   596,   118,   282,   597,   368,   815,   816,   889,
     344,   817,   190,   282,   598,   599,   600,   664,   890,   794,
     795,   140,   141,   598,   599,   600,   369,   297,   298,   299,
     300,   301,   302,   316,   345,   307,   309,   665,   311,   427,
     368,   312,   777,   778,  1048,    71,   815,   816,   346,    71,
     817,   235,   598,   599,   600,   178,   775,   188,   335,   337,
     369,   339,   724,   598,   599,   600,    71,   746,    71,  1011,
    1051,   357,   527,   367,  1121,   372,   961,   374,  1017,   965,
    1052,   601,   595,   596,  1122,   125,   597,   126,   818,  1134,
     641,   962,  1049,   639,   146,   147,   148,   401,   149,   403,
     636,   191,   408,   667,   409,   410,  1050,   411,   412,   863,
     413,   415,   416,   413,   417,   370,   386,   150,   371,   745,
     386,   796,   151,   668,   280,   281,   188,   944,   595,   596,
     821,  1053,   597,   666,  1152,  1123,   387,   331,  1161,   125,
     387,   126,   822,   823,   598,   599,   600,  1126,   341,   370,
     214,   342,   371,   591,   485,   322,   844,  1127,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   282,   458,   459,
     460,   461,   462,   463,   464,   465,   779,   466,   119,  1054,
    1187,   419,   420,  1124,   490,   491,  1018,    87,   625,  1202,
     598,   599,   600,  1055,   172,   972,   120,  1125,   173,   497,
     498,   499,   500,   501,   502,   503,  1211,   376,  1128,   595,
     596,  1107,   191,   597,   164,   166,   819,   670,   239,   669,
     671,   672,   152,   323,   518,   388,   192,   183,   185,   388,
     673,   215,   846,   674,   675,   199,   201,   676,   677,  1231,
     537,   324,   539,   540,   541,   542,   543,   544,   545,   546,
    1145,   397,  1007,  1019,  1020,   434,  1129,  1111,    71,   377,
     553,   554,    71,   556,   332,   561,   333,   399,   389,   398,
    1130,   598,   599,   600,   107,   108,   574,   575,   576,   577,
     578,   579,   580,   593,   594,   400,   583,   584,   390,   386,
     626,   824,   825,   826,   405,   117,   119,   973,   379,   380,
     349,   350,   351,   603,   605,   606,   116,   607,   608,   387,
     609,   610,   406,   656,   657,   531,   615,   164,   617,   770,
     896,   347,   368,   621,   622,   353,   258,   736,   386,   354,
     595,   596,   630,   678,   597,   348,   837,   632,  1179,   359,
     838,   282,   369,   373,   368,   375,   378,   200,   387,   839,
     646,   627,   840,   841,   827,   915,   842,   239,   974,   859,
     659,   660,   661,   663,   369,   402,   125,   404,   126,   926,
     259,   342,   927,   928,    71,   710,   711,   391,   125,   435,
     126,   436,   929,   712,  1021,   930,   931,   359,  1022,   932,
     933,   262,   598,   599,   600,   953,   954,   361,   388,  1193,
     362,   363,  1194,  1195,  1023,  1026,   384,   916,   917,   385,
    1215,   918,  1196,   359,  1216,  1197,  1198,   628,   562,  1199,
    1200,   564,   566,   361,   975,    87,   362,   363,   205,   206,
    1217,   370,   563,   701,   371,   565,   567,   388,  1025,   706,
     707,   121,   122,   946,  1220,   365,   121,   122,   366,   721,
     687,   279,   843,   370,   553,   688,   371,   976,   123,   523,
    1219,   524,   977,   658,   305,   263,   264,   265,   266,   919,
     920,   921,  1002,   732,   321,   121,   122,   235,   258,   689,
     741,  1166,   743,   293,   744,   934,   978,   294,   747,   748,
    1168,   340,  1188,   365,   287,  1167,   366,   288,   289,   296,
    1024,   267,   268,   523,  1169,   726,   767,   358,   258,   690,
     359,   360,   290,   291,   292,  1201,   979,   713,   714,   365,
     361,   786,   366,   362,   363,   715,  1218,   364,   295,   184,
    1063,  1064,  1056,   241,    71,  1057,   242,   243,  1058,  1059,
    1108,   947,   807,  1112,   948,   949,   244,   950,   951,   245,
     246,   303,   810,   247,   248,   107,   108,   325,   326,   327,
     328,   329,   713,   714,    88,   519,   691,   310,   520,   831,
    1072,   692,   330,   980,   834,   835,   836,   693,   981,   342,
    1062,   729,   847,   848,   982,   849,   850,   116,   851,   852,
     853,   854,   855,   856,   857,   858,   342,   250,   832,   384,
     251,   252,   385,   631,   694,   633,   845,   381,   751,   313,
     253,   983,   382,   254,   255,  1073,   365,   256,   257,   366,
     568,   482,   483,   484,   753,   382,   352,   874,   205,   206,
     875,  1074,  1180,   876,   754,   322,   219,   755,   756,   392,
     880,  1075,   249,   757,  1076,  1077,   220,   355,   221,   222,
    1078,   393,   687,   886,   887,   394,   688,   223,   224,  1097,
     892,  1041,   225,  1098,   260,   261,  1042,   900,   226,   227,
     269,   270,   271,   272,   273,   274,   228,   229,   230,   231,
     548,   689,   275,   690,   905,   691,   396,   945,  1099,  1044,
    1100,   692,  1101,   911,  1042,   219,   276,   693,  1102,   505,
     922,   923,   925,   694,  1103,   220,   258,   221,   222,   422,
    1104,   867,   935,   751,   581,   753,   223,   224,  1135,   585,
    1147,   225,  1148,   757,   940,   592,  1133,   226,   227,   395,
    1149,  1042,   989,   604,   991,   228,   229,   230,   231,  1182,
     527,  1183,   687,   612,   688,   689,   690,  1233,   418,  1237,
     437,  1238,  1239,  1240,   691,   966,   967,   968,   969,   970,
     971,  1241,   733,   692,   424,   734,   693,   694,   509,   219,
    1242,   763,   765,  1243,  1244,   768,   769,   771,   648,   220,
     650,   221,   222,   654,   425,   751,   753,   757,    87,   430,
     223,   224,  1245,  1246,  1247,   225,   342,   738,   873,   993,
     739,   226,   227,   801,   802,   803,   487,   488,   489,   228,
     229,   230,   231,   758,   428,   762,   342,   808,   342,   787,
     811,   106,   342,   107,   108,   109,   110,   111,   112,   113,
    1015,    87,   870,   893,   996,   342,   894,   342,  1003,  1085,
     114,  1004,   342,   429,  1027,  1028,  1029,  1030,  1031,  1032,
    1033,  1034,   523,   115,   963,   116,   700,  1143,   510,  1040,
     342,   511,   277,   278,   703,   283,   284,   512,   705,   285,
     286,   708,   121,   122,   507,   508,   717,   513,   720,  1068,
     514,   107,   108,   325,   326,   327,   328,   329,   106,   525,
     107,   108,   109,   110,   111,   112,   113,   526,   330,   431,
     432,   433,   434,   517,   342,   587,   588,   114,   521,   522,
     533,   534,  1080,   116,  -403,  -403,  1082,  1083,   535,   536,
     655,   750,   116,   527,   552,   342,   572,   434,   431,   573,
     759,   431,   582,   586,   432,  1090,   590,   432,   528,  1094,
    1095,  1096,   618,   432,   620,   432,   431,   704,   780,   781,
     820,   781,   906,   829,   342,   529,  1105,   830,   342,   866,
     342,   868,   342,  1110,   877,   342,   530,  1116,   878,   534,
     879,   342,  1120,   883,   534,   710,   711,   713,   714,   532,
     936,   431,   884,   888,   739,   897,   781,   901,   342,  1138,
    1139,   902,   342,   903,   588,   942,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   538,  1146,
     955,   781,   956,   781,   984,   342,   985,   534,   986,   342,
     988,   342,   431,  1000,  1009,   432,  1010,   588,  1160,  1081,
     342,  1137,   342,  1142,   342,  1144,   588,  1153,   781,  1162,
     781,   549,  1174,   550,   861,   551,   864,  1178,  1212,   781,
     194,   196,   869,   570,   493,   494,   571,   495,   496,   611,
     614,   616,   619,   623,   624,   625,   626,   629,  1040,   627,
     651,   628,   652,   643,  1189,  1190,  1192,   644,   695,   679,
     680,   681,   885,   682,  1203,   683,  1204,   684,  1008,   685,
     686,   687,  1012,  1013,  1014,   688,   689,   690,   691,   692,
     693,   276,   282,   694,   697,   696,  1214,   698,   709,   722,
     727,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,   728,
    1035,   735,   749,   730,  1037,   731,   740,   751,   752,   753,
     754,   798,   755,  1234,  1235,  1236,   342,   756,   757,   760,
     761,   382,   788,   797,     4,     5,     6,     7,     8,   789,
     790,   791,   136,   137,   129,   130,   317,   792,    14,    15,
      16,    17,    18,    19,    20,   793,   799,   800,   804,   805,
     809,   806,   812,   813,   833,   132,    30,   871,   865,   867,
     872,   881,    36,   882,   907,   908,   891,   895,    38,   898,
     909,   899,   910,   912,  1087,  1088,    46,    47,    48,    49,
     913,   914,    52,   937,   938,   939,   941,   943,   952,   957,
     958,   959,    57,    58,   960,   964,   987,   989,    59,   990,
     991,   998,   997,   992,   994,   999,  1005,  1065,  1006,  1038,
    1060,  1036,   995,  1066,  1061,  1067,  1071,  1079,  1089,  1091,
    1092,  1093,  1001,  1106,  1109,  1115,  1113,  1114,    62,    63,
    1117,  1118,  1119,  1136,  1131,  1132,  1154,  1141,  1140,  1150,
    1163,  1164,  1165,  1170,  1042,  1171,    64,  1172,  1173,  1175,
    1176,  1177,    65,    66,  1181,  1184,  1205,  1186,    67,   165,
     133,   318,  1213,    70,  1206,    71,  1207,   319,  1208,  1209,
    1210,  1151,  1229,  1230,   737,    87,   814,   486,   467,   492,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
    1070,   136,   137,   129,   130,   317,   469,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,  1185,
       0,     0,     0,     0,   132,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,  1084,     0,
       0,     0,  1086,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1232,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,     0,     0,     0,     0,    67,     0,   133,
     718,     0,    70,     0,    71,     0,   319,     4,     5,     6,
       7,     8,     0,     0,     0,   136,   137,   129,   130,   317,
       0,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   132,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,    65,    66,     0,     0,     0,
       0,    67,     0,   133,   862,     0,    70,     0,    71,     0,
     319,     4,     5,     6,     7,     8,     0,     0,     0,   136,
     137,   129,   130,   317,     0,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,    65,
      66,     0,     0,     0,     0,    67,     0,   133,     0,     0,
      70,     0,    71,     0,   319,     4,     5,     6,     7,     8,
       0,     0,     0,   438,   439,   440,   441,    13,     0,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,   442,
     443,    26,     0,     0,    27,    28,    29,    30,   444,   445,
      33,    34,    35,    36,    37,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,   446,   447,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,    60,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,    65,    66,     0,     0,     0,     0,    67,
      68,    69,     0,     0,    70,     0,    71,   448,     4,     5,
       6,     7,     8,     0,     0,     0,     9,    10,    11,    12,
      13,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,   782,   783,    26,     0,     0,    27,    28,    29,
      30,    31,   784,    33,    34,    35,    36,    37,     0,     0,
       0,     0,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,    60,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,    65,    66,     0,     0,
       0,     0,    67,    68,    69,     0,     0,    70,     0,    71,
     785,     4,     5,     6,     7,     8,     0,     0,     0,     9,
      10,    11,    12,    13,     0,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,     0,     0,     0,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,    60,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,    65,
      66,     0,     0,     0,     0,    67,    68,    69,     0,     0,
      70,     0,    71,   828,     4,     5,     6,     7,     8,     0,
       0,     0,     9,    10,    11,  1155,    13,     0,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,  1156,  1157,
      26,     0,     0,    27,    28,    29,    30,    31,  1158,    33,
      34,    35,    36,    37,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,    62,    63,
       0,   136,   137,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,   132,    30,     0,     0,    67,    68,
      69,    36,     0,    70,     0,    71,  1159,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,    62,    63,     0,
     136,   137,   129,   130,   131,     0,    14,    15,    16,    17,
      18,    19,    20,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,   132,    30,     0,     0,    67,   168,   133,
      36,     0,    70,     0,    71,   662,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,    62,    63,     0,   136,
     137,   129,   130,   131,     0,    14,    15,    16,    17,    18,
      19,    20,     0,     0,    64,     0,     0,     0,     0,     0,
      65,    66,   132,    30,     0,     0,    67,   168,   133,    36,
       0,    70,     0,    71,   924,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,    65,
      66,     0,     0,     0,     0,    67,   168,   133,     0,     0,
      70,     0,    71,  1191,     4,     5,     6,     7,     8,     0,
       0,     0,     9,    10,    11,    12,    13,     0,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,    62,    63,
       0,   136,   137,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,   132,    30,     0,     0,    67,    68,
      69,    36,     0,    70,     0,    71,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,    62,    63,     0,
     136,   137,   129,   130,   131,     0,    14,    15,    16,    17,
      18,    19,    20,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,   132,    30,     0,     0,    67,     0,   133,
      36,     0,    70,   138,    71,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,    62,    63,     0,   136,
     137,   129,   130,   131,     0,    14,    15,    16,    17,    18,
      19,    20,     0,     0,    64,     0,     0,     0,     0,     0,
      65,    66,   132,    30,     0,     0,    67,   142,   143,    36,
       0,    70,     0,    71,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,    62,    63,     0,   136,   137,
     129,   130,   131,     0,    14,    15,    16,    17,    18,    19,
      20,     0,     0,    64,     0,     0,     0,     0,     0,    65,
      66,   132,    30,     0,     0,    67,   168,   133,    36,     0,
      70,     0,    71,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,    62,    63,     0,   136,   137,   129,
     130,   131,     0,    14,    15,    16,    17,    18,    19,    20,
       0,     0,    64,     0,     0,     0,     0,     0,    65,    66,
     132,    30,     0,     0,    67,   170,   133,    36,     0,    70,
       0,    71,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,    62,    63,     0,   233,   137,   129,   130,
     131,     0,    14,    15,    16,    17,    18,    19,    20,     0,
       0,    64,     0,     0,     0,     0,     0,    65,    66,   132,
      30,     0,     0,    67,   203,   133,    36,     0,    70,     0,
      71,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,    65,    66,     0,     0,
       0,     0,    67,     0,   133,   234,     0,    70,     0,    71,
       4,     5,     6,     7,     8,     0,     0,     0,   136,   137,
     129,   130,   131,     0,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,    62,    63,     0,   136,   137,   129,
     130,   131,     0,    14,    15,    16,    17,    18,    19,    20,
       0,     0,    64,     0,     0,     0,     0,     0,    65,    66,
     132,    30,     0,     0,    67,     0,   133,    36,     0,    70,
     237,    71,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,    65,    66,     0,
       0,     0,     0,    67,     0,   133,   304,     0,    70,     0,
      71,     4,     5,     6,     7,     8,     0,     0,     0,   136,
     137,   129,   130,   131,     0,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,    65,
      66,   308,     0,     0,     0,    67,     0,   133,     0,     0,
      70,     0,    71,     4,     5,     6,     7,     8,     0,     0,
       0,   136,   137,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   132,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,     0,     0,     0,     0,    67,     0,   133,
     234,     0,    70,     0,    71,     4,     5,     6,     7,     8,
       0,     0,     0,   136,   137,   129,   130,   131,     0,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   132,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,    62,
      63,     0,   136,   137,   129,   130,   131,     0,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    64,     0,     0,
       0,     0,     0,    65,    66,   132,    30,     0,     0,    67,
     334,   133,    36,     0,    70,     0,    71,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,    62,    63,
       0,   136,   137,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,   132,    30,     0,     0,    67,   336,
     133,    36,     0,    70,     0,    71,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,     0,     0,     0,     0,    67,     0,   133,
     338,     0,    70,     0,    71,     4,     5,     6,     7,     8,
       0,     0,     0,   136,   137,   129,   130,   131,     0,    14,
      15,    16,    17,    18,    19,    20,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,   132,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,    62,
      63,     0,   136,   137,   129,   130,   131,     0,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    64,     0,     0,
       0,     0,     0,    65,    66,   132,    30,     0,     0,    67,
       0,   133,    36,     0,    70,     0,    71,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,    62,    63,
       0,   136,   137,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,   132,    30,     0,     0,    67,     0,
     133,    36,   414,    70,     0,    71,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
     239,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,    62,    63,     0,
     136,   137,   129,   130,   131,     0,    14,    15,    16,    17,
      18,    19,    20,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,   132,    30,     0,     0,    67,     0,   133,
      36,     0,    70,     0,    71,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,    62,    63,     0,   136,
     137,   129,   130,   131,     0,    14,    15,    16,    17,    18,
      19,    20,     0,     0,    64,     0,     0,     0,     0,     0,
      65,    66,   132,    30,     0,     0,    67,     0,   133,    36,
       0,    70,   547,    71,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,    65,
      66,     0,     0,     0,     0,    67,     0,   133,   647,     0,
      70,     0,    71,     4,     5,     6,     7,     8,     0,     0,
       0,   136,   137,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   132,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,     0,     0,     0,     0,    67,     0,   133,
     649,     0,    70,     0,    71,     4,     5,     6,     7,     8,
       0,     0,     0,   136,   137,   129,   130,   131,     0,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   132,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,    62,
      63,     0,   136,   137,   129,   130,   131,     0,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    64,     0,     0,
       0,     0,     0,    65,    66,   132,    30,     0,     0,    67,
       0,   133,    36,     0,    70,   653,    71,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,     0,     0,     0,     0,    67,     0,
     133,   699,     0,    70,     0,    71,     4,     5,     6,     7,
       8,     0,     0,     0,   136,   137,   129,   130,   131,     0,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,    65,    66,     0,     0,     0,     0,
      67,     0,   133,   702,     0,    70,     0,    71,     4,     5,
       6,     7,     8,     0,     0,     0,   136,   137,   129,   130,
     131,     0,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   132,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,    65,    66,     0,     0,
       0,     0,    67,     0,   133,   716,     0,    70,     0,    71,
       4,     5,     6,   557,     8,     0,     0,     0,   136,   137,
     129,   130,   558,     0,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,    65,    66,
       0,     0,     0,     0,    67,     0,   133,   234,     0,    70,
       0,    71,     4,     5,     6,     7,     8,     0,     0,     0,
     136,   137,   129,   130,   131,     0,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   132,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
      65,    66,     0,     0,     0,     0,    67,     0,   133,   860,
       0,    70,     0,    71,     4,     5,     6,     7,     8,     0,
       0,     0,   136,   137,   129,   130,   131,     0,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   132,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,     0,     0,     0,     0,    67,     0,
     133,  1069,     0,    70,     0,    71,     4,     5,     6,     7,
       8,     0,     0,     0,   136,   137,   129,   130,   131,     0,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
      62,    63,     0,   136,   137,   129,   130,   131,     0,    14,
      15,    16,    17,    18,    19,    20,     0,     0,    64,     0,
       0,     0,     0,     0,    65,    66,   132,    30,     0,     0,
      67,     0,   153,    36,     0,    70,     0,    71,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,    62,
      63,     0,   136,   137,   129,   130,   131,     0,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    64,     0,     0,
       0,     0,     0,    65,    66,   132,    30,     0,     0,    67,
       0,   155,    36,     0,    70,     0,    71,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,    62,    63,
       0,   136,   137,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,   132,    30,     0,     0,    67,     0,
     157,    36,     0,    70,     0,    71,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,    62,    63,     0,
     136,   137,   129,   130,   131,     0,    14,    15,    16,    17,
      18,    19,    20,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,   132,    30,     0,     0,    67,     0,   159,
      36,     0,    70,     0,    71,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,    62,    63,     0,   136,
     137,   129,   130,   131,     0,    14,    15,    16,    17,    18,
      19,    20,     0,     0,    64,     0,     0,     0,     0,     0,
      65,    66,   132,    30,     0,     0,    67,     0,   133,    36,
       0,    70,     0,    71,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,    62,    63,     0,   136,   137,
     129,   130,   131,     0,    14,    15,    16,    17,    18,    19,
      20,     0,     0,    64,     0,     0,     0,     0,     0,    65,
      66,   132,    30,     0,     0,    67,     0,   179,    36,     0,
      70,     0,    71,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,    62,    63,     0,   136,   137,   129,
     130,   131,     0,    14,    15,    16,    17,    18,    19,    20,
       0,     0,    64,     0,     0,     0,     0,     0,    65,    66,
     132,    30,     0,     0,    67,     0,   181,    36,     0,    70,
       0,    71,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,    62,    63,     0,   136,   137,   129,   130,
     131,     0,    14,    15,    16,    17,    18,    19,    20,     0,
       0,    64,     0,     0,     0,     0,     0,    65,    66,   132,
      30,     0,     0,    67,     0,   133,    36,     0,    70,     0,
     195,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,    62,    63,     0,   136,   137,   129,   130,   131,
       0,    14,    15,    16,    17,    18,    19,    20,     0,     0,
      64,     0,     0,     0,     0,     0,    65,    66,   132,    30,
       0,     0,    67,     0,   306,    36,     0,    70,     0,    71,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,   557,
       8,    62,    63,     0,   136,   137,   129,   130,   558,     0,
      14,    15,    16,    17,    18,    19,    20,     0,     0,    64,
       0,     0,     0,     0,     0,    65,    66,   132,    30,     0,
       0,    67,     0,   555,    36,     0,    70,     0,    71,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
      62,    63,     0,   136,   137,   129,   130,   131,     0,    14,
      15,    16,    17,    18,    19,    20,     0,     0,    64,     0,
       0,     0,     0,     0,    65,    66,   132,    30,     0,     0,
      67,     0,   559,    36,     0,    70,     0,    71,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,    62,
      63,     0,   136,   137,   129,   130,   131,     0,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    64,     0,     0,
       0,     0,     0,    65,    66,   132,    30,     0,     0,    67,
       0,   742,    36,     0,    70,     0,    71,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,    62,    63,
       0,   127,   128,   129,   130,   131,     0,    14,    15,    16,
      17,    18,    19,    20,     0,     0,    64,     0,     0,     0,
       0,     0,    65,    66,   132,     0,     0,     0,    67,     0,
     766,    36,     0,    70,     0,    71,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -503,  -503,     0,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    65,    66,     0,     0,     0,     0,    67,     0,   133,
       0,     0,    70,     0,    71,  -503,  -503,  -503,     0,  -503,
    -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,
    -503,  -503,     0,     0,     0,  -503,     0,     0,     0,     0,
       0,     0,     0,  -503,  -503,     0,     0,  -503,  -503,  -503,
    -503,  -503,  -503,     0,     0,  -503,  -503,  -503,  -503,  -503,
       0,     0,  -503,  -503,  -503
};

static const yytype_int16 yycheck[] =
{
       2,     0,   294,   295,   425,   261,   763,   352,    16,   152,
      29,    15,    28,     0,    55,    17,   587,    15,    33,    60,
      61,   530,    24,    25,    26,    27,    28,   536,    29,    31,
      32,    33,    11,    12,    36,    37,    15,    25,    40,    41,
      12,    11,    12,    89,    15,    15,    48,    49,    49,    57,
      52,    53,    89,   946,    25,    11,    64,    65,    66,    25,
     124,    15,    15,    15,    15,   124,    19,    69,    70,    15,
      11,    12,    29,    25,    15,   139,   124,    13,    43,    44,
      34,    11,    12,   142,   130,    15,    29,    64,    65,     6,
     138,    68,    49,   130,    73,    74,    75,    29,    15,    43,
      44,    15,    16,    73,    74,    75,    49,   109,   110,   111,
     112,   113,   114,    66,   124,   117,   118,    49,   120,    70,
      29,   123,    78,    79,    76,   144,    64,    65,   138,   144,
      68,   133,    73,    74,    75,    13,   124,   138,   140,   141,
      49,   143,   146,    73,    74,    75,   144,   568,   144,   906,
      15,   153,   138,   155,    15,   157,   124,   159,   124,   145,
      25,   140,    11,    12,    25,   137,    15,   139,   145,  1062,
     140,   139,   124,   144,    11,    12,    13,   179,    15,   181,
     145,   138,   184,    29,   186,   187,   138,   189,   190,   698,
     192,   193,   194,   195,   196,   138,    29,    34,   141,   140,
      29,   145,    39,    49,    87,    88,   138,   145,    11,    12,
     140,    76,    15,   145,  1107,    76,    49,   124,  1111,   137,
      49,   139,   643,   644,    73,    74,    75,    15,   138,   138,
      11,   141,   141,   376,   275,   142,   145,    25,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   130,   250,   251,
     252,   253,   254,   255,   256,   257,   601,   259,   126,   124,
    1153,    43,    44,   124,   280,   281,    11,     2,   138,  1162,
      73,    74,    75,   138,    11,   145,   144,   138,    15,   287,
     288,   289,   290,   291,   292,   293,  1179,    89,    76,    11,
      12,   140,   138,    15,    29,    30,   641,    29,    80,   145,
      32,    33,   139,   126,   306,   138,   144,    42,    43,   138,
      42,    11,   145,    45,    46,    50,    51,    49,    50,  1212,
     322,   144,   324,   325,   326,   327,   328,   329,   330,   331,
    1087,   126,   903,    78,    79,   141,   124,   140,   144,   141,
     342,   343,   144,   345,   141,   347,   143,   126,    29,   144,
     138,    73,    74,    75,   107,   108,   358,   359,   360,   361,
     362,   363,   364,   379,   380,   144,   368,   369,    49,    29,
     138,    82,    83,    84,    32,   124,   126,   145,    87,    88,
      11,    12,    13,   382,   386,   387,   139,   389,   390,    49,
     392,   393,    50,   142,   144,    86,   398,   132,   400,    89,
     745,   124,    29,   405,   406,    11,   138,    86,    29,    15,
      11,    12,   414,   145,    15,   138,    29,   419,   140,    32,
      33,   130,    49,   158,    29,   160,   161,   144,    49,    42,
     429,   138,    45,    46,   145,   780,    49,    80,   145,   695,
     442,   443,   444,   445,    49,   180,   137,   182,   139,    29,
      86,   141,    32,    33,   144,   116,   117,   138,   137,   141,
     139,   143,    42,   124,    29,    45,    46,    32,    33,    49,
      50,   100,    73,    74,    75,   820,   821,    42,   138,    29,
      45,    46,    32,    33,    49,   145,   138,    11,    12,   141,
      29,    15,    42,    32,    33,    45,    46,   138,   124,    49,
      50,   124,   124,    42,   145,   240,    45,    46,    15,    16,
      49,   138,   138,   515,   141,   138,   138,   138,   145,   521,
     522,   107,   108,   815,   145,   138,   107,   108,   141,   531,
     138,   114,   145,   138,   536,   138,   141,   145,   124,   141,
     145,   143,   145,   124,   116,    90,    91,    92,    93,    73,
      74,    75,   897,   555,   126,   107,   108,   559,   138,   138,
     562,   124,   564,   123,   566,   145,   145,   137,   570,   571,
     124,   143,   124,   138,   115,   138,   141,   118,   119,    15,
     145,   126,   127,   141,   138,   143,   588,    29,   138,   138,
      32,    33,   133,   134,   135,   145,   145,   116,   117,   138,
      42,   603,   141,    45,    46,   124,   145,    49,   137,   139,
     955,   956,    11,    29,   144,    14,    32,    33,    17,    18,
    1041,    11,   624,  1044,    14,    15,    42,    17,    18,    45,
      46,    15,   634,    49,    50,   107,   108,   109,   110,   111,
     112,   113,   116,   117,   646,   138,   138,    15,   141,   651,
     124,   138,   124,   145,   656,   657,   658,   138,   145,   141,
     952,   143,   664,   665,   145,   667,   668,   139,   670,   671,
     672,   673,   674,   675,   676,   677,   141,    29,   143,   138,
      32,    33,   141,   418,   138,   420,   145,   139,   138,   138,
      42,   145,   144,    45,    46,   145,   138,    49,    50,   141,
     139,     8,     9,    10,   138,   144,    15,   709,    15,    16,
     712,   145,  1133,   715,   138,   142,    15,   138,   138,   124,
     722,   145,   138,   138,   145,   145,    25,    11,    27,    28,
     145,   124,   138,   735,   736,    77,   138,    36,    37,   145,
     742,   139,    41,   145,   101,   102,   144,   749,    47,    48,
      94,    95,    96,    97,    98,    99,    55,    56,    57,    58,
     332,   138,   106,   138,   766,   138,   144,    66,   145,   139,
     145,   138,   145,   775,   144,    15,   120,   138,   145,    19,
     782,   783,   784,   138,   145,    25,   138,    27,    28,   138,
     145,   138,   794,   138,   366,   138,    36,    37,   145,   371,
     145,    41,   145,   138,   806,   377,   139,    47,    48,   141,
     145,   144,   138,   385,   138,    55,    56,    57,    58,   145,
     138,   145,   138,   395,   138,   138,   138,   145,    60,   145,
     145,   145,   145,   145,   138,   837,   838,   839,   840,   841,
     842,   145,   138,   138,   144,   141,   138,   138,   138,    15,
     145,   586,   587,   145,   145,   590,   591,   592,   430,    25,
     432,    27,    28,   435,   139,   138,   138,   138,   603,   139,
      36,    37,   145,   145,   145,    41,   141,   138,   143,   881,
     141,    47,    48,   618,   619,   620,   277,   278,   279,    55,
      56,    57,    58,   138,   144,   138,   141,   632,   141,   138,
     635,   105,   141,   107,   108,   109,   110,   111,   112,   113,
     912,   646,   138,   138,   138,   141,   141,   141,   138,   138,
     124,   141,   141,   144,   926,   927,   928,   929,   930,   931,
     932,   933,   141,   137,   143,   139,   508,   138,   138,   941,
     141,   138,   128,   129,   516,   121,   122,   138,   520,   131,
     132,   523,   107,   108,   138,   139,   528,   138,   530,   961,
     138,   107,   108,   109,   110,   111,   112,   113,   105,   127,
     107,   108,   109,   110,   111,   112,   113,   145,   124,   140,
     141,   140,   141,   140,   141,    30,    31,   124,   131,   132,
     140,   141,   994,   139,   140,   141,   998,   999,   140,   141,
     137,   573,   139,   138,   140,   141,   140,   141,   140,   141,
     582,   140,   141,   140,   141,  1017,   140,   141,   139,  1021,
    1022,  1023,   140,   141,   140,   141,   140,   141,   140,   141,
     140,   141,   767,   140,   141,    15,  1038,   140,   141,   140,
     141,   140,   141,  1042,   140,   141,   139,  1049,   140,   141,
     140,   141,  1054,   140,   141,   116,   117,   116,   117,    15,
     795,   140,   141,   140,   141,   140,   141,   140,   141,  1071,
    1072,   140,   141,    30,    31,   810,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,    15,  1091,
     140,   141,   140,   141,   140,   141,   140,   141,   140,   141,
     140,   141,   140,   141,   140,   141,    30,    31,  1110,   140,
     141,   140,   141,   140,   141,    30,    31,   140,   141,   140,
     141,   138,  1124,   138,   696,   138,   698,  1129,   140,   141,
      47,    48,   704,   124,   283,   284,   124,   285,   286,   138,
      15,    15,    30,    11,   138,   138,   138,   145,  1150,   138,
     124,   138,    11,   139,  1156,  1157,  1158,   139,   103,   138,
     138,   138,   734,   138,  1166,   138,  1168,   138,   903,   138,
     138,   138,   907,   908,   909,   138,   138,   138,   138,   138,
     138,   120,   130,   138,    15,   139,  1188,   139,   124,   139,
     127,  1193,  1194,  1195,  1196,  1197,  1198,  1199,  1200,   145,
     935,    86,   124,   138,   939,   138,   138,   138,   138,   138,
     138,   145,   138,  1215,  1216,  1217,   141,   138,   138,   138,
     138,   144,   138,   127,     3,     4,     5,     6,     7,   138,
     138,   138,    11,    12,    13,    14,    15,   138,    17,    18,
      19,    20,    21,    22,    23,   138,   127,   145,   138,   138,
     145,   124,    15,    15,    15,    34,    35,   143,   138,   138,
     143,    86,    41,    15,    11,    11,   138,   138,    47,   138,
      11,   138,    15,   124,  1009,  1010,    55,    56,    57,    58,
      11,    11,    61,   138,   138,    30,   138,   145,    64,    15,
      15,    15,    71,    72,   138,   145,   138,   138,    77,   138,
     138,    86,   138,   140,   139,    86,   138,     8,   138,   138,
     138,   145,   884,     8,   138,     8,   124,   138,    15,   124,
      11,    11,   894,   140,    80,   138,    15,    15,   107,   108,
      15,    15,   138,   145,   138,   138,   145,   138,   140,   138,
     138,   138,   138,   138,   144,    15,   125,    15,   138,    15,
      15,   138,   131,   132,   145,   138,   138,  1150,   137,    30,
     139,   140,   140,   142,   138,   144,   138,   146,   138,   138,
     138,  1106,   138,   138,   559,  1110,   639,   276,   260,   282,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
     962,    11,    12,    13,    14,    15,   262,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,  1144,
      -1,    -1,    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    47,  1000,    -1,
      -1,    -1,  1004,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,    -1,   142,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      -1,    17,    18,    19,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,    -1,   139,   140,    -1,   142,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,   139,    -1,    -1,
     142,    -1,   144,    -1,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,   142,    -1,   144,   145,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    -1,
      -1,    -1,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    -1,    67,    68,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,   138,   139,    -1,    -1,   142,    -1,   144,
     145,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
     142,    -1,   144,   145,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    -1,    -1,    -1,    -1,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    -1,    -1,    67,    68,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,   107,   108,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,   138,
     139,    41,    -1,   142,    -1,   144,   145,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,   107,   108,    -1,
      11,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    34,    35,    -1,    -1,   137,   138,   139,
      41,    -1,   142,    -1,   144,   145,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,   107,   108,    -1,    11,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
     131,   132,    34,    35,    -1,    -1,   137,   138,   139,    41,
      -1,   142,    -1,   144,   145,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
     142,    -1,   144,   145,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    -1,    -1,    -1,    -1,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    -1,    -1,    67,    68,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,   107,   108,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,   138,
     139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,   107,   108,    -1,
      11,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    34,    35,    -1,    -1,   137,    -1,   139,
      41,    -1,   142,   143,   144,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,   107,   108,    -1,    11,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
     131,   132,    34,    35,    -1,    -1,   137,   138,   139,    41,
      -1,   142,    -1,   144,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,   107,   108,    -1,    11,    12,
      13,    14,    15,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,
     132,    34,    35,    -1,    -1,   137,   138,   139,    41,    -1,
     142,    -1,   144,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,   107,   108,    -1,    11,    12,    13,
      14,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,   132,
      34,    35,    -1,    -1,   137,   138,   139,    41,    -1,   142,
      -1,   144,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,   107,   108,    -1,    11,    12,    13,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,   131,   132,    34,
      35,    -1,    -1,   137,   138,   139,    41,    -1,   142,    -1,
     144,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,   139,   140,    -1,   142,    -1,   144,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,   107,   108,    -1,    11,    12,    13,
      14,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,   132,
      34,    35,    -1,    -1,   137,    -1,   139,    41,    -1,   142,
     143,   144,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,   139,   140,    -1,   142,    -1,
     144,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,    -1,
     142,    -1,   144,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,    -1,   142,    -1,   144,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,   107,
     108,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,
     138,   139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,   107,   108,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,   138,
     139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,    -1,   142,    -1,   144,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,   107,
     108,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,
      -1,   139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,   107,   108,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,    -1,
     139,    41,   141,   142,    -1,   144,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,   107,   108,    -1,
      11,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    34,    35,    -1,    -1,   137,    -1,   139,
      41,    -1,   142,    -1,   144,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,   107,   108,    -1,    11,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
     131,   132,    34,    35,    -1,    -1,   137,    -1,   139,    41,
      -1,   142,   143,   144,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,    -1,
     142,    -1,   144,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,    -1,   142,    -1,   144,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,   107,
     108,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,
      -1,   139,    41,    -1,   142,   143,   144,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
     139,   140,    -1,   142,    -1,   144,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,   139,   140,    -1,   142,    -1,   144,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      35,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,   139,   140,    -1,   142,    -1,   144,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,    -1,   139,   140,    -1,   142,
      -1,   144,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,
      -1,   142,    -1,   144,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
     139,   140,    -1,   142,    -1,   144,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
     107,   108,    -1,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,   131,   132,    34,    35,    -1,    -1,
     137,    -1,   139,    41,    -1,   142,    -1,   144,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,   107,
     108,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,
      -1,   139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,   107,   108,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,    -1,
     139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,   107,   108,    -1,
      11,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    34,    35,    -1,    -1,   137,    -1,   139,
      41,    -1,   142,    -1,   144,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,   107,   108,    -1,    11,
      12,    13,    14,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
     131,   132,    34,    35,    -1,    -1,   137,    -1,   139,    41,
      -1,   142,    -1,   144,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,   107,   108,    -1,    11,    12,
      13,    14,    15,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,
     132,    34,    35,    -1,    -1,   137,    -1,   139,    41,    -1,
     142,    -1,   144,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,   107,   108,    -1,    11,    12,    13,
      14,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,   131,   132,
      34,    35,    -1,    -1,   137,    -1,   139,    41,    -1,   142,
      -1,   144,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,   107,   108,    -1,    11,    12,    13,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,   131,   132,    34,
      35,    -1,    -1,   137,    -1,   139,    41,    -1,   142,    -1,
     144,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,   107,   108,    -1,    11,    12,    13,    14,    15,
      -1,    17,    18,    19,    20,    21,    22,    23,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,   131,   132,    34,    35,
      -1,    -1,   137,    -1,   139,    41,    -1,   142,    -1,   144,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,   107,   108,    -1,    11,    12,    13,    14,    15,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,   131,   132,    34,    35,    -1,
      -1,   137,    -1,   139,    41,    -1,   142,    -1,   144,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
     107,   108,    -1,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,   131,   132,    34,    35,    -1,    -1,
     137,    -1,   139,    41,    -1,   142,    -1,   144,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,   107,
     108,    -1,    11,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,   131,   132,    34,    35,    -1,    -1,   137,
      -1,   139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,   107,   108,
      -1,    11,    12,    13,    14,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   131,   132,    34,    -1,    -1,    -1,   137,    -1,
     139,    41,    -1,   142,    -1,   144,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,   139,
      -1,    -1,   142,    -1,   144,    86,    87,    88,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,    -1,   118,   119,   120,
     121,   122,   123,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   148,   149,     0,     3,     4,     5,     6,     7,    11,
      12,    13,    14,    15,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    67,    68,    71,    72,    77,
      81,    85,   107,   108,   125,   131,   132,   137,   138,   139,
     142,   144,   150,   151,   153,   154,   155,   156,   157,   159,
     160,   163,   164,   168,   169,   170,   173,   178,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   197,   198,   200,   105,   107,   108,   109,
     110,   111,   112,   113,   124,   137,   139,   124,   142,   126,
     144,   107,   108,   124,    12,   137,   139,    11,    12,    13,
      14,    15,    34,   139,   178,   193,    11,    12,   143,   180,
      15,    16,   138,   139,   180,   199,    11,    12,    13,    15,
      34,    39,   139,   139,   180,   139,   180,   139,   180,   139,
     180,   180,   189,    15,   178,   159,   178,   180,   138,   180,
     138,   180,    11,    15,    15,   180,   180,    13,    13,   139,
     180,   139,   180,   178,   139,   178,    29,    49,   138,    29,
      49,   138,   144,   179,   179,   144,   179,   180,   180,   178,
     144,   178,   180,   138,   180,    15,    16,   167,   167,    15,
      34,   193,   167,   167,    11,    11,   193,   193,   193,    15,
      25,    27,    28,    36,    37,    41,    47,    48,    55,    56,
      57,    58,   196,    11,   140,   180,   195,   143,   180,    80,
     149,    29,    32,    33,    42,    45,    46,    49,    50,   138,
      29,    32,    33,    42,    45,    46,    49,    50,   138,    86,
     101,   102,   100,    90,    91,    92,    93,   126,   127,    94,
      95,    96,    97,    98,    99,   106,   120,   128,   129,   114,
      87,    88,   130,   121,   122,   131,   132,   115,   118,   119,
     133,   134,   135,   123,   137,   137,    15,   180,   180,   180,
     180,   180,   180,    15,   140,   199,   139,   180,   133,   180,
      15,   180,   180,   138,    15,    19,    66,    15,   140,   146,
     175,   199,   142,   126,   144,   109,   110,   111,   112,   113,
     124,   124,   141,   143,   138,   180,   138,   180,   140,   180,
     199,   138,   141,   124,   138,   124,   138,   124,   138,    11,
      12,    13,    15,    11,    15,    11,   195,   180,    29,    32,
      33,    42,    45,    46,    49,   138,   141,   180,    29,    49,
     138,   141,   180,   178,   180,   178,    89,   141,   178,    87,
      88,   139,   144,   165,   138,   141,    29,    49,   138,    29,
      49,   138,   124,   124,    77,   141,   144,   126,   144,   126,
     144,   180,   178,   180,   178,    32,    50,    26,   180,   180,
     180,   180,   180,   180,   141,   180,   180,   180,    60,    43,
      44,   161,   138,   171,   144,   139,    15,    70,   144,   144,
     139,   140,   141,   140,   141,   141,   143,   145,    11,    12,
      13,    14,    27,    28,    36,    37,    53,    54,   145,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   183,   181,   184,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,     8,     9,    10,   167,   186,   188,   188,   188,
     189,   189,   190,   191,   191,   192,   192,   193,   193,   193,
     193,   193,   193,   193,   196,    19,   196,   138,   139,   138,
     138,   138,   138,   138,   138,   124,   139,   140,   180,   138,
     141,   131,   132,   141,   143,   127,   145,   138,   139,    15,
     139,    86,    15,   140,   141,   140,   141,   180,    15,   180,
     180,   180,   180,   180,   180,   180,   180,   143,   199,   138,
     138,   138,   140,   180,   180,   139,   180,     6,    15,   139,
     176,   180,   124,   138,   124,   138,   124,   138,   139,   165,
     124,   124,   140,   141,   180,   180,   180,   180,   180,   180,
     180,   199,   141,   180,   180,   199,   140,    30,    31,   152,
     140,   195,   199,   189,   189,    11,    12,    15,    73,    74,
      75,   140,   177,   149,   199,   180,   180,   180,   180,   180,
     180,   138,   199,   162,    15,   180,    15,   180,   140,    30,
     140,   180,   180,    11,   138,   138,   138,   138,   138,   145,
     180,   178,   180,   178,    43,    44,   145,    15,    25,   144,
     172,   140,   177,   139,   139,   174,   149,   140,   199,   140,
     199,   124,    11,   143,   199,   137,   142,   144,   124,   180,
     180,   180,   145,   180,    29,    49,   145,    29,    49,   145,
      29,    32,    33,    42,    45,    46,    49,    50,   145,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   103,   139,    15,   139,   140,
     199,   180,   140,   199,   141,   199,   180,   180,   199,   124,
     116,   117,   124,   116,   117,   124,   140,   199,   140,   175,
     199,   180,   139,    15,   146,   175,   143,   127,   145,   143,
     138,   138,   180,   138,   141,    86,    86,   176,   138,   141,
     138,   180,   139,   180,   180,   140,   177,   180,   180,   124,
     199,   138,   138,   138,   138,   138,   138,   138,   138,   199,
     138,   138,   138,   178,   151,   178,   139,   180,   178,   178,
      89,   178,    89,    89,    25,   124,    11,    78,    79,   165,
     140,   141,    27,    28,    37,   145,   180,   138,   138,   138,
     138,   138,   138,   138,    43,    44,   145,   127,   145,   127,
     145,   178,   178,   178,   138,   138,   124,   180,   178,   145,
     180,   178,    15,    15,   172,    64,    65,    68,   145,   165,
     140,   140,   177,   177,    82,    83,    84,   145,   145,   140,
     140,   180,   143,    15,   180,   180,   180,    29,    33,    42,
      45,    46,    49,   145,   145,   145,   145,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   181,
     140,   199,   140,   175,   199,   138,   140,   138,   140,   199,
     138,   143,   143,   143,   180,   180,   180,   140,   140,   140,
     180,    86,    15,   140,   141,   199,   180,   180,   140,     6,
      15,   138,   180,   138,   141,   138,   165,   140,   138,   138,
     180,   140,   140,    30,   152,   180,   178,    11,    11,    11,
      15,   180,   124,    11,    11,   165,    11,    12,    15,    73,
      74,    75,   180,   180,   145,   180,    29,    32,    33,    42,
      45,    46,    49,    50,   145,   180,   178,   138,   138,    30,
     180,   138,   178,   145,   145,    66,   196,    11,    14,    15,
      17,    18,    64,   165,   165,   140,   140,    15,    15,    15,
     138,   124,   139,   143,   145,   145,   180,   180,   180,   180,
     180,   180,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   140,   140,   140,   138,   140,   138,
     138,   138,   140,   180,   139,   199,   138,   138,    86,    86,
     141,   199,   165,   138,   141,   138,   138,   151,   178,   140,
      30,   152,   178,   178,   178,   180,    25,   124,    11,    78,
      79,    29,    33,    49,   145,   145,   145,   180,   180,   180,
     180,   180,   180,   180,   180,   178,   145,   178,   138,   158,
     180,   139,   144,   166,   139,   166,    15,    25,    76,   124,
     138,    15,    25,    76,   124,   138,    11,    14,    17,    18,
     138,   138,   196,   165,   165,     8,     8,     8,   180,   140,
     199,   124,   124,   145,   145,   145,   145,   145,   145,   138,
     180,   140,   180,   180,   199,   138,   199,   178,   178,    15,
     180,   124,    11,    11,   180,   180,   180,   145,   145,   145,
     145,   145,   145,   145,   145,   180,   140,   140,   177,    80,
     149,   140,   177,    15,    15,   138,   180,    15,    15,   138,
     180,    15,    25,    76,   124,   138,    15,    25,    76,   124,
     138,   138,   138,   139,   166,   145,   145,   140,   180,   180,
     140,   138,   140,   138,    30,   152,   180,   145,   145,   145,
     138,   178,   166,   140,   145,    14,    27,    28,    37,   145,
     180,   166,   140,   138,   138,   138,   124,   138,   124,   138,
     138,    15,    15,   138,   180,    15,    15,   138,   180,   140,
     177,   145,   145,   145,   138,   178,   158,   166,   124,   180,
     180,   145,   180,    29,    32,    33,    42,    45,    46,    49,
      50,   145,   166,   180,   180,   138,   138,   138,   138,   138,
     138,   166,   140,   140,   180,    29,    33,    49,   145,   145,
     145,   180,   180,   180,   180,   180,   180,   180,   180,   138,
     138,   166,   178,   145,   180,   180,   180,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   147,   148,   149,   149,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   151,   151,   151,   151,   151,   151,   151,   151,   152,
     152,   152,   152,   152,   152,   153,   153,   154,   154,   154,
     154,   155,   155,   156,   156,   156,   157,   157,   157,   158,
     159,   159,   159,   159,   159,   159,   160,   160,   161,   161,
     162,   162,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   164,   164,   164,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   167,   167,   168,   169,   170,   171,   171,   171,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   173,   174,   174,   174,
     174,   175,   175,   175,   175,   176,   176,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   179,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   181,   181,   182,
     182,   183,   183,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     185,   185,   186,   186,   186,   187,   187,   188,   188,   188,
     189,   189,   190,   190,   190,   191,   191,   191,   192,   192,
     192,   192,   192,   192,   192,   193,   193,   193,   193,   193,
     193,   194,   194,   195,   195,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   197,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   199,   199,   199,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     9,     4,     6,     8,     5,     5,
       7,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       4,     3,     4,     5,     5,     6,     6,     4,     2,     5,
       4,     3,     3,     3,     5,     7,     3,     5,     7,     3,
       5,     3,     2,     3,     2,     5,     5,     3,     2,     5,
       5,     4,     7,     6,     4,     6,     4,     2,     4,     4,
       4,     4,     4,     4,     4,     4,     7,     7,     7,     6,
       6,     4,     4,     4,     4,     4,     4,     4,     4,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     4,     4,
       4,     4,     4,     2,     1,     1,     1,     1,     1,     2,
       4,     4,     6,     5,     2,     1,     1,     1,     1,     1,
       2,     2,     4,     4,     4,     4,     1,     1,     1,     1,
       1,     5,     7,     7,     3,     5,     5,     4,     6,     3,
       5,     5,     7,     4,     6,     5,     3,     5,     7,     3,
       5,     5,     3,     2,     5,     5,     2,    12,     9,     1,
       7,     7,     5,     7,     5,     3,     5,     7,     3,     4,
       0,     4,     6,     5,     3,     7,     6,     4,     7,     6,
       7,     6,     5,     6,     5,     3,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     3,     3,     6,     4,     5,     4,     5,
       5,     6,     6,     6,     6,     6,     6,     6,     6,     7,
       7,     7,     1,     1,     6,     5,     5,     0,     3,     3,
       0,     4,     4,     4,     5,     5,     4,     5,     5,     6,
       6,     7,     7,     6,     8,     8,     6,     6,     6,     5,
       5,     6,     6,     7,     7,     7,     7,     7,     6,     4,
       7,     6,     4,     8,     7,     5,     5,     0,     4,     4,
       4,     3,     5,     5,     7,     3,     3,     5,     5,     1,
       3,     5,     1,     3,     2,     3,     3,     3,     4,     5,
       5,     3,     5,     4,     6,     1,     3,     1,     3,     1,
       3,     3,     3,     4,     6,     6,     6,     6,     6,     6,
       6,     6,     5,     7,     7,     7,     7,     7,     7,     5,
       9,     8,     8,     6,     9,     9,     5,     5,     4,     4,
       4,     6,     6,     6,     6,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     3,     1,     5,     1,     3,
       1,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     2,     2,     2,
       1,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     3,     4,     6,     4,     3,     6,     6,     5,     3,
       3,     6,     5,     4,     4,     6,     6,     5,     3,     6,
       5,     3,     5,     4,     2,     2,     3,     4,     3,     2,
       3,     2,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     4,     6,     6,     6,     1,     4,
       4,     5,     5,     5,     5,     5,     5,     1,     1,     1,
       1,     2,     3,     4,     5,     2,     3,     4,     5,     2,
       3,     4,     5,     1,     2
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
#line 547 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3761 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 557 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3767 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 558 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3773 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 562 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3779 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 564 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3785 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 566 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3791 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 568 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3799 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 572 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3805 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 574 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3811 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 576 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3817 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 578 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3823 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 580 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3831 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 584 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3839 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 588 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3845 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 590 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3853 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 594 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3861 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 598 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3867 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 600 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3873 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 602 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3879 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3885 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3891 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 608 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3899 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 612 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3907 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 616 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3913 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 618 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3919 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 620 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3925 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 622 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3931 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 624 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3937 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 626 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3943 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 628 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3949 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 630 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3955 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 632 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3961 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 634 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3967 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 636 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3973 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 638 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3979 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 640 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3991 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 648 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3997 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 650 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); free((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4003 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 652 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 4009 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 654 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); free((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4015 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 656 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4021 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 658 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4027 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 660 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4034 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 663 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4041 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 666 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4047 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 668 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4054 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 671 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4061 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 674 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4067 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 676 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4075 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 680 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4081 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 682 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4087 "raku.tab.c"
    break;

  case 53: /* stmt: KW_FAIL expr ';'  */
#line 684 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4093 "raku.tab.c"
    break;

  case 54: /* stmt: KW_FAIL ';'  */
#line 686 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4099 "raku.tab.c"
    break;

  case 55: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 688 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4106 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 691 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4113 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr ';'  */
#line 694 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4119 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT ';'  */
#line 696 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4126 "raku.tab.c"
    break;

  case 59: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 699 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4133 "raku.tab.c"
    break;

  case 60: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 702 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4140 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 705 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4146 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 707 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4155 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 712 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4163 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 716 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4171 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 720 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4180 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 725 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4188 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall ';'  */
#line 728 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 4194 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 730 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4200 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 732 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4206 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 734 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4212 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 736 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4218 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 738 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4225 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 741 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4231 "raku.tab.c"
    break;

  case 74: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 743 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4237 "raku.tab.c"
    break;

  case 75: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 745 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4243 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 747 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4250 "raku.tab.c"
    break;

  case 77: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 750 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4257 "raku.tab.c"
    break;

  case 78: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 753 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4264 "raku.tab.c"
    break;

  case 79: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 756 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4271 "raku.tab.c"
    break;

  case 80: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 759 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4278 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_IF expr ';'  */
#line 762 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4284 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNLESS expr ';'  */
#line 764 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4290 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WHILE expr ';'  */
#line 766 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4296 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_UNTIL expr ';'  */
#line 768 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4302 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_FOR expr ';'  */
#line 770 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4309 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_WITH expr ';'  */
#line 773 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4315 "raku.tab.c"
    break;

  case 87: /* stmt: expr KW_WITHOUT expr ';'  */
#line 775 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4321 "raku.tab.c"
    break;

  case 88: /* stmt: expr KW_GIVEN expr ';'  */
#line 777 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4327 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 779 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4334 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 782 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4341 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 785 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4349 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 789 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4356 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 792 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4362 "raku.tab.c"
    break;

  case 94: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 794 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4368 "raku.tab.c"
    break;

  case 95: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 796 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4374 "raku.tab.c"
    break;

  case 96: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 798 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4381 "raku.tab.c"
    break;

  case 97: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 801 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4388 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 804 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4395 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 807 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4402 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 810 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4409 "raku.tab.c"
    break;

  case 101: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 813 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4416 "raku.tab.c"
    break;

  case 102: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 816 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4423 "raku.tab.c"
    break;

  case 103: /* stmt: expr ';'  */
#line 818 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4429 "raku.tab.c"
    break;

  case 104: /* stmt: ';'  */
#line 819 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4435 "raku.tab.c"
    break;

  case 105: /* stmt: if_stmt  */
#line 820 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4441 "raku.tab.c"
    break;

  case 106: /* stmt: while_stmt  */
#line 821 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4447 "raku.tab.c"
    break;

  case 107: /* stmt: for_stmt  */
#line 822 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4453 "raku.tab.c"
    break;

  case 108: /* stmt: given_stmt  */
#line 823 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4459 "raku.tab.c"
    break;

  case 109: /* stmt: KW_TRY block  */
#line 825 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4465 "raku.tab.c"
    break;

  case 110: /* stmt: KW_TRY block KW_CATCH block  */
#line 827 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4471 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 829 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4479 "raku.tab.c"
    break;

  case 112: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 833 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4488 "raku.tab.c"
    break;

  case 113: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 838 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4496 "raku.tab.c"
    break;

  case 114: /* stmt: KW_CATCH block  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4502 "raku.tab.c"
    break;

  case 115: /* stmt: block  */
#line 844 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4508 "raku.tab.c"
    break;

  case 116: /* stmt: unless_stmt  */
#line 845 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4514 "raku.tab.c"
    break;

  case 117: /* stmt: until_stmt  */
#line 846 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4520 "raku.tab.c"
    break;

  case 118: /* stmt: repeat_stmt  */
#line 847 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4526 "raku.tab.c"
    break;

  case 119: /* stmt: loop_stmt  */
#line 848 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4532 "raku.tab.c"
    break;

  case 120: /* stmt: KW_LAST ';'  */
#line 849 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4538 "raku.tab.c"
    break;

  case 121: /* stmt: KW_NEXT ';'  */
#line 850 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4544 "raku.tab.c"
    break;

  case 122: /* stmt: KW_LAST KW_IF expr ';'  */
#line 852 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4550 "raku.tab.c"
    break;

  case 123: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 854 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4556 "raku.tab.c"
    break;

  case 124: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 856 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4562 "raku.tab.c"
    break;

  case 125: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 858 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4568 "raku.tab.c"
    break;

  case 126: /* stmt: sub_decl  */
#line 859 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4574 "raku.tab.c"
    break;

  case 127: /* stmt: class_decl  */
#line 860 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4580 "raku.tab.c"
    break;

  case 128: /* stmt: role_decl  */
#line 861 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4586 "raku.tab.c"
    break;

  case 129: /* stmt: grammar_decl  */
#line 862 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4592 "raku.tab.c"
    break;

  case 130: /* stmt: module_decl  */
#line 863 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4598 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF '(' expr ')' block  */
#line 867 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4604 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4610 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 871 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4616 "raku.tab.c"
    break;

  case 134: /* if_stmt: KW_IF expr block  */
#line 873 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4622 "raku.tab.c"
    break;

  case 135: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 875 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4628 "raku.tab.c"
    break;

  case 136: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 877 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4634 "raku.tab.c"
    break;

  case 137: /* if_stmt: KW_IF expr block elsif_tail  */
#line 879 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4640 "raku.tab.c"
    break;

  case 138: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 881 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4646 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF expr block  */
#line 885 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4652 "raku.tab.c"
    break;

  case 140: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 887 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4658 "raku.tab.c"
    break;

  case 141: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 889 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4664 "raku.tab.c"
    break;

  case 142: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 891 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4670 "raku.tab.c"
    break;

  case 143: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 893 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4676 "raku.tab.c"
    break;

  case 144: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 895 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4682 "raku.tab.c"
    break;

  case 145: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 899 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4688 "raku.tab.c"
    break;

  case 146: /* while_stmt: KW_WHILE expr block  */
#line 901 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4694 "raku.tab.c"
    break;

  case 147: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 905 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4700 "raku.tab.c"
    break;

  case 148: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 907 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4706 "raku.tab.c"
    break;

  case 149: /* unless_stmt: KW_UNLESS expr block  */
#line 909 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4712 "raku.tab.c"
    break;

  case 150: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 911 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4718 "raku.tab.c"
    break;

  case 151: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 915 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4724 "raku.tab.c"
    break;

  case 152: /* until_stmt: KW_UNTIL expr block  */
#line 917 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4730 "raku.tab.c"
    break;

  case 153: /* repeat_stmt: KW_REPEAT block  */
#line 921 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4736 "raku.tab.c"
    break;

  case 154: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 923 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4742 "raku.tab.c"
    break;

  case 155: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 925 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4748 "raku.tab.c"
    break;

  case 156: /* loop_stmt: KW_LOOP block  */
#line 929 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4754 "raku.tab.c"
    break;

  case 157: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 931 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4760 "raku.tab.c"
    break;

  case 158: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 933 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4766 "raku.tab.c"
    break;

  case 159: /* loop_incr: expr  */
#line 936 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4772 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 940 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4782 "raku.tab.c"
    break;

  case 161: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 946 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4792 "raku.tab.c"
    break;

  case 162: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 952 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node));
          } else (yyval.node) = rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)); }
#line 4804 "raku.tab.c"
    break;

  case 163: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 960 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4814 "raku.tab.c"
    break;

  case 164: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 966 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4823 "raku.tab.c"
    break;

  case 165: /* for_stmt: KW_FOR expr block  */
#line 971 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4830 "raku.tab.c"
    break;

  case 166: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 976 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4842 "raku.tab.c"
    break;

  case 167: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 984 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4855 "raku.tab.c"
    break;

  case 168: /* catch_when_list: KW_WHEN expr block  */
#line 995 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,(yyvsp[-1].node)); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 4861 "raku.tab.c"
    break;

  case 169: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 997 "raku.y"
        { exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 4867 "raku.tab.c"
    break;

  case 170: /* when_list: %empty  */
#line 1000 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4873 "raku.tab.c"
    break;

  case 171: /* when_list: when_list KW_WHEN expr block  */
#line 1002 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4881 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1008 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4893 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1016 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4903 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1022 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4913 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1028 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4925 "raku.tab.c"
    break;

  case 176: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1036 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4935 "raku.tab.c"
    break;

  case 177: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1042 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4945 "raku.tab.c"
    break;

  case 178: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1048 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4958 "raku.tab.c"
    break;

  case 179: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1057 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4969 "raku.tab.c"
    break;

  case 180: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1064 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4982 "raku.tab.c"
    break;

  case 181: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1073 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4995 "raku.tab.c"
    break;

  case 182: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1082 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5006 "raku.tab.c"
    break;

  case 183: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1091 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5017 "raku.tab.c"
    break;

  case 184: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1098 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5026 "raku.tab.c"
    break;

  case 185: /* scalar_methcall: VAR_SCALAR '.' IDENT  */
#line 1103 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5035 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list '}'  */
#line 1109 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5041 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr '}'  */
#line 1111 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5048 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1114 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5055 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1117 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5061 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1119 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5067 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1121 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5073 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1123 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5079 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1125 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5085 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1127 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5091 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1129 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5097 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1131 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5104 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1134 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5110 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1136 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5116 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1138 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5122 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1140 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5129 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1143 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5136 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1146 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5144 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list '}'  */
#line 1151 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5150 "raku.tab.c"
    break;

  case 204: /* method_body: '{' YADA '}'  */
#line 1152 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5156 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1154 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-4].list); exprlist_append(l,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5163 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr '}'  */
#line 1157 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5170 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1160 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5177 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1163 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5183 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1165 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5189 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1167 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5195 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1169 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5201 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1171 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5207 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1173 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5213 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1175 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5219 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1177 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5226 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1180 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5232 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1182 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5238 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1184 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5244 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1186 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5251 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1189 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5258 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1192 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5266 "raku.tab.c"
    break;

  case 222: /* pkg_name: IDENT  */
#line 1197 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5272 "raku.tab.c"
    break;

  case 223: /* pkg_name: QIDENT  */
#line 1198 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5278 "raku.tab.c"
    break;

  case 224: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1202 "raku.y"
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
#line 5296 "raku.tab.c"
    break;

  case 225: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1218 "raku.y"
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
#line 5313 "raku.tab.c"
    break;

  case 226: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1233 "raku.y"
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
#line 5330 "raku.tab.c"
    break;

  case 227: /* is_clauses: %empty  */
#line 1247 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5336 "raku.tab.c"
    break;

  case 228: /* is_clauses: is_clauses IDENT IDENT  */
#line 1249 "raku.y"
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
#line 5352 "raku.tab.c"
    break;

  case 229: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1261 "raku.y"
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
#line 5368 "raku.tab.c"
    break;

  case 230: /* class_body_list: %empty  */
#line 1274 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5374 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1276 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5381 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1279 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5388 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1282 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5395 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1285 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5402 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1288 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5409 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1291 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5416 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1294 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5423 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1297 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5430 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1300 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5437 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1303 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5444 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1306 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5451 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1309 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5458 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1312 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5469 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1319 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          free((yyvsp[-5].sval)); free((yyvsp[-4].sval)); free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5479 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1325 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          free((yyvsp[-5].sval)); free((yyvsp[-4].sval)); free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5489 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1331 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5500 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1338 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5511 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1345 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5522 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1352 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5530 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1356 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5538 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1360 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5546 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1364 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5554 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1368 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5565 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1375 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5576 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1382 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5587 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1389 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5598 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1396 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5612 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1406 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5624 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1414 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5636 "raku.tab.c"
    break;

  case 260: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1422 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5649 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1431 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5660 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1438 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5671 "raku.tab.c"
    break;

  case 263: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1445 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5685 "raku.tab.c"
    break;

  case 264: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1455 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5697 "raku.tab.c"
    break;

  case 265: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1463 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5709 "raku.tab.c"
    break;

  case 266: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1473 "raku.y"
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
#line 5726 "raku.tab.c"
    break;

  case 267: /* grammar_body_list: %empty  */
#line 1487 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5732 "raku.tab.c"
    break;

  case 268: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1489 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5741 "raku.tab.c"
    break;

  case 269: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1494 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5750 "raku.tab.c"
    break;

  case 270: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1499 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5759 "raku.tab.c"
    break;

  case 271: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1506 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5767 "raku.tab.c"
    break;

  case 272: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1510 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5775 "raku.tab.c"
    break;

  case 273: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1514 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5783 "raku.tab.c"
    break;

  case 274: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1518 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5791 "raku.tab.c"
    break;

  case 275: /* pair_list: IDENT OP_FATARROW expr  */
#line 1524 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5797 "raku.tab.c"
    break;

  case 276: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1526 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5803 "raku.tab.c"
    break;

  case 277: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1528 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5809 "raku.tab.c"
    break;

  case 278: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1530 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5815 "raku.tab.c"
    break;

  case 279: /* param_list: VAR_SCALAR  */
#line 1533 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5821 "raku.tab.c"
    break;

  case 280: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1534 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5827 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1535 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5833 "raku.tab.c"
    break;

  case 282: /* param_list: VAR_ARRAY  */
#line 1536 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5839 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' VAR_ARRAY  */
#line 1537 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5845 "raku.tab.c"
    break;

  case 284: /* param_list: IDENT VAR_SCALAR  */
#line 1538 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5851 "raku.tab.c"
    break;

  case 285: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1539 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5857 "raku.tab.c"
    break;

  case 286: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1540 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5863 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' VAR_SCALAR  */
#line 1541 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5869 "raku.tab.c"
    break;

  case 288: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1542 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5875 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1543 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5881 "raku.tab.c"
    break;

  case 290: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1544 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5887 "raku.tab.c"
    break;

  case 291: /* param_list: VAR_SCALAR '=' expr  */
#line 1545 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5893 "raku.tab.c"
    break;

  case 292: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1546 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5899 "raku.tab.c"
    break;

  case 293: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1547 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5905 "raku.tab.c"
    break;

  case 294: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1548 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5911 "raku.tab.c"
    break;

  case 295: /* param_list: SLURPY_POS  */
#line 1549 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5917 "raku.tab.c"
    break;

  case 296: /* param_list: param_list ',' SLURPY_POS  */
#line 1550 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5923 "raku.tab.c"
    break;

  case 297: /* param_list: SLURPY_LOL  */
#line 1551 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5929 "raku.tab.c"
    break;

  case 298: /* param_list: param_list ',' SLURPY_LOL  */
#line 1552 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5935 "raku.tab.c"
    break;

  case 299: /* param_list: SLURPY_NAMED  */
#line 1553 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5941 "raku.tab.c"
    break;

  case 300: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1554 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5947 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list '}'  */
#line 1557 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5953 "raku.tab.c"
    break;

  case 302: /* block: '{' YADA '}'  */
#line 1558 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5959 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list expr '}'  */
#line 1560 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5965 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1562 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5971 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1564 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5977 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1566 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5983 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1568 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5989 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1570 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5996 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1573 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6002 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1575 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6008 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1577 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6014 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1579 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 6020 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1581 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6027 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1584 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6034 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1587 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6042 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1591 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6049 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1594 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6056 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1597 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6063 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1600 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 6069 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1602 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6077 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1606 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6084 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1609 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6091 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1612 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6098 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1615 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6105 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1618 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6112 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1621 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 6118 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1623 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6124 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1625 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6130 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_LAST '}'  */
#line 1627 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6136 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1629 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6142 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1631 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6148 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1633 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6154 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1635 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6160 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1637 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6166 "raku.tab.c"
    break;

  case 335: /* closure: '{' expr '}'  */
#line 1640 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6172 "raku.tab.c"
    break;

  case 336: /* expr: VAR_SCALAR '=' expr  */
#line 1643 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6178 "raku.tab.c"
    break;

  case 337: /* expr: VAR_ARRAY '=' expr  */
#line 1644 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6184 "raku.tab.c"
    break;

  case 338: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1645 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6190 "raku.tab.c"
    break;

  case 339: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1646 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6196 "raku.tab.c"
    break;

  case 340: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1647 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6202 "raku.tab.c"
    break;

  case 341: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1648 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6208 "raku.tab.c"
    break;

  case 342: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1649 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6214 "raku.tab.c"
    break;

  case 343: /* expr: KW_GATHER block  */
#line 1650 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6224 "raku.tab.c"
    break;

  case 344: /* expr: KW_GATHER for_stmt  */
#line 1655 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6234 "raku.tab.c"
    break;

  case 345: /* expr: tern_expr OP_FATARROW expr  */
#line 1661 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6240 "raku.tab.c"
    break;

  case 346: /* expr: tern_expr  */
#line 1662 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6246 "raku.tab.c"
    break;

  case 347: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1666 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6252 "raku.tab.c"
    break;

  case 348: /* tern_expr: or_expr  */
#line 1667 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6258 "raku.tab.c"
    break;

  case 349: /* or_expr: or_expr OP_OR and_expr  */
#line 1670 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6264 "raku.tab.c"
    break;

  case 350: /* or_expr: and_expr  */
#line 1671 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6270 "raku.tab.c"
    break;

  case 351: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1674 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6276 "raku.tab.c"
    break;

  case 352: /* and_expr: cmp_expr  */
#line 1675 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6282 "raku.tab.c"
    break;

  case 353: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1678 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 6288 "raku.tab.c"
    break;

  case 354: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1679 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 6294 "raku.tab.c"
    break;

  case 355: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1680 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 6300 "raku.tab.c"
    break;

  case 356: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1681 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 6306 "raku.tab.c"
    break;

  case 357: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1682 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 6312 "raku.tab.c"
    break;

  case 358: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1683 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 6318 "raku.tab.c"
    break;

  case 359: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1684 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6324 "raku.tab.c"
    break;

  case 360: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1685 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6330 "raku.tab.c"
    break;

  case 361: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1686 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6336 "raku.tab.c"
    break;

  case 362: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1687 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6342 "raku.tab.c"
    break;

  case 363: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1688 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6348 "raku.tab.c"
    break;

  case 364: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1689 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6354 "raku.tab.c"
    break;

  case 365: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1691 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 6364 "raku.tab.c"
    break;

  case 366: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1697 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 6374 "raku.tab.c"
    break;

  case 367: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1703 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 6384 "raku.tab.c"
    break;

  case 368: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1709 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 6394 "raku.tab.c"
    break;

  case 369: /* cmp_expr: divis_expr  */
#line 1714 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 6400 "raku.tab.c"
    break;

  case 370: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1717 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6406 "raku.tab.c"
    break;

  case 371: /* divis_expr: jct_expr  */
#line 1718 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6412 "raku.tab.c"
    break;

  case 372: /* jct_expr: jct_expr '|' range_expr  */
#line 1721 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6418 "raku.tab.c"
    break;

  case 373: /* jct_expr: jct_expr '&' range_expr  */
#line 1722 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6424 "raku.tab.c"
    break;

  case 374: /* jct_expr: dor_expr  */
#line 1723 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6430 "raku.tab.c"
    break;

  case 375: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1727 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6436 "raku.tab.c"
    break;

  case 376: /* dor_expr: range_expr  */
#line 1728 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6442 "raku.tab.c"
    break;

  case 377: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1731 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6448 "raku.tab.c"
    break;

  case 378: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1732 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6454 "raku.tab.c"
    break;

  case 379: /* range_expr: add_expr  */
#line 1733 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6460 "raku.tab.c"
    break;

  case 380: /* add_expr: add_expr '~' repl_expr  */
#line 1736 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6466 "raku.tab.c"
    break;

  case 381: /* add_expr: repl_expr  */
#line 1737 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6472 "raku.tab.c"
    break;

  case 382: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1740 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6478 "raku.tab.c"
    break;

  case 383: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1741 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6484 "raku.tab.c"
    break;

  case 384: /* repl_expr: addsub_expr  */
#line 1742 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6490 "raku.tab.c"
    break;

  case 385: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1745 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6496 "raku.tab.c"
    break;

  case 386: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1746 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6502 "raku.tab.c"
    break;

  case 387: /* addsub_expr: mul_expr  */
#line 1747 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6508 "raku.tab.c"
    break;

  case 388: /* mul_expr: mul_expr '*' unary_expr  */
#line 1750 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6514 "raku.tab.c"
    break;

  case 389: /* mul_expr: mul_expr '/' unary_expr  */
#line 1751 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6520 "raku.tab.c"
    break;

  case 390: /* mul_expr: mul_expr '%' unary_expr  */
#line 1752 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6526 "raku.tab.c"
    break;

  case 391: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1754 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6532 "raku.tab.c"
    break;

  case 392: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1756 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6538 "raku.tab.c"
    break;

  case 393: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1758 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6544 "raku.tab.c"
    break;

  case 394: /* mul_expr: unary_expr  */
#line 1759 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6550 "raku.tab.c"
    break;

  case 395: /* unary_expr: '-' unary_expr  */
#line 1762 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6556 "raku.tab.c"
    break;

  case 396: /* unary_expr: '+' unary_expr  */
#line 1763 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6562 "raku.tab.c"
    break;

  case 397: /* unary_expr: '!' unary_expr  */
#line 1764 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6568 "raku.tab.c"
    break;

  case 398: /* unary_expr: CARET unary_expr  */
#line 1765 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6574 "raku.tab.c"
    break;

  case 399: /* unary_expr: OP_REDUCE unary_expr  */
#line 1767 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6583 "raku.tab.c"
    break;

  case 400: /* unary_expr: pow_expr  */
#line 1771 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6589 "raku.tab.c"
    break;

  case 401: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1774 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6595 "raku.tab.c"
    break;

  case 402: /* pow_expr: postfix_expr  */
#line 1775 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6601 "raku.tab.c"
    break;

  case 403: /* scalar_list: VAR_SCALAR  */
#line 1778 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6607 "raku.tab.c"
    break;

  case 404: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1779 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6613 "raku.tab.c"
    break;

  case 405: /* meth_name: IDENT  */
#line 1782 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6619 "raku.tab.c"
    break;

  case 406: /* meth_name: KW_SORT  */
#line 1783 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6625 "raku.tab.c"
    break;

  case 407: /* meth_name: KW_REVERSE  */
#line 1784 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6631 "raku.tab.c"
    break;

  case 408: /* meth_name: KW_MAP  */
#line 1785 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6637 "raku.tab.c"
    break;

  case 409: /* meth_name: KW_GREP  */
#line 1786 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6643 "raku.tab.c"
    break;

  case 410: /* meth_name: KW_SAY  */
#line 1787 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6649 "raku.tab.c"
    break;

  case 411: /* meth_name: KW_PRINT  */
#line 1788 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6655 "raku.tab.c"
    break;

  case 412: /* meth_name: KW_TAKE  */
#line 1789 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6661 "raku.tab.c"
    break;

  case 413: /* meth_name: KW_RETURN  */
#line 1790 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6667 "raku.tab.c"
    break;

  case 414: /* meth_name: KW_EXISTS  */
#line 1791 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6673 "raku.tab.c"
    break;

  case 415: /* meth_name: KW_DELETE  */
#line 1792 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6679 "raku.tab.c"
    break;

  case 416: /* meth_name: KW_JOIN  */
#line 1793 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6685 "raku.tab.c"
    break;

  case 417: /* meth_name: TESTOP  */
#line 1794 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6691 "raku.tab.c"
    break;

  case 418: /* postfix_expr: call_expr  */
#line 1796 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6697 "raku.tab.c"
    break;

  case 419: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1799 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6707 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '(' arg_list ')'  */
#line 1805 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6716 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '(' ')'  */
#line 1809 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6722 "raku.tab.c"
    break;

  case 422: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1811 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6728 "raku.tab.c"
    break;

  case 423: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1813 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6734 "raku.tab.c"
    break;

  case 424: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1815 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6743 "raku.tab.c"
    break;

  case 425: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1820 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6749 "raku.tab.c"
    break;

  case 426: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1822 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6759 "raku.tab.c"
    break;

  case 427: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1828 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6769 "raku.tab.c"
    break;

  case 428: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1834 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6777 "raku.tab.c"
    break;

  case 429: /* call_expr: IDENT '.' KW_NEW  */
#line 1838 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6785 "raku.tab.c"
    break;

  case 430: /* call_expr: IDENT '.' IDENT  */
#line 1842 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6794 "raku.tab.c"
    break;

  case 431: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1847 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6805 "raku.tab.c"
    break;

  case 432: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1854 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6814 "raku.tab.c"
    break;

  case 433: /* call_expr: IDENT '.' CARET IDENT  */
#line 1859 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6824 "raku.tab.c"
    break;

  case 434: /* call_expr: atom '.' CARET IDENT  */
#line 1865 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6834 "raku.tab.c"
    break;

  case 435: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1871 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6845 "raku.tab.c"
    break;

  case 436: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1878 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6856 "raku.tab.c"
    break;

  case 437: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1885 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6865 "raku.tab.c"
    break;

  case 438: /* call_expr: atom '.' meth_name  */
#line 1890 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6874 "raku.tab.c"
    break;

  case 439: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1895 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6885 "raku.tab.c"
    break;

  case 440: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1902 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6894 "raku.tab.c"
    break;

  case 441: /* call_expr: call_expr '.' meth_name  */
#line 1907 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6903 "raku.tab.c"
    break;

  case 442: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 1912 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6914 "raku.tab.c"
    break;

  case 443: /* call_expr: '.' meth_name '(' ')'  */
#line 1919 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6923 "raku.tab.c"
    break;

  case 444: /* call_expr: '.' meth_name  */
#line 1924 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6932 "raku.tab.c"
    break;

  case 445: /* call_expr: KW_DIE expr  */
#line 1929 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6938 "raku.tab.c"
    break;

  case 446: /* call_expr: KW_MAP closure expr  */
#line 1931 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6944 "raku.tab.c"
    break;

  case 447: /* call_expr: KW_MAP closure ',' expr  */
#line 1933 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6950 "raku.tab.c"
    break;

  case 448: /* call_expr: KW_GREP closure expr  */
#line 1935 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6956 "raku.tab.c"
    break;

  case 449: /* call_expr: KW_SORT expr  */
#line 1937 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6962 "raku.tab.c"
    break;

  case 450: /* call_expr: KW_SORT closure expr  */
#line 1939 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6968 "raku.tab.c"
    break;

  case 451: /* call_expr: KW_REVERSE expr  */
#line 1941 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6974 "raku.tab.c"
    break;

  case 452: /* call_expr: atom  */
#line 1942 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6980 "raku.tab.c"
    break;

  case 453: /* arg_list: expr  */
#line 1945 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6986 "raku.tab.c"
    break;

  case 454: /* arg_list: arg_list ',' expr  */
#line 1946 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6992 "raku.tab.c"
    break;

  case 455: /* arg_list: arg_list ','  */
#line 1947 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6998 "raku.tab.c"
    break;

  case 456: /* atom: LIT_INT  */
#line 1950 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7004 "raku.tab.c"
    break;

  case 457: /* atom: LIT_BOOL  */
#line 1952 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7012 "raku.tab.c"
    break;

  case 458: /* atom: LIT_FLOAT  */
#line 1955 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7018 "raku.tab.c"
    break;

  case 459: /* atom: LIT_STR  */
#line 1956 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7024 "raku.tab.c"
    break;

  case 460: /* atom: WORDLIST  */
#line 1958 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7037 "raku.tab.c"
    break;

  case 461: /* atom: LIT_INTERP_STR  */
#line 1966 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7043 "raku.tab.c"
    break;

  case 462: /* atom: VAR_SCALAR  */
#line 1967 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7049 "raku.tab.c"
    break;

  case 463: /* atom: OP_INC VAR_SCALAR  */
#line 1968 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7055 "raku.tab.c"
    break;

  case 464: /* atom: OP_DEC VAR_SCALAR  */
#line 1969 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7061 "raku.tab.c"
    break;

  case 465: /* atom: VAR_SCALAR OP_INC  */
#line 1970 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7067 "raku.tab.c"
    break;

  case 466: /* atom: VAR_SCALAR OP_DEC  */
#line 1971 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7073 "raku.tab.c"
    break;

  case 467: /* atom: VAR_TWIGIL OP_INC  */
#line 1972 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); free((yyvsp[-1].sval)); }
#line 7079 "raku.tab.c"
    break;

  case 468: /* atom: VAR_TWIGIL OP_DEC  */
#line 1973 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); free((yyvsp[-1].sval)); }
#line 7085 "raku.tab.c"
    break;

  case 469: /* atom: VAR_ARRAY  */
#line 1974 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7091 "raku.tab.c"
    break;

  case 470: /* atom: VAR_HASH  */
#line 1975 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7097 "raku.tab.c"
    break;

  case 471: /* atom: VAR_CAPTURE  */
#line 1977 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7105 "raku.tab.c"
    break;

  case 472: /* atom: VAR_FH  */
#line 1981 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7113 "raku.tab.c"
    break;

  case 473: /* atom: VAR_NAMED_CAPTURE  */
#line 1985 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 7120 "raku.tab.c"
    break;

  case 474: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1988 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 7126 "raku.tab.c"
    break;

  case 475: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1990 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 7132 "raku.tab.c"
    break;

  case 476: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1992 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 7138 "raku.tab.c"
    break;

  case 477: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1994 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 7144 "raku.tab.c"
    break;

  case 478: /* atom: ARR_ALL_SLICE  */
#line 1996 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 7150 "raku.tab.c"
    break;

  case 479: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1998 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7156 "raku.tab.c"
    break;

  case 480: /* atom: VAR_HASH '{' expr '}'  */
#line 2000 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7162 "raku.tab.c"
    break;

  case 481: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2002 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7168 "raku.tab.c"
    break;

  case 482: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2004 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7174 "raku.tab.c"
    break;

  case 483: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2006 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7180 "raku.tab.c"
    break;

  case 484: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2008 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7186 "raku.tab.c"
    break;

  case 485: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2010 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7192 "raku.tab.c"
    break;

  case 486: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2012 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7198 "raku.tab.c"
    break;

  case 487: /* atom: IDENT  */
#line 2013 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7204 "raku.tab.c"
    break;

  case 488: /* atom: VAR_TWIGIL  */
#line 2015 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7212 "raku.tab.c"
    break;

  case 489: /* atom: VAR_ARRAY_TWIGIL  */
#line 2019 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7220 "raku.tab.c"
    break;

  case 490: /* atom: VAR_HASH_TWIGIL  */
#line 2023 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7228 "raku.tab.c"
    break;

  case 491: /* atom: '[' ']'  */
#line 2026 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 7234 "raku.tab.c"
    break;

  case 492: /* atom: '[' expr ']'  */
#line 2028 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7240 "raku.tab.c"
    break;

  case 493: /* atom: '[' expr ',' ']'  */
#line 2030 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7246 "raku.tab.c"
    break;

  case 494: /* atom: '[' expr ',' arg_list ']'  */
#line 2032 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7253 "raku.tab.c"
    break;

  case 495: /* atom: DOLLAR_LBRACKET ']'  */
#line 2034 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 7259 "raku.tab.c"
    break;

  case 496: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2036 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7265 "raku.tab.c"
    break;

  case 497: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2038 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7271 "raku.tab.c"
    break;

  case 498: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2040 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7278 "raku.tab.c"
    break;

  case 499: /* atom: '(' ')'  */
#line 2042 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7284 "raku.tab.c"
    break;

  case 500: /* atom: '(' expr ')'  */
#line 2043 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7290 "raku.tab.c"
    break;

  case 501: /* atom: '(' expr ',' ')'  */
#line 2045 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7296 "raku.tab.c"
    break;

  case 502: /* atom: '(' expr ',' arg_list ')'  */
#line 2047 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7303 "raku.tab.c"
    break;

  case 503: /* atom: block  */
#line 2049 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7309 "raku.tab.c"
    break;

  case 504: /* atom: KW_SUB block  */
#line 2050 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7315 "raku.tab.c"
    break;


#line 7319 "raku.tab.c"

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

#line 2052 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
