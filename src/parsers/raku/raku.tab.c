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

#line 546 "raku.tab.c"

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
  YYSYMBOL_OP_DIVIS = 122,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 123,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 124,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 125,                   /* OP_POW  */
  YYSYMBOL_126_ = 126,                     /* '='  */
  YYSYMBOL_127_ = 127,                     /* '!'  */
  YYSYMBOL_128_ = 128,                     /* '<'  */
  YYSYMBOL_129_ = 129,                     /* '>'  */
  YYSYMBOL_130_ = 130,                     /* '|'  */
  YYSYMBOL_131_ = 131,                     /* '&'  */
  YYSYMBOL_132_ = 132,                     /* '~'  */
  YYSYMBOL_133_ = 133,                     /* '+'  */
  YYSYMBOL_134_ = 134,                     /* '-'  */
  YYSYMBOL_135_ = 135,                     /* '*'  */
  YYSYMBOL_136_ = 136,                     /* '/'  */
  YYSYMBOL_137_ = 137,                     /* '%'  */
  YYSYMBOL_UMINUS = 138,                   /* UMINUS  */
  YYSYMBOL_139_ = 139,                     /* '.'  */
  YYSYMBOL_140_ = 140,                     /* ';'  */
  YYSYMBOL_141_ = 141,                     /* '('  */
  YYSYMBOL_142_ = 142,                     /* ')'  */
  YYSYMBOL_143_ = 143,                     /* ','  */
  YYSYMBOL_144_ = 144,                     /* '['  */
  YYSYMBOL_145_ = 145,                     /* ']'  */
  YYSYMBOL_146_ = 146,                     /* '{'  */
  YYSYMBOL_147_ = 147,                     /* '}'  */
  YYSYMBOL_148_ = 148,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 149,                 /* $accept  */
  YYSYMBOL_program = 150,                  /* program  */
  YYSYMBOL_stmt_list = 151,                /* stmt_list  */
  YYSYMBOL_stmt = 152,                     /* stmt  */
  YYSYMBOL_if_stmt = 153,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 154,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 155,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 156,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 157,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 158,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 159,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 160,                /* loop_incr  */
  YYSYMBOL_for_stmt = 161,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 162,               /* given_stmt  */
  YYSYMBOL_catch_when_list = 163,          /* catch_when_list  */
  YYSYMBOL_when_list = 164,                /* when_list  */
  YYSYMBOL_sub_decl = 165,                 /* sub_decl  */
  YYSYMBOL_sub_body = 166,                 /* sub_body  */
  YYSYMBOL_method_body = 167,              /* method_body  */
  YYSYMBOL_pkg_name = 168,                 /* pkg_name  */
  YYSYMBOL_class_decl = 169,               /* class_decl  */
  YYSYMBOL_role_decl = 170,                /* role_decl  */
  YYSYMBOL_module_decl = 171,              /* module_decl  */
  YYSYMBOL_is_clauses = 172,               /* is_clauses  */
  YYSYMBOL_class_body_list = 173,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 174,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 175,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 176,           /* named_arg_list  */
  YYSYMBOL_pair_list = 177,                /* pair_list  */
  YYSYMBOL_param_list = 178,               /* param_list  */
  YYSYMBOL_block = 179,                    /* block  */
  YYSYMBOL_closure = 180,                  /* closure  */
  YYSYMBOL_expr = 181,                     /* expr  */
  YYSYMBOL_tern_expr = 182,                /* tern_expr  */
  YYSYMBOL_or_expr = 183,                  /* or_expr  */
  YYSYMBOL_and_expr = 184,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 185,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 186,               /* divis_expr  */
  YYSYMBOL_jct_expr = 187,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 188,                 /* dor_expr  */
  YYSYMBOL_range_expr = 189,               /* range_expr  */
  YYSYMBOL_add_expr = 190,                 /* add_expr  */
  YYSYMBOL_repl_expr = 191,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 192,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 193,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 194,               /* unary_expr  */
  YYSYMBOL_pow_expr = 195,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 196,              /* scalar_list  */
  YYSYMBOL_meth_name = 197,                /* meth_name  */
  YYSYMBOL_postfix_expr = 198,             /* postfix_expr  */
  YYSYMBOL_call_expr = 199,                /* call_expr  */
  YYSYMBOL_arg_list = 200,                 /* arg_list  */
  YYSYMBOL_atom = 201                      /* atom  */
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
#define YYLAST   8276

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  149
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  494
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1233

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   381


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
       2,     2,     2,   127,     2,     2,     2,   137,   131,     2,
     141,   142,   135,   133,   143,   134,   139,   136,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   148,   140,
     128,   126,   129,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   144,     2,   145,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   146,   130,   147,   132,     2,     2,     2,
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
     125,   138
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   568,   568,   579,   580,   583,   585,   587,   589,   593,
     595,   597,   599,   601,   605,   609,   611,   615,   619,   621,
     623,   625,   627,   629,   633,   637,   639,   641,   643,   645,
     647,   649,   651,   653,   655,   657,   665,   667,   669,   671,
     673,   675,   677,   680,   683,   685,   688,   691,   693,   697,
     699,   701,   703,   705,   708,   711,   713,   716,   719,   722,
     724,   729,   733,   737,   740,   743,   747,   750,   753,   756,
     759,   762,   764,   766,   768,   770,   773,   775,   777,   779,
     782,   785,   789,   792,   794,   796,   798,   801,   804,   807,
     810,   813,   816,   819,   820,   821,   822,   823,   824,   825,
     827,   829,   833,   838,   842,   844,   846,   847,   848,   849,
     850,   851,   852,   854,   856,   858,   860,   861,   862,   863,
     864,   867,   869,   871,   873,   875,   877,   879,   881,   885,
     887,   889,   891,   893,   895,   899,   901,   905,   907,   909,
     911,   915,   917,   921,   923,   925,   929,   931,   933,   937,
     940,   946,   952,   960,   966,   971,   976,   984,   995,   997,
    1001,  1002,  1008,  1016,  1022,  1028,  1036,  1042,  1048,  1057,
    1064,  1073,  1082,  1091,  1092,  1095,  1098,  1100,  1102,  1104,
    1106,  1108,  1110,  1112,  1115,  1117,  1119,  1121,  1124,  1127,
    1133,  1134,  1135,  1138,  1141,  1144,  1146,  1148,  1150,  1152,
    1154,  1156,  1158,  1161,  1163,  1165,  1167,  1170,  1173,  1179,
    1182,  1198,  1213,  1228,  1229,  1241,  1255,  1256,  1259,  1262,
    1265,  1268,  1271,  1274,  1277,  1280,  1283,  1286,  1289,  1292,
    1299,  1305,  1311,  1318,  1325,  1332,  1336,  1340,  1344,  1348,
    1355,  1362,  1369,  1376,  1386,  1394,  1402,  1411,  1418,  1425,
    1435,  1443,  1453,  1468,  1469,  1474,  1479,  1486,  1490,  1494,
    1498,  1504,  1506,  1508,  1510,  1514,  1515,  1516,  1517,  1518,
    1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,
    1529,  1530,  1531,  1532,  1533,  1534,  1535,  1538,  1539,  1540,
    1542,  1544,  1546,  1548,  1550,  1553,  1555,  1557,  1559,  1561,
    1564,  1567,  1571,  1574,  1577,  1580,  1582,  1585,  1588,  1591,
    1594,  1597,  1599,  1601,  1603,  1605,  1607,  1609,  1611,  1613,
    1617,  1620,  1621,  1622,  1623,  1624,  1625,  1626,  1627,  1632,
    1637,  1639,  1642,  1644,  1647,  1648,  1651,  1652,  1655,  1656,
    1657,  1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,
    1667,  1668,  1669,  1670,  1676,  1682,  1688,  1694,  1697,  1698,
    1701,  1702,  1703,  1706,  1708,  1711,  1712,  1713,  1716,  1717,
    1720,  1721,  1722,  1725,  1726,  1727,  1730,  1731,  1732,  1733,
    1735,  1737,  1739,  1742,  1743,  1744,  1745,  1746,  1751,  1754,
    1755,  1758,  1759,  1762,  1763,  1764,  1765,  1766,  1767,  1768,
    1769,  1770,  1771,  1772,  1773,  1774,  1776,  1778,  1784,  1789,
    1790,  1792,  1794,  1799,  1801,  1810,  1819,  1826,  1830,  1835,
    1842,  1847,  1853,  1859,  1866,  1873,  1878,  1883,  1890,  1895,
    1900,  1907,  1912,  1917,  1919,  1921,  1923,  1925,  1927,  1929,
    1931,  1934,  1935,  1936,  1939,  1940,  1944,  1945,  1946,  1955,
    1956,  1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,
    1969,  1973,  1976,  1978,  1980,  1982,  1984,  1986,  1988,  1990,
    1992,  1994,  1996,  1998,  2000,  2002,  2003,  2007,  2011,  2015,
    2016,  2018,  2020,  2023,  2024,  2026,  2028,  2031,  2032,  2033,
    2035,  2038,  2039,  2040,  2043
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
  "OP_SHL", "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'",
  "':'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "elsif_tail", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "loop_stmt", "loop_incr", "for_stmt", "given_stmt", "catch_when_list",
  "when_list", "sub_decl", "sub_body", "method_body", "pkg_name",
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

