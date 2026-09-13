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
     814,   816,   817,   818,   819,   820,   821,   822,   824,   826,
     828,   830,   831,   832,   833,   834,   837,   839,   841,   843,
     845,   847,   849,   851,   855,   857,   859,   861,   863,   865,
     869,   871,   875,   877,   879,   881,   885,   887,   891,   893,
     895,   899,   901,   903,   907,   910,   916,   922,   930,   936,
     941,   946,   954,   965,   966,   972,   980,   986,   992,  1000,
    1006,  1012,  1021,  1028,  1037,  1046,  1055,  1062,  1069,  1070,
    1073,  1076,  1078,  1080,  1082,  1084,  1086,  1088,  1090,  1093,
    1095,  1097,  1099,  1102,  1105,  1111,  1112,  1113,  1116,  1119,
    1121,  1123,  1125,  1127,  1129,  1131,  1133,  1136,  1138,  1140,
    1142,  1145,  1148,  1154,  1155,  1158,  1174,  1189,  1204,  1205,
    1217,  1231,  1232,  1235,  1238,  1241,  1244,  1247,  1250,  1253,
    1256,  1259,  1262,  1265,  1268,  1275,  1282,  1289,  1296,  1300,
    1304,  1308,  1312,  1319,  1326,  1333,  1340,  1350,  1358,  1366,
    1375,  1382,  1389,  1399,  1407,  1417,  1432,  1433,  1438,  1443,
    1450,  1454,  1458,  1462,  1468,  1470,  1472,  1474,  1478,  1479,
    1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,
    1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,  1498,  1499,
    1502,  1503,  1504,  1506,  1508,  1510,  1512,  1514,  1517,  1519,
    1521,  1523,  1525,  1528,  1531,  1535,  1538,  1541,  1544,  1546,
    1550,  1553,  1556,  1559,  1562,  1565,  1567,  1569,  1571,  1573,
    1575,  1577,  1579,  1581,  1585,  1588,  1589,  1590,  1591,  1592,
    1593,  1594,  1595,  1600,  1605,  1607,  1610,  1612,  1615,  1616,
    1619,  1620,  1623,  1624,  1625,  1626,  1627,  1628,  1629,  1630,
    1631,  1632,  1633,  1634,  1635,  1641,  1647,  1653,  1656,  1657,
    1660,  1661,  1662,  1665,  1667,  1670,  1671,  1672,  1675,  1676,
    1679,  1680,  1681,  1684,  1685,  1686,  1689,  1690,  1691,  1692,
    1694,  1696,  1698,  1701,  1702,  1703,  1704,  1705,  1710,  1713,
    1714,  1717,  1718,  1721,  1722,  1723,  1724,  1725,  1726,  1727,
    1728,  1729,  1730,  1731,  1732,  1733,  1735,  1737,  1743,  1748,
    1749,  1751,  1753,  1758,  1760,  1766,  1772,  1776,  1780,  1785,
    1792,  1797,  1803,  1809,  1816,  1823,  1828,  1833,  1840,  1845,
    1850,  1852,  1854,  1856,  1858,  1860,  1862,  1864,  1867,  1868,
    1869,  1872,  1873,  1874,  1875,  1884,  1885,  1886,  1887,  1888,
    1889,  1890,  1891,  1892,  1896,  1900,  1903,  1905,  1907,  1909,
    1911,  1913,  1915,  1917,  1919,  1921,  1923,  1925,  1927,  1929,
    1930,  1934,  1938,  1942,  1943,  1945,  1947,  1950,  1951,  1953,
    1955,  1958,  1959,  1960,  1962,  1965,  1966
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
    -887,    50,  2416,  -887,  -887,  -887,  -887,  -887,   753,   -41,
      16,   -47,    32,  -887,  -887,  5411,  2514,  -887,  -887,  -887,
     270,  2592,   143,  4397,  4475,  4553,  4631,  4709,    -1,   -14,
    4709,  2670,  2748,    48,   125,  4709,  4709,   179,   200,  4787,
    4865,    83,   -51,    62,   239,    92,    92,  4943,  4709,    83,
      83,  4709,   482,   482,    38,  5411,  -887,  -887,   482,   482,
     243,   258,  5411,  5411,  5411,  -887,  2826,  2905,   237,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,   565,
    -887,  -887,  -887,  -887,  5470,   630,   244,   539,   259,   403,
     636,   597,   255,  -887,   192,   804,   796,   408,  -887,  -887,
     278,   245,   275,   413,  -887,  -887,  4709,  4709,  4709,  4709,
    4709,  4709,   423,  2983,  5021,  3062,   440,  4709,  4709,   366,
     185,   811,   104,   386,   115,  -887,   409,    83,  3140,  -887,
    -887,   558,   232,  -887,   417,  3219,  3297,  -887,  3375,  -887,
     108,   159,   257,   264,   827,   492,   465,   508,  3140,   557,
    3140,    26,  3140,    83,  3140,    83,   113,   203,    19,  -887,
    -887,  -887,   428,  -887,   299,  -887,   302,   430,   448,   459,
     442,   449,   201,   235,  3140,    83,  3140,    83,   352,  3454,
    -887,  4709,  4709,  -887,  4709,  4709,  -887,  4709,  3532,  4709,
    3610,  4709,  -887,  -887,   536,  -887,  -887,  -887,  -887,  -887,
     475,   535,    12,  -887,   544,   571,  -887,  -887,  -887,  -887,
    -887,   800,  -887,   797,   807,  -887,   606,   595,  1598,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  4709,  -887,  4709,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  -887,  4709,  5411,  5411,
    5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,
    5411,  5411,  5411,   884,  5411,  5411,  5411,  5411,  5411,  5411,
    5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,  5411,
    5411,  5411,  1010,   975,   806,   617,   627,   667,   710,   719,
     742,   -50,  -887,   845,  3140,   520,   836,   679,   764,   771,
     793,  -887,   810,   936,   815,    -8,  -887,   940,   849,   857,
    3062,   943,  4709,  4709,  4709,  4709,  4709,  4709,  4709,  4709,
    3688,  -887,  -887,   826,  -887,   864,   897,   797,   859,  -887,
    4709,  4709,  -887,  5099,  -887,  5177,  -887,   311,   330,   348,
     172,   863,   922,  -887,   865,   867,  4709,  4709,  4709,  4709,
    4709,  4709,  4709,  -887,  4709,   870,  4709,  4709,  -887,  4709,
     874,   987,   881,  -887,   508,  4709,  -887,  5411,  5411,     6,
    -887,  -887,  -887,  4709,  4709,  4709,  -887,  4709,  4709,  -887,
    4709,  4709,   899,  4709,  -887,   967,  4709,  1035,  4709,   887,
    1026,   889,  -887,  4709,  4709,  1053,   933,   950,   963,  1020,
    1021,   949,  4709,  -887,  -887,  -887,    83,    18,  -887,    25,
     981,  1001,  -887,  -887,  -887,  3766,  1046,  1093,  3845,  -887,
    -887,   965,   269,   306,  1047,  4397,  4475,  4709,  2119,    35,
     217,  -887,   380,  1034,  1036,  1049,  1057,  1058,  1059,  1060,
    1061,  1062,  1063,  1064,  1065,  1066,  1069,  1070,  1071,  -887,
     259,  1107,   403,  1030,  1030,  1030,  1030,  1030,  1030,  1030,
    1030,  1030,  1030,  1030,  1030,  -887,  -887,  -887,   597,  -887,
    -887,  -887,  1043,  1043,   804,   796,   796,   408,   408,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  1072,
    1197,  1075,  -887,  3923,  -887,  -887,  -887,  -887,  -887,  -887,
    4709,  4002,  -887,   900,  -887,  4709,  4709,  4709,  4709,  1091,
     202,   373,  -887,  4081,  -887,  1170,  4709,  1077,  -887,    -6,
    -887,  1456,   743,  1092,  1076,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,   746,  -887,  -887,  -887,  1082,  -887,  1084,
    3140,   521,  1137,     0,  4160,   545,  1086,  4709,  -887,  5255,
    -887,  4709,  -887,    30,  -887,  4709,  4709,  1101,  3766,  1095,
    1097,  1098,  1099,  1105,  1108,  1110,   561,  3766,  1111,  1112,
     569,    83,   -18,  5333,  -887,    83,   580,   210,   -40,   -39,
       3,  -887,    45,  -887,  -887,  -887,  1087,   904,  1739,   666,
    1114,  1115,  1116,  1118,  1119,  1120,  -887,  1089,    51,  1117,
    1100,  1124,  1122,    83,    83,    83,  1123,  1125,  1136,  4709,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  1246,  1248,  -887,
     230,  1087,   909,    46,   378,   337,  1880,  -887,   913,  4709,
    -887,  -887,   774,  1249,  3062,  4709,  4709,   412,    72,   374,
    -887,   228,  4709,  4709,  -887,  4709,  4709,  -887,  4709,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  5411,  4239,  -887,  1312,  1130,   915,  1131,
    -887,   920,  3766,   668,  1128,  1129,   779,  4709,  -887,  -887,
    4709,  -887,  -887,  4709,  -887,   923,  -887,   939,   942,  -887,
    4709,  1187,  1258,   944,  -887,   970,   974,  -887,  -887,  -887,
     952,  -887,  4709,  4709,  4709,   956,  -887,   109,  -887,  1138,
    3140,   711,  1141,  1087,   958,  1142,  1143,  4709,   966,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,   968,  -887,  -887,
    -887,  1079,  -887,  -887,  3140,    83,  -887,  -887,  1263,  -887,
    1270,  1271,  1268,  4709,  1161,  1274,  1275,  -887,  1087,   549,
    4397,  4475,  2197,  -887,   414,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  4709,    83,  -887,  -887,  -887,  1150,  1151,  1259,
    -887,  -887,  -887,  -887,  4709,  1153,  -887,  -887,   360,   680,
     602,  1228,  -887,  -887,  1087,  1087,   972,   976,  1277,  1278,
    1280,  -887,  -887,  -887,  1159,  -887,    27,   780,  1154,    70,
    4709,  4709,  4709,  4709,  4709,  4709,  -887,  -887,  -887,  -887,
     296,   365,   379,   394,   425,   437,   447,   451,   468,   487,
     493,   499,  -887,  -887,   978,  -887,   982,   984,  -887,  1160,
    -887,  -887,   989,  -887,  -887,  -887,  -887,  1162,  1165,  1166,
    -887,  -887,  -887,  1167,  4709,  1169,  -887,  3766,   733,  -887,
    -887,  1168,  1219,  1225,  -887,   992,  -887,  4709,  -887,  -887,
    1087,  -887,  -887,   734,  1175,  1177,   -18,  -887,   994,  1083,
      83,    83,    83,  -887,  -887,  4709,  -887,  -887,  -887,    29,
    -887,   256,  -887,  -887,  -887,   433,   147,  -887,   251,  4709,
    4709,  4709,  4709,  4709,  4709,  4709,  4709,  -887,    83,  1176,
    -887,  -887,    83,  1184,  4709,  -887,   265,   334,    37,    68,
     519,  1185,  1191,  1010,  -887,  -887,  1087,  1087,  1328,  1331,
    1332,  -887,  4709,  4318,  1218,   385,  -887,   540,   546,   556,
     566,   567,   578,  -887,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  -887,  1205,
    -887,  -887,  -887,  -887,  -887,  4709,   996,  -887,  -887,  4709,
    4709,  3766,   738,  -887,  -887,  4709,  -887,  -887,  -887,  -887,
      83,    83,  -887,  -887,  -887,  -887,  -887,  1329,  4709,  1220,
    1334,  1337,  4709,  4709,  4709,  -887,  -887,  -887,   603,   605,
     607,   613,   619,   621,   625,   629,  -887,  -887,  -887,  4709,
    1210,  -887,    59,  1272,  -887,   191,  -887,  1335,  1339,  1215,
    4709,  -887,  1340,  1341,  1221,  4709,  -887,    71,   234,  1223,
    1224,  -887,  -887,   466,  -887,  -887,  -887,  -887,  -887,   631,
    1213,   998,  4709,  4709,  -887,  -887,  -887,  -887,  -887,  -887,
    -887,  1226,  1227,  -887,  -887,  1002,  -887,   740,  1113,  -887,
    -887,  -887,  4709,  -887,  -887,   633,   635,   637,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  1229,    83,  1231,  1006,
    1232,  2021,  1231,  1008,  1234,  1235,  -887,  1238,  1240,  1241,
    -887,  1242,  1347,  1348,  1243,  4709,  -887,  1366,  1369,  1250,
    4709,  -887,  -887,  -887,   240,  -887,  -887,  -887,  1245,   641,
     643,  -887,  -887,  1253,  -887,    83,  -887,  -887,  -887,  -887,
    -887,  4709,  -887,  -887,  1231,  -887,  4397,  4475,  2275,  -887,
     441,  -887,  1231,  -887,  -887,  -887,  -887,  -887,  -887,  1254,
    1255,  -887,  1256,  1257,  1260,  -887,  1261,  1231,  1012,  -887,
    -887,  -887,  -887,  -887,  1247,  -887,   534,   212,  -887,   298,
    4709,  4709,  4709,  4709,  4709,  4709,  4709,  4709,  -887,  -887,
    -887,  -887,  -887,  -887,  -887,  -887,  -887,  1231,    83,  4709,
    4709,  4709,  -887,  -887,  -887,   645,   649,   653,   655,   657,
     658,   663,   683,  -887,  -887,   698,   699,   700,  -887,  -887,
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
    -887,   263,  1365,  -887,  -887,  -887,  -887,  -327,  -886,   795,
    -887,  -887,  -887,  -887,   776,  -887,  -887,  -506,   854,  -387,
     356,  1106,    -2,  -237,  -887,  1163,  1164,   728,  1145,  -887,
     639,   -16,  1140,   893,   903,     4,  -887,  -140,  -269,  -887,
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
      85,     2,   441,   554,   491,   877,   742,   334,   701,   697,
      25,   157,   622,   158,   134,   703,   580,   581,    27,   130,
     582,   149,   151,   153,   155,   156,   400,   752,   162,   164,
     166,  1016,   617,   170,   171,   580,   581,   175,   177,   582,
     580,   581,   618,   119,   582,   192,   193,   750,   751,   196,
       3,  1017,   201,   987,   346,   754,   580,   581,   167,   203,
     582,  1018,   168,   642,   213,   216,   208,   209,   210,   580,
     581,   202,   500,   582,   347,   118,   516,   583,   584,   585,
     401,   114,  1022,   643,   714,  1092,   179,   501,   260,   260,
     181,    68,  1023,   772,   773,  1093,   583,   584,   585,   115,
     346,   583,   584,   585,   275,   276,   277,   278,   279,   280,
     182,  1019,   285,   287,   862,   289,   290,   583,   584,   585,
     347,   755,   756,   863,    68,   753,   213,   120,    68,   121,
     583,   584,   585,   313,   315,   120,   317,   121,   702,   169,
     116,    68,  1024,   982,   586,  1094,   335,  1105,   345,   932,
     350,   988,   352,   141,   142,   143,   359,   144,   117,  1020,
     619,   360,   348,   621,   933,   349,   724,   120,   723,   121,
     836,   183,   379,  1021,   381,   346,   145,   386,   644,   387,
     388,   146,   389,   390,   795,   391,   393,   394,   391,   395,
    1025,   172,  1123,  1095,   774,   347,  1131,  1078,   183,   292,
     354,   580,   581,   293,  1026,   582,   512,  1096,   348,   104,
     105,   349,   173,   936,   576,   817,   422,   423,   424,   425,
     426,   427,   428,   429,   430,    68,   431,   432,   433,   434,
     435,   436,   437,   438,   187,   439,   796,   797,  1155,   301,
     346,   113,   462,   463,   319,   645,  1169,   320,  1097,   294,
     580,   581,   355,   206,   582,    68,   364,   302,  1098,   757,
     347,  1176,   583,   584,   585,   646,   989,   184,   207,   469,
     470,   471,   472,   473,   474,   475,   365,   258,   259,   364,
     147,   321,   503,   348,   135,   136,   349,   185,   357,   358,
     996,  1193,   789,   790,   793,   322,   791,   748,   522,   365,
     524,   525,   526,   527,   528,   529,   530,   531,  1099,   553,
     978,   583,   584,   585,   360,   217,   688,   689,   538,   539,
     260,   541,  1116,   546,   690,   375,   364,   364,   237,  1082,
     367,   260,   990,   991,   559,   560,   561,   562,   563,   564,
     565,   578,   579,   376,   568,   569,   365,   365,   348,   320,
     368,   349,    68,   186,   309,  1183,  1100,   240,    84,   377,
     647,   588,   590,   591,   366,   592,   593,   257,   594,   595,
    1101,   819,   300,   792,   600,   186,   602,   378,  1147,   323,
     272,   606,   607,   383,   159,   161,   325,   366,   580,   581,
     615,   114,   582,   324,   997,   283,   869,   178,   180,   271,
     326,   384,  1012,   299,   626,   194,   195,  1013,   648,   634,
     273,   649,   650,   637,   638,   639,   641,   798,   799,   800,
     318,   651,   789,   790,   652,   653,   791,   274,   654,   655,
     116,   888,   610,   547,   366,   366,   832,   281,   369,   943,
     810,  1184,   899,   337,   811,   900,   901,   548,   635,   583,
     584,   585,   549,   812,   288,   902,   813,   814,   903,   904,
     815,   992,   905,   906,   337,   993,   550,   924,   925,  1160,
     551,  1015,  1161,  1162,   339,   331,  1013,   340,   341,   332,
     801,   994,  1163,   159,   552,  1164,  1165,   691,   692,  1166,
    1167,   241,   242,   243,   244,   693,   197,   198,   679,   691,
     692,   611,   291,   915,   684,   685,   330,  1043,   944,   351,
     362,   353,   356,   363,   699,   612,   236,   818,   333,   538,
     917,   265,   945,   656,   266,   267,   300,   245,   246,  1027,
     613,   380,  1028,   382,   372,  1029,  1030,   946,   710,   268,
     269,   270,   213,   973,   120,   719,   121,   721,   343,   722,
     236,   344,   370,   725,   726,   816,   310,   907,   311,   889,
     890,   665,  1179,   891,   362,   337,  1180,   363,   947,   343,
     371,   745,   344,   666,    84,   339,   995,   236,   340,   341,
     948,   373,  1181,   667,  1168,   336,   764,   668,   337,   338,
     949,   374,   533,   219,   950,   396,   220,   221,   339,  1034,
    1035,   340,   341,  1104,   669,   342,   222,   785,  1013,   223,
     224,   951,   918,   225,   226,   919,   920,   398,   921,   922,
     892,   893,   894,   670,    85,  1079,   566,   804,  1083,   671,
     952,   570,   807,   808,   809,   672,   953,   577,   238,   239,
     820,   821,   954,   822,   823,   589,   824,   825,   826,   827,
     828,   829,   830,   831,  1033,   597,   504,   711,   228,   505,
     712,   229,   230,   104,   105,   303,   304,   305,   306,   307,
     343,   231,   399,   344,   232,   233,   729,  1182,   234,   235,
     308,   716,   731,  1044,   717,   847,   402,   628,   848,  1045,
     632,   849,   732,   343,   476,   113,   344,   736,   853,  1046,
     320,   227,   733,   734,   477,   740,   478,   479,   320,  1047,
    1048,   859,   860,   403,   735,   480,   481,  1148,   865,   407,
     482,  1049,    68,   255,   256,   873,   483,   484,   247,   248,
     249,   250,   251,   252,   485,   486,   487,   488,   410,   665,
     253,   666,   878,   667,   916,   408,  1068,   409,  1069,   668,
    1070,   884,   616,   494,   254,   669,  1071,   670,   895,   896,
     898,   671,  1072,   495,  1073,   672,   236,   840,  1074,   729,
     908,   731,  1075,   735,  1106,   678,  1118,   960,  1119,   962,
    1120,   665,   913,   681,  1150,   666,  1151,   683,  1198,   667,
     686,   668,  1199,   669,   670,   695,  1200,   698,  1201,   671,
    1202,  1203,   765,   496,   843,   320,  1204,   320,   937,   938,
     939,   940,   941,   942,     4,     5,     6,     7,   508,   672,
     509,   131,   132,   124,   125,   295,  1205,    13,    14,    15,
      16,    17,    18,    19,   729,   731,   735,   327,   328,   329,
     728,  1206,  1207,  1208,   127,    29,   497,   866,   200,   737,
     867,    35,   964,   204,   205,   498,   103,    37,   104,   105,
     106,   107,   108,   109,   110,    45,    46,    47,    48,   967,
     974,    51,   320,   975,  1056,   111,  1114,   320,   499,   320,
      55,    56,   508,   986,   704,   320,    57,   707,   112,   510,
     113,   455,   456,   457,   459,   460,   461,   998,   999,  1000,
    1001,  1002,  1003,  1004,  1005,   104,   105,   303,   304,   305,
     306,   307,  1011,   320,   511,   805,    60,    61,   320,   508,
     846,   934,   308,   261,   262,   263,   264,   741,   743,   512,
    1039,   746,   747,   749,    62,   404,   405,   113,  -391,  -391,
      63,    64,   492,   493,    84,   406,   407,   513,   128,   296,
     514,    67,   515,    68,   517,   297,   834,   523,   837,   779,
     780,   781,   534,  1051,   842,   506,   507,  1053,  1054,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   599,    84,   502,   320,   555,  1061,   518,   519,   476,
    1065,  1066,  1067,   490,   858,   520,   521,   537,   320,   477,
     535,   478,   479,   557,   407,   404,   558,  1076,   404,   567,
     480,   481,   571,   405,  1081,   482,   572,   573,  1087,   575,
     405,   483,   484,  1091,   476,   603,   405,   605,   405,   485,
     486,   487,   488,   536,   477,   596,   478,   479,   404,   682,
    1109,  1110,   758,   759,   556,   480,   481,   794,   759,   601,
     482,   803,   320,   839,   320,   604,   483,   484,   841,   320,
    1117,   850,   320,   608,   485,   486,   487,   488,   103,   609,
     104,   105,   106,   107,   108,   109,   110,   851,   519,  1130,
     852,   320,   856,   519,   688,   689,   610,   111,   691,   692,
     404,   857,   614,  1142,   861,   717,   870,   759,  1146,   611,
     633,   879,   113,   630,   874,   320,   875,   320,   876,   573,
     926,   759,   981,   573,   927,   759,   955,   320,   623,  1011,
     956,   519,   957,   320,  1156,  1157,  1159,   959,   320,   909,
     404,   971,   980,   405,  1052,   320,  1108,   320,   624,   966,
    1113,   320,  1115,   573,  1124,   759,  1132,   759,   254,   972,
    1177,   759,   189,   191,   465,   466,   612,   613,  1185,  1186,
    1187,  1188,  1189,  1190,  1191,  1192,   467,   468,   629,   636,
     657,   260,   658,     4,     5,     6,     7,  1195,  1196,  1197,
     131,   132,   124,   125,   295,   659,    13,    14,    15,    16,
      17,    18,    19,   660,   661,   662,   663,   664,   665,   666,
     667,   668,   669,   127,    29,   670,   671,   672,   673,   674,
      35,   675,   676,   687,   700,  1041,    37,   705,   708,   706,
     709,   713,   718,   727,    45,    46,    47,    48,   320,   360,
      51,   729,   979,   730,   731,   732,   983,   984,   985,    55,
      56,   733,   775,   776,   734,    57,   735,   738,   739,   777,
     766,   767,   768,  1055,   769,   770,   771,  1057,   784,   782,
     786,   783,   787,   806,  1006,   778,   838,   840,  1008,   844,
     845,   854,   855,   880,   864,    60,    61,   868,   871,   872,
     881,   882,   883,   885,   886,   887,   910,   911,   912,   914,
     923,   928,   929,    62,   930,   931,   958,   935,   960,    63,
      64,   961,   962,   969,   968,   963,   965,   128,   696,   970,
      67,   976,    68,   977,   297,     4,     5,     6,     7,  1007,
    1009,  1031,   131,   132,   124,   125,   295,  1032,    13,    14,
      15,    16,    17,    18,    19,  1036,  1058,  1059,  1037,  1038,
    1042,  1050,  1062,  1060,  1063,   127,    29,  1064,  1077,  1084,
    1080,  1086,    35,  1085,  1088,  1089,  1107,  1090,    37,  1102,
    1103,  1139,  1140,  1112,  1111,  1121,    45,    46,    47,    48,
    1133,  1134,    51,  1013,  1135,  1125,  1136,  1137,  1138,  1141,
    1143,    55,    56,  1144,  1154,  1178,  1145,    57,  1149,  1152,
    1170,  1171,  1172,  1173,   160,   788,  1174,  1175,   715,   458,
     464,   440,     0,     0,   442,     0,     0,     0,     0,     0,
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
       2,     0,   239,   330,   273,   741,   572,   147,    14,   515,
      28,    27,   399,    14,    16,   521,    10,    11,    32,    15,
      14,    23,    24,    25,    26,    27,    14,    24,    30,    31,
      32,   917,    14,    35,    36,    10,    11,    39,    40,    14,
      10,    11,    24,    11,    14,    47,    48,    87,    87,    51,
       0,    14,    14,    24,    28,    10,    10,    11,    10,    55,
      14,    24,    14,    28,    66,    67,    62,    63,    64,    10,
      11,    33,   122,    14,    48,   122,    84,    71,    72,    73,
      68,   122,    14,    48,    84,    14,   137,   137,   128,   128,
      28,   142,    24,    42,    43,    24,    71,    72,    73,   140,
      28,    71,    72,    73,   106,   107,   108,   109,   110,   111,
      48,    74,   114,   115,     5,   117,   118,    71,    72,    73,
      48,    76,    77,    14,   142,   122,   128,   135,   142,   137,
      71,    72,    73,   135,   136,   135,   138,   137,   144,    14,
     124,   142,    74,   879,   138,    74,   148,  1033,   150,   122,
     152,   122,   154,    10,    11,    12,   137,    14,   142,   122,
     142,   142,   136,   138,   137,   139,   553,   135,   138,   137,
     676,   136,   174,   136,   176,    28,    33,   179,   143,   181,
     182,    38,   184,   185,   138,   187,   188,   189,   190,   191,
     122,    12,  1078,   122,   143,    48,  1082,   138,   136,    14,
      87,    10,    11,    18,   136,    14,   136,   136,   136,   105,
     106,   139,    12,   143,   354,   143,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   142,   228,   229,   230,   231,
     232,   233,   234,   235,   142,   237,   623,   624,  1124,   124,
      28,   137,   258,   259,   136,    28,  1132,   139,    14,    64,
      10,    11,   139,    10,    14,   142,    28,   142,    24,   586,
      48,  1147,    71,    72,    73,    48,    10,    28,    10,   265,
     266,   267,   268,   269,   270,   271,    48,    85,    86,    28,
     137,   122,   284,   136,    14,    15,   139,    48,    85,    86,
     143,  1177,    62,    63,   621,   136,    66,    87,   300,    48,
     302,   303,   304,   305,   306,   307,   308,   309,    74,   137,
     876,    71,    72,    73,   142,    78,   114,   115,   320,   321,
     128,   323,  1058,   325,   122,   124,    28,    28,    84,   138,
      28,   128,    76,    77,   336,   337,   338,   339,   340,   341,
     342,   357,   358,   142,   346,   347,    48,    48,   136,   139,
      48,   139,   142,   136,   122,   143,   122,    98,     2,   124,
     143,   360,   364,   365,   136,   367,   368,   112,   370,   371,
     136,   143,   140,   143,   376,   136,   378,   142,   138,   122,
     135,   383,   384,    31,    28,    29,   122,   136,    10,    11,
     392,   122,    14,   136,   143,   113,   723,    41,    42,   121,
     136,    49,   137,   121,   403,    49,    50,   142,    28,   140,
     135,    31,    32,   415,   416,   417,   418,    80,    81,    82,
     138,    41,    62,    63,    44,    45,    66,    14,    48,    49,
     124,   758,   136,   122,   136,   136,   673,    14,   136,   143,
      28,   143,    28,    31,    32,    31,    32,   136,   142,    71,
      72,    73,   122,    41,    14,    41,    44,    45,    44,    45,
      48,    28,    48,    49,    31,    32,   136,   794,   795,    28,
     122,   137,    31,    32,    41,    10,   142,    44,    45,    14,
     143,    48,    41,   127,   136,    44,    45,   114,   115,    48,
      49,    88,    89,    90,    91,   122,    14,    15,   500,   114,
     115,   136,   136,   143,   506,   507,    14,   122,   143,   153,
     136,   155,   156,   139,   516,   136,   136,   143,    10,   521,
     789,   113,   143,   143,   116,   117,   140,   124,   125,    10,
     136,   175,    13,   177,    75,    16,    17,   143,   540,   131,
     132,   133,   544,   870,   135,   547,   137,   549,   136,   551,
     136,   139,   122,   555,   556,   143,   139,   143,   141,    10,
      11,   136,    28,    14,   136,    31,    32,   139,   143,   136,
     122,   573,   139,   136,   218,    41,   143,   136,    44,    45,
     143,   139,    48,   136,   143,    28,   588,   136,    31,    32,
     143,   142,   310,    28,   143,    59,    31,    32,    41,   926,
     927,    44,    45,   137,   136,    48,    41,   609,   142,    44,
      45,   143,    10,    48,    49,    13,    14,   142,    16,    17,
      71,    72,    73,   136,   626,  1012,   344,   629,  1015,   136,
     143,   349,   634,   635,   636,   136,   143,   355,    99,   100,
     642,   643,   143,   645,   646,   363,   648,   649,   650,   651,
     652,   653,   654,   655,   923,   373,   136,   136,    28,   139,
     139,    31,    32,   105,   106,   107,   108,   109,   110,   111,
     136,    41,   137,   139,    44,    45,   136,   143,    48,    49,
     122,   136,   136,   143,   139,   687,   142,   405,   690,   143,
     408,   693,   136,   136,    14,   137,   139,   136,   700,   143,
     139,   136,   136,   136,    24,   136,    26,    27,   139,   143,
     143,   713,   714,   142,   136,    35,    36,  1104,   720,   139,
      40,   143,   142,   126,   127,   727,    46,    47,    92,    93,
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
      19,    20,    21,    22,   136,   136,   136,    10,    11,    12,
     558,   143,   143,   143,    33,    34,   136,   136,    53,   567,
     139,    40,   854,    58,    59,   136,   103,    46,   105,   106,
     107,   108,   109,   110,   111,    54,    55,    56,    57,   136,
     136,    60,   139,   139,   136,   122,   136,   139,   136,   139,
      69,    70,   139,   885,   141,   139,    75,   141,   135,   125,
     137,     7,     8,     9,   255,   256,   257,   899,   900,   901,
     902,   903,   904,   905,   906,   105,   106,   107,   108,   109,
     110,   111,   914,   139,   143,   141,   105,   106,   139,   139,
     141,   141,   122,   119,   120,   129,   130,   571,   572,   136,
     932,   575,   576,   577,   123,   138,   139,   137,   138,   139,
     129,   130,   136,   137,   588,   138,   139,   137,   137,   138,
      14,   140,   137,   142,    14,   144,   674,    14,   676,   603,
     604,   605,   136,   965,   682,   129,   130,   969,   970,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,    14,   626,   138,   139,   122,   988,   138,   139,    14,
     992,   993,   994,    18,   712,   138,   139,   138,   139,    24,
     136,    26,    27,   138,   139,   138,   139,  1009,   138,   139,
      35,    36,   138,   139,  1013,    40,    29,    30,  1020,   138,
     139,    46,    47,  1025,    14,   138,   139,   138,   139,    54,
      55,    56,    57,   136,    24,   136,    26,    27,   138,   139,
    1042,  1043,   138,   139,   122,    35,    36,   138,   139,    14,
      40,   138,   139,   138,   139,    29,    46,    47,   138,   139,
    1062,   138,   139,    10,    54,    55,    56,    57,   103,   136,
     105,   106,   107,   108,   109,   110,   111,   138,   139,  1081,
     138,   139,   138,   139,   114,   115,   136,   122,   114,   115,
     138,   139,   143,  1095,   138,   139,   138,   139,  1100,   136,
     135,   745,   137,    10,   138,   139,   138,   139,    29,    30,
     138,   139,    29,    30,   138,   139,   138,   139,   137,  1121,
     138,   139,   138,   139,  1126,  1127,  1128,   138,   139,   773,
     138,   139,   138,   139,   138,   139,   138,   139,   137,   857,
     138,   139,    29,    30,   138,   139,   138,   139,   118,   867,
     138,   139,    46,    47,   261,   262,   136,   136,  1160,  1161,
    1162,  1163,  1164,  1165,  1166,  1167,   263,   264,   122,   122,
     136,   128,   136,     3,     4,     5,     6,  1179,  1180,  1181,
      10,    11,    12,    13,    14,   136,    16,    17,    18,    19,
      20,    21,    22,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,    33,    34,   136,   136,   136,   101,   137,
      40,    14,   137,   122,   137,   933,    46,   125,   136,   143,
     136,    84,   136,   122,    54,    55,    56,    57,   139,   142,
      60,   136,   876,   136,   136,   136,   880,   881,   882,    69,
      70,   136,   125,   143,   136,    75,   136,   136,   136,   125,
     136,   136,   136,   971,   136,   136,   136,   975,   122,   136,
      14,   136,    14,    14,   908,   143,   136,   136,   912,   141,
     141,    84,    14,    10,   136,   105,   106,   136,   136,   136,
      10,    10,    14,   122,    10,    10,   136,   136,    29,   136,
      62,    14,    14,   123,    14,   136,   136,   143,   136,   129,
     130,   136,   136,    84,   136,   138,   137,   137,   138,    84,
     140,   136,   142,   136,   144,     3,     4,     5,     6,   143,
     136,   136,    10,    11,    12,    13,    14,   136,    16,    17,
      18,    19,    20,    21,    22,     7,   980,   981,     7,     7,
     122,   136,   122,    14,    10,    33,    34,    10,   138,    14,
      78,   136,    40,    14,    14,    14,   143,   136,    46,   136,
     136,    14,    14,   136,   138,   136,    54,    55,    56,    57,
     136,   136,    60,   142,   136,   143,   136,   136,   136,   136,
      14,    69,    70,    14,  1121,   138,   136,    75,   143,   136,
     136,   136,   136,   136,    29,   619,   136,   136,   544,   254,
     260,   238,    -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,
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
     196,   138,    29,    30,   150,   138,   192,   196,   186,   186,
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
#line 535 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3369 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 545 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3375 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 546 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3381 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 550 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3387 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 552 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3393 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 554 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3399 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 556 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3407 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 560 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3413 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 562 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3419 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 564 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3425 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 566 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3431 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 568 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3439 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 572 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3447 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 576 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3453 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 578 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3461 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 582 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3469 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 586 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3475 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 588 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3481 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 590 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3487 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 592 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3493 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 594 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3499 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 596 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3507 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 600 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3515 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3521 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3527 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 608 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3533 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 610 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3539 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 612 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3545 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 614 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3551 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 616 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3557 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 618 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3563 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 620 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3569 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 622 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3575 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 624 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3581 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 626 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3587 "raku.tab.c"
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
#line 3599 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 636 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3605 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 638 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); free((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3611 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 640 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3617 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 642 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); free((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 3623 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 644 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3629 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 646 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3635 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 648 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3642 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 651 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3649 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 654 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3655 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 656 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3662 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 659 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3669 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 662 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3675 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 664 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3683 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 668 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3689 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 670 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3695 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 672 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3702 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 675 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3709 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 678 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3715 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 680 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3722 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 683 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3729 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 686 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 689 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3742 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 691 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3751 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 696 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3759 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 700 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3767 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 704 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3776 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 709 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3784 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 712 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3790 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3796 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 716 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3802 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 718 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3808 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3814 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 722 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3821 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 725 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3827 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 727 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3833 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 729 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3839 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 731 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3846 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 734 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3853 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 737 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3860 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 740 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3867 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 743 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3874 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 746 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3880 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 748 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3886 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 750 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3892 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 752 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3898 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 754 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3905 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 757 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3911 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 759 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3917 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 761 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3923 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 763 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3930 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 766 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3937 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 769 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3945 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 773 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3952 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 776 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3958 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 778 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3964 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 780 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3970 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 782 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3977 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 785 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3984 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 788 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3991 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 791 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3998 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 794 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4005 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 797 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4012 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 800 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4019 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 802 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4025 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 803 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4031 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 804 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4037 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 805 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4043 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 806 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4049 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 807 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4055 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 809 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4061 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 811 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4067 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH block  */
#line 813 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4073 "raku.tab.c"
    break;

  case 110: /* stmt: block  */
