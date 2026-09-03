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
static int rk_is_array_name(const char *bare) {
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
static tree_t *var_node(const char *name) {
    const char *bare = strip_sigil(name);
    if (name && name[0] == '@') rk_mark_array_name(bare);
    return leaf_sval(TT_VAR, bare);
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

#line 473 "raku.tab.c"

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
  YYSYMBOL_OP_FATARROW = 83,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 84,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 85,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 86,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 87,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 88,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 89,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 90,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 91,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 92,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 93,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 94,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 95,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 96,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 97,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 98,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 99,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 100,              /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 101,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 102,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 103,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 104,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 105,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 106,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 107,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 108,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 109,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 110,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 111,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 112,                   /* OP_DIV  */
  YYSYMBOL_OP_BAND = 113,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 114,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 115,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 116,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 117,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 118,                   /* OP_POW  */
  YYSYMBOL_119_ = 119,                     /* '='  */
  YYSYMBOL_120_ = 120,                     /* '!'  */
  YYSYMBOL_121_ = 121,                     /* '<'  */
  YYSYMBOL_122_ = 122,                     /* '>'  */
  YYSYMBOL_123_ = 123,                     /* '|'  */
  YYSYMBOL_124_ = 124,                     /* '&'  */
  YYSYMBOL_125_ = 125,                     /* '~'  */
  YYSYMBOL_126_ = 126,                     /* '+'  */
  YYSYMBOL_127_ = 127,                     /* '-'  */
  YYSYMBOL_128_ = 128,                     /* '*'  */
  YYSYMBOL_129_ = 129,                     /* '/'  */
  YYSYMBOL_130_ = 130,                     /* '%'  */
  YYSYMBOL_UMINUS = 131,                   /* UMINUS  */
  YYSYMBOL_132_ = 132,                     /* '.'  */
  YYSYMBOL_133_ = 133,                     /* ';'  */
  YYSYMBOL_134_ = 134,                     /* '('  */
  YYSYMBOL_135_ = 135,                     /* ')'  */
  YYSYMBOL_136_ = 136,                     /* ','  */
  YYSYMBOL_137_ = 137,                     /* '['  */
  YYSYMBOL_138_ = 138,                     /* ']'  */
  YYSYMBOL_139_ = 139,                     /* '{'  */
  YYSYMBOL_140_ = 140,                     /* '}'  */
  YYSYMBOL_141_ = 141,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 142,                 /* $accept  */
  YYSYMBOL_program = 143,                  /* program  */
  YYSYMBOL_stmt_list = 144,                /* stmt_list  */
  YYSYMBOL_stmt = 145,                     /* stmt  */
  YYSYMBOL_if_stmt = 146,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 147,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 148,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 149,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 150,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 151,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 152,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 153,                /* loop_incr  */
  YYSYMBOL_for_stmt = 154,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 155,               /* given_stmt  */
  YYSYMBOL_when_list = 156,                /* when_list  */
  YYSYMBOL_sub_decl = 157,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 158,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 159,                 /* sub_body  */
  YYSYMBOL_method_body = 160,              /* method_body  */
  YYSYMBOL_pkg_name = 161,                 /* pkg_name  */
  YYSYMBOL_class_decl = 162,               /* class_decl  */
  YYSYMBOL_role_decl = 163,                /* role_decl  */
  YYSYMBOL_is_clauses = 164,               /* is_clauses  */
  YYSYMBOL_class_body_list = 165,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 166,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 167,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 168,           /* named_arg_list  */
  YYSYMBOL_pair_list = 169,                /* pair_list  */
  YYSYMBOL_param_list = 170,               /* param_list  */
  YYSYMBOL_block = 171,                    /* block  */
  YYSYMBOL_closure = 172,                  /* closure  */
  YYSYMBOL_expr = 173,                     /* expr  */
  YYSYMBOL_tern_expr = 174,                /* tern_expr  */
  YYSYMBOL_or_expr = 175,                  /* or_expr  */
  YYSYMBOL_and_expr = 176,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 177,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 178,               /* divis_expr  */
  YYSYMBOL_jct_expr = 179,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 180,                 /* dor_expr  */
  YYSYMBOL_range_expr = 181,               /* range_expr  */
  YYSYMBOL_add_expr = 182,                 /* add_expr  */
  YYSYMBOL_repl_expr = 183,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 184,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 185,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 186,               /* unary_expr  */
  YYSYMBOL_pow_expr = 187,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 188,              /* scalar_list  */
  YYSYMBOL_meth_name = 189,                /* meth_name  */
  YYSYMBOL_postfix_expr = 190,             /* postfix_expr  */
  YYSYMBOL_call_expr = 191,                /* call_expr  */
  YYSYMBOL_arg_list = 192,                 /* arg_list  */
  YYSYMBOL_atom = 193                      /* atom  */
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
#define YYLAST   5231

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  142
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  480
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1199

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   374


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
       2,     2,     2,   120,     2,     2,     2,   130,   124,     2,
     134,   135,   128,   126,   136,   127,   132,   129,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   141,   133,
     121,   119,   122,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   137,     2,   138,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   139,   123,   140,   125,     2,     2,     2,
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
     115,   116,   117,   118,   131
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   492,   492,   503,   504,   507,   509,   511,   513,   517,
     519,   521,   523,   525,   529,   533,   535,   539,   543,   545,
     547,   549,   551,   553,   557,   561,   563,   565,   567,   569,
     571,   573,   575,   577,   579,   581,   583,   585,   593,   595,
     597,   599,   601,   603,   605,   608,   611,   613,   616,   619,
     621,   625,   627,   629,   632,   635,   637,   640,   643,   646,
     648,   653,   657,   661,   666,   670,   671,   673,   675,   677,
     679,   682,   684,   686,   688,   691,   694,   697,   700,   703,
     705,   707,   709,   711,   714,   716,   718,   720,   723,   726,
     730,   733,   735,   737,   739,   742,   745,   748,   751,   754,
     757,   760,   761,   762,   763,   764,   765,   766,   768,   770,
     772,   774,   775,   776,   777,   778,   779,   780,   782,   784,
     786,   788,   789,   790,   791,   794,   796,   798,   800,   802,
     804,   806,   808,   812,   814,   816,   818,   820,   822,   826,
     828,   832,   834,   836,   838,   842,   844,   848,   850,   852,
     856,   858,   860,   864,   867,   873,   879,   884,   890,   895,
     900,   908,   919,   920,   926,   934,   940,   946,   954,   960,
     966,   975,   982,   991,  1000,  1009,  1016,  1023,  1024,  1027,
    1030,  1032,  1034,  1036,  1038,  1040,  1042,  1044,  1047,  1049,
    1051,  1053,  1056,  1059,  1065,  1066,  1067,  1070,  1073,  1075,
    1077,  1079,  1081,  1083,  1085,  1087,  1090,  1092,  1094,  1096,
    1099,  1102,  1108,  1109,  1112,  1128,  1143,  1144,  1156,  1170,
    1171,  1174,  1177,  1180,  1183,  1186,  1189,  1192,  1195,  1198,
    1201,  1204,  1207,  1214,  1221,  1228,  1235,  1239,  1243,  1247,
    1251,  1258,  1265,  1272,  1279,  1289,  1297,  1305,  1314,  1321,
    1328,  1338,  1346,  1356,  1371,  1372,  1377,  1382,  1389,  1393,
    1397,  1401,  1407,  1409,  1411,  1413,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1437,  1438,  1441,  1442,
    1443,  1445,  1447,  1449,  1451,  1453,  1456,  1458,  1460,  1462,
    1464,  1467,  1470,  1474,  1477,  1480,  1483,  1485,  1489,  1492,
    1495,  1498,  1501,  1504,  1506,  1508,  1510,  1512,  1514,  1516,
    1518,  1520,  1524,  1527,  1528,  1529,  1530,  1531,  1532,  1533,
    1534,  1539,  1544,  1546,  1549,  1551,  1554,  1555,  1558,  1559,
    1562,  1563,  1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,
    1572,  1573,  1574,  1580,  1586,  1592,  1595,  1596,  1599,  1600,
    1601,  1604,  1606,  1609,  1610,  1611,  1614,  1615,  1618,  1619,
    1620,  1623,  1624,  1625,  1628,  1629,  1630,  1631,  1632,  1634,
    1636,  1639,  1640,  1641,  1642,  1643,  1648,  1651,  1652,  1655,
    1656,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1673,  1675,  1681,  1686,  1687,  1689,
    1691,  1696,  1698,  1704,  1710,  1714,  1718,  1723,  1730,  1735,
    1741,  1747,  1754,  1761,  1766,  1771,  1778,  1783,  1788,  1790,
    1792,  1794,  1796,  1798,  1800,  1802,  1805,  1806,  1807,  1810,
    1811,  1812,  1813,  1822,  1823,  1824,  1825,  1826,  1827,  1828,
    1829,  1830,  1834,  1838,  1841,  1843,  1845,  1847,  1849,  1851,
    1853,  1855,  1857,  1859,  1860,  1864,  1868,  1872,  1873,  1875,
    1877,  1880,  1881,  1883,  1885,  1888,  1889,  1890,  1892,  1895,
    1896
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
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW",
  "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE",
  "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT", "OP_SLE", "OP_SGT", "OP_SGE",
  "OP_AND", "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ",
  "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ",
  "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_BAND", "OP_SHL",
  "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'", "'<'",
  "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS",
  "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'", "':'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "elsif_tail",
  "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt", "loop_stmt",
  "loop_incr", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "scalar_methcall", "sub_body", "method_body", "pkg_name", "class_decl",
  "role_decl", "is_clauses", "class_body_list", "grammar_decl",
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

#define YYPACT_NINF (-877)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-480)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -877,    53,  2234,  -877,  -877,  -877,  -877,  -877,   882,   -72,
      43,   -45,    21,  -877,  -877,   801,  2330,  -877,  -877,  -877,
      94,  2405,   242,  4142,  4217,  4292,  4367,  4442,     2,   -19,
    4442,  2480,  2555,   311,   105,  4442,  4442,   130,   149,  4517,
    4592,    49,   150,    23,   349,    68,    68,  4667,  4442,    49,
      49,  4442,   482,   482,    55,   801,  -877,  -877,   482,   191,
     225,   801,   801,   801,  -877,  2630,  2706,   166,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,   623,  -877,
    -877,  -877,  5099,   647,   205,   608,   215,   280,   619,   669,
     238,  -877,   125,   680,   741,   571,  -877,  -877,   243,   221,
     260,   382,  -877,  -877,  4442,  4442,  4442,  4442,  4442,  4442,
     391,  2781,  4742,  2857,   399,  4442,  4442,   296,    69,  1167,
     107,   307,   190,  -877,   410,    49,  2932,  -877,  -877,   720,
     227,  -877,   376,  3008,  3083,  -877,  3158,  -877,   -15,   112,
     124,   141,   867,   472,   341,   471,  2932,   557,  2932,   347,
    2932,    49,  2932,    49,   -13,   152,   218,  -877,  -877,  -877,
      62,  -877,   366,  -877,   380,   457,   461,   521,   421,   513,
     235,   259,  2932,    49,  2932,    49,   351,  3234,  -877,  4442,
    4442,  -877,  4442,  4442,  -877,  4442,  3309,  4442,  3384,  4442,
    -877,  -877,   553,  -877,  -877,  -877,  -877,  -877,   522,   490,
       4,  -877,   526,  -877,  -877,  -877,  -877,  -877,   776,  -877,
     717,   783,  -877,   624,   529,  1574,  4442,  4442,  4442,  4442,
    4442,  4442,  4442,  4442,  -877,  4442,  4442,  4442,  4442,  4442,
    4442,  4442,  4442,  -877,  4442,   801,   801,   801,   801,   801,
     801,   801,   801,   801,   801,   801,   801,   801,   801,   801,
     889,   801,   801,   801,   801,   801,   801,   801,   801,   801,
     801,   801,   801,   801,   801,   801,   801,   801,   801,   915,
     890,   815,   594,   598,   612,   621,   727,   742,   -75,  -877,
     824,  2932,   122,   837,   672,   778,   546,   770,  -877,   797,
     940,   818,    -5,  -877,   942,   830,   838,  2857,   961,  4442,
    4442,  4442,  4442,  4442,  4442,  4442,  4442,  3460,  -877,  -877,
     846,  -877,   850,   852,   717,   842,  -877,  4442,  4442,  -877,
    4817,  -877,  4892,  -877,   297,   317,   322,   286,   622,   888,
    -877,   858,   860,  4442,  4442,  4442,  4442,  4442,  4442,  4442,
    -877,  4442,   863,  4442,  4442,  -877,  4442,   870,   938,   875,
    -877,  1002,  4442,  -877,   801,   801,    25,  -877,  -877,  -877,
    4442,  4442,  4442,  -877,  4442,  4442,  -877,  4442,  4442,   884,
    4442,  -877,  1001,  4442,  1015,  4442,   883,  1004,   885,  -877,
    4442,  4442,  1038,   932,   935,   996,  1000,  1006,  1003,  4442,
    -877,  -877,  -877,    49,    -9,  -877,    44,   929,   948,  -877,
    -877,  3535,  1021,  1131,  3611,  -877,  -877,   930,   284,   283,
    1023,  4142,  4217,  4442,  1946,   248,   300,  -877,   420,  1011,
    1012,  1013,  1014,  1024,  1025,  1026,  1027,  1028,  1030,  1031,
    1032,  1041,  1042,  1043,  1049,  -877,   215,  1056,   280,  1075,
    1075,  1075,  1075,  1075,  1075,  1075,  1075,  1075,  1075,  1075,
    1075,  -877,  -877,  -877,   669,  -877,  -877,  -877,  1066,  1066,
     680,   741,   741,   571,   571,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  1058,  1179,  1060,  -877,  3686,
    -877,  -877,  -877,  -877,  -877,  -877,  4442,  3762,  -877,   887,
    -877,  4442,  4442,  4442,  4442,  1076,  1077,  1078,  -877,  3838,
    -877,  1263,  4442,  1064,  -877,    -6,  -877,  1435,   736,  1080,
    1059,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,   763,
    -877,  -877,  -877,  1070,  -877,  1071,  2932,   154,  1122,    42,
    3914,   309,  1073,  4442,  -877,  4967,  -877,  4442,  -877,    65,
    -877,  4442,  4442,  1089,  3535,  1079,  1082,  1083,  1084,  1085,
    1086,  1087,   422,  3535,  1092,  1093,   487,    49,   -11,  5042,
    -877,    49,    49,    15,   -43,   -30,     3,  -877,     9,  -877,
    -877,  -877,  1072,   891,  1712,   501,  1095,  1096,  1097,  1098,
    1099,  1100,  -877,  1074,    -1,  1116,  1069,  1117,  1101,    49,
      49,    49,  1107,  1110,  1125,  4442,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  1200,  1231,  -877,    18,  1072,   906,   192,
     559,   109,  -877,   908,  4442,  -877,  -877,   771,  1232,  2857,
    4442,  4442,   426,    29,   396,  -877,   358,  4442,  4442,  -877,
    4442,  4442,  -877,  4442,  4442,  4442,  4442,  4442,  4442,  4442,
    4442,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,   801,  3990,
    -877,  1339,  1114,   910,  1115,  -877,   917,  3535,   530,  1111,
    1112,   777,  4442,  4442,  4442,  -877,   919,  -877,   921,   923,
    -877,  4442,  1168,  1238,   925,  -877,  -877,  -877,  -877,  -877,
    -877,   931,  -877,  4442,  4442,  4442,   949,  -877,    56,  -877,
    1120,  2932,   596,  1121,  1072,   951,  1123,  1124,  4442,   954,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,   956,  -877,
    -877,  -877,   973,  -877,  -877,  2932,    49,  -877,  -877,  1245,
    -877,  1248,  1249,  1246,  4442,  1142,  1252,  1253,  -877,  1072,
     648,  4142,  4217,  2021,  -877,   444,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  4442,    49,  -877,  -877,  -877,  1132,  1137,
    1235,  -877,  -877,  -877,  -877,  4442,  1145,  -877,  -877,    28,
     712,   660,  1224,  -877,  -877,  1072,  1072,   958,   960,  1274,
    1275,  1276,  -877,  -877,  1158,  -877,    33,   796,  1152,   184,
    4442,  4442,  4442,  4442,  4442,  4442,  -877,  -877,  -877,  -877,
     291,   320,   326,   344,   362,   400,   406,   414,   423,   438,
     439,   441,  -877,  -877,   962,  -877,   964,   966,  -877,  1162,
    -877,  -877,   968,  -877,  -877,  -877,  -877,  1165,  1166,  1172,
    -877,  -877,  -877,  1175,  4442,  1173,  -877,  3535,   607,  -877,
    -877,  1178,  1217,  1229,  -877,   970,  -877,  4442,  -877,  -877,
    1072,  -877,  -877,   713,  1180,  1181,   -11,  -877,   972,   995,
      49,    49,    49,  -877,  -877,  4442,  -877,  -877,  -877,    24,
    -877,    16,  -877,  -877,  -877,   475,   145,  -877,   371,  4442,
    4442,  4442,  4442,  4442,  4442,  4442,  4442,  -877,    49,  1176,
    -877,  -877,    49,  1182,  4442,  -877,   339,   452,    26,    38,
     514,  1188,  1189,   915,  -877,  -877,  1072,  1072,  1317,  1318,
    1319,  -877,  4442,  4066,  1208,  1209,  -877,   459,   467,   473,
     481,   485,   493,  -877,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  1196,
    -877,  -877,  -877,  -877,  -877,  4442,   975,  -877,  -877,  4442,
    4442,  3535,   726,  -877,  -877,  4442,  -877,  -877,  -877,  -877,
      49,    49,  -877,  -877,  -877,  -877,  -877,  1316,  4442,  1215,
    1326,  1327,  4442,  4442,  4442,  -877,  -877,  -877,   554,   564,
     565,   584,   585,   590,   595,   604,  -877,  -877,  -877,  4442,
    1204,  -877,   237,  1262,  -877,   272,  -877,  1332,  1333,  1221,
    4442,  -877,  1334,  1348,  1230,  4442,  -877,    75,   316,  1233,
    1237,  -877,  -877,   470,  -877,  -877,  -877,  -877,  -877,   613,
    1201,   977,  4442,  4442,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  1236,  1241,  -877,  -877,   982,  -877,   733,  1090,  -877,
    -877,  -877,  4442,  -877,  -877,   631,   632,   637,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  1242,    49,  1225,   986,
    1240,  1850,  1225,   988,  1243,  1244,  -877,  1251,  1254,  1255,
    -877,  1258,  1351,  1364,  1259,  4442,  -877,  1367,  1368,  1268,
    4442,  -877,  -877,  -877,   312,  -877,  -877,  -877,  1265,   641,
     642,  -877,  -877,  1270,  -877,    49,  -877,  -877,  -877,  -877,
    -877,  4442,  -877,  -877,  1225,  -877,  4142,  4217,  2096,  -877,
     477,  -877,  1225,  -877,  -877,  -877,  -877,  -877,  -877,  1273,
    1277,  -877,  1278,  1279,  1280,  -877,  1282,  1225,   990,  -877,
    -877,  -877,  -877,  -877,  1272,  -877,   520,   214,  -877,   395,
    4442,  4442,  4442,  4442,  4442,  4442,  4442,  4442,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  1225,    49,  4442,
    4442,  4442,  -877,  -877,  -877,   645,   646,   650,   651,   654,
     655,   676,   700,  -877,  -877,   705,   710,   711,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   439,   440,   441,   443,   444,   449,
     450,   464,   463,   465,   466,     0,     0,   451,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   442,     0,     0,
       0,     0,     0,     0,   102,     0,     0,     3,     4,   103,
     104,   111,   112,   113,   114,   105,   106,   121,     0,   122,
     123,   124,   110,     0,   333,   335,   337,   339,   355,   357,
     360,   362,   365,   367,   370,   373,   380,   386,   388,   404,
     435,     0,   447,   448,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,   449,   450,   464,   463,     0,     0,   479,   384,   444,
     449,   471,     0,     0,     0,    38,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,   480,   331,   330,
       0,    52,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,     0,   150,     0,
       0,   115,     0,     0,   116,     0,     0,     0,     3,     0,
     432,   434,   107,   109,   428,   212,   213,   216,     0,     0,
       0,   385,     0,   445,   446,   383,   382,   381,   444,   475,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   326,   327,   328,   329,   323,     0,   411,
       0,     0,   324,     0,     0,     0,     0,     0,    42,   416,
       0,   415,   463,   407,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   472,    29,
       0,    31,     0,   475,     0,     0,    41,   438,     0,     6,
       0,    10,     0,    11,     0,     0,     0,     0,     0,     0,
     389,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,    46,     0,     0,   128,     0,
     140,     0,     0,   159,     0,     0,     0,     3,   166,    49,
       0,     0,     0,    51,     0,     0,    55,     0,     0,     0,
       0,   162,     0,     0,     0,     0,     0,   143,     0,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,   431,   433,     0,     0,   219,     0,     0,     0,   254,
     476,     0,     0,     0,     0,   468,   289,   444,   449,   450,
     464,     0,     0,     0,     0,     0,     0,   288,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   332,   336,     0,   338,   340,
     341,   344,   345,   342,   343,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   356,   358,   359,   361,   363,   364,
     366,   368,   369,   371,   372,   377,   378,   379,   374,   375,
     376,   387,   391,   403,   396,   397,   398,   399,   402,   400,
     401,   394,   395,   392,   393,   427,     0,   424,    62,     0,
      96,    97,    98,    99,   100,    59,     0,     0,   410,     0,
      15,     0,     0,     0,     0,   454,   459,   460,    64,     0,
     419,     0,     0,     0,   408,     0,   406,   438,     0,     0,
       0,   325,   326,   327,   328,   329,   323,   324,   473,     0,
      30,    32,    40,     0,   437,     0,     0,     0,   441,   463,
       0,     0,     0,     0,    26,     0,    27,     0,    28,     0,
     169,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
     131,   476,     0,     0,   363,   364,   266,   269,     0,   282,
     284,   286,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,   405,     0,     0,     0,     0,     0,   476,
       0,   476,     0,     0,     0,     0,   117,   118,   119,   120,
     322,   430,   108,     0,     0,   219,     0,     0,     0,     0,
       0,     0,   477,     0,     0,   390,   469,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,   316,
       0,     0,   317,     0,     0,     0,     0,     0,     0,     0,
       0,   290,    66,    68,    70,    73,    71,    72,    67,    69,
      79,    81,    83,    86,    84,    85,    80,    82,     0,     0,
     420,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   418,     0,   414,     0,     0,
     258,     0,     0,     0,     0,   454,   459,   460,   474,    39,
       5,     0,    12,     0,     0,     0,     0,    19,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    90,    89,    93,    91,    92,    88,    44,     0,    94,
      95,    47,   125,   130,   129,     0,     0,   139,   156,     0,
     158,     0,     0,     0,     0,   271,     0,     0,   165,     0,
       0,     0,     0,     0,   177,     0,    50,    53,    54,    57,
      58,    34,    33,     0,     0,   160,   461,   462,     0,     0,
     141,   144,   145,   148,   149,     0,     0,   217,   218,     0,
       0,     0,     0,   215,   174,     0,     0,     0,     0,     0,
       0,     0,   253,   478,     0,   470,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   299,   306,   313,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,   426,     0,   423,     0,     0,    61,     0,
      63,   175,     0,    16,   457,   456,   455,     0,     0,     0,
     417,   412,   413,     0,     0,     0,   409,     0,     0,   263,
     262,     0,     0,     0,    21,     0,    22,     0,    25,   168,
       0,    36,    35,     0,   478,   478,     0,   132,     0,   133,
       0,     0,     0,   267,   278,     0,   272,   273,   164,   274,
     270,     0,   283,   285,   287,     0,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
      77,    78,     0,     0,     0,   214,     0,     0,     0,     0,
       0,     0,     0,     0,   173,   171,     0,     0,     0,     0,
       0,     9,     0,     0,   454,   460,   310,     0,     0,     0,
       0,     0,     0,   318,   319,   320,   321,   291,   293,   295,
     298,   296,   297,   292,   294,   425,   422,   421,    60,   478,
      74,    75,    76,   259,   260,     0,     0,    13,    20,     0,
       0,     0,     0,   167,     7,     0,    45,    48,   127,   126,
     476,     0,   137,   157,   154,   155,   280,     0,     0,   275,
       0,     0,     0,     0,     0,   181,   182,   179,     0,     0,
       0,     0,     0,     0,     0,     0,   163,   161,   142,     0,
       0,   153,     0,     3,   249,     0,   246,     0,     0,     0,
       0,   225,     0,     0,     0,     0,   220,     0,     0,     0,
       0,   221,   222,     0,   170,   172,   255,   256,   257,     0,
     176,     0,     0,     0,   300,   302,   305,   303,   304,   301,
      17,     0,   478,   265,   264,     0,    23,     0,   134,   135,
     268,   279,     0,   276,   277,     0,     0,     0,   183,   185,
     187,   190,   188,   189,   184,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,     0,     0,     0,
     237,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,   226,   223,   224,     0,   252,   309,   308,   175,     0,
       0,   261,    14,   478,     8,     0,   138,   281,   191,   193,
     192,     0,   152,   248,     0,   195,     0,     0,     0,   194,
       0,   245,     0,   234,   235,   229,   232,   233,   228,     0,
       0,   238,     0,     0,     0,   239,     0,     0,     0,   307,
     311,   312,    24,   136,     0,   247,     0,     0,   198,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   196,   244,
     242,   243,   231,   240,   241,   230,   251,     0,     0,     0,
       0,     0,   199,   200,   197,     0,     0,     0,     0,     0,
       0,     0,     0,   250,   151,     0,     0,     0,   201,   203,
     205,   208,   206,   207,   202,   204,   209,   211,   210
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -877,  -877,     1,  -877,  -559,  -729,  -877,  -877,  -877,  -877,
    -877,   275,  1387,  -877,  -877,  -877,  -877,  -323,  -876,   146,
    -877,  -877,  -877,   802,  -877,  -877,  -505,   878,  -386,   264,
    1081,    -2,  -234,  -877,  1184,  1183,   832,  1170,  -877,   670,
     -16,  1169,   873,   877,     5,  -877,  1283,  -263,  -877,  -877,
      86,  -877
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   215,    68,    69,   570,    70,    71,    72,    73,
      74,  1000,    75,    76,   594,    77,    78,   358,  1004,   197,
      79,    80,   394,   616,    81,   621,   295,   541,   583,   127,
     186,   137,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,   211,   485,    98,    99,
     138,   100
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      83,     2,   437,   867,   550,   613,   688,   487,   692,   733,
     618,   155,   694,    27,   132,   614,   156,    25,   397,   745,
     128,   147,   149,   151,   153,   154,   979,   743,   160,   162,
     164,  1006,   117,   168,   169,   576,   577,   173,   175,   578,
    1007,   763,   764,   741,   496,   190,   191,   112,   977,   194,
    1008,   179,  1012,     3,   576,   577,   742,   343,   578,   497,
     201,   852,  1013,   210,   213,   113,   205,   206,   207,   199,
     853,   180,   398,   351,   116,   576,   577,   344,   512,   578,
     780,   781,   257,   289,   782,   746,   747,   290,   200,  1082,
     780,   781,   980,   981,   782,   257,   579,   580,   581,  1083,
    1009,   739,   272,   273,   274,   275,   276,   277,   133,   134,
     282,   284,  1014,   286,   287,   579,   580,   581,   316,   167,
      67,   317,   744,   352,   210,   705,    67,   118,    67,   119,
     615,   310,   312,   291,   314,   693,   579,   580,   581,   765,
     972,    67,   170,   978,   332,  1010,   342,  1095,   347,  1084,
     349,   317,   922,   118,    67,   119,   181,  1015,   783,  1011,
     582,   171,   345,   715,   114,   346,   826,   923,   905,   807,
     376,  1016,   378,   343,   118,   383,   119,   384,   385,   617,
     386,   387,   115,   388,   390,   391,   388,   392,    67,   789,
     790,   791,  1113,   344,  1085,   359,  1121,   280,   360,   198,
     714,   203,   576,   577,   202,   296,   578,   185,  1086,   255,
     256,   102,   103,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   315,   427,   428,   429,   430,   431,   432,   433,
     434,   318,   435,   787,   788,   204,   354,   355,  1145,   458,
     459,   111,   343,   320,   214,   319,  1159,   576,   577,   792,
     257,   578,   139,   140,   141,   500,   142,   321,   501,   748,
     322,  1166,   344,   579,   580,   581,    82,   465,   466,   467,
     468,   469,   470,   471,   323,   143,   637,   257,   345,   499,
     144,   346,   576,   577,   177,   986,   578,   702,   234,    67,
     703,  1183,   157,   159,   784,   518,   638,   520,   521,   522,
     523,   524,   525,   526,   527,   176,   178,   968,   579,   580,
     581,   298,   237,   192,   193,   534,   535,   508,   537,  1106,
     542,   165,   576,   577,   926,   166,   578,   786,   640,   299,
    1087,   555,   556,   557,   558,   559,   560,   561,   574,   575,
    1088,   564,   565,   579,   580,   581,   306,   345,   641,   254,
     346,   328,   356,   269,  1173,   329,   372,   357,   584,   586,
     587,   268,   588,   589,   297,   590,   591,   238,   239,   240,
     241,   596,  1068,   598,   373,   343,   145,   182,   602,   603,
     374,   181,   380,   579,   580,   581,   361,   611,   639,   157,
    1089,   859,   270,   529,   361,   344,   271,   183,   375,   361,
     381,   242,   243,   112,   114,   278,   362,  1072,   364,   632,
     633,   634,   636,   285,   362,   348,   543,   350,   353,   362,
     549,   629,   630,   361,   606,   357,   878,   562,   365,   288,
     544,   933,   566,   184,   822,  1090,   545,   377,   573,   379,
     642,   547,   707,   362,   297,   708,   585,  1137,   643,  1091,
     546,   644,   645,   607,   800,   548,   593,   334,   801,   608,
     934,   646,   914,   915,   647,   648,   935,   802,   649,   650,
     803,   804,   889,  1002,   805,   890,   891,   609,  1003,    82,
     345,   330,   184,   346,   936,   892,   327,   623,   893,   894,
     627,   363,   895,   896,   674,   660,   195,   196,   809,   363,
     679,   680,   937,   982,   363,  1150,   334,   983,  1151,  1152,
     690,   987,   307,   366,   308,   534,   336,   907,  1153,   337,
     338,  1154,  1155,   984,  1017,  1156,  1157,  1018,   363,   359,
    1019,  1020,   360,   661,   701,  1174,   808,   963,   210,   662,
     938,   710,   118,   712,   119,   713,   939,   663,  1169,   716,
     717,   334,  1170,   233,   940,   727,   664,   370,   317,   340,
     651,   336,   341,   941,   337,   338,   806,   736,  1171,   576,
     577,   665,   666,   578,   667,   673,   367,   233,   942,   943,
     368,   944,   755,   676,   897,   333,  1005,   678,   334,   335,
     681,  1003,   720,  1024,  1025,   686,   369,   689,   336,  1034,
     722,   337,   338,   776,  1094,   339,   723,  1035,   340,  1003,
     233,   341,   393,  1036,   724,   985,  1069,  1158,   725,  1073,
     731,  1037,   794,   317,   396,  1038,   726,   797,   798,   799,
     579,   580,   581,  1039,   756,   810,   811,   317,   812,   813,
     719,   814,   815,   816,   817,   818,   819,   820,   821,   728,
    1023,   216,   371,   340,   217,   218,   341,   612,   879,   880,
    1172,   395,   881,   833,   219,   399,   317,   220,   221,   406,
     908,   222,   223,   909,   910,   225,   911,   912,   226,   227,
     837,   838,   839,   262,   263,   264,   507,   660,   228,   843,
     340,   229,   230,   341,  1058,   231,   232,   661,   662,   265,
     266,   267,   849,   850,  1059,  1060,   235,   236,  1138,   855,
     244,   245,   246,   247,   248,   249,   863,   663,   664,   882,
     883,   884,   250,   665,  1061,  1062,   472,   490,   666,   856,
    1063,   491,   857,   868,   251,  1064,   473,   667,   474,   475,
     957,   551,   874,   317,  1065,   492,   830,   476,   477,   885,
     886,   888,   478,  1096,   493,   824,   224,   827,   479,   480,
     404,   898,   405,   832,   720,   722,   481,   482,   483,   484,
     726,  1108,  1109,   903,   950,   952,   906,  1110,   660,   661,
     233,  1140,  1141,   662,   663,  1188,  1189,   664,   665,   848,
    1190,  1191,   252,   253,  1192,  1193,   258,   259,   927,   928,
     929,   930,   931,   932,     4,     5,     6,     7,   504,   666,
     505,   120,   121,   122,   123,   124,  1194,    13,    14,    15,
      16,    17,    18,    19,   102,   103,   300,   301,   302,   303,
     304,   732,   734,   667,   125,   737,   738,   740,   720,   305,
    1195,    35,   954,   722,   726,  1196,   964,    37,    82,   965,
    1197,  1198,   400,   401,   111,    45,    46,    47,    48,  1046,
     494,    51,   317,   770,   771,   772,  1104,   260,   261,   317,
      55,    56,   504,   976,   695,   495,    57,   324,   325,   326,
     102,   103,   300,   301,   302,   303,   304,   988,   989,   990,
     991,   992,   993,   994,   995,   305,   451,   452,   453,   317,
     506,   698,  1001,   508,   472,    59,    60,   317,   486,   795,
     111,  -389,  -389,   317,   473,   836,   474,   475,   402,   403,
    1029,    61,   455,   456,   457,   476,   477,    62,    63,   472,
     478,   509,   504,   956,   924,   126,   479,   480,    66,   473,
      67,   474,   475,   962,   481,   482,   483,   484,   488,   489,
     476,   477,   511,  1041,   510,   478,   513,  1043,  1044,   498,
     317,   479,   480,   502,   503,   514,   515,   568,   569,   481,
     482,   483,   484,   516,   517,   519,  1051,   533,   317,   530,
    1055,  1056,  1057,   531,   101,   532,   102,   103,   104,   105,
     106,   107,   108,   553,   403,   400,   554,  1066,   400,   563,
     869,   109,   866,   569,  1071,   567,   401,   552,  1077,  1031,
     571,   401,   572,  1081,   110,   595,   111,   592,   599,   401,
     601,   401,   400,   677,   971,   569,   749,   750,   899,   597,
    1099,  1100,   101,   600,   102,   103,   104,   105,   106,   107,
     108,   785,   750,   793,   317,   829,   317,  1045,   604,   109,
    1107,  1047,   831,   317,   840,   317,   841,   515,   842,   317,
     846,   515,   628,   619,   111,   605,   400,   847,   606,  1120,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   620,  1132,   851,   708,   860,   750,  1136,   864,
     317,   865,   317,   916,   750,   917,   750,   945,   317,   946,
     515,   947,   317,   949,   317,   400,   961,   970,   401,  1001,
    1042,   317,  1098,   317,  1146,  1147,  1149,  1103,   317,  1105,
     569,  1114,   750,  1122,   750,  1167,   750,   187,   189,   607,
     969,   461,   462,   608,   973,   974,   975,   463,   464,   609,
     624,   625,   631,   610,   652,   653,   654,   655,  1175,  1176,
    1177,  1178,  1179,  1180,  1181,  1182,   668,   656,   657,   658,
     659,   660,   996,   661,   662,   663,   998,  1185,  1186,  1187,
       4,     5,     6,     7,   664,   665,   666,   129,   130,   122,
     123,   292,   667,    13,    14,    15,    16,    17,    18,    19,
     251,   257,   669,   670,   671,   682,   683,   684,   691,   697,
     125,    29,   696,   699,   700,   704,   709,    35,   718,   767,
     317,   357,   720,    37,   777,   721,   722,   723,   724,   725,
     726,    45,    46,    47,    48,   729,   730,    51,   757,   758,
     759,   760,   761,   762,  1048,  1049,    55,    56,   766,   768,
     773,   769,    57,   774,   775,   778,   796,   828,   830,   834,
     835,   844,   845,   854,   858,   870,   861,   862,   871,   872,
     873,   875,   876,   877,   902,   900,     4,     5,     6,     7,
     901,    59,    60,   129,   130,   122,   123,   292,   904,    13,
      14,    15,    16,    17,    18,    19,   913,    61,   918,   919,
     920,   921,   925,    62,    63,   948,   125,    29,   950,   951,
     959,   126,   293,    35,    66,   952,    67,   955,   294,    37,
     953,   958,   960,   966,   967,   999,   997,    45,    46,    47,
      48,  1021,  1022,    51,  1026,  1027,  1028,  1032,  1033,  1040,
    1050,  1112,    55,    56,  1052,    82,  1053,  1054,    57,  1067,
    1070,  1097,     4,     5,     6,     7,  1074,  1075,  1078,   129,
     130,   122,   123,   292,  1076,    13,    14,    15,    16,    17,
      18,    19,  1079,  1080,  1003,  1129,  1092,    59,    60,  1143,
    1093,  1101,   125,    29,  1102,  1111,  1123,  1124,  1130,    35,
    1115,  1133,  1134,    61,  1125,    37,  1144,  1126,  1127,    62,
      63,  1128,  1131,    45,    46,    47,    48,   126,   687,    51,
      66,  1135,    67,  1142,   294,  1139,  1160,  1168,    55,    56,
    1161,  1162,  1163,  1164,    57,  1165,   158,   779,   706,   436,
     438,   454,     0,     0,     0,     0,   460,     0,   331,     0,
       0,     0,  1184,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,    59,    60,   129,   130,   122,   123,   292,
       0,    13,    14,    15,    16,    17,    18,    19,     0,    61,
       0,     0,     0,     0,     0,    62,    63,     0,   125,    29,
       0,     0,     0,   126,   825,    35,    66,     0,    67,     0,
     294,    37,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,    48,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,    62,    63,     0,     0,     0,     0,     0,     0,   126,
       0,     0,    66,     0,    67,     0,   294,     4,     5,     6,
       7,     0,     0,     0,   407,   408,   409,   410,    12,     0,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
     411,   412,    25,     0,     0,    26,    27,    28,    29,   413,
     414,    32,    33,    34,    35,    36,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,   415,   416,    45,    46,
      47,    48,    49,    50,    51,    52,     0,     0,     0,    53,
      54,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
      62,    63,     0,     0,     0,     0,     0,    64,    65,     0,
       0,    66,     0,    67,   417,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,     0,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,   751,   752,
      25,     0,     0,    26,    27,    28,    29,    30,   753,    32,
      33,    34,    35,    36,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,     0,     0,     0,    53,    54,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,    62,    63,
       0,     0,     0,     0,     0,    64,    65,     0,     0,    66,
       0,    67,   754,     4,     5,     6,     7,     0,     0,     0,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,  1116,  1117,    25,     0,
       0,    26,    27,    28,    29,    30,  1118,    32,    33,    34,
      35,    36,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     0,     0,     0,    53,    54,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,    59,    60,   129,   130,   122,   123,
     124,     0,    13,    14,    15,    16,    17,    18,    19,     0,
      61,     0,     0,     0,     0,     0,    62,    63,     0,   125,
      29,     0,     0,    64,    65,     0,    35,    66,     0,    67,
    1119,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   129,   130,   122,   123,   124,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,   125,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    61,    37,     0,     0,
       0,     0,    62,    63,     0,    45,    46,    47,    48,   161,
     126,    51,     0,    66,     0,    67,   635,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   129,   130,   122,   123,
     124,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,   125,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    61,    37,     0,     0,     0,     0,    62,    63,     0,
      45,    46,    47,    48,   161,   126,    51,     0,    66,     0,
      67,   887,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,    62,    63,     0,     0,     0,     0,     0,   161,
     126,     0,     0,    66,     0,    67,  1148,     4,     5,     6,
       7,     0,     0,     0,     8,     9,    10,    11,    12,     0,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,     0,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     0,     0,     0,    53,
      54,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,    59,    60,
     129,   130,   122,   123,   124,     0,    13,    14,    15,    16,
      17,    18,    19,     0,    61,     0,     0,     0,     0,     0,
      62,    63,     0,   125,    29,     0,     0,    64,    65,     0,
      35,    66,     0,    67,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,    45,    46,    47,    48,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   129,   130,   122,   123,   124,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,   125,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      61,    37,     0,     0,     0,     0,    62,    63,     0,    45,
      46,    47,    48,     0,   126,    51,     0,    66,   131,    67,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     129,   130,   122,   123,   124,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,     0,    59,
      60,     0,     0,   125,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    61,    37,     0,     0,     0,
       0,    62,    63,     0,    45,    46,    47,    48,   135,   136,
      51,     0,    66,     0,    67,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   129,   130,   122,   123,   124,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,   125,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      61,    37,     0,     0,     0,     0,    62,    63,     0,    45,
      46,    47,    48,   161,   126,    51,     0,    66,     0,    67,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     208,   130,   122,   123,   124,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,     0,    59,
      60,     0,     0,   125,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    61,    37,     0,     0,     0,
       0,    62,    63,     0,    45,    46,    47,    48,   163,   126,
      51,     0,    66,     0,    67,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   129,   130,   122,   123,
     124,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,    59,    60,     0,     0,     0,   125,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
      61,     0,    37,     0,     0,     0,    62,    63,     0,     0,
      45,    46,    47,    48,   126,   209,    51,    66,     0,    67,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   129,   130,   122,   123,   124,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,   125,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    61,    37,     0,     0,
       0,     0,    62,    63,     0,    45,    46,    47,    48,     0,
     126,    51,     0,    66,   212,    67,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   129,   130,   122,
     123,   124,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    59,    60,     0,     0,     0,
     125,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    61,     0,    37,     0,     0,     0,    62,    63,     0,
       0,    45,    46,    47,    48,   126,   279,    51,    66,     0,
      67,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,   283,    45,    46,    47,    48,
       0,   126,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   129,   130,
     122,   123,   124,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,   125,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    61,     0,    37,     0,     0,     0,    62,    63,
       0,     0,    45,    46,    47,    48,   126,   209,    51,    66,
       0,    67,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   129,   130,   122,   123,   124,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   125,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,   309,   126,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   129,   130,
     122,   123,   124,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   125,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,   311,   126,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   129,   130,   122,   123,   124,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,   382,
       0,     0,    59,    60,     0,     0,     0,   125,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    61,     0,
      37,     0,     0,     0,    62,    63,     0,     0,    45,    46,
      47,    48,   126,   313,    51,    66,     0,    67,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   129,
     130,   122,   123,   124,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   125,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   126,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   129,   130,   122,   123,   124,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   125,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   126,    51,   389,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,   214,     4,     5,     6,     7,     0,     0,     0,
     129,   130,   122,   123,   124,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    59,    60,
       0,     0,     0,   125,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,    61,     0,    37,     0,     0,     0,
      62,    63,     0,     0,    45,    46,    47,    48,   126,     0,
      51,    66,     0,    67,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   129,   130,   122,   123,   124,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,   125,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      61,    37,     0,     0,     0,     0,    62,    63,     0,    45,
      46,    47,    48,     0,   126,    51,     0,    66,   528,    67,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   129,   130,   122,   123,   124,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,   125,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    61,     0,    37,     0,     0,
       0,    62,    63,     0,     0,    45,    46,    47,    48,   126,
     622,    51,    66,     0,    67,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   129,   130,   122,   123,
     124,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,   125,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    61,    37,     0,     0,     0,     0,    62,    63,     0,
      45,    46,    47,    48,     0,   126,    51,     0,    66,   626,
      67,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      59,    60,     0,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    61,     0,    37,     0,
       0,     0,    62,    63,     0,     0,    45,    46,    47,    48,
     126,   672,    51,    66,     0,    67,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   129,   130,
     122,   123,   124,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,   125,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    61,     0,    37,     0,     0,     0,    62,    63,
       0,     0,    45,    46,    47,    48,   126,   675,    51,    66,
       0,    67,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     4,     5,   538,
       7,     0,     0,     0,   129,   130,   122,   123,   539,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    59,    60,     0,     0,     0,   125,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    61,     0,
      37,     0,     0,     0,    62,    63,     0,     0,    45,    46,
      47,    48,   126,   685,    51,    66,     0,    67,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     129,   130,   122,   123,   124,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    59,    60,
       0,     0,     0,   125,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,    61,     0,    37,     0,     0,     0,
      62,    63,     0,     0,    45,    46,    47,    48,   126,   209,
      51,    66,     0,    67,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   129,   130,   122,   123,
     124,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,    59,    60,     0,     0,     0,   125,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
      61,     0,    37,     0,     0,     0,    62,    63,     0,     0,
      45,    46,    47,    48,   126,   823,    51,    66,     0,    67,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      59,    60,     0,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    61,     0,    37,     0,
       0,     0,    62,    63,     0,     0,    45,    46,    47,    48,
     126,  1030,    51,    66,     0,    67,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   129,   130,   122,
     123,   124,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     125,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   146,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   148,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   129,   130,   122,
     123,   124,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     125,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   150,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   152,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   129,   130,   122,
     123,   124,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     125,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   126,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   172,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   129,   130,   122,
     123,   124,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     125,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   174,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   126,    51,     0,    66,     0,   188,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   129,   130,   122,
     123,   124,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     125,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   281,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,   538,     7,     0,
       0,     0,   129,   130,   122,   123,   539,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   536,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   129,   130,   122,
     123,   124,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     125,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   540,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   129,   130,   122,   123,   124,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   125,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   711,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -479,  -479,     0,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,    62,    63,
       0,     0,     0,     0,     0,     0,   735,     0,     0,    66,
       0,    67,  -479,  -479,  -479,     0,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,     0,
       0,     0,  -479,     0,     0,     0,     0,     0,     0,     0,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,     0,     0,
    -479,  -479,  -479,  -479,  -479,     0,     0,  -479,  -479,  -479,
       0,  -479
};

static const yytype_int16 yycheck[] =
{
       2,     0,   236,   732,   327,    14,   511,   270,    14,   568,
     396,    27,   517,    32,    16,    24,    14,    28,    14,    10,
      15,    23,    24,    25,    26,    27,    10,    24,    30,    31,
      32,   907,    11,    35,    36,    10,    11,    39,    40,    14,
      14,    42,    43,    86,   119,    47,    48,   119,    24,    51,
      24,    28,    14,     0,    10,    11,    86,    28,    14,   134,
      55,     5,    24,    65,    66,   137,    61,    62,    63,    14,
      14,    48,    68,    86,   119,    10,    11,    48,    83,    14,
      62,    63,   125,    14,    66,    76,    77,    18,    33,    14,
      62,    63,    76,    77,    66,   125,    71,    72,    73,    24,
      74,    86,   104,   105,   106,   107,   108,   109,    14,    15,
     112,   113,    74,   115,   116,    71,    72,    73,   133,    14,
     139,   136,   119,   136,   126,    83,   139,   132,   139,   134,
     139,   133,   134,    64,   136,   141,    71,    72,    73,   140,
     869,   139,    12,   119,   146,   119,   148,  1023,   150,    74,
     152,   136,   119,   132,   139,   134,   133,   119,   140,   133,
     135,    12,   133,   549,   121,   136,   671,   134,   140,   140,
     172,   133,   174,    28,   132,   177,   134,   179,   180,   135,
     182,   183,   139,   185,   186,   187,   188,   189,   139,    80,
      81,    82,  1068,    48,   119,   133,  1072,   111,   136,    53,
     135,    10,    10,    11,    58,   119,    14,   139,   133,    84,
      85,   104,   105,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   136,   225,   226,   227,   228,   229,   230,   231,
     232,   119,   234,   619,   620,    10,    84,    85,  1114,   255,
     256,   134,    28,   119,    78,   133,  1122,    10,    11,   140,
     125,    14,    10,    11,    12,   133,    14,   133,   136,   582,
     119,  1137,    48,    71,    72,    73,     2,   262,   263,   264,
     265,   266,   267,   268,   133,    33,    28,   125,   133,   281,
      38,   136,    10,    11,   134,   140,    14,   133,    83,   139,
     136,  1167,    28,    29,   617,   297,    48,   299,   300,   301,
     302,   303,   304,   305,   306,    41,    42,   866,    71,    72,
      73,   121,    97,    49,    50,   317,   318,   133,   320,  1048,
     322,    10,    10,    11,   140,    14,    14,   135,    28,   139,
      14,   333,   334,   335,   336,   337,   338,   339,   354,   355,
      24,   343,   344,    71,    72,    73,   119,   133,    48,   111,
     136,    10,   134,   132,   140,    14,   121,   139,   357,   361,
     362,   118,   364,   365,   137,   367,   368,    87,    88,    89,
      90,   373,   135,   375,   139,    28,   134,    28,   380,   381,
     121,   133,    31,    71,    72,    73,    28,   389,   140,   125,
      74,   714,   132,   307,    28,    48,    14,    48,   139,    28,
      49,   121,   122,   119,   121,    14,    48,   135,    28,   411,
     412,   413,   414,    14,    48,   151,   119,   153,   154,    48,
     134,   137,   139,    28,   133,   139,   749,   341,    48,   133,
     133,   140,   346,   133,   668,   119,   119,   173,   352,   175,
     140,   119,   133,    48,   137,   136,   360,   135,    28,   133,
     133,    31,    32,   133,    28,   133,   370,    31,    32,   133,
     140,    41,   785,   786,    44,    45,   140,    41,    48,    49,
      44,    45,    28,   134,    48,    31,    32,   133,   139,   215,
     133,    10,   133,   136,   140,    41,    14,   401,    44,    45,
     404,   133,    48,    49,   496,   133,    14,    15,   140,   133,
     502,   503,   140,    28,   133,    28,    31,    32,    31,    32,
     512,   140,   136,   133,   138,   517,    41,   780,    41,    44,
      45,    44,    45,    48,    10,    48,    49,    13,   133,   133,
      16,    17,   136,   133,   536,   140,   140,   860,   540,   133,
     140,   543,   132,   545,   134,   547,   140,   133,    28,   551,
     552,    31,    32,   133,   140,   133,   133,   136,   136,   133,
     140,    41,   136,   140,    44,    45,   140,   569,    48,    10,
      11,   133,   133,    14,   133,   489,   119,   133,   140,   140,
     119,   140,   584,   497,   140,    28,   134,   501,    31,    32,
     504,   139,   133,   916,   917,   509,    75,   511,    41,   140,
     133,    44,    45,   605,   134,    48,   133,   140,   133,   139,
     133,   136,    59,   140,   133,   140,  1002,   140,   133,  1005,
     133,   140,   624,   136,   134,   140,   133,   629,   630,   631,
      71,    72,    73,   140,   133,   637,   638,   136,   640,   641,
     554,   643,   644,   645,   646,   647,   648,   649,   650,   563,
     913,    28,   139,   133,    31,    32,   136,   393,    10,    11,
     140,   139,    14,   133,    41,   139,   136,    44,    45,   140,
      10,    48,    49,    13,    14,    28,    16,    17,    31,    32,
     682,   683,   684,   112,   113,   114,   140,   133,    41,   691,
     133,    44,    45,   136,   140,    48,    49,   133,   133,   128,
     129,   130,   704,   705,   140,   140,    98,    99,  1094,   711,
      91,    92,    93,    94,    95,    96,   718,   133,   133,    71,
      72,    73,   103,   133,   140,   140,    14,   133,   133,   133,
     140,   133,   136,   735,   115,   140,    24,   133,    26,    27,
     133,   119,   744,   136,   140,   133,   133,    35,    36,   751,
     752,   753,    40,   140,   133,   669,   133,   671,    46,    47,
     136,   763,   138,   677,   133,   133,    54,    55,    56,    57,
     133,   140,   140,   775,   133,   133,    64,   140,   133,   133,
     133,   140,   140,   133,   133,   140,   140,   133,   133,   703,
     140,   140,   123,   124,   140,   140,   116,   117,   800,   801,
     802,   803,   804,   805,     3,     4,     5,     6,   136,   133,
     138,    10,    11,    12,    13,    14,   140,    16,    17,    18,
      19,    20,    21,    22,   104,   105,   106,   107,   108,   109,
     110,   567,   568,   133,    33,   571,   572,   573,   133,   119,
     140,    40,   844,   133,   133,   140,   133,    46,   584,   136,
     140,   140,   135,   136,   134,    54,    55,    56,    57,   133,
     133,    60,   136,   599,   600,   601,   133,   126,   127,   136,
      69,    70,   136,   875,   138,   133,    75,    10,    11,    12,
     104,   105,   106,   107,   108,   109,   110,   889,   890,   891,
     892,   893,   894,   895,   896,   119,     7,     8,     9,   136,
     122,   138,   904,   133,    14,   104,   105,   136,    18,   138,
     134,   135,   136,   136,    24,   138,    26,    27,   135,   136,
     922,   120,   252,   253,   254,    35,    36,   126,   127,    14,
      40,   134,   136,   847,   138,   134,    46,    47,   137,    24,
     139,    26,    27,   857,    54,    55,    56,    57,   133,   134,
      35,    36,   134,   955,    14,    40,    14,   959,   960,   135,
     136,    46,    47,   126,   127,   135,   136,    29,    30,    54,
      55,    56,    57,   135,   136,    14,   978,   135,   136,   133,
     982,   983,   984,   133,   102,   133,   104,   105,   106,   107,
     108,   109,   110,   135,   136,   135,   136,   999,   135,   136,
     736,   119,    29,    30,  1003,   135,   136,   119,  1010,   923,
     135,   136,    10,  1015,   132,    14,   134,   133,   135,   136,
     135,   136,   135,   136,    29,    30,   135,   136,   764,    14,
    1032,  1033,   102,    29,   104,   105,   106,   107,   108,   109,
     110,   135,   136,   135,   136,   135,   136,   961,    10,   119,
    1052,   965,   135,   136,   135,   136,   135,   136,   135,   136,
     135,   136,   132,   134,   134,   133,   135,   136,   133,  1071,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   134,  1085,   135,   136,   135,   136,  1090,   135,
     136,   135,   136,   135,   136,   135,   136,   135,   136,   135,
     136,   135,   136,   135,   136,   135,   136,   135,   136,  1111,
     135,   136,   135,   136,  1116,  1117,  1118,   135,   136,    29,
      30,   135,   136,   135,   136,   135,   136,    46,    47,   133,
     866,   258,   259,   133,   870,   871,   872,   260,   261,   133,
     119,    10,   119,   140,   133,   133,   133,   133,  1150,  1151,
    1152,  1153,  1154,  1155,  1156,  1157,   100,   133,   133,   133,
     133,   133,   898,   133,   133,   133,   902,  1169,  1170,  1171,
       3,     4,     5,     6,   133,   133,   133,    10,    11,    12,
      13,    14,   133,    16,    17,    18,    19,    20,    21,    22,
     115,   125,   134,    14,   134,   119,   119,   119,   134,   140,
      33,    34,   122,   133,   133,    83,   133,    40,   119,   140,
     136,   139,   133,    46,    14,   133,   133,   133,   133,   133,
     133,    54,    55,    56,    57,   133,   133,    60,   133,   133,
     133,   133,   133,   133,   970,   971,    69,    70,   122,   122,
     133,   140,    75,   133,   119,    14,    14,   133,   133,   138,
     138,    83,    14,   133,   133,    10,   133,   133,    10,    10,
      14,   119,    10,    10,    29,   133,     3,     4,     5,     6,
     133,   104,   105,    10,    11,    12,    13,    14,   133,    16,
      17,    18,    19,    20,    21,    22,    62,   120,    14,    14,
      14,   133,   140,   126,   127,   133,    33,    34,   133,   133,
      83,   134,   135,    40,   137,   133,   139,   134,   141,    46,
     135,   133,    83,   133,   133,   133,   140,    54,    55,    56,
      57,   133,   133,    60,     7,     7,     7,   119,   119,   133,
      14,  1067,    69,    70,   119,  1071,    10,    10,    75,   135,
      78,   140,     3,     4,     5,     6,    14,    14,    14,    10,
      11,    12,    13,    14,   133,    16,    17,    18,    19,    20,
      21,    22,    14,   133,   139,    14,   133,   104,   105,  1105,
     133,   135,    33,    34,   133,   133,   133,   133,    14,    40,
     140,    14,    14,   120,   133,    46,  1111,   133,   133,   126,
     127,   133,   133,    54,    55,    56,    57,   134,   135,    60,
     137,   133,   139,   133,   141,   140,   133,   135,    69,    70,
     133,   133,   133,   133,    75,   133,    29,   615,   540,   235,
     237,   251,    -1,    -1,    -1,    -1,   257,    -1,   145,    -1,
      -1,    -1,  1168,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,   104,   105,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,   120,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,    33,    34,
      -1,    -1,    -1,   134,   135,    40,   137,    -1,   139,    -1,
     141,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,   134,
      -1,    -1,   137,    -1,   139,    -1,   141,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    -1,    65,
      66,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,   137,    -1,   139,   140,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    -1,    -1,    65,    66,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,   137,
      -1,   139,   140,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,   104,   105,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
     120,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,    33,
      34,    -1,    -1,   133,   134,    -1,    40,   137,    -1,   139,
     140,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,
      -1,    -1,   126,   127,    -1,    54,    55,    56,    57,   133,
     134,    60,    -1,   137,    -1,   139,   140,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,
      54,    55,    56,    57,   133,   134,    60,    -1,   137,    -1,
     139,   140,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,   126,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,   137,    -1,   139,   140,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    -1,    65,
      66,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,   104,   105,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,   120,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,    33,    34,    -1,    -1,   133,   134,    -1,
      40,   137,    -1,   139,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,    54,
      55,    56,    57,    -1,   134,    60,    -1,   137,   138,   139,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,    -1,
      -1,   126,   127,    -1,    54,    55,    56,    57,   133,   134,
      60,    -1,   137,    -1,   139,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,    54,
      55,    56,    57,   133,   134,    60,    -1,   137,    -1,   139,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,    -1,
      -1,   126,   127,    -1,    54,    55,    56,    57,   133,   134,
      60,    -1,   137,    -1,   139,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
     120,    -1,    46,    -1,    -1,    -1,   126,   127,    -1,    -1,
      54,    55,    56,    57,   134,   135,    60,   137,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,
      -1,    -1,   126,   127,    -1,    54,    55,    56,    57,    -1,
     134,    60,    -1,   137,   138,   139,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   120,    -1,    46,    -1,    -1,    -1,   126,   127,    -1,
      -1,    54,    55,    56,    57,   134,   135,    60,   137,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,   128,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,   120,    -1,    46,    -1,    -1,    -1,   126,   127,
      -1,    -1,    54,    55,    56,    57,   134,   135,    60,   137,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,   133,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,   133,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      -1,    -1,   104,   105,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   120,    -1,
      46,    -1,    -1,    -1,   126,   127,    -1,    -1,    54,    55,
      56,    57,   134,   135,    60,   137,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,
     126,   127,    -1,    54,    55,    56,    57,    -1,   134,    60,
      -1,   137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,
      46,    -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,
      56,    57,    -1,   134,    60,   136,   137,    -1,   139,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    78,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,
     126,   127,    -1,    -1,    54,    55,    56,    57,   134,    -1,
      60,   137,    -1,   139,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,    54,
      55,    56,    57,    -1,   134,    60,    -1,   137,   138,   139,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   120,    -1,    46,    -1,    -1,
      -1,   126,   127,    -1,    -1,    54,    55,    56,    57,   134,
     135,    60,   137,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,
      54,    55,    56,    57,    -1,   134,    60,    -1,   137,   138,
     139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,   120,    -1,    46,    -1,
      -1,    -1,   126,   127,    -1,    -1,    54,    55,    56,    57,
     134,   135,    60,   137,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,   120,    -1,    46,    -1,    -1,    -1,   126,   127,
      -1,    -1,    54,    55,    56,    57,   134,   135,    60,   137,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   120,    -1,
      46,    -1,    -1,    -1,   126,   127,    -1,    -1,    54,    55,
      56,    57,   134,   135,    60,   137,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,
     126,   127,    -1,    -1,    54,    55,    56,    57,   134,   135,
      60,   137,    -1,   139,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
     120,    -1,    46,    -1,    -1,    -1,   126,   127,    -1,    -1,
      54,    55,    56,    57,   134,   135,    60,   137,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,   120,    -1,    46,    -1,
      -1,    -1,   126,   127,    -1,    -1,    54,    55,    56,    57,
     134,   135,    60,   137,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,    -1,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,    -1,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,    -1,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,    -1,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,    -1,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,    -1,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    45,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,   137,
      -1,   139,    83,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,   125,    -1,    -1,   128,   129,   130,
      -1,   132
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   143,   144,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    65,    66,    69,    70,    75,    79,   104,
     105,   120,   126,   127,   133,   134,   137,   139,   145,   146,
     148,   149,   150,   151,   152,   154,   155,   157,   158,   162,
     163,   166,   171,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   190,   191,
     193,   102,   104,   105,   106,   107,   108,   109,   110,   119,
     132,   134,   119,   137,   121,   139,   119,    11,   132,   134,
      10,    11,    12,    13,    14,    33,   134,   171,   186,    10,
      11,   138,   173,    14,    15,   133,   134,   173,   192,    10,
      11,    12,    14,    33,    38,   134,   134,   173,   134,   173,
     134,   173,   134,   173,   173,   182,    14,   171,   154,   171,
     173,   133,   173,   133,   173,    10,    14,    14,   173,   173,
      12,    12,   134,   173,   134,   173,   171,   134,   171,    28,
      48,   133,    28,    48,   133,   139,   172,   172,   139,   172,
     173,   173,   171,   171,   173,    14,    15,   161,   161,    14,
      33,   186,   161,    10,    10,   186,   186,   186,    10,   135,
     173,   188,   138,   173,    78,   144,    28,    31,    32,    41,
      44,    45,    48,    49,   133,    28,    31,    32,    41,    44,
      45,    48,    49,   133,    83,    98,    99,    97,    87,    88,
      89,    90,   121,   122,    91,    92,    93,    94,    95,    96,
     103,   115,   123,   124,   111,    84,    85,   125,   116,   117,
     126,   127,   112,   113,   114,   128,   129,   130,   118,   132,
     132,    14,   173,   173,   173,   173,   173,   173,    14,   135,
     192,   134,   173,   128,   173,    14,   173,   173,   133,    14,
      18,    64,    14,   135,   141,   168,   192,   137,   121,   139,
     106,   107,   108,   109,   110,   119,   119,   136,   138,   133,
     173,   133,   173,   135,   173,   192,   133,   136,   119,   133,
     119,   133,   119,   133,    10,    11,    12,    14,    10,    14,
      10,   188,   173,    28,    31,    32,    41,    44,    45,    48,
     133,   136,   173,    28,    48,   133,   136,   173,   171,   173,
     171,    86,   136,   171,    84,    85,   134,   139,   159,   133,
     136,    28,    48,   133,    28,    48,   133,   119,   119,    75,
     136,   139,   121,   139,   121,   139,   173,   171,   173,   171,
      31,    49,    25,   173,   173,   173,   173,   173,   173,   136,
     173,   173,   173,    59,   164,   139,   134,    14,    68,   139,
     135,   136,   135,   136,   136,   138,   140,    10,    11,    12,
      13,    26,    27,    35,    36,    52,    53,   140,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   176,   174,   177,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,     7,     8,     9,   179,   181,   181,   181,   182,   182,
     183,   184,   184,   185,   185,   186,   186,   186,   186,   186,
     186,   186,    14,    24,    26,    27,    35,    36,    40,    46,
      47,    54,    55,    56,    57,   189,    18,   189,   133,   134,
     133,   133,   133,   133,   133,   133,   119,   134,   135,   173,
     133,   136,   126,   127,   136,   138,   122,   140,   133,   134,
      14,   134,    83,    14,   135,   136,   135,   136,   173,    14,
     173,   173,   173,   173,   173,   173,   173,   173,   138,   192,
     133,   133,   133,   135,   173,   173,   134,   173,     5,    14,
     134,   169,   173,   119,   133,   119,   133,   119,   133,   134,
     159,   119,   119,   135,   136,   173,   173,   173,   173,   173,
     173,   173,   192,   136,   173,   173,   192,   135,    29,    30,
     147,   135,    10,   192,   182,   182,    10,    11,    14,    71,
      72,    73,   135,   170,   144,   192,   173,   173,   173,   173,
     173,   173,   133,   192,   156,    14,   173,    14,   173,   135,
      29,   135,   173,   173,    10,   133,   133,   133,   133,   133,
     140,   173,   171,    14,    24,   139,   165,   135,   170,   134,
     134,   167,   135,   192,   119,    10,   138,   192,   132,   137,
     139,   119,   173,   173,   173,   140,   173,    28,    48,   140,
      28,    48,   140,    28,    31,    32,    41,    44,    45,    48,
      49,   140,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   100,   134,
      14,   134,   135,   192,   173,   135,   192,   136,   192,   173,
     173,   192,   119,   119,   119,   135,   192,   135,   168,   192,
     173,   134,    14,   141,   168,   138,   122,   140,   138,   133,
     133,   173,   133,   136,    83,    83,   169,   133,   136,   133,
     173,   134,   173,   173,   135,   170,   173,   173,   119,   192,
     133,   133,   133,   133,   133,   133,   133,   133,   192,   133,
     133,   133,   171,   146,   171,   134,   173,   171,   171,    86,
     171,    86,    86,    24,   119,    10,    76,    77,   159,   135,
     136,    26,    27,    36,   140,   173,   133,   133,   133,   133,
     133,   133,   133,    42,    43,   140,   122,   140,   122,   140,
     171,   171,   171,   133,   133,   119,   173,    14,    14,   165,
      62,    63,    66,   140,   159,   135,   135,   170,   170,    80,
      81,    82,   140,   135,   173,   138,    14,   173,   173,   173,
      28,    32,    41,    44,    45,    48,   140,   140,   140,   140,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   174,   135,   192,   135,   168,   192,   133,   135,
     133,   135,   192,   133,   138,   138,   138,   173,   173,   173,
     135,   135,   135,   173,    83,    14,   135,   136,   192,   173,
     173,   135,     5,    14,   133,   173,   133,   136,   133,   159,
     135,   133,   133,   173,   135,   135,    29,   147,   173,   171,
      10,    10,    10,    14,   173,   119,    10,    10,   159,    10,
      11,    14,    71,    72,    73,   173,   173,   140,   173,    28,
      31,    32,    41,    44,    45,    48,    49,   140,   173,   171,
     133,   133,    29,   173,   133,   140,    64,   189,    10,    13,
      14,    16,    17,    62,   159,   159,   135,   135,    14,    14,
      14,   133,   119,   134,   138,   140,   140,   173,   173,   173,
     173,   173,   173,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   135,   135,   135,   133,   135,
     133,   133,   133,   135,   173,   134,   192,   133,   133,    83,
      83,   136,   192,   159,   133,   136,   133,   133,   146,   171,
     135,    29,   147,   171,   171,   171,   173,    24,   119,    10,
      76,    77,    28,    32,    48,   140,   140,   140,   173,   173,
     173,   173,   173,   173,   173,   173,   171,   140,   171,   133,
     153,   173,   134,   139,   160,   134,   160,    14,    24,    74,
     119,   133,    14,    24,    74,   119,   133,    10,    13,    16,
      17,   133,   133,   189,   159,   159,     7,     7,     7,   173,
     135,   192,   119,   119,   140,   140,   140,   140,   140,   140,
     133,   173,   135,   173,   173,   192,   133,   192,   171,   171,
      14,   173,   119,    10,    10,   173,   173,   173,   140,   140,
     140,   140,   140,   140,   140,   140,   173,   135,   135,   170,
      78,   144,   135,   170,    14,    14,   133,   173,    14,    14,
     133,   173,    14,    24,    74,   119,   133,    14,    24,    74,
     119,   133,   133,   133,   134,   160,   140,   140,   135,   173,
     173,   135,   133,   135,   133,    29,   147,   173,   140,   140,
     140,   133,   171,   160,   135,   140,    26,    27,    36,   140,
     173,   160,   135,   133,   133,   133,   133,   133,   133,    14,
      14,   133,   173,    14,    14,   133,   173,   135,   170,   140,
     140,   140,   133,   171,   153,   160,   173,   173,   140,   173,
      28,    31,    32,    41,    44,    45,    48,    49,   140,   160,
     133,   133,   133,   133,   133,   133,   160,   135,   135,    28,
      32,    48,   140,   140,   140,   173,   173,   173,   173,   173,
     173,   173,   173,   160,   171,   173,   173,   173,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   142,   143,   144,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   146,   146,   146,   146,   146,
     146,   146,   146,   147,   147,   147,   147,   147,   147,   148,
     148,   149,   149,   149,   149,   150,   150,   151,   151,   151,
     152,   152,   152,   153,   154,   154,   154,   154,   154,   154,
     155,   155,   156,   156,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   158,   158,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   161,   161,   162,   163,   164,   164,   164,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   166,   167,   167,   167,   167,   168,   168,
     168,   168,   169,   169,   169,   169,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   172,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   174,   174,   175,   175,   176,   176,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   178,   178,   179,   179,
     179,   180,   180,   181,   181,   181,   182,   182,   183,   183,
     183,   184,   184,   184,   185,   185,   185,   185,   185,   185,
     185,   186,   186,   186,   186,   186,   186,   187,   187,   188,
     188,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   190,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   192,   192,   192,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193
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
       4,     1,     1,     1,     1,     5,     7,     7,     3,     5,
       5,     4,     6,     3,     5,     5,     7,     4,     6,     5,
       3,     5,     7,     3,     5,     5,     3,     2,     5,     5,
       2,    12,     9,     1,     7,     7,     5,     7,     5,     3,
       5,     7,     0,     4,     6,     5,     3,     7,     6,     4,
       7,     6,     7,     6,     5,     6,     5,     3,     4,     5,
       4,     5,     5,     6,     6,     6,     6,     6,     6,     6,
       6,     7,     7,     7,     3,     3,     4,     5,     4,     5,
       5,     6,     6,     6,     6,     6,     6,     6,     6,     7,
       7,     7,     1,     1,     6,     5,     0,     3,     3,     0,
       4,     4,     4,     5,     5,     4,     5,     5,     6,     6,
       7,     7,     6,     6,     6,     6,     5,     5,     6,     6,
       7,     7,     7,     7,     7,     6,     4,     7,     6,     4,
       8,     7,     5,     5,     0,     4,     4,     4,     3,     5,
       5,     7,     3,     3,     5,     5,     1,     3,     5,     1,
       3,     2,     3,     3,     3,     4,     5,     5,     3,     5,
       4,     6,     1,     3,     1,     3,     1,     3,     3,     3,
       4,     6,     6,     6,     6,     6,     6,     6,     6,     5,
       7,     7,     7,     7,     7,     7,     5,     9,     8,     8,
       6,     9,     9,     5,     5,     4,     4,     4,     6,     6,
       6,     6,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     3,     1,     5,     1,     3,     1,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     2,     2,     2,     1,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     3,     4,     6,
       4,     3,     6,     6,     5,     3,     3,     6,     5,     4,
       4,     6,     6,     5,     3,     6,     5,     3,     2,     3,
       4,     3,     2,     3,     2,     1,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     4,     6,     6,     6,     1,     4,
       4,     5,     5,     1,     1,     1,     1,     2,     3,     4,
       5,     2,     3,     4,     5,     2,     3,     4,     5,     1,
       2
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
#line 493 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3248 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 503 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3254 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 504 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3260 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 508 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3266 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 510 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3272 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 512 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3278 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 514 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3286 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 518 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3292 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 520 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3298 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 522 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3304 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 524 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3310 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 526 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3318 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 530 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3326 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 534 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3332 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 536 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3340 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 540 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3348 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 544 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3354 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 546 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3360 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 548 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3366 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 550 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3372 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 552 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3378 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 554 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3386 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 558 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3394 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 562 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3400 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 564 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3406 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 566 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3412 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 568 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3418 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 570 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3424 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 572 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3430 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 574 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3436 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 576 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3442 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 578 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3448 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 580 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3454 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 582 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3460 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 584 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3466 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 586 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3478 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 594 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3484 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 596 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3490 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 598 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3496 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 600 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3502 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 602 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3508 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 604 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3514 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 606 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3521 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 609 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3528 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 612 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3534 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 614 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3541 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 617 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3548 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 620 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3554 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 622 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3562 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 626 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3568 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 628 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3574 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 630 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3581 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 633 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3588 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 636 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3594 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 638 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3601 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 641 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3608 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 644 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3615 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 647 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3621 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 649 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3630 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 654 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3638 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 658 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3646 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 662 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3655 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 667 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3663 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 670 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3669 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 672 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3675 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 674 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3681 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 676 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3687 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 678 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3693 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 680 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3700 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 683 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3706 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 685 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3712 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 687 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3718 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 689 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3725 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 692 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3732 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 695 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3739 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 698 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3746 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 701 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3753 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 704 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3759 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 706 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3765 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 708 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3771 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 710 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3777 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 712 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3784 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 715 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3790 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 717 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3796 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 719 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3802 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 721 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3809 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 724 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3816 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 727 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3824 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 731 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3831 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 734 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3837 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 736 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3843 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 738 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3849 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 740 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3856 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 743 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3863 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 746 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3870 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 749 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3877 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 752 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3884 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 755 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3891 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 758 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3898 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 760 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3904 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 761 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3910 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 762 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3916 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 763 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3922 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 764 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3928 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 765 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3934 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 767 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3940 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 769 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3946 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH block  */
#line 771 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3952 "raku.tab.c"
    break;

  case 110: /* stmt: block  */
#line 773 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3958 "raku.tab.c"
    break;

  case 111: /* stmt: unless_stmt  */
#line 774 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3964 "raku.tab.c"
    break;

  case 112: /* stmt: until_stmt  */
#line 775 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3970 "raku.tab.c"
    break;

  case 113: /* stmt: repeat_stmt  */
#line 776 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3976 "raku.tab.c"
    break;

  case 114: /* stmt: loop_stmt  */
#line 777 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3982 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST ';'  */
#line 778 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3988 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT ';'  */
#line 779 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3994 "raku.tab.c"
    break;

  case 117: /* stmt: KW_LAST KW_IF expr ';'  */
#line 781 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4000 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 783 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4006 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 785 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4012 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 787 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4018 "raku.tab.c"
    break;

  case 121: /* stmt: sub_decl  */
#line 788 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4024 "raku.tab.c"
    break;

  case 122: /* stmt: class_decl  */
#line 789 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4030 "raku.tab.c"
    break;

  case 123: /* stmt: role_decl  */
#line 790 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4036 "raku.tab.c"
    break;

  case 124: /* stmt: grammar_decl  */
#line 791 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4042 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF '(' expr ')' block  */
#line 795 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4048 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 797 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4054 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 799 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4060 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block  */
#line 801 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4066 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 803 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4072 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 805 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4078 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF expr block elsif_tail  */
#line 807 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4084 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 809 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4090 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block  */
#line 813 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4096 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 815 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4102 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 817 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4108 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 819 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4114 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 821 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4120 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 823 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4126 "raku.tab.c"
    break;

  case 139: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 827 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4132 "raku.tab.c"
    break;

  case 140: /* while_stmt: KW_WHILE expr block  */
#line 829 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4138 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 833 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4144 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 835 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4150 "raku.tab.c"
    break;

  case 143: /* unless_stmt: KW_UNLESS expr block  */
#line 837 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4156 "raku.tab.c"
    break;

  case 144: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 839 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4162 "raku.tab.c"
    break;

  case 145: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 843 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4168 "raku.tab.c"
    break;

  case 146: /* until_stmt: KW_UNTIL expr block  */
#line 845 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4174 "raku.tab.c"
    break;

  case 147: /* repeat_stmt: KW_REPEAT block  */
#line 849 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4180 "raku.tab.c"
    break;

  case 148: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 851 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4186 "raku.tab.c"
    break;

  case 149: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4192 "raku.tab.c"
    break;

  case 150: /* loop_stmt: KW_LOOP block  */
#line 857 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4198 "raku.tab.c"
    break;

  case 151: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 859 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4204 "raku.tab.c"
    break;

  case 152: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 861 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4210 "raku.tab.c"
    break;

  case 153: /* loop_incr: expr  */
#line 864 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4216 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 868 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4226 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 874 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4236 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 880 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4245 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 885 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4255 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 891 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4264 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR expr block  */
#line 896 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4271 "raku.tab.c"
    break;

  case 160: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 901 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4283 "raku.tab.c"
    break;

  case 161: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 909 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4296 "raku.tab.c"
    break;

  case 162: /* when_list: %empty  */
#line 919 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4302 "raku.tab.c"
    break;

  case 163: /* when_list: when_list KW_WHEN expr block  */
#line 921 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4310 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 927 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4322 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 935 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4332 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT sub_body  */
#line 941 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4342 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 947 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4354 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 955 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4364 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 961 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4374 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 967 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4387 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 976 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4398 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 983 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4411 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 992 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4424 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1001 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4435 "raku.tab.c"
    break;

  case 175: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1010 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4446 "raku.tab.c"
    break;

  case 176: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1017 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4455 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list '}'  */
#line 1023 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4461 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr '}'  */
#line 1025 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4468 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1028 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4475 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1031 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4481 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1033 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4487 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1035 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4493 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1037 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4499 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1039 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4505 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1041 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4511 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1043 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4517 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1045 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4524 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1048 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4530 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1050 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4536 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1052 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4542 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1054 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4549 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1057 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4556 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1060 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4564 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list '}'  */
#line 1065 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4570 "raku.tab.c"
    break;

  case 195: /* method_body: '{' YADA '}'  */
#line 1066 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4576 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr '}'  */
#line 1068 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4583 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1071 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4590 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1074 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4596 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1076 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4602 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1078 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4608 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1080 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4614 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1082 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4620 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1084 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4626 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1086 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4632 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1088 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4639 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1091 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4645 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1093 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4651 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1095 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4657 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1097 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4664 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1100 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4671 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1103 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4679 "raku.tab.c"
    break;

  case 212: /* pkg_name: IDENT  */
#line 1108 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4685 "raku.tab.c"
    break;

  case 213: /* pkg_name: QIDENT  */
#line 1109 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4691 "raku.tab.c"
    break;

  case 214: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1113 "raku.y"
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
#line 4709 "raku.tab.c"
    break;

  case 215: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1129 "raku.y"
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
#line 4726 "raku.tab.c"
    break;

  case 216: /* is_clauses: %empty  */
#line 1143 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4732 "raku.tab.c"
    break;

  case 217: /* is_clauses: is_clauses IDENT IDENT  */
#line 1145 "raku.y"
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
#line 4748 "raku.tab.c"
    break;

  case 218: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1157 "raku.y"
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
#line 4764 "raku.tab.c"
    break;

  case 219: /* class_body_list: %empty  */
#line 1170 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4770 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1172 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4777 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1175 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4784 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1178 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4791 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1181 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4798 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1184 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4805 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1187 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4812 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1190 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4819 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1193 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4826 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1196 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4833 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1199 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4840 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1202 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4847 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1205 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4854 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1208 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4865 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1215 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4876 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1222 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4887 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1229 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4898 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1236 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4906 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1240 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4914 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1244 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4922 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1248 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4930 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1252 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4941 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1259 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4952 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1266 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4963 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1273 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4974 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1280 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4988 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1290 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5000 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1298 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5012 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1306 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5025 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1315 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5036 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1322 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5047 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1329 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5061 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1339 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5073 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1347 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5085 "raku.tab.c"
    break;

  case 253: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1357 "raku.y"
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
#line 5102 "raku.tab.c"
    break;

  case 254: /* grammar_body_list: %empty  */
#line 1371 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5108 "raku.tab.c"
    break;

  case 255: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1373 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5117 "raku.tab.c"
    break;

  case 256: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1378 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5126 "raku.tab.c"
    break;

  case 257: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1383 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5135 "raku.tab.c"
    break;

  case 258: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1390 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5143 "raku.tab.c"
    break;

  case 259: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1394 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5151 "raku.tab.c"
    break;

  case 260: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1398 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5159 "raku.tab.c"
    break;

  case 261: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1402 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5167 "raku.tab.c"
    break;

  case 262: /* pair_list: IDENT OP_FATARROW expr  */
#line 1408 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5173 "raku.tab.c"
    break;

  case 263: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1410 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5179 "raku.tab.c"
    break;

  case 264: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1412 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5185 "raku.tab.c"
    break;

  case 265: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1414 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5191 "raku.tab.c"
    break;

  case 266: /* param_list: VAR_SCALAR  */
#line 1417 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5197 "raku.tab.c"
    break;

  case 267: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1418 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5203 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1419 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5209 "raku.tab.c"
    break;

  case 269: /* param_list: VAR_ARRAY  */
#line 1420 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5215 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_ARRAY  */
#line 1421 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5221 "raku.tab.c"
    break;

  case 271: /* param_list: IDENT VAR_SCALAR  */
#line 1422 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5227 "raku.tab.c"
    break;

  case 272: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1423 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5233 "raku.tab.c"
    break;

  case 273: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1424 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5239 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR  */
#line 1425 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5245 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1426 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5251 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1427 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5257 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1428 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5263 "raku.tab.c"
    break;

  case 278: /* param_list: VAR_SCALAR '=' expr  */
#line 1429 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5269 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1430 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5275 "raku.tab.c"
    break;

  case 280: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1431 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5281 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1432 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5287 "raku.tab.c"
    break;

  case 282: /* param_list: SLURPY_POS  */
#line 1433 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5293 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' SLURPY_POS  */
#line 1434 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5299 "raku.tab.c"
    break;

  case 284: /* param_list: SLURPY_LOL  */
#line 1435 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5305 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' SLURPY_LOL  */
#line 1436 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5311 "raku.tab.c"
    break;

  case 286: /* param_list: SLURPY_NAMED  */
#line 1437 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5317 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1438 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5323 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list '}'  */
#line 1441 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5329 "raku.tab.c"
    break;

  case 289: /* block: '{' YADA '}'  */
#line 1442 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5335 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr '}'  */
#line 1444 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5341 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1446 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5347 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1448 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5353 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1450 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5359 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1452 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5365 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1454 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5372 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1457 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5378 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1459 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5384 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1461 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5390 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1463 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5396 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1465 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5403 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1468 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5410 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1471 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5418 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1475 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5425 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1478 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5432 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1481 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5439 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1484 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5445 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1486 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5453 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1490 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5460 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1493 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5467 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1496 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5474 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1499 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5481 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1502 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5488 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1505 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5494 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1507 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5500 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1509 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5506 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_LAST '}'  */
#line 1511 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5512 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1513 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5518 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1515 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5524 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1517 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5530 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1519 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5536 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1521 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5542 "raku.tab.c"
    break;

  case 322: /* closure: '{' expr '}'  */
#line 1524 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5548 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR '=' expr  */
#line 1527 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5554 "raku.tab.c"
    break;

  case 324: /* expr: VAR_ARRAY '=' expr  */
#line 1528 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5560 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1529 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5566 "raku.tab.c"
    break;

  case 326: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1530 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5572 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1531 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5578 "raku.tab.c"
    break;

  case 328: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1532 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5584 "raku.tab.c"
    break;

  case 329: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1533 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5590 "raku.tab.c"
    break;

  case 330: /* expr: KW_GATHER block  */
#line 1534 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5600 "raku.tab.c"
    break;

  case 331: /* expr: KW_GATHER for_stmt  */
#line 1539 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5610 "raku.tab.c"
    break;

  case 332: /* expr: tern_expr OP_FATARROW expr  */
#line 1545 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5616 "raku.tab.c"
    break;

  case 333: /* expr: tern_expr  */
#line 1546 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5622 "raku.tab.c"
    break;

  case 334: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1550 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5628 "raku.tab.c"
    break;

  case 335: /* tern_expr: or_expr  */
#line 1551 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5634 "raku.tab.c"
    break;

  case 336: /* or_expr: or_expr OP_OR and_expr  */
#line 1554 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5640 "raku.tab.c"
    break;

  case 337: /* or_expr: and_expr  */
#line 1555 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5646 "raku.tab.c"
    break;

  case 338: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1558 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5652 "raku.tab.c"
    break;

  case 339: /* and_expr: cmp_expr  */
#line 1559 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5658 "raku.tab.c"
    break;

  case 340: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1562 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5664 "raku.tab.c"
    break;

  case 341: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1563 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5670 "raku.tab.c"
    break;

  case 342: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1564 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5676 "raku.tab.c"
    break;

  case 343: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1565 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5682 "raku.tab.c"
    break;

  case 344: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1566 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5688 "raku.tab.c"
    break;

  case 345: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1567 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5694 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1568 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5700 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1569 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5706 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1570 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5712 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1571 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5718 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1572 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5724 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1573 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5730 "raku.tab.c"
    break;

  case 352: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1575 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5740 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1581 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5750 "raku.tab.c"
    break;

  case 354: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1587 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5760 "raku.tab.c"
    break;

  case 355: /* cmp_expr: divis_expr  */
#line 1592 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5766 "raku.tab.c"
    break;

  case 356: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1595 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5772 "raku.tab.c"
    break;

  case 357: /* divis_expr: jct_expr  */
#line 1596 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5778 "raku.tab.c"
    break;

  case 358: /* jct_expr: jct_expr '|' range_expr  */
#line 1599 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5784 "raku.tab.c"
    break;

  case 359: /* jct_expr: jct_expr '&' range_expr  */
#line 1600 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5790 "raku.tab.c"
    break;

  case 360: /* jct_expr: dor_expr  */
#line 1601 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5796 "raku.tab.c"
    break;

  case 361: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1605 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5802 "raku.tab.c"
    break;

  case 362: /* dor_expr: range_expr  */
#line 1606 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5808 "raku.tab.c"
    break;

  case 363: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1609 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5814 "raku.tab.c"
    break;

  case 364: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1610 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5820 "raku.tab.c"
    break;

  case 365: /* range_expr: add_expr  */
#line 1611 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5826 "raku.tab.c"
    break;

  case 366: /* add_expr: add_expr '~' repl_expr  */
#line 1614 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5832 "raku.tab.c"
    break;

  case 367: /* add_expr: repl_expr  */
#line 1615 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5838 "raku.tab.c"
    break;

  case 368: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1618 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5844 "raku.tab.c"
    break;

  case 369: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1619 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5850 "raku.tab.c"
    break;

  case 370: /* repl_expr: addsub_expr  */
#line 1620 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5856 "raku.tab.c"
    break;

  case 371: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1623 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5862 "raku.tab.c"
    break;

  case 372: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1624 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5868 "raku.tab.c"
    break;

  case 373: /* addsub_expr: mul_expr  */
#line 1625 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5874 "raku.tab.c"
    break;

  case 374: /* mul_expr: mul_expr '*' unary_expr  */
#line 1628 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5880 "raku.tab.c"
    break;

  case 375: /* mul_expr: mul_expr '/' unary_expr  */
#line 1629 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5886 "raku.tab.c"
    break;

  case 376: /* mul_expr: mul_expr '%' unary_expr  */
#line 1630 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5892 "raku.tab.c"
    break;

  case 377: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1631 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5898 "raku.tab.c"
    break;

  case 378: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1633 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5904 "raku.tab.c"
    break;

  case 379: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1635 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5910 "raku.tab.c"
    break;

  case 380: /* mul_expr: unary_expr  */
#line 1636 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5916 "raku.tab.c"
    break;

  case 381: /* unary_expr: '-' unary_expr  */
#line 1639 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5922 "raku.tab.c"
    break;

  case 382: /* unary_expr: '+' unary_expr  */
#line 1640 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 5928 "raku.tab.c"
    break;

  case 383: /* unary_expr: '!' unary_expr  */
#line 1641 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5934 "raku.tab.c"
    break;

  case 384: /* unary_expr: CARET unary_expr  */
#line 1642 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5940 "raku.tab.c"
    break;

  case 385: /* unary_expr: OP_REDUCE unary_expr  */
#line 1644 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5949 "raku.tab.c"
    break;

  case 386: /* unary_expr: pow_expr  */
#line 1648 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5955 "raku.tab.c"
    break;

  case 387: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1651 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5961 "raku.tab.c"
    break;

  case 388: /* pow_expr: postfix_expr  */
#line 1652 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5967 "raku.tab.c"
    break;

  case 389: /* scalar_list: VAR_SCALAR  */
#line 1655 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5973 "raku.tab.c"
    break;

  case 390: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1656 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5979 "raku.tab.c"
    break;

  case 391: /* meth_name: IDENT  */
#line 1659 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5985 "raku.tab.c"
    break;

  case 392: /* meth_name: KW_SORT  */
#line 1660 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5991 "raku.tab.c"
    break;

  case 393: /* meth_name: KW_REVERSE  */
#line 1661 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 5997 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_MAP  */
#line 1662 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6003 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_GREP  */
#line 1663 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6009 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_SAY  */
#line 1664 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6015 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_PRINT  */
#line 1665 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6021 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_TAKE  */
#line 1666 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6027 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_RETURN  */
#line 1667 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6033 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_EXISTS  */
#line 1668 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6039 "raku.tab.c"
    break;

  case 401: /* meth_name: KW_DELETE  */
#line 1669 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6045 "raku.tab.c"
    break;

  case 402: /* meth_name: KW_JOIN  */
#line 1670 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6051 "raku.tab.c"
    break;

  case 403: /* meth_name: TESTOP  */
#line 1671 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6057 "raku.tab.c"
    break;

  case 404: /* postfix_expr: call_expr  */
#line 1673 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6063 "raku.tab.c"
    break;

  case 405: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1676 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6073 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '(' arg_list ')'  */
#line 1682 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6082 "raku.tab.c"
    break;

  case 407: /* call_expr: IDENT '(' ')'  */
#line 1686 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6088 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1688 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6094 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1690 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6100 "raku.tab.c"
    break;

  case 410: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1692 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6109 "raku.tab.c"
    break;

  case 411: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1697 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6115 "raku.tab.c"
    break;

  case 412: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1699 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6125 "raku.tab.c"
    break;

  case 413: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1705 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6135 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1711 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6143 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' KW_NEW  */
#line 1715 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6151 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '.' IDENT  */
#line 1719 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6160 "raku.tab.c"
    break;

  case 417: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1724 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6171 "raku.tab.c"
    break;

  case 418: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1731 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6180 "raku.tab.c"
    break;

  case 419: /* call_expr: IDENT '.' CARET IDENT  */
#line 1736 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6190 "raku.tab.c"
    break;

  case 420: /* call_expr: atom '.' CARET IDENT  */
#line 1742 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6200 "raku.tab.c"
    break;

  case 421: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1748 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6211 "raku.tab.c"
    break;

  case 422: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1755 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6222 "raku.tab.c"
    break;

  case 423: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1762 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6231 "raku.tab.c"
    break;

  case 424: /* call_expr: atom '.' meth_name  */
#line 1767 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6240 "raku.tab.c"
    break;

  case 425: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1772 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6251 "raku.tab.c"
    break;

  case 426: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1779 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6260 "raku.tab.c"
    break;

  case 427: /* call_expr: call_expr '.' meth_name  */
#line 1784 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6269 "raku.tab.c"
    break;

  case 428: /* call_expr: KW_DIE expr  */
#line 1789 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6275 "raku.tab.c"
    break;

  case 429: /* call_expr: KW_MAP closure expr  */
#line 1791 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6281 "raku.tab.c"
    break;

  case 430: /* call_expr: KW_MAP closure ',' expr  */
#line 1793 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6287 "raku.tab.c"
    break;

  case 431: /* call_expr: KW_GREP closure expr  */
#line 1795 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6293 "raku.tab.c"
    break;

  case 432: /* call_expr: KW_SORT expr  */
#line 1797 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6299 "raku.tab.c"
    break;

  case 433: /* call_expr: KW_SORT closure expr  */
#line 1799 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6305 "raku.tab.c"
    break;

  case 434: /* call_expr: KW_REVERSE expr  */
#line 1801 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6311 "raku.tab.c"
    break;

  case 435: /* call_expr: atom  */
#line 1802 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6317 "raku.tab.c"
    break;

  case 436: /* arg_list: expr  */
#line 1805 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6323 "raku.tab.c"
    break;

  case 437: /* arg_list: arg_list ',' expr  */
#line 1806 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6329 "raku.tab.c"
    break;

  case 438: /* arg_list: arg_list ','  */
#line 1807 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6335 "raku.tab.c"
    break;

  case 439: /* atom: LIT_INT  */
#line 1810 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6341 "raku.tab.c"
    break;

  case 440: /* atom: LIT_FLOAT  */
#line 1811 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6347 "raku.tab.c"
    break;

  case 441: /* atom: LIT_STR  */
#line 1812 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6353 "raku.tab.c"
    break;

  case 442: /* atom: WORDLIST  */
#line 1814 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6366 "raku.tab.c"
    break;

  case 443: /* atom: LIT_INTERP_STR  */
#line 1822 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6372 "raku.tab.c"
    break;

  case 444: /* atom: VAR_SCALAR  */
#line 1823 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6378 "raku.tab.c"
    break;

  case 445: /* atom: OP_INC VAR_SCALAR  */
#line 1824 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6384 "raku.tab.c"
    break;

  case 446: /* atom: OP_DEC VAR_SCALAR  */
#line 1825 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6390 "raku.tab.c"
    break;

  case 447: /* atom: VAR_SCALAR OP_INC  */
#line 1826 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6396 "raku.tab.c"
    break;

  case 448: /* atom: VAR_SCALAR OP_DEC  */
#line 1827 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6402 "raku.tab.c"
    break;

  case 449: /* atom: VAR_ARRAY  */
#line 1828 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6408 "raku.tab.c"
    break;

  case 450: /* atom: VAR_HASH  */
#line 1829 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6414 "raku.tab.c"
    break;

  case 451: /* atom: VAR_CAPTURE  */
#line 1831 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6422 "raku.tab.c"
    break;

  case 452: /* atom: VAR_FH  */
#line 1835 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6430 "raku.tab.c"
    break;

  case 453: /* atom: VAR_NAMED_CAPTURE  */
#line 1839 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6437 "raku.tab.c"
    break;

  case 454: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1842 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6443 "raku.tab.c"
    break;

  case 455: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1844 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6449 "raku.tab.c"
    break;

  case 456: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1846 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6455 "raku.tab.c"
    break;

  case 457: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1848 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6461 "raku.tab.c"
    break;

  case 458: /* atom: ARR_ALL_SLICE  */
#line 1850 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6467 "raku.tab.c"
    break;

  case 459: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1852 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6473 "raku.tab.c"
    break;

  case 460: /* atom: VAR_HASH '{' expr '}'  */
#line 1854 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6479 "raku.tab.c"
    break;

  case 461: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1856 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6485 "raku.tab.c"
    break;

  case 462: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1858 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6491 "raku.tab.c"
    break;

  case 463: /* atom: IDENT  */
#line 1859 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6497 "raku.tab.c"
    break;

  case 464: /* atom: VAR_TWIGIL  */
#line 1861 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6505 "raku.tab.c"
    break;

  case 465: /* atom: VAR_ARRAY_TWIGIL  */
#line 1865 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6513 "raku.tab.c"
    break;

  case 466: /* atom: VAR_HASH_TWIGIL  */
#line 1869 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6521 "raku.tab.c"
    break;

  case 467: /* atom: '[' ']'  */
#line 1872 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6527 "raku.tab.c"
    break;

  case 468: /* atom: '[' expr ']'  */
#line 1874 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6533 "raku.tab.c"
    break;

  case 469: /* atom: '[' expr ',' ']'  */
#line 1876 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6539 "raku.tab.c"
    break;

  case 470: /* atom: '[' expr ',' arg_list ']'  */
#line 1878 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6546 "raku.tab.c"
    break;

  case 471: /* atom: DOLLAR_LBRACKET ']'  */
#line 1880 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6552 "raku.tab.c"
    break;

  case 472: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1882 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6558 "raku.tab.c"
    break;

  case 473: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1884 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6564 "raku.tab.c"
    break;

  case 474: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1886 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6571 "raku.tab.c"
    break;

  case 475: /* atom: '(' ')'  */
#line 1888 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6577 "raku.tab.c"
    break;

  case 476: /* atom: '(' expr ')'  */
#line 1889 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6583 "raku.tab.c"
    break;

  case 477: /* atom: '(' expr ',' ')'  */
#line 1891 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6589 "raku.tab.c"
    break;

  case 478: /* atom: '(' expr ',' arg_list ')'  */
#line 1893 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6596 "raku.tab.c"
    break;

  case 479: /* atom: block  */
#line 1895 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6602 "raku.tab.c"
    break;

  case 480: /* atom: KW_SUB block  */
#line 1896 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6608 "raku.tab.c"
    break;


#line 6612 "raku.tab.c"

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

#line 1898 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