#define YYPACT_NINF (-930)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-492)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -930,    76,  2802,  -930,  -930,  -930,  -930,  -930,  -930,   792,
     -64,   -67,    89,    61,  -930,  -930,  8130,  2946,  -930,  -930,
    -930,    49,  3090,    91,  6258,  6402,  6546,  6690,  6834,    41,
     -24,  6834,  3234,  3378,   506,   152,  6834,  6834,   171,   177,
    6978,  7122,    92,   191,   120,   178,   116,   116,  7266,  6834,
      92,   126,  6834,  3522,   276,   276,   181,  8130,  -930,  -930,
     276,   276,   293,   409,   415,  8130,  8130,  8130,  1031,  -930,
    3666,  3810,   353,  -930,  -930,  -930,  -930,  -930,  -930,  -930,
    -930,  -930,  -930,  -930,  -930,  -930,  -930,   757,   673,   420,
     359,   386,   497,  1004,   454,   421,  -930,   -32,   634,   660,
     557,  -930,  -930,   449,   462,   507,   578,  -930,  -930,  6834,
    6834,  6834,  6834,  6834,  6834,  3954,  7410,  4098,   666,  6834,
    -930,  -930,  6834,   558,   279,  1198,   239,   563,    93,   774,
     230,   436,  4242,  -930,  -930,   577,   614,   891,   105,  -930,
     403,  4386,  -930,  4530,  -930,   260,   205,   289,   296,   616,
     712,   626,   293,  4242,   591,  4242,    77,  4242,    92,  4242,
      92,   -30,   169,   650,   434,  -930,  -930,  -930,   391,  -930,
     236,  -930,   241,   649,   655,   747,   661,   716,   133,   160,
    4242,    92,  4242,    92,   285,  4674,  -930,  6834,  6834,  -930,
    6834,  6834,  -930,  6834,  4818,  6834,  4962,  6834,  -930,  -930,
     811,   332,  -930,  -930,  -930,   735,  -930,  -930,   786,   759,
      30,  -930,   801,   813,  -930,   515,  -930,  -930,  -930,  -930,
    -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,
    -930,  -930,  -930,  -930,   797,   815,  -930,   748,   754,  -930,
     472,   817,  1787,  6834,  6834,  6834,  6834,  6834,  6834,  6834,
    6834,  -930,  6834,  8130,  8130,  8130,  8130,  8130,  8130,  8130,
    8130,  8130,  8130,  8130,  8130,  8130,  8130,  8130,  8130,  8130,
    8130,   635,  8130,  8130,  8130,  8130,  8130,  8130,  8130,  8130,
    8130,  8130,  8130,  8130,  8130,  8130,  8130,  8130,  8130,  8130,
    1031,  1007,   768,   849,   888,   890,   899,   901,   904,  -930,
     809,  4242,   598,   841,   694,   856,   877,   911,  -930,   915,
    1050,   929,   144,  -930,  1061,   834,   836,  4098,  1080,  6834,
    1031,  1007,  6834,  6834,  6834,  6834,  6834,  6834,  6834,  5106,
    -930,  -930,   999,  1009,   748,   838,  -930,  6834,  6834,  -930,
    7554,  -930,  7698,  -930,   327,   345,   364,   662,   909,   996,
     845,   850,  6834,  6834,  6834,  6834,  6834,  6834,  6834,  -930,
    6834,   852,  6834,  6834,  -930,  6834,   855,   982,   871,  -930,
     293,  6834,  -930,  8130,  8130,    21,  -930,  -930,    -5,  -930,
       7,  -930,  -930,  -930,   873,  -930,  6834,  6834,  6834,  -930,
    6834,  6834,  -930,  6834,  6834,  1011,  6834,  -930,  1096,  6834,
    1118,  6834,   876,  1141,   878,  -930,  6834,  6834,  1164,  1046,
    1047,  1048,  1049,  1051,  1043,  6834,  -930,  -930,  -930,    92,
    6834,    92,   -15,  -930,    -4,  -930,    63,  1052,  1054,  -930,
    -930,  1181,  -930,  5250,  -930,  5394,  1068,  5538,  -930,  -930,
     220,   284,   297,  6258,  6402,  6834,  2367,    90,   319,  -930,
     226,  1057,  1058,  1066,  1067,  1081,  1082,  1083,  1084,  1085,
    1086,  -930,   386,  1103,   497,  1105,  1105,  1105,  1105,  1105,
    1105,  1105,  1105,  1105,  1105,  1105,  1105,  1105,  1105,  1105,
    -930,  -930,  -930,  -930,   454,  -930,  -930,  -930,  1097,  1097,
     634,   660,   660,   557,   557,  -930,  -930,  -930,  -930,  -930,
    -930,  -930,   -49,  1213,   216,  -930,  5682,  -930,  -930,  -930,
    -930,  -930,  -930,  -930,   884,  -930,  6834,  6834,  6834,  6834,
    1104,   392,   426,  -930,  5826,  -930,  1344,  6834,  1092,  -930,
      -8,  -930,  1641,   799,  1106,  1087,  1098,  1099,  -930,  -930,
    -930,  -930,  -930,  -930,  -930,  -930,   803,  -930,  -930,  1101,
    -930,  1102,  4242,   698,  1151,   294,  5970,   723,  1107,  6834,
    -930,  7842,  -930,  6834,  -930,    71,  -930,  6834,  6834,  1111,
    5394,  1108,  1109,  1110,  1116,  1117,  1119,  1120,   724,  5394,
    1122,  1123,   728,    92,    25,  7986,  -930,    92,   515,    31,
     -44,   -36,  1100,   895,  1932,  1228,  6834,  1125,  1234,  1250,
      92,   469,   729,  1127,  1133,  1135,  1136,  1137,  1138,  -930,
    1139,    17,  1150,  1134,  1154,  1140,    92,    92,    92,  1144,
    1145,  1162,  6834,  -930,  -930,  -930,  -930,  -930,  -930,  -930,
      92,  1142,  6834,    92,  -930,  1249,  1265,  -930,    64,  1100,
     907,    86,   434,   356,  2077,  -930,  -930,   926,  -930,   930,
    6834,  -930,   822,  4098,  6834,  6834,   424,     9,   -77,  -930,
     354,  6834,  6834,  -930,  6834,  6834,  -930,  6834,  6834,  6834,
    6834,  6834,  6834,  6834,  6834,  -930,  1031,  1007,  -930,  -930,
    -930,  -930,  -930,  -930,  -930,  -930,  8130,  6834,  6114,  -930,
    6834,  1495,  1152,   932,  5394,   755,  1146,  1149,   825,  6834,
    -930,  -930,  6834,  -930,  -930,  6834,  -930,   937,  -930,   939,
     941,  -930,  6834,  1180,  1275,   947,  -930,   973,   975,  -930,
    -930,  -930,   964,  -930,  6834,  6834,  6834,   971,  -930,   523,
    -930,  1155,  4242,   779,  1156,  1100,   974,  1158,  1159,  6834,
     976,  -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,   978,
    -930,  -930,  -930,  1095,  -930,  -930,  4242,    92,  -930,  -930,
    1289,  -930,  1290,  1291,  -930,  1100,  6258,  6402,  2512,  -930,
     429,  -930,  -930,  6834,  -930,  -930,  -930,    -3,  -930,    56,
    -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,
    6834,    92,  -930,  -930,  -930,  1163,  1165,  1277,  -930,  -930,
    -930,  -930,  6834,  1184,  -930,  -930,    92,  1157,  -930,  -930,
     223,   771,   805,  1263,  -930,  -930,  1100,  1100,   985,   987,
    1312,  1313,  1314,  -930,  -930,  -930,  -930,  1190,  -930,   826,
    1186,   155,  6834,  6834,  6834,  6834,  6834,  6834,  -930,  -930,
    -930,  -930,   218,   346,   393,   411,   428,   432,   450,   455,
     463,   471,   474,   500,   246,   252,  -930,  1194,  -930,   989,
    1195,  -930,   993,   995,  -930,  1196,  1001,  -930,  -930,  -930,
    -930,  1201,  1203,  1205,  -930,  -930,  -930,  1210,  6834,  1197,
    -930,  5394,   780,  -930,  -930,  1214,  1268,  1282,  -930,  1003,
    -930,  6834,  -930,  -930,  1100,  -930,  -930,   794,  1231,  1232,
      25,  -930,  1005,  1124,    92,    92,    92,  -930,   491,   153,
    -930,   362,  6834,  6834,  6834,  6834,  6834,  6834,  6834,  6834,
    -930,  -930,  1353,  6834,  1247,  1363,  1364,    92,  1229,  -930,
    -930,    92,  1239,  6834,  -930,  -930,  -930,   664,   683,   188,
     201,   798,  1240,  1241,  1031,  -930,  -930,  1100,  1100,  1374,
    1375,  1377,  -930,  1260,   437,  -930,   509,   517,   535,   539,
     540,   543,  -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,
    -930,  -930,  -930,  -930,  6834,  6834,  -930,  -930,  -930,  -930,
    -930,  -930,  1248,  -930,  -930,  -930,  -930,  -930,  6834,  1013,
    -930,  -930,  6834,  6834,  5394,   796,  -930,  -930,  6834,  -930,
    -930,  -930,  -930,    92,    92,  -930,  -930,  -930,  -930,  6834,
    6834,  6834,  -930,  -930,  -930,   544,   549,   559,   562,   572,
     573,   585,   586,  -930,  -930,  6834,  -930,  -930,  -930,  -930,
    -930,  6834,  1245,  -930,   135,  1310,  -930,   150,  -930,  1376,
    1378,  1252,  6834,  -930,  1379,  1380,  1256,  6834,  -930,   203,
     258,  1257,  1262,  -930,  -930,   699,  -930,  -930,  -930,  -930,
    -930,  6834,  6834,  -930,  -930,  -930,  -930,  -930,  -930,   588,
     589,  -930,  1261,  1266,  -930,  -930,  1016,  -930,   800,  1131,
    -930,   599,   600,   603,  -930,  -930,  -930,  -930,  -930,  -930,
    -930,  -930,  -930,  1267,    92,  1259,  1020,  1264,  2222,  1259,
    1022,  1269,  1270,  -930,  1272,   372,   388,  -930,  1273,  1393,
    1401,  1278,  6834,  -930,  1402,  1404,  1281,  6834,  -930,  -930,
    -930,   161,  -930,   604,   609,  -930,  -930,  -930,  -930,  1283,
    -930,    92,  -930,  -930,  -930,  -930,  6834,  -930,  -930,  1259,
    -930,   338,  6258,  6402,  2657,  -930,   451,  -930,  1259,  -930,
    -930,  -930,  6834,  -930,  6834,  -930,  -930,  1284,  1285,  -930,
    1286,  1287,  1288,  -930,  1293,  1259,  1024,  -930,  -930,  -930,
    -930,  1280,  -930,  6834,   568,   311,  -930,   373,  6834,  6834,
    6834,  6834,  6834,  6834,  6834,  6834,  -930,  -930,  1294,  1295,
    -930,  -930,  -930,  -930,  -930,  -930,  -930,  1259,    92,   612,
    6834,  6834,  6834,  -930,  -930,  -930,   615,   621,   623,   627,
     629,   632,   633,   637,  -930,  -930,  -930,  -930,  -930,   642,
     643,   652,  -930,  -930,  -930,  -930,  -930,  -930,  -930,  -930,
    -930,  -930,  -930
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   444,   445,   446,   447,   449,   450,
     457,   458,   476,   475,   477,   478,     0,     0,   459,   460,
     461,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   466,   448,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     3,     4,    95,    96,   106,   107,   108,   109,
      97,    98,   116,   117,   118,   120,   119,   105,     0,   331,
     333,   335,   337,   357,   359,   362,   364,   367,   369,   372,
     375,   382,   388,   390,   406,   440,     0,   453,   454,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     455,   456,     0,     0,     0,     0,   450,   457,   458,   476,
     475,     0,     0,   491,   386,   406,   440,   450,   457,   483,
       0,     0,    36,     0,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,   492,   329,   328,     0,    50,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,   146,     0,     0,   110,
       0,     0,   111,     0,     0,     0,     3,     0,   437,   439,
      99,     3,   104,   433,    52,     0,   209,   213,     0,     0,
       0,   387,     0,     0,   391,     0,   451,   452,   385,   384,
     383,   393,   405,   398,   399,   400,   401,   404,   402,   403,
     396,   397,   394,   395,   432,   450,   487,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,   326,   327,   321,   413,
       0,     0,   322,     0,     0,     0,     0,     0,    40,   418,
       0,   417,   475,   409,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     484,    29,     0,   487,     0,     0,    39,   443,     0,     6,
       0,    10,     0,    11,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
       0,     0,     0,     0,    44,     0,     0,   124,     0,   136,
       0,     0,   155,     0,     0,     0,     3,   164,   265,   268,
       0,   281,   283,   285,     0,    47,     0,     0,     0,    49,
       0,     0,    55,     0,     0,     0,     0,   160,     0,     0,
       0,     0,     0,   139,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   436,   438,     0,
       0,     0,     0,    51,     0,   216,     0,     0,     0,   253,
       3,     0,   494,     0,   488,     0,     0,     0,   480,   288,
     457,   458,   476,     0,     0,     0,     0,     0,     0,   287,
       0,   406,   440,     0,     0,     0,     0,     0,     0,     0,
       0,   330,   334,     0,   336,   338,   339,   342,   343,   340,
     341,   344,   348,   349,   350,   351,   352,   345,   346,   347,
     353,   354,   355,   356,   358,   360,   361,   363,   365,   366,
     368,   370,   371,   373,   374,   379,   380,   381,   376,   377,
     378,   389,   429,     0,   426,    62,     0,    88,    89,    90,
      91,    92,    59,   412,     0,    15,     0,     0,     0,     0,
     462,   467,   468,    65,     0,   421,     0,     0,     0,   410,
       0,   408,   443,     0,     0,     0,   429,   426,   323,   324,
     325,   326,   327,   321,   322,   485,     0,    30,    38,     0,
     442,     0,     0,     0,   447,   475,     0,     0,     0,     0,
      26,     0,    27,     0,    28,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,     0,   127,   488,     0,     0,
     365,   366,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
     407,     0,     0,     0,     0,     0,   488,     0,   488,     0,
       0,     0,     0,   112,   113,   114,   115,   320,   435,   100,
       0,     0,     0,     0,   101,     0,     0,   216,     0,     0,
       0,     0,     0,     0,     0,   392,   431,     0,   489,     0,
       0,   481,     0,     0,     0,     0,     0,     0,     0,   313,
       0,     0,     0,   314,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,   289,     0,     0,    71,    73,
      75,    78,    76,    77,    72,    74,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     472,   474,     0,   471,   473,     0,   420,     0,   416,     0,
       0,   257,     0,     0,     0,     0,   462,   467,   468,   486,
      37,     5,     0,    12,     0,     0,     0,     0,    19,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    82,    81,    85,    83,    84,    80,    42,     0,
      86,    87,    45,   121,   126,   125,     0,     0,   135,   152,
       0,   154,     0,     0,   163,     0,     0,     0,     0,   173,
       0,   266,   277,     0,   271,   272,   493,   273,   269,     0,
     282,   284,   286,    48,    53,    54,    57,    58,    32,    31,
       0,     0,   156,   469,   470,     0,     0,   137,   140,   141,
     144,   145,     0,     0,   158,   103,     0,     0,   214,   215,
       0,     0,     0,     0,   211,   172,     0,     0,     0,     0,
       0,     0,     0,   252,   212,   430,   490,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,   305,
     311,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   426,   332,     0,   428,     0,
       0,   425,     0,     0,    61,     0,     0,    16,   465,   464,
     463,     0,     0,     0,   419,   414,   415,     0,     0,     0,
     411,     0,     0,   262,   261,     0,     0,     0,    21,     0,
      22,     0,    25,   166,     0,    34,    33,     0,   490,   490,
       0,   128,     0,   129,     0,     0,     0,   162,     0,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     174,   279,     0,     0,   274,     0,     0,     0,     0,    69,
      70,     0,     0,     0,   159,   102,   210,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   169,     0,     0,     0,
       0,     0,     9,   462,   468,   308,     0,     0,     0,     0,
       0,     0,   316,   317,   318,   319,   290,   292,   294,   297,
     295,   296,   291,   293,     0,     0,    63,   427,    64,   424,
     423,    60,   490,    66,    67,    68,   258,   259,     0,     0,
      13,    20,     0,     0,     0,     0,   165,     7,     0,    43,
      46,   123,   122,   488,     0,   133,   153,   150,   151,     0,
       0,     0,   177,   178,   175,     0,     0,     0,     0,     0,
       0,     0,     0,   267,   278,     0,   275,   276,   161,   157,
     138,     0,     0,   149,     0,     3,   248,     0,   245,     0,
       0,     0,     0,   222,     0,     0,     0,     0,   217,     0,
       0,     0,     0,   218,   219,     0,   168,   170,   254,   255,
     256,     0,     0,   299,   301,   304,   302,   303,   300,     0,
       0,    17,     0,   490,   264,   263,     0,    23,     0,   130,
     131,     0,     0,     0,   179,   181,   183,   186,   184,   185,
     180,   182,   280,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,   223,   220,
     221,     0,   251,     0,     0,   306,   307,   260,    14,   490,
       8,     0,   134,   187,   189,   188,     0,   148,   247,     0,
     191,   476,     0,     0,     0,   190,     0,   244,     0,   233,
     234,   226,     0,   229,     0,   232,   225,     0,     0,   237,
       0,     0,     0,   238,     0,     0,     0,   309,   310,    24,
     132,     0,   246,     0,     0,     0,   195,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   193,   243,     0,     0,
     241,   242,   228,   239,   240,   227,   250,     0,     0,     0,
       0,     0,     0,   196,   197,   194,     0,     0,     0,     0,
       0,     0,     0,     0,   230,   231,   249,   147,   192,     0,
       0,     0,   198,   200,   202,   205,   203,   204,   199,   201,
     206,   208,   207
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -930,  -930,     1,  -930,  -581,  -749,  -930,  -930,  -930,  -930,
    -930,   295,  1399,  -930,  -930,  -930,  -930,  -341,  -929,   -12,
    -930,  -930,  -930,  -930,   793,  -930,  -930,  -516,   880,  -361,
     366,  1121,    -2,  -252,  -930,  1185,  1182,  1053,  1167,  -930,
     688,   -11,  1166,   894,   903,    24,  -930,   -65,  -249,  -930,
      10,    54,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   242,    73,    74,   586,    75,    76,    77,    78,
      79,  1032,    80,    81,   422,   611,    82,   377,  1036,   207,
      83,    84,    85,   424,   638,    86,   643,   315,   557,   384,
     133,   194,   144,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   215,   234,   103,
     135,   145,   136
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      88,     2,   463,   754,   901,   238,   566,   713,    28,  1038,
     709,   635,   104,   105,   593,   140,   715,   162,   597,   595,
     636,   922,   154,   156,   158,   160,   161,   632,   633,   168,
     170,   172,   378,   379,   176,   177,   380,   362,   181,   183,
     134,   502,   504,   208,   762,   427,   198,   199,   212,   213,
     203,   205,   763,    26,   276,   277,   163,   363,   370,   790,
     791,   118,   116,   385,   141,   640,   386,   924,   237,   240,
     840,   536,   537,   123,   378,   379,     3,   687,   380,   119,
     117,   211,   378,   379,   598,   599,   380,   350,   278,   218,
     219,   220,   688,   381,   382,   383,   278,   378,   379,   428,
     278,   380,   146,   147,   148,   362,   149,   293,   294,   295,
     296,   297,   298,   371,   302,   304,    72,   306,   661,   760,
     307,   596,    72,   923,   150,   363,  1122,   811,   812,   151,
     237,   813,   634,   925,   926,   381,   382,   383,   662,   332,
     714,   334,   637,   381,   382,   383,   378,   379,   187,   364,
     380,   351,   365,   361,  1005,   366,   839,   368,   381,   382,
     383,   378,   379,   592,   792,   380,  1138,   175,   188,   300,
    1147,    72,   378,   379,   337,   862,   380,    72,   402,   316,
     404,   362,   164,   409,   178,   410,   411,    72,   412,   413,
     179,   414,   416,   417,   414,   418,   209,   335,   120,   121,
     124,   363,   125,  1039,   736,   639,   190,   381,   382,   383,
    1172,   814,  1040,   735,   210,   122,  1044,   364,  1109,  1187,
     365,   318,   381,   382,   383,  1045,   191,  1110,   817,   527,
     189,   328,   152,   381,   382,   383,  1196,   663,    72,   319,
     450,   453,   454,   455,   456,   457,   458,   459,   460,   317,
     461,   764,   451,   452,   667,   373,   374,   668,   669,   483,
     189,   398,   193,  1041,   387,   488,   489,   670,  1216,   390,
     671,   672,   201,  1114,   673,   674,  1046,  1095,  1111,   399,
     818,   819,  1115,   124,   388,   125,   811,   812,   400,   391,
     813,   206,  1099,   364,   309,   523,   365,   310,   815,   514,
    1013,   278,   955,  1165,   214,   588,   401,   495,   496,   497,
     498,   499,   500,   501,  1042,   533,   406,   535,   192,  1001,
     538,   539,   540,   541,   542,   543,   544,  1047,  1043,  1112,
    1132,   338,   185,  1116,   407,   550,   551,    72,   553,   362,
     558,  1048,   690,  1113,   311,   339,   116,   664,   107,   108,
     571,   572,   573,   574,   575,   576,   577,   691,   623,   363,
     580,   581,   590,   591,   653,   962,   251,   665,    87,   124,
     936,   125,   974,   675,   420,   421,   389,   594,   975,   726,
     115,   392,   387,   546,  1117,   603,   604,   688,   605,   606,
     387,   607,   608,   691,   893,   165,   167,   613,  1118,   615,
     336,   387,   388,   337,   619,   620,   120,   121,   184,   186,
     388,   241,   118,   628,   578,   340,   200,   202,   630,   582,
     216,   388,   342,   655,   907,   589,   217,   854,   855,   341,
     654,   644,   241,   124,   856,   125,   343,   820,   821,   822,
     602,   656,   657,   658,   660,   378,   379,   120,   121,   380,
     610,   364,   832,   559,   365,   353,   833,   912,  1204,   192,
     913,   914,   253,   254,  1173,   834,   666,   560,   835,   836,
     915,   561,   837,   916,   917,   945,   946,   918,   919,  1178,
     777,   778,  1179,  1180,   779,   562,   624,   647,   255,   649,
     563,   652,  1181,   963,   389,  1182,  1183,   165,  1152,  1184,
    1185,   841,   389,   823,   564,   252,   381,   382,   383,  1014,
     700,   701,  1153,   389,  1154,   696,   697,   173,   702,  1009,
    1205,   174,   353,  1010,   367,   711,   369,   372,  1155,   886,
     550,   385,   355,   625,   386,   356,   357,   275,   887,  1011,
     964,   780,   781,   782,   703,   704,   329,   403,   330,   405,
     722,   626,   705,   996,   237,   703,   704,   731,   965,   733,
     693,   734,   938,  1062,   359,   737,   738,   360,   678,   251,
     695,   838,   679,   698,   289,   966,   920,   164,   707,   967,
     710,   432,    72,   757,   273,   274,   256,   257,   258,   259,
     680,   251,   770,   292,   772,   681,  1200,   968,  1186,   353,
    1201,   290,   969,   682,   104,   105,  1056,  1057,    87,   355,
     970,   683,   356,   357,   684,   437,  1202,   438,   971,   352,
     803,   972,   353,   354,   740,   260,   261,   344,   345,   346,
     806,   359,   355,   749,   360,   356,   357,   348,  1012,   358,
     685,   349,    88,   480,   481,   482,   291,   973,   827,   741,
     206,   829,   830,   831,   104,   105,  1063,   743,   431,   842,
     843,    72,   844,   845,  1064,   846,   847,   848,   849,   850,
     851,   852,   853,  1096,   283,   744,  1100,   284,   285,   745,
     746,   305,  1065,   747,   678,   857,  1066,  1067,   860,   679,
    1068,  1084,   286,   287,   288,  1055,  1085,   871,   308,   680,
     872,   243,   681,   873,   244,   245,  1086,   317,   359,  1087,
     877,   360,   682,   683,   246,  1203,   320,   247,   248,  1088,
    1089,   249,   250,   883,   884,   684,   685,   347,   976,   978,
     889,   359,  1090,  1091,   360,  1125,  1126,   897,   515,   741,
     743,   516,   859,   747,   983,   863,  1133,  1134,   866,   985,
    1135,  1167,   523,   321,   902,   678,  1168,   279,   280,  1218,
    1166,   679,  1222,   680,   908,   909,   911,   681,  1223,   682,
    1224,   921,   683,   684,  1225,   393,  1226,   685,   882,  1227,
    1228,   394,   741,   743,  1229,   629,   221,   631,   927,  1230,
    1231,   375,   747,   281,   282,   222,   376,   223,   224,  1232,
     932,  -491,  -491,   565,   396,  1034,   225,   226,   376,  1049,
    1035,   227,  1050,   251,  1051,  1052,   939,   228,   229,   940,
     941,   942,   943,   395,  1037,   230,   231,   232,   233,  1035,
     956,   957,   958,   959,   960,   961,   937,   519,   723,   520,
    1121,   724,  -491,  -491,  -491,  1035,  -491,  -491,  -491,  -491,
    -491,  -491,  -491,  -491,  -491,  -491,  -491,  -491,  -491,  -491,
    -491,  -491,   397,   728,   748,  -491,   729,   337,   752,   783,
     419,   337,   337,  -491,  -491,   423,   987,  -491,  -491,  -491,
    -491,  -491,  -491,   120,   121,  -491,  -491,  -491,  -491,  -491,
     434,   435,  -491,  -491,  -491,   867,   436,   431,   337,   106,
     426,   107,   108,   109,   110,   111,   112,   113,   505,   506,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,   114,   890,
     990,  1024,   891,   337,   107,   108,   322,   323,   324,   325,
     326,  1033,   425,   115,   997,   989,  1077,   998,   433,   337,
    1130,   327,   519,   337,   716,   995,   337,   429,   719,   753,
     755,   513,   337,   758,   759,   761,   115,  -391,  -391,   430,
      87,   485,   486,   487,   439,   337,   776,   828,   337,   519,
     870,   953,  1069,  1070,   517,   518,   529,   530,   531,   532,
     549,   337,   797,   798,   799,   521,  1072,   569,   431,   507,
    1074,  1075,   434,   570,   434,   579,   804,   583,   435,   807,
     107,   108,   322,   323,   324,   325,   326,  1081,  1082,  1083,
      87,   584,   585,   587,   435,   600,   601,   327,   616,   435,
     618,   435,   221,  1092,   522,   503,   434,   694,   508,  1093,
     509,   222,   115,   223,   224,   567,  1098,   765,   601,   510,
    1104,   511,   225,   226,   512,  1108,   221,   227,  1076,   816,
     601,   523,  1078,   228,   229,   222,   524,   223,   224,  1123,
    1124,   230,   231,   232,   233,   525,   225,   226,   825,   337,
     526,   227,   826,   337,   865,   337,   528,   228,   229,   874,
     337,   875,   530,   876,   337,   230,   231,   232,   233,   880,
     530,   700,   701,   703,   704,   534,  1146,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   434,   881,   104,   105,
    1160,   612,   271,   885,   729,  1164,   894,   601,   898,   337,
     899,   337,   568,   903,   900,   585,   272,   947,   601,   948,
     601,   977,   337,   614,  1033,   979,   530,   980,   337,   547,
    1174,  1175,  1177,   982,   337,   434,   994,  1003,   435,   548,
    1188,   609,  1189,  1004,   585,  1073,   337,   928,  1129,   337,
    1131,   585,  1139,   601,  1148,   601,  1197,   601,   195,   197,
     617,  1199,   934,   491,   492,   621,  1206,  1207,  1208,  1209,
    1210,  1211,  1212,  1213,   493,   494,   622,   623,   624,   625,
     627,   626,   645,   641,   650,   642,   676,   677,  1219,  1220,
    1221,     4,     5,     6,     7,     8,   678,   679,   686,   137,
     138,   128,   129,   312,    14,    15,    16,    17,    18,    19,
      20,   680,   681,   682,   683,   684,   685,   272,   689,   278,
     699,   131,    30,   712,   718,   717,   725,   739,    36,   688,
     691,   720,   721,   771,    38,   774,   376,   730,   741,   742,
     743,   773,    46,    47,    48,    49,   744,   745,    52,   746,
     747,   775,   750,   751,   808,   878,  1002,   784,    57,    58,
    1006,  1007,  1008,   785,    59,   786,   787,   788,   789,   793,
     809,   794,   337,   795,   800,   801,    62,   796,   802,   805,
     879,   868,   864,  1028,   869,   888,   892,  1030,   895,   896,
     904,   905,   906,   929,   935,   930,   931,    63,    64,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   933,    65,   944,   949,   950,   951,
     952,    66,    67,   954,   976,   978,   981,    68,   988,   132,
     313,   983,    71,   984,    72,   985,   314,     4,     5,     6,
       7,     8,   986,   992,   991,   137,   138,   128,   129,   312,
      14,    15,    16,    17,    18,    19,    20,   993,  1023,  1079,
    1080,   999,  1000,  1025,  1026,  1027,  1029,   131,    30,  1031,
    1053,  1054,  1058,  1059,    36,  1060,  1061,  1094,  1071,  1097,
      38,  1101,  1103,  1102,  1105,  1106,  1107,  1119,    46,    47,
      48,    49,  1120,  1127,    52,  1035,  1128,  1136,  1157,  1149,
    1150,  1140,  1151,  1156,    57,    58,  1158,  1161,  1159,  1162,
      59,  1163,  1198,  1169,  1190,  1191,  1192,  1193,  1194,   166,
     810,  1171,    62,  1195,  1214,  1215,   727,   464,   462,   484,
       0,     0,     0,     0,   490,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
    1137,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,   132,   708,     0,    71,     0,
      72,     0,   314,     0,     0,     0,     0,  1170,     4,     5,
       6,     7,     8,     0,     0,     0,   137,   138,   128,   129,
     312,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   131,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,  1217,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,    66,    67,
       0,     0,     0,     0,    68,     0,   132,   861,     0,    71,
       0,    72,     0,   314,     4,     5,     6,     7,     8,     0,
       0,     0,   137,   138,   128,   129,   312,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   131,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,    66,    67,     0,     0,     0,     0,
      68,     0,   132,     0,     0,    71,     0,    72,     0,   314,
       4,     5,     6,     7,     8,     0,     0,     0,     9,   440,
     441,   442,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,   443,   444,    26,     0,     0,    27,    28,
      29,    30,   445,   446,    33,    34,    35,    36,    37,     0,
       0,     0,     0,    38,    39,    40,    41,    42,    43,   447,
     448,    46,    47,    48,    49,    50,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,    60,     0,     0,
       0,    61,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,    69,    70,     0,
       0,    71,     0,    72,   449,     4,     5,     6,     7,     8,
       0,     0,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,   766,   767,
      26,     0,     0,    27,    28,    29,    30,    31,   768,    33,
      34,    35,    36,    37,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,    61,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,    69,    70,     0,     0,    71,     0,    72,   769,
       4,     5,     6,     7,     8,     0,     0,     0,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,     0,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,     0,
       0,     0,     0,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,    60,     0,     0,
       0,    61,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,    69,    70,     0,
       0,    71,     0,    72,   824,     4,     5,     6,     7,     8,
       0,     0,     0,     9,    10,    11,  1141,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,  1142,  1143,
      26,     0,     0,    27,    28,    29,    30,    31,  1144,    33,
      34,    35,    36,    37,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,    61,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,    69,    70,     0,     0,    71,     0,    72,  1145,
       4,     5,     6,     7,     8,     0,     0,     0,   137,   138,
     128,   129,   130,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     131,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,   169,   132,     0,
       0,    71,     0,    72,   659,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,   169,   132,     0,     0,    71,     0,    72,   910,
       4,     5,     6,     7,     8,     0,     0,     0,   137,   138,
     128,   129,   130,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     131,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
      66,    67,     0,     0,     0,     0,    68,   169,   132,     0,
       0,    71,     0,    72,  1176,     4,     5,     6,     7,     8,
       0,     0,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,    61,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,    69,    70,     0,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   137,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,   132,     0,     0,
      71,   139,    72,     4,     5,     6,     7,     8,     0,     0,
       0,   137,   138,   128,   129,   130,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
     142,   143,     0,     0,    71,     0,    72,     4,     5,     6,
       7,     8,     0,     0,     0,   137,   138,   128,   129,   130,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,   169,   132,     0,     0,    71,     0,
      72,     4,     5,     6,     7,     8,     0,     0,     0,   137,
     138,   128,   129,   130,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,    68,   171,   132,
       0,     0,    71,     0,    72,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,   204,   132,     0,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   235,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,   132,   236,     0,
      71,     0,    72,     4,     5,     6,     7,     8,     0,     0,
       0,   137,   138,   128,   129,   130,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,   132,     0,     0,    71,   239,    72,     4,     5,     6,
       7,     8,     0,     0,     0,   137,   138,   128,   129,   130,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,   132,   299,     0,    71,     0,
      72,     4,     5,     6,     7,     8,     0,     0,     0,   137,
     138,   128,   129,   130,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,   303,     0,     0,     0,    68,     0,   132,
       0,     0,    71,     0,    72,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,   132,   236,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   137,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,   331,   132,     0,     0,
      71,     0,    72,     4,     5,     6,     7,     8,     0,     0,
       0,   137,   138,   128,   129,   130,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,   132,   333,     0,    71,     0,    72,     4,     5,     6,
       7,     8,     0,     0,     0,   137,   138,   128,   129,   130,
      14,    15,    16,    17,    18,    19,    20,     0,     0,   408,
       0,     0,     0,     0,     0,     0,     0,   131,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,   132,     0,     0,    71,     0,
      72,     4,     5,     6,     7,     8,     0,     0,     0,   137,
     138,   128,   129,   130,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,    68,     0,   132,
       0,   415,    71,     0,    72,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,   132,     0,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   137,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,   132,     0,     0,
      71,   545,    72,     4,     5,     6,     7,     8,     0,     0,
       0,   137,   138,   128,   129,   130,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,   132,   646,     0,    71,     0,    72,     4,     5,     6,
       7,     8,     0,     0,     0,   137,   138,   128,   129,   130,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,   132,   648,     0,    71,     0,
      72,     4,     5,     6,     7,     8,     0,     0,     0,   137,
     138,   128,   129,   130,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,    68,     0,   132,
       0,     0,    71,   651,    72,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,   132,   692,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   137,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,   132,   706,     0,
      71,     0,    72,     4,     5,     6,   554,     8,     0,     0,
       0,   137,   138,   128,   129,   555,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,   132,   236,     0,    71,     0,    72,     4,     5,     6,
       7,     8,     0,     0,     0,   137,   138,   128,   129,   130,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,   132,   858,     0,    71,     0,
      72,     4,     5,     6,     7,     8,     0,     0,     0,   137,
     138,   128,   129,   130,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,    68,     0,   153,
       0,     0,    71,     0,    72,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,   155,     0,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   137,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,   157,     0,     0,
      71,     0,    72,     4,     5,     6,     7,     8,     0,     0,
       0,   137,   138,   128,   129,   130,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,   159,     0,     0,    71,     0,    72,     4,     5,     6,
       7,     8,     0,     0,     0,   137,   138,   128,   129,   130,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,   132,     0,     0,    71,     0,
      72,     4,     5,     6,     7,     8,     0,     0,     0,   137,
     138,   128,   129,   130,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,    68,     0,   180,
       0,     0,    71,     0,    72,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,   182,     0,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   137,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,   132,     0,     0,
      71,     0,   196,     4,     5,     6,     7,     8,     0,     0,
       0,   137,   138,   128,   129,   130,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,   301,     0,     0,    71,     0,    72,     4,     5,     6,
       7,     8,     0,     0,     0,   137,   138,   128,   129,   130,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,    66,    67,     0,
       0,     0,     0,    68,     0,   552,     0,     0,    71,     0,
      72,     4,     5,     6,   554,     8,     0,     0,     0,   137,
     138,   128,   129,   555,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   131,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,    68,     0,   556,
       0,     0,    71,     0,    72,     4,     5,     6,     7,     8,
       0,     0,     0,   137,   138,   128,   129,   130,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,    66,    67,     0,     0,     0,
       0,    68,     0,   732,     0,     0,    71,     0,    72,     4,
       5,     6,     7,     8,     0,     0,     0,   137,   138,   128,
     129,   130,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,    68,     0,   756,     0,     0,
      71,     0,    72,     4,     5,     6,     7,     8,     0,     0,
       0,   126,   127,   128,   129,   130,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,     0,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,    68,
       0,   132,     0,     0,    71,     0,    72
};