#line 815 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4079 "raku.tab.c"
    break;

  case 111: /* stmt: unless_stmt  */
#line 816 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4085 "raku.tab.c"
    break;

  case 112: /* stmt: until_stmt  */
#line 817 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4091 "raku.tab.c"
    break;

  case 113: /* stmt: repeat_stmt  */
#line 818 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4097 "raku.tab.c"
    break;

  case 114: /* stmt: loop_stmt  */
#line 819 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4103 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST ';'  */
#line 820 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4109 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT ';'  */
#line 821 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4115 "raku.tab.c"
    break;

  case 117: /* stmt: KW_LAST KW_IF expr ';'  */
#line 823 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4121 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 825 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4127 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 827 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4133 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 829 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4139 "raku.tab.c"
    break;

  case 121: /* stmt: sub_decl  */
#line 830 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4145 "raku.tab.c"
    break;

  case 122: /* stmt: class_decl  */
#line 831 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4151 "raku.tab.c"
    break;

  case 123: /* stmt: role_decl  */
#line 832 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4157 "raku.tab.c"
    break;

  case 124: /* stmt: grammar_decl  */
#line 833 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4163 "raku.tab.c"
    break;

  case 125: /* stmt: module_decl  */
#line 834 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4169 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF '(' expr ')' block  */
#line 838 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4175 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4181 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4187 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block  */
#line 844 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4193 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 846 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4199 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 848 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4205 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF expr block elsif_tail  */
#line 850 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4211 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 852 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4217 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block  */
#line 856 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4223 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 858 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4229 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 860 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4235 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 862 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4241 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 864 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4247 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 866 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4253 "raku.tab.c"
    break;

  case 140: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 870 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4259 "raku.tab.c"
    break;

  case 141: /* while_stmt: KW_WHILE expr block  */
