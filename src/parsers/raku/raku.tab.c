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
  YYSYMBOL_KW_FAIL = 61,                   /* KW_FAIL  */
  YYSYMBOL_KW_CLASS = 62,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 63,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 64,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 65,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 66,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 67,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 68,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 69,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 70,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 71,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 72,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 73,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 74,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 75,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 76,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 77,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 78,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 79,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 80,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 81,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 82,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 83,                  /* KW_REGEX  */
  YYSYMBOL_KW_MODULE = 84,                 /* KW_MODULE  */
  YYSYMBOL_OP_FATARROW = 85,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 86,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 87,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 88,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 89,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 90,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 91,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 92,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 93,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 94,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 95,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 96,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 97,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 98,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 99,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 100,                    /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 101,              /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 102,              /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 103,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 104,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 105,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 106,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 107,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 108,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 109,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 110,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 111,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 112,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 113,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 114,                   /* OP_DIV  */
  YYSYMBOL_ADV_EXISTS = 115,               /* ADV_EXISTS  */
  YYSYMBOL_ADV_DELETE = 116,               /* ADV_DELETE  */
  YYSYMBOL_OP_BAND = 117,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 118,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 119,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 120,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 121,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 122,                   /* OP_POW  */
  YYSYMBOL_123_ = 123,                     /* '='  */
  YYSYMBOL_124_ = 124,                     /* '!'  */
  YYSYMBOL_125_ = 125,                     /* '<'  */
  YYSYMBOL_126_ = 126,                     /* '>'  */
  YYSYMBOL_127_ = 127,                     /* '|'  */
  YYSYMBOL_128_ = 128,                     /* '&'  */
  YYSYMBOL_129_ = 129,                     /* '~'  */
  YYSYMBOL_130_ = 130,                     /* '+'  */
  YYSYMBOL_131_ = 131,                     /* '-'  */
  YYSYMBOL_132_ = 132,                     /* '*'  */
  YYSYMBOL_133_ = 133,                     /* '/'  */
  YYSYMBOL_134_ = 134,                     /* '%'  */
  YYSYMBOL_UMINUS = 135,                   /* UMINUS  */
  YYSYMBOL_136_ = 136,                     /* '.'  */
  YYSYMBOL_137_ = 137,                     /* ';'  */
  YYSYMBOL_138_ = 138,                     /* '('  */
  YYSYMBOL_139_ = 139,                     /* ')'  */
  YYSYMBOL_140_ = 140,                     /* ','  */
  YYSYMBOL_141_ = 141,                     /* '['  */
  YYSYMBOL_142_ = 142,                     /* ']'  */
  YYSYMBOL_143_ = 143,                     /* '{'  */
  YYSYMBOL_144_ = 144,                     /* '}'  */
  YYSYMBOL_145_ = 145,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 146,                 /* $accept  */
  YYSYMBOL_program = 147,                  /* program  */
  YYSYMBOL_stmt_list = 148,                /* stmt_list  */
  YYSYMBOL_stmt = 149,                     /* stmt  */
  YYSYMBOL_if_stmt = 150,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 151,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 152,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 153,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 154,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 155,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 156,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 157,                /* loop_incr  */
  YYSYMBOL_for_stmt = 158,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 159,               /* given_stmt  */
  YYSYMBOL_catch_when_list = 160,          /* catch_when_list  */
  YYSYMBOL_when_list = 161,                /* when_list  */
  YYSYMBOL_sub_decl = 162,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 163,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 164,                 /* sub_body  */
  YYSYMBOL_method_body = 165,              /* method_body  */
  YYSYMBOL_pkg_name = 166,                 /* pkg_name  */
  YYSYMBOL_class_decl = 167,               /* class_decl  */
  YYSYMBOL_role_decl = 168,                /* role_decl  */
  YYSYMBOL_module_decl = 169,              /* module_decl  */
  YYSYMBOL_is_clauses = 170,               /* is_clauses  */
  YYSYMBOL_class_body_list = 171,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 172,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 173,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 174,           /* named_arg_list  */
  YYSYMBOL_pair_list = 175,                /* pair_list  */
  YYSYMBOL_param_list = 176,               /* param_list  */
  YYSYMBOL_block = 177,                    /* block  */
  YYSYMBOL_closure = 178,                  /* closure  */
  YYSYMBOL_expr = 179,                     /* expr  */
  YYSYMBOL_tern_expr = 180,                /* tern_expr  */
  YYSYMBOL_or_expr = 181,                  /* or_expr  */
  YYSYMBOL_and_expr = 182,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 183,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 184,               /* divis_expr  */
  YYSYMBOL_jct_expr = 185,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 186,                 /* dor_expr  */
  YYSYMBOL_range_expr = 187,               /* range_expr  */
  YYSYMBOL_add_expr = 188,                 /* add_expr  */
  YYSYMBOL_repl_expr = 189,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 190,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 191,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 192,               /* unary_expr  */
  YYSYMBOL_pow_expr = 193,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 194,              /* scalar_list  */
  YYSYMBOL_meth_name = 195,                /* meth_name  */
  YYSYMBOL_postfix_expr = 196,             /* postfix_expr  */
  YYSYMBOL_call_expr = 197,                /* call_expr  */
  YYSYMBOL_arg_list = 198,                 /* arg_list  */
  YYSYMBOL_atom = 199                      /* atom  */
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
#define YYLAST   7230

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  146
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  503
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1247

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   378


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
       2,     2,     2,   124,     2,     2,     2,   134,   128,     2,
     138,   139,   132,   130,   140,   131,   136,   133,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   145,   137,
     125,   123,   126,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   141,     2,   142,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   143,   127,   144,   129,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   121,   122,   135
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
    1938,  1940,  1942,  1945,  1946,  1947,  1950,  1951,  1952,  1953,
    1962,  1963,  1964,  1965,  1966,  1967,  1968,  1969,  1970,  1971,
    1972,  1976,  1980,  1983,  1985,  1987,  1989,  1991,  1993,  1995,
    1997,  1999,  2001,  2003,  2005,  2007,  2009,  2010,  2014,  2018,
    2022,  2023,  2025,  2027,  2030,  2031,  2033,  2035,  2038,  2039,
    2040,  2042,  2045,  2046
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