static const yytype_int16 yycheck[] =
{
       2,     0,   254,   584,   753,    70,   347,    15,    32,   938,
     526,    15,     2,     2,   375,    17,   532,    28,    11,    24,
      24,    24,    24,    25,    26,    27,    28,    42,    43,    31,
      32,    33,    11,    12,    36,    37,    15,    28,    40,    41,
      16,   290,   291,    55,    88,    15,    48,    49,    60,    61,
      52,    53,    88,    28,    86,    87,    15,    48,    88,    42,
      43,   128,   126,   140,    15,   426,   143,    11,    70,    71,
     147,   320,   321,    12,    11,    12,     0,   126,    15,   146,
     144,    57,    11,    12,    77,    78,    15,   152,   132,    65,
      66,    67,   141,    72,    73,    74,   132,    11,    12,    69,
     132,    15,    11,    12,    13,    28,    15,   109,   110,   111,
     112,   113,   114,   143,   116,   117,   146,   119,    28,    88,
     122,   126,   146,   126,    33,    48,  1055,    63,    64,    38,
     132,    67,   147,    77,    78,    72,    73,    74,    48,   141,
     148,   143,   146,    72,    73,    74,    11,    12,    28,   140,
      15,   153,   143,   155,   903,   157,   147,   159,    72,    73,
      74,    11,    12,   142,   147,    15,  1095,    15,    48,   115,
    1099,   146,    11,    12,   143,   691,    15,   146,   180,   125,
     182,    28,   141,   185,    13,   187,   188,   146,   190,   191,
      13,   193,   194,   195,   196,   197,    15,   143,   109,   110,
     139,    48,   141,    15,   565,   142,    28,    72,    73,    74,
    1139,   147,    24,   142,    33,   126,    15,   140,    15,  1148,
     143,   128,    72,    73,    74,    24,    48,    24,   142,    85,
     140,   126,   141,    72,    73,    74,  1165,   147,   146,   146,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   144,
     252,   592,   242,   242,    28,    86,    87,    31,    32,   271,
     140,   128,   146,    75,    28,   276,   277,    41,  1197,    28,
      44,    45,   146,    15,    48,    49,    75,   142,    75,   146,
     641,   642,    24,   139,    48,   141,    63,    64,   128,    48,
      67,    15,   142,   140,    15,   140,   143,    18,   639,   301,
     147,   132,   147,   142,    11,   370,   146,   283,   284,   285,
     286,   287,   288,   289,   126,   317,    31,   319,   140,   900,
     322,   323,   324,   325,   326,   327,   328,   126,   140,   126,
    1079,   126,   141,    75,    49,   337,   338,   146,   340,    28,
     342,   140,   126,   140,    65,   140,   126,    28,   109,   110,
     352,   353,   354,   355,   356,   357,   358,   141,   140,    48,
     362,   363,   373,   374,   144,   147,   140,    48,     2,   139,
     147,   141,   126,   147,    42,    43,   140,   376,   126,    85,
     141,   140,    28,   329,   126,   387,   388,   141,   390,   391,
      28,   393,   394,   141,   735,    29,    30,   399,   140,   401,
     140,    28,    48,   143,   406,   407,   109,   110,    42,    43,
      48,    79,   128,   415,   360,   126,    50,    51,   420,   365,
      11,    48,   126,   126,   765,   371,    11,   676,   677,   140,
     146,   430,    79,   139,   686,   141,   140,    81,    82,    83,
     386,   443,   444,   445,   446,    11,    12,   109,   110,    15,
     396,   140,    28,   126,   143,    31,    32,    28,   147,   140,
      31,    32,   103,   104,   126,    41,   147,   140,    44,    45,
      41,   126,    48,    44,    45,   816,   817,    48,    49,    28,
      11,    12,    31,    32,    15,   140,   140,   433,   102,   435,
     126,   437,    41,   147,   140,    44,    45,   131,   126,    48,
      49,   147,   140,   147,   140,    85,    72,    73,    74,   147,
     118,   119,   140,   140,   126,   517,   518,    11,   126,    28,
     147,    15,    31,    32,   158,   527,   160,   161,   140,     6,
     532,   140,    41,   140,   143,    44,    45,   116,    15,    48,
     147,    72,    73,    74,   118,   119,   143,   181,   145,   183,
     552,   140,   126,   894,   556,   118,   119,   559,   147,   561,
     506,   563,   811,   126,   140,   567,   568,   143,   140,   140,
     516,   147,   140,   519,   125,   147,   147,   141,   524,   147,
     526,   215,   146,   585,   130,   131,    89,    90,    91,    92,
     140,   140,   594,    15,   596,   140,    28,   147,   147,    31,
      32,   139,   147,   140,   594,   594,   947,   948,   242,    41,
     147,   140,    44,    45,   140,   143,    48,   145,   147,    28,
     622,   147,    31,    32,   570,   128,   129,    11,    12,    13,
     632,   140,    41,   579,   143,    44,    45,    11,   147,    48,
     140,    15,   644,     8,     9,    10,   139,   147,   650,   140,
      15,   653,   654,   655,   644,   644,   147,   140,   143,   661,
     662,   146,   664,   665,   147,   667,   668,   669,   670,   671,
     672,   673,   674,  1034,   117,   140,  1037,   120,   121,   140,
     140,    15,   147,   140,   140,   687,   147,   147,   690,   140,
     147,   147,   135,   136,   137,   944,   147,   699,   140,   140,
     702,    28,   140,   705,    31,    32,   147,   144,   140,   147,
     712,   143,   140,   140,    41,   147,   139,    44,    45,   147,
     147,    48,    49,   725,   726,   140,   140,    15,   140,   140,
     732,   140,   147,   147,   143,   147,   147,   739,   140,   140,
     140,   143,   688,   140,   140,   691,   147,   147,   694,   140,
     147,   147,   140,   139,   756,   140,   147,   123,   124,   147,
    1121,   140,   147,   140,   766,   767,   768,   140,   147,   140,
     147,   773,   140,   140,   147,   126,   147,   140,   724,   147,
     147,   126,   140,   140,   147,   419,    15,   421,   790,   147,
     147,   141,   140,   133,   134,    24,   146,    26,    27,   147,
     802,    44,    45,   141,   143,   141,    35,    36,   146,    11,
     146,    40,    14,   140,    16,    17,    11,    46,    47,    14,
      15,    16,    17,    76,   141,    54,    55,    56,    57,   146,
     832,   833,   834,   835,   836,   837,    65,   143,   140,   145,
     141,   143,    85,    86,    87,   146,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   146,   140,   140,   108,   143,   143,   140,   140,
      59,   143,   143,   116,   117,   140,   878,   120,   121,   122,
     123,   124,   125,   109,   110,   128,   129,   130,   131,   132,
     142,   143,   135,   136,   137,   140,   142,   143,   143,   107,
     141,   109,   110,   111,   112,   113,   114,   115,   140,   141,
     912,   913,   914,   915,   916,   917,   918,   919,   126,   140,
     140,   923,   143,   143,   109,   110,   111,   112,   113,   114,
     115,   933,   146,   141,   140,   881,   140,   143,   141,   143,
     140,   126,   143,   143,   145,   891,   143,   146,   145,   583,
     584,   142,   143,   587,   588,   589,   141,   142,   143,   146,
     594,   273,   274,   275,   147,   143,   600,   145,   143,   143,
     145,   145,   974,   975,   133,   134,   142,   143,   142,   143,
     142,   143,   616,   617,   618,   129,   988,   142,   143,   140,
     992,   993,   142,   143,   142,   143,   630,   142,   143,   633,
     109,   110,   111,   112,   113,   114,   115,  1009,  1010,  1011,
     644,    29,    30,   142,   143,   142,   143,   126,   142,   143,
     142,   143,    15,  1025,   147,    18,   142,   143,   140,  1031,
     140,    24,   141,    26,    27,   126,  1035,   142,   143,   140,
    1042,   140,    35,    36,   140,  1047,    15,    40,   994,   142,
     143,   140,   998,    46,    47,    24,   141,    26,    27,  1061,
    1062,    54,    55,    56,    57,    15,    35,    36,   142,   143,
     141,    40,   142,   143,   142,   143,    15,    46,    47,   142,
     143,   142,   143,   142,   143,    54,    55,    56,    57,   142,
     143,   118,   119,   118,   119,    15,  1098,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   142,   143,  1098,  1098,
    1112,    15,   108,   142,   143,  1117,   142,   143,   142,   143,
     142,   143,   126,   757,    29,    30,   122,   142,   143,   142,
     143,   142,   143,    15,  1136,   142,   143,   142,   143,   140,
    1142,  1143,  1144,   142,   143,   142,   143,   142,   143,   140,
    1152,   140,  1154,    29,    30,   142,   143,   791,   142,   143,
      29,    30,   142,   143,   142,   143,   142,   143,    47,    48,
      29,  1173,   806,   279,   280,    11,  1178,  1179,  1180,  1181,
    1182,  1183,  1184,  1185,   281,   282,   140,   140,   140,   140,
     147,   140,    11,   141,   126,   141,   139,   139,  1200,  1201,
    1202,     3,     4,     5,     6,     7,   140,   140,   105,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,   140,   140,   140,   140,   140,   140,   122,    15,   132,
     126,    33,    34,   141,   147,   129,    85,   126,    40,   141,
     141,   140,   140,    15,    46,    11,   146,   140,   140,   140,
     140,   126,    54,    55,    56,    57,   140,   140,    60,   140,
     140,    11,   140,   140,    15,    85,   900,   140,    70,    71,
     904,   905,   906,   140,    76,   140,   140,   140,   140,   129,
      15,   147,   143,   129,   140,   140,    88,   147,   126,   147,
      15,   145,   140,   927,   145,   140,   140,   931,   140,   140,
      11,    11,    11,   140,   147,   140,    29,   109,   110,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   140,   127,    63,    15,    15,    15,
     140,   133,   134,   147,   140,   140,   140,   139,   141,   141,
     142,   140,   144,   140,   146,   140,   148,     3,     4,     5,
       6,     7,   142,    85,   140,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    85,    15,  1003,
    1004,   140,   140,   126,    11,    11,   147,    33,    34,   140,
     140,   140,     8,     8,    40,     8,   126,   142,   140,    79,
      46,    15,   140,    15,    15,    15,   140,   140,    54,    55,
      56,    57,   140,   142,    60,   146,   140,   140,    15,   140,
     140,   147,   140,   140,    70,    71,    15,    15,   140,    15,
      76,   140,   142,   140,   140,   140,   140,   140,   140,    30,
     637,  1136,    88,   140,   140,   140,   556,   255,   253,   272,
      -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
    1094,    -1,    -1,    -1,  1098,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,    -1,   144,    -1,
     146,    -1,   148,    -1,    -1,    -1,    -1,  1131,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1198,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,   141,   142,    -1,   144,
      -1,   146,    -1,   148,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,   141,    -1,    -1,   144,    -1,   146,    -1,   148,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,   146,   147,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,   146,   147,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,   146,   147,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,   146,   147,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,   146,   147,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,   146,   147,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,   146,   147,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,
     144,   145,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,   144,    -1,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,   142,    -1,
     144,    -1,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,   141,    -1,    -1,   144,   145,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,   141,
      -1,    -1,   144,    -1,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,   141,   142,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
     144,    -1,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,   141,   142,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,    -1,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,   141,
      -1,   143,   144,    -1,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,   141,    -1,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,
     144,   145,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,   141,   142,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,   141,
      -1,    -1,   144,   145,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,   141,   142,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,   142,    -1,
     144,    -1,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,   141,   142,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,   142,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,   141,
      -1,    -1,   144,    -1,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,   141,    -1,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,
     144,    -1,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,   141,    -1,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,    -1,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,   141,
      -1,    -1,   144,    -1,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,   141,    -1,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,
     144,    -1,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,   141,    -1,    -1,   144,    -1,   146,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,    -1,    -1,   144,    -1,
     146,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,   141,
      -1,    -1,   144,    -1,   146,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,   141,    -1,    -1,   144,    -1,   146,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,
     144,    -1,   146,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,   141,    -1,    -1,   144,    -1,   146
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   150,   151,     0,     3,     4,     5,     6,     7,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    66,    67,    70,    71,    76,
      80,    84,    88,   109,   110,   127,   133,   134,   139,   140,
     141,   144,   146,   152,   153,   155,   156,   157,   158,   159,
     161,   162,   165,   169,   170,   171,   174,   179,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   198,   199,   201,   107,   109,   110,   111,
     112,   113,   114,   115,   126,   141,   126,   144,   128,   146,
     109,   110,   126,    12,   139,   141,    11,    12,    13,    14,
      15,    33,   141,   179,   194,   199,   201,    11,    12,   145,
     181,    15,   140,   141,   181,   200,    11,    12,    13,    15,
      33,    38,   141,   141,   181,   141,   181,   141,   181,   141,
     181,   181,   190,    15,   141,   179,   161,   179,   181,   140,
     181,   140,   181,    11,    15,    15,   181,   181,    13,    13,
     141,   181,   141,   181,   179,   141,   179,    28,    48,   140,
      28,    48,   140,   146,   180,   180,   146,   180,   181,   181,
     179,   146,   179,   181,   140,   181,    15,   168,   168,    15,
      33,   194,   168,   168,    11,   196,    11,    11,   194,   194,
     194,    15,    24,    26,    27,    35,    36,    40,    46,    47,
      54,    55,    56,    57,   197,    11,   142,   181,   196,   145,
     181,    79,   151,    28,    31,    32,    41,    44,    45,    48,
      49,   140,    85,   103,   104,   102,    89,    90,    91,    92,
     128,   129,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   108,   122,   130,   131,   116,    86,    87,   132,   123,
     124,   133,   134,   117,   120,   121,   135,   136,   137,   125,
     139,   139,    15,   181,   181,   181,   181,   181,   181,   142,
     200,   141,   181,   135,   181,    15,   181,   181,   140,    15,
      18,    65,    15,   142,   148,   176,   200,   144,   128,   146,
     139,   139,   111,   112,   113,   114,   115,   126,   126,   143,
     145,   140,   181,   142,   181,   200,   140,   143,   126,   140,
     126,   140,   126,   140,    11,    12,    13,    15,    11,    15,
     196,   181,    28,    31,    32,    41,    44,    45,    48,   140,
     143,   181,    28,    48,   140,   143,   181,   179,   181,   179,
      88,   143,   179,    86,    87,   141,   146,   166,    11,    12,
      15,    72,    73,    74,   178,   140,   143,    28,    48,   140,
      28,    48,   140,   126,   126,    76,   143,   146,   128,   146,
     128,   146,   181,   179,   181,   179,    31,    49,    25,   181,
     181,   181,   181,   181,   181,   143,   181,   181,   181,    59,
      42,    43,   163,   140,   172,   146,   141,    15,    69,   146,
     146,   143,   179,   141,   142,   143,   142,   143,   145,   147,
      12,    13,    14,    26,    27,    35,    36,    52,    53,   147,
     181,   199,   201,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   184,   182,   185,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
       8,     9,    10,   168,   187,   189,   189,   189,   190,   190,
     191,   192,   192,   193,   193,   194,   194,   194,   194,   194,
     194,   194,   197,    18,   197,   140,   141,   140,   140,   140,
     140,   140,   140,   142,   181,   140,   143,   133,   134,   143,
     145,   129,   147,   140,   141,    15,   141,    85,    15,   142,
     143,   142,   143,   181,    15,   181,   197,   197,   181,   181,
     181,   181,   181,   181,   181,   145,   200,   140,   140,   142,
     181,   181,   141,   181,     6,    15,   141,   177,   181,   126,
     140,   126,   140,   126,   140,   141,   166,   126,   126,   142,
     143,   181,   181,   181,   181,   181,   181,   181,   200,   143,
     181,   181,   200,   142,    29,    30,   154,   142,   196,   200,
     190,   190,   142,   178,   151,    24,   126,    11,    77,    78,
     142,   143,   200,   181,   181,   181,   181,   181,   181,   140,
     200,   164,    15,   181,    15,   181,   142,    29,   142,   181,
     181,    11,   140,   140,   140,   140,   140,   147,   181,   179,
     181,   179,    42,    43,   147,    15,    24,   146,   173,   142,
     178,   141,   141,   175,   151,    11,   142,   200,   142,   200,
     126,   145,   200,   144,   146,   126,   181,   181,   181,   147,
     181,    28,    48,   147,    28,    48,   147,    28,    31,    32,
      41,    44,    45,    48,    49,   147,   139,   139,   140,   140,
     140,   140,   140,   140,   140,   140,   105,   126,   141,    15,
     126,   141,   142,   200,   143,   200,   181,   181,   200,   126,
     118,   119,   126,   118,   119,   126,   142,   200,   142,   176,
     200,   181,   141,    15,   148,   176,   145,   129,   147,   145,
     140,   140,   181,   140,   143,    85,    85,   177,   140,   143,
     140,   181,   141,   181,   181,   142,   178,   181,   181,   126,
     200,   140,   140,   140,   140,   140,   140,   140,   140,   200,
     140,   140,   140,   179,   153,   179,   141,   181,   179,   179,
      88,   179,    88,    88,   166,   142,    26,    27,    36,   147,
     181,    15,   181,   126,    11,    11,   179,    11,    12,    15,
      72,    73,    74,   140,   140,   140,   140,   140,   140,   140,
      42,    43,   147,   129,   147,   129,   147,   179,   179,   179,
     140,   140,   126,   181,   179,   147,   181,   179,    15,    15,
     173,    63,    64,    67,   147,   166,   142,   142,   178,   178,
      81,    82,    83,   147,   147,   142,   142,   181,   145,   181,
     181,   181,    28,    32,    41,    44,    45,    48,   147,   147,
     147,   147,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   197,   197,   182,   181,   142,   200,
     181,   142,   176,   200,   140,   142,   200,   140,   145,   145,
     145,   181,   181,   181,   142,   142,   142,   181,    85,    15,
     142,   143,   200,   181,   181,   142,     6,    15,   140,   181,
     140,   143,   140,   166,   142,   140,   140,   181,   142,   142,
      29,   154,   181,   179,    11,    11,    11,   166,   181,   181,
     147,   181,    28,    31,    32,    41,    44,    45,    48,    49,
     147,   181,    24,   126,    11,    77,    78,   181,   179,   140,
     140,    29,   181,   140,   179,   147,   147,    65,   197,    11,
      14,    15,    16,    17,    63,   166,   166,   142,   142,    15,
      15,    15,   140,   145,   147,   147,   181,   181,   181,   181,
     181,   181,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   126,   126,   140,   142,   140,   142,
     142,   140,   142,   140,   140,   140,   142,   181,   141,   200,
     140,   140,    85,    85,   143,   200,   166,   140,   143,   140,
     140,   153,   179,   142,    29,   154,   179,   179,   179,    28,
      32,    48,   147,   147,   147,   181,   181,   181,   181,   181,
     181,   181,   181,    15,   181,   126,    11,    11,   179,   147,
     179,   140,   160,   181,   141,   146,   167,   141,   167,    15,
      24,    75,   126,   140,    15,    24,    75,   126,   140,    11,
      14,    16,    17,   140,   140,   197,   166,   166,     8,     8,
       8,   126,   126,   147,   147,   147,   147,   147,   147,   181,
     181,   140,   181,   142,   181,   181,   200,   140,   200,   179,
     179,   181,   181,   181,   147,   147,   147,   147,   147,   147,
     147,   147,   181,   181,   142,   142,   178,    79,   151,   142,
     178,    15,    15,   140,   181,    15,    15,   140,   181,    15,
      24,    75,   126,   140,    15,    24,    75,   126,   140,   140,
     140,   141,   167,   181,   181,   147,   147,   142,   140,   142,
     140,    29,   154,   147,   147,   147,   140,   179,   167,   142,
     147,    14,    26,    27,    36,   147,   181,   167,   142,   140,
     140,   140,   126,   140,   126,   140,   140,    15,    15,   140,
     181,    15,    15,   140,   181,   142,   178,   147,   147,   140,
     179,   160,   167,   126,   181,   181,   147,   181,    28,    31,
      32,    41,    44,    45,    48,    49,   147,   167,   181,   181,
     140,   140,   140,   140,   140,   140,   167,   142,   142,   181,
      28,    32,    48,   147,   147,   147,   181,   181,   181,   181,
     181,   181,   181,   181,   140,   140,   167,   179,   147,   181,
     181,   181,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   149,   150,   151,   151,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   153,   153,   153,   153,   153,   153,   153,   153,   154,
     154,   154,   154,   154,   154,   155,   155,   156,   156,   156,
     156,   157,   157,   158,   158,   158,   159,   159,   159,   160,
     161,   161,   161,   161,   161,   161,   162,   162,   163,   163,
     164,   164,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   168,
     169,   170,   171,   172,   172,   172,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   174,   175,   175,   175,   175,   176,   176,   176,
     176,   177,   177,   177,   177,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     180,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   182,   182,   183,   183,   184,   184,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   186,   186,
     187,   187,   187,   188,   188,   189,   189,   189,   190,   190,
     191,   191,   191,   192,   192,   192,   193,   193,   193,   193,
     193,   193,   193,   194,   194,   194,   194,   194,   194,   195,
     195,   196,   196,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   198,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   200,   200,   200,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
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
       0,     4,     6,     5,     3,     7,     6,     4,     7,     6,
       7,     6,     5,     3,     4,     5,     4,     5,     5,     6,
       6,     6,     6,     6,     6,     6,     6,     7,     7,     7,
       3,     3,     6,     4,     5,     4,     5,     5,     6,     6,
       6,     6,     6,     6,     6,     6,     7,     7,     7,     1,
       6,     5,     5,     0,     3,     3,     0,     4,     4,     4,
       5,     5,     4,     5,     5,     6,     6,     7,     7,     6,
       8,     8,     6,     6,     6,     5,     5,     6,     6,     7,
       7,     7,     7,     7,     6,     4,     7,     6,     4,     8,
       7,     5,     5,     0,     4,     4,     4,     3,     5,     5,
       7,     3,     3,     5,     5,     1,     3,     5,     1,     3,
       2,     3,     3,     3,     4,     5,     5,     3,     5,     4,
       6,     1,     3,     1,     3,     1,     3,     3,     3,     4,
       6,     6,     6,     6,     6,     6,     6,     6,     5,     7,
       7,     7,     7,     7,     7,     5,     8,     8,     6,     9,
       9,     5,     5,     4,     4,     4,     6,     6,     6,     6,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     1,     5,     1,     3,     1,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     1,     2,     2,     2,     2,     2,     1,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     3,
       4,     6,     4,     3,     6,     6,     5,     3,     3,     6,
       5,     4,     4,     6,     6,     5,     3,     6,     5,     3,
       5,     4,     2,     2,     3,     4,     3,     2,     3,     2,
       1,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     4,     6,     6,     6,     1,     4,     4,     5,
       5,     5,     5,     5,     5,     1,     1,     1,     1,     2,
       3,     4,     5,     2,     3,     4,     5,     2,     3,     4,
       5,     1,     2,     5,     3
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
#line 569 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3954 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 579 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3960 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 580 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3966 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 584 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3972 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 586 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3978 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 588 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3984 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 590 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3992 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 594 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3998 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 596 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4004 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 598 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4010 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 600 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4016 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 602 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4024 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 606 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4032 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 610 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4038 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 612 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4046 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 616 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4054 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 620 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4060 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 622 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4066 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 624 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4072 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 626 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4078 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4084 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 630 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4092 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 634 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4100 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 638 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4106 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 640 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4112 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 642 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4118 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 644 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4124 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 646 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4130 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 648 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4136 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 650 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 4142 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 652 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 4148 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 654 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 4154 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 656 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 4160 "raku.tab.c"
    break;

  case 35: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 658 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4172 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP ';'  */
#line 666 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 4178 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 668 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); free((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4184 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP '(' ')' ';'  */
#line 670 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 4190 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP arg_list ';'  */
#line 672 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); free((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4196 "raku.tab.c"
    break;

  case 40: /* stmt: IDENT VAR_ARRAY ';'  */
#line 674 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4202 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ';'  */
#line 676 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4208 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 678 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4215 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 681 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4222 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ';'  */
#line 684 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4228 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 686 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4235 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 689 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4242 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ';'  */
#line 692 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4248 "raku.tab.c"
    break;

  case 48: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 694 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4256 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr ';'  */
#line 698 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4262 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN ';'  */
#line 700 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4268 "raku.tab.c"
    break;

  case 51: /* stmt: KW_FAIL expr ';'  */
#line 702 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4274 "raku.tab.c"
    break;

  case 52: /* stmt: KW_FAIL ';'  */
#line 704 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4280 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 706 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4287 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 709 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4294 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 712 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4300 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 714 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4307 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 717 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4314 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 720 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4321 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 723 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4327 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 725 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4336 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 730 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4344 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 734 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4352 "raku.tab.c"
    break;

  case 63: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 738 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4359 "raku.tab.c"
    break;

  case 64: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 741 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4366 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 744 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4374 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 748 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4381 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 751 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4388 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 754 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4395 "raku.tab.c"
    break;

  case 69: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 757 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4402 "raku.tab.c"
    break;

  case 70: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 760 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4409 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_IF expr ';'  */
#line 763 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4415 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_UNLESS expr ';'  */
#line 765 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4421 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WHILE expr ';'  */
#line 767 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4427 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_UNTIL expr ';'  */
#line 769 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4433 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_FOR expr ';'  */
#line 771 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4440 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_WITH expr ';'  */
#line 774 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4446 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_WITHOUT expr ';'  */
#line 776 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4452 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_GIVEN expr ';'  */
#line 778 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4458 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 780 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4465 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 783 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4472 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 786 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4480 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 790 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4487 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 793 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4493 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 795 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4499 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 797 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4505 "raku.tab.c"
    break;

  case 86: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 799 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4512 "raku.tab.c"
    break;

  case 87: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 802 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4519 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 805 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4526 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 808 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4533 "raku.tab.c"
    break;

  case 90: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 811 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4540 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 814 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4547 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 817 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4554 "raku.tab.c"
    break;

  case 93: /* stmt: expr ';'  */
#line 819 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4560 "raku.tab.c"
    break;

  case 94: /* stmt: ';'  */
#line 820 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4566 "raku.tab.c"
    break;

  case 95: /* stmt: if_stmt  */
#line 821 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4572 "raku.tab.c"
    break;

  case 96: /* stmt: while_stmt  */
#line 822 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4578 "raku.tab.c"
    break;

  case 97: /* stmt: for_stmt  */
#line 823 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4584 "raku.tab.c"
    break;

  case 98: /* stmt: given_stmt  */
#line 824 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4590 "raku.tab.c"
    break;

  case 99: /* stmt: KW_TRY block  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4596 "raku.tab.c"
    break;

  case 100: /* stmt: KW_TRY block KW_CATCH block  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4602 "raku.tab.c"
    break;

  case 101: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 830 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4610 "raku.tab.c"
    break;

  case 102: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 834 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4619 "raku.tab.c"
    break;

  case 103: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 839 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4627 "raku.tab.c"
    break;

  case 104: /* stmt: KW_CATCH block  */
#line 843 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4633 "raku.tab.c"
    break;

  case 105: /* stmt: block  */
#line 845 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4639 "raku.tab.c"
    break;

  case 106: /* stmt: unless_stmt  */
#line 846 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4645 "raku.tab.c"
    break;

  case 107: /* stmt: until_stmt  */
#line 847 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4651 "raku.tab.c"
    break;

  case 108: /* stmt: repeat_stmt  */
#line 848 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4657 "raku.tab.c"
    break;

  case 109: /* stmt: loop_stmt  */
#line 849 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4663 "raku.tab.c"
    break;

  case 110: /* stmt: KW_LAST ';'  */
#line 850 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4669 "raku.tab.c"
    break;

  case 111: /* stmt: KW_NEXT ';'  */
#line 851 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4675 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST KW_IF expr ';'  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4681 "raku.tab.c"
    break;

  case 113: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 855 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4687 "raku.tab.c"
    break;

  case 114: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 857 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4693 "raku.tab.c"
    break;

  case 115: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 859 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4699 "raku.tab.c"
    break;

  case 116: /* stmt: sub_decl  */
#line 860 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4705 "raku.tab.c"
    break;

  case 117: /* stmt: class_decl  */
#line 861 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4711 "raku.tab.c"
    break;

  case 118: /* stmt: role_decl  */
#line 862 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4717 "raku.tab.c"
    break;

  case 119: /* stmt: grammar_decl  */
#line 863 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4723 "raku.tab.c"
    break;

  case 120: /* stmt: module_decl  */
#line 864 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4729 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block  */
#line 868 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4735 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 870 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4741 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 872 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4747 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF expr block  */
#line 874 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4753 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 876 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4759 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 878 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4765 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block elsif_tail  */
#line 880 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4771 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 882 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4777 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF expr block  */
#line 886 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4783 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 888 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4789 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 890 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4795 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 892 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4801 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 894 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4807 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 896 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4813 "raku.tab.c"
    break;

  case 135: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 900 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4819 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE expr block  */
#line 902 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4825 "raku.tab.c"
    break;

  case 137: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 906 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4831 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 908 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4837 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS expr block  */
#line 910 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4843 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 912 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4849 "raku.tab.c"
    break;

  case 141: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 916 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4855 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL expr block  */
#line 918 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4861 "raku.tab.c"
    break;

  case 143: /* repeat_stmt: KW_REPEAT block  */
#line 922 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4867 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 924 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4873 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 926 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4879 "raku.tab.c"
    break;

  case 146: /* loop_stmt: KW_LOOP block  */
#line 930 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4885 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 932 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4891 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 934 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4897 "raku.tab.c"
    break;

  case 149: /* loop_incr: expr  */
#line 937 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4903 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 941 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4913 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 947 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4923 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 953 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node));
          } else (yyval.node) = rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)); }