#line 872 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4265 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 876 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4271 "raku.tab.c"
    break;

  case 143: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 878 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4277 "raku.tab.c"
    break;

  case 144: /* unless_stmt: KW_UNLESS expr block  */
#line 880 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4283 "raku.tab.c"
    break;

  case 145: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 882 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4289 "raku.tab.c"
    break;

  case 146: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 886 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4295 "raku.tab.c"
    break;

  case 147: /* until_stmt: KW_UNTIL expr block  */
#line 888 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4301 "raku.tab.c"
    break;

  case 148: /* repeat_stmt: KW_REPEAT block  */
#line 892 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4307 "raku.tab.c"
    break;

  case 149: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 894 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4313 "raku.tab.c"
    break;

  case 150: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 896 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4319 "raku.tab.c"
    break;

  case 151: /* loop_stmt: KW_LOOP block  */
#line 900 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4325 "raku.tab.c"
    break;

  case 152: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 902 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4331 "raku.tab.c"
    break;

  case 153: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 904 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4337 "raku.tab.c"
    break;

  case 154: /* loop_incr: expr  */
#line 907 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4343 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 911 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4353 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 917 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4363 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 923 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node));
          } else (yyval.node) = rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)); }
#line 4375 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 931 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4385 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 937 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4394 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR expr block  */
#line 942 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4401 "raku.tab.c"
    break;

  case 161: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 947 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4413 "raku.tab.c"
    break;

  case 162: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 955 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4426 "raku.tab.c"
    break;

  case 163: /* when_list: %empty  */
