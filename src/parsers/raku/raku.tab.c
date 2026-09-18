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

#include "ct_arena.h"
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
    ExprList *l = ct_zalloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
static ExprList *exprlist_append(ExprList *l, tree_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = ct_grow(l->items, l->cap * sizeof(tree_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
static void exprlist_free(ExprList *l) { if (l) { ct_drop(l->items); ct_drop(l); } }
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
    if (!strcmp(s, "subtest")) return "__rk_test_subtest";
    if (!strcmp(s, "is-deeply")) return "__rk_test_is_deeply";
    if (!strcmp(s, "isa-ok")) return "__rk_test_isa_ok";
    if (!strcmp(s, "does-ok")) return "__rk_test_does_ok";
    if (!strcmp(s, "cmp-ok")) return "__rk_test_cmp_ok";
    if (!strcmp(s, "lives-ok")) return "__rk_test_lives_ok";
    if (!strcmp(s, "dies-ok")) return "__rk_test_dies_ok";
    if (!strcmp(s, "throws-like")) return "__rk_test_throws_like";
    if (!strcmp(s, "eval-lives-ok")) return "__rk_test_eval_lives_ok";
    if (!strcmp(s, "eval-dies-ok")) return "__rk_test_eval_dies_ok";
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
    if (a->count == 1 && a->items[0] && a->items[0]->t == TT_FNC && a->items[0]->v.sval && !strcmp(a->items[0]->v.sval, "__rk_pair")
        && name && !strcmp(name, "__rk_test_subtest") && a->items[0]->n >= 3) {
        tree_t *pr = a->items[0];
        for (int i = 1; i < pr->n; i++) expr_add_child(c, pr->c[i]);
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
static ExprList *rk_group_targets(tree_t *grp) {
    ExprList *t = exprlist_new();
    if (grp && grp->t == TT_FNC && grp->v.sval && strcmp(grp->v.sval, "__rk_arr") == 0) { for (int i = 1; i < grp->n; i++) exprlist_append(t, grp->c[i]); }
    else if (grp) { exprlist_append(t, grp); }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = ct_strdup(e->v.sval);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_adhoc_name(const char *nm) { return nm && !strcmp(nm, "X::AdHoc"); }
static tree_t *rk_adhoc_new(const char *cname, ExprList *named, ExprList *pos) {
    if (!rk_is_adhoc_name(cname)) return NULL;
    tree_t *val = NULL;
    if (named) { for (int i = 0; i + 1 < named->count; i += 2) {
            tree_t *k = named->items[i]; const char *ks = k ? k->v.sval : NULL;
            if (ks && (!strcmp(ks, "payload") || !strcmp(ks, "message"))) { val = named->items[i + 1]; break; } } }
    if (!val && pos && pos->count > 0) val = pos->items[0];
    if (!val) { val = ast_node_new(TT_QLIT); val->v.sval = (char *)intern(""); }
    return val;
}
static tree_t *rk_catch_when_cond(tree_t *e) {
    if (e && e->t == TT_VAR && rk_is_adhoc_name(e->v.sval)) return ast_node_new(TT_NUL);
    return e;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_order_call(const char *fn, tree_t *left, tree_t *right) {
    tree_t *c = ast_node_new(TT_FNC); c->v.sval = (char *)intern(fn);
    ast_push(c, leaf_sval(TT_VAR, fn)); ast_push(c, left); ast_push(c, right); return c;
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

#line 570 "raku.tab.c"

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
  YYSYMBOL_OP_CMP3 = 99,                   /* OP_CMP3  */
  YYSYMBOL_OP_CMPG = 100,                  /* OP_CMPG  */
  YYSYMBOL_OP_LEG = 101,                   /* OP_LEG  */
  YYSYMBOL_OP_AND = 102,                   /* OP_AND  */
  YYSYMBOL_OP_OR = 103,                    /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 104,              /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 105,              /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 106,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 107,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 108,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 109,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 110,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 111,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 112,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 113,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 114,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 115,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 116,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 117,                   /* OP_DIV  */
  YYSYMBOL_ADV_EXISTS = 118,               /* ADV_EXISTS  */
  YYSYMBOL_ADV_DELETE = 119,               /* ADV_DELETE  */
  YYSYMBOL_OP_BAND = 120,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 121,                   /* OP_SHL  */
  YYSYMBOL_OP_GCD = 122,                   /* OP_GCD  */
  YYSYMBOL_OP_LCM = 123,                   /* OP_LCM  */
  YYSYMBOL_OP_MODW = 124,                  /* OP_MODW  */
  YYSYMBOL_OP_NBAND = 125,                 /* OP_NBAND  */
  YYSYMBOL_OP_UMUL = 126,                  /* OP_UMUL  */
  YYSYMBOL_OP_UDIV = 127,                  /* OP_UDIV  */
  YYSYMBOL_OP_BORT = 128,                  /* OP_BORT  */
  YYSYMBOL_OP_NBOR = 129,                  /* OP_NBOR  */
  YYSYMBOL_OP_QBOR = 130,                  /* OP_QBOR  */
  YYSYMBOL_OP_QBXOR = 131,                 /* OP_QBXOR  */
  YYSYMBOL_OP_UMINUS_I = 132,              /* OP_UMINUS_I  */
  YYSYMBOL_OP_COMPOSE = 133,               /* OP_COMPOSE  */
  YYSYMBOL_OP_COMPOSEU = 134,              /* OP_COMPOSEU  */
  YYSYMBOL_OP_SETINT = 135,                /* OP_SETINT  */
  YYSYMBOL_OP_SETMUL = 136,                /* OP_SETMUL  */
  YYSYMBOL_OP_SETUNI = 137,                /* OP_SETUNI  */
  YYSYMBOL_OP_SETSUM = 138,                /* OP_SETSUM  */
  YYSYMBOL_OP_SETDIF = 139,                /* OP_SETDIF  */
  YYSYMBOL_OP_SETSYM = 140,                /* OP_SETSYM  */
  YYSYMBOL_OP_XORJ = 141,                  /* OP_XORJ  */
  YYSYMBOL_OP_RANGE_XL = 142,              /* OP_RANGE_XL  */
  YYSYMBOL_OP_RANGE_XB = 143,              /* OP_RANGE_XB  */
  YYSYMBOL_OP_BUT = 144,                   /* OP_BUT  */
  YYSYMBOL_OP_DOESW = 145,                 /* OP_DOESW  */
  YYSYMBOL_OP_COLL = 146,                  /* OP_COLL  */
  YYSYMBOL_OP_UNICMP = 147,                /* OP_UNICMP  */
  YYSYMBOL_OP_IDENT3 = 148,                /* OP_IDENT3  */
  YYSYMBOL_OP_EQV = 149,                   /* OP_EQV  */
  YYSYMBOL_OP_BEFORE = 150,                /* OP_BEFORE  */
  YYSYMBOL_OP_AFTER = 151,                 /* OP_AFTER  */
  YYSYMBOL_OP_SETCONT = 152,               /* OP_SETCONT  */
  YYSYMBOL_OP_SETELEM = 153,               /* OP_SETELEM  */
  YYSYMBOL_OP_APPROX = 154,                /* OP_APPROX  */
  YYSYMBOL_OP_SMARTM = 155,                /* OP_SMARTM  */
  YYSYMBOL_OP_NSMARTM = 156,               /* OP_NSMARTM  */
  YYSYMBOL_OP_MINOP = 157,                 /* OP_MINOP  */
  YYSYMBOL_OP_MAXOP = 158,                 /* OP_MAXOP  */
  YYSYMBOL_OP_XOROP = 159,                 /* OP_XOROP  */
  YYSYMBOL_OP_DIVIS = 160,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 161,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 162,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 163,                   /* OP_POW  */
  YYSYMBOL_164_ = 164,                     /* '='  */
  YYSYMBOL_165_ = 165,                     /* '!'  */
  YYSYMBOL_166_ = 166,                     /* '<'  */
  YYSYMBOL_167_ = 167,                     /* '>'  */
  YYSYMBOL_168_ = 168,                     /* '|'  */
  YYSYMBOL_169_ = 169,                     /* '&'  */
  YYSYMBOL_170_ = 170,                     /* '~'  */
  YYSYMBOL_171_ = 171,                     /* '+'  */
  YYSYMBOL_172_ = 172,                     /* '-'  */
  YYSYMBOL_173_ = 173,                     /* '*'  */
  YYSYMBOL_174_ = 174,                     /* '/'  */
  YYSYMBOL_175_ = 175,                     /* '%'  */
  YYSYMBOL_UMINUS = 176,                   /* UMINUS  */
  YYSYMBOL_177_ = 177,                     /* '.'  */
  YYSYMBOL_178_ = 178,                     /* ';'  */
  YYSYMBOL_179_ = 179,                     /* '('  */
  YYSYMBOL_180_ = 180,                     /* ')'  */
  YYSYMBOL_181_ = 181,                     /* ','  */
  YYSYMBOL_182_ = 182,                     /* '['  */
  YYSYMBOL_183_ = 183,                     /* ']'  */
  YYSYMBOL_184_ = 184,                     /* '{'  */
  YYSYMBOL_185_ = 185,                     /* '}'  */
  YYSYMBOL_186_ = 186,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 187,                 /* $accept  */
  YYSYMBOL_program = 188,                  /* program  */
  YYSYMBOL_stmt_list = 189,                /* stmt_list  */
  YYSYMBOL_stmt = 190,                     /* stmt  */
  YYSYMBOL_if_stmt = 191,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 192,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 193,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 194,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 195,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 196,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 197,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 198,                /* loop_incr  */
  YYSYMBOL_for_stmt = 199,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 200,               /* given_stmt  */
  YYSYMBOL_catch_when_list = 201,          /* catch_when_list  */
  YYSYMBOL_when_list = 202,                /* when_list  */
  YYSYMBOL_sub_trait_list = 203,           /* sub_trait_list  */
  YYSYMBOL_sub_decl = 204,                 /* sub_decl  */
  YYSYMBOL_sub_body = 205,                 /* sub_body  */
  YYSYMBOL_method_body = 206,              /* method_body  */
  YYSYMBOL_pkg_name = 207,                 /* pkg_name  */
  YYSYMBOL_class_decl = 208,               /* class_decl  */
  YYSYMBOL_role_decl = 209,                /* role_decl  */
  YYSYMBOL_module_decl = 210,              /* module_decl  */
  YYSYMBOL_is_clauses = 211,               /* is_clauses  */
  YYSYMBOL_class_body_list = 212,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 213,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 214,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 215,           /* named_arg_list  */
  YYSYMBOL_pair_list = 216,                /* pair_list  */
  YYSYMBOL_param_list = 217,               /* param_list  */
  YYSYMBOL_block = 218,                    /* block  */
  YYSYMBOL_closure = 219,                  /* closure  */
  YYSYMBOL_expr = 220,                     /* expr  */
  YYSYMBOL_tern_expr = 221,                /* tern_expr  */
  YYSYMBOL_or_expr = 222,                  /* or_expr  */
  YYSYMBOL_and_expr = 223,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 224,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 225,               /* divis_expr  */
  YYSYMBOL_jct_expr = 226,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 227,                 /* dor_expr  */
  YYSYMBOL_range_expr = 228,               /* range_expr  */
  YYSYMBOL_add_expr = 229,                 /* add_expr  */
  YYSYMBOL_repl_expr = 230,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 231,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 232,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 233,               /* unary_expr  */
  YYSYMBOL_pow_expr = 234,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 235,              /* scalar_list  */
  YYSYMBOL_meth_name = 236,                /* meth_name  */
  YYSYMBOL_postfix_expr = 237,             /* postfix_expr  */
  YYSYMBOL_call_expr = 238,                /* call_expr  */
  YYSYMBOL_arg_list = 239,                 /* arg_list  */
  YYSYMBOL_paren_group = 240,              /* paren_group  */
  YYSYMBOL_atom = 241                      /* atom  */
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

/* The parser invokes alloca or the compile-time arena; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include \"ct_arena.h\"
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
       && ! ((defined YYMALLOC) \
             && (defined YYFREE)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC ct_alloc
#   if 0
void *ct_alloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ct_drop
#   if 0
void ct_drop(void *); /* INFRINGES ON USER NAME SPACE */
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
#define YYLAST   6955

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  540
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1322

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   419


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
       2,     2,     2,   165,     2,     2,     2,   175,   169,     2,
     179,   180,   173,   171,   181,   172,   177,   174,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   186,   178,
     166,   164,   167,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   182,     2,   183,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   184,   168,   185,   170,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   176
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   594,   594,   605,   606,   609,   611,   613,   615,   619,
     621,   623,   625,   627,   631,   635,   637,   641,   645,   647,
     649,   651,   653,   655,   659,   663,   665,   667,   669,   671,
     673,   675,   677,   679,   681,   683,   691,   693,   695,   697,
     699,   701,   703,   706,   709,   711,   714,   717,   719,   723,
     725,   727,   729,   731,   734,   737,   739,   742,   745,   748,
     750,   755,   759,   763,   766,   769,   773,   776,   779,   782,
     785,   788,   790,   792,   794,   796,   799,   801,   803,   805,
     808,   811,   815,   818,   820,   822,   824,   827,   830,   833,
     836,   839,   842,   845,   846,   847,   848,   849,   850,   851,
     853,   855,   859,   864,   868,   870,   872,   873,   874,   875,
     876,   877,   878,   880,   882,   884,   886,   887,   888,   889,
     890,   893,   895,   897,   899,   901,   903,   905,   907,   911,
     913,   915,   917,   919,   921,   925,   927,   931,   933,   935,
     937,   941,   943,   947,   949,   951,   955,   957,   959,   963,
     966,   972,   978,   986,   992,   997,  1002,  1010,  1021,  1023,
    1027,  1028,  1034,  1035,  1038,  1046,  1052,  1058,  1066,  1072,
    1080,  1086,  1092,  1101,  1108,  1117,  1126,  1135,  1136,  1139,
    1142,  1144,  1146,  1148,  1150,  1152,  1154,  1156,  1159,  1161,
    1163,  1165,  1168,  1171,  1177,  1178,  1179,  1182,  1185,  1188,
    1190,  1192,  1194,  1196,  1198,  1200,  1202,  1205,  1207,  1209,
    1211,  1214,  1217,  1223,  1226,  1242,  1257,  1272,  1273,  1285,
    1299,  1300,  1303,  1306,  1309,  1312,  1315,  1318,  1321,  1324,
    1327,  1330,  1333,  1336,  1343,  1349,  1355,  1362,  1369,  1376,
    1380,  1384,  1388,  1392,  1399,  1406,  1413,  1420,  1430,  1438,
    1446,  1455,  1462,  1469,  1479,  1487,  1495,  1504,  1513,  1528,
    1529,  1534,  1539,  1546,  1550,  1554,  1561,  1565,  1571,  1573,
    1575,  1577,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1589,  1590,  1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,
    1599,  1600,  1601,  1602,  1603,  1604,  1607,  1608,  1609,  1611,
    1613,  1615,  1617,  1619,  1622,  1624,  1626,  1628,  1630,  1633,
    1636,  1640,  1643,  1646,  1649,  1651,  1654,  1657,  1660,  1663,
    1666,  1668,  1670,  1672,  1674,  1676,  1678,  1680,  1682,  1686,
    1689,  1690,  1691,  1692,  1693,  1694,  1695,  1696,  1701,  1706,
    1708,  1711,  1713,  1716,  1717,  1718,  1719,  1720,  1723,  1724,
    1727,  1728,  1729,  1730,  1731,  1732,  1733,  1734,  1735,  1736,
    1737,  1738,  1739,  1740,  1741,  1742,  1743,  1744,  1745,  1746,
    1747,  1748,  1749,  1750,  1756,  1762,  1768,  1774,  1777,  1778,
    1781,  1782,  1783,  1784,  1785,  1786,  1787,  1788,  1789,  1792,
    1794,  1797,  1798,  1799,  1800,  1801,  1802,  1803,  1806,  1807,
    1808,  1809,  1812,  1813,  1814,  1817,  1818,  1819,  1820,  1821,
    1822,  1823,  1824,  1827,  1828,  1829,  1830,  1831,  1832,  1833,
    1834,  1835,  1837,  1839,  1841,  1843,  1846,  1847,  1848,  1849,
    1850,  1855,  1858,  1859,  1862,  1863,  1866,  1867,  1868,  1869,
    1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,  1878,  1880,
    1882,  1888,  1893,  1894,  1896,  1898,  1903,  1905,  1914,  1923,
    1930,  1934,  1939,  1946,  1951,  1957,  1963,  1970,  1977,  1982,
    1987,  1994,  1999,  2004,  2011,  2018,  2025,  2030,  2035,  2037,
    2039,  2041,  2043,  2045,  2047,  2049,  2052,  2053,  2054,  2057,
    2058,  2059,  2061,  2066,  2067,  2071,  2072,  2073,  2082,  2083,
    2084,  2085,  2086,  2087,  2088,  2089,  2090,  2091,  2092,  2096,
    2100,  2103,  2105,  2107,  2109,  2111,  2113,  2115,  2117,  2119,
    2121,  2123,  2125,  2127,  2129,  2130,  2134,  2138,  2142,  2143,
    2145,  2147,  2150,  2151,  2153,  2155,  2158,  2159,  2160,  2161,
    2164
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
  "VAR_TWIGIL", "IDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL", "CARET",
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
  "OP_SGE", "OP_CMP3", "OP_CMPG", "OP_LEG", "OP_AND", "OP_OR",
  "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH",
  "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ",
  "OP_CAT_EQ", "OP_DOR", "OP_DIV", "ADV_EXISTS", "ADV_DELETE", "OP_BAND",
  "OP_SHL", "OP_GCD", "OP_LCM", "OP_MODW", "OP_NBAND", "OP_UMUL",
  "OP_UDIV", "OP_BORT", "OP_NBOR", "OP_QBOR", "OP_QBXOR", "OP_UMINUS_I",
  "OP_COMPOSE", "OP_COMPOSEU", "OP_SETINT", "OP_SETMUL", "OP_SETUNI",
  "OP_SETSUM", "OP_SETDIF", "OP_SETSYM", "OP_XORJ", "OP_RANGE_XL",
  "OP_RANGE_XB", "OP_BUT", "OP_DOESW", "OP_COLL", "OP_UNICMP", "OP_IDENT3",
  "OP_EQV", "OP_BEFORE", "OP_AFTER", "OP_SETCONT", "OP_SETELEM",
  "OP_APPROX", "OP_SMARTM", "OP_NSMARTM", "OP_MINOP", "OP_MAXOP",
  "OP_XOROP", "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'",
  "':'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "elsif_tail", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "loop_stmt", "loop_incr", "for_stmt", "given_stmt", "catch_when_list",
  "when_list", "sub_trait_list", "sub_decl", "sub_body", "method_body",
  "pkg_name", "class_decl", "role_decl", "module_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "or_expr", "and_expr", "cmp_expr", "divis_expr", "jct_expr", "dor_expr",
  "range_expr", "add_expr", "repl_expr", "addsub_expr", "mul_expr",
  "unary_expr", "pow_expr", "scalar_list", "meth_name", "postfix_expr",
  "call_expr", "arg_list", "paren_group", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-976)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-106)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -976,    64,  2943,  -976,  -976,  -976,  -976,  -976,  -976,   590,
     -21,   141,   -19,    36,  -976,  -976,  6771,  3029,  -976,  -976,
    -976,    65,  3163,    43,  5365,  5499,  5577,  5711,  5789,    -1,
     -11,  5789,  3241,  3375,   117,   140,  5789,  5789,    39,    88,
    5923,  6001,   -79,   -57,    86,   313,     7,     7,  6135,  5789,
     -79,    32,  5789,  3453,   148,   148,   296,  6771,  -976,  -976,
     148,   148,   207,   283,   308,  6771,  6771,  6771,  1082,  -976,
    3587,  3665,   182,  -976,  -976,  -976,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  -976,  -976,  -976,  -976,  1292,   537,   192,
     484,   246,   742,   781,   673,   205,   859,   266,   319,   633,
     793,  -976,  -976,   234,   191,   276,   295,   474,  -976,  -976,
    5789,  5789,  5789,  5789,  5789,  5789,  3799,  6213,  3877,   482,
    5789,  -976,  -976,  5789,   372,    52,  1432,   -22,   454,   240,
     536,    29,   136,  -976,  -976,   414,  -976,   478,   676,   304,
    -976,   335,  4011,  -976,  4089,  -976,   514,  -107,  -104,   121,
     734,   541,   713,   207,  3587,   638,  3587,   169,  3587,   -79,
    3587,   -79,   289,   409,   269,   467,  -976,  -976,  -976,   648,
    -976,   368,  -976,   381,   520,   542,   649,   577,   584,   383,
     386,  3587,   -79,  3587,   -79,   579,  4224,  -976,  5789,  5789,
    -976,  5789,  5789,  -976,  5789,  4302,  5789,  4436,  5789,  -976,
    -976,   758,   294,  -976,  -976,  -976,   669,  -976,  -976,   701,
     672,    67,  -976,   727,   746,  -976,   716,  -976,  -976,  -976,
    -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  -976,  -976,   685,  -976,   391,  -976,   495,
     751,  1913,  5789,  5789,  5789,  5789,  5789,  5789,  5789,  5789,
    -976,  5789,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,
    6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,
    6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,
    6771,   644,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,
    6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,
    6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,
    6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,  6771,
    6771,  6771,  6771,  1082,  5789,   998,   485,   767,   783,   804,
     825,   831,   837,  -976,   513,  3587,   802,   700,   703,   913,
     918,   907,  -976,   919,  1092,   936,   -77,  -976,  1097,   745,
     850,  3877,  1105,  5789,  1082,   998,  5789,  5789,  5789,  5789,
    5789,  5789,  5789,  4514,  -976,  -976,   947,   962,   391,   894,
    -976,  5789,  5789,  -976,  6347,  -976,  6425,  -976,   273,   324,
     397,   465,  1014,  1041,   943,   950,  5789,  5789,  5789,  5789,
    5789,  5789,  5789,  -976,  5789,   952,  5789,  5789,  -976,  5789,
     954,   930,   963,  -976,   207,  5789,  -976,  6771,  6771,     5,
    -976,  -976,    37,  -976,   218,  -976,  -976,  -976,   965,  -976,
    5789,  5789,  5789,  -976,  5789,  5789,  -976,  5789,  5789,  1055,
    5789,  -976,  1153,  5789,  1208,  5789,   967,  1206,   969,  -976,
    5789,  5789,  1228,  1067,  1068,  1069,  1070,  1071,  1065,  5789,
    -976,  -976,  -976,   -79,  5789,   -79,    50,  -976,     4,  -976,
      25,  1072,  1073,  -976,  -976,  1242,  -976,  4648,  -976,  4726,
    4861,  -976,  -976,   453,   455,   282,  5365,  5499,  5789,  2548,
     250,   264,  -976,   256,  1077,  1078,  1080,  1081,  1083,  1093,
    1094,  1095,  1096,  1098,  -976,   246,  1151,   246,   246,   246,
     742,  1100,  1100,  1100,  1100,  1100,  1100,  1100,  1100,  1100,
    1100,  1100,  1100,  1100,  1100,  1100,  1100,  1100,  1100,  1100,
    1100,  1100,  1100,  1100,  -976,  -976,  -976,  -976,   673,   859,
     859,   859,   859,   859,   859,   859,   859,   859,   456,   456,
     456,   456,   456,   456,   319,   319,   319,   633,   633,   793,
     793,   793,   793,   793,   793,   793,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,   -35,
    1099,  1255,   358,  -976,  4939,  -976,  -976,  -976,  -976,  -976,
    -976,  -976,   971,  -976,  5789,  5789,  5789,  5789,  1111,   336,
     341,  -976,  5074,  -976,  1510,  5789,  1101,  -976,    -6,  -976,
    1729,   718,  1114,  1104,    35,   197,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  -976,   840,  -976,  -976,  1106,  -976,  1107,
    3587,   803,  1197,   253,  5152,   833,  1112,  5789,  -976,  6559,
    -976,  5789,  -976,    51,  -976,  5789,  5789,  1119,  4726,  1113,
    1115,  1122,  1123,  1124,  1143,  1144,   839,  4726,  1156,  1157,
     854,   -79,   -10,  6637,  -976,   -79,   716,   333,   -39,   375,
     -14,   973,  2096,  1263,  5789,    59,  1325,  1326,   -79,   702,
     865,  1177,  1178,  1179,  1182,  1183,  1186,  -976,  1184,    77,
    1199,  1188,  1200,  1189,   -79,   -79,   -79,  1191,  1201,  1207,
    5789,  -976,  -976,  -976,  -976,  -976,  -976,  -976,   -79,  1190,
    5789,   -79,  -976,  1355,  1362,  -976,   219,  1194,   975,    74,
     467,   208,  2279,  -976,  -976,   977,  -976,   979,  -976,   866,
    3877,  5789,  5789,   334,    78,   480,  -976,   265,  5789,  5789,
    -976,  5789,  5789,  -976,  5789,  5789,  5789,  5789,  5789,  5789,
    5789,  5789,  -976,  1082,   998,  -976,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  6771,  5789,  5287,  5789,  -976,  -976,  5789,
    1651,  5789,  1203,   981,  4726,   879,  1202,  1204,   882,  5789,
    -976,  -976,  5789,  -976,  -976,  5789,  -976,   983,  -976,   986,
     990,  -976,  5789,  1297,  1368,   992,  -976,  1056,  1058,  -976,
    -976,  -976,  1000,  -976,  5789,  5789,  5789,  1002,  -976,   124,
    -976,  1210,  3587,   895,  1212,  1194,  1004,  1213,  1214,  5789,
    1006,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  1008,
    -976,  -976,  -976,  1165,  -976,  -976,  3587,   -79,  -976,  -976,
    1375,  -976,  1385,  1386,  1369,    20,  -976,   -14,  5365,  5499,
    2682,  -976,   462,  -976,  -976,  1383,  5789,  -976,  -976,  -976,
      72,  -976,   249,  -976,  -976,  -976,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  5789,   -79,  -976,  -976,  -976,  1221,  1222,
    1374,  -976,  -976,  -976,  -976,  5789,  1226,  -976,  -976,   -79,
    1220,  -976,  -976,   300,   897,  1025,  1343,  -976,  -976,  1194,
    1194,  1016,  1019,  1392,  1393,  1394,  -976,  -976,  -976,  -976,
    -976,   903,  1225,   225,  5789,  5789,  5789,  5789,  5789,  5789,
    -976,  -976,  -976,  -976,   416,   426,   472,   490,   496,   511,
     531,   565,   566,   570,   581,   593,   394,   451,  -976,  1233,
    -976,  1021,  1184,  1234,  -976,  1023,  1026,  1184,  -976,  1235,
    1028,  -976,  -976,  -976,  -976,  1236,  1237,  1238,  -976,  -976,
    -976,  1239,  5789,  1241,  -976,  4726,   900,  -976,  -976,  1240,
    1332,  1337,  -976,  1030,  -976,  5789,  -976,  -976,  1194,  -976,
    -976,   901,  1245,  1246,   -10,  -976,  1032,  1185,   -79,   -79,
     -79,  -976,  1411,  -976,    20,  -976,   370,   154,  -976,   286,
    5789,  5789,  5789,  5789,  5789,  5789,  5789,  5789,  -976,  -976,
    -976,  1412,  5789,   109,  1417,  1418,   -79,  1247,  -976,  -976,
     -79,  1252,  5789,  -976,  -976,  -976,   528,   622,    60,   145,
    1045,  1253,  1256,   972,  -976,  -976,  1194,  1194,  1425,  1433,
    1434,  1276,   402,  -976,   594,   599,   614,   617,   618,   636,
    -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,
    -976,  -976,  5789,  5789,  -976,  -976,  -976,  -976,  -976,  -976,
    1277,  -976,  -976,  -976,  -976,  -976,  5789,  1036,  -976,  -976,
    5789,  5789,  4726,   924,  -976,  -976,  5789,  -976,  -976,  -976,
    -976,   -79,   -79,  -976,  -976,  -976,  -976,  -976,  -976,  5789,
    5789,  5789,  -976,  -976,  -976,   637,   640,   642,   645,   650,
     661,   665,   667,  -976,  -976,  1441,  5789,  -976,  -976,  -976,
    -976,  -976,  5789,  1278,  -976,    92,  1380,  -976,   126,  -976,
    1445,  1446,  1284,  5789,  -976,  1452,  1453,  1295,  5789,  -976,
     152,   153,  1301,  1302,  -976,  -976,  1296,   704,  -976,  -976,
    -976,  -976,  -976,  5789,  5789,  -976,  -976,  -976,  -976,  -976,
    -976,   678,   680,  -976,  1303,  1304,  -976,  -976,  1039,  -976,
     933,  1192,  -976,   681,   682,   684,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  -976,  -976,  -976,  1306,   -79,  1307,  1047,
    1300,  2462,  1307,  1049,  1312,  1315,  -976,  1316,   436,   460,
    -976,  1317,  1466,  1483,  1321,  5789,  -976,  1485,  1486,  1327,
    5789,  -976,  -976,  -976,   138,   160,  -976,   744,   749,  -976,
    -976,  -976,  -976,  1328,  -976,   -79,  -976,  -976,  -976,  -976,
    5789,  -976,  -976,  1307,  -976,   374,  5365,  5499,  2760,  -976,
     564,  -976,  1307,  -976,  -976,  -976,  5789,  -976,  5789,  -976,
    -976,  1329,  1331,  -976,  1333,  1334,  1340,  -976,  1341,  1307,
    1051,  1307,  1057,  -976,  -976,  -976,  -976,  1324,  -976,  5789,
     575,   227,  -976,   379,  5789,  5789,  5789,  5789,  5789,  5789,
    5789,  5789,  -976,  -976,  1356,  1357,  -976,  -976,  -976,  -976,
    -976,  -976,  -976,  1307,  -976,  1307,   -79,   750,  5789,  5789,
    5789,  -976,  -976,  -976,   753,   761,   762,   764,   770,   778,
     779,   780,  -976,  -976,  -976,  -976,  -976,  -976,   800,   801,
     819,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,  -976,
    -976,  -976
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   493,   494,   495,   496,   498,   499,
     506,   507,   525,   524,   526,   527,     0,     0,   508,   509,
     510,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   515,   497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     3,     4,    95,    96,   106,   107,   108,   109,
      97,    98,   116,   117,   118,   120,   119,   537,     0,   340,
     342,   347,   349,   377,   379,   388,   390,   397,   401,   404,
     412,   425,   431,   433,   449,   536,   485,     0,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   504,   505,     0,     0,     0,     0,   499,   506,   507,
     525,   524,     0,   537,   429,   449,   536,   485,   499,   506,
     532,     0,     0,    36,     0,   486,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,     0,     0,   538,   338,   337,     0,
      50,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,   146,     0,     0,
     110,     0,     0,   111,     0,     0,     0,     3,     0,   482,
     484,    99,     3,   104,   478,    52,     0,   213,   217,     0,
       0,     0,   430,     0,     0,   434,     0,   500,   501,   428,
     427,   426,   436,   448,   441,   442,   443,   444,   447,   445,
     446,   439,   440,   437,   438,   477,   489,     0,   528,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,   333,   334,
     335,   336,   330,   456,     0,     0,   331,     0,     0,     0,
       0,     0,    40,   461,     0,   460,   524,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   533,    29,     0,   489,     0,     0,
      39,   488,     0,     6,     0,    10,     0,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,    44,     0,
       0,   124,     0,   136,     0,     0,   155,     0,     0,     0,
       3,   166,   272,   277,     0,   290,   292,   294,     0,    47,
       0,     0,     0,    49,     0,     0,    55,     0,     0,     0,
       0,   160,     0,     0,     0,     0,     0,   139,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     479,   481,   483,     0,     0,     0,     0,    51,     0,   220,
       0,     0,     0,   259,     3,     0,   540,     0,   490,     0,
       0,   529,   297,   506,   507,   525,     0,     0,     0,     0,
       0,     0,   296,     0,   449,   485,     0,     0,     0,     0,
       0,     0,     0,     0,   339,   343,     0,   346,   345,   344,
     348,   350,   359,   362,   363,   358,   357,   356,   355,   354,
     353,   352,   351,   360,   361,   364,   368,   369,   370,   371,
     372,   365,   366,   367,   373,   374,   375,   376,   378,   386,
     385,   384,   383,   382,   381,   380,   387,   389,   395,   394,
     393,   392,   391,   396,   400,   399,   398,   402,   403,   410,
     409,   408,   407,   406,   405,   411,   421,   423,   424,   422,
     418,   417,   416,   415,   414,   413,   419,   420,   432,   472,
       0,     0,   469,    62,     0,    88,    89,    90,    91,    92,
      59,   455,     0,    15,     0,     0,     0,     0,   511,   516,
     517,    65,     0,   464,     0,     0,   265,   453,     0,   451,
     488,     0,     0,     0,   472,   469,   332,   333,   334,   335,
     336,   330,   331,   534,     0,    30,    38,     0,   487,     0,
       0,     0,   496,   524,     0,     0,     0,     0,    26,     0,
      27,     0,    28,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,     0,     0,   127,   490,     0,     0,   391,   396,
       0,     0,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,   450,     0,
       0,     0,     0,     0,   490,     0,   490,     0,     0,     0,
       0,   112,   113,   114,   115,   329,   480,   100,     0,     0,
       0,     0,   101,     0,     0,   220,     0,     0,     0,     0,
       0,     0,     0,   435,   476,     0,   491,     0,   530,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,    71,    73,    75,    78,    76,
      77,    72,    74,     0,     0,     0,     0,     9,   465,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   523,     0,   520,   522,     0,   463,     0,   459,     0,
       0,   263,     0,     0,     0,     0,   511,   516,   517,   535,
      37,     5,     0,    12,     0,     0,     0,     0,    19,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    82,    81,    85,    83,    84,    80,    42,     0,
      86,    87,    45,   121,   126,   125,     0,     0,   135,   152,
       0,   154,     0,     0,     0,     0,   165,     0,     0,     0,
       0,   177,     0,   273,   286,     0,     0,   280,   281,   539,
     282,   278,     0,   291,   293,   295,    48,    53,    54,    57,
      58,    32,    31,     0,     0,   156,   518,   519,     0,     0,
     137,   140,   141,   144,   145,     0,     0,   158,   103,     0,
       0,   218,   219,     0,     0,     0,     0,   215,   176,     0,
       0,     0,     0,     0,     0,     0,   258,   216,   475,   492,
     531,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   314,   320,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   472,   469,   341,     0,
     471,     0,   474,     0,   468,     0,     0,   473,    61,     0,
       0,    16,   514,   513,   512,     0,     0,     0,   462,   457,
     458,     0,     0,     0,   454,     0,     0,   269,   268,     0,
       0,     0,    21,     0,    22,     0,    25,   170,     0,    34,
      33,     0,   492,   492,     0,   128,     0,   129,     0,     0,
       0,   162,     0,   168,     0,   164,     0,     0,   180,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,   275,
     288,     0,     0,   283,     0,     0,     0,     0,    69,    70,
       0,     0,     0,   159,   102,   214,     0,     0,     0,     0,
       0,     0,     0,     0,   175,   173,     0,     0,     0,     0,
       0,   511,   517,   317,     0,     0,     0,     0,     0,     0,
     325,   326,   327,   328,   299,   301,   303,   306,   304,   305,
     300,   302,     0,     0,    63,   470,    64,   467,   466,    60,
     492,    66,    67,    68,   264,   266,     0,     0,    13,    20,
       0,     0,     0,     0,   169,     7,     0,    43,    46,   123,
     122,   490,     0,   133,   153,   150,   151,   163,   167,     0,
       0,     0,   181,   182,   179,     0,     0,     0,     0,     0,
       0,     0,     0,   274,   287,     0,     0,   284,   285,   161,
     157,   138,     0,     0,   149,     0,     3,   252,     0,   249,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   221,
       0,     0,     0,     0,   222,   223,     0,     0,   172,   174,
     260,   261,   262,     0,     0,   308,   310,   313,   311,   312,
     309,     0,     0,    17,     0,   492,   271,   270,     0,    23,
       0,   130,   131,     0,     0,     0,   183,   185,   187,   190,
     188,   189,   184,   186,   276,   289,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,     0,     0,
     240,     0,     0,     0,     0,     0,   228,     0,     0,     0,
       0,   227,   224,   225,     0,     0,   255,     0,     0,   315,
     316,   267,    14,   492,     8,     0,   134,   191,   193,   192,
       0,   148,   251,     0,   195,   525,     0,     0,     0,   194,
       0,   248,     0,   237,   238,   230,     0,   233,     0,   236,
     229,     0,     0,   241,     0,     0,     0,   242,     0,     0,
       0,     0,     0,   318,   319,    24,   132,     0,   250,     0,
       0,     0,   199,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   247,     0,     0,   245,   246,   232,   243,
     244,   231,   257,     0,   254,     0,     0,     0,     0,     0,
       0,   200,   201,   198,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   235,   256,   253,   147,   196,     0,     0,
       0,   202,   204,   206,   209,   207,   208,   203,   205,   210,
     212,   211
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -976,  -976,     3,  -976,  -646,  -818,  -976,  -976,  -976,  -976,
    -976,   290,  1503,  -976,  -976,  -976,   699,  -976,  -336,  -975,
     -28,  -976,  -976,  -976,  -976,   832,  -976,  -976,  -535,   914,
    -407,   415,  1193,    -2,  -249,  -976,    90,  1282,  1373,  1258,
    -976,   686,   -27,   483,   942,   787,   214,  -976,  -146,  -282,
    -976,     9,    93,    10,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   241,    73,    74,   654,    75,    76,    77,    78,
      79,  1113,    80,    81,   456,   679,   835,    82,   411,  1117,
     208,    83,    84,    85,   458,   706,    86,   711,   349,   625,
     418,   133,   195,   145,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   216,   235,
     103,   135,   146,   136,   137
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      88,   163,   661,     2,   496,   975,   824,   384,   595,   783,
     834,   104,   105,   106,   164,   141,   412,   413,    26,   703,
     414,    28,   155,   157,   159,   161,   162,   209,   704,   169,
     171,   173,   213,   214,   177,   178,   412,   413,   182,   184,
     414,   569,  1119,   572,   982,   634,   199,   200,   124,   832,
     204,   206,   179,   708,   147,   148,   149,   372,   150,   779,
     374,   663,   412,   413,     3,   785,   414,   343,   237,   239,
     344,   373,   604,   605,   375,  1120,   151,   415,   416,   417,
     142,   152,   461,   845,  1121,   412,   413,   108,   109,   414,
     121,   122,   700,   701,   298,   299,  1001,   415,   416,   417,
     125,   180,   126,   412,   413,    72,   396,   414,   327,   328,
     329,   330,   331,   332,   188,   336,   338,   345,   340,   863,
     864,   341,   186,   415,   416,   417,   397,    72,   174,   754,
     960,   300,   175,  1105,   189,  1122,   462,   412,   413,   961,
     366,   414,   368,   117,   755,   123,   415,   416,   417,   412,
     413,   756,   385,   414,   395,   176,   400,   116,   402,  1083,
    1125,   118,  1206,   207,   415,   416,   417,  1192,  1197,  1126,
     410,   412,   413,    72,    72,   414,  1193,  1198,   165,   436,
     784,   438,   396,    72,   443,   660,   444,   445,   705,   446,
     447,   194,   448,   450,   451,   448,   452,   396,   415,   416,
     417,   664,   397,  1222,   410,   707,   125,  1231,   126,   334,
     415,   416,   417,   125,   755,   126,   202,   397,   215,   350,
    1127,   756,   153,   846,  1123,   935,   806,  1194,  1199,   665,
     134,   805,   415,   416,   417,   702,  1002,   369,  1124,   483,
     486,   487,   488,   489,   490,   491,   492,   493,  1258,   494,
     484,   105,   485,   527,   890,   396,   398,  1273,   656,   399,
    1003,   240,   865,   911,   190,   538,   539,   540,   541,   542,
     543,   212,  1178,  1106,  1282,   397,  1284,   251,   728,   219,
     220,   221,   884,   885,   734,   376,   886,   735,   736,   893,
     894,   895,   731,   421,   217,   666,   667,   737,   729,   377,
     738,   739,   891,   892,   740,   741,  1182,   119,  1304,  1128,
    1305,   210,   732,   422,   421,   165,  1195,  1200,  1249,   218,
      72,   291,   570,  1129,   836,   120,  1004,  1005,  1079,   211,
    1196,  1201,   398,   582,   422,   399,   454,   455,   796,  1093,
    1251,   191,   495,  1216,   497,   498,   499,   398,   257,   601,
     399,   603,   296,   297,   606,   607,   608,   609,   610,   611,
     612,   192,   904,   884,   885,   387,   905,   886,   323,   618,
     619,   888,   621,   240,   626,   906,   760,   404,   907,   908,
     658,   659,   909,   761,   639,   640,   641,   642,   643,   644,
     645,   121,   122,   896,   648,   649,   421,   322,  1089,   298,
     299,   387,  1090,   591,   887,   398,   352,   421,   399,   424,
    1033,   389,  1292,   662,   390,   391,   422,    87,  1091,   671,
     672,   830,   673,   674,   353,   675,   676,   422,   190,   425,
     125,   681,   126,   683,   250,   730,   300,   627,   687,   688,
     324,   742,   193,   423,   166,   168,   722,   696,   409,   733,
     913,   628,   698,   410,   770,   771,   614,   185,   187,   773,
     774,   926,   927,   833,   423,   201,   203,   712,   362,   967,
     405,  1094,   325,    72,   723,   724,   725,   727,   412,   413,
     301,   302,   414,   121,   122,  1015,   351,   646,   629,   326,
     990,   193,   650,   991,   992,   407,   408,   339,   657,   983,
     772,   985,   630,   993,   928,   775,   994,   995,   298,   299,
     996,   997,   393,   670,   371,   394,   363,    72,   364,   910,
     773,   774,   759,   678,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   760,  1259,   415,
     416,   417,   298,   299,   761,   300,   423,   166,   393,   432,
     342,   394,   434,  1024,  1025,  1092,   381,   423,  1052,   426,
     715,   631,   717,   719,  1293,   242,  1144,   433,   243,   244,
     435,   468,   469,   755,   401,   632,   403,   406,   245,   300,
     756,   246,   247,   766,   767,   248,   249,   252,   253,   298,
     299,   354,  1264,   781,   691,  1265,  1266,   437,   618,   439,
    1236,  1040,  1017,  1288,   692,  1267,   387,  1289,  1268,  1269,
     440,  1041,  1270,  1271,  1237,  1053,   389,   117,   792,   390,
     391,   119,   237,  1290,  1238,   801,   300,   803,   441,   804,
     760,   466,  1074,   807,   808,   720,   351,   761,  1239,   721,
     250,   254,   255,   256,   633,   121,   122,   998,  1088,   410,
     693,   827,   524,   525,   526,   355,    87,  1042,   419,   207,
     842,   420,   844,   573,   574,   912,   386,   763,   694,   387,
     388,   104,   105,   106,   745,  1043,   470,   765,   471,   389,
     768,  1044,   390,   391,   427,   777,   392,   780,   876,   746,
    1138,  1139,   370,   581,   371,   371,  1045,   107,   879,   108,
     109,   110,   111,   112,   113,   114,   428,  1115,  1179,   747,
      88,  1183,  1116,   850,   851,   250,  1046,   852,   901,   902,
     903,   104,   105,   106,   382,   429,   914,   915,   383,   916,
     917,   810,   918,   919,   920,   921,   922,   923,   924,   925,
     819,  1137,   250,   748,   749,   378,   379,   380,   750,  1272,
    1047,  1048,   929,   393,   115,  1049,   394,   933,   430,   751,
    1291,   303,   304,   305,   306,   307,  1050,   945,   431,   116,
     946,   752,   811,   947,   853,   854,   855,   813,  1051,  1145,
     951,   544,   545,   546,  1146,   108,   109,   356,   357,   358,
     359,   360,   814,   957,   958,   815,   816,  1250,  1252,  1147,
     963,  1118,  1148,  1149,   308,   309,  1116,   971,   283,   284,
     285,   286,   287,   288,   817,   745,   393,   453,   746,   394,
     747,  1150,  1166,   748,   976,  1167,   419,  1168,   749,   420,
    1169,   258,   259,   260,   261,  1170,   986,   987,   989,   750,
     361,   289,   290,   751,  1000,   752,  1171,   457,   931,   932,
    1172,   460,  1173,   936,   937,   116,  1054,   940,  1056,   811,
     813,  1006,   817,  1209,   467,  1210,  1217,  1218,   697,  1219,
     699,   585,   586,  1011,   272,   273,   274,   275,   276,   277,
     278,   279,   280,  1205,   587,   459,   588,   956,  1116,   281,
     262,   263,   264,   265,   266,   267,   268,   465,   269,   587,
      72,   786,  1034,  1035,  1036,  1037,  1038,  1039,   270,   271,
     310,   463,   222,   311,   312,   313,   314,   315,   316,   317,
     318,   223,  1061,   224,   225,   597,   598,  1063,   591,  1253,
     464,   745,   226,   227,  1254,  1307,   472,   228,  1311,   746,
     747,   282,   748,   229,   230,   575,  1312,  1313,   749,  1314,
    1065,   231,   232,   233,   234,  1315,   750,   751,   752,   652,
     653,   576,  1016,  1316,  1317,  1318,   319,   320,   321,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   811,   813,
     583,   793,   577,   584,   794,  1319,  1320,   222,  1095,  1096,
    1097,  1098,  1099,  1100,  1101,  1102,   223,   817,   224,   225,
    1104,   292,   293,   578,  1321,   294,   295,   226,   227,   579,
    1114,   798,   228,   222,   799,   580,   571,   818,   229,   230,
     371,   371,   223,   789,   224,   225,   231,   232,   233,   234,
     599,   600,   822,   226,   227,   371,  1018,  1136,   228,  1019,
    1020,  1021,  1022,   856,   229,   230,   371,   371,  1067,   900,
    1151,  1152,   231,   232,   233,   234,  1130,   941,  1073,  1131,
     371,  1132,  1133,   371,  1154,   944,   823,   825,  1156,  1157,
     828,   829,   831,   964,   617,   371,   965,    87,  1068,  1075,
     589,   371,  1076,   849,   587,   591,  1031,  1163,  1164,  1165,
     549,   550,   551,   552,   553,   554,   555,   222,   592,   870,
     871,   872,  1159,   590,  1175,   371,   223,   593,   224,   225,
    1176,  1214,   596,   877,   371,   594,   880,   226,   227,  1181,
     602,  1187,   228,   637,   465,   615,  1191,    87,   229,   230,
     468,   638,   468,   647,   651,   469,   231,   232,   233,   234,
     616,  1207,  1208,   655,   469,   668,   669,   684,   469,   686,
     469,   468,   764,   837,   669,   889,   669,   898,   371,   899,
     371,   939,   371,   948,   371,  1158,   949,   598,   680,  1160,
     950,   371,   954,   598,   770,   771,   773,   774,   635,  1230,
     468,   955,   959,   799,   968,   669,   972,   371,   973,   371,
     104,   105,   106,  1244,   974,   653,  1026,   669,  1248,  1027,
     669,  1055,   371,  1057,   598,   636,  1058,   371,  1060,   371,
     468,  1072,  1081,   469,  1082,   653,  1155,   371,  1114,  1213,
     371,  1215,   653,   682,  1260,  1261,  1263,  1223,   669,  1232,
     669,  1283,   669,   677,  1274,   685,  1275,  1285,   669,   689,
     196,   198,   977,   547,   548,   690,   691,   692,   693,   694,
     695,   709,   710,   713,   743,   744,   753,  1287,   745,   746,
     282,   747,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,
     758,   748,   749,   750,   751,   769,   752,   757,   843,  1007,
     782,   787,   795,   809,   790,   791,  1308,  1309,  1310,   788,
     800,   811,  -105,   812,  1013,  -105,  -105,  -105,  -105,  -105,
     813,   814,   815,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,   816,   817,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,   820,   821,   847,   848,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,   857,   858,   859,  -105,  -105,
     860,   861,  -105,  -105,   862,   371,   866,   868,  -105,   873,
     881,   875,  -105,   867,   869,   878,  -105,   882,   410,   874,
    -105,   938,   952,   953,   981,   942,   978,   943,   962,  1080,
     966,   969,   970,  1084,  1085,  1086,   979,   980,   999,  1008,
    1009,  -105,  -105,  1010,  1012,  1014,  1023,  1028,  1029,  1030,
    1032,  1054,  1056,  1059,  1061,  1062,  1063,  1070,  1069,  1064,
    1066,  1109,  1071,  1077,  1078,  1111,  1087,  1103,  1107,  1108,
    1112,  1134,  1110,  1140,  1135,     4,     5,     6,     7,     8,
    1143,  1141,  1142,   138,   139,   129,   130,   346,    14,    15,
      16,    17,    18,    19,    20,  1153,  1174,  -105,  1177,  1180,
    1184,  1185,  1186,  -105,  -105,   132,    30,  1188,  1189,  -105,
    -105,  -105,    36,  1190,  -105,  1204,  -105,  -105,    38,  1202,
    1203,  1241,  1212,  1211,  1220,  1224,    46,    47,    48,    49,
    1233,  1116,    52,  1234,  1235,  1240,  1161,  1162,  1242,  1243,
    1245,  1246,    57,    58,  1286,  1247,  1255,  1276,    59,  1277,
    1257,  1278,  1279,     4,     5,     6,     7,     8,  1280,  1281,
      62,   138,   139,   129,   130,   346,    14,    15,    16,    17,
      18,    19,    20,   167,  1302,  1303,   984,   883,   797,   500,
     528,    63,    64,   132,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,  1221,     0,     0,     0,    87,    65,    62,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,    70,   347,     0,    71,     0,    72,     0,   348,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1256,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,     4,     5,     6,     7,     8,     0,
       0,     0,   138,   139,   129,   130,   346,    14,    15,    16,
      17,    18,    19,    20,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,   132,    30,     0,    68,     0,    70,
     778,    36,    71,     0,    72,     0,   348,    38,     0,     0,
       0,  1306,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    62,
     138,   139,   129,   130,   346,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,   132,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    62,     0,     0,
       0,     0,    66,    67,     0,     0,     0,     0,    68,     0,
      70,   934,     0,    71,     0,    72,     0,   348,    63,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,     0,    70,     0,
       0,    71,     0,    72,     0,   348,     4,     5,     6,     7,
       8,     0,     0,     0,     9,   473,   474,   475,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,   476,
     477,    26,     0,     0,    27,    28,    29,    30,   478,   479,
      33,    34,    35,    36,    37,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,   480,   481,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,    60,     0,     0,     0,    61,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,    66,    67,     0,     0,     0,     0,
      68,    69,    70,     0,     0,    71,     0,    72,   482,     4,
       5,     6,     7,     8,     0,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,   838,   839,    26,     0,     0,    27,    28,    29,
      30,    31,   840,    33,    34,    35,    36,    37,     0,     0,
       0,     0,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,    60,     0,     0,     0,
      61,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,    69,    70,     0,     0,    71,     0,
      72,   841,     4,     5,     6,     7,     8,     0,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,     0,     0,     0,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,    60,
       0,     0,     0,    61,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,    69,    70,     0,
       0,    71,     0,    72,   897,     4,     5,     6,     7,     8,
       0,     0,     0,     9,    10,    11,  1225,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,  1226,  1227,
      26,     0,     0,    27,    28,    29,    30,    31,  1228,    33,
      34,    35,    36,    37,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,    61,     0,     0,     0,
      62,     4,     5,     6,     7,     8,     0,     0,     0,   138,
     139,   129,   130,   131,    14,    15,    16,    17,    18,    19,
      20,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,   132,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,    62,     0,     0,    68,
      69,    70,     0,     0,    71,     0,    72,  1229,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   138,   139,   129,   130,   131,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,   132,    30,     0,     0,    66,
      67,     0,    36,     0,     0,    68,   170,    70,    38,     0,
      71,     0,    72,   726,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      62,   138,   139,   129,   130,   131,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,   132,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    62,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
     170,    70,     0,     0,    71,     0,    72,   988,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,    68,   170,    70,
       0,     0,    71,     0,    72,  1262,     4,     5,     6,     7,
       8,     0,     0,     0,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,    60,     0,     0,     0,    61,     0,     0,
       0,    62,     4,     5,     6,     7,     8,     0,     0,     0,
     138,   139,   129,   130,   131,    14,    15,    16,    17,    18,
      19,    20,    63,    64,     0,     0,     0,     0,     0,     0,
       0,     0,   132,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,    65,     0,
       0,     0,     0,     0,    66,    67,     0,    62,     0,     0,
      68,    69,    70,     0,     0,    71,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   138,   139,   129,   130,   131,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,   132,    30,     0,     0,
      66,    67,     0,    36,     0,     0,    68,     0,    70,    38,
       0,    71,   140,    72,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    62,   138,   139,   129,   130,   131,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,   132,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    62,
       0,     0,     0,     0,    66,    67,     0,     0,     0,     0,
      68,   143,   144,     0,     0,    71,     0,    72,     0,     0,
      63,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   138,   139,   129,   130,
     131,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,   132,    30,
       0,     0,    66,    67,     0,    36,     0,     0,    68,   170,
      70,    38,     0,    71,     0,    72,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,    62,   138,   139,   129,   130,   131,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,   132,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    62,     0,     0,     0,     0,    66,    67,     0,     0,
       0,     0,    68,   172,    70,     0,     0,    71,     0,    72,
       0,     0,    63,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   138,   139,
     129,   130,   131,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
     132,    30,     0,     0,    66,    67,     0,    36,     0,     0,
      68,   205,    70,    38,     0,    71,     0,    72,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    62,   138,   139,   129,   130,
     131,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,   132,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    62,     0,     0,     0,     0,    66,    67,
       0,     0,     0,     0,    68,     0,    70,   236,     0,    71,
       0,    72,     0,     0,    63,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     138,   139,   129,   130,   131,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,   132,    30,     0,     0,    66,    67,     0,    36,
       0,     0,    68,     0,    70,    38,     0,    71,   238,    72,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    62,   138,   139,
     129,   130,   131,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
     132,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    62,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,     0,    70,   333,
       0,    71,     0,    72,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   138,   139,   129,   130,   131,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,   132,    30,     0,     0,    66,    67,
     337,    36,     0,     0,    68,     0,    70,    38,     0,    71,
       0,    72,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    62,
     138,   139,   129,   130,   131,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,   132,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    62,     0,     0,
       0,     0,    66,    67,     0,     0,     0,     0,    68,   365,
      70,     0,     0,    71,     0,    72,     0,     0,    63,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   138,   139,   129,   130,   131,
      14,    15,    16,    17,    18,    19,    20,     0,     0,   442,
       0,     0,     0,     0,    65,     0,     0,   132,    30,     0,
      66,    67,     0,     0,    36,     0,    68,     0,    70,   367,
      38,    71,     0,    72,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    62,   138,   139,   129,   130,   131,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,   132,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      62,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,    70,     0,     0,    71,     0,    72,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   138,   139,   129,
     130,   131,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,   132,
      30,     0,     0,    66,    67,     0,    36,     0,     0,    68,
       0,    70,    38,   449,    71,     0,    72,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,   240,     0,     4,     5,     6,
       7,     8,     0,     0,    62,   138,   139,   129,   130,   131,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,   132,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    62,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,    70,     0,     0,    71,     0,
      72,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   138,
     139,   129,   130,   131,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,   132,    30,     0,     0,    66,    67,     0,    36,     0,
       0,    68,     0,    70,    38,     0,    71,   613,    72,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    62,   138,   139,   129,
     130,   131,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,   132,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    62,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,    70,   714,     0,
      71,     0,    72,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   138,   139,   129,   130,   131,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,   132,    30,     0,    66,    67,     0,
       0,    36,     0,    68,     0,    70,   716,    38,    71,     0,
      72,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    62,
     138,   139,   129,   130,   131,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,   132,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    62,     0,     0,
       0,     0,    66,    67,     0,     0,     0,     0,    68,     0,
      70,     0,     0,    71,   718,    72,     0,     0,    63,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   138,   139,   129,   130,   131,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,   132,    30,     0,
      66,    67,     0,     0,    36,     0,    68,     0,    70,   762,
      38,    71,     0,    72,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     4,     5,     6,   622,     8,
       0,     0,    62,   138,   139,   129,   130,   623,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,   132,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      62,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,    70,   776,     0,    71,     0,    72,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   138,   139,
     129,   130,   131,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
     132,    30,     0,    66,    67,     0,     0,    36,     0,    68,
       0,    70,   236,    38,    71,     0,    72,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    62,   138,   139,   129,   130,
     131,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,   132,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    62,     0,     0,     0,     0,    66,    67,
       0,     0,     0,     0,    68,     0,    70,   930,     0,    71,
       0,    72,     0,     0,    63,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     138,   139,   129,   130,   131,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,   132,    30,     0,     0,    66,    67,     0,    36,
       0,     0,    68,     0,   154,    38,     0,    71,     0,    72,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    62,   138,   139,
     129,   130,   131,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
     132,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    62,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,     0,   156,     0,
       0,    71,     0,    72,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   138,   139,   129,   130,   131,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,   132,    30,     0,     0,    66,    67,
       0,    36,     0,     0,    68,     0,   158,    38,     0,    71,
       0,    72,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    62,
     138,   139,   129,   130,   131,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,   132,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    62,     0,     0,
       0,     0,    66,    67,     0,     0,     0,     0,    68,     0,
     160,     0,     0,    71,     0,    72,     0,     0,    63,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   138,   139,   129,   130,   131,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,   132,    30,     0,     0,
      66,    67,     0,    36,     0,     0,    68,     0,    70,    38,
       0,    71,     0,    72,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    62,   138,   139,   129,   130,   131,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,   132,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    62,
       0,     0,     0,     0,    66,    67,     0,     0,     0,     0,
      68,     0,   181,     0,     0,    71,     0,    72,     0,     0,
      63,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   138,   139,   129,   130,
     131,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,   132,    30,
       0,     0,    66,    67,     0,    36,     0,     0,    68,     0,
     183,    38,     0,    71,     0,    72,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,    62,   138,   139,   129,   130,   131,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,   132,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    62,     0,     0,     0,     0,    66,    67,     0,     0,
       0,     0,    68,     0,    70,     0,     0,    71,     0,   197,
       0,     0,    63,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   138,   139,
     129,   130,   131,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
     132,    30,     0,     0,    66,    67,     0,    36,     0,     0,
      68,     0,   335,    38,     0,    71,     0,    72,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     4,     5,
       6,   622,     8,     0,     0,    62,   138,   139,   129,   130,
     623,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,   132,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    62,     0,     0,     0,     0,    66,    67,
       0,     0,     0,     0,    68,     0,   620,     0,     0,    71,
       0,    72,     0,     0,    63,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     138,   139,   129,   130,   131,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,   132,    30,     0,     0,    66,    67,     0,    36,
       0,     0,    68,     0,   624,    38,     0,    71,     0,    72,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    62,   138,   139,
     129,   130,   131,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
     132,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    62,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,     0,   802,     0,
       0,    71,     0,    72,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   127,   128,   129,   130,   131,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,   132,     0,     0,     0,    66,    67,
       0,    36,     0,     0,    68,     0,   826,    38,     0,    71,
       0,    72,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,    66,    67,     0,     0,     0,     0,    68,     0,
      70,     0,     0,    71,     0,    72
};

static const yytype_int16 yycheck[] =
{
       2,    28,   409,     0,   253,   823,   652,   153,    85,    15,
      24,     2,     2,     2,    15,    17,    11,    12,    28,    15,
      15,    32,    24,    25,    26,    27,    28,    55,    24,    31,
      32,    33,    60,    61,    36,    37,    11,    12,    40,    41,
      15,   323,  1017,   325,    24,   381,    48,    49,    12,    88,
      52,    53,    13,   460,    11,    12,    13,   164,    15,   594,
     164,    24,    11,    12,     0,   600,    15,    15,    70,    71,
      18,   178,   354,   355,   178,    15,    33,    72,    73,    74,
      15,    38,    15,    24,    24,    11,    12,   109,   110,    15,
     109,   110,    42,    43,   133,   134,    24,    72,    73,    74,
     177,    13,   179,    11,    12,   184,    28,    15,   110,   111,
     112,   113,   114,   115,    28,   117,   118,    65,   120,    42,
      43,   123,   179,    72,    73,    74,    48,   184,    11,   164,
       6,   170,    15,    24,    48,    75,    69,    11,    12,    15,
     142,    15,   144,   164,   179,   164,    72,    73,    74,    11,
      12,   186,   154,    15,   156,    15,   158,   179,   160,   977,
      15,   182,  1137,    15,    72,    73,    74,    15,    15,    24,
     184,    11,    12,   184,   184,    15,    24,    24,   179,   181,
     186,   183,    28,   184,   186,   180,   188,   189,   184,   191,
     192,   184,   194,   195,   196,   197,   198,    28,    72,    73,
      74,   164,    48,  1178,   184,   180,   177,  1182,   179,   116,
      72,    73,    74,   177,   179,   179,   184,    48,    11,   126,
      75,   186,   179,   164,   164,   760,   633,    75,    75,    11,
      16,   180,    72,    73,    74,   185,   164,   144,   178,   241,
     242,   243,   244,   245,   246,   247,   248,   249,  1223,   251,
     241,   241,   241,   281,   180,    28,   178,  1232,   404,   181,
      11,    79,   185,   185,   178,   292,   293,   294,   295,   296,
     297,    57,   180,   164,  1249,    48,  1251,    85,    28,    65,
      66,    67,    63,    64,    28,   164,    67,    31,    32,    81,
      82,    83,    28,    28,    11,    77,    78,    41,    48,   178,
      44,    45,   709,   710,    48,    49,   180,   166,  1283,   164,
    1285,    15,    48,    48,    28,   179,   164,   164,   180,    11,
     184,   116,   324,   178,   660,   184,    77,    78,   974,    33,
     178,   178,   178,   335,    48,   181,    42,    43,    85,   185,
     180,    28,   252,  1161,   254,   255,   256,   178,   102,   351,
     181,   353,    86,    87,   356,   357,   358,   359,   360,   361,
     362,    48,    28,    63,    64,    31,    32,    67,   177,   371,
     372,   707,   374,    79,   376,    41,   179,    88,    44,    45,
     407,   408,    48,   186,   386,   387,   388,   389,   390,   391,
     392,   109,   110,   185,   396,   397,    28,   163,    28,   133,
     134,    31,    32,   178,   185,   178,   166,    28,   181,    28,
     185,    41,   185,   410,    44,    45,    48,     2,    48,   421,
     422,    88,   424,   425,   184,   427,   428,    48,   178,    48,
     177,   433,   179,   435,   178,   185,   170,   164,   440,   441,
     164,   185,   178,   178,    29,    30,   164,   449,   179,   185,
     185,   178,   454,   184,   118,   119,   363,    42,    43,   118,
     119,   743,   744,    88,   178,    50,    51,   464,   164,   805,
     181,   185,   177,   184,   476,   477,   478,   479,    11,    12,
     161,   162,    15,   109,   110,   185,   182,   394,   164,    15,
      28,   178,   399,    31,    32,    86,    87,    15,   405,   835,
     164,   837,   178,    41,   753,   164,    44,    45,   133,   134,
      48,    49,   178,   420,   181,   181,   181,   184,   183,   185,
     118,   119,   164,   430,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   179,   164,    72,
      73,    74,   133,   134,   186,   170,   178,   132,   178,   166,
     178,   181,   166,   889,   890,   185,    15,   178,   164,   178,
     467,   164,   469,   470,   185,    28,   164,   184,    31,    32,
     184,   180,   181,   179,   159,   178,   161,   162,    41,   170,
     186,    44,    45,   585,   586,    48,    49,   103,   104,   133,
     134,   177,    28,   595,   178,    31,    32,   182,   600,   184,
     164,   185,   884,    28,   178,    41,    31,    32,    44,    45,
      31,   185,    48,    49,   178,   164,    41,   164,   620,    44,
      45,   166,   624,    48,   164,   627,   170,   629,    49,   631,
     179,   216,   968,   635,   636,   182,   182,   186,   178,   184,
     178,   157,   158,   159,   179,   109,   110,   185,   984,   184,
     178,   653,     8,     9,    10,   177,   241,   185,   178,    15,
     662,   181,   664,   178,   179,   185,    28,   574,   178,    31,
      32,   662,   662,   662,   178,   185,   181,   584,   183,    41,
     587,   185,    44,    45,   164,   592,    48,   594,   690,   178,
    1026,  1027,   178,   180,   181,   181,   185,   107,   700,   109,
     110,   111,   112,   113,   114,   115,   164,   179,  1115,   178,
     712,  1118,   184,    11,    12,   178,   185,    15,   720,   721,
     722,   712,   712,   712,    11,    76,   728,   729,    15,   731,
     732,   638,   734,   735,   736,   737,   738,   739,   740,   741,
     647,  1023,   178,   178,   178,    11,    12,    13,   178,   185,
     185,   185,   754,   178,   164,   185,   181,   759,   181,   178,
     185,   128,   129,   130,   131,   132,   185,   769,   184,   179,
     772,   178,   178,   775,    72,    73,    74,   178,   185,   185,
     782,   298,   299,   300,   185,   109,   110,   111,   112,   113,
     114,   115,   178,   795,   796,   178,   178,  1204,  1205,   185,
     802,   179,   185,   185,   171,   172,   184,   809,   135,   136,
     137,   138,   139,   140,   178,   178,   178,    59,   178,   181,
     178,   185,   185,   178,   826,   185,   178,   185,   178,   181,
     185,    89,    90,    91,    92,   185,   838,   839,   840,   178,
     164,   168,   169,   178,   846,   178,   185,   178,   755,   756,
     185,   179,   185,   760,   761,   179,   178,   764,   178,   178,
     178,   863,   178,   185,   179,   185,   185,   185,   453,   185,
     455,   171,   172,   875,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   179,   181,   184,   183,   794,   184,   108,
     148,   149,   150,   151,   152,   153,   154,   181,   156,   181,
     184,   183,   904,   905,   906,   907,   908,   909,   166,   167,
     117,   184,    15,   120,   121,   122,   123,   124,   125,   126,
     127,    24,   178,    26,    27,   180,   181,   178,   178,   185,
     184,   178,    35,    36,   185,   185,   185,    40,   185,   178,
     178,   160,   178,    46,    47,   178,   185,   185,   178,   185,
     952,    54,    55,    56,    57,   185,   178,   178,   178,    29,
      30,   178,    65,   185,   185,   185,   173,   174,   175,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   178,   178,
     178,   178,   178,   181,   181,   185,   185,    15,   990,   991,
     992,   993,   994,   995,   996,   997,    24,   178,    26,    27,
    1002,   142,   143,   178,   185,   146,   147,    35,    36,   178,
    1012,   178,    40,    15,   181,   178,    18,   178,    46,    47,
     181,   181,    24,   183,    26,    27,    54,    55,    56,    57,
     180,   181,   178,    35,    36,   181,    11,    65,    40,    14,
      15,    16,    17,   178,    46,    47,   181,   181,   955,   183,
    1052,  1053,    54,    55,    56,    57,    11,   178,   965,    14,
     181,    16,    17,   181,  1066,   183,   651,   652,  1070,  1071,
     655,   656,   657,   178,   180,   181,   181,   662,   178,   178,
     167,   181,   181,   668,   181,   178,   183,  1089,  1090,  1091,
     303,   304,   305,   306,   307,   308,   309,    15,   179,   684,
     685,   686,   178,   185,  1106,   181,    24,    15,    26,    27,
    1112,   178,    15,   698,   181,   179,   701,    35,    36,  1116,
      15,  1123,    40,   180,   181,   178,  1128,   712,    46,    47,
     180,   181,   180,   181,   180,   181,    54,    55,    56,    57,
     178,  1143,  1144,   180,   181,   180,   181,   180,   181,   180,
     181,   180,   181,   180,   181,   180,   181,   180,   181,   180,
     181,   180,   181,   180,   181,  1072,   180,   181,    15,  1076,
     180,   181,   180,   181,   118,   119,   118,   119,   164,  1181,
     180,   181,   180,   181,   180,   181,   180,   181,   180,   181,
    1181,  1181,  1181,  1195,    29,    30,   180,   181,  1200,   180,
     181,   180,   181,   180,   181,   164,   180,   181,   180,   181,
     180,   181,   180,   181,    29,    30,   180,   181,  1220,   180,
     181,    29,    30,    15,  1226,  1227,  1228,   180,   181,   180,
     181,   180,   181,   178,  1236,    29,  1238,   180,   181,    11,
      47,    48,   827,   301,   302,   178,   178,   178,   178,   178,
     185,   179,   179,    11,   177,   177,   105,  1259,   178,   178,
     160,   178,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,
      15,   178,   178,   178,   178,   164,   178,   178,    15,   864,
     179,   167,    85,   164,   178,   178,  1288,  1289,  1290,   185,
     178,   178,     0,   178,   879,     3,     4,     5,     6,     7,
     178,   178,   178,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,   178,   178,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,   178,   178,    11,    11,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,   178,   178,   178,    66,    67,
     178,   178,    70,    71,   178,   181,   167,   167,    76,   178,
      15,   164,    80,   185,   185,   185,    84,    15,   184,   178,
      88,   178,    85,    15,    15,   183,    11,   183,   178,   974,
     178,   178,   178,   978,   979,   980,    11,    11,    15,   178,
     178,   109,   110,    29,   178,   185,    63,    15,    15,    15,
     185,   178,   178,   178,   178,   178,   178,    85,   178,   180,
     179,  1006,    85,   178,   178,  1010,    15,    15,    11,    11,
     178,   178,   185,     8,   178,     3,     4,     5,     6,     7,
     164,     8,     8,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   178,    15,   165,   180,    79,
      15,    15,   178,   171,   172,    33,    34,    15,    15,   177,
     178,   179,    40,   178,   182,   179,   184,   185,    46,   178,
     178,    15,   178,   180,   178,   185,    54,    55,    56,    57,
     178,   184,    60,   178,   178,   178,  1081,  1082,    15,   178,
      15,    15,    70,    71,   180,   178,   178,   178,    76,   178,
    1220,   178,   178,     3,     4,     5,     6,     7,   178,   178,
      88,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    30,   178,   178,   837,   705,   624,   257,
     282,   109,   110,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,  1177,    -1,    -1,    -1,  1181,   165,    88,    -1,
      -1,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,   177,
      -1,   179,   180,    -1,   182,    -1,   184,    -1,   186,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1215,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,   172,    -1,    33,    34,    -1,   177,    -1,   179,
     180,    40,   182,    -1,   184,    -1,   186,    46,    -1,    -1,
      -1,  1286,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    88,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    88,    -1,    -1,
      -1,    -1,   171,   172,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,    -1,   182,    -1,   184,    -1,   186,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,   172,    -1,    -1,    -1,    -1,   177,    -1,   179,    -1,
      -1,   182,    -1,   184,    -1,   186,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,
     177,   178,   179,    -1,    -1,   182,    -1,   184,   185,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,   172,    -1,
      -1,    -1,    -1,   177,   178,   179,    -1,    -1,   182,    -1,
     184,   185,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,   172,    -1,    -1,    -1,    -1,   177,   178,   179,    -1,
      -1,   182,    -1,   184,   185,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      88,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,   172,    -1,    88,    -1,    -1,   177,
     178,   179,    -1,    -1,   182,    -1,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    33,    34,    -1,    -1,   171,
     172,    -1,    40,    -1,    -1,   177,   178,   179,    46,    -1,
     182,    -1,   184,   185,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      88,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    88,    -1,
      -1,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,   177,
     178,   179,    -1,    -1,   182,    -1,   184,   185,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,   172,    -1,    -1,    -1,    -1,   177,   178,   179,
      -1,    -1,   182,    -1,   184,   185,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    88,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,   172,    -1,    88,    -1,    -1,
     177,   178,   179,    -1,    -1,   182,    -1,   184,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    33,    34,    -1,    -1,
     171,   172,    -1,    40,    -1,    -1,   177,    -1,   179,    46,
      -1,   182,   183,   184,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    88,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    88,
      -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,
     177,   178,   179,    -1,    -1,   182,    -1,   184,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    33,    34,
      -1,    -1,   171,   172,    -1,    40,    -1,    -1,   177,   178,
     179,    46,    -1,   182,    -1,   184,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    88,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    88,    -1,    -1,    -1,    -1,   171,   172,    -1,    -1,
      -1,    -1,   177,   178,   179,    -1,    -1,   182,    -1,   184,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      33,    34,    -1,    -1,   171,   172,    -1,    40,    -1,    -1,
     177,   178,   179,    46,    -1,   182,    -1,   184,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    88,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    88,    -1,    -1,    -1,    -1,   171,   172,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,    -1,   182,
      -1,   184,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    33,    34,    -1,    -1,   171,   172,    -1,    40,
      -1,    -1,   177,    -1,   179,    46,    -1,   182,   183,   184,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    88,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    88,    -1,    -1,    -1,    -1,
     171,   172,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
      -1,   182,    -1,   184,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    33,    34,    -1,    -1,   171,   172,
     173,    40,    -1,    -1,   177,    -1,   179,    46,    -1,   182,
      -1,   184,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    88,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    88,    -1,    -1,
      -1,    -1,   171,   172,    -1,    -1,    -1,    -1,   177,   178,
     179,    -1,    -1,   182,    -1,   184,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    33,    34,    -1,
     171,   172,    -1,    -1,    40,    -1,   177,    -1,   179,   180,
      46,   182,    -1,   184,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    88,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      88,    -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,
      -1,   177,    -1,   179,    -1,    -1,   182,    -1,   184,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    33,
      34,    -1,    -1,   171,   172,    -1,    40,    -1,    -1,   177,
      -1,   179,    46,   181,   182,    -1,   184,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    79,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    88,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    88,    -1,    -1,    -1,    -1,   171,   172,    -1,
      -1,    -1,    -1,   177,    -1,   179,    -1,    -1,   182,    -1,
     184,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    33,    34,    -1,    -1,   171,   172,    -1,    40,    -1,
      -1,   177,    -1,   179,    46,    -1,   182,   183,   184,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    88,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    88,    -1,    -1,    -1,    -1,   171,
     172,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,    -1,
     182,    -1,   184,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    33,    34,    -1,   171,   172,    -1,
      -1,    40,    -1,   177,    -1,   179,   180,    46,   182,    -1,
     184,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    88,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    88,    -1,    -1,
      -1,    -1,   171,   172,    -1,    -1,    -1,    -1,   177,    -1,
     179,    -1,    -1,   182,   183,   184,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    33,    34,    -1,
     171,   172,    -1,    -1,    40,    -1,   177,    -1,   179,   180,
      46,   182,    -1,   184,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    88,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      88,    -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,
      -1,   177,    -1,   179,   180,    -1,   182,    -1,   184,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      33,    34,    -1,   171,   172,    -1,    -1,    40,    -1,   177,
      -1,   179,   180,    46,   182,    -1,   184,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    88,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    88,    -1,    -1,    -1,    -1,   171,   172,
      -1,    -1,    -1,    -1,   177,    -1,   179,   180,    -1,   182,
      -1,   184,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    33,    34,    -1,    -1,   171,   172,    -1,    40,
      -1,    -1,   177,    -1,   179,    46,    -1,   182,    -1,   184,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    88,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    88,    -1,    -1,    -1,    -1,
     171,   172,    -1,    -1,    -1,    -1,   177,    -1,   179,    -1,
      -1,   182,    -1,   184,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    33,    34,    -1,    -1,   171,   172,
      -1,    40,    -1,    -1,   177,    -1,   179,    46,    -1,   182,
      -1,   184,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    88,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    88,    -1,    -1,
      -1,    -1,   171,   172,    -1,    -1,    -1,    -1,   177,    -1,
     179,    -1,    -1,   182,    -1,   184,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    33,    34,    -1,    -1,
     171,   172,    -1,    40,    -1,    -1,   177,    -1,   179,    46,
      -1,   182,    -1,   184,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    88,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    88,
      -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,
     177,    -1,   179,    -1,    -1,   182,    -1,   184,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    33,    34,
      -1,    -1,   171,   172,    -1,    40,    -1,    -1,   177,    -1,
     179,    46,    -1,   182,    -1,   184,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    88,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    88,    -1,    -1,    -1,    -1,   171,   172,    -1,    -1,
      -1,    -1,   177,    -1,   179,    -1,    -1,   182,    -1,   184,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      33,    34,    -1,    -1,   171,   172,    -1,    40,    -1,    -1,
     177,    -1,   179,    46,    -1,   182,    -1,   184,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    88,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    88,    -1,    -1,    -1,    -1,   171,   172,
      -1,    -1,    -1,    -1,   177,    -1,   179,    -1,    -1,   182,
      -1,   184,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    33,    34,    -1,    -1,   171,   172,    -1,    40,
      -1,    -1,   177,    -1,   179,    46,    -1,   182,    -1,   184,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    88,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    88,    -1,    -1,    -1,    -1,
     171,   172,    -1,    -1,    -1,    -1,   177,    -1,   179,    -1,
      -1,   182,    -1,   184,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    33,    -1,    -1,    -1,   171,   172,
      -1,    40,    -1,    -1,   177,    -1,   179,    46,    -1,   182,
      -1,   184,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,   172,    -1,    -1,    -1,    -1,   177,    -1,
     179,    -1,    -1,   182,    -1,   184
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   188,   189,     0,     3,     4,     5,     6,     7,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    66,    67,    70,    71,    76,
      80,    84,    88,   109,   110,   165,   171,   172,   177,   178,
     179,   182,   184,   190,   191,   193,   194,   195,   196,   197,
     199,   200,   204,   208,   209,   210,   213,   218,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   237,   238,   240,   241,   107,   109,   110,
     111,   112,   113,   114,   115,   164,   179,   164,   182,   166,
     184,   109,   110,   164,    12,   177,   179,    11,    12,    13,
      14,    15,    33,   218,   233,   238,   240,   241,    11,    12,
     183,   220,    15,   178,   179,   220,   239,    11,    12,    13,
      15,    33,    38,   179,   179,   220,   179,   220,   179,   220,
     179,   220,   220,   229,    15,   179,   218,   199,   218,   220,
     178,   220,   178,   220,    11,    15,    15,   220,   220,    13,
      13,   179,   220,   179,   220,   218,   179,   218,    28,    48,
     178,    28,    48,   178,   184,   219,   219,   184,   219,   220,
     220,   218,   184,   218,   220,   178,   220,    15,   207,   207,
      15,    33,   233,   207,   207,    11,   235,    11,    11,   233,
     233,   233,    15,    24,    26,    27,    35,    36,    40,    46,
      47,    54,    55,    56,    57,   236,   180,   220,   183,   220,
      79,   189,    28,    31,    32,    41,    44,    45,    48,    49,
     178,    85,   103,   104,   157,   158,   159,   102,    89,    90,
      91,    92,   148,   149,   150,   151,   152,   153,   154,   156,
     166,   167,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   108,   160,   135,   136,   137,   138,   139,   140,   168,
     169,   116,   142,   143,   146,   147,    86,    87,   133,   134,
     170,   161,   162,   128,   129,   130,   131,   132,   171,   172,
     117,   120,   121,   122,   123,   124,   125,   126,   127,   173,
     174,   175,   163,   177,   164,   177,    15,   220,   220,   220,
     220,   220,   220,   180,   239,   179,   220,   173,   220,    15,
     220,   220,   178,    15,    18,    65,    15,   180,   186,   215,
     239,   182,   166,   184,   177,   177,   111,   112,   113,   114,
     115,   164,   164,   181,   183,   178,   220,   180,   220,   239,
     178,   181,   164,   178,   164,   178,   164,   178,    11,    12,
      13,    15,    11,    15,   235,   220,    28,    31,    32,    41,
      44,    45,    48,   178,   181,   220,    28,    48,   178,   181,
     220,   218,   220,   218,    88,   181,   218,    86,    87,   179,
     184,   205,    11,    12,    15,    72,    73,    74,   217,   178,
     181,    28,    48,   178,    28,    48,   178,   164,   164,    76,
     181,   184,   166,   184,   166,   184,   220,   218,   220,   218,
      31,    49,    25,   220,   220,   220,   220,   220,   220,   181,
     220,   220,   220,    59,    42,    43,   201,   178,   211,   184,
     179,    15,    69,   184,   184,   181,   218,   179,   180,   181,
     181,   183,   185,    12,    13,    14,    26,    27,    35,    36,
      52,    53,   185,   220,   238,   241,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   223,   221,   223,   223,   223,
     224,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,     8,     9,    10,   207,   226,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     229,   229,   229,   229,   230,   230,   230,   231,   231,   232,
     232,   232,   232,   232,   232,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   236,
     220,    18,   236,   178,   179,   178,   178,   178,   178,   178,
     178,   180,   220,   178,   181,   171,   172,   181,   183,   167,
     185,   178,   179,    15,   179,    85,    15,   180,   181,   180,
     181,   220,    15,   220,   236,   236,   220,   220,   220,   220,
     220,   220,   220,   183,   239,   178,   178,   180,   220,   220,
     179,   220,     6,    15,   179,   216,   220,   164,   178,   164,
     178,   164,   178,   179,   205,   164,   164,   180,   181,   220,
     220,   220,   220,   220,   220,   220,   239,   181,   220,   220,
     239,   180,    29,    30,   192,   180,   235,   239,   229,   229,
     180,   217,   189,    24,   164,    11,    77,    78,   180,   181,
     239,   220,   220,   220,   220,   220,   220,   178,   239,   202,
      15,   220,    15,   220,   180,    29,   180,   220,   220,    11,
     178,   178,   178,   178,   178,   185,   220,   218,   220,   218,
      42,    43,   185,    15,    24,   184,   212,   180,   217,   179,
     179,   214,   189,    11,   180,   239,   180,   239,   183,   239,
     182,   184,   164,   220,   220,   220,   185,   220,    28,    48,
     185,    28,    48,   185,    28,    31,    32,    41,    44,    45,
      48,    49,   185,   177,   177,   178,   178,   178,   178,   178,
     178,   178,   178,   105,   164,   179,   186,   178,    15,   164,
     179,   186,   180,   239,   181,   239,   220,   220,   239,   164,
     118,   119,   164,   118,   119,   164,   180,   239,   180,   215,
     239,   220,   179,    15,   186,   215,   183,   167,   185,   183,
     178,   178,   220,   178,   181,    85,    85,   216,   178,   181,
     178,   220,   179,   220,   220,   180,   217,   220,   220,   164,
     239,   178,   178,   178,   178,   178,   178,   178,   178,   239,
     178,   178,   178,   218,   191,   218,   179,   220,   218,   218,
      88,   218,    88,    88,    24,   203,   205,   180,    26,    27,
      36,   185,   220,    15,   220,    24,   164,    11,    11,   218,
      11,    12,    15,    72,    73,    74,   178,   178,   178,   178,
     178,   178,   178,    42,    43,   185,   167,   185,   167,   185,
     218,   218,   218,   178,   178,   164,   220,   218,   185,   220,
     218,    15,    15,   212,    63,    64,    67,   185,   205,   180,
     180,   217,   217,    81,    82,    83,   185,   185,   180,   180,
     183,   220,   220,   220,    28,    32,    41,    44,    45,    48,
     185,   185,   185,   185,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   236,   236,   221,   220,
     180,   239,   239,   220,   180,   215,   239,   239,   178,   180,
     239,   178,   183,   183,   183,   220,   220,   220,   180,   180,
     180,   220,    85,    15,   180,   181,   239,   220,   220,   180,
       6,    15,   178,   220,   178,   181,   178,   205,   180,   178,
     178,   220,   180,   180,    29,   192,   220,   218,    11,    11,
      11,    15,    24,   205,   203,   205,   220,   220,   185,   220,
      28,    31,    32,    41,    44,    45,    48,    49,   185,    15,
     220,    24,   164,    11,    77,    78,   220,   218,   178,   178,
      29,   220,   178,   218,   185,   185,    65,   236,    11,    14,
      15,    16,    17,    63,   205,   205,   180,   180,    15,    15,
      15,   183,   185,   185,   220,   220,   220,   220,   220,   220,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   164,   164,   178,   180,   178,   180,   180,   178,
     180,   178,   178,   178,   180,   220,   179,   239,   178,   178,
      85,    85,   181,   239,   205,   178,   181,   178,   178,   191,
     218,   180,    29,   192,   218,   218,   218,    15,   205,    28,
      32,    48,   185,   185,   185,   220,   220,   220,   220,   220,
     220,   220,   220,    15,   220,    24,   164,    11,    11,   218,
     185,   218,   178,   198,   220,   179,   184,   206,   179,   206,
      15,    24,    75,   164,   178,    15,    24,    75,   164,   178,
      11,    14,    16,    17,   178,   178,    65,   236,   205,   205,
       8,     8,     8,   164,   164,   185,   185,   185,   185,   185,
     185,   220,   220,   178,   220,   180,   220,   220,   239,   178,
     239,   218,   218,   220,   220,   220,   185,   185,   185,   185,
     185,   185,   185,   185,    15,   220,   220,   180,   180,   217,
      79,   189,   180,   217,    15,    15,   178,   220,    15,    15,
     178,   220,    15,    24,    75,   164,   178,    15,    24,    75,
     164,   178,   178,   178,   179,   179,   206,   220,   220,   185,
     185,   180,   178,   180,   178,    29,   192,   185,   185,   185,
     178,   218,   206,   180,   185,    14,    26,    27,    36,   185,
     220,   206,   180,   178,   178,   178,   164,   178,   164,   178,
     178,    15,    15,   178,   220,    15,    15,   178,   220,   180,
     217,   180,   217,   185,   185,   178,   218,   198,   206,   164,
     220,   220,   185,   220,    28,    31,    32,    41,    44,    45,
      48,    49,   185,   206,   220,   220,   178,   178,   178,   178,
     178,   178,   206,   180,   206,   180,   180,   220,    28,    32,
      48,   185,   185,   185,   220,   220,   220,   220,   220,   220,
     220,   220,   178,   178,   206,   206,   218,   185,   220,   220,
     220,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   187,   188,   189,   189,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   191,   191,   191,   191,   191,   191,   191,   191,   192,
     192,   192,   192,   192,   192,   193,   193,   194,   194,   194,
     194,   195,   195,   196,   196,   196,   197,   197,   197,   198,
     199,   199,   199,   199,   199,   199,   200,   200,   201,   201,
     202,   202,   203,   203,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   207,   208,   209,   210,   211,   211,   211,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   213,   214,
     214,   214,   214,   215,   215,   215,   215,   215,   216,   216,
     216,   216,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   219,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   221,   221,   222,   222,   222,   222,   222,   223,   223,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   225,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   227,
     227,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     229,   229,   230,   230,   230,   231,   231,   231,   231,   231,
     231,   231,   231,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   233,   233,   233,   233,
     233,   233,   234,   234,   235,   235,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   237,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   239,   239,   239,   240,
     240,   240,   240,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     4,
       3,     3,     5,     7,     9,     4,     6,     8,     5,     5,
       7,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       4,     5,     5,     6,     6,     4,     2,     5,     4,     3,
       3,     3,     5,     7,     3,     5,     7,     3,     5,     3,
       2,     3,     2,     5,     5,     3,     2,     5,     5,     4,
       7,     6,     4,     6,     6,     4,     7,     7,     7,     6,
       6,     4,     4,     4,     4,     4,     4,     4,     4,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     4,     4,
       4,     4,     4,     2,     1,     1,     1,     1,     1,     2,
       4,     4,     6,     5,     2,     1,     1,     1,     1,     1,
       2,     2,     4,     4,     4,     4,     1,     1,     1,     1,
       1,     5,     7,     7,     3,     5,     5,     4,     6,     3,
       5,     5,     7,     4,     6,     5,     3,     5,     7,     3,
       5,     5,     3,     2,     5,     5,     2,    12,     9,     1,
       7,     7,     5,     7,     5,     3,     5,     7,     3,     4,
       0,     4,     2,     3,     6,     5,     3,     7,     6,     7,
       6,     4,     7,     6,     7,     6,     5,     3,     4,     5,
       4,     5,     5,     6,     6,     6,     6,     6,     6,     6,
       6,     7,     7,     7,     3,     3,     6,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     1,     6,     5,     5,     0,     3,     3,
       0,     4,     4,     4,     5,     5,     4,     5,     5,     6,
       6,     7,     7,     6,     8,     8,     6,     6,     6,     5,
       5,     6,     6,     7,     7,     7,     7,     7,     6,     4,
       7,     6,     4,     8,     7,     5,     8,     7,     5,     0,
       4,     4,     4,     3,     5,     2,     5,     7,     3,     3,
       5,     5,     1,     3,     5,     4,     6,     1,     3,     2,
       3,     3,     3,     4,     5,     5,     3,     5,     4,     6,
       1,     3,     1,     3,     1,     3,     3,     3,     4,     6,
       6,     6,     6,     6,     6,     6,     6,     5,     7,     7,
       7,     7,     7,     7,     5,     8,     8,     6,     9,     9,
       5,     5,     4,     4,     4,     6,     6,     6,     6,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     3,
       1,     5,     1,     3,     3,     3,     3,     1,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     2,     2,     2,
       2,     1,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     3,     4,     6,     4,     3,     6,     6,     5,
       3,     3,     6,     5,     4,     4,     6,     6,     5,     3,
       6,     5,     3,     5,     5,     5,     4,     2,     2,     3,
       4,     3,     2,     3,     2,     1,     1,     3,     2,     2,
       3,     4,     5,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     4,     6,     6,     6,     1,     4,     4,     5,     5,
       5,     5,     5,     5,     1,     1,     1,     1,     2,     3,
       4,     5,     2,     3,     4,     5,     1,     1,     2,     5,
       3
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
#line 595 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3806 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 605 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3812 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 606 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3818 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 610 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3824 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 612 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3830 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 614 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3836 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 616 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3844 "raku.tab.c"
    break;

  case 9: /* stmt: paren_group '=' expr ';'  */
#line 620 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3850 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 622 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3856 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 624 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3862 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 626 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3868 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 628 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3876 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 632 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3884 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 636 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3890 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 638 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3898 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 642 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3906 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 646 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3912 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 648 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3918 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 650 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3924 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 652 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3930 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3936 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 656 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3944 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 660 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3952 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 664 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3958 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 666 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3964 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 668 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3970 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 670 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3976 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 672 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3982 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 674 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3988 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 676 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 3994 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 678 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4000 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 680 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4006 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 682 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4012 "raku.tab.c"
    break;

  case 35: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 684 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); ct_drop(tok); }
          ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4024 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP ';'  */
#line 692 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4030 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 694 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4036 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP '(' ')' ';'  */
#line 696 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4042 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP arg_list ';'  */
#line 698 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4048 "raku.tab.c"
    break;

  case 40: /* stmt: IDENT VAR_ARRAY ';'  */
#line 700 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4054 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ';'  */
#line 702 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4060 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 704 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4067 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 707 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4074 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ';'  */
#line 710 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4080 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 712 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4087 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 715 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4094 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ';'  */
#line 718 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4100 "raku.tab.c"
    break;

  case 48: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 720 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4108 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr ';'  */
#line 724 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4114 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN ';'  */
#line 726 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4120 "raku.tab.c"
    break;

  case 51: /* stmt: KW_FAIL expr ';'  */
#line 728 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4126 "raku.tab.c"
    break;

  case 52: /* stmt: KW_FAIL ';'  */
#line 730 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4132 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 732 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4139 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 735 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4146 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 738 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4152 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 740 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4159 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 743 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4166 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 746 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4173 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 749 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4179 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 751 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4188 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 756 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4196 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 760 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4204 "raku.tab.c"
    break;

  case 63: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 764 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4211 "raku.tab.c"
    break;

  case 64: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 767 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4218 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 770 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4226 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 774 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4233 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 777 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4240 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 780 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4247 "raku.tab.c"
    break;

  case 69: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 783 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4254 "raku.tab.c"
    break;

  case 70: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 786 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4261 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_IF expr ';'  */
#line 789 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4267 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_UNLESS expr ';'  */
#line 791 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4273 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WHILE expr ';'  */
#line 793 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4279 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_UNTIL expr ';'  */
#line 795 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4285 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_FOR expr ';'  */
#line 797 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4292 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_WITH expr ';'  */
#line 800 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4298 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_WITHOUT expr ';'  */
#line 802 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4304 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_GIVEN expr ';'  */
#line 804 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4310 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 806 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4317 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 809 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4324 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 812 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4332 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 816 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4339 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 819 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4345 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 821 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4351 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 823 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4357 "raku.tab.c"
    break;

  case 86: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 825 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4364 "raku.tab.c"
    break;

  case 87: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 828 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4371 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 831 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4378 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 834 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4385 "raku.tab.c"
    break;

  case 90: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 837 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4392 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 840 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4399 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 843 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4406 "raku.tab.c"
    break;

  case 93: /* stmt: expr ';'  */
#line 845 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4412 "raku.tab.c"
    break;

  case 94: /* stmt: ';'  */
#line 846 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4418 "raku.tab.c"
    break;

  case 95: /* stmt: if_stmt  */
#line 847 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4424 "raku.tab.c"
    break;

  case 96: /* stmt: while_stmt  */
#line 848 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4430 "raku.tab.c"
    break;

  case 97: /* stmt: for_stmt  */
#line 849 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4436 "raku.tab.c"
    break;

  case 98: /* stmt: given_stmt  */
#line 850 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4442 "raku.tab.c"
    break;

  case 99: /* stmt: KW_TRY block  */
#line 852 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4448 "raku.tab.c"
    break;

  case 100: /* stmt: KW_TRY block KW_CATCH block  */
#line 854 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4454 "raku.tab.c"
    break;

  case 101: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 856 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4462 "raku.tab.c"
    break;

  case 102: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 860 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4471 "raku.tab.c"
    break;

  case 103: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 865 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4479 "raku.tab.c"
    break;

  case 104: /* stmt: KW_CATCH block  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4485 "raku.tab.c"
    break;

  case 105: /* stmt: block  */
#line 871 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4491 "raku.tab.c"
    break;

  case 106: /* stmt: unless_stmt  */
#line 872 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4497 "raku.tab.c"
    break;

  case 107: /* stmt: until_stmt  */
#line 873 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4503 "raku.tab.c"
    break;

  case 108: /* stmt: repeat_stmt  */
#line 874 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4509 "raku.tab.c"
    break;

  case 109: /* stmt: loop_stmt  */
#line 875 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4515 "raku.tab.c"
    break;

  case 110: /* stmt: KW_LAST ';'  */
#line 876 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4521 "raku.tab.c"
    break;

  case 111: /* stmt: KW_NEXT ';'  */
#line 877 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4527 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST KW_IF expr ';'  */
#line 879 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4533 "raku.tab.c"
    break;

  case 113: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 881 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4539 "raku.tab.c"
    break;

  case 114: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 883 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4545 "raku.tab.c"
    break;

  case 115: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 885 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4551 "raku.tab.c"
    break;

  case 116: /* stmt: sub_decl  */
#line 886 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4557 "raku.tab.c"
    break;

  case 117: /* stmt: class_decl  */
#line 887 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4563 "raku.tab.c"
    break;

  case 118: /* stmt: role_decl  */
#line 888 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4569 "raku.tab.c"
    break;

  case 119: /* stmt: grammar_decl  */
#line 889 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4575 "raku.tab.c"
    break;

  case 120: /* stmt: module_decl  */
#line 890 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4581 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block  */
#line 894 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4587 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 896 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4593 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 898 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4599 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF expr block  */
#line 900 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4605 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 902 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4611 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 904 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4617 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block elsif_tail  */
#line 906 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4623 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 908 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4629 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF expr block  */
#line 912 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4635 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 914 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4641 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 916 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4647 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 918 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4653 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 920 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4659 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 922 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4665 "raku.tab.c"
    break;

  case 135: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 926 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4671 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE expr block  */
#line 928 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4677 "raku.tab.c"
    break;

  case 137: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 932 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4683 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 934 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4689 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS expr block  */
#line 936 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4695 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 938 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4701 "raku.tab.c"
    break;

  case 141: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 942 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4707 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL expr block  */
#line 944 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4713 "raku.tab.c"
    break;

  case 143: /* repeat_stmt: KW_REPEAT block  */
#line 948 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4719 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 950 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4725 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 952 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4731 "raku.tab.c"
    break;

  case 146: /* loop_stmt: KW_LOOP block  */
#line 956 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4737 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 958 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4743 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 960 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4749 "raku.tab.c"
    break;

  case 149: /* loop_incr: expr  */
#line 963 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4755 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 967 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4765 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 973 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4775 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 979 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node));
          } else (yyval.node) = rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)); }