#line 4935 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 961 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4945 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 967 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4954 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr block  */
#line 972 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4961 "raku.tab.c"
    break;

  case 156: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 977 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4973 "raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 985 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4986 "raku.tab.c"
    break;

  case 158: /* catch_when_list: KW_WHEN expr block  */
#line 996 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 4992 "raku.tab.c"
    break;

  case 159: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 998 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 4998 "raku.tab.c"
    break;

  case 160: /* when_list: %empty  */
#line 1001 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 5004 "raku.tab.c"
    break;

  case 161: /* when_list: when_list KW_WHEN expr block  */
#line 1003 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 5012 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1009 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5024 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1017 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5034 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1023 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5044 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1029 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5056 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1037 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5066 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1043 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5076 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1049 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5089 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1058 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5100 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1065 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5113 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1074 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5126 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1083 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5137 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list '}'  */
#line 1091 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5143 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr '}'  */
#line 1093 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5150 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1096 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5157 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1099 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5163 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1101 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5169 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1103 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5175 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1105 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5181 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1107 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5187 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1109 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5193 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1111 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5199 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1113 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5206 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1116 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5212 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1118 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5218 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1120 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5224 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1122 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5231 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1125 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5238 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1128 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5246 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list '}'  */
#line 1133 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5252 "raku.tab.c"
    break;

  case 191: /* method_body: '{' YADA '}'  */