#line 965 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4432 "raku.tab.c"
    break;

  case 164: /* when_list: when_list KW_WHEN expr block  */
#line 967 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4440 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 973 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4452 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 981 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4462 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_SUB IDENT sub_body  */
#line 987 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4472 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 993 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4484 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1001 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4494 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1007 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4504 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1013 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4517 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1022 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4528 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1029 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4541 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1038 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4554 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1047 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4565 "raku.tab.c"
    break;

  case 176: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1056 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4576 "raku.tab.c"
    break;

  case 177: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1063 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4585 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list '}'  */
#line 1069 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4591 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr '}'  */
#line 1071 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4598 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1074 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4605 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1077 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4611 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1079 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4617 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1081 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4623 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1083 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4629 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1085 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4635 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1087 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4641 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1089 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4647 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1091 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4654 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1094 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4660 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1096 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4666 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1098 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4672 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1100 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4679 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1103 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4686 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1106 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4694 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list '}'  */
#line 1111 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4700 "raku.tab.c"
    break;

  case 196: /* method_body: '{' YADA '}'  */
#line 1112 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4706 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr '}'  */
#line 1114 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4713 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1117 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4720 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1120 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4726 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1122 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4732 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1124 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4738 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1126 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4744 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1128 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4750 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1130 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4756 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1132 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4762 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1134 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4769 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1137 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4775 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1139 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4781 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1141 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4787 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1143 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4794 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1146 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4801 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1149 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4809 "raku.tab.c"
    break;

  case 213: /* pkg_name: IDENT  */