#line 4787 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 987 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4797 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 993 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4806 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr block  */
#line 998 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4813 "raku.tab.c"
    break;

  case 156: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 1003 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4825 "raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 1011 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4838 "raku.tab.c"
    break;

  case 158: /* catch_when_list: KW_WHEN expr block  */
#line 1022 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 4844 "raku.tab.c"
    break;

  case 159: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1024 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 4850 "raku.tab.c"
    break;

  case 160: /* when_list: %empty  */
#line 1027 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4856 "raku.tab.c"
    break;

  case 161: /* when_list: when_list KW_WHEN expr block  */
#line 1029 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4864 "raku.tab.c"
    break;

  case 162: /* sub_trait_list: TESTOP IDENT  */
#line 1034 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 4870 "raku.tab.c"
    break;

  case 163: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1035 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 4876 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1039 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4888 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1047 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4898 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1053 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4908 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_trait_list sub_body  */
#line 1059 "raku.y"
        { ExprList *params=(yyvsp[-3].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-5].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-5].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4920 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1067 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4930 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1073 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4942 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1081 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4952 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1087 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4962 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1093 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4975 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1102 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4986 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1109 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4999 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1118 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5012 "raku.tab.c"
    break;

  case 176: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1127 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5023 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list '}'  */
#line 1135 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5029 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr '}'  */
#line 1137 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5036 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1140 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5043 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1143 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5049 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1145 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5055 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1147 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5061 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1149 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5067 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1151 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5073 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1153 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5079 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1155 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5085 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1157 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5092 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1160 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5098 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1162 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5104 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1164 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5110 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1166 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5117 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1169 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5124 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1172 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5132 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list '}'  */
#line 1177 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5138 "raku.tab.c"
    break;

  case 195: /* method_body: '{' YADA '}'  */