#define YYPACT_NINF (-907)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-503)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -907,   136,  2531,  -907,  -907,  -907,  -907,  -907,   954,    62,
     188,   246,    61,  -907,  -907,  7037,  2630,  -907,  -907,  -907,
     613,  2729,   163,  5750,  5849,  5948,  6047,  6146,    29,   -15,
    6146,  2828,  2927,    87,    48,  6146,  6146,    14,   141,  6245,
    6344,    16,   205,    97,   181,    51,    51,  6443,  6146,    16,
     117,  6146,  3026,   635,   635,    57,  7037,  -907,  -907,   635,
     635,   201,   278,  7037,  7037,  7037,   865,  -907,  3125,  3266,
     187,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,
    -907,   769,  -907,  -907,  -907,  -907,  7096,   832,   192,   673,
     209,   409,   671,   559,   228,  -907,   -11,   811,   787,   524,
    -907,  -907,   297,   296,   311,   363,  -907,  -907,  6146,  6146,
    6146,  6146,  6146,  6146,   414,  3365,  6542,  3506,   478,  6146,
    -907,  -907,  6146,   263,    47,   451,   164,   375,   204,   878,
     404,    16,  3647,  -907,  -907,   497,   210,  -907,   -13,  3788,
    3887,  -907,  3986,  -907,   353,    90,   152,   173,   892,   515,
     435,   534,  3647,   709,  3647,   264,  3647,    16,  3647,    16,
      76,    -5,   338,  -907,  -907,  -907,   380,  -907,   269,  -907,
     340,   431,   457,   498,   448,   456,   255,   267,  3647,    16,
    3647,    16,   374,  4127,  -907,  6146,  6146,  -907,  6146,  6146,
    -907,  6146,  4226,  6146,  4325,  6146,  -907,  -907,   578,   330,
    -907,  -907,  -907,   523,  -907,  -907,  -907,   572,   567,    44,
    -907,   583,   641,  -907,  -907,  -907,  -907,  -907,  -907,  -907,
    -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,
    -907,   590,   439,  -907,   858,   862,  -907,   424,   598,  1666,
    6146,  6146,  6146,  6146,  6146,  6146,  6146,  6146,  -907,  6146,
    6146,  6146,  6146,  6146,  6146,  6146,  6146,  -907,  6146,  7037,
    7037,  7037,  7037,  7037,  7037,  7037,  7037,  7037,  7037,  7037,
    7037,  7037,  7037,  7037,   637,  7037,  7037,  7037,  7037,  7037,
    7037,  7037,  7037,  7037,  7037,  7037,  7037,  7037,  7037,  7037,
    7037,  7037,  7037,   865,   998,   866,   670,   674,   678,   692,
     696,   716,   -70,  -907,   867,  3647,   562,   880,   455,   746,
     741,   761,  -907,   798,   934,   816,   -38,  -907,   948,   887,
     889,  3506,   962,  6146,  6146,  6146,  6146,  6146,  6146,  6146,
    6146,  4424,  -907,  -907,   831,  -907,   856,   868,   858,   897,
    -907,  6146,  6146,  -907,  6641,  -907,  6740,  -907,   241,   244,
     256,   417,   876,   885,  -907,   900,   908,  6146,  6146,  6146,
    6146,  6146,  6146,  6146,  -907,  6146,   910,  6146,  6146,  -907,
    6146,   917,  1041,   933,  -907,   534,  6146,  -907,  7037,  7037,
      21,  -907,  -907,  -907,  6146,  6146,  6146,  -907,  6146,  6146,
    -907,  6146,  6146,   884,  6146,  -907,   999,  6146,  1001,  6146,
     946,   994,   957,  -907,  6146,  6146,  1007,   893,   895,   904,
     906,   930,   931,  6146,  -907,  -907,  -907,    16,  6146,    16,
      35,  -907,    28,  -907,    30,   949,   951,  -907,  -907,  4523,
    -907,  4664,   968,  1099,  4805,  -907,  -907,   972,   292,   295,
     408,  5750,  5849,  6146,  2191,    68,   154,  -907,   386,   974,
     983,   985,   987,  1009,  1015,  1023,  1025,  1027,  1043,  1084,
    1090,  1114,  1118,  1119,  1120,  -907,   209,  1004,   409,  1051,
    1051,  1051,  1051,  1051,  1051,  1051,  1051,  1051,  1051,  1051,
    1051,  -907,  -907,  -907,  -907,   559,  -907,  -907,  -907,  1037,
    1037,   811,   787,   787,   524,   524,  -907,  -907,  -907,  -907,
    -907,  -907,  -907,  1060,  1188,  1121,  -907,  4904,  -907,  -907,
    -907,  -907,  -907,  -907,  6146,  5045,  -907,   959,  -907,  6146,
    6146,  6146,  6146,  1137,   337,   379,  -907,  5186,  -907,  1212,
    6146,  1123,  -907,    -1,  -907,  1523,   470,  1136,  1126,  -907,
    -907,  -907,  -907,  -907,  -907,  -907,  -907,   535,  -907,  -907,
    -907,  1127,  -907,  1128,  3647,   587,  1178,    82,  5327,   621,
    1134,  6146,  -907,  6839,  -907,  6146,  -907,    49,  -907,  6146,
    6146,  1152,  4664,  1139,  1140,  1141,  1142,  1143,  1144,  1148,
     646,  4664,  1149,  1150,   668,    16,    -9,  6938,  -907,    16,
     496,   122,   -37,   -24,     3,  -907,   196,  -907,  -907,  -907,
    1146,   961,  1808,   776,  1153,  1154,  1155,  1156,  1157,  1158,
    -907,  1159,   115,  1170,  1160,  1171,  1161,    16,    16,    16,
    1163,  1164,  1175,  6146,  -907,  -907,  -907,  -907,  -907,  -907,
    -907,    16,  1162,  6146,    16,  -907,  1270,  1288,  -907,    24,
    1146,   963,    59,   464,   293,  1950,  -907,   965,  -907,   973,
    6146,  -907,  -907,   620,  1289,  3506,  6146,  6146,   570,    26,
     342,  -907,   167,  6146,  6146,  -907,  6146,  6146,  -907,  6146,
    6146,  6146,  6146,  6146,  6146,  6146,  6146,  -907,  -907,  -907,
    -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,
    -907,  -907,  -907,  -907,  7037,  5468,  -907,  1380,  1172,   975,
    1173,  -907,   977,  4664,   805,  1165,  1166,   702,  6146,  -907,
    -907,  6146,  -907,  -907,  6146,  -907,   979,  -907,   988,   990,
    -907,  6146,  1227,  1299,  1005,  -907,   978,  1033,  -907,  -907,
    -907,  1011,  -907,  6146,  6146,  6146,  1017,  -907,   331,  -907,
    1177,  3647,   827,  1180,  1146,  1019,  1183,  1185,  6146,  1029,
    -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  1035,  -907,
    -907,  -907,  1147,  -907,  -907,  3647,    16,  -907,  -907,  1305,
    -907,  1306,  1313,  1310,  6146,  1202,  1316,  1317,  -907,  1146,
     499,  5750,  5849,  2290,  -907,   585,  -907,  -907,  -907,  -907,
    -907,  -907,  -907,  6146,    16,  -907,  -907,  -907,  1191,  1192,
    1301,  -907,  -907,  -907,  -907,  6146,  1194,  -907,  -907,    16,
    1189,  -907,  -907,   194,   925,   681,  1269,  -907,  -907,  1146,
    1146,  1039,  1042,  1320,  1321,  1323,  -907,  -907,  -907,  -907,
    1203,  -907,   -39,   852,  1195,   314,  6146,  6146,  6146,  6146,
    6146,  6146,  -907,  -907,  -907,  -907,   359,   395,   432,   449,
     488,   503,   522,   541,   544,   546,   564,   581,  -907,  -907,
    1044,  -907,  1047,  1049,  -907,  1204,  -907,  -907,  1061,  -907,
    -907,  -907,  -907,  1207,  1209,  1210,  -907,  -907,  -907,  1199,
    6146,  1211,  -907,  4664,   833,  -907,  -907,  1215,  1260,  1271,
    -907,  1065,  -907,  6146,  -907,  -907,  1146,  -907,  -907,   837,
    1217,  1221,    -9,  -907,  1067,  1179,    16,    16,    16,  -907,
    -907,  6146,  -907,  -907,  -907,    12,  -907,   222,  -907,  -907,
    -907,   648,    91,  -907,   306,  6146,  6146,  6146,  6146,  6146,
    6146,  6146,  6146,  -907,    16,  1216,  -907,  -907,    16,  1222,
    6146,  -907,  -907,  -907,   728,   731,   193,   212,   739,  1224,
    1225,   865,  -907,  -907,  1146,  1146,  1356,  1357,  1358,  -907,
    6146,  5609,  1243,   468,  -907,   586,   599,   601,   633,   634,
     638,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  -907,
    -907,  -907,  -907,  -907,  -907,  -907,  -907,  1230,  -907,  -907,
    -907,  -907,  -907,  6146,  1071,  -907,  -907,  6146,  6146,  4664,
     838,  -907,  -907,  6146,  -907,  -907,  -907,  -907,    16,    16,
    -907,  -907,  -907,  -907,  -907,  1354,  6146,  1246,  1361,  1362,
    6146,  6146,  6146,  -907,  -907,  -907,   650,   652,   658,   662,
     682,   683,   684,   686,  -907,  -907,  -907,  6146,  1234,  -907,
      69,  1296,  -907,    75,  -907,  1363,  1364,  1239,  6146,  -907,
    1365,  1366,  1244,  6146,  -907,   262,   271,  1245,  1250,  -907,
    -907,   732,  -907,  -907,  -907,  -907,  -907,   687,  1251,  1080,
    6146,  6146,  -907,  -907,  -907,  -907,  -907,  -907,  -907,  1249,
    1266,  -907,  -907,  1096,  -907,   846,  1208,  -907,  -907,  -907,
    6146,  -907,  -907,   688,   704,   708,  -907,  -907,  -907,  -907,
    -907,  -907,  -907,  -907,  1267,    16,  1262,  1100,  1263,  2092,
    1262,  1102,  1272,  1273,  -907,  1274,   284,   289,  -907,  1275,
    1392,  1401,  1279,  6146,  -907,  1403,  1404,  1282,  6146,  -907,
    -907,  -907,   151,  -907,  -907,  -907,  1277,   721,   724,  -907,
    -907,  1285,  -907,    16,  -907,  -907,  -907,  -907,  -907,  6146,
    -907,  -907,  1262,  -907,   418,  5750,  5849,  2389,  -907,   672,
    -907,  1262,  -907,  -907,  -907,  6146,  -907,  6146,  -907,  -907,
    1286,  1287,  -907,  1291,  1292,  1293,  -907,  1295,  1262,  1104,
    -907,  -907,  -907,  -907,  -907,  1294,  -907,  6146,   703,   343,
    -907,   322,  6146,  6146,  6146,  6146,  6146,  6146,  6146,  6146,
    -907,  -907,  1302,  1304,  -907,  -907,  -907,  -907,  -907,  -907,
    -907,  1262,    16,   749,  6146,  6146,  6146,  -907,  -907,  -907,
     750,   751,   753,   770,   771,   796,   797,   800,  -907,  -907,
    -907,  -907,  -907,   809,   819,   822,  -907,  -907,  -907,  -907,
    -907,  -907,  -907,  -907,  -907,  -907,  -907
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   456,   457,   458,   460,   461,   468,
     469,   487,   486,   488,   489,     0,     0,   470,   471,   472,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   477,   459,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       3,     4,   105,   106,   116,   117,   118,   119,   107,   108,
     126,     0,   127,   128,   130,   129,   115,     0,   346,   348,
     350,   352,   369,   371,   374,   376,   379,   381,   384,   387,
     394,   400,   402,   418,   452,     0,   464,   465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     466,   467,     0,     0,     0,     0,   461,   468,   469,   487,
     486,     0,     0,   502,   398,   461,   468,   494,     0,     0,
       0,    38,     0,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,     0,   503,   344,   343,     0,    52,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   153,     0,   156,     0,     0,   120,     0,     0,
     121,     0,     0,     0,     3,     0,   449,   451,   109,     3,
     114,   445,    54,     0,   222,   223,   227,     0,     0,     0,
     399,     0,     0,   462,   463,   397,   396,   395,   405,   417,
     410,   411,   412,   413,   416,   414,   415,   408,   409,   406,
     407,   444,   461,   498,     0,     0,   490,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,   340,   341,
     342,   336,   185,   425,     0,     0,   337,     0,     0,     0,
       0,     0,    42,   430,     0,   429,   486,   421,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,    29,     0,    31,     0,   498,     0,     0,
      41,   455,     0,     6,     0,    10,     0,    11,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,    46,
       0,     0,   134,     0,   146,     0,     0,   165,     0,     0,
       0,     3,   174,    49,     0,     0,     0,    51,     0,     0,
      57,     0,     0,     0,     0,   170,     0,     0,     0,     0,
       0,   149,     0,   152,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   446,   448,   450,     0,     0,     0,
       0,    53,     0,   230,     0,     0,     0,   267,     3,     0,
     499,     0,     0,     0,     0,   491,   302,   461,   468,   469,
     487,     0,     0,     0,     0,     0,     0,   301,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,   349,     0,   351,   353,
     354,   357,   358,   355,   356,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   370,   372,   373,   375,   377,
     378,   380,   382,   383,   385,   386,   391,   392,   393,   388,
     389,   390,   401,   441,     0,   438,    64,     0,    98,    99,
     100,   101,   102,    61,     0,     0,   424,     0,    15,     0,
       0,     0,     0,   473,   478,   479,    66,     0,   433,     0,
       0,     0,   422,     0,   420,   455,     0,     0,     0,   338,
     339,   340,   341,   342,   336,   337,   496,     0,    30,    32,
      40,     0,   454,     0,     0,     0,   458,   486,     0,     0,
       0,     0,    26,     0,    27,     0,    28,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   499,     0,     0,   137,   499,
       0,     0,   377,   378,   279,   282,     0,   295,   297,   299,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,   419,     0,     0,     0,     0,     0,   499,     0,   499,
       0,     0,     0,     0,   122,   123,   124,   125,   335,   447,
     110,     0,     0,     0,     0,   111,     0,     0,   230,     0,
       0,     0,     0,     0,     0,     0,   443,     0,   500,     0,
       0,   404,   492,     0,     0,     0,     0,     0,     0,     0,
       0,   328,     0,     0,     0,   329,     0,     0,   330,     0,
       0,     0,     0,     0,     0,     0,     0,   303,    68,    70,
      72,    75,    73,    74,    69,    71,    81,    83,    85,    88,
      86,    87,    82,    84,     0,     0,   434,     0,     0,     0,
       0,   184,     0,     0,     0,     0,     0,     0,     0,   483,
     485,     0,   482,   484,     0,   432,     0,   428,     0,     0,
     271,     0,     0,     0,     0,   473,   478,   479,   497,    39,
       5,     0,    12,     0,     0,     0,     0,    19,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    92,    91,    95,    93,    94,    90,    44,     0,    96,
      97,    47,   131,   136,   135,     0,     0,   145,   162,     0,
     164,     0,     0,     0,     0,   284,     0,     0,   173,     0,
       0,     0,     0,     0,   186,     0,    50,    55,    56,    59,
      60,    34,    33,     0,     0,   166,   480,   481,     0,     0,
     147,   150,   151,   154,   155,     0,     0,   168,   113,     0,
       0,   228,   229,     0,     0,     0,     0,   225,   182,     0,
       0,     0,     0,     0,     0,     0,   266,   226,   442,   501,
       0,   493,   185,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   319,   326,   327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,   440,
       0,   437,     0,     0,    63,     0,    65,   183,     0,    16,
     476,   475,   474,     0,     0,     0,   431,   426,   427,     0,
       0,     0,   423,     0,     0,   276,   275,     0,     0,     0,
      21,     0,    22,     0,    25,   176,     0,    36,    35,     0,
     501,   501,     0,   138,     0,   139,     0,     0,     0,   280,
     291,     0,   285,   286,   172,   287,   283,     0,   296,   298,
     300,     0,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   187,     0,     0,    79,    80,     0,     0,
       0,   169,   112,   224,     0,     0,     0,     0,     0,     0,
       0,     0,   181,   179,     0,     0,     0,     0,     0,     9,
       0,     0,   473,   479,   323,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,   304,   306,   308,   311,   309,
     310,   305,   307,   439,   436,   435,    62,   501,    76,    77,
      78,   272,   273,     0,     0,    13,    20,     0,     0,     0,
       0,   175,     7,     0,    45,    48,   133,   132,   499,     0,
     143,   163,   160,   161,   293,     0,     0,   288,     0,     0,
       0,     0,     0,   190,   191,   188,     0,     0,     0,     0,
       0,     0,     0,     0,   171,   167,   148,     0,     0,   159,
       0,     3,   262,     0,   259,     0,     0,     0,     0,   236,
       0,     0,     0,     0,   231,     0,     0,     0,     0,   232,
     233,     0,   178,   180,   268,   269,   270,     0,   184,     0,
       0,     0,   313,   315,   318,   316,   317,   314,    17,     0,
     501,   278,   277,     0,    23,     0,   140,   141,   281,   292,
       0,   289,   290,     0,     0,     0,   192,   194,   196,   199,
     197,   198,   193,   195,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,     0,     0,     0,   250,     0,
       0,     0,     0,     0,   238,     0,     0,     0,     0,   237,
     234,   235,     0,   265,   322,   321,   183,     0,     0,   274,
      14,   501,     8,     0,   144,   294,   200,   202,   201,     0,
     158,   261,     0,   204,   487,     0,     0,     0,   203,     0,
     258,     0,   247,   248,   240,     0,   243,     0,   246,   239,
       0,     0,   251,     0,     0,     0,   252,     0,     0,     0,
     320,   324,   325,    24,   142,     0,   260,     0,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     206,   257,     0,     0,   255,   256,   242,   253,   254,   241,
     264,     0,     0,     0,     0,     0,     0,   209,   210,   207,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   245,
     263,   157,   205,     0,     0,     0,   211,   213,   215,   218,
     216,   217,   212,   214,   219,   221,   220
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -907,  -907,     1,  -907,  -582,  -754,  -907,  -907,  -907,  -907,
    -907,   276,  1409,  -907,  -907,  -907,  -907,  -907,  -331,  -906,
     -44,  -907,  -907,  -907,  -907,   804,  -907,  -907,  -517,   886,
    -421,   265,  1201,    -2,  -258,  -907,  1184,  1186,   870,  1174,
    -907,   711,   -16,  1167,   967,   969,    -8,  -907,  -142,  -288,
    -907,  -907,   428,  -907
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   239,    71,    72,   588,    73,    74,    75,    76,
      77,  1038,    78,    79,   420,   612,    80,    81,   382,  1042,
     206,    82,    83,    84,   422,   639,    85,   644,   319,   559,
     601,   133,   192,   143,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   235,   231,
     102,   103,   144,   104
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      87,     2,   467,   641,   763,   503,   505,   134,   903,   355,
     207,   161,   718,   722,   138,   211,   212,    27,   724,    25,
     568,   153,   155,   157,   159,   160,   176,   773,   166,   168,
     170,   594,   595,   174,   175,   596,  1015,   179,   181,  1044,
     594,   595,   636,   162,   596,   196,   197,   530,   210,   201,
     203,   771,   637,   514,   367,   215,   216,   217,   425,   594,
     595,   313,   173,   596,   772,   314,   234,   237,   515,   594,
     595,   208,   123,   596,   368,   279,   280,   633,   634,   594,
     595,   378,   379,   596,   960,   594,   595,   814,   815,   596,
     209,   816,   281,   597,   598,   599,   663,   171,   124,   961,
     125,   172,   597,   598,   599,   281,   296,   297,   298,   299,
     300,   301,   315,   426,   306,   308,   664,   310,   281,   367,
     311,   597,   598,   599,   281,   185,   774,   331,    70,   332,
     234,   597,   598,   599,    70,  1016,     3,   334,   336,   368,
     338,   597,   598,   599,   723,   186,   745,   597,   598,   599,
     356,  1010,   366,   177,   371,  1133,   373,   793,   794,    70,
     600,   594,   595,   369,   375,   596,   370,   735,   817,   640,
     843,   638,    70,   145,   146,   147,   400,   148,   402,   635,
     862,   407,   666,   408,   409,   116,   410,   411,   744,   412,
     414,   415,   412,   416,   191,   385,   149,   124,   820,   125,
    1151,   150,   667,   117,  1160,   187,   775,  1045,  1106,   188,
     769,   213,   665,   342,  1110,   386,   376,  1046,   124,    70,
     125,   821,   822,   597,   598,   599,  1050,   343,   369,   189,
     484,   370,  1017,   590,   187,  1024,  1051,   448,   449,   450,
     451,   452,   453,   454,   455,   456,  1186,   457,   458,   459,
     460,   461,   462,   463,   464,  1201,   465,   814,   815,   795,
     199,   816,   341,   489,   490,    70,   238,    86,  1047,   778,
     106,   107,  1210,   776,   777,   344,  1120,   258,   496,   497,
     498,   499,   500,   501,   502,  1125,  1121,  1052,   214,   345,
    1178,   190,   367,   163,   165,  1126,   346,   385,   668,  1018,
    1019,   151,   115,   517,   387,  1230,   182,   184,   261,   818,
     347,   845,   368,   118,   198,   200,  1048,   386,   190,   536,
    1006,   538,   539,   540,   541,   542,   543,   544,   545,   322,
    1049,   119,  1144,   330,   385,  1053,   888,  1122,   943,   552,
     553,   278,   555,   183,   560,   889,  1127,   323,    70,  1054,
     385,   321,   120,   121,   386,   573,   574,   575,   576,   577,
     578,   579,   592,   593,   561,   582,   583,   563,   388,   122,
     386,   367,   418,   419,   823,   824,   825,   295,   562,   565,
     396,   564,   602,   604,   605,  1123,   606,   607,   389,   608,
     609,   368,   398,   566,  1128,   614,   163,   616,   397,  1124,
     312,   369,   620,   621,   370,   404,   387,  1165,  1129,   238,
     399,   629,  1167,   895,   669,   116,   631,   670,   671,   292,
     118,  1166,   372,   405,   374,   377,  1168,   672,   302,   645,
     673,   674,   293,   655,   675,   676,   858,   826,   656,   658,
     659,   660,   662,   387,   401,   352,   403,   294,   914,   353,
    1025,   526,   709,   710,     4,     5,     6,     7,   964,   387,
     711,   135,   136,   128,   129,   316,  1219,    13,    14,    15,
      16,    17,    18,    19,   594,   595,   380,   390,   596,   383,
     369,   381,   384,   370,   131,    29,   844,  1218,   952,   953,
     340,    35,   309,   341,   712,   713,   624,    37,   262,   263,
     264,   265,   714,   971,    86,    45,    46,    47,    48,   915,
     916,    51,   700,   917,   120,   121,   321,   383,   705,   706,
     384,    56,    57,   257,   120,   121,   945,    58,   720,   351,
     677,   657,   625,   552,   266,   267,   597,   598,   599,   972,
     124,  1187,   125,   304,   354,   106,   107,   324,   325,   326,
     327,   328,   731,   320,   391,   567,   234,    61,    62,   740,
     381,   742,   329,   743,   434,  1001,   435,   746,   747,   626,
     339,   918,   919,   920,   393,    63,   973,   115,  -403,  -403,
     392,    64,    65,   712,   713,   766,   627,    66,   394,   132,
     317,  1071,    69,   974,    70,   522,   318,   523,   836,   395,
     785,   358,   837,   106,   107,   324,   325,   326,   327,   328,
     522,   838,   725,   925,   839,   840,   926,   927,   841,  1107,
     329,   806,  1111,  1062,  1063,   686,   928,   139,   140,   929,
     930,   809,   975,   931,   932,   115,   433,   417,   286,    70,
     687,   287,   288,    87,   481,   482,   483,   976,   830,   204,
     205,   204,   205,   833,   834,   835,   289,   290,   291,   688,
     421,   846,   847,  1061,   848,   849,   977,   850,   851,   852,
     853,   854,   855,   856,   857,   341,  1020,   728,   689,   358,
    1021,   690,   630,   691,   632,   978,   276,   277,   979,   360,
     980,   946,   361,   362,   947,   948,  1022,   949,   950,   518,
    1192,   692,   519,  1193,  1194,   424,   873,   364,   981,   874,
     365,  1179,   875,  1195,   842,   423,  1196,  1197,   693,   879,
    1198,  1199,   257,   750,   732,   982,   427,   733,   429,   933,
    1072,  1214,   885,   886,   358,  1215,   752,   357,   753,   891,
     358,   359,   436,  1073,   360,  1074,   899,   361,   362,  1055,
     360,  1216,  1056,   361,   362,  1057,  1058,   363,   737,   547,
     341,   738,   831,   904,   268,   269,   270,   271,   272,   273,
     754,   755,   910,   259,   260,   756,   274,  1075,  1076,   921,
     922,   924,  1077,   757,   428,   364,   341,   686,   365,   687,
     275,   934,  1023,   580,  1096,   688,  1097,   240,   584,   689,
     241,   242,  1098,   939,   591,   761,  1099,   508,   341,   257,
     243,   509,   603,   244,   245,   510,  1200,   246,   247,   690,
     691,   692,   611,   693,   866,   750,  1100,  1101,  1102,   511,
    1103,  1134,  1146,   512,   965,   966,   967,   968,   969,   970,
     364,   752,   341,   365,   872,   756,   364,  1217,  1147,   365,
     762,   764,  1148,   513,   767,   768,   770,   647,   988,   649,
     249,   990,   653,   250,   251,  1181,  1040,    86,  1182,  1043,
    1132,  1041,   524,   252,  1041,  1041,   253,   254,   992,   218,
     255,   256,   800,   801,   802,   525,   526,   686,   687,   219,
     688,   220,   221,  1232,  1236,  1237,   807,  1238,   526,   810,
     222,   223,   348,   349,   350,   224,   248,   689,   690,  1014,
      86,   225,   226,   786,  1239,  1240,   341,   284,   285,   227,
     228,   229,   230,  1026,  1027,  1028,  1029,  1030,  1031,  1032,
    1033,   282,   283,   691,   692,   699,   527,   693,  1039,   218,
    1241,  1242,   869,   702,  1243,   341,   750,   704,   528,   219,
     707,   220,   221,  1244,   529,   716,   752,   719,  1067,   756,
     222,   223,   531,  1245,   892,   224,  1246,   893,   548,   257,
     995,   225,   226,   341,  1002,  1084,   537,  1003,   341,   227,
     228,   229,   230,  1142,   120,   121,   341,   486,   487,   488,
     944,  1079,   522,   549,   962,  1081,  1082,   430,   431,   569,
     749,   432,   433,   506,   507,   550,   516,   341,   570,   758,
     520,   521,   218,   613,  1089,   615,   504,   622,  1093,  1094,
    1095,   610,   219,   618,   220,   221,   532,   533,   534,   535,
     623,   905,   624,   222,   223,  1104,   551,   341,   224,   571,
     433,   625,  1109,   626,   225,   226,  1115,   430,   572,   430,
     581,  1119,   227,   228,   229,   230,   585,   431,   105,   935,
     106,   107,   108,   109,   110,   111,   112,   627,  1137,  1138,
     586,   587,   589,   431,   941,   628,   105,   113,   106,   107,
     108,   109,   110,   111,   112,   617,   431,   642,  1145,   643,
     114,   650,   115,   709,   710,   113,   619,   431,   430,   703,
     779,   780,   819,   780,   828,   341,   694,  1159,   654,   651,
     115,   678,   829,   341,   865,   341,   867,   341,   876,   341,
     679,  1173,   680,   860,   681,   863,  1177,   877,   533,   878,
     341,   868,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   882,   533,   682,  1039,   712,   713,
     430,   883,   683,  1188,  1189,  1191,   887,   738,   896,   780,
     684,   884,   685,  1202,   686,  1203,   281,  1007,   900,   341,
     275,  1011,  1012,  1013,   901,   341,   902,   587,   954,   780,
     687,   955,   780,   983,   341,  1213,   984,   533,   985,   341,
    1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,   695,  1034,
     987,   341,   696,  1036,   430,   999,  1008,   431,  1009,   587,
    1080,   341,  1233,  1234,  1235,     4,     5,     6,     7,  1136,
     341,   688,   135,   136,   128,   129,   316,   689,    13,    14,
      15,    16,    17,    18,    19,  1141,   341,  1143,   587,  1152,
     780,  1161,   780,  1211,   780,   131,    29,   193,   195,   492,
     493,   690,    35,   494,   495,   691,   692,   693,    37,   697,
     708,   721,   726,   734,   729,   730,    45,    46,    47,    48,
     727,   739,    51,  1086,  1087,   748,   750,   751,   752,   753,
     754,   755,    56,    57,   811,   756,   759,   760,    58,   381,
     787,   788,   789,   790,   791,   792,   796,   798,   805,   341,
     803,   804,   812,   832,   797,   799,   808,   870,   871,   864,
     866,   994,   880,   881,   890,   906,   907,   894,    61,    62,
     897,  1000,   898,   908,   909,   911,   912,   913,   936,   937,
     938,   940,   951,   942,   956,   957,    63,   958,   991,   963,
     959,   986,    64,    65,   988,   997,   989,   990,    66,   993,
     132,   717,   996,    69,  1004,    70,   998,   318,  1005,  1037,
    1035,  1059,  1060,  1064,  1065,  1066,  1070,  1078,  1088,  1090,
    1150,  1091,  1092,  1105,    86,  1108,  1114,  1112,  1113,  1116,
    1117,  1118,  1130,     4,     5,     6,     7,  1131,  1139,  1069,
     135,   136,   128,   129,   316,  1135,    13,    14,    15,    16,
      17,    18,    19,  1140,  1149,  1041,  1170,  1153,  1184,  1162,
    1163,  1164,  1169,   131,    29,  1171,  1172,  1174,  1175,  1176,
      35,  1180,  1183,  1204,  1205,  1185,    37,  1083,  1206,  1207,
    1208,  1085,  1209,  1212,    45,    46,    47,    48,   164,  1228,
      51,  1229,   813,   466,   736,     0,     0,   468,   491,   485,
      56,    57,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1231,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
      64,    65,     0,     0,     0,     0,    66,     0,   132,   861,
       0,    69,     0,    70,     0,   318,     4,     5,     6,     7,
       0,     0,     0,   135,   136,   128,   129,   316,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,    45,    46,    47,
      48,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,    64,    65,     0,     0,     0,     0,    66,
       0,   132,     0,     0,    69,     0,    70,     0,   318,     4,
       5,     6,     7,     0,     0,     0,   437,   438,   439,   440,
      12,     0,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,   441,   442,    25,     0,     0,    26,    27,    28,
      29,   443,   444,    32,    33,    34,    35,    36,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,   445,   446,
      45,    46,    47,    48,    49,    50,    51,    52,    53,     0,
       0,     0,    54,    55,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,    59,     0,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,     0,     0,
       0,     0,    66,    67,    68,     0,     0,    69,     0,    70,
     447,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,     0,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,   781,   782,    25,     0,     0,    26,
      27,    28,    29,    30,   783,    32,    33,    34,    35,    36,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,     0,     0,     0,    54,    55,     0,     0,    56,    57,
       0,     0,     0,     0,    58,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,    64,    65,
       0,     0,     0,     0,    66,    67,    68,     0,     0,    69,
       0,    70,   784,     4,     5,     6,     7,     0,     0,     0,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,     0,     0,     0,    54,    55,     0,     0,
      56,    57,     0,     0,     0,     0,    58,     0,     0,     0,
      59,     0,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
      64,    65,     0,     0,     0,     0,    66,    67,    68,     0,
       0,    69,     0,    70,   827,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,  1154,    12,     0,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,  1155,  1156,
      25,     0,     0,    26,    27,    28,    29,    30,  1157,    32,
      33,    34,    35,    36,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,     0,     0,     0,    54,    55,
       0,     0,    56,    57,     0,     0,     0,     0,    58,     0,
       0,     0,    59,     0,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,    61,    62,
       0,   135,   136,   128,   129,   130,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    63,     0,     0,     0,
       0,     0,    64,    65,   131,    29,     0,     0,    66,    67,
      68,    35,     0,    69,     0,    70,  1158,    37,     0,     0,
       0,     0,     0,     0,     0,    45,    46,    47,    48,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,    61,    62,     0,
     135,   136,   128,   129,   130,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    63,     0,     0,     0,     0,
       0,    64,    65,   131,    29,     0,     0,    66,   167,   132,
      35,     0,    69,     0,    70,   661,    37,     0,     0,     0,
       0,     0,     0,     0,    45,    46,    47,    48,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,    57,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    61,    62,     0,   135,
     136,   128,   129,   130,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    63,     0,     0,     0,     0,     0,
      64,    65,   131,    29,     0,     0,    66,   167,   132,    35,
       0,    69,     0,    70,   923,    37,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,    64,
      65,     0,     0,     0,     0,    66,   167,   132,     0,     0,
      69,     0,    70,  1190,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       0,     0,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,     0,     0,     0,    54,    55,     0,
       0,    56,    57,     0,     0,     0,     0,    58,     0,     0,
       0,    59,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,    61,    62,     0,
     135,   136,   128,   129,   130,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    63,     0,     0,     0,     0,
       0,    64,    65,   131,    29,     0,     0,    66,    67,    68,
      35,     0,    69,     0,    70,     0,    37,     0,     0,     0,
       0,     0,     0,     0,    45,    46,    47,    48,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,    57,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    61,    62,     0,   135,
     136,   128,   129,   130,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    63,     0,     0,     0,     0,     0,
      64,    65,   131,    29,     0,     0,    66,     0,   132,    35,
       0,    69,   137,    70,     0,    37,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,    61,    62,     0,   135,   136,
     128,   129,   130,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    63,     0,     0,     0,     0,     0,    64,
      65,   131,    29,     0,     0,    66,   141,   142,    35,     0,
      69,     0,    70,     0,    37,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    61,    62,     0,   135,   136,   128,
     129,   130,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    63,     0,     0,     0,     0,     0,    64,    65,
     131,    29,     0,     0,    66,   167,   132,    35,     0,    69,
       0,    70,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,    61,    62,     0,   135,   136,   128,   129,
     130,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    63,     0,     0,     0,     0,     0,    64,    65,   131,
      29,     0,     0,    66,   169,   132,    35,     0,    69,     0,
      70,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    61,    62,     0,   232,   136,   128,   129,   130,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,   131,    29,
       0,     0,    66,   202,   132,    35,     0,    69,     0,    70,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,    48,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,    64,    65,     0,     0,     0,
       0,    66,     0,   132,   233,     0,    69,     0,    70,     4,
       5,     6,     7,     0,     0,     0,   135,   136,   128,   129,
     130,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    61,    62,     0,   135,   136,   128,   129,   130,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,   131,    29,
       0,     0,    66,     0,   132,    35,     0,    69,   236,    70,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,    48,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,    64,    65,     0,     0,     0,
       0,    66,     0,   132,   303,     0,    69,     0,    70,     4,
       5,     6,     7,     0,     0,     0,   135,   136,   128,   129,
     130,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,   307,     0,
       0,     0,    66,     0,   132,     0,     0,    69,     0,    70,
       4,     5,     6,     7,     0,     0,     0,   135,   136,   128,
     129,   130,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     131,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,    64,    65,     0,
       0,     0,     0,    66,     0,   132,   233,     0,    69,     0,
      70,     4,     5,     6,     7,     0,     0,     0,   135,   136,
     128,   129,   130,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    61,    62,     0,   135,   136,   128,
     129,   130,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    63,     0,     0,     0,     0,     0,    64,    65,
     131,    29,     0,     0,    66,   333,   132,    35,     0,    69,
       0,    70,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,    61,    62,     0,   135,   136,   128,   129,
     130,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    63,     0,     0,     0,     0,     0,    64,    65,   131,
      29,     0,     0,    66,   335,   132,    35,     0,    69,     0,
      70,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,     0,     0,
       0,     0,    66,     0,   132,   337,     0,    69,     0,    70,
       4,     5,     6,     7,     0,     0,     0,   135,   136,   128,
     129,   130,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,   406,     0,     0,     0,     0,     0,     0,     0,
     131,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,    61,    62,     0,   135,   136,   128,   129,
     130,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    63,     0,     0,     0,     0,     0,    64,    65,   131,
      29,     0,     0,    66,     0,   132,    35,     0,    69,     0,
      70,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    61,    62,     0,   135,   136,   128,   129,   130,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,   131,    29,
       0,     0,    66,     0,   132,    35,   413,    69,     0,    70,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,    48,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,    58,     0,     0,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,    61,    62,     0,   135,   136,   128,   129,   130,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    63,
       0,     0,     0,     0,     0,    64,    65,   131,    29,     0,
       0,    66,     0,   132,    35,     0,    69,     0,    70,     0,
      37,     0,     0,     0,     0,     0,     0,     0,    45,    46,
      47,    48,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      61,    62,     0,   135,   136,   128,   129,   130,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    63,     0,
       0,     0,     0,     0,    64,    65,   131,    29,     0,     0,
      66,     0,   132,    35,     0,    69,   546,    70,     0,    37,
       0,     0,     0,     0,     0,     0,     0,    45,    46,    47,
      48,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,    64,    65,     0,     0,     0,     0,    66,
       0,   132,   646,     0,    69,     0,    70,     4,     5,     6,
       7,     0,     0,     0,   135,   136,   128,   129,   130,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,     0,    45,    46,
      47,    48,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,    64,    65,     0,     0,     0,     0,
      66,     0,   132,   648,     0,    69,     0,    70,     4,     5,
       6,     7,     0,     0,     0,   135,   136,   128,   129,   130,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   131,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,    48,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,    61,    62,     0,   135,   136,   128,   129,   130,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    63,
       0,     0,     0,     0,     0,    64,    65,   131,    29,     0,
       0,    66,     0,   132,    35,     0,    69,   652,    70,     0,
      37,     0,     0,     0,     0,     0,     0,     0,    45,    46,
      47,    48,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,    64,    65,     0,     0,     0,     0,
      66,     0,   132,   698,     0,    69,     0,    70,     4,     5,
       6,     7,     0,     0,     0,   135,   136,   128,   129,   130,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   131,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,    48,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,    64,    65,     0,     0,     0,
       0,    66,     0,   132,   701,     0,    69,     0,    70,     4,
       5,     6,     7,     0,     0,     0,   135,   136,   128,   129,
     130,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,     0,     0,
       0,     0,    66,     0,   132,   715,     0,    69,     0,    70,
       4,     5,   556,     7,     0,     0,     0,   135,   136,   128,
     129,   557,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     131,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,    64,    65,     0,
       0,     0,     0,    66,     0,   132,   233,     0,    69,     0,
      70,     4,     5,     6,     7,     0,     0,     0,   135,   136,
     128,   129,   130,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,    64,    65,
       0,     0,     0,     0,    66,     0,   132,   859,     0,    69,
       0,    70,     4,     5,     6,     7,     0,     0,     0,   135,
     136,   128,   129,   130,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   131,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,    64,
      65,     0,     0,     0,     0,    66,     0,   132,  1068,     0,
      69,     0,    70,     4,     5,     6,     7,     0,     0,     0,
     135,   136,   128,   129,   130,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,    45,    46,    47,    48,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,    57,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    61,    62,     0,   135,
     136,   128,   129,   130,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    63,     0,     0,     0,     0,     0,
      64,    65,   131,    29,     0,     0,    66,     0,   152,    35,
       0,    69,     0,    70,     0,    37,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,    61,    62,     0,   135,   136,
     128,   129,   130,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    63,     0,     0,     0,     0,     0,    64,
      65,   131,    29,     0,     0,    66,     0,   154,    35,     0,
      69,     0,    70,     0,    37,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    61,    62,     0,   135,   136,   128,
     129,   130,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    63,     0,     0,     0,     0,     0,    64,    65,
     131,    29,     0,     0,    66,     0,   156,    35,     0,    69,
       0,    70,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,    61,    62,     0,   135,   136,   128,   129,
     130,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    63,     0,     0,     0,     0,     0,    64,    65,   131,
      29,     0,     0,    66,     0,   158,    35,     0,    69,     0,
      70,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    61,    62,     0,   135,   136,   128,   129,   130,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      63,     0,     0,     0,     0,     0,    64,    65,   131,    29,
       0,     0,    66,     0,   132,    35,     0,    69,     0,    70,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,    48,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,    61,    62,     0,   135,   136,   128,   129,   130,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    63,
       0,     0,     0,     0,     0,    64,    65,   131,    29,     0,
       0,    66,     0,   178,    35,     0,    69,     0,    70,     0,
      37,     0,     0,     0,     0,     0,     0,     0,    45,    46,
      47,    48,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      61,    62,     0,   135,   136,   128,   129,   130,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    63,     0,
       0,     0,     0,     0,    64,    65,   131,    29,     0,     0,
      66,     0,   180,    35,     0,    69,     0,    70,     0,    37,
       0,     0,     0,     0,     0,     0,     0,    45,    46,    47,
      48,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,    61,
      62,     0,   135,   136,   128,   129,   130,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,    63,     0,     0,
       0,     0,     0,    64,    65,   131,    29,     0,     0,    66,
       0,   132,    35,     0,    69,     0,   194,     0,    37,     0,
       0,     0,     0,     0,     0,     0,    45,    46,    47,    48,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,    61,    62,
       0,   135,   136,   128,   129,   130,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    63,     0,     0,     0,
       0,     0,    64,    65,   131,    29,     0,     0,    66,     0,
     305,    35,     0,    69,     0,    70,     0,    37,     0,     0,
       0,     0,     0,     0,     0,    45,    46,    47,    48,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,   556,     7,    61,    62,     0,
     135,   136,   128,   129,   557,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    63,     0,     0,     0,     0,
       0,    64,    65,   131,    29,     0,     0,    66,     0,   554,
      35,     0,    69,     0,    70,     0,    37,     0,     0,     0,
       0,     0,     0,     0,    45,    46,    47,    48,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,    57,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    61,    62,     0,   135,
     136,   128,   129,   130,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    63,     0,     0,     0,     0,     0,
      64,    65,   131,    29,     0,     0,    66,     0,   558,    35,
       0,    69,     0,    70,     0,    37,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,    61,    62,     0,   135,   136,
     128,   129,   130,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    63,     0,     0,     0,     0,     0,    64,
      65,   131,    29,     0,     0,    66,     0,   741,    35,     0,
      69,     0,    70,     0,    37,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    61,    62,     0,   126,   127,   128,
     129,   130,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    63,     0,     0,     0,     0,     0,    64,    65,
     131,     0,     0,     0,    66,     0,   765,    35,     0,    69,
       0,    70,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -502,  -502,     0,    61,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,    64,    65,     0,
       0,     0,     0,    66,     0,   132,     0,     0,    69,     0,
      70,  -502,  -502,  -502,     0,  -502,  -502,  -502,  -502,  -502,
    -502,  -502,  -502,  -502,  -502,  -502,  -502,  -502,     0,     0,
       0,  -502,     0,     0,     0,     0,     0,     0,     0,  -502,
    -502,     0,     0,  -502,  -502,  -502,  -502,  -502,  -502,     0,
       0,  -502,  -502,  -502,  -502,  -502,     0,     0,  -502,  -502,
    -502
};