#line 1154 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4815 "raku.tab.c"
    break;

  case 214: /* pkg_name: QIDENT  */
#line 1155 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4821 "raku.tab.c"
    break;

  case 215: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1159 "raku.y"
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
#line 4839 "raku.tab.c"
    break;

  case 216: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1175 "raku.y"
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
#line 4856 "raku.tab.c"
    break;

  case 217: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1190 "raku.y"
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
#line 4873 "raku.tab.c"
    break;

  case 218: /* is_clauses: %empty  */
#line 1204 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4879 "raku.tab.c"
    break;

  case 219: /* is_clauses: is_clauses IDENT IDENT  */
#line 1206 "raku.y"
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
#line 4895 "raku.tab.c"
    break;

  case 220: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1218 "raku.y"
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
#line 4911 "raku.tab.c"
    break;

  case 221: /* class_body_list: %empty  */
#line 1231 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4917 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1233 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4924 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1236 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4931 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1239 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4938 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1242 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4945 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1245 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4952 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1248 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4959 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1251 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4966 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1254 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4973 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1257 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4980 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1260 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4987 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1263 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4994 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1266 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5001 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1269 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5012 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1276 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5023 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1283 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5034 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1290 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5045 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1297 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5053 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1301 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5061 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1305 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5069 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1309 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5077 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1313 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5088 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1320 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5099 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1327 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5110 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1334 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5121 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1341 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5135 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1351 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5147 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1359 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5159 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1367 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5172 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1376 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5183 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1383 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5194 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1390 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5208 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1400 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5220 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1408 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5232 "raku.tab.c"
    break;

  case 255: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1418 "raku.y"
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
#line 5249 "raku.tab.c"
    break;

  case 256: /* grammar_body_list: %empty  */