#line 1134 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5258 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1136 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-4].list); exprlist_append(l,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5265 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr '}'  */
#line 1139 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5272 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1142 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5279 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1145 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5285 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1147 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5291 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1149 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5297 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1151 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5303 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1153 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5309 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1155 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5315 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1157 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5321 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1159 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5328 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1162 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5334 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1164 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5340 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1166 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5346 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1168 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5353 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1171 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5360 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1174 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5368 "raku.tab.c"
    break;

  case 209: /* pkg_name: IDENT  */
#line 1179 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5374 "raku.tab.c"
    break;

  case 210: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1183 "raku.y"
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
#line 5392 "raku.tab.c"
    break;

  case 211: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1199 "raku.y"
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
#line 5409 "raku.tab.c"
    break;

  case 212: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1214 "raku.y"
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
#line 5426 "raku.tab.c"
    break;

  case 213: /* is_clauses: %empty  */
#line 1228 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5432 "raku.tab.c"
    break;

  case 214: /* is_clauses: is_clauses IDENT IDENT  */
#line 1230 "raku.y"
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
#line 5448 "raku.tab.c"
    break;

  case 215: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1242 "raku.y"
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
#line 5464 "raku.tab.c"
    break;

  case 216: /* class_body_list: %empty  */
#line 1255 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5470 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1257 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5477 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1260 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5484 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1263 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5491 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1266 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5498 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1269 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5505 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1272 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5512 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1275 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5519 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1278 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5526 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1281 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5533 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1284 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5540 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1287 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5547 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1290 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5554 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1293 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5565 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1300 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          free((yyvsp[-5].sval)); free((yyvsp[-4].sval)); free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5575 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1306 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          free((yyvsp[-5].sval)); free((yyvsp[-4].sval)); free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5585 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1312 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5596 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1319 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5607 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1326 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5618 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1333 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5626 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1337 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5634 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1341 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5642 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1345 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5650 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1349 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5661 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1356 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5672 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1363 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5683 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1370 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5694 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1377 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5708 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1387 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5720 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1395 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5732 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1403 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5745 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1412 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5756 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1419 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5767 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1426 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5781 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1436 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5793 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1444 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5805 "raku.tab.c"
    break;

  case 252: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1454 "raku.y"
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
#line 5822 "raku.tab.c"
    break;

  case 253: /* grammar_body_list: %empty  */