#line 1178 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5144 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1180 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-4].list); exprlist_append(l,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5151 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr '}'  */
#line 1183 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5158 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1186 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5165 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1189 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5171 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1191 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5177 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1193 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5183 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1195 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5189 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1197 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5195 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1199 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5201 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1201 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5207 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1203 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5214 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1206 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5220 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1208 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5226 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1210 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5232 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1212 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5239 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1215 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5246 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1218 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5254 "raku.tab.c"
    break;

  case 213: /* pkg_name: IDENT  */
#line 1223 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5260 "raku.tab.c"
    break;

  case 214: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1227 "raku.y"
        {
            const char *cname = intern((yyvsp[-4].sval)); ct_drop((yyvsp[-4].sval));
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
#line 5278 "raku.tab.c"
    break;

  case 215: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1243 "raku.y"
        {
            const char *rname = intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
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
#line 5295 "raku.tab.c"
    break;

  case 216: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1258 "raku.y"
        {
            const char *mname = intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
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
#line 5312 "raku.tab.c"
    break;

  case 217: /* is_clauses: %empty  */
#line 1272 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5318 "raku.tab.c"
    break;

  case 218: /* is_clauses: is_clauses IDENT IDENT  */
#line 1274 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)ct_alloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)ct_alloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); ct_drop((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval));
        }
#line 5334 "raku.tab.c"
    break;

  case 219: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1286 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)ct_alloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)ct_alloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); ct_drop((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval));
        }