#line 1432 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5255 "raku.tab.c"
    break;

  case 257: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1434 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5264 "raku.tab.c"
    break;

  case 258: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1439 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5273 "raku.tab.c"
    break;

  case 259: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1444 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5282 "raku.tab.c"
    break;

  case 260: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1451 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5290 "raku.tab.c"
    break;

  case 261: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1455 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5298 "raku.tab.c"
    break;

  case 262: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1459 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5306 "raku.tab.c"
    break;

  case 263: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1463 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5314 "raku.tab.c"
    break;

  case 264: /* pair_list: IDENT OP_FATARROW expr  */
#line 1469 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5320 "raku.tab.c"
    break;

  case 265: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1471 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5326 "raku.tab.c"
    break;

  case 266: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1473 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5332 "raku.tab.c"
    break;

  case 267: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1475 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5338 "raku.tab.c"
    break;

  case 268: /* param_list: VAR_SCALAR  */
#line 1478 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5344 "raku.tab.c"
    break;

  case 269: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1479 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5350 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1480 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5356 "raku.tab.c"
    break;

  case 271: /* param_list: VAR_ARRAY  */
#line 1481 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5362 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' VAR_ARRAY  */
#line 1482 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5368 "raku.tab.c"
    break;

  case 273: /* param_list: IDENT VAR_SCALAR  */
#line 1483 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5374 "raku.tab.c"
    break;

  case 274: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1484 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5380 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1485 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5386 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' VAR_SCALAR  */
#line 1486 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5392 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1487 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5398 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1488 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5404 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1489 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5410 "raku.tab.c"
    break;

  case 280: /* param_list: VAR_SCALAR '=' expr  */
#line 1490 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5416 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1491 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5422 "raku.tab.c"
    break;

  case 282: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1492 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5428 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1493 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5434 "raku.tab.c"
    break;

  case 284: /* param_list: SLURPY_POS  */
#line 1494 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5440 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' SLURPY_POS  */
#line 1495 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5446 "raku.tab.c"
    break;

  case 286: /* param_list: SLURPY_LOL  */
#line 1496 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5452 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' SLURPY_LOL  */
#line 1497 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5458 "raku.tab.c"
    break;

  case 288: /* param_list: SLURPY_NAMED  */
#line 1498 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5464 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1499 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5470 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list '}'  */
#line 1502 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5476 "raku.tab.c"
    break;

  case 291: /* block: '{' YADA '}'  */