static const yytype_int16 yycheck[] =
{
       2,     0,   260,   424,   586,   293,   294,    15,   762,   151,
      54,    27,   529,    14,    16,    59,    60,    32,   535,    28,
     351,    23,    24,    25,    26,    27,    12,    24,    30,    31,
      32,    10,    11,    35,    36,    14,    24,    39,    40,   945,
      10,    11,    14,    14,    14,    47,    48,    85,    56,    51,
      52,    88,    24,   123,    28,    63,    64,    65,    14,    10,
      11,    14,    14,    14,    88,    18,    68,    69,   138,    10,
      11,    14,    11,    14,    48,    86,    87,    42,    43,    10,
      11,    86,    87,    14,   123,    10,    11,    63,    64,    14,
      33,    67,   129,    72,    73,    74,    28,    10,   136,   138,
     138,    14,    72,    73,    74,   129,   108,   109,   110,   111,
     112,   113,    65,    69,   116,   117,    48,   119,   129,    28,
     122,    72,    73,    74,   129,    28,   123,   140,   143,   142,
     132,    72,    73,    74,   143,   123,     0,   139,   140,    48,
     142,    72,    73,    74,   145,    48,   567,    72,    73,    74,
     152,   905,   154,    12,   156,  1061,   158,    42,    43,   143,
     139,    10,    11,   137,    88,    14,   140,    85,   144,   139,
     144,   143,   143,    10,    11,    12,   178,    14,   180,   144,
     697,   183,    28,   185,   186,   123,   188,   189,   139,   191,
     192,   193,   194,   195,   143,    28,    33,   136,   139,   138,
    1106,    38,    48,   141,  1110,   137,    10,    14,   139,    28,
      88,    10,   144,   123,   139,    48,   140,    24,   136,   143,
     138,   642,   643,    72,    73,    74,    14,   137,   137,    48,
     274,   140,    10,   375,   137,   144,    24,   239,   240,   241,
     242,   243,   244,   245,   246,   247,  1152,   249,   250,   251,
     252,   253,   254,   255,   256,  1161,   258,    63,    64,   144,
     143,    67,   140,   279,   280,   143,    79,     2,    75,   600,
     106,   107,  1178,    77,    78,   123,    14,    85,   286,   287,
     288,   289,   290,   291,   292,    14,    24,    75,    10,   137,
     139,   137,    28,    28,    29,    24,   123,    28,   144,    77,
      78,   138,   138,   305,   137,  1211,    41,    42,    99,   640,
     137,   144,    48,   125,    49,    50,   123,    48,   137,   321,
     902,   323,   324,   325,   326,   327,   328,   329,   330,   125,
     137,   143,  1086,   123,    28,   123,     5,    75,   144,   341,
     342,   113,   344,   138,   346,    14,    75,   143,   143,   137,
      28,   141,   106,   107,    48,   357,   358,   359,   360,   361,
     362,   363,   378,   379,   123,   367,   368,   123,    28,   123,
      48,    28,    42,    43,    81,    82,    83,    14,   137,   123,
     125,   137,   381,   385,   386,   123,   388,   389,    48,   391,
     392,    48,   125,   137,   123,   397,   131,   399,   143,   137,
     137,   137,   404,   405,   140,    31,   137,   123,   137,    79,
     143,   413,   123,   744,    28,   123,   418,    31,    32,   122,
     125,   137,   157,    49,   159,   160,   137,    41,    14,   428,
      44,    45,   136,   141,    48,    49,   694,   144,   143,   441,
     442,   443,   444,   137,   179,    10,   181,   136,   779,    14,
     144,   137,   115,   116,     3,     4,     5,     6,   144,   137,
     123,    10,    11,    12,    13,    14,   144,    16,    17,    18,
      19,    20,    21,    22,    10,    11,   138,   137,    14,   137,
     137,   143,   140,   140,    33,    34,   144,   144,   819,   820,
     137,    40,    14,   140,   115,   116,   137,    46,    89,    90,
      91,    92,   123,   144,   239,    54,    55,    56,    57,    10,
      11,    60,   514,    14,   106,   107,   141,   137,   520,   521,
     140,    70,    71,   137,   106,   107,   814,    76,   530,    14,
     144,   123,   137,   535,   125,   126,    72,    73,    74,   144,
     136,   123,   138,   115,    10,   106,   107,   108,   109,   110,
     111,   112,   554,   125,   123,   138,   558,   106,   107,   561,
     143,   563,   123,   565,   140,   896,   142,   569,   570,   137,
     142,    72,    73,    74,    76,   124,   144,   138,   139,   140,
     123,   130,   131,   115,   116,   587,   137,   136,   140,   138,
     139,   123,   141,   144,   143,   140,   145,   142,    28,   143,
     602,    31,    32,   106,   107,   108,   109,   110,   111,   112,
     140,    41,   142,    28,    44,    45,    31,    32,    48,  1040,
     123,   623,  1043,   954,   955,   137,    41,    14,    15,    44,
      45,   633,   144,    48,    49,   138,   140,    59,   114,   143,
     137,   117,   118,   645,     7,     8,     9,   144,   650,    14,
      15,    14,    15,   655,   656,   657,   132,   133,   134,   137,
     137,   663,   664,   951,   666,   667,   144,   669,   670,   671,
     672,   673,   674,   675,   676,   140,    28,   142,   137,    31,
      32,   137,   417,   137,   419,   144,   127,   128,   144,    41,
     144,    10,    44,    45,    13,    14,    48,    16,    17,   137,
      28,   137,   140,    31,    32,   138,   708,   137,   144,   711,
     140,  1132,   714,    41,   144,   143,    44,    45,   137,   721,
      48,    49,   137,   137,   137,   144,   143,   140,   138,   144,
     144,    28,   734,   735,    31,    32,   137,    28,   137,   741,
      31,    32,   144,   144,    41,   144,   748,    44,    45,    10,
      41,    48,    13,    44,    45,    16,    17,    48,   137,   331,
     140,   140,   142,   765,    93,    94,    95,    96,    97,    98,
     137,   137,   774,   100,   101,   137,   105,   144,   144,   781,
     782,   783,   144,   137,   143,   137,   140,   137,   140,   137,
     119,   793,   144,   365,   144,   137,   144,    28,   370,   137,
      31,    32,   144,   805,   376,   137,   144,   137,   140,   137,
      41,   137,   384,    44,    45,   137,   144,    48,    49,   137,
     137,   137,   394,   137,   137,   137,   144,   144,   144,   137,
     144,   144,   144,   137,   836,   837,   838,   839,   840,   841,
     137,   137,   140,   140,   142,   137,   137,   144,   144,   140,
     585,   586,   144,   137,   589,   590,   591,   429,   137,   431,
      28,   137,   434,    31,    32,   144,   138,   602,   144,   138,
     138,   143,   126,    41,   143,   143,    44,    45,   880,    14,
      48,    49,   617,   618,   619,   144,   137,   137,   137,    24,
     137,    26,    27,   144,   144,   144,   631,   144,   137,   634,
      35,    36,    10,    11,    12,    40,   137,   137,   137,   911,
     645,    46,    47,   137,   144,   144,   140,   130,   131,    54,
      55,    56,    57,   925,   926,   927,   928,   929,   930,   931,
     932,   120,   121,   137,   137,   507,   138,   137,   940,    14,
     144,   144,   137,   515,   144,   140,   137,   519,    14,    24,
     522,    26,    27,   144,   138,   527,   137,   529,   960,   137,
      35,    36,    14,   144,   137,    40,   144,   140,   137,   137,
     137,    46,    47,   140,   137,   137,    14,   140,   140,    54,
      55,    56,    57,   137,   106,   107,   140,   276,   277,   278,
      65,   993,   140,   137,   142,   997,   998,   139,   140,   123,
     572,   139,   140,   137,   138,   137,   139,   140,   123,   581,
     130,   131,    14,    14,  1016,    14,    18,    10,  1020,  1021,
    1022,   137,    24,    29,    26,    27,   139,   140,   139,   140,
     137,   766,   137,    35,    36,  1037,   139,   140,    40,   139,
     140,   137,  1041,   137,    46,    47,  1048,   139,   140,   139,
     140,  1053,    54,    55,    56,    57,   139,   140,   104,   794,
     106,   107,   108,   109,   110,   111,   112,   137,  1070,  1071,
      29,    30,   139,   140,   809,   144,   104,   123,   106,   107,
     108,   109,   110,   111,   112,   139,   140,   138,  1090,   138,
     136,   123,   138,   115,   116,   123,   139,   140,   139,   140,
     139,   140,   139,   140,   139,   140,   102,  1109,   136,    10,
     138,   137,   139,   140,   139,   140,   139,   140,   139,   140,
     137,  1123,   137,   695,   137,   697,  1128,   139,   140,   139,
     140,   703,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   139,   140,   137,  1149,   115,   116,
     139,   140,   137,  1155,  1156,  1157,   139,   140,   139,   140,
     137,   733,   137,  1165,   137,  1167,   129,   902,   139,   140,
     119,   906,   907,   908,   139,   140,    29,    30,   139,   140,
     137,   139,   140,   139,   140,  1187,   139,   140,   139,   140,
    1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,   138,   934,
     139,   140,    14,   938,   139,   140,   139,   140,    29,    30,
     139,   140,  1214,  1215,  1216,     3,     4,     5,     6,   139,
     140,   137,    10,    11,    12,    13,    14,   137,    16,    17,
      18,    19,    20,    21,    22,   139,   140,    29,    30,   139,
     140,   139,   140,   139,   140,    33,    34,    46,    47,   282,
     283,   137,    40,   284,   285,   137,   137,   137,    46,   138,
     123,   138,   126,    85,   137,   137,    54,    55,    56,    57,
     144,   137,    60,  1008,  1009,   123,   137,   137,   137,   137,
     137,   137,    70,    71,    14,   137,   137,   137,    76,   143,
     137,   137,   137,   137,   137,   137,   126,   126,   123,   140,
     137,   137,    14,    14,   144,   144,   144,   142,   142,   137,
     137,   883,    85,    14,   137,    10,    10,   137,   106,   107,
     137,   893,   137,    10,    14,   123,    10,    10,   137,   137,
      29,   137,    63,   144,    14,    14,   124,    14,   139,   144,
     137,   137,   130,   131,   137,    85,   137,   137,   136,   138,
     138,   139,   137,   141,   137,   143,    85,   145,   137,   137,
     144,   137,   137,     7,     7,     7,   123,   137,    14,   123,
    1105,    10,    10,   139,  1109,    79,   137,    14,    14,    14,
      14,   137,   137,     3,     4,     5,     6,   137,   139,   961,
      10,    11,    12,    13,    14,   144,    16,    17,    18,    19,
      20,    21,    22,   137,   137,   143,    14,   144,  1143,   137,
     137,   137,   137,    33,    34,    14,   137,    14,    14,   137,
      40,   144,   137,   137,   137,  1149,    46,   999,   137,   137,
     137,  1003,   137,   139,    54,    55,    56,    57,    29,   137,
      60,   137,   638,   259,   558,    -1,    -1,   261,   281,   275,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,   141,    -1,   143,    -1,   145,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,    -1,   141,    -1,   143,    -1,   145,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,   141,    -1,   143,
     144,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,   141,
      -1,   143,   144,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,   141,    -1,   143,   144,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,   106,   107,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,   130,   131,    33,    34,    -1,    -1,   136,   137,
     138,    40,    -1,   141,    -1,   143,   144,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,   106,   107,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,   130,   131,    33,    34,    -1,    -1,   136,   137,   138,
      40,    -1,   141,    -1,   143,   144,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,   106,   107,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
     130,   131,    33,    34,    -1,    -1,   136,   137,   138,    40,
      -1,   141,    -1,   143,   144,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,
     141,    -1,   143,   144,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,   106,   107,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,   130,   131,    33,    34,    -1,    -1,   136,   137,   138,
      40,    -1,   141,    -1,   143,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,   106,   107,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
     130,   131,    33,    34,    -1,    -1,   136,    -1,   138,    40,
      -1,   141,   142,   143,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,   106,   107,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,
     131,    33,    34,    -1,    -1,   136,   137,   138,    40,    -1,
     141,    -1,   143,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,   106,   107,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
      33,    34,    -1,    -1,   136,   137,   138,    40,    -1,   141,
      -1,   143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,   106,   107,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,
      34,    -1,    -1,   136,   137,   138,    40,    -1,   141,    -1,
     143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,   106,   107,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,    34,
      -1,    -1,   136,   137,   138,    40,    -1,   141,    -1,   143,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,   141,    -1,   143,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,   106,   107,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,    34,
      -1,    -1,   136,    -1,   138,    40,    -1,   141,   142,   143,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,   141,    -1,   143,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,   141,    -1,
     143,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,   106,   107,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
      33,    34,    -1,    -1,   136,   137,   138,    40,    -1,   141,
      -1,   143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,   106,   107,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,
      34,    -1,    -1,   136,   137,   138,    40,    -1,   141,    -1,
     143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,   141,    -1,   143,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,   106,   107,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,
      34,    -1,    -1,   136,    -1,   138,    40,    -1,   141,    -1,
     143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,   106,   107,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,    34,
      -1,    -1,   136,    -1,   138,    40,   140,   141,    -1,   143,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,   106,   107,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,    33,    34,    -1,
      -1,   136,    -1,   138,    40,    -1,   141,    -1,   143,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
     106,   107,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,   130,   131,    33,    34,    -1,    -1,
     136,    -1,   138,    40,    -1,   141,   142,   143,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,   141,    -1,   143,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,   141,    -1,   143,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,   106,   107,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,    33,    34,    -1,
      -1,   136,    -1,   138,    40,    -1,   141,   142,   143,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,   141,    -1,   143,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,   141,    -1,   143,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,   141,    -1,   143,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,   141,    -1,
     143,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,   141,
      -1,   143,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
     141,    -1,   143,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,   106,   107,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
     130,   131,    33,    34,    -1,    -1,   136,    -1,   138,    40,
      -1,   141,    -1,   143,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,   106,   107,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,
     131,    33,    34,    -1,    -1,   136,    -1,   138,    40,    -1,
     141,    -1,   143,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,   106,   107,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
      33,    34,    -1,    -1,   136,    -1,   138,    40,    -1,   141,
      -1,   143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,   106,   107,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,
      34,    -1,    -1,   136,    -1,   138,    40,    -1,   141,    -1,
     143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,   106,   107,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    33,    34,
      -1,    -1,   136,    -1,   138,    40,    -1,   141,    -1,   143,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,   106,   107,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,    33,    34,    -1,
      -1,   136,    -1,   138,    40,    -1,   141,    -1,   143,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
     106,   107,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,   130,   131,    33,    34,    -1,    -1,
     136,    -1,   138,    40,    -1,   141,    -1,   143,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,   106,
     107,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,   130,   131,    33,    34,    -1,    -1,   136,
      -1,   138,    40,    -1,   141,    -1,   143,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,   106,   107,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,   130,   131,    33,    34,    -1,    -1,   136,    -1,
     138,    40,    -1,   141,    -1,   143,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,   106,   107,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,   130,   131,    33,    34,    -1,    -1,   136,    -1,   138,
      40,    -1,   141,    -1,   143,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,   106,   107,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
     130,   131,    33,    34,    -1,    -1,   136,    -1,   138,    40,
      -1,   141,    -1,   143,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,   106,   107,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,
     131,    33,    34,    -1,    -1,   136,    -1,   138,    40,    -1,
     141,    -1,   143,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,   106,   107,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
      33,    -1,    -1,    -1,   136,    -1,   138,    40,    -1,   141,
      -1,   143,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    45,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
     143,    85,    86,    87,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,   117,   118,   119,   120,   121,   122,    -1,
      -1,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   147,   148,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    66,    67,    70,    71,    76,    80,
      84,   106,   107,   124,   130,   131,   136,   137,   138,   141,
     143,   149,   150,   152,   153,   154,   155,   156,   158,   159,
     162,   163,   167,   168,   169,   172,   177,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   196,   197,   199,   104,   106,   107,   108,   109,
     110,   111,   112,   123,   136,   138,   123,   141,   125,   143,
     106,   107,   123,    11,   136,   138,    10,    11,    12,    13,
      14,    33,   138,   177,   192,    10,    11,   142,   179,    14,
      15,   137,   138,   179,   198,    10,    11,    12,    14,    33,
      38,   138,   138,   179,   138,   179,   138,   179,   138,   179,
     179,   188,    14,   177,   158,   177,   179,   137,   179,   137,
     179,    10,    14,    14,   179,   179,    12,    12,   138,   179,
     138,   179,   177,   138,   177,    28,    48,   137,    28,    48,
     137,   143,   178,   178,   143,   178,   179,   179,   177,   143,
     177,   179,   137,   179,    14,    15,   166,   166,    14,    33,
     192,   166,   166,    10,    10,   192,   192,   192,    14,    24,
      26,    27,    35,    36,    40,    46,    47,    54,    55,    56,
      57,   195,    10,   139,   179,   194,   142,   179,    79,   148,
      28,    31,    32,    41,    44,    45,    48,    49,   137,    28,
      31,    32,    41,    44,    45,    48,    49,   137,    85,   100,
     101,    99,    89,    90,    91,    92,   125,   126,    93,    94,
      95,    96,    97,    98,   105,   119,   127,   128,   113,    86,
      87,   129,   120,   121,   130,   131,   114,   117,   118,   132,
     133,   134,   122,   136,   136,    14,   179,   179,   179,   179,
     179,   179,    14,   139,   198,   138,   179,   132,   179,    14,
     179,   179,   137,    14,    18,    65,    14,   139,   145,   174,
     198,   141,   125,   143,   108,   109,   110,   111,   112,   123,
     123,   140,   142,   137,   179,   137,   179,   139,   179,   198,
     137,   140,   123,   137,   123,   137,   123,   137,    10,    11,
      12,    14,    10,    14,    10,   194,   179,    28,    31,    32,
      41,    44,    45,    48,   137,   140,   179,    28,    48,   137,
     140,   179,   177,   179,   177,    88,   140,   177,    86,    87,
     138,   143,   164,   137,   140,    28,    48,   137,    28,    48,
     137,   123,   123,    76,   140,   143,   125,   143,   125,   143,
     179,   177,   179,   177,    31,    49,    25,   179,   179,   179,
     179,   179,   179,   140,   179,   179,   179,    59,    42,    43,
     160,   137,   170,   143,   138,    14,    69,   143,   143,   138,
     139,   140,   139,   140,   140,   142,   144,    10,    11,    12,
      13,    26,    27,    35,    36,    52,    53,   144,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   182,   180,   183,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,     7,     8,     9,   166,   185,   187,   187,   187,   188,
     188,   189,   190,   190,   191,   191,   192,   192,   192,   192,
     192,   192,   192,   195,    18,   195,   137,   138,   137,   137,
     137,   137,   137,   137,   123,   138,   139,   179,   137,   140,
     130,   131,   140,   142,   126,   144,   137,   138,    14,   138,
      85,    14,   139,   140,   139,   140,   179,    14,   179,   179,
     179,   179,   179,   179,   179,   179,   142,   198,   137,   137,
     137,   139,   179,   179,   138,   179,     5,    14,   138,   175,
     179,   123,   137,   123,   137,   123,   137,   138,   164,   123,
     123,   139,   140,   179,   179,   179,   179,   179,   179,   179,
     198,   140,   179,   179,   198,   139,    29,    30,   151,   139,
     194,   198,   188,   188,    10,    11,    14,    72,    73,    74,
     139,   176,   148,   198,   179,   179,   179,   179,   179,   179,
     137,   198,   161,    14,   179,    14,   179,   139,    29,   139,
     179,   179,    10,   137,   137,   137,   137,   137,   144,   179,
     177,   179,   177,    42,    43,   144,    14,    24,   143,   171,
     139,   176,   138,   138,   173,   148,   139,   198,   139,   198,
     123,    10,   142,   198,   136,   141,   143,   123,   179,   179,
     179,   144,   179,    28,    48,   144,    28,    48,   144,    28,
      31,    32,    41,    44,    45,    48,    49,   144,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   102,   138,    14,   138,   139,   198,
     179,   139,   198,   140,   198,   179,   179,   198,   123,   115,
     116,   123,   115,   116,   123,   139,   198,   139,   174,   198,
     179,   138,    14,   145,   174,   142,   126,   144,   142,   137,
     137,   179,   137,   140,    85,    85,   175,   137,   140,   137,
     179,   138,   179,   179,   139,   176,   179,   179,   123,   198,
     137,   137,   137,   137,   137,   137,   137,   137,   198,   137,
     137,   137,   177,   150,   177,   138,   179,   177,   177,    88,
     177,    88,    88,    24,   123,    10,    77,    78,   164,   139,
     140,    26,    27,    36,   144,   179,   137,   137,   137,   137,
     137,   137,   137,    42,    43,   144,   126,   144,   126,   144,
     177,   177,   177,   137,   137,   123,   179,   177,   144,   179,
     177,    14,    14,   171,    63,    64,    67,   144,   164,   139,
     139,   176,   176,    81,    82,    83,   144,   144,   139,   139,
     179,   142,    14,   179,   179,   179,    28,    32,    41,    44,
      45,    48,   144,   144,   144,   144,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   180,   139,
     198,   139,   174,   198,   137,   139,   137,   139,   198,   137,
     142,   142,   142,   179,   179,   179,   139,   139,   139,   179,
      85,    14,   139,   140,   198,   179,   179,   139,     5,    14,
     137,   179,   137,   140,   137,   164,   139,   137,   137,   179,
     139,   139,    29,   151,   179,   177,    10,    10,    10,    14,
     179,   123,    10,    10,   164,    10,    11,    14,    72,    73,
      74,   179,   179,   144,   179,    28,    31,    32,    41,    44,
      45,    48,    49,   144,   179,   177,   137,   137,    29,   179,
     137,   177,   144,   144,    65,   195,    10,    13,    14,    16,
      17,    63,   164,   164,   139,   139,    14,    14,    14,   137,
     123,   138,   142,   144,   144,   179,   179,   179,   179,   179,
     179,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   139,   139,   139,   137,   139,   137,   137,
     137,   139,   179,   138,   198,   137,   137,    85,    85,   140,
     198,   164,   137,   140,   137,   137,   150,   177,   139,    29,
     151,   177,   177,   177,   179,    24,   123,    10,    77,    78,
      28,    32,    48,   144,   144,   144,   179,   179,   179,   179,
     179,   179,   179,   179,   177,   144,   177,   137,   157,   179,
     138,   143,   165,   138,   165,    14,    24,    75,   123,   137,
      14,    24,    75,   123,   137,    10,    13,    16,    17,   137,
     137,   195,   164,   164,     7,     7,     7,   179,   139,   198,
     123,   123,   144,   144,   144,   144,   144,   144,   137,   179,
     139,   179,   179,   198,   137,   198,   177,   177,    14,   179,
     123,    10,    10,   179,   179,   179,   144,   144,   144,   144,
     144,   144,   144,   144,   179,   139,   139,   176,    79,   148,
     139,   176,    14,    14,   137,   179,    14,    14,   137,   179,
      14,    24,    75,   123,   137,    14,    24,    75,   123,   137,
     137,   137,   138,   165,   144,   144,   139,   179,   179,   139,
     137,   139,   137,    29,   151,   179,   144,   144,   144,   137,
     177,   165,   139,   144,    13,    26,    27,    36,   144,   179,
     165,   139,   137,   137,   137,   123,   137,   123,   137,   137,
      14,    14,   137,   179,    14,    14,   137,   179,   139,   176,
     144,   144,   144,   137,   177,   157,   165,   123,   179,   179,
     144,   179,    28,    31,    32,    41,    44,    45,    48,    49,
     144,   165,   179,   179,   137,   137,   137,   137,   137,   137,
     165,   139,   139,   179,    28,    32,    48,   144,   144,   144,
     179,   179,   179,   179,   179,   179,   179,   179,   137,   137,
     165,   177,   144,   179,   179,   179,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   146,   147,   148,   148,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   150,   150,   150,   150,   150,   150,   150,   150,   151,
     151,   151,   151,   151,   151,   152,   152,   153,   153,   153,
     153,   154,   154,   155,   155,   155,   156,   156,   156,   157,
     158,   158,   158,   158,   158,   158,   159,   159,   160,   160,
     161,   161,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   163,   163,   163,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   166,   166,   167,   168,   169,   170,   170,   170,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   172,   173,   173,   173,
     173,   174,   174,   174,   174,   175,   175,   175,   175,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   178,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   180,   180,   181,
     181,   182,   182,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     184,   184,   185,   185,   185,   186,   186,   187,   187,   187,
     188,   188,   189,   189,   189,   190,   190,   190,   191,   191,
     191,   191,   191,   191,   191,   192,   192,   192,   192,   192,
     192,   193,   193,   194,   194,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   196,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   198,   198,   198,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199
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
       1,     1,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     4,     6,     6,     6,     1,     4,     4,
       5,     5,     5,     5,     5,     5,     1,     1,     1,     1,
       2,     3,     4,     5,     2,     3,     4,     5,     2,     3,
       4,     5,     1,     2
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
#line 3728 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 557 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3734 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 558 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3740 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 562 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3746 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 564 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3752 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 566 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3758 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 568 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3766 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 572 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3772 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 574 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3778 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 576 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3784 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 578 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3790 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 580 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3798 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 584 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3806 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 588 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3812 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 590 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3820 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 594 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3828 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 598 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3834 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 600 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3840 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 602 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3846 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3852 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3858 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 608 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3866 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 612 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3874 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 616 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3880 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 618 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3886 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 620 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3892 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 622 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3898 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 624 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3904 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 626 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3910 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 628 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3916 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 630 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3922 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 632 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3928 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 634 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3934 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 636 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3940 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 638 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3946 "raku.tab.c"
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
#line 3958 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 648 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3964 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 650 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); free((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3970 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 652 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3976 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 654 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); free((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3982 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 656 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3988 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 658 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3994 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 660 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4001 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 663 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4008 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 666 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4014 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 668 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4021 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 671 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4028 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 674 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4034 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 676 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4042 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 680 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4048 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 682 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4054 "raku.tab.c"
    break;

  case 53: /* stmt: KW_FAIL expr ';'  */
#line 684 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4060 "raku.tab.c"
    break;

  case 54: /* stmt: KW_FAIL ';'  */
#line 686 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4066 "raku.tab.c"
    break;

  case 55: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 688 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4073 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 691 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4080 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr ';'  */
#line 694 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4086 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT ';'  */
#line 696 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4093 "raku.tab.c"
    break;

  case 59: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 699 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4100 "raku.tab.c"
    break;

  case 60: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 702 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4107 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 705 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4113 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 707 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4122 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 712 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4130 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 716 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4138 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 720 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4147 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 725 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4155 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall ';'  */
#line 728 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 4161 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 730 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4167 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 732 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4173 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 734 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4179 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 736 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4185 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 738 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4192 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 741 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4198 "raku.tab.c"
    break;

  case 74: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 743 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4204 "raku.tab.c"
    break;

  case 75: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 745 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4210 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 747 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4217 "raku.tab.c"
    break;

  case 77: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 750 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4224 "raku.tab.c"
    break;

  case 78: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 753 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4231 "raku.tab.c"
    break;

  case 79: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 756 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4238 "raku.tab.c"
    break;

  case 80: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 759 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4245 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_IF expr ';'  */
#line 762 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4251 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNLESS expr ';'  */
#line 764 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4257 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WHILE expr ';'  */
#line 766 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4263 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_UNTIL expr ';'  */
#line 768 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4269 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_FOR expr ';'  */
#line 770 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4276 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_WITH expr ';'  */
#line 773 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4282 "raku.tab.c"
    break;

  case 87: /* stmt: expr KW_WITHOUT expr ';'  */
#line 775 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4288 "raku.tab.c"
    break;

  case 88: /* stmt: expr KW_GIVEN expr ';'  */
#line 777 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4294 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 779 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4301 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 782 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4308 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 785 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4316 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 789 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4323 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 792 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4329 "raku.tab.c"
    break;

  case 94: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 794 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4335 "raku.tab.c"
    break;

  case 95: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 796 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4341 "raku.tab.c"
    break;

  case 96: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 798 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4348 "raku.tab.c"
    break;

  case 97: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 801 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4355 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 804 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4362 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 807 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4369 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 810 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4376 "raku.tab.c"
    break;

  case 101: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 813 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4383 "raku.tab.c"
    break;

  case 102: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 816 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4390 "raku.tab.c"
    break;

  case 103: /* stmt: expr ';'  */
#line 818 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4396 "raku.tab.c"
    break;

  case 104: /* stmt: ';'  */
#line 819 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4402 "raku.tab.c"
    break;

  case 105: /* stmt: if_stmt  */
#line 820 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4408 "raku.tab.c"
    break;

  case 106: /* stmt: while_stmt  */
#line 821 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4414 "raku.tab.c"
    break;

  case 107: /* stmt: for_stmt  */
#line 822 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4420 "raku.tab.c"
    break;

  case 108: /* stmt: given_stmt  */
#line 823 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4426 "raku.tab.c"
    break;

  case 109: /* stmt: KW_TRY block  */
#line 825 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4432 "raku.tab.c"
    break;

  case 110: /* stmt: KW_TRY block KW_CATCH block  */
#line 827 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4438 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 829 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4446 "raku.tab.c"
    break;

  case 112: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 833 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4455 "raku.tab.c"
    break;

  case 113: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 838 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4463 "raku.tab.c"
    break;

  case 114: /* stmt: KW_CATCH block  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4469 "raku.tab.c"
    break;

  case 115: /* stmt: block  */
#line 844 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4475 "raku.tab.c"
    break;

  case 116: /* stmt: unless_stmt  */
#line 845 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4481 "raku.tab.c"
    break;

  case 117: /* stmt: until_stmt  */
#line 846 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4487 "raku.tab.c"
    break;

  case 118: /* stmt: repeat_stmt  */
#line 847 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4493 "raku.tab.c"
    break;

  case 119: /* stmt: loop_stmt  */
#line 848 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4499 "raku.tab.c"
    break;

  case 120: /* stmt: KW_LAST ';'  */
#line 849 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4505 "raku.tab.c"
    break;

  case 121: /* stmt: KW_NEXT ';'  */
#line 850 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4511 "raku.tab.c"
    break;

  case 122: /* stmt: KW_LAST KW_IF expr ';'  */
#line 852 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4517 "raku.tab.c"
    break;

  case 123: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 854 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4523 "raku.tab.c"
    break;

  case 124: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 856 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4529 "raku.tab.c"
    break;

  case 125: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 858 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4535 "raku.tab.c"
    break;

  case 126: /* stmt: sub_decl  */
#line 859 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4541 "raku.tab.c"
    break;

  case 127: /* stmt: class_decl  */
#line 860 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4547 "raku.tab.c"
    break;

  case 128: /* stmt: role_decl  */
#line 861 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4553 "raku.tab.c"
    break;

  case 129: /* stmt: grammar_decl  */
#line 862 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4559 "raku.tab.c"
    break;

  case 130: /* stmt: module_decl  */
#line 863 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4565 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF '(' expr ')' block  */
#line 867 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4571 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4577 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 871 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4583 "raku.tab.c"
    break;

  case 134: /* if_stmt: KW_IF expr block  */
#line 873 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4589 "raku.tab.c"
    break;

  case 135: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 875 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4595 "raku.tab.c"
    break;

  case 136: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 877 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4601 "raku.tab.c"
    break;

  case 137: /* if_stmt: KW_IF expr block elsif_tail  */
#line 879 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4607 "raku.tab.c"
    break;

  case 138: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 881 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4613 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF expr block  */
#line 885 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4619 "raku.tab.c"
    break;

  case 140: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 887 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4625 "raku.tab.c"
    break;

  case 141: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 889 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4631 "raku.tab.c"
    break;

  case 142: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 891 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4637 "raku.tab.c"
    break;

  case 143: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 893 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4643 "raku.tab.c"
    break;

  case 144: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 895 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4649 "raku.tab.c"
    break;

  case 145: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 899 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4655 "raku.tab.c"
    break;

  case 146: /* while_stmt: KW_WHILE expr block  */
#line 901 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4661 "raku.tab.c"
    break;

  case 147: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 905 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4667 "raku.tab.c"
    break;

  case 148: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 907 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4673 "raku.tab.c"
    break;

  case 149: /* unless_stmt: KW_UNLESS expr block  */
#line 909 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4679 "raku.tab.c"
    break;

  case 150: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 911 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4685 "raku.tab.c"
    break;

  case 151: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 915 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4691 "raku.tab.c"
    break;

  case 152: /* until_stmt: KW_UNTIL expr block  */
#line 917 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4697 "raku.tab.c"
    break;

  case 153: /* repeat_stmt: KW_REPEAT block  */
#line 921 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4703 "raku.tab.c"
    break;

  case 154: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 923 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4709 "raku.tab.c"
    break;

  case 155: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 925 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4715 "raku.tab.c"
    break;

  case 156: /* loop_stmt: KW_LOOP block  */
#line 929 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4721 "raku.tab.c"
    break;

  case 157: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 931 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4727 "raku.tab.c"
    break;

  case 158: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 933 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4733 "raku.tab.c"
    break;

  case 159: /* loop_incr: expr  */
#line 936 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4739 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 940 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4749 "raku.tab.c"
    break;

  case 161: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 946 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4759 "raku.tab.c"
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
#line 4771 "raku.tab.c"
    break;

  case 163: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 960 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4781 "raku.tab.c"
    break;

  case 164: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 966 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4790 "raku.tab.c"
    break;

  case 165: /* for_stmt: KW_FOR expr block  */
#line 971 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4797 "raku.tab.c"
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
#line 4809 "raku.tab.c"
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
#line 4822 "raku.tab.c"
    break;

  case 168: /* catch_when_list: KW_WHEN expr block  */
#line 995 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,(yyvsp[-1].node)); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 4828 "raku.tab.c"
    break;

  case 169: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 997 "raku.y"
        { exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 4834 "raku.tab.c"
    break;

  case 170: /* when_list: %empty  */
#line 1000 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4840 "raku.tab.c"
    break;

  case 171: /* when_list: when_list KW_WHEN expr block  */
#line 1002 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4848 "raku.tab.c"
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
#line 4860 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1016 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4870 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1022 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4880 "raku.tab.c"
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
#line 4892 "raku.tab.c"
    break;

  case 176: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1036 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4902 "raku.tab.c"
    break;

  case 177: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1042 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4912 "raku.tab.c"
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
#line 4925 "raku.tab.c"
    break;

  case 179: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1057 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4936 "raku.tab.c"
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
#line 4949 "raku.tab.c"
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
#line 4962 "raku.tab.c"
    break;

  case 182: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1082 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4973 "raku.tab.c"
    break;

  case 183: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1091 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4984 "raku.tab.c"
    break;

  case 184: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1098 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4993 "raku.tab.c"
    break;

  case 185: /* scalar_methcall: VAR_SCALAR '.' IDENT  */
#line 1103 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5002 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list '}'  */
#line 1109 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5008 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr '}'  */
#line 1111 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5015 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1114 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5022 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1117 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5028 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1119 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5034 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1121 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5040 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1123 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5046 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1125 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5052 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1127 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5058 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1129 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5064 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1131 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5071 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1134 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5077 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1136 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5083 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1138 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5089 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1140 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5096 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1143 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5103 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1146 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5111 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list '}'  */
#line 1151 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5117 "raku.tab.c"
    break;

  case 204: /* method_body: '{' YADA '}'  */
#line 1152 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5123 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1154 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-4].list); exprlist_append(l,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5130 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr '}'  */
#line 1157 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5137 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1160 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5144 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1163 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5150 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1165 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5156 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1167 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5162 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1169 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5168 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1171 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5174 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1173 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5180 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1175 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5186 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1177 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5193 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1180 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5199 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1182 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5205 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1184 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5211 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1186 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5218 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1189 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5225 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1192 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5233 "raku.tab.c"
    break;

  case 222: /* pkg_name: IDENT  */
#line 1197 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5239 "raku.tab.c"
    break;

  case 223: /* pkg_name: QIDENT  */
#line 1198 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5245 "raku.tab.c"
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
#line 5263 "raku.tab.c"
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
#line 5280 "raku.tab.c"
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
#line 5297 "raku.tab.c"
    break;

  case 227: /* is_clauses: %empty  */
#line 1247 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5303 "raku.tab.c"
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
#line 5319 "raku.tab.c"
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
#line 5335 "raku.tab.c"
    break;

  case 230: /* class_body_list: %empty  */
#line 1274 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5341 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1276 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5348 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1279 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5355 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1282 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5362 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1285 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5369 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1288 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5376 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1291 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5383 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1294 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5390 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1297 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5397 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1300 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5404 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1303 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5411 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1306 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5418 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1309 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5425 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1312 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5436 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1319 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          free((yyvsp[-5].sval)); free((yyvsp[-4].sval)); free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5446 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1325 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          free((yyvsp[-5].sval)); free((yyvsp[-4].sval)); free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5456 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1331 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5467 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1338 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5478 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1345 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5489 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1352 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5497 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1356 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5505 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1360 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5513 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1364 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5521 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1368 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5532 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1375 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5543 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1382 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5554 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1389 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5565 "raku.tab.c"
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
#line 5579 "raku.tab.c"
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
#line 5591 "raku.tab.c"
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
#line 5603 "raku.tab.c"
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
#line 5616 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1431 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5627 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1438 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5638 "raku.tab.c"
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
#line 5652 "raku.tab.c"
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
#line 5664 "raku.tab.c"
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
#line 5676 "raku.tab.c"
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
#line 5693 "raku.tab.c"
    break;

  case 267: /* grammar_body_list: %empty  */
#line 1487 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5699 "raku.tab.c"
    break;

  case 268: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1489 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5708 "raku.tab.c"
    break;

  case 269: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1494 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5717 "raku.tab.c"
    break;

  case 270: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1499 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5726 "raku.tab.c"
    break;

  case 271: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1506 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5734 "raku.tab.c"
    break;

  case 272: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1510 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5742 "raku.tab.c"
    break;

  case 273: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1514 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5750 "raku.tab.c"
    break;

  case 274: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1518 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5758 "raku.tab.c"
    break;

  case 275: /* pair_list: IDENT OP_FATARROW expr  */
#line 1524 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5764 "raku.tab.c"
    break;

  case 276: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1526 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5770 "raku.tab.c"
    break;

  case 277: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1528 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5776 "raku.tab.c"
    break;

  case 278: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1530 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5782 "raku.tab.c"
    break;

  case 279: /* param_list: VAR_SCALAR  */
#line 1533 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5788 "raku.tab.c"
    break;

  case 280: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1534 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5794 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1535 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5800 "raku.tab.c"
    break;

  case 282: /* param_list: VAR_ARRAY  */
#line 1536 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5806 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' VAR_ARRAY  */
#line 1537 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5812 "raku.tab.c"
    break;

  case 284: /* param_list: IDENT VAR_SCALAR  */
#line 1538 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5818 "raku.tab.c"
    break;

  case 285: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1539 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5824 "raku.tab.c"
    break;

  case 286: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1540 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5830 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' VAR_SCALAR  */
#line 1541 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5836 "raku.tab.c"
    break;

  case 288: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1542 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5842 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1543 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5848 "raku.tab.c"
    break;

  case 290: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1544 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5854 "raku.tab.c"
    break;

  case 291: /* param_list: VAR_SCALAR '=' expr  */
#line 1545 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5860 "raku.tab.c"
    break;

  case 292: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1546 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5866 "raku.tab.c"
    break;

  case 293: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1547 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5872 "raku.tab.c"
    break;

  case 294: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1548 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5878 "raku.tab.c"
    break;

  case 295: /* param_list: SLURPY_POS  */
#line 1549 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5884 "raku.tab.c"
    break;

  case 296: /* param_list: param_list ',' SLURPY_POS  */
#line 1550 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5890 "raku.tab.c"
    break;

  case 297: /* param_list: SLURPY_LOL  */
#line 1551 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5896 "raku.tab.c"
    break;

  case 298: /* param_list: param_list ',' SLURPY_LOL  */
#line 1552 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5902 "raku.tab.c"
    break;

  case 299: /* param_list: SLURPY_NAMED  */
#line 1553 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5908 "raku.tab.c"
    break;

  case 300: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1554 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5914 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list '}'  */
#line 1557 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5920 "raku.tab.c"
    break;

  case 302: /* block: '{' YADA '}'  */
#line 1558 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5926 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list expr '}'  */
#line 1560 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5932 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1562 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5938 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1564 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5944 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1566 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5950 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1568 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5956 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1570 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5963 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1573 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5969 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1575 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5975 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1577 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5981 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1579 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5987 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1581 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5994 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1584 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6001 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1587 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6009 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1591 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6016 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1594 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6023 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1597 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6030 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1600 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 6036 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1602 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6044 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1606 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6051 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1609 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6058 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1612 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6065 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1615 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6072 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1618 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6079 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1621 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 6085 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1623 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6091 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1625 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6097 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_LAST '}'  */
#line 1627 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6103 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1629 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6109 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1631 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6115 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1633 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6121 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1635 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6127 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1637 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6133 "raku.tab.c"
    break;

  case 335: /* closure: '{' expr '}'  */
#line 1640 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6139 "raku.tab.c"
    break;

  case 336: /* expr: VAR_SCALAR '=' expr  */
#line 1643 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6145 "raku.tab.c"
    break;

  case 337: /* expr: VAR_ARRAY '=' expr  */
#line 1644 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6151 "raku.tab.c"
    break;

  case 338: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1645 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6157 "raku.tab.c"
    break;

  case 339: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1646 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6163 "raku.tab.c"
    break;

  case 340: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1647 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6169 "raku.tab.c"
    break;

  case 341: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1648 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6175 "raku.tab.c"
    break;

  case 342: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1649 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6181 "raku.tab.c"
    break;

  case 343: /* expr: KW_GATHER block  */
#line 1650 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6191 "raku.tab.c"
    break;

  case 344: /* expr: KW_GATHER for_stmt  */
#line 1655 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6201 "raku.tab.c"
    break;

  case 345: /* expr: tern_expr OP_FATARROW expr  */
#line 1661 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6207 "raku.tab.c"
    break;

  case 346: /* expr: tern_expr  */
#line 1662 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6213 "raku.tab.c"
    break;

  case 347: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1666 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6219 "raku.tab.c"
    break;

  case 348: /* tern_expr: or_expr  */
#line 1667 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6225 "raku.tab.c"
    break;

  case 349: /* or_expr: or_expr OP_OR and_expr  */
#line 1670 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6231 "raku.tab.c"
    break;

  case 350: /* or_expr: and_expr  */
#line 1671 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6237 "raku.tab.c"
    break;

  case 351: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1674 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6243 "raku.tab.c"
    break;

  case 352: /* and_expr: cmp_expr  */
#line 1675 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6249 "raku.tab.c"
    break;

  case 353: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1678 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 6255 "raku.tab.c"
    break;

  case 354: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1679 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 6261 "raku.tab.c"
    break;

  case 355: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1680 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 6267 "raku.tab.c"
    break;

  case 356: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1681 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 6273 "raku.tab.c"
    break;

  case 357: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1682 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 6279 "raku.tab.c"
    break;

  case 358: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1683 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 6285 "raku.tab.c"
    break;

  case 359: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1684 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6291 "raku.tab.c"
    break;

  case 360: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1685 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6297 "raku.tab.c"
    break;

  case 361: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1686 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6303 "raku.tab.c"
    break;

  case 362: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1687 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6309 "raku.tab.c"
    break;

  case 363: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1688 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6315 "raku.tab.c"
    break;

  case 364: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1689 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6321 "raku.tab.c"
    break;

  case 365: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1691 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 6331 "raku.tab.c"
    break;

  case 366: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1697 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 6341 "raku.tab.c"
    break;

  case 367: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1703 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 6351 "raku.tab.c"
    break;

  case 368: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1709 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 6361 "raku.tab.c"
    break;

  case 369: /* cmp_expr: divis_expr  */
#line 1714 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 6367 "raku.tab.c"
    break;

  case 370: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1717 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6373 "raku.tab.c"
    break;

  case 371: /* divis_expr: jct_expr  */
#line 1718 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6379 "raku.tab.c"
    break;

  case 372: /* jct_expr: jct_expr '|' range_expr  */
#line 1721 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6385 "raku.tab.c"
    break;

  case 373: /* jct_expr: jct_expr '&' range_expr  */
#line 1722 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6391 "raku.tab.c"
    break;

  case 374: /* jct_expr: dor_expr  */
#line 1723 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6397 "raku.tab.c"
    break;

  case 375: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1727 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6403 "raku.tab.c"
    break;

  case 376: /* dor_expr: range_expr  */
#line 1728 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6409 "raku.tab.c"
    break;

  case 377: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1731 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6415 "raku.tab.c"
    break;

  case 378: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1732 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6421 "raku.tab.c"
    break;

  case 379: /* range_expr: add_expr  */
#line 1733 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6427 "raku.tab.c"
    break;

  case 380: /* add_expr: add_expr '~' repl_expr  */
#line 1736 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6433 "raku.tab.c"
    break;

  case 381: /* add_expr: repl_expr  */
#line 1737 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6439 "raku.tab.c"
    break;

  case 382: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1740 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6445 "raku.tab.c"
    break;

  case 383: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1741 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6451 "raku.tab.c"
    break;

  case 384: /* repl_expr: addsub_expr  */
#line 1742 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6457 "raku.tab.c"
    break;

  case 385: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1745 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6463 "raku.tab.c"
    break;

  case 386: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1746 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6469 "raku.tab.c"
    break;

  case 387: /* addsub_expr: mul_expr  */
#line 1747 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6475 "raku.tab.c"
    break;

  case 388: /* mul_expr: mul_expr '*' unary_expr  */
#line 1750 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6481 "raku.tab.c"
    break;

  case 389: /* mul_expr: mul_expr '/' unary_expr  */
#line 1751 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6487 "raku.tab.c"
    break;

  case 390: /* mul_expr: mul_expr '%' unary_expr  */
#line 1752 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6493 "raku.tab.c"
    break;

  case 391: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1754 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6499 "raku.tab.c"
    break;

  case 392: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1756 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6505 "raku.tab.c"
    break;

  case 393: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1758 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6511 "raku.tab.c"
    break;

  case 394: /* mul_expr: unary_expr  */
#line 1759 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6517 "raku.tab.c"
    break;

  case 395: /* unary_expr: '-' unary_expr  */
#line 1762 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6523 "raku.tab.c"
    break;

  case 396: /* unary_expr: '+' unary_expr  */
#line 1763 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6529 "raku.tab.c"
    break;

  case 397: /* unary_expr: '!' unary_expr  */
#line 1764 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6535 "raku.tab.c"
    break;

  case 398: /* unary_expr: CARET unary_expr  */
#line 1765 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6541 "raku.tab.c"
    break;

  case 399: /* unary_expr: OP_REDUCE unary_expr  */
#line 1767 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6550 "raku.tab.c"
    break;

  case 400: /* unary_expr: pow_expr  */
#line 1771 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6556 "raku.tab.c"
    break;

  case 401: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1774 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6562 "raku.tab.c"
    break;

  case 402: /* pow_expr: postfix_expr  */
#line 1775 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6568 "raku.tab.c"
    break;

  case 403: /* scalar_list: VAR_SCALAR  */
#line 1778 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6574 "raku.tab.c"
    break;

  case 404: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1779 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6580 "raku.tab.c"
    break;

  case 405: /* meth_name: IDENT  */
#line 1782 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6586 "raku.tab.c"
    break;

  case 406: /* meth_name: KW_SORT  */
#line 1783 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6592 "raku.tab.c"
    break;

  case 407: /* meth_name: KW_REVERSE  */
#line 1784 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6598 "raku.tab.c"
    break;

  case 408: /* meth_name: KW_MAP  */
#line 1785 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6604 "raku.tab.c"
    break;

  case 409: /* meth_name: KW_GREP  */
#line 1786 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6610 "raku.tab.c"
    break;

  case 410: /* meth_name: KW_SAY  */
#line 1787 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6616 "raku.tab.c"
    break;

  case 411: /* meth_name: KW_PRINT  */
#line 1788 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6622 "raku.tab.c"
    break;

  case 412: /* meth_name: KW_TAKE  */
#line 1789 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6628 "raku.tab.c"
    break;

  case 413: /* meth_name: KW_RETURN  */
#line 1790 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6634 "raku.tab.c"
    break;

  case 414: /* meth_name: KW_EXISTS  */
#line 1791 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6640 "raku.tab.c"
    break;

  case 415: /* meth_name: KW_DELETE  */
#line 1792 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6646 "raku.tab.c"
    break;

  case 416: /* meth_name: KW_JOIN  */
#line 1793 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6652 "raku.tab.c"
    break;

  case 417: /* meth_name: TESTOP  */
#line 1794 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6658 "raku.tab.c"
    break;

  case 418: /* postfix_expr: call_expr  */
#line 1796 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6664 "raku.tab.c"
    break;

  case 419: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1799 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6674 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '(' arg_list ')'  */
#line 1805 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6683 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '(' ')'  */
#line 1809 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6689 "raku.tab.c"
    break;

  case 422: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1811 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6695 "raku.tab.c"
    break;

  case 423: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1813 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6701 "raku.tab.c"
    break;

  case 424: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1815 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6710 "raku.tab.c"
    break;

  case 425: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1820 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6716 "raku.tab.c"
    break;

  case 426: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1822 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6726 "raku.tab.c"
    break;

  case 427: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1828 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6736 "raku.tab.c"
    break;

  case 428: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1834 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6744 "raku.tab.c"
    break;

  case 429: /* call_expr: IDENT '.' KW_NEW  */
#line 1838 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6752 "raku.tab.c"
    break;

  case 430: /* call_expr: IDENT '.' IDENT  */
#line 1842 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6761 "raku.tab.c"
    break;

  case 431: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1847 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6772 "raku.tab.c"
    break;

  case 432: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1854 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6781 "raku.tab.c"
    break;

  case 433: /* call_expr: IDENT '.' CARET IDENT  */
#line 1859 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6791 "raku.tab.c"
    break;

  case 434: /* call_expr: atom '.' CARET IDENT  */
#line 1865 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6801 "raku.tab.c"
    break;

  case 435: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1871 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6812 "raku.tab.c"
    break;

  case 436: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1878 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6823 "raku.tab.c"
    break;

  case 437: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1885 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6832 "raku.tab.c"
    break;

  case 438: /* call_expr: atom '.' meth_name  */
#line 1890 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6841 "raku.tab.c"
    break;

  case 439: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1895 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6852 "raku.tab.c"
    break;

  case 440: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1902 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6861 "raku.tab.c"
    break;

  case 441: /* call_expr: call_expr '.' meth_name  */
#line 1907 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6870 "raku.tab.c"
    break;

  case 442: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 1912 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6881 "raku.tab.c"
    break;

  case 443: /* call_expr: '.' meth_name '(' ')'  */
#line 1919 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6890 "raku.tab.c"
    break;

  case 444: /* call_expr: '.' meth_name  */
#line 1924 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6899 "raku.tab.c"
    break;

  case 445: /* call_expr: KW_DIE expr  */
#line 1929 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6905 "raku.tab.c"
    break;

  case 446: /* call_expr: KW_MAP closure expr  */
#line 1931 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6911 "raku.tab.c"
    break;

  case 447: /* call_expr: KW_MAP closure ',' expr  */
#line 1933 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6917 "raku.tab.c"
    break;

  case 448: /* call_expr: KW_GREP closure expr  */
#line 1935 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6923 "raku.tab.c"
    break;

  case 449: /* call_expr: KW_SORT expr  */
#line 1937 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6929 "raku.tab.c"
    break;

  case 450: /* call_expr: KW_SORT closure expr  */
#line 1939 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6935 "raku.tab.c"
    break;

  case 451: /* call_expr: KW_REVERSE expr  */
#line 1941 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6941 "raku.tab.c"
    break;

  case 452: /* call_expr: atom  */
#line 1942 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6947 "raku.tab.c"
    break;

  case 453: /* arg_list: expr  */
#line 1945 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6953 "raku.tab.c"
    break;

  case 454: /* arg_list: arg_list ',' expr  */
#line 1946 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6959 "raku.tab.c"
    break;

  case 455: /* arg_list: arg_list ','  */
#line 1947 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6965 "raku.tab.c"
    break;

  case 456: /* atom: LIT_INT  */
#line 1950 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6971 "raku.tab.c"
    break;

  case 457: /* atom: LIT_FLOAT  */
#line 1951 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6977 "raku.tab.c"
    break;

  case 458: /* atom: LIT_STR  */
#line 1952 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6983 "raku.tab.c"
    break;

  case 459: /* atom: WORDLIST  */
#line 1954 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6996 "raku.tab.c"
    break;

  case 460: /* atom: LIT_INTERP_STR  */
#line 1962 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7002 "raku.tab.c"
    break;

  case 461: /* atom: VAR_SCALAR  */
#line 1963 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7008 "raku.tab.c"
    break;

  case 462: /* atom: OP_INC VAR_SCALAR  */
#line 1964 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7014 "raku.tab.c"
    break;

  case 463: /* atom: OP_DEC VAR_SCALAR  */
#line 1965 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7020 "raku.tab.c"
    break;

  case 464: /* atom: VAR_SCALAR OP_INC  */
#line 1966 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7026 "raku.tab.c"
    break;

  case 465: /* atom: VAR_SCALAR OP_DEC  */
#line 1967 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7032 "raku.tab.c"
    break;

  case 466: /* atom: VAR_TWIGIL OP_INC  */
#line 1968 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); free((yyvsp[-1].sval)); }
#line 7038 "raku.tab.c"
    break;

  case 467: /* atom: VAR_TWIGIL OP_DEC  */
#line 1969 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); free((yyvsp[-1].sval)); }
#line 7044 "raku.tab.c"
    break;

  case 468: /* atom: VAR_ARRAY  */
#line 1970 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7050 "raku.tab.c"
    break;

  case 469: /* atom: VAR_HASH  */
#line 1971 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7056 "raku.tab.c"
    break;

  case 470: /* atom: VAR_CAPTURE  */
#line 1973 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7064 "raku.tab.c"
    break;

  case 471: /* atom: VAR_FH  */
#line 1977 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7072 "raku.tab.c"
    break;

  case 472: /* atom: VAR_NAMED_CAPTURE  */
#line 1981 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 7079 "raku.tab.c"
    break;

  case 473: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1984 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 7085 "raku.tab.c"
    break;

  case 474: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1986 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 7091 "raku.tab.c"
    break;

  case 475: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1988 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 7097 "raku.tab.c"
    break;

  case 476: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1990 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 7103 "raku.tab.c"
    break;

  case 477: /* atom: ARR_ALL_SLICE  */
#line 1992 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 7109 "raku.tab.c"
    break;

  case 478: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1994 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7115 "raku.tab.c"
    break;

  case 479: /* atom: VAR_HASH '{' expr '}'  */
#line 1996 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7121 "raku.tab.c"
    break;

  case 480: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1998 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7127 "raku.tab.c"
    break;

  case 481: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2000 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7133 "raku.tab.c"
    break;

  case 482: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2002 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7139 "raku.tab.c"
    break;

  case 483: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2004 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7145 "raku.tab.c"
    break;

  case 484: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2006 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7151 "raku.tab.c"
    break;

  case 485: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2008 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7157 "raku.tab.c"
    break;

  case 486: /* atom: IDENT  */
#line 2009 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7163 "raku.tab.c"
    break;

  case 487: /* atom: VAR_TWIGIL  */
#line 2011 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7171 "raku.tab.c"
    break;

  case 488: /* atom: VAR_ARRAY_TWIGIL  */
#line 2015 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7179 "raku.tab.c"
    break;

  case 489: /* atom: VAR_HASH_TWIGIL  */
#line 2019 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7187 "raku.tab.c"
    break;

  case 490: /* atom: '[' ']'  */
#line 2022 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 7193 "raku.tab.c"
    break;

  case 491: /* atom: '[' expr ']'  */
#line 2024 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7199 "raku.tab.c"
    break;

  case 492: /* atom: '[' expr ',' ']'  */
#line 2026 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7205 "raku.tab.c"
    break;

  case 493: /* atom: '[' expr ',' arg_list ']'  */
#line 2028 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7212 "raku.tab.c"
    break;

  case 494: /* atom: DOLLAR_LBRACKET ']'  */
#line 2030 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 7218 "raku.tab.c"
    break;

  case 495: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2032 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7224 "raku.tab.c"
    break;

  case 496: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2034 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7230 "raku.tab.c"
    break;

  case 497: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2036 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7237 "raku.tab.c"
    break;

  case 498: /* atom: '(' ')'  */
#line 2038 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7243 "raku.tab.c"
    break;

  case 499: /* atom: '(' expr ')'  */
#line 2039 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7249 "raku.tab.c"
    break;

  case 500: /* atom: '(' expr ',' ')'  */
#line 2041 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7255 "raku.tab.c"
    break;

  case 501: /* atom: '(' expr ',' arg_list ')'  */
#line 2043 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7262 "raku.tab.c"
    break;

  case 502: /* atom: block  */
#line 2045 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7268 "raku.tab.c"
    break;

  case 503: /* atom: KW_SUB block  */
#line 2046 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7274 "raku.tab.c"
    break;


#line 7278 "raku.tab.c"

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

#line 2048 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