#line 1468 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5828 "raku.tab.c"
    break;

  case 254: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1470 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5837 "raku.tab.c"
    break;

  case 255: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1475 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5846 "raku.tab.c"
    break;

  case 256: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1480 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5855 "raku.tab.c"
    break;

  case 257: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1487 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5863 "raku.tab.c"
    break;

  case 258: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1491 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5871 "raku.tab.c"
    break;

  case 259: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1495 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5879 "raku.tab.c"
    break;

  case 260: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1499 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5887 "raku.tab.c"
    break;

  case 261: /* pair_list: IDENT OP_FATARROW expr  */
#line 1505 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5893 "raku.tab.c"
    break;

  case 262: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1507 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5899 "raku.tab.c"
    break;

  case 263: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1509 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5905 "raku.tab.c"
    break;

  case 264: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1511 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5911 "raku.tab.c"
    break;

  case 265: /* param_list: VAR_SCALAR  */
#line 1514 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5917 "raku.tab.c"
    break;

  case 266: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1515 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5923 "raku.tab.c"
    break;

  case 267: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1516 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5929 "raku.tab.c"
    break;

  case 268: /* param_list: VAR_ARRAY  */
#line 1517 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5935 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' VAR_ARRAY  */
#line 1518 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5941 "raku.tab.c"
    break;

  case 270: /* param_list: IDENT VAR_SCALAR  */