#line 1503 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5482 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr '}'  */
#line 1505 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5488 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1507 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5494 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1509 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5500 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1511 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5506 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1513 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5512 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1515 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5519 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1518 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5525 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1520 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5531 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1522 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5537 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1524 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5543 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1526 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5550 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1529 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5557 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1532 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5565 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1536 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5572 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1539 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5579 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1542 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5586 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1545 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5592 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1547 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5600 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1551 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5607 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1554 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5614 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1557 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5621 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1560 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5628 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1563 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5635 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1566 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5641 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1568 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5647 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1570 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5653 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_LAST '}'  */
#line 1572 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5659 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1574 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5665 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1576 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5671 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1578 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5677 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1580 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5683 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1582 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5689 "raku.tab.c"
    break;

  case 324: /* closure: '{' expr '}'  */
#line 1585 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5695 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR '=' expr  */
#line 1588 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5701 "raku.tab.c"
    break;

  case 326: /* expr: VAR_ARRAY '=' expr  */
#line 1589 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5707 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1590 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5713 "raku.tab.c"
    break;

  case 328: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1591 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5719 "raku.tab.c"
    break;

  case 329: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1592 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5725 "raku.tab.c"
    break;

  case 330: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1593 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5731 "raku.tab.c"
    break;

  case 331: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1594 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5737 "raku.tab.c"
    break;

  case 332: /* expr: KW_GATHER block  */
#line 1595 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5747 "raku.tab.c"
    break;

  case 333: /* expr: KW_GATHER for_stmt  */
#line 1600 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5757 "raku.tab.c"
    break;

  case 334: /* expr: tern_expr OP_FATARROW expr  */
#line 1606 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5763 "raku.tab.c"
    break;

  case 335: /* expr: tern_expr  */
#line 1607 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5769 "raku.tab.c"
    break;

  case 336: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1611 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5775 "raku.tab.c"
    break;

  case 337: /* tern_expr: or_expr  */
#line 1612 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5781 "raku.tab.c"
    break;

  case 338: /* or_expr: or_expr OP_OR and_expr  */
#line 1615 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5787 "raku.tab.c"
    break;

  case 339: /* or_expr: and_expr  */
#line 1616 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5793 "raku.tab.c"
    break;

  case 340: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1619 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5799 "raku.tab.c"
    break;

  case 341: /* and_expr: cmp_expr  */
#line 1620 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5805 "raku.tab.c"
    break;

  case 342: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1623 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5811 "raku.tab.c"
    break;

  case 343: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1624 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5817 "raku.tab.c"
    break;

  case 344: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1625 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5823 "raku.tab.c"
    break;

  case 345: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1626 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5829 "raku.tab.c"
    break;

  case 346: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1627 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5835 "raku.tab.c"
    break;

  case 347: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1628 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5841 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1629 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5847 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1630 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5853 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1631 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5859 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1632 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5865 "raku.tab.c"
    break;

  case 352: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1633 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5871 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1634 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5877 "raku.tab.c"
    break;

  case 354: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1636 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5887 "raku.tab.c"
    break;

  case 355: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1642 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5897 "raku.tab.c"
    break;

  case 356: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1648 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5907 "raku.tab.c"
    break;

  case 357: /* cmp_expr: divis_expr  */
#line 1653 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5913 "raku.tab.c"
    break;

  case 358: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1656 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5919 "raku.tab.c"
    break;

  case 359: /* divis_expr: jct_expr  */
#line 1657 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5925 "raku.tab.c"
    break;

  case 360: /* jct_expr: jct_expr '|' range_expr  */
#line 1660 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5931 "raku.tab.c"
    break;

  case 361: /* jct_expr: jct_expr '&' range_expr  */
#line 1661 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5937 "raku.tab.c"
    break;

  case 362: /* jct_expr: dor_expr  */
#line 1662 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5943 "raku.tab.c"
    break;

  case 363: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1666 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5949 "raku.tab.c"
    break;

  case 364: /* dor_expr: range_expr  */
#line 1667 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5955 "raku.tab.c"
    break;

  case 365: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1670 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5961 "raku.tab.c"
    break;

  case 366: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1671 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5967 "raku.tab.c"
    break;

  case 367: /* range_expr: add_expr  */
#line 1672 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5973 "raku.tab.c"
    break;

  case 368: /* add_expr: add_expr '~' repl_expr  */
#line 1675 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5979 "raku.tab.c"
    break;

  case 369: /* add_expr: repl_expr  */
#line 1676 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5985 "raku.tab.c"
    break;

  case 370: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1679 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5991 "raku.tab.c"
    break;

  case 371: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1680 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5997 "raku.tab.c"
    break;

  case 372: /* repl_expr: addsub_expr  */
#line 1681 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6003 "raku.tab.c"
    break;

  case 373: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1684 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6009 "raku.tab.c"
    break;

  case 374: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1685 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6015 "raku.tab.c"
    break;

  case 375: /* addsub_expr: mul_expr  */
#line 1686 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6021 "raku.tab.c"
    break;

  case 376: /* mul_expr: mul_expr '*' unary_expr  */
#line 1689 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6027 "raku.tab.c"
    break;

  case 377: /* mul_expr: mul_expr '/' unary_expr  */
#line 1690 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6033 "raku.tab.c"
    break;

  case 378: /* mul_expr: mul_expr '%' unary_expr  */
#line 1691 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6039 "raku.tab.c"
    break;

  case 379: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1693 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6045 "raku.tab.c"
    break;

  case 380: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1695 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6051 "raku.tab.c"
    break;

  case 381: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1697 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6057 "raku.tab.c"
    break;

  case 382: /* mul_expr: unary_expr  */
#line 1698 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6063 "raku.tab.c"
    break;

  case 383: /* unary_expr: '-' unary_expr  */
#line 1701 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6069 "raku.tab.c"
    break;

  case 384: /* unary_expr: '+' unary_expr  */
#line 1702 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6075 "raku.tab.c"
    break;

  case 385: /* unary_expr: '!' unary_expr  */
#line 1703 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6081 "raku.tab.c"
    break;

  case 386: /* unary_expr: CARET unary_expr  */
#line 1704 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6087 "raku.tab.c"
    break;

  case 387: /* unary_expr: OP_REDUCE unary_expr  */
#line 1706 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6096 "raku.tab.c"
    break;

  case 388: /* unary_expr: pow_expr  */
#line 1710 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6102 "raku.tab.c"
    break;

  case 389: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1713 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6108 "raku.tab.c"
    break;

  case 390: /* pow_expr: postfix_expr  */
#line 1714 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6114 "raku.tab.c"
    break;

  case 391: /* scalar_list: VAR_SCALAR  */
#line 1717 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6120 "raku.tab.c"
    break;

  case 392: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1718 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6126 "raku.tab.c"
    break;

  case 393: /* meth_name: IDENT  */
#line 1721 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6132 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_SORT  */
#line 1722 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6138 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_REVERSE  */
#line 1723 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6144 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_MAP  */
#line 1724 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6150 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_GREP  */
#line 1725 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6156 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_SAY  */
#line 1726 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6162 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_PRINT  */
#line 1727 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6168 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_TAKE  */
#line 1728 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6174 "raku.tab.c"
    break;

  case 401: /* meth_name: KW_RETURN  */
#line 1729 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6180 "raku.tab.c"
    break;

  case 402: /* meth_name: KW_EXISTS  */
#line 1730 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6186 "raku.tab.c"
    break;

  case 403: /* meth_name: KW_DELETE  */
#line 1731 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6192 "raku.tab.c"
    break;

  case 404: /* meth_name: KW_JOIN  */
#line 1732 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6198 "raku.tab.c"
    break;

  case 405: /* meth_name: TESTOP  */