#line 5350 "raku.tab.c"
    break;

  case 220: /* class_body_list: %empty  */
#line 1299 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5356 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1301 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5363 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1304 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5370 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1307 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5377 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1310 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5384 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1313 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5391 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1316 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5398 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1319 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5405 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1322 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5412 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1325 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5419 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1328 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5426 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1331 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5433 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1334 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5440 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1337 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5451 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1344 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5461 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1350 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5471 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1356 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5482 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1363 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5493 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1370 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5504 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1377 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5512 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1381 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5520 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1385 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5528 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1389 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5536 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1393 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5547 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1400 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5558 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1407 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5569 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1414 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5580 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1421 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5594 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1431 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5606 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1439 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5618 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1447 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5631 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1456 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5642 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1463 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5653 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1470 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5667 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1480 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5679 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1488 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5691 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1496 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle(ct_strdup("new"), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5704 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1505 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5715 "raku.tab.c"
    break;

  case 258: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1514 "raku.y"
        {
            const char *gname = intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
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
#line 5732 "raku.tab.c"
    break;

  case 259: /* grammar_body_list: %empty  */
#line 1528 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5738 "raku.tab.c"
    break;

  case 260: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1530 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5747 "raku.tab.c"
    break;

  case 261: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1535 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5756 "raku.tab.c"
    break;

  case 262: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1540 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5765 "raku.tab.c"
    break;

  case 263: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1547 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5773 "raku.tab.c"
    break;

  case 264: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1551 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5781 "raku.tab.c"
    break;

  case 265: /* named_arg_list: ':' IDENT  */
#line 1555 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 5792 "raku.tab.c"
    break;

  case 266: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1562 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5800 "raku.tab.c"
    break;

  case 267: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1566 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5808 "raku.tab.c"
    break;

  case 268: /* pair_list: IDENT OP_FATARROW expr  */
#line 1572 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5814 "raku.tab.c"
    break;

  case 269: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1574 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5820 "raku.tab.c"
    break;

  case 270: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1576 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5826 "raku.tab.c"
    break;

  case 271: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1578 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5832 "raku.tab.c"
    break;

  case 272: /* param_list: VAR_SCALAR  */
#line 1581 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5838 "raku.tab.c"
    break;

  case 273: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1582 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5844 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1583 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5850 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1584 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 5856 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1585 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 5862 "raku.tab.c"
    break;

  case 277: /* param_list: VAR_ARRAY  */
#line 1586 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5868 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' VAR_ARRAY  */
#line 1587 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5874 "raku.tab.c"
    break;

  case 279: /* param_list: IDENT VAR_SCALAR  */
#line 1588 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 5880 "raku.tab.c"
    break;

  case 280: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1589 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 5886 "raku.tab.c"
    break;

  case 281: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1590 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 5892 "raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' VAR_SCALAR  */
#line 1591 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5898 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1592 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 5904 "raku.tab.c"
    break;

  case 284: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1593 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 5910 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1594 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 5916 "raku.tab.c"
    break;

  case 286: /* param_list: VAR_SCALAR '=' expr  */
#line 1595 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5922 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1596 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5928 "raku.tab.c"
    break;

  case 288: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1597 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 5934 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1598 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 5940 "raku.tab.c"
    break;

  case 290: /* param_list: SLURPY_POS  */
#line 1599 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5946 "raku.tab.c"
    break;

  case 291: /* param_list: param_list ',' SLURPY_POS  */
#line 1600 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5952 "raku.tab.c"
    break;

  case 292: /* param_list: SLURPY_LOL  */
#line 1601 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5958 "raku.tab.c"
    break;

  case 293: /* param_list: param_list ',' SLURPY_LOL  */
#line 1602 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5964 "raku.tab.c"
    break;

  case 294: /* param_list: SLURPY_NAMED  */
#line 1603 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5970 "raku.tab.c"
    break;

  case 295: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1604 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5976 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list '}'  */
#line 1607 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5982 "raku.tab.c"
    break;

  case 297: /* block: '{' YADA '}'  */
#line 1608 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5988 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr '}'  */
#line 1610 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5994 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1612 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6000 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1614 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6006 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1616 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6012 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1618 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6018 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1620 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6025 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1623 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6031 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1625 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6037 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1627 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6043 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1629 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 6049 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1631 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6056 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1634 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6063 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1637 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6071 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1641 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6078 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1644 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6085 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1647 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6092 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1650 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 6098 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1652 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6105 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1655 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6112 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1658 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6119 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1661 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6126 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1664 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6133 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1667 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 6139 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1669 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6145 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1671 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6151 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_LAST '}'  */
#line 1673 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6157 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1675 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6163 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1677 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6169 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1679 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6175 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1681 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6181 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1683 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6187 "raku.tab.c"
    break;

  case 329: /* closure: '{' expr '}'  */
#line 1686 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6193 "raku.tab.c"
    break;

  case 330: /* expr: VAR_SCALAR '=' expr  */
#line 1689 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6199 "raku.tab.c"
    break;

  case 331: /* expr: VAR_ARRAY '=' expr  */
#line 1690 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6205 "raku.tab.c"
    break;

  case 332: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1691 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6211 "raku.tab.c"
    break;

  case 333: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1692 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6217 "raku.tab.c"
    break;

  case 334: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1693 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6223 "raku.tab.c"
    break;

  case 335: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1694 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6229 "raku.tab.c"
    break;

  case 336: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1695 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6235 "raku.tab.c"
    break;

  case 337: /* expr: KW_GATHER block  */
#line 1696 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6245 "raku.tab.c"
    break;

  case 338: /* expr: KW_GATHER for_stmt  */
#line 1701 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6255 "raku.tab.c"
    break;

  case 339: /* expr: tern_expr OP_FATARROW expr  */
#line 1707 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6261 "raku.tab.c"
    break;

  case 340: /* expr: tern_expr  */
#line 1708 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6267 "raku.tab.c"
    break;

  case 341: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1712 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6273 "raku.tab.c"
    break;

  case 342: /* tern_expr: or_expr  */
#line 1713 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6279 "raku.tab.c"
    break;

  case 343: /* or_expr: or_expr OP_OR and_expr  */
#line 1716 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6285 "raku.tab.c"
    break;

  case 344: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1717 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6291 "raku.tab.c"
    break;

  case 345: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1718 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6297 "raku.tab.c"
    break;

  case 346: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1719 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6303 "raku.tab.c"
    break;

  case 347: /* or_expr: and_expr  */
#line 1720 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6309 "raku.tab.c"
    break;

  case 348: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1723 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6315 "raku.tab.c"
    break;

  case 349: /* and_expr: cmp_expr  */
#line 1724 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6321 "raku.tab.c"
    break;

  case 350: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1727 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 6327 "raku.tab.c"
    break;

  case 351: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1728 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6333 "raku.tab.c"
    break;

  case 352: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1729 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6339 "raku.tab.c"
    break;

  case 353: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1730 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6345 "raku.tab.c"
    break;

  case 354: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1731 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6351 "raku.tab.c"
    break;

  case 355: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1732 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6357 "raku.tab.c"
    break;

  case 356: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1733 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6363 "raku.tab.c"
    break;

  case 357: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1734 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6369 "raku.tab.c"
    break;

  case 358: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1735 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6375 "raku.tab.c"
    break;

  case 359: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1736 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 6381 "raku.tab.c"
    break;

  case 360: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1737 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 6387 "raku.tab.c"
    break;

  case 361: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1738 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 6393 "raku.tab.c"
    break;

  case 362: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1739 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 6399 "raku.tab.c"
    break;

  case 363: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1740 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 6405 "raku.tab.c"
    break;

  case 364: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1741 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6411 "raku.tab.c"
    break;

  case 365: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1742 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6417 "raku.tab.c"
    break;

  case 366: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1743 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6423 "raku.tab.c"
    break;

  case 367: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1744 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6429 "raku.tab.c"
    break;

  case 368: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1745 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6435 "raku.tab.c"
    break;

  case 369: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1746 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6441 "raku.tab.c"
    break;

  case 370: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1747 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6447 "raku.tab.c"
    break;

  case 371: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1748 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6453 "raku.tab.c"
    break;

  case 372: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1749 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6459 "raku.tab.c"
    break;

  case 373: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1751 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 6469 "raku.tab.c"
    break;

  case 374: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1757 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 6479 "raku.tab.c"
    break;

  case 375: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1763 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 6489 "raku.tab.c"
    break;

  case 376: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1769 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 6499 "raku.tab.c"
    break;

  case 377: /* cmp_expr: divis_expr  */
#line 1774 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 6505 "raku.tab.c"
    break;

  case 378: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1777 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6511 "raku.tab.c"
    break;

  case 379: /* divis_expr: jct_expr  */
#line 1778 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6517 "raku.tab.c"
    break;

  case 380: /* jct_expr: jct_expr '|' range_expr  */
#line 1781 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6523 "raku.tab.c"
    break;

  case 381: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1782 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6529 "raku.tab.c"
    break;

  case 382: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1783 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6535 "raku.tab.c"
    break;

  case 383: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1784 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6541 "raku.tab.c"
    break;

  case 384: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1785 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6547 "raku.tab.c"
    break;

  case 385: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1786 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6553 "raku.tab.c"
    break;

  case 386: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1787 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6559 "raku.tab.c"
    break;

  case 387: /* jct_expr: jct_expr '&' range_expr  */
#line 1788 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6565 "raku.tab.c"
    break;

  case 388: /* jct_expr: dor_expr  */
#line 1789 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6571 "raku.tab.c"
    break;

  case 389: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1793 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6577 "raku.tab.c"
    break;

  case 390: /* dor_expr: range_expr  */
#line 1794 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6583 "raku.tab.c"
    break;

  case 391: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1797 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6589 "raku.tab.c"
    break;

  case 392: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 1798 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6595 "raku.tab.c"
    break;

  case 393: /* range_expr: range_expr OP_COLL add_expr  */
#line 1799 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6601 "raku.tab.c"
    break;

  case 394: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 1800 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6607 "raku.tab.c"
    break;

  case 395: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 1801 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6613 "raku.tab.c"
    break;

  case 396: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1802 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6619 "raku.tab.c"
    break;

  case 397: /* range_expr: add_expr  */
#line 1803 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6625 "raku.tab.c"
    break;

  case 398: /* add_expr: add_expr '~' repl_expr  */
#line 1806 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6631 "raku.tab.c"
    break;

  case 399: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 1807 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6637 "raku.tab.c"
    break;

  case 400: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 1808 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6643 "raku.tab.c"
    break;

  case 401: /* add_expr: repl_expr  */
#line 1809 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6649 "raku.tab.c"
    break;

  case 402: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1812 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6655 "raku.tab.c"
    break;

  case 403: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1813 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6661 "raku.tab.c"
    break;

  case 404: /* repl_expr: addsub_expr  */
#line 1814 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6667 "raku.tab.c"
    break;

  case 405: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1817 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6673 "raku.tab.c"
    break;

  case 406: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 1818 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6679 "raku.tab.c"
    break;

  case 407: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 1819 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6685 "raku.tab.c"
    break;

  case 408: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 1820 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6691 "raku.tab.c"
    break;

  case 409: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 1821 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6697 "raku.tab.c"
    break;

  case 410: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 1822 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6703 "raku.tab.c"
    break;

  case 411: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1823 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6709 "raku.tab.c"
    break;

  case 412: /* addsub_expr: mul_expr  */
#line 1824 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6715 "raku.tab.c"
    break;

  case 413: /* mul_expr: mul_expr '*' unary_expr  */
#line 1827 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6721 "raku.tab.c"
    break;

  case 414: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 1828 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6727 "raku.tab.c"
    break;

  case 415: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 1829 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6733 "raku.tab.c"
    break;

  case 416: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 1830 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6739 "raku.tab.c"
    break;

  case 417: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 1831 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6745 "raku.tab.c"
    break;

  case 418: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 1832 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6751 "raku.tab.c"
    break;

  case 419: /* mul_expr: mul_expr '/' unary_expr  */
#line 1833 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6757 "raku.tab.c"
    break;

  case 420: /* mul_expr: mul_expr '%' unary_expr  */
#line 1834 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6763 "raku.tab.c"
    break;

  case 421: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1836 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6769 "raku.tab.c"
    break;

  case 422: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 1838 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6775 "raku.tab.c"
    break;

  case 423: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1840 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6781 "raku.tab.c"
    break;

  case 424: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1842 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6787 "raku.tab.c"
    break;

  case 425: /* mul_expr: unary_expr  */
#line 1843 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6793 "raku.tab.c"
    break;

  case 426: /* unary_expr: '-' unary_expr  */
#line 1846 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6799 "raku.tab.c"
    break;

  case 427: /* unary_expr: '+' unary_expr  */
#line 1847 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6805 "raku.tab.c"
    break;

  case 428: /* unary_expr: '!' unary_expr  */
#line 1848 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6811 "raku.tab.c"
    break;

  case 429: /* unary_expr: CARET unary_expr  */
#line 1849 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6817 "raku.tab.c"
    break;

  case 430: /* unary_expr: OP_REDUCE unary_expr  */
#line 1851 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6826 "raku.tab.c"
    break;

  case 431: /* unary_expr: pow_expr  */
#line 1855 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6832 "raku.tab.c"
    break;

  case 432: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1858 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6838 "raku.tab.c"
    break;

  case 433: /* pow_expr: postfix_expr  */
#line 1859 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6844 "raku.tab.c"
    break;

  case 434: /* scalar_list: VAR_SCALAR  */
#line 1862 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 6850 "raku.tab.c"
    break;

  case 435: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1863 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 6856 "raku.tab.c"
    break;

  case 436: /* meth_name: IDENT  */
#line 1866 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6862 "raku.tab.c"
    break;

  case 437: /* meth_name: KW_SORT  */
#line 1867 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 6868 "raku.tab.c"
    break;

  case 438: /* meth_name: KW_REVERSE  */
#line 1868 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 6874 "raku.tab.c"
    break;

  case 439: /* meth_name: KW_MAP  */
#line 1869 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 6880 "raku.tab.c"
    break;

  case 440: /* meth_name: KW_GREP  */
#line 1870 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 6886 "raku.tab.c"
    break;

  case 441: /* meth_name: KW_SAY  */
#line 1871 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 6892 "raku.tab.c"
    break;

  case 442: /* meth_name: KW_PRINT  */
#line 1872 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 6898 "raku.tab.c"
    break;

  case 443: /* meth_name: KW_TAKE  */
#line 1873 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 6904 "raku.tab.c"
    break;

  case 444: /* meth_name: KW_RETURN  */
#line 1874 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 6910 "raku.tab.c"
    break;

  case 445: /* meth_name: KW_EXISTS  */
#line 1875 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 6916 "raku.tab.c"
    break;

  case 446: /* meth_name: KW_DELETE  */
#line 1876 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 6922 "raku.tab.c"
    break;

  case 447: /* meth_name: KW_JOIN  */
#line 1877 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 6928 "raku.tab.c"
    break;

  case 448: /* meth_name: TESTOP  */
#line 1878 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6934 "raku.tab.c"
    break;

  case 449: /* postfix_expr: call_expr  */
#line 1880 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6940 "raku.tab.c"
    break;

  case 450: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1883 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6950 "raku.tab.c"
    break;

  case 451: /* call_expr: IDENT '(' arg_list ')'  */
#line 1889 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6959 "raku.tab.c"
    break;

  case 452: /* call_expr: IDENT '(' ')'  */
#line 1893 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6965 "raku.tab.c"
    break;

  case 453: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1895 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 6971 "raku.tab.c"
    break;

  case 454: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1897 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 6977 "raku.tab.c"
    break;

  case 455: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1899 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6986 "raku.tab.c"
    break;

  case 456: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1904 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6992 "raku.tab.c"
    break;

  case 457: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1906 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7005 "raku.tab.c"
    break;

  case 458: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1915 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7018 "raku.tab.c"
    break;

  case 459: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1924 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7029 "raku.tab.c"
    break;

  case 460: /* call_expr: IDENT '.' KW_NEW  */
#line 1931 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7037 "raku.tab.c"
    break;

  case 461: /* call_expr: IDENT '.' IDENT  */
#line 1935 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7046 "raku.tab.c"
    break;

  case 462: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1940 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7057 "raku.tab.c"
    break;

  case 463: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1947 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7066 "raku.tab.c"
    break;

  case 464: /* call_expr: IDENT '.' CARET IDENT  */
#line 1952 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7076 "raku.tab.c"
    break;

  case 465: /* call_expr: atom '.' CARET IDENT  */
#line 1958 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7086 "raku.tab.c"
    break;

  case 466: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1964 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7097 "raku.tab.c"
    break;

  case 467: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1971 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7108 "raku.tab.c"
    break;

  case 468: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1978 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7117 "raku.tab.c"
    break;

  case 469: /* call_expr: atom '.' meth_name  */
#line 1983 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7126 "raku.tab.c"
    break;

  case 470: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1988 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7137 "raku.tab.c"
    break;

  case 471: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1995 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7146 "raku.tab.c"
    break;

  case 472: /* call_expr: call_expr '.' meth_name  */
#line 2000 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7155 "raku.tab.c"
    break;

  case 473: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2005 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7166 "raku.tab.c"
    break;

  case 474: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2012 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7177 "raku.tab.c"
    break;

  case 475: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2019 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7188 "raku.tab.c"
    break;

  case 476: /* call_expr: '.' meth_name '(' ')'  */
#line 2026 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7197 "raku.tab.c"
    break;

  case 477: /* call_expr: '.' meth_name  */
#line 2031 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7206 "raku.tab.c"
    break;

  case 478: /* call_expr: KW_DIE expr  */
#line 2036 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7212 "raku.tab.c"
    break;

  case 479: /* call_expr: KW_MAP closure expr  */
#line 2038 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7218 "raku.tab.c"
    break;

  case 480: /* call_expr: KW_MAP closure ',' expr  */
#line 2040 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7224 "raku.tab.c"
    break;

  case 481: /* call_expr: KW_GREP closure expr  */
#line 2042 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7230 "raku.tab.c"
    break;

  case 482: /* call_expr: KW_SORT expr  */
#line 2044 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7236 "raku.tab.c"
    break;

  case 483: /* call_expr: KW_SORT closure expr  */
#line 2046 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7242 "raku.tab.c"
    break;

  case 484: /* call_expr: KW_REVERSE expr  */
#line 2048 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7248 "raku.tab.c"
    break;

  case 485: /* call_expr: atom  */
#line 2049 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7254 "raku.tab.c"
    break;

  case 486: /* arg_list: expr  */
#line 2052 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7260 "raku.tab.c"
    break;

  case 487: /* arg_list: arg_list ',' expr  */
#line 2053 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7266 "raku.tab.c"
    break;

  case 488: /* arg_list: arg_list ','  */
#line 2054 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7272 "raku.tab.c"
    break;

  case 489: /* paren_group: '(' ')'  */
#line 2057 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7278 "raku.tab.c"
    break;

  case 490: /* paren_group: '(' expr ')'  */
#line 2058 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7284 "raku.tab.c"
    break;

  case 491: /* paren_group: '(' expr ',' ')'  */
#line 2060 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7290 "raku.tab.c"
    break;

  case 492: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2062 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7297 "raku.tab.c"
    break;

  case 493: /* atom: LIT_INT  */
#line 2066 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7303 "raku.tab.c"
    break;

  case 494: /* atom: LIT_BOOL  */
#line 2068 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7311 "raku.tab.c"
    break;

  case 495: /* atom: LIT_FLOAT  */
#line 2071 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7317 "raku.tab.c"
    break;

  case 496: /* atom: LIT_STR  */
#line 2072 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7323 "raku.tab.c"
    break;

  case 497: /* atom: WORDLIST  */
#line 2074 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7336 "raku.tab.c"
    break;

  case 498: /* atom: LIT_INTERP_STR  */
#line 2082 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7342 "raku.tab.c"
    break;

  case 499: /* atom: VAR_SCALAR  */
#line 2083 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7348 "raku.tab.c"
    break;

  case 500: /* atom: OP_INC VAR_SCALAR  */
#line 2084 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7354 "raku.tab.c"
    break;

  case 501: /* atom: OP_DEC VAR_SCALAR  */
#line 2085 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7360 "raku.tab.c"
    break;

  case 502: /* atom: VAR_SCALAR OP_INC  */
#line 2086 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7366 "raku.tab.c"
    break;

  case 503: /* atom: VAR_SCALAR OP_DEC  */
#line 2087 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7372 "raku.tab.c"
    break;

  case 504: /* atom: VAR_TWIGIL OP_INC  */
#line 2088 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 7378 "raku.tab.c"
    break;

  case 505: /* atom: VAR_TWIGIL OP_DEC  */
#line 2089 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 7384 "raku.tab.c"
    break;

  case 506: /* atom: VAR_ARRAY  */
#line 2090 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7390 "raku.tab.c"
    break;

  case 507: /* atom: VAR_HASH  */
#line 2091 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7396 "raku.tab.c"
    break;

  case 508: /* atom: VAR_CAPTURE  */
#line 2093 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7404 "raku.tab.c"
    break;

  case 509: /* atom: VAR_FH  */
#line 2097 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7412 "raku.tab.c"
    break;

  case 510: /* atom: VAR_NAMED_CAPTURE  */
#line 2101 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 7419 "raku.tab.c"
    break;

  case 511: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2104 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 7425 "raku.tab.c"
    break;

  case 512: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2106 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 7431 "raku.tab.c"
    break;

  case 513: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2108 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 7437 "raku.tab.c"
    break;

  case 514: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2110 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 7443 "raku.tab.c"
    break;

  case 515: /* atom: ARR_ALL_SLICE  */
#line 2112 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 7449 "raku.tab.c"
    break;

  case 516: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2114 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7455 "raku.tab.c"
    break;

  case 517: /* atom: VAR_HASH '{' expr '}'  */
#line 2116 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7461 "raku.tab.c"
    break;

  case 518: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2118 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7467 "raku.tab.c"
    break;

  case 519: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2120 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7473 "raku.tab.c"
    break;

  case 520: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2122 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7479 "raku.tab.c"
    break;

  case 521: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2124 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7485 "raku.tab.c"
    break;

  case 522: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2126 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7491 "raku.tab.c"
    break;

  case 523: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2128 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7497 "raku.tab.c"
    break;

  case 524: /* atom: IDENT  */
#line 2129 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7503 "raku.tab.c"
    break;

  case 525: /* atom: VAR_TWIGIL  */
#line 2131 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7511 "raku.tab.c"
    break;

  case 526: /* atom: VAR_ARRAY_TWIGIL  */
#line 2135 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7519 "raku.tab.c"
    break;

  case 527: /* atom: VAR_HASH_TWIGIL  */
#line 2139 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7527 "raku.tab.c"
    break;

  case 528: /* atom: '[' ']'  */
#line 2142 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 7533 "raku.tab.c"
    break;

  case 529: /* atom: '[' expr ']'  */
#line 2144 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7539 "raku.tab.c"
    break;

  case 530: /* atom: '[' expr ',' ']'  */
#line 2146 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7545 "raku.tab.c"
    break;

  case 531: /* atom: '[' expr ',' arg_list ']'  */
#line 2148 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7552 "raku.tab.c"
    break;

  case 532: /* atom: DOLLAR_LBRACKET ']'  */
#line 2150 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 7558 "raku.tab.c"
    break;

  case 533: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2152 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7564 "raku.tab.c"
    break;

  case 534: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2154 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7570 "raku.tab.c"
    break;

  case 535: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2156 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7577 "raku.tab.c"
    break;

  case 536: /* atom: paren_group  */
#line 2158 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 7583 "raku.tab.c"
    break;

  case 537: /* atom: block  */
#line 2159 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7589 "raku.tab.c"
    break;

  case 538: /* atom: KW_SUB block  */
#line 2160 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7595 "raku.tab.c"
    break;

  case 539: /* atom: KW_SUB '(' param_list ')' block  */
#line 2162 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 7602 "raku.tab.c"
    break;

  case 540: /* atom: OP_ARROW scalar_list block  */
#line 2165 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 7609 "raku.tab.c"
    break;


#line 7613 "raku.tab.c"

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

#line 2168 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