#line 1519 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5947 "raku.tab.c"
    break;

  case 271: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1520 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5953 "raku.tab.c"
    break;

  case 272: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1521 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5959 "raku.tab.c"
    break;

  case 273: /* param_list: param_list ',' VAR_SCALAR  */
#line 1522 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5965 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1523 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5971 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1524 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5977 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1525 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5983 "raku.tab.c"
    break;

  case 277: /* param_list: VAR_SCALAR '=' expr  */
#line 1526 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5989 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1527 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5995 "raku.tab.c"
    break;

  case 279: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1528 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 6001 "raku.tab.c"
    break;

  case 280: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1529 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 6007 "raku.tab.c"
    break;

  case 281: /* param_list: SLURPY_POS  */
#line 1530 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6013 "raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' SLURPY_POS  */
#line 1531 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6019 "raku.tab.c"
    break;

  case 283: /* param_list: SLURPY_LOL  */
#line 1532 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6025 "raku.tab.c"
    break;

  case 284: /* param_list: param_list ',' SLURPY_LOL  */
#line 1533 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6031 "raku.tab.c"
    break;

  case 285: /* param_list: SLURPY_NAMED  */
#line 1534 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6037 "raku.tab.c"
    break;

  case 286: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1535 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6043 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list '}'  */
#line 1538 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 6049 "raku.tab.c"
    break;

  case 288: /* block: '{' YADA '}'  */
#line 1539 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6055 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr '}'  */
#line 1541 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6061 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1543 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6067 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1545 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6073 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1547 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6079 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1549 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6085 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1551 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6092 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1554 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6098 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1556 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6104 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1558 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6110 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1560 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 6116 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1562 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6123 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1565 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6130 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1568 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6138 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1572 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6145 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1575 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6152 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1578 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6159 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1581 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 6165 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1583 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6172 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1586 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6179 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1589 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6186 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1592 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6193 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1595 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6200 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1598 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 6206 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1600 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6212 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1602 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6218 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST '}'  */
#line 1604 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6224 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1606 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6230 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1608 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6236 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1610 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6242 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1612 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6248 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1614 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6254 "raku.tab.c"
    break;

  case 320: /* closure: '{' expr '}'  */
#line 1617 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6260 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR '=' expr  */
#line 1620 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6266 "raku.tab.c"
    break;

  case 322: /* expr: VAR_ARRAY '=' expr  */
#line 1621 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6272 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1622 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6278 "raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1623 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6284 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1624 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6290 "raku.tab.c"
    break;

  case 326: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1625 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6296 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1626 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6302 "raku.tab.c"
    break;

  case 328: /* expr: KW_GATHER block  */
#line 1627 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6312 "raku.tab.c"
    break;

  case 329: /* expr: KW_GATHER for_stmt  */
#line 1632 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6322 "raku.tab.c"
    break;

  case 330: /* expr: tern_expr OP_FATARROW expr  */
#line 1638 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6328 "raku.tab.c"
    break;

  case 331: /* expr: tern_expr  */
#line 1639 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6334 "raku.tab.c"
    break;

  case 332: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1643 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6340 "raku.tab.c"
    break;

  case 333: /* tern_expr: or_expr  */
#line 1644 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6346 "raku.tab.c"
    break;

  case 334: /* or_expr: or_expr OP_OR and_expr  */
#line 1647 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6352 "raku.tab.c"
    break;

  case 335: /* or_expr: and_expr  */
#line 1648 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6358 "raku.tab.c"
    break;

  case 336: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1651 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6364 "raku.tab.c"
    break;

  case 337: /* and_expr: cmp_expr  */
#line 1652 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6370 "raku.tab.c"
    break;

  case 338: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1655 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 6376 "raku.tab.c"
    break;

  case 339: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1656 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 6382 "raku.tab.c"
    break;

  case 340: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1657 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 6388 "raku.tab.c"
    break;

  case 341: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1658 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 6394 "raku.tab.c"
    break;

  case 342: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1659 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 6400 "raku.tab.c"
    break;

  case 343: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1660 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 6406 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1661 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6412 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1662 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6418 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1663 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6424 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1664 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6430 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1665 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6436 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1666 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6442 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1667 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6448 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1668 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6454 "raku.tab.c"
    break;

  case 352: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1669 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6460 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1671 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 6470 "raku.tab.c"
    break;

  case 354: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1677 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 6480 "raku.tab.c"
    break;

  case 355: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1683 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 6490 "raku.tab.c"
    break;

  case 356: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1689 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 6500 "raku.tab.c"
    break;

  case 357: /* cmp_expr: divis_expr  */
#line 1694 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 6506 "raku.tab.c"
    break;

  case 358: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1697 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6512 "raku.tab.c"
    break;

  case 359: /* divis_expr: jct_expr  */
#line 1698 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6518 "raku.tab.c"
    break;

  case 360: /* jct_expr: jct_expr '|' range_expr  */
#line 1701 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6524 "raku.tab.c"
    break;

  case 361: /* jct_expr: jct_expr '&' range_expr  */
#line 1702 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6530 "raku.tab.c"
    break;

  case 362: /* jct_expr: dor_expr  */
#line 1703 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6536 "raku.tab.c"
    break;

  case 363: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1707 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6542 "raku.tab.c"
    break;

  case 364: /* dor_expr: range_expr  */