#line 1733 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6204 "raku.tab.c"
    break;

  case 406: /* postfix_expr: call_expr  */
#line 1735 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6210 "raku.tab.c"
    break;

  case 407: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1738 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6220 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '(' arg_list ')'  */
#line 1744 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6229 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '(' ')'  */
#line 1748 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6235 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1750 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6241 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1752 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6247 "raku.tab.c"
    break;

  case 412: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1754 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6256 "raku.tab.c"
    break;

  case 413: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1759 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6262 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1761 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6272 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1767 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6282 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1773 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6290 "raku.tab.c"
    break;

  case 417: /* call_expr: IDENT '.' KW_NEW  */
#line 1777 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6298 "raku.tab.c"
    break;

  case 418: /* call_expr: IDENT '.' IDENT  */
#line 1781 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6307 "raku.tab.c"
    break;

  case 419: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1786 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6318 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1793 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6327 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '.' CARET IDENT  */
#line 1798 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6337 "raku.tab.c"
    break;

  case 422: /* call_expr: atom '.' CARET IDENT  */
#line 1804 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6347 "raku.tab.c"
    break;

  case 423: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1810 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6358 "raku.tab.c"
    break;

  case 424: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1817 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6369 "raku.tab.c"
    break;

  case 425: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1824 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6378 "raku.tab.c"
    break;

  case 426: /* call_expr: atom '.' meth_name  */
#line 1829 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6387 "raku.tab.c"
    break;

  case 427: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1834 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6398 "raku.tab.c"
    break;

  case 428: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1841 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6407 "raku.tab.c"
    break;

  case 429: /* call_expr: call_expr '.' meth_name  */
#line 1846 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6416 "raku.tab.c"
    break;

  case 430: /* call_expr: KW_DIE expr  */
#line 1851 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6422 "raku.tab.c"
    break;

  case 431: /* call_expr: KW_MAP closure expr  */
#line 1853 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6428 "raku.tab.c"
    break;

  case 432: /* call_expr: KW_MAP closure ',' expr  */
#line 1855 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6434 "raku.tab.c"
    break;

  case 433: /* call_expr: KW_GREP closure expr  */
#line 1857 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6440 "raku.tab.c"
    break;

  case 434: /* call_expr: KW_SORT expr  */
#line 1859 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6446 "raku.tab.c"
    break;

  case 435: /* call_expr: KW_SORT closure expr  */
#line 1861 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6452 "raku.tab.c"
    break;

  case 436: /* call_expr: KW_REVERSE expr  */
#line 1863 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6458 "raku.tab.c"
    break;

  case 437: /* call_expr: atom  */
#line 1864 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6464 "raku.tab.c"
    break;

  case 438: /* arg_list: expr  */
#line 1867 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6470 "raku.tab.c"
    break;

  case 439: /* arg_list: arg_list ',' expr  */
#line 1868 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6476 "raku.tab.c"
    break;

  case 440: /* arg_list: arg_list ','  */
#line 1869 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6482 "raku.tab.c"
    break;

  case 441: /* atom: LIT_INT  */
#line 1872 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6488 "raku.tab.c"
    break;

  case 442: /* atom: LIT_FLOAT  */
#line 1873 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6494 "raku.tab.c"
    break;

  case 443: /* atom: LIT_STR  */
#line 1874 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6500 "raku.tab.c"
    break;

  case 444: /* atom: WORDLIST  */
#line 1876 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6513 "raku.tab.c"
    break;

  case 445: /* atom: LIT_INTERP_STR  */
#line 1884 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6519 "raku.tab.c"
    break;

  case 446: /* atom: VAR_SCALAR  */
#line 1885 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6525 "raku.tab.c"
    break;

  case 447: /* atom: OP_INC VAR_SCALAR  */
#line 1886 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6531 "raku.tab.c"
    break;

  case 448: /* atom: OP_DEC VAR_SCALAR  */
#line 1887 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6537 "raku.tab.c"
    break;

  case 449: /* atom: VAR_SCALAR OP_INC  */
#line 1888 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6543 "raku.tab.c"
    break;

  case 450: /* atom: VAR_SCALAR OP_DEC  */
#line 1889 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6549 "raku.tab.c"
    break;

  case 451: /* atom: VAR_ARRAY  */
#line 1890 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6555 "raku.tab.c"
    break;

  case 452: /* atom: VAR_HASH  */
#line 1891 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6561 "raku.tab.c"
    break;

  case 453: /* atom: VAR_CAPTURE  */
#line 1893 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6569 "raku.tab.c"
    break;

  case 454: /* atom: VAR_FH  */
#line 1897 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6577 "raku.tab.c"
    break;

  case 455: /* atom: VAR_NAMED_CAPTURE  */
#line 1901 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6584 "raku.tab.c"
    break;

  case 456: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1904 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6590 "raku.tab.c"
    break;

  case 457: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1906 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6596 "raku.tab.c"
    break;

  case 458: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1908 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6602 "raku.tab.c"
    break;

  case 459: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1910 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6608 "raku.tab.c"
    break;

  case 460: /* atom: ARR_ALL_SLICE  */
#line 1912 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6614 "raku.tab.c"
    break;

  case 461: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1914 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6620 "raku.tab.c"
    break;

  case 462: /* atom: VAR_HASH '{' expr '}'  */
#line 1916 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6626 "raku.tab.c"
    break;

  case 463: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1918 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6632 "raku.tab.c"
    break;

  case 464: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1920 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6638 "raku.tab.c"
    break;

  case 465: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 1922 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 6644 "raku.tab.c"
    break;

  case 466: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 1924 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 6650 "raku.tab.c"
    break;

  case 467: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 1926 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 6656 "raku.tab.c"
    break;

  case 468: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 1928 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 6662 "raku.tab.c"
    break;

  case 469: /* atom: IDENT  */
#line 1929 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6668 "raku.tab.c"
    break;

  case 470: /* atom: VAR_TWIGIL  */
#line 1931 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6676 "raku.tab.c"
    break;

  case 471: /* atom: VAR_ARRAY_TWIGIL  */
#line 1935 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6684 "raku.tab.c"
    break;

  case 472: /* atom: VAR_HASH_TWIGIL  */
#line 1939 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6692 "raku.tab.c"
    break;

  case 473: /* atom: '[' ']'  */
#line 1942 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6698 "raku.tab.c"
    break;

  case 474: /* atom: '[' expr ']'  */
#line 1944 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6704 "raku.tab.c"
    break;

  case 475: /* atom: '[' expr ',' ']'  */
#line 1946 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6710 "raku.tab.c"
    break;

  case 476: /* atom: '[' expr ',' arg_list ']'  */
#line 1948 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6717 "raku.tab.c"
    break;

  case 477: /* atom: DOLLAR_LBRACKET ']'  */
#line 1950 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 6723 "raku.tab.c"
    break;

  case 478: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1952 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6729 "raku.tab.c"
    break;

  case 479: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1954 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6735 "raku.tab.c"
    break;

  case 480: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1956 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6742 "raku.tab.c"
    break;

  case 481: /* atom: '(' ')'  */
#line 1958 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6748 "raku.tab.c"
    break;

  case 482: /* atom: '(' expr ')'  */
#line 1959 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6754 "raku.tab.c"
    break;

  case 483: /* atom: '(' expr ',' ')'  */
#line 1961 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6760 "raku.tab.c"
    break;

  case 484: /* atom: '(' expr ',' arg_list ')'  */
#line 1963 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6767 "raku.tab.c"
    break;

  case 485: /* atom: block  */
#line 1965 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6773 "raku.tab.c"
    break;

  case 486: /* atom: KW_SUB block  */
#line 1966 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6779 "raku.tab.c"
    break;


#line 6783 "raku.tab.c"

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

#line 1968 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