#line 1708 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6548 "raku.tab.c"
    break;

  case 365: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1711 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6554 "raku.tab.c"
    break;

  case 366: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1712 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6560 "raku.tab.c"
    break;

  case 367: /* range_expr: add_expr  */
#line 1713 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6566 "raku.tab.c"
    break;

  case 368: /* add_expr: add_expr '~' repl_expr  */
#line 1716 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6572 "raku.tab.c"
    break;

  case 369: /* add_expr: repl_expr  */
#line 1717 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6578 "raku.tab.c"
    break;

  case 370: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1720 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6584 "raku.tab.c"
    break;

  case 371: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1721 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6590 "raku.tab.c"
    break;

  case 372: /* repl_expr: addsub_expr  */
#line 1722 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6596 "raku.tab.c"
    break;

  case 373: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1725 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6602 "raku.tab.c"
    break;

  case 374: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1726 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6608 "raku.tab.c"
    break;

  case 375: /* addsub_expr: mul_expr  */
#line 1727 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6614 "raku.tab.c"
    break;

  case 376: /* mul_expr: mul_expr '*' unary_expr  */
#line 1730 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6620 "raku.tab.c"
    break;

  case 377: /* mul_expr: mul_expr '/' unary_expr  */
#line 1731 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6626 "raku.tab.c"
    break;

  case 378: /* mul_expr: mul_expr '%' unary_expr  */
#line 1732 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6632 "raku.tab.c"
    break;

  case 379: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1734 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6638 "raku.tab.c"
    break;

  case 380: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1736 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6644 "raku.tab.c"
    break;

  case 381: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1738 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6650 "raku.tab.c"
    break;

  case 382: /* mul_expr: unary_expr  */
#line 1739 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6656 "raku.tab.c"
    break;

  case 383: /* unary_expr: '-' unary_expr  */
#line 1742 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6662 "raku.tab.c"
    break;

  case 384: /* unary_expr: '+' unary_expr  */
#line 1743 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6668 "raku.tab.c"
    break;

  case 385: /* unary_expr: '!' unary_expr  */
#line 1744 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6674 "raku.tab.c"
    break;

  case 386: /* unary_expr: CARET unary_expr  */
#line 1745 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6680 "raku.tab.c"
    break;

  case 387: /* unary_expr: OP_REDUCE unary_expr  */
#line 1747 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6689 "raku.tab.c"
    break;

  case 388: /* unary_expr: pow_expr  */
#line 1751 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6695 "raku.tab.c"
    break;

  case 389: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1754 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6701 "raku.tab.c"
    break;

  case 390: /* pow_expr: postfix_expr  */
#line 1755 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6707 "raku.tab.c"
    break;

  case 391: /* scalar_list: VAR_SCALAR  */
#line 1758 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6713 "raku.tab.c"
    break;

  case 392: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1759 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6719 "raku.tab.c"
    break;

  case 393: /* meth_name: IDENT  */
#line 1762 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6725 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_SORT  */
#line 1763 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6731 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_REVERSE  */
#line 1764 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6737 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_MAP  */
#line 1765 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6743 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_GREP  */
#line 1766 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6749 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_SAY  */
#line 1767 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6755 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_PRINT  */
#line 1768 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6761 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_TAKE  */
#line 1769 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6767 "raku.tab.c"
    break;

  case 401: /* meth_name: KW_RETURN  */
#line 1770 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6773 "raku.tab.c"
    break;

  case 402: /* meth_name: KW_EXISTS  */
#line 1771 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6779 "raku.tab.c"
    break;

  case 403: /* meth_name: KW_DELETE  */
#line 1772 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6785 "raku.tab.c"
    break;

  case 404: /* meth_name: KW_JOIN  */
#line 1773 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6791 "raku.tab.c"
    break;

  case 405: /* meth_name: TESTOP  */
#line 1774 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6797 "raku.tab.c"
    break;

  case 406: /* postfix_expr: call_expr  */
#line 1776 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6803 "raku.tab.c"
    break;

  case 407: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1779 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6813 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '(' arg_list ')'  */
#line 1785 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6822 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '(' ')'  */
#line 1789 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6828 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1791 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6834 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1793 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6840 "raku.tab.c"
    break;

  case 412: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1795 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6849 "raku.tab.c"
    break;

  case 413: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1800 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6855 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1802 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { free((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 6868 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1811 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { free((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 6881 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1820 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { free((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 6892 "raku.tab.c"
    break;

  case 417: /* call_expr: IDENT '.' KW_NEW  */
#line 1827 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6900 "raku.tab.c"
    break;

  case 418: /* call_expr: IDENT '.' IDENT  */
#line 1831 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6909 "raku.tab.c"
    break;

  case 419: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1836 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6920 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1843 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6929 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '.' CARET IDENT  */
#line 1848 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6939 "raku.tab.c"
    break;

  case 422: /* call_expr: atom '.' CARET IDENT  */
#line 1854 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6949 "raku.tab.c"
    break;

  case 423: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1860 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6960 "raku.tab.c"
    break;

  case 424: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1867 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6971 "raku.tab.c"
    break;

  case 425: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1874 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6980 "raku.tab.c"
    break;

  case 426: /* call_expr: atom '.' meth_name  */
#line 1879 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6989 "raku.tab.c"
    break;

  case 427: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1884 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7000 "raku.tab.c"
    break;

  case 428: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1891 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7009 "raku.tab.c"
    break;

  case 429: /* call_expr: call_expr '.' meth_name  */
#line 1896 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7018 "raku.tab.c"
    break;

  case 430: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 1901 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7029 "raku.tab.c"
    break;

  case 431: /* call_expr: '.' meth_name '(' ')'  */
#line 1908 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7038 "raku.tab.c"
    break;

  case 432: /* call_expr: '.' meth_name  */
#line 1913 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7047 "raku.tab.c"
    break;

  case 433: /* call_expr: KW_DIE expr  */
#line 1918 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7053 "raku.tab.c"
    break;

  case 434: /* call_expr: KW_MAP closure expr  */
#line 1920 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7059 "raku.tab.c"
    break;

  case 435: /* call_expr: KW_MAP closure ',' expr  */
#line 1922 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7065 "raku.tab.c"
    break;

  case 436: /* call_expr: KW_GREP closure expr  */
#line 1924 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7071 "raku.tab.c"
    break;

  case 437: /* call_expr: KW_SORT expr  */
#line 1926 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7077 "raku.tab.c"
    break;

  case 438: /* call_expr: KW_SORT closure expr  */
#line 1928 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7083 "raku.tab.c"
    break;

  case 439: /* call_expr: KW_REVERSE expr  */
#line 1930 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7089 "raku.tab.c"
    break;

  case 440: /* call_expr: atom  */
#line 1931 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7095 "raku.tab.c"
    break;

  case 441: /* arg_list: expr  */
#line 1934 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7101 "raku.tab.c"
    break;

  case 442: /* arg_list: arg_list ',' expr  */
#line 1935 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7107 "raku.tab.c"
    break;

  case 443: /* arg_list: arg_list ','  */
#line 1936 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7113 "raku.tab.c"
    break;

  case 444: /* atom: LIT_INT  */
#line 1939 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7119 "raku.tab.c"
    break;

  case 445: /* atom: LIT_BOOL  */
#line 1941 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7127 "raku.tab.c"
    break;

  case 446: /* atom: LIT_FLOAT  */
#line 1944 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7133 "raku.tab.c"
    break;

  case 447: /* atom: LIT_STR  */
#line 1945 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7139 "raku.tab.c"
    break;

  case 448: /* atom: WORDLIST  */
#line 1947 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7152 "raku.tab.c"
    break;

  case 449: /* atom: LIT_INTERP_STR  */
#line 1955 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7158 "raku.tab.c"
    break;

  case 450: /* atom: VAR_SCALAR  */
#line 1956 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7164 "raku.tab.c"
    break;

  case 451: /* atom: OP_INC VAR_SCALAR  */
#line 1957 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7170 "raku.tab.c"
    break;

  case 452: /* atom: OP_DEC VAR_SCALAR  */
#line 1958 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7176 "raku.tab.c"
    break;

  case 453: /* atom: VAR_SCALAR OP_INC  */
#line 1959 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7182 "raku.tab.c"
    break;

  case 454: /* atom: VAR_SCALAR OP_DEC  */
#line 1960 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7188 "raku.tab.c"
    break;

  case 455: /* atom: VAR_TWIGIL OP_INC  */
#line 1961 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); free((yyvsp[-1].sval)); }
#line 7194 "raku.tab.c"
    break;

  case 456: /* atom: VAR_TWIGIL OP_DEC  */
#line 1962 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); free((yyvsp[-1].sval)); }
#line 7200 "raku.tab.c"
    break;

  case 457: /* atom: VAR_ARRAY  */
#line 1963 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7206 "raku.tab.c"
    break;

  case 458: /* atom: VAR_HASH  */
#line 1964 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7212 "raku.tab.c"
    break;

  case 459: /* atom: VAR_CAPTURE  */
#line 1966 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7220 "raku.tab.c"
    break;

  case 460: /* atom: VAR_FH  */
#line 1970 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7228 "raku.tab.c"
    break;

  case 461: /* atom: VAR_NAMED_CAPTURE  */
#line 1974 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 7235 "raku.tab.c"
    break;

  case 462: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1977 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 7241 "raku.tab.c"
    break;

  case 463: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1979 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 7247 "raku.tab.c"
    break;

  case 464: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1981 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 7253 "raku.tab.c"
    break;

  case 465: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1983 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 7259 "raku.tab.c"
    break;

  case 466: /* atom: ARR_ALL_SLICE  */
#line 1985 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 7265 "raku.tab.c"
    break;

  case 467: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1987 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7271 "raku.tab.c"
    break;

  case 468: /* atom: VAR_HASH '{' expr '}'  */
#line 1989 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7277 "raku.tab.c"
    break;

  case 469: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1991 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7283 "raku.tab.c"
    break;

  case 470: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1993 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7289 "raku.tab.c"
    break;

  case 471: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 1995 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7295 "raku.tab.c"
    break;

  case 472: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 1997 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7301 "raku.tab.c"
    break;

  case 473: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 1999 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7307 "raku.tab.c"
    break;

  case 474: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2001 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7313 "raku.tab.c"
    break;

  case 475: /* atom: IDENT  */
#line 2002 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7319 "raku.tab.c"
    break;

  case 476: /* atom: VAR_TWIGIL  */
#line 2004 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7327 "raku.tab.c"
    break;

  case 477: /* atom: VAR_ARRAY_TWIGIL  */
#line 2008 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7335 "raku.tab.c"
    break;

  case 478: /* atom: VAR_HASH_TWIGIL  */
#line 2012 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7343 "raku.tab.c"
    break;

  case 479: /* atom: '[' ']'  */
#line 2015 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 7349 "raku.tab.c"
    break;

  case 480: /* atom: '[' expr ']'  */
#line 2017 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7355 "raku.tab.c"
    break;

  case 481: /* atom: '[' expr ',' ']'  */
#line 2019 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7361 "raku.tab.c"
    break;

  case 482: /* atom: '[' expr ',' arg_list ']'  */
#line 2021 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7368 "raku.tab.c"
    break;

  case 483: /* atom: DOLLAR_LBRACKET ']'  */
#line 2023 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 7374 "raku.tab.c"
    break;

  case 484: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2025 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7380 "raku.tab.c"
    break;

  case 485: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2027 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7386 "raku.tab.c"
    break;

  case 486: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2029 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7393 "raku.tab.c"
    break;

  case 487: /* atom: '(' ')'  */
#line 2031 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7399 "raku.tab.c"
    break;

  case 488: /* atom: '(' expr ')'  */
#line 2032 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7405 "raku.tab.c"
    break;

  case 489: /* atom: '(' expr ',' ')'  */
#line 2034 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7411 "raku.tab.c"
    break;

  case 490: /* atom: '(' expr ',' arg_list ')'  */
#line 2036 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7418 "raku.tab.c"
    break;

  case 491: /* atom: block  */
#line 2038 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7424 "raku.tab.c"
    break;

  case 492: /* atom: KW_SUB block  */
#line 2039 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7430 "raku.tab.c"
    break;

  case 493: /* atom: KW_SUB '(' param_list ')' block  */
#line 2041 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 7437 "raku.tab.c"
    break;

  case 494: /* atom: OP_ARROW scalar_list block  */
#line 2044 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 7444 "raku.tab.c"
    break;


#line 7448 "raku.tab.c"

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

#line 2047 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
