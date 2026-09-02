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

#line 470 "raku.tab.c"

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
  YYSYMBOL_cmp_expr = 175,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 176,               /* divis_expr  */
  YYSYMBOL_jct_expr = 177,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 178,                 /* dor_expr  */
  YYSYMBOL_range_expr = 179,               /* range_expr  */
  YYSYMBOL_add_expr = 180,                 /* add_expr  */
  YYSYMBOL_repl_expr = 181,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 182,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 183,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 184,               /* unary_expr  */
  YYSYMBOL_pow_expr = 185,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 186,              /* scalar_list  */
  YYSYMBOL_meth_name = 187,                /* meth_name  */
  YYSYMBOL_postfix_expr = 188,             /* postfix_expr  */
  YYSYMBOL_call_expr = 189,                /* call_expr  */
  YYSYMBOL_arg_list = 190,                 /* arg_list  */
  YYSYMBOL_atom = 191                      /* atom  */
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
#define YYLAST   5367

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  142
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  478
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1197

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
       0,   489,   489,   500,   501,   504,   506,   508,   510,   514,
     516,   518,   520,   522,   526,   530,   532,   536,   540,   542,
     544,   546,   548,   550,   554,   558,   560,   562,   564,   566,
     568,   570,   572,   574,   576,   578,   580,   582,   590,   592,
     594,   596,   598,   600,   602,   605,   608,   610,   613,   616,
     618,   622,   624,   626,   629,   632,   634,   637,   640,   643,
     645,   650,   654,   658,   663,   667,   668,   670,   672,   674,
     676,   679,   681,   683,   685,   688,   691,   694,   697,   700,
     702,   704,   706,   708,   711,   713,   715,   717,   720,   723,
     727,   730,   732,   734,   736,   739,   742,   745,   748,   751,
     754,   757,   758,   759,   760,   761,   762,   763,   765,   767,
     769,   771,   772,   773,   774,   775,   776,   777,   779,   781,
     783,   785,   786,   787,   788,   791,   793,   795,   797,   799,
     801,   803,   805,   809,   811,   813,   815,   817,   819,   823,
     825,   829,   831,   833,   835,   839,   841,   845,   847,   849,
     853,   855,   857,   861,   864,   870,   876,   881,   887,   892,
     897,   905,   916,   917,   923,   931,   937,   943,   951,   957,
     963,   972,   979,   988,   997,  1006,  1013,  1020,  1021,  1024,
    1027,  1029,  1031,  1033,  1035,  1037,  1039,  1041,  1044,  1046,
    1048,  1050,  1053,  1056,  1062,  1063,  1064,  1067,  1070,  1072,
    1074,  1076,  1078,  1080,  1082,  1084,  1087,  1089,  1091,  1093,
    1096,  1099,  1105,  1106,  1109,  1125,  1140,  1141,  1153,  1167,
    1168,  1171,  1174,  1177,  1180,  1183,  1186,  1189,  1192,  1195,
    1198,  1201,  1204,  1211,  1218,  1225,  1232,  1236,  1240,  1244,
    1248,  1255,  1262,  1269,  1276,  1286,  1294,  1302,  1311,  1318,
    1325,  1335,  1343,  1353,  1368,  1369,  1374,  1379,  1386,  1390,
    1394,  1398,  1404,  1406,  1408,  1410,  1414,  1415,  1416,  1417,
    1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,
    1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1438,  1439,
    1440,  1442,  1444,  1446,  1448,  1450,  1453,  1455,  1457,  1459,
    1461,  1464,  1467,  1471,  1474,  1477,  1480,  1482,  1486,  1489,
    1492,  1495,  1498,  1501,  1503,  1505,  1507,  1509,  1511,  1513,
    1515,  1517,  1521,  1524,  1525,  1526,  1527,  1528,  1529,  1530,
    1531,  1536,  1541,  1543,  1546,  1548,  1551,  1552,  1553,  1554,
    1555,  1556,  1557,  1558,  1559,  1560,  1561,  1562,  1563,  1564,
    1565,  1571,  1577,  1583,  1586,  1587,  1590,  1591,  1592,  1595,
    1597,  1600,  1601,  1602,  1605,  1606,  1609,  1610,  1611,  1614,
    1615,  1616,  1619,  1620,  1621,  1622,  1623,  1625,  1627,  1630,
    1631,  1632,  1633,  1634,  1639,  1642,  1643,  1646,  1647,  1650,
    1651,  1652,  1653,  1654,  1655,  1656,  1657,  1658,  1659,  1660,
    1661,  1662,  1664,  1666,  1672,  1677,  1678,  1680,  1682,  1687,
    1689,  1695,  1701,  1705,  1709,  1714,  1721,  1726,  1732,  1738,
    1745,  1752,  1757,  1762,  1769,  1774,  1779,  1781,  1783,  1785,
    1787,  1789,  1791,  1793,  1796,  1797,  1798,  1801,  1802,  1803,
    1804,  1813,  1814,  1815,  1816,  1817,  1818,  1819,  1820,  1821,
    1825,  1829,  1832,  1834,  1836,  1838,  1840,  1842,  1844,  1846,
    1848,  1850,  1851,  1855,  1859,  1863,  1864,  1866,  1868,  1871,
    1872,  1874,  1876,  1879,  1880,  1881,  1883,  1886,  1887
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
  "block", "closure", "expr", "tern_expr", "cmp_expr", "divis_expr",
  "jct_expr", "dor_expr", "range_expr", "add_expr", "repl_expr",
  "addsub_expr", "mul_expr", "unary_expr", "pow_expr", "scalar_list",
  "meth_name", "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-879)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-478)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -879,    18,  2295,  -879,  -879,  -879,  -879,  -879,   673,   -43,
     153,     2,    -1,  -879,  -879,  5178,  2391,  -879,  -879,  -879,
     604,  2466,    60,  4203,  4278,  4353,  4428,  4503,    25,   -20,
    4503,  2541,  2616,    41,    83,  4503,  4503,   191,   221,  4578,
    4653,   115,   109,    58,   262,   134,   134,  4728,  4503,   115,
     115,  4503,   658,   658,   166,  5178,  -879,  -879,   658,   338,
     364,  5178,  5178,  5178,  -879,  2691,  2767,   320,  -879,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,   538,  -879,
    -879,  -879,  5235,   620,   347,   501,   481,   588,   302,  -879,
     151,   656,   663,   591,  -879,  -879,   303,   315,   345,   443,
    -879,  -879,  4503,  4503,  4503,  4503,  4503,  4503,   471,  2842,
    4803,  2918,   511,  4503,  4503,   396,   145,  1122,   -73,   407,
     206,  -879,   188,   115,  2993,  -879,  -879,   589,    34,  -879,
     666,  3069,  3144,  -879,  3219,  -879,   353,     4,   209,   323,
     583,   547,   495,   628,  2993,   523,  2993,   158,  2993,   115,
    2993,   115,   -19,   265,   283,  -879,  -879,  -879,   426,  -879,
     295,  -879,   325,   534,   539,   587,   531,   590,   233,   266,
    2993,   115,  2993,   115,   355,  3295,  -879,  4503,  4503,  -879,
    4503,  4503,  -879,  4503,  3370,  4503,  3445,  4503,  -879,  -879,
     627,  -879,  -879,  -879,  -879,  -879,   613,   609,     5,  -879,
     619,  -879,  -879,  -879,  -879,  -879,   807,  -879,   712,   727,
    -879,   670,   537,  1635,  4503,  4503,  4503,  4503,  4503,  4503,
    4503,  4503,  -879,  4503,  4503,  4503,  4503,  4503,  4503,  4503,
    4503,  -879,  4503,  5178,  5178,  5178,  5178,  5178,  5178,  5178,
    5178,  5178,  5178,  5178,  5178,  5178,  5178,  5178,   886,  5178,
    5178,  5178,  5178,  5178,  5178,  5178,  5178,  5178,  5178,  5178,
    5178,  5178,  5178,  5178,  5178,  5178,  5178,   819,   795,   750,
     594,   635,   652,   683,   695,   701,    74,  -879,   767,  2993,
     445,   780,   676,   600,   698,   737,  -879,   626,   866,   785,
     124,  -879,   932,   804,   813,  2918,   936,  4503,  4503,  4503,
    4503,  4503,  4503,  4503,  4503,  3521,  -879,  -879,   791,  -879,
     821,   827,   712,   817,  -879,  4503,  4503,  -879,  4878,  -879,
    4953,  -879,   326,   327,   330,   406,   839,   854,  -879,   826,
     828,  4503,  4503,  4503,  4503,  4503,  4503,  4503,  -879,  4503,
     830,  4503,  4503,  -879,  4503,   832,   940,   836,  -879,   967,
    4503,  -879,  5178,  5178,     6,  -879,  -879,  -879,  4503,  4503,
    4503,  -879,  4503,  4503,  -879,  4503,  4503,   903,  4503,  -879,
     970,  4503,   976,  4503,   840,   899,   846,  -879,  4503,  4503,
     984,   939,   956,   957,   960,   961,   876,  4503,  -879,  -879,
    -879,   115,    12,  -879,    42,   871,   913,  -879,  -879,  3596,
     977,  1070,  3672,  -879,  -879,   825,   287,   293,   978,  4203,
    4278,  4503,  2007,    14,    32,  -879,   371,   962,   965,   966,
     969,   971,   972,   973,   975,   982,   983,   985,   986,   987,
     988,   989,   990,  -879,   994,   994,   994,   994,   994,   994,
    1000,   994,   994,   994,   994,   994,   994,   994,   994,  -879,
    -879,  -879,   588,  -879,  -879,  -879,   992,   992,   656,   663,
     663,   591,   591,  -879,  -879,  -879,  -879,  -879,  -879,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,
    -879,  -879,  -879,   995,  1089,   996,  -879,  3747,  -879,  -879,
    -879,  -879,  -879,  -879,  4503,  3823,  -879,   851,  -879,  4503,
    4503,  4503,  4503,   991,  1005,  1012,  -879,  3899,  -879,  1279,
    4503,  1003,  -879,    -7,  -879,  1496,   688,  1023,  1014,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,  -879,   763,  -879,  -879,
    -879,  1024,  -879,  1025,  2993,   474,  1028,   183,  3975,   580,
    1026,  4503,  -879,  5028,  -879,  4503,  -879,    55,  -879,  4503,
    4503,  1041,  3596,  1030,  1031,  1036,  1037,  1039,  1040,  1042,
     605,  3596,  1047,  1048,   606,   115,   -23,  5103,  -879,   115,
     115,   185,   -38,   -36,    19,  -879,    48,  -879,  -879,  -879,
    1035,   853,  1773,   720,  1050,  1051,  1052,  1053,  1054,  1055,
    -879,  1057,    49,  1067,  1056,  1068,  1058,   115,   115,   115,
    1061,  1062,  1080,  4503,  -879,  -879,  -879,  -879,  -879,  -879,
    -879,  1186,  1187,  -879,   -22,  1035,   857,    85,   382,   171,
    -879,   861,  4503,  -879,  -879,   784,  1188,  2918,  4503,  4503,
     334,   139,   301,  -879,    62,  4503,  4503,  -879,  4503,  4503,
    -879,  4503,  4503,  4503,  4503,  4503,  4503,  4503,  4503,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,  5178,  4051,  -879,  1355,
    1071,   863,  1072,  -879,   865,  3596,   724,  1073,  1074,   787,
    4503,  4503,  4503,  -879,   868,  -879,   872,   874,  -879,  4503,
    1123,  1196,   877,  -879,  -879,  -879,  -879,  -879,  -879,   879,
    -879,  4503,  4503,  4503,   883,  -879,    54,  -879,  1081,  2993,
     725,  1082,  1035,   885,  1083,  1084,  4503,   887,  -879,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,   889,  -879,  -879,  -879,
     997,  -879,  -879,  2993,   115,  -879,  -879,  1203,  -879,  1208,
    1209,  1206,  4503,  1102,  1212,  1213,  -879,  1035,   618,  4203,
    4278,  2082,  -879,   395,  -879,  -879,  -879,  -879,  -879,  -879,
    -879,  4503,   115,  -879,  -879,  -879,  1091,  1092,  1199,  -879,
    -879,  -879,  -879,  4503,  1096,  -879,  -879,    22,   710,   465,
    1168,  -879,  -879,  1035,  1035,   910,   914,  1217,  1218,  1219,
    -879,  -879,  1101,  -879,   110,   809,  1098,   172,  4503,  4503,
    4503,  4503,  4503,  4503,  -879,  -879,  -879,  -879,   251,   278,
     333,   336,   354,   362,   384,   393,   394,   397,   398,   409,
    -879,  -879,   916,  -879,   918,   920,  -879,  1103,  -879,  -879,
     922,  -879,  -879,  -879,  -879,  1107,  1108,  1110,  -879,  -879,
    -879,  1109,  4503,  1111,  -879,  3596,   731,  -879,  -879,  1113,
    1164,  1167,  -879,   924,  -879,  4503,  -879,  -879,  1035,  -879,
    -879,   736,  1118,  1119,   -23,  -879,   926,  1034,   115,   115,
     115,  -879,  -879,  4503,  -879,  -879,  -879,    30,  -879,   164,
    -879,  -879,  -879,   420,   211,  -879,   261,  4503,  4503,  4503,
    4503,  4503,  4503,  4503,  4503,  -879,   115,  1114,  -879,  -879,
     115,  1120,  4503,  -879,   424,   496,    33,   131,   505,  1125,
    1127,   819,  -879,  -879,  1035,  1035,  1248,  1255,  1257,  -879,
    4503,  4127,  1146,  1147,  -879,   417,   464,   472,   473,   476,
     491,  -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,  -879,  1134,  -879,  -879,
    -879,  -879,  -879,  4503,   930,  -879,  -879,  4503,  4503,  3596,
     745,  -879,  -879,  4503,  -879,  -879,  -879,  -879,   115,   115,
    -879,  -879,  -879,  -879,  -879,  1254,  4503,  1150,  1260,  1261,
    4503,  4503,  4503,  -879,  -879,  -879,   517,   530,   542,   543,
     548,   552,   569,   577,  -879,  -879,  -879,  4503,  1137,  -879,
     125,  1195,  -879,   190,  -879,  1262,  1263,  1141,  4503,  -879,
    1264,  1266,  1148,  4503,  -879,   136,   264,  1153,  1154,  -879,
    -879,   515,  -879,  -879,  -879,  -879,  -879,   592,  1162,   933,
    4503,  4503,  -879,  -879,  -879,  -879,  -879,  -879,  -879,  1159,
    1155,  -879,  -879,   935,  -879,   746,  1045,  -879,  -879,  -879,
    4503,  -879,  -879,   593,   595,   611,  -879,  -879,  -879,  -879,
    -879,  -879,  -879,  -879,  1170,   115,  1165,   941,  1166,  1911,
    1165,   943,  1172,  1176,  -879,  1177,  1178,  1181,  -879,  1182,
    1302,  1303,  1185,  4503,  -879,  1306,  1307,  1189,  4503,  -879,
    -879,  -879,   235,  -879,  -879,  -879,  1183,   621,   622,  -879,
    -879,  1191,  -879,   115,  -879,  -879,  -879,  -879,  -879,  4503,
    -879,  -879,  1165,  -879,  4203,  4278,  2157,  -879,   452,  -879,
    1165,  -879,  -879,  -879,  -879,  -879,  -879,  1193,  1194,  -879,
    1197,  1198,  1204,  -879,  1205,  1165,   947,  -879,  -879,  -879,
    -879,  -879,  1207,  -879,   475,   219,  -879,   298,  4503,  4503,
    4503,  4503,  4503,  4503,  4503,  4503,  -879,  -879,  -879,  -879,
    -879,  -879,  -879,  -879,  -879,  1165,   115,  4503,  4503,  4503,
    -879,  -879,  -879,   630,   636,   651,   653,   654,   655,   677,
     687,  -879,  -879,   696,   699,   704,  -879,  -879,  -879,  -879,
    -879,  -879,  -879,  -879,  -879,  -879,  -879
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   437,   438,   439,   441,   442,   447,
     448,   462,   461,   463,   464,     0,     0,   449,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   456,   440,     0,     0,
       0,     0,     0,     0,   102,     0,     0,     3,     4,   103,
     104,   111,   112,   113,   114,   105,   106,   121,     0,   122,
     123,   124,   110,     0,   333,   335,   353,   355,   358,   360,
     363,   365,   368,   371,   378,   384,   386,   402,   433,     0,
     445,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,   447,
     448,   462,   461,     0,     0,   477,   382,   442,   447,   469,
       0,     0,     0,    38,     0,   434,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,   478,   331,   330,     0,    52,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,     0,   150,     0,     0,   115,
       0,     0,   116,     0,     0,     0,     3,     0,   430,   432,
     107,   109,   426,   212,   213,   216,     0,     0,     0,   383,
       0,   443,   444,   381,   380,   379,   442,   473,     0,     0,
     465,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   326,   327,   328,   329,   323,     0,   409,     0,     0,
     324,     0,     0,     0,     0,     0,    42,   414,     0,   413,
     461,   405,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   470,    29,     0,    31,
       0,   473,     0,     0,    41,   436,     0,     6,     0,    10,
       0,    11,     0,     0,     0,     0,     0,     0,   387,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,    46,     0,     0,   128,     0,   140,     0,
       0,   159,     0,     0,     0,     3,   166,    49,     0,     0,
       0,    51,     0,     0,    55,     0,     0,     0,     0,   162,
       0,     0,     0,     0,     0,   143,     0,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   427,   429,
     431,     0,     0,   219,     0,     0,     0,   254,   474,     0,
       0,     0,     0,   466,   289,   442,   447,   448,   462,     0,
       0,     0,     0,     0,     0,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,   338,   339,   342,   343,   336,   337,
       0,   340,   341,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   354,   356,   357,   359,   361,   362,   364,   366,
     367,   369,   370,   375,   376,   377,   372,   373,   374,   385,
     389,   401,   394,   395,   396,   397,   400,   398,   399,   392,
     393,   390,   391,   425,     0,   422,    62,     0,    96,    97,
      98,    99,   100,    59,     0,     0,   408,     0,    15,     0,
       0,     0,     0,   452,   457,   458,    64,     0,   417,     0,
       0,     0,   406,     0,   404,   436,     0,     0,     0,   325,
     326,   327,   328,   329,   323,   324,   471,     0,    30,    32,
      40,     0,   435,     0,     0,     0,   439,   461,     0,     0,
       0,     0,    26,     0,    27,     0,    28,     0,   169,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,     0,     0,   131,   474,
       0,     0,   361,   362,   266,   269,     0,   282,   284,   286,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,   403,     0,     0,     0,     0,     0,   474,     0,   474,
       0,     0,     0,     0,   117,   118,   119,   120,   322,   428,
     108,     0,     0,   219,     0,     0,     0,     0,     0,     0,
     475,     0,     0,   388,   467,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,   316,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,   290,
      66,    68,    70,    73,    71,    72,    67,    69,    79,    81,
      83,    86,    84,    85,    80,    82,     0,     0,   418,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   416,     0,   412,     0,     0,   258,     0,
       0,     0,     0,   452,   457,   458,   472,    39,     5,     0,
      12,     0,     0,     0,     0,    19,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    90,
      89,    93,    91,    92,    88,    44,     0,    94,    95,    47,
     125,   130,   129,     0,     0,   139,   156,     0,   158,     0,
       0,     0,     0,   271,     0,     0,   165,     0,     0,     0,
       0,     0,   177,     0,    50,    53,    54,    57,    58,    34,
      33,     0,     0,   160,   459,   460,     0,     0,   141,   144,
     145,   148,   149,     0,     0,   217,   218,     0,     0,     0,
       0,   215,   174,     0,     0,     0,     0,     0,     0,     0,
     253,   476,     0,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   299,   306,   313,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     334,   424,     0,   421,     0,     0,    61,     0,    63,   175,
       0,    16,   455,   454,   453,     0,     0,     0,   415,   410,
     411,     0,     0,     0,   407,     0,     0,   263,   262,     0,
       0,     0,    21,     0,    22,     0,    25,   168,     0,    36,
      35,     0,   476,   476,     0,   132,     0,   133,     0,     0,
       0,   267,   278,     0,   272,   273,   164,   274,   270,     0,
     283,   285,   287,     0,     0,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,     0,     0,    77,    78,
       0,     0,     0,   214,     0,     0,     0,     0,     0,     0,
       0,     0,   173,   171,     0,     0,     0,     0,     0,     9,
       0,     0,   452,   458,   310,     0,     0,     0,     0,     0,
       0,   318,   319,   320,   321,   291,   293,   295,   298,   296,
     297,   292,   294,   423,   420,   419,    60,   476,    74,    75,
      76,   259,   260,     0,     0,    13,    20,     0,     0,     0,
       0,   167,     7,     0,    45,    48,   127,   126,   474,     0,
     137,   157,   154,   155,   280,     0,     0,   275,     0,     0,
       0,     0,     0,   181,   182,   179,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   161,   142,     0,     0,   153,
       0,     3,   249,     0,   246,     0,     0,     0,     0,   225,
       0,     0,     0,     0,   220,     0,     0,     0,     0,   221,
     222,     0,   170,   172,   255,   256,   257,     0,   176,     0,
       0,     0,   300,   302,   305,   303,   304,   301,    17,     0,
     476,   265,   264,     0,    23,     0,   134,   135,   268,   279,
       0,   276,   277,     0,     0,     0,   183,   185,   187,   190,
     188,   189,   184,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,     0,     0,     0,   237,     0,
       0,     0,     0,     0,   227,     0,     0,     0,     0,   226,
     223,   224,     0,   252,   309,   308,   175,     0,     0,   261,
      14,   476,     8,     0,   138,   281,   191,   193,   192,     0,
     152,   248,     0,   195,     0,     0,     0,   194,     0,   245,
       0,   234,   235,   229,   232,   233,   228,     0,     0,   238,
       0,     0,     0,   239,     0,     0,     0,   307,   311,   312,
      24,   136,     0,   247,     0,     0,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   196,   244,   242,   243,
     231,   240,   241,   230,   251,     0,     0,     0,     0,     0,
     199,   200,   197,     0,     0,     0,     0,     0,     0,     0,
       0,   250,   151,     0,     0,     0,   201,   203,   205,   208,
     206,   207,   202,   204,   209,   211,   210
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -879,  -879,     1,  -879,  -560,  -727,  -879,  -879,  -879,  -879,
    -879,   220,  1299,  -879,  -879,  -879,  -879,  -312,  -878,   212,
    -879,  -879,  -879,   719,  -879,  -879,  -500,   802,  -386,   339,
    1038,    -2,  -237,  -879,   797,  1094,  -879,   646,   -16,  1086,
     831,   833,    20,  -879,  1201,  -264,  -879,  -879,   316,  -879
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   213,    68,    69,   568,    70,    71,    72,    73,
      74,   998,    75,    76,   592,    77,    78,   356,  1002,   195,
      79,    80,   392,   614,    81,   619,   293,   539,   581,   125,
     184,   135,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,   209,   483,    96,    97,   136,    98
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      83,     2,   440,   865,   485,    25,   731,   690,   616,   686,
     115,   153,    27,   548,   130,   692,   574,   575,     3,   395,
     576,   145,   147,   149,   151,   152,   611,  1004,   158,   160,
     162,   100,   101,   166,   167,   126,   612,   171,   173,   154,
     778,   779,   635,   741,   780,   188,   189,  1005,   739,   192,
     740,   163,   574,   575,   975,   164,   576,  1006,   743,   850,
     638,   109,   636,   208,   211,   574,   575,   349,   851,   576,
     137,   138,   139,   396,   140,   199,   110,   577,   578,   579,
     639,   203,   204,   205,   778,   779,   177,   255,   780,   255,
     359,   761,   762,   141,   111,   574,   575,   165,   142,   576,
     270,   271,   272,   273,   274,   275,   178,  1007,   280,   282,
     360,   284,   285,   577,   578,   579,    67,   350,   781,    67,
      67,   114,   208,   316,   744,   745,   577,   578,   579,   308,
     310,   116,   312,   117,   691,   574,   575,   317,   742,   576,
     970,   580,   330,  1093,   340,  1010,   345,   179,   347,   976,
    1080,   613,  1008,   304,   637,  1011,   577,   578,   579,   287,
    1081,   713,   903,   288,    67,   182,  1009,   341,   374,   824,
     376,   295,   640,   381,   977,   382,   383,   615,   384,   385,
     197,   386,   388,   389,   386,   390,   341,   342,  1111,   763,
     712,   179,  1119,   494,   143,   361,   577,   578,   579,   198,
     574,   575,   807,   168,   576,  1012,   342,   510,   495,   289,
    1082,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     784,   425,   426,   427,   428,   429,   430,   431,   432,   920,
     433,   785,   786,   169,  1143,   253,   254,   456,   457,   341,
     978,   979,  1157,   175,   921,   574,   575,   341,    67,   576,
    1013,   787,   788,   789,    67,  1083,   116,  1164,   117,   342,
    1066,   577,   578,   579,  1014,   196,   703,   342,   746,  1084,
     200,   737,   343,   183,   112,   344,   255,   497,  1085,   805,
     463,   464,   465,   466,   467,   468,   469,  1181,  1086,   359,
     180,   343,   113,   516,   344,   518,   519,   520,   521,   522,
     523,   524,   525,   782,   966,   506,   577,   578,   579,   360,
     181,   790,   924,   532,   533,   116,   535,   117,   540,  1104,
     116,   315,   117,   359,    67,  1070,   359,   296,   318,   553,
     554,   555,   556,   557,   558,   559,   572,   573,  1087,   562,
     563,    82,   319,   360,   343,   297,   360,   344,   201,   352,
     353,   984,   343,   362,   370,   344,   582,   584,   585,  1171,
     586,   587,   798,   588,   589,   332,   799,   155,   157,   594,
    1135,   596,   371,   363,   202,   800,   600,   601,   801,   802,
     174,   176,   803,  1088,   604,   609,   378,   372,   190,   191,
     255,   931,   574,   575,   361,   182,   576,  1089,   212,   641,
     857,   985,   642,   643,   379,   373,   110,   630,   631,   632,
     634,   605,   644,   252,   112,   645,   646,   354,   932,   647,
     648,   266,   355,   887,   627,   278,   888,   889,   361,   820,
     232,   361,   628,   294,   357,   876,   890,   358,  1172,   891,
     892,   806,   320,   893,   894,   541,   543,   267,   980,   545,
     313,   332,   981,   577,   578,   579,   321,   269,   364,   542,
     544,   334,   155,   546,   335,   336,   606,   338,   982,   607,
     339,   912,   913,   933,   804,   906,   934,   268,   907,   908,
    1148,   909,   910,  1149,  1150,   276,   314,   658,   346,   315,
     348,   351,   672,  1151,   935,   659,  1152,  1153,   677,   678,
    1154,  1155,   936,  1167,   231,   326,   332,  1168,   688,   327,
     375,   649,   377,   532,   905,  1015,   334,   660,  1016,   335,
     336,  1017,  1018,  1169,   937,   283,   661,   662,   231,   286,
     663,   664,   699,   938,   939,   895,   208,   940,   941,   708,
     547,   710,   665,   711,   295,   355,   961,   714,   715,   942,
     718,   331,    82,   338,   332,   333,   339,  1032,  1000,   357,
     983,   325,   358,  1001,   334,   734,   214,   335,   336,   215,
     216,   337,   242,   243,   244,   245,   246,   247,   498,   217,
     753,   499,   218,   219,   248,   231,   220,   221,   233,   234,
     235,   236,  1156,   322,   323,   324,   249,   720,   237,   238,
     239,   774,  1022,  1023,  1033,   721,   722,   700,   338,   723,
     701,   339,  1034,  1035,  1067,  1170,  1036,  1071,   131,   132,
     792,   527,   240,   241,   724,   795,   796,   797,   877,   878,
    1003,  1037,   879,   808,   809,  1001,   810,   811,   328,   812,
     813,   814,   815,   816,   817,   818,   819,  1021,   223,  1092,
     658,   224,   225,   365,  1001,   560,   338,  1056,   366,   339,
     564,   226,   367,   659,   227,   228,   571,   368,   229,   230,
    1057,   222,   193,   194,   583,   660,   661,   404,   835,   836,
     837,   662,  1058,  1059,   591,   663,   391,   841,  1060,   880,
     881,   882,  1061,   100,   101,   298,   299,   300,   301,   302,
     847,   848,   664,   260,   261,   262,  1136,   853,   303,  1062,
     665,   250,   251,   705,   861,   621,   706,  1063,   625,   263,
     264,   265,   504,   109,   470,   828,   718,   488,   720,   369,
     610,   866,  1094,  1106,   471,  1107,   472,   473,   725,   729,
     872,   315,   315,   394,   724,   474,   475,   883,   884,   886,
     476,  1108,   393,   231,   948,   950,   477,   478,   397,   896,
     507,  1138,  1139,   658,   479,   480,   481,   482,   489,   659,
    1186,   901,   256,   257,   904,    99,  1187,   100,   101,   102,
     103,   104,   105,   106,   660,   490,   661,   662,   663,   258,
     259,  1188,   107,  1189,  1190,  1191,   925,   926,   927,   928,
     929,   930,   305,   671,   306,   108,   402,   109,   403,   470,
     664,   674,   502,   484,   503,   676,   491,  1192,   679,   471,
     665,   472,   473,   684,   502,   687,   693,  1193,   492,   718,
     474,   475,   720,   470,   493,   476,  1194,   724,   505,  1195,
     952,   477,   478,   471,  1196,   472,   473,   398,   399,   479,
     480,   481,   482,   754,   474,   475,   315,   831,   854,   476,
     315,   855,   400,   401,   955,   477,   478,   315,   717,   962,
     506,   974,   963,   479,   480,   481,   482,   726,  1044,  1102,
     508,   315,   315,   486,   487,   986,   987,   988,   989,   990,
     991,   992,   993,   449,   450,   451,   453,   454,   455,   315,
     999,   696,   496,   315,   730,   732,   500,   501,   735,   736,
     738,   100,   101,   298,   299,   300,   301,   302,  1027,   509,
     315,    82,   793,   315,   528,   834,   303,    99,   598,   100,
     101,   102,   103,   104,   105,   106,   768,   769,   770,   512,
     513,   109,  -387,  -387,   107,   502,   511,   922,   514,   515,
     517,  1039,   531,   315,   529,  1041,  1042,   626,   549,   109,
     530,   551,   401,   398,   552,   398,   561,   565,   399,   566,
     567,   569,   399,   550,  1049,   597,   399,   570,  1053,  1054,
    1055,   599,   399,   822,   593,   825,   398,   675,   747,   748,
     595,   830,   783,   748,   602,  1064,   791,   315,   827,   315,
     829,   315,  1069,   838,   315,   617,  1075,   839,   513,   840,
     315,  1079,   844,   513,   398,   845,   608,   846,   849,   706,
     858,   748,   862,   315,   863,   315,   864,   567,  1097,  1098,
     434,   435,   436,   437,   438,   439,   590,   441,   442,   443,
     444,   445,   446,   447,   448,   914,   748,   618,  1105,   915,
     748,   943,   315,   944,   513,   945,   315,   947,   315,   398,
     959,   968,   399,   969,   567,  1040,   315,  1118,  1096,   315,
    1101,   315,   603,   867,  1103,   567,  1112,   748,  1120,   748,
     623,  1130,  1165,   748,   185,   187,  1134,   459,   460,   604,
     605,   461,   462,   606,   607,   650,   622,   629,   651,   652,
     666,   897,   653,   668,   654,   655,   656,   999,   657,   249,
     680,   702,  1144,  1145,  1147,   658,   659,   255,   660,   661,
     662,   663,   664,   665,   681,     4,     5,     6,     7,   667,
     669,   682,   127,   128,   120,   121,   290,   689,    13,    14,
      15,    16,    17,    18,    19,   694,  1173,  1174,  1175,  1176,
    1177,  1178,  1179,  1180,   695,   123,    29,   697,   698,   707,
     716,   954,    35,   718,   719,  1183,  1184,  1185,    37,   720,
     721,   960,   722,   723,   355,   724,    45,    46,    47,    48,
     727,   728,    51,   755,   756,   757,   758,   759,   760,   764,
     766,    55,    56,   315,   771,   772,   765,    57,   767,   773,
     775,   776,   794,   967,   826,   828,   842,   971,   972,   973,
     843,   832,   833,   868,   852,   856,   859,   860,   869,   870,
     871,   873,   874,   875,   898,   899,    59,    60,   900,   902,
     911,   916,   917,   918,   919,   994,   946,  1029,   923,   996,
     948,   949,    61,   950,   951,   953,   956,   957,    62,    63,
     958,   964,   965,   997,   995,  1024,   124,   291,  1019,    66,
    1020,    67,  1025,   292,  1026,  1030,  1031,  1038,  1048,  1050,
    1051,  1052,  1065,  1068,  1074,  1043,  1072,  1073,  1076,  1045,
    1077,  1078,     4,     5,     6,     7,  1090,  1091,  1100,   127,
     128,   120,   121,   290,  1099,    13,    14,    15,    16,    17,
      18,    19,  1095,  1109,  1001,  1121,  1113,  1046,  1047,  1122,
    1123,  1124,   123,    29,  1125,  1126,  1127,  1128,  1129,    35,
    1131,  1132,  1133,  1137,  1140,    37,  1158,  1159,   156,  1142,
    1160,  1161,   777,    45,    46,    47,    48,  1162,  1163,    51,
     704,   458,  1166,   452,   329,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   290,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    61,
       0,    37,     0,     0,  1110,    62,    63,     0,    82,    45,
      46,    47,    48,   124,   685,    51,    66,     0,    67,     0,
     292,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,    62,    63,     0,     0,     0,     0,     0,     0,   124,
     823,     0,    66,     0,    67,     0,   292,     0,     0,     4,
       5,     6,     7,     0,     0,  1182,   127,   128,   120,   121,
     290,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   123,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,    62,    63,     0,     0,     0,     0,     0,     0,
     124,     0,     0,    66,     0,    67,     0,   292,     4,     5,
       6,     7,     0,     0,     0,   405,   406,   407,   408,    12,
       0,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,   409,   410,    25,     0,     0,    26,    27,    28,    29,
     411,   412,    32,    33,    34,    35,    36,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,   413,   414,    45,
      46,    47,    48,    49,    50,    51,    52,     0,     0,     0,
      53,    54,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,    62,    63,     0,     0,     0,     0,     0,    64,    65,
       0,     0,    66,     0,    67,   415,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,     0,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,   749,
     750,    25,     0,     0,    26,    27,    28,    29,    30,   751,
      32,    33,    34,    35,    36,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,     0,     0,    53,    54,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,    62,
      63,     0,     0,     0,     0,     0,    64,    65,     0,     0,
      66,     0,    67,   752,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,  1114,  1115,    25,
       0,     0,    26,    27,    28,    29,    30,  1116,    32,    33,
      34,    35,    36,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,     0,     0,    53,    54,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,    59,    60,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,    61,     0,     0,     0,     0,     0,    62,    63,     0,
     123,    29,     0,     0,    64,    65,     0,    35,    66,     0,
      67,  1117,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
     159,   124,    51,     0,    66,     0,    67,   633,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,   159,   124,    51,     0,    66,
       0,    67,   885,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,    62,    63,     0,     0,     0,     0,     0,
     159,   124,     0,     0,    66,     0,    67,  1146,     4,     5,
       6,     7,     0,     0,     0,     8,     9,    10,    11,    12,
       0,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     0,     0,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     0,     0,     0,
      53,    54,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,    59,
      60,   127,   128,   120,   121,   122,     0,    13,    14,    15,
      16,    17,    18,    19,     0,    61,     0,     0,     0,     0,
       0,    62,    63,     0,   123,    29,     0,     0,    64,    65,
       0,    35,    66,     0,    67,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,    45,    46,    47,    48,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   127,   128,   120,   121,
     122,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,   123,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    61,    37,     0,     0,     0,     0,    62,    63,     0,
      45,    46,    47,    48,     0,   124,    51,     0,    66,   129,
      67,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   127,   128,   120,   121,   122,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    61,    37,     0,     0,
       0,     0,    62,    63,     0,    45,    46,    47,    48,   133,
     134,    51,     0,    66,     0,    67,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   127,   128,   120,   121,
     122,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,   123,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    61,    37,     0,     0,     0,     0,    62,    63,     0,
      45,    46,    47,    48,   159,   124,    51,     0,    66,     0,
      67,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   206,   128,   120,   121,   122,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    61,    37,     0,     0,
       0,     0,    62,    63,     0,    45,    46,    47,    48,   161,
     124,    51,     0,    66,     0,    67,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    59,    60,     0,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    61,     0,    37,     0,     0,     0,    62,    63,     0,
       0,    45,    46,    47,    48,   124,   207,    51,    66,     0,
      67,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   124,    51,     0,    66,   210,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    61,     0,    37,     0,     0,     0,    62,    63,
       0,     0,    45,    46,    47,    48,   124,   277,    51,    66,
       0,    67,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,   281,    45,    46,    47,
      48,     0,   124,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    59,    60,     0,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    61,     0,    37,     0,     0,     0,    62,
      63,     0,     0,    45,    46,    47,    48,   124,   207,    51,
      66,     0,    67,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,   307,   124,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,   309,   124,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
     380,     0,     0,    59,    60,     0,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    61,
       0,    37,     0,     0,     0,    62,    63,     0,     0,    45,
      46,    47,    48,   124,   311,    51,    66,     0,    67,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,     0,    59,
      60,     0,     0,   123,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    61,    37,     0,     0,     0,
       0,    62,    63,     0,    45,    46,    47,    48,     0,   124,
      51,     0,    66,     0,    67,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      61,    37,     0,     0,     0,     0,    62,    63,     0,    45,
      46,    47,    48,     0,   124,    51,   387,    66,     0,    67,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,   212,     4,     5,     6,     7,     0,     0,
       0,   127,   128,   120,   121,   122,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    61,     0,    37,     0,     0,
       0,    62,    63,     0,     0,    45,    46,    47,    48,   124,
       0,    51,    66,     0,    67,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   127,   128,   120,   121,
     122,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,   123,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    61,    37,     0,     0,     0,     0,    62,    63,     0,
      45,    46,    47,    48,     0,   124,    51,     0,    66,   526,
      67,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      59,    60,     0,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    61,     0,    37,     0,
       0,     0,    62,    63,     0,     0,    45,    46,    47,    48,
     124,   620,    51,    66,     0,    67,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   124,    51,     0,    66,
     624,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    59,    60,     0,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,    61,     0,    37,
       0,     0,     0,    62,    63,     0,     0,    45,    46,    47,
      48,   124,   670,    51,    66,     0,    67,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    59,    60,     0,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    61,     0,    37,     0,     0,     0,    62,
      63,     0,     0,    45,    46,    47,    48,   124,   673,    51,
      66,     0,    67,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
     536,     7,     0,     0,     0,   127,   128,   120,   121,   537,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    61,
       0,    37,     0,     0,     0,    62,    63,     0,     0,    45,
      46,    47,    48,   124,   683,    51,    66,     0,    67,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   127,   128,   120,   121,   122,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    61,     0,    37,     0,     0,
       0,    62,    63,     0,     0,    45,    46,    47,    48,   124,
     207,    51,    66,     0,    67,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    59,    60,     0,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    61,     0,    37,     0,     0,     0,    62,    63,     0,
       0,    45,    46,    47,    48,   124,   821,    51,    66,     0,
      67,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    59,    60,     0,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,    61,     0,    37,
       0,     0,     0,    62,    63,     0,     0,    45,    46,    47,
      48,   124,  1028,    51,    66,     0,    67,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   144,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,     0,   146,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   148,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,     0,   150,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   124,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,     0,   170,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   172,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,     0,   124,    51,     0,    66,     0,   186,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   279,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,   536,     7,
       0,     0,     0,   127,   128,   120,   121,   537,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,     0,   534,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   538,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,     0,   709,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,     0,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   733,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -477,
    -477,     0,    59,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,    62,    63,     0,     0,     0,     0,
       0,     0,   124,     0,     0,    66,     0,    67,  -477,  -477,
    -477,     0,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,     0,     0,     0,  -477,     0,
       0,     0,     0,     0,     0,     0,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,     0,     0,  -477,  -477,  -477,  -477,
    -477,     0,     0,  -477,  -477,  -477,     0,  -477
};

static const yytype_int16 yycheck[] =
{
       2,     0,   239,   730,   268,    28,   566,    14,   394,   509,
      11,    27,    32,   325,    16,   515,    10,    11,     0,    14,
      14,    23,    24,    25,    26,    27,    14,   905,    30,    31,
      32,   104,   105,    35,    36,    15,    24,    39,    40,    14,
      62,    63,    28,    24,    66,    47,    48,    14,    86,    51,
      86,    10,    10,    11,    24,    14,    14,    24,    10,     5,
      28,   134,    48,    65,    66,    10,    11,    86,    14,    14,
      10,    11,    12,    68,    14,    55,   119,    71,    72,    73,
      48,    61,    62,    63,    62,    63,    28,   125,    66,   125,
      28,    42,    43,    33,   137,    10,    11,    14,    38,    14,
     102,   103,   104,   105,   106,   107,    48,    74,   110,   111,
      48,   113,   114,    71,    72,    73,   139,   136,   140,   139,
     139,   119,   124,   119,    76,    77,    71,    72,    73,   131,
     132,   132,   134,   134,   141,    10,    11,   133,   119,    14,
     867,   135,   144,  1021,   146,    14,   148,   133,   150,   119,
      14,   139,   119,   119,   140,    24,    71,    72,    73,    14,
      24,   547,   140,    18,   139,   133,   133,    28,   170,   669,
     172,   137,   140,   175,    10,   177,   178,   135,   180,   181,
      14,   183,   184,   185,   186,   187,    28,    48,  1066,   140,
     135,   133,  1070,   119,   134,   133,    71,    72,    73,    33,
      10,    11,   140,    12,    14,    74,    48,    83,   134,    64,
      74,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     135,   223,   224,   225,   226,   227,   228,   229,   230,   119,
     232,   617,   618,    12,  1112,    84,    85,   253,   254,    28,
      76,    77,  1120,   134,   134,    10,    11,    28,   139,    14,
     119,    80,    81,    82,   139,   119,   132,  1135,   134,    48,
     135,    71,    72,    73,   133,    53,    83,    48,   580,   133,
      58,    86,   133,   139,   121,   136,   125,   279,    14,   140,
     260,   261,   262,   263,   264,   265,   266,  1165,    24,    28,
      28,   133,   139,   295,   136,   297,   298,   299,   300,   301,
     302,   303,   304,   615,   864,   133,    71,    72,    73,    48,
      48,   140,   140,   315,   316,   132,   318,   134,   320,  1046,
     132,   136,   134,    28,   139,   135,    28,   121,   119,   331,
     332,   333,   334,   335,   336,   337,   352,   353,    74,   341,
     342,     2,   133,    48,   133,   139,    48,   136,    10,    84,
      85,   140,   133,    28,   121,   136,   355,   359,   360,   140,
     362,   363,    28,   365,   366,    31,    32,    28,    29,   371,
     135,   373,   139,    48,    10,    41,   378,   379,    44,    45,
      41,    42,    48,   119,   133,   387,    31,   121,    49,    50,
     125,   140,    10,    11,   133,   133,    14,   133,    78,    28,
     712,   140,    31,    32,    49,   139,   119,   409,   410,   411,
     412,   133,    41,   111,   121,    44,    45,   134,   140,    48,
      49,   118,   139,    28,   137,   109,    31,    32,   133,   666,
      83,   133,   139,   117,   133,   747,    41,   136,   140,    44,
      45,   140,   119,    48,    49,   119,   119,   132,    28,   119,
     134,    31,    32,    71,    72,    73,   133,    14,   133,   133,
     133,    41,   123,   133,    44,    45,   133,   133,    48,   133,
     136,   783,   784,   140,   140,    10,   140,   132,    13,    14,
      28,    16,    17,    31,    32,    14,   133,   133,   149,   136,
     151,   152,   494,    41,   140,   133,    44,    45,   500,   501,
      48,    49,   140,    28,   133,    10,    31,    32,   510,    14,
     171,   140,   173,   515,   778,    10,    41,   133,    13,    44,
      45,    16,    17,    48,   140,    14,   133,   133,   133,   133,
     133,   133,   534,   140,   140,   140,   538,   140,   140,   541,
     134,   543,   133,   545,   137,   139,   858,   549,   550,   140,
     133,    28,   213,   133,    31,    32,   136,   140,   134,   133,
     140,    14,   136,   139,    41,   567,    28,    44,    45,    31,
      32,    48,    91,    92,    93,    94,    95,    96,   133,    41,
     582,   136,    44,    45,   103,   133,    48,    49,    87,    88,
      89,    90,   140,    10,    11,    12,   115,   133,    97,    98,
      99,   603,   914,   915,   140,   133,   133,   133,   133,   133,
     136,   136,   140,   140,  1000,   140,   140,  1003,    14,    15,
     622,   305,   121,   122,   133,   627,   628,   629,    10,    11,
     134,   140,    14,   635,   636,   139,   638,   639,    10,   641,
     642,   643,   644,   645,   646,   647,   648,   911,    28,   134,
     133,    31,    32,   119,   139,   339,   133,   140,   119,   136,
     344,    41,    75,   133,    44,    45,   350,   136,    48,    49,
     140,   133,    14,    15,   358,   133,   133,   140,   680,   681,
     682,   133,   140,   140,   368,   133,    59,   689,   140,    71,
      72,    73,   140,   104,   105,   106,   107,   108,   109,   110,
     702,   703,   133,   112,   113,   114,  1092,   709,   119,   140,
     133,   123,   124,   133,   716,   399,   136,   140,   402,   128,
     129,   130,   122,   134,    14,   133,   133,   133,   133,   139,
     391,   733,   140,   140,    24,   140,    26,    27,   133,   133,
     742,   136,   136,   134,   133,    35,    36,   749,   750,   751,
      40,   140,   139,   133,   133,   133,    46,    47,   139,   761,
     134,   140,   140,   133,    54,    55,    56,    57,   133,   133,
     140,   773,   116,   117,    64,   102,   140,   104,   105,   106,
     107,   108,   109,   110,   133,   133,   133,   133,   133,   126,
     127,   140,   119,   140,   140,   140,   798,   799,   800,   801,
     802,   803,   136,   487,   138,   132,   136,   134,   138,    14,
     133,   495,   136,    18,   138,   499,   133,   140,   502,    24,
     133,    26,    27,   507,   136,   509,   138,   140,   133,   133,
      35,    36,   133,    14,   133,    40,   140,   133,   140,   140,
     842,    46,    47,    24,   140,    26,    27,   135,   136,    54,
      55,    56,    57,   133,    35,    36,   136,   133,   133,    40,
     136,   136,   135,   136,   133,    46,    47,   136,   552,   133,
     133,   873,   136,    54,    55,    56,    57,   561,   133,   133,
      14,   136,   136,   133,   134,   887,   888,   889,   890,   891,
     892,   893,   894,     7,     8,     9,   250,   251,   252,   136,
     902,   138,   135,   136,   565,   566,   126,   127,   569,   570,
     571,   104,   105,   106,   107,   108,   109,   110,   920,   134,
     136,   582,   138,   136,   133,   138,   119,   102,    29,   104,
     105,   106,   107,   108,   109,   110,   597,   598,   599,   135,
     136,   134,   135,   136,   119,   136,    14,   138,   135,   136,
      14,   953,   135,   136,   133,   957,   958,   132,   119,   134,
     133,   135,   136,   135,   136,   135,   136,   135,   136,    29,
      30,   135,   136,   119,   976,   135,   136,    10,   980,   981,
     982,   135,   136,   667,    14,   669,   135,   136,   135,   136,
      14,   675,   135,   136,    10,   997,   135,   136,   135,   136,
     135,   136,  1001,   135,   136,   134,  1008,   135,   136,   135,
     136,  1013,   135,   136,   135,   136,   140,   701,   135,   136,
     135,   136,   135,   136,   135,   136,    29,    30,  1030,  1031,
     233,   234,   235,   236,   237,   238,   133,   240,   241,   242,
     243,   244,   245,   246,   247,   135,   136,   134,  1050,   135,
     136,   135,   136,   135,   136,   135,   136,   135,   136,   135,
     136,   135,   136,    29,    30,   135,   136,  1069,   135,   136,
     135,   136,   133,   734,    29,    30,   135,   136,   135,   136,
      10,  1083,   135,   136,    46,    47,  1088,   256,   257,   133,
     133,   258,   259,   133,   133,   133,   119,   119,   133,   133,
     100,   762,   133,    14,   133,   133,   133,  1109,   133,   115,
     119,    83,  1114,  1115,  1116,   133,   133,   125,   133,   133,
     133,   133,   133,   133,   119,     3,     4,     5,     6,   134,
     134,   119,    10,    11,    12,    13,    14,   134,    16,    17,
      18,    19,    20,    21,    22,   122,  1148,  1149,  1150,  1151,
    1152,  1153,  1154,  1155,   140,    33,    34,   133,   133,   133,
     119,   845,    40,   133,   133,  1167,  1168,  1169,    46,   133,
     133,   855,   133,   133,   139,   133,    54,    55,    56,    57,
     133,   133,    60,   133,   133,   133,   133,   133,   133,   122,
     122,    69,    70,   136,   133,   133,   140,    75,   140,   119,
      14,    14,    14,   864,   133,   133,    83,   868,   869,   870,
      14,   138,   138,    10,   133,   133,   133,   133,    10,    10,
      14,   119,    10,    10,   133,   133,   104,   105,    29,   133,
      62,    14,    14,    14,   133,   896,   133,   921,   140,   900,
     133,   133,   120,   133,   135,   134,   133,    83,   126,   127,
      83,   133,   133,   133,   140,     7,   134,   135,   133,   137,
     133,   139,     7,   141,     7,   119,   119,   133,    14,   119,
      10,    10,   135,    78,   133,   959,    14,    14,    14,   963,
      14,   133,     3,     4,     5,     6,   133,   133,   133,    10,
      11,    12,    13,    14,   135,    16,    17,    18,    19,    20,
      21,    22,   140,   133,   139,   133,   140,   968,   969,   133,
     133,   133,    33,    34,   133,   133,    14,    14,   133,    40,
      14,    14,   133,   140,   133,    46,   133,   133,    29,  1109,
     133,   133,   613,    54,    55,    56,    57,   133,   133,    60,
     538,   255,   135,   249,   143,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   120,
      -1,    46,    -1,    -1,  1065,   126,   127,    -1,  1069,    54,
      55,    56,    57,   134,   135,    60,   137,    -1,   139,    -1,
     141,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,    -1,   139,    -1,   141,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,  1166,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,   137,    -1,   139,    -1,   141,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,    -1,   137,    -1,   139,   140,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    65,    66,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,   126,
     127,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,    -1,
     137,    -1,   139,   140,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,   104,   105,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,
      33,    34,    -1,    -1,   133,   134,    -1,    40,   137,    -1,
     139,   140,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
     133,   134,    60,    -1,   137,    -1,   139,   140,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,   133,   134,    60,    -1,   137,
      -1,   139,   140,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,   126,   127,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,    -1,   137,    -1,   139,   140,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,   104,
     105,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,    33,    34,    -1,    -1,   133,   134,
      -1,    40,   137,    -1,   139,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,
      54,    55,    56,    57,    -1,   134,    60,    -1,   137,   138,
     139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,
      -1,    -1,   126,   127,    -1,    54,    55,    56,    57,   133,
     134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,
      54,    55,    56,    57,   133,   134,    60,    -1,   137,    -1,
     139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,
      -1,    -1,   126,   127,    -1,    54,    55,    56,    57,   133,
     134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,   138,   139,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   126,   127,   128,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,   126,
     127,    -1,    -1,    54,    55,    56,    57,   134,   135,    60,
     137,    -1,   139,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,
      46,    -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,
      56,    57,   133,   134,    60,    -1,   137,    -1,   139,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,
     126,   127,    -1,    54,    55,    56,    57,   133,   134,    60,
      -1,   137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    -1,    -1,   104,   105,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   120,
      -1,    46,    -1,    -1,    -1,   126,   127,    -1,    -1,    54,
      55,    56,    57,   134,   135,    60,   137,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,    -1,
      -1,   126,   127,    -1,    54,    55,    56,    57,    -1,   134,
      60,    -1,   137,    -1,   139,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
     120,    46,    -1,    -1,    -1,    -1,   126,   127,    -1,    54,
      55,    56,    57,    -1,   134,    60,   136,   137,    -1,   139,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    78,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   120,    -1,    46,    -1,    -1,
      -1,   126,   127,    -1,    -1,    54,    55,    56,    57,   134,
      -1,    60,   137,    -1,   139,    -1,    -1,    -1,    -1,    -1,
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
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,    -1,   134,    60,    -1,   137,
     138,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,   120,    -1,    46,
      -1,    -1,    -1,   126,   127,    -1,    -1,    54,    55,    56,
      57,   134,   135,    60,   137,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,   120,    -1,    46,    -1,    -1,    -1,   126,
     127,    -1,    -1,    54,    55,    56,    57,   134,   135,    60,
     137,    -1,   139,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   120,
      -1,    46,    -1,    -1,    -1,   126,   127,    -1,    -1,    54,
      55,    56,    57,   134,   135,    60,   137,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   120,    -1,    46,    -1,    -1,
      -1,   126,   127,    -1,    -1,    54,    55,    56,    57,   134,
     135,    60,   137,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   120,    -1,    46,    -1,    -1,    -1,   126,   127,    -1,
      -1,    54,    55,    56,    57,   134,   135,    60,   137,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,   120,    -1,    46,
      -1,    -1,    -1,   126,   127,    -1,    -1,    54,    55,    56,
      57,   134,   135,    60,   137,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,    -1,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,    -1,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,    -1,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,    -1,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,    -1,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,    -1,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,    -1,   134,    60,    -1,
     137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    -1,    -1,   137,    -1,   139,    83,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,   121,   122,   123,   124,
     125,    -1,    -1,   128,   129,   130,    -1,   132
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
     180,   181,   182,   183,   184,   185,   188,   189,   191,   102,
     104,   105,   106,   107,   108,   109,   110,   119,   132,   134,
     119,   137,   121,   139,   119,    11,   132,   134,    10,    11,
      12,    13,    14,    33,   134,   171,   184,    10,    11,   138,
     173,    14,    15,   133,   134,   173,   190,    10,    11,    12,
      14,    33,    38,   134,   134,   173,   134,   173,   134,   173,
     134,   173,   173,   180,    14,   171,   154,   171,   173,   133,
     173,   133,   173,    10,    14,    14,   173,   173,    12,    12,
     134,   173,   134,   173,   171,   134,   171,    28,    48,   133,
      28,    48,   133,   139,   172,   172,   139,   172,   173,   173,
     171,   171,   173,    14,    15,   161,   161,    14,    33,   184,
     161,    10,    10,   184,   184,   184,    10,   135,   173,   186,
     138,   173,    78,   144,    28,    31,    32,    41,    44,    45,
      48,    49,   133,    28,    31,    32,    41,    44,    45,    48,
      49,   133,    83,    87,    88,    89,    90,    97,    98,    99,
     121,   122,    91,    92,    93,    94,    95,    96,   103,   115,
     123,   124,   111,    84,    85,   125,   116,   117,   126,   127,
     112,   113,   114,   128,   129,   130,   118,   132,   132,    14,
     173,   173,   173,   173,   173,   173,    14,   135,   190,   134,
     173,   128,   173,    14,   173,   173,   133,    14,    18,    64,
      14,   135,   141,   168,   190,   137,   121,   139,   106,   107,
     108,   109,   110,   119,   119,   136,   138,   133,   173,   133,
     173,   135,   173,   190,   133,   136,   119,   133,   119,   133,
     119,   133,    10,    11,    12,    14,    10,    14,    10,   186,
     173,    28,    31,    32,    41,    44,    45,    48,   133,   136,
     173,    28,    48,   133,   136,   173,   171,   173,   171,    86,
     136,   171,    84,    85,   134,   139,   159,   133,   136,    28,
      48,   133,    28,    48,   133,   119,   119,    75,   136,   139,
     121,   139,   121,   139,   173,   171,   173,   171,    31,    49,
      25,   173,   173,   173,   173,   173,   173,   136,   173,   173,
     173,    59,   164,   139,   134,    14,    68,   139,   135,   136,
     135,   136,   136,   138,   140,    10,    11,    12,    13,    26,
      27,    35,    36,    52,    53,   140,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   176,   176,   176,   176,   176,   176,
     174,   176,   176,   176,   176,   176,   176,   176,   176,     7,
       8,     9,   177,   179,   179,   179,   180,   180,   181,   182,
     182,   183,   183,   184,   184,   184,   184,   184,   184,   184,
      14,    24,    26,    27,    35,    36,    40,    46,    47,    54,
      55,    56,    57,   187,    18,   187,   133,   134,   133,   133,
     133,   133,   133,   133,   119,   134,   135,   173,   133,   136,
     126,   127,   136,   138,   122,   140,   133,   134,    14,   134,
      83,    14,   135,   136,   135,   136,   173,    14,   173,   173,
     173,   173,   173,   173,   173,   173,   138,   190,   133,   133,
     133,   135,   173,   173,   134,   173,     5,    14,   134,   169,
     173,   119,   133,   119,   133,   119,   133,   134,   159,   119,
     119,   135,   136,   173,   173,   173,   173,   173,   173,   173,
     190,   136,   173,   173,   190,   135,    29,    30,   147,   135,
      10,   190,   180,   180,    10,    11,    14,    71,    72,    73,
     135,   170,   144,   190,   173,   173,   173,   173,   173,   173,
     133,   190,   156,    14,   173,    14,   173,   135,    29,   135,
     173,   173,    10,   133,   133,   133,   133,   133,   140,   173,
     171,    14,    24,   139,   165,   135,   170,   134,   134,   167,
     135,   190,   119,    10,   138,   190,   132,   137,   139,   119,
     173,   173,   173,   140,   173,    28,    48,   140,    28,    48,
     140,    28,    31,    32,    41,    44,    45,    48,    49,   140,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   100,   134,    14,   134,
     135,   190,   173,   135,   190,   136,   190,   173,   173,   190,
     119,   119,   119,   135,   190,   135,   168,   190,   173,   134,
      14,   141,   168,   138,   122,   140,   138,   133,   133,   173,
     133,   136,    83,    83,   169,   133,   136,   133,   173,   134,
     173,   173,   135,   170,   173,   173,   119,   190,   133,   133,
     133,   133,   133,   133,   133,   133,   190,   133,   133,   133,
     171,   146,   171,   134,   173,   171,   171,    86,   171,    86,
      86,    24,   119,    10,    76,    77,   159,   135,   136,    26,
      27,    36,   140,   173,   133,   133,   133,   133,   133,   133,
     133,    42,    43,   140,   122,   140,   122,   140,   171,   171,
     171,   133,   133,   119,   173,    14,    14,   165,    62,    63,
      66,   140,   159,   135,   135,   170,   170,    80,    81,    82,
     140,   135,   173,   138,    14,   173,   173,   173,    28,    32,
      41,    44,    45,    48,   140,   140,   140,   140,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     174,   135,   190,   135,   168,   190,   133,   135,   133,   135,
     190,   133,   138,   138,   138,   173,   173,   173,   135,   135,
     135,   173,    83,    14,   135,   136,   190,   173,   173,   135,
       5,    14,   133,   173,   133,   136,   133,   159,   135,   133,
     133,   173,   135,   135,    29,   147,   173,   171,    10,    10,
      10,    14,   173,   119,    10,    10,   159,    10,    11,    14,
      71,    72,    73,   173,   173,   140,   173,    28,    31,    32,
      41,    44,    45,    48,    49,   140,   173,   171,   133,   133,
      29,   173,   133,   140,    64,   187,    10,    13,    14,    16,
      17,    62,   159,   159,   135,   135,    14,    14,    14,   133,
     119,   134,   138,   140,   140,   173,   173,   173,   173,   173,
     173,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   135,   135,   135,   133,   135,   133,   133,
     133,   135,   173,   134,   190,   133,   133,    83,    83,   136,
     190,   159,   133,   136,   133,   133,   146,   171,   135,    29,
     147,   171,   171,   171,   173,    24,   119,    10,    76,    77,
      28,    32,    48,   140,   140,   140,   173,   173,   173,   173,
     173,   173,   173,   173,   171,   140,   171,   133,   153,   173,
     134,   139,   160,   134,   160,    14,    24,    74,   119,   133,
      14,    24,    74,   119,   133,    10,    13,    16,    17,   133,
     133,   187,   159,   159,     7,     7,     7,   173,   135,   190,
     119,   119,   140,   140,   140,   140,   140,   140,   133,   173,
     135,   173,   173,   190,   133,   190,   171,   171,    14,   173,
     119,    10,    10,   173,   173,   173,   140,   140,   140,   140,
     140,   140,   140,   140,   173,   135,   135,   170,    78,   144,
     135,   170,    14,    14,   133,   173,    14,    14,   133,   173,
      14,    24,    74,   119,   133,    14,    24,    74,   119,   133,
     133,   133,   134,   160,   140,   140,   135,   173,   173,   135,
     133,   135,   133,    29,   147,   173,   140,   140,   140,   133,
     171,   160,   135,   140,    26,    27,    36,   140,   173,   160,
     135,   133,   133,   133,   133,   133,   133,    14,    14,   133,
     173,    14,    14,   133,   173,   135,   170,   140,   140,   140,
     133,   171,   153,   160,   173,   173,   140,   173,    28,    31,
      32,    41,    44,    45,    48,    49,   140,   160,   133,   133,
     133,   133,   133,   133,   160,   135,   135,    28,    32,    48,
     140,   140,   140,   173,   173,   173,   173,   173,   173,   173,
     173,   160,   171,   173,   173,   173,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140
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
     173,   173,   173,   173,   174,   174,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   176,   176,   177,   177,   177,   178,
     178,   179,   179,   179,   180,   180,   181,   181,   181,   182,
     182,   182,   183,   183,   183,   183,   183,   183,   183,   184,
     184,   184,   184,   184,   184,   185,   185,   186,   186,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   188,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   190,   190,   190,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191
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
       2,     2,     3,     1,     5,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     3,     3,     1,     2,
       2,     2,     2,     2,     1,     3,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     3,     4,     6,     4,     3,
       6,     6,     5,     3,     3,     6,     5,     4,     4,     6,
       6,     5,     3,     6,     5,     3,     2,     3,     4,     3,
       2,     3,     2,     1,     1,     3,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     4,     6,     6,     6,     1,     4,     4,     5,
       5,     1,     1,     1,     1,     2,     3,     4,     5,     2,
       3,     4,     5,     2,     3,     4,     5,     1,     2
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
#line 490 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3263 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 500 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3269 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 501 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3275 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 505 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3281 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 507 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3287 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 509 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3293 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 511 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3301 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 515 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3307 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 517 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3313 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 519 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3319 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 521 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3325 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 523 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3333 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 527 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3341 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 531 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3347 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 533 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3355 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 537 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3363 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 541 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3369 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 543 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3375 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 545 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3381 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3387 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 549 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3393 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 551 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3401 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 555 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3409 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 559 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3415 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 561 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3421 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 563 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3427 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 565 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3433 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 567 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3439 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 569 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3445 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 571 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3451 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 573 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3457 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 575 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3463 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 577 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3469 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 579 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3475 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 581 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3481 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 583 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3493 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 591 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3499 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 593 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3505 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 595 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3511 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 597 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3517 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 599 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3523 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 601 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3529 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 603 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3536 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 606 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3543 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 609 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3549 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 611 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3556 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 614 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3563 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 617 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3569 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 619 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3577 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 623 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3583 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 625 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3589 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 627 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3596 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 630 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3603 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 633 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3609 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 635 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3616 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 638 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3623 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 641 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3630 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 644 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3636 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 646 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3645 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 651 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3653 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 655 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3661 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 659 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3670 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 664 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3678 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 667 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3684 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 669 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3690 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 671 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3696 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 673 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3702 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 675 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3708 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 677 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3715 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 680 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3721 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 682 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3727 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 684 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3733 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 686 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3740 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 689 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3747 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 692 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3754 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 695 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3761 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 698 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3768 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 701 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3774 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 703 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3780 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 705 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3786 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 707 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3792 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 709 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3799 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 712 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3805 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 714 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3811 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 716 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3817 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 718 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3824 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 721 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3831 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 724 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3839 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 728 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3846 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 731 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3852 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 733 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3858 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 735 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3864 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 737 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3871 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 740 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3878 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 743 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3885 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 746 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3892 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 749 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3899 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 752 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3906 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 755 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3913 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 757 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3919 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 758 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3925 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 759 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3931 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 760 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3937 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 761 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3943 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 762 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3949 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 764 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3955 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 766 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3961 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH block  */
#line 768 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3967 "raku.tab.c"
    break;

  case 110: /* stmt: block  */
#line 770 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3973 "raku.tab.c"
    break;

  case 111: /* stmt: unless_stmt  */
#line 771 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3979 "raku.tab.c"
    break;

  case 112: /* stmt: until_stmt  */
#line 772 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3985 "raku.tab.c"
    break;

  case 113: /* stmt: repeat_stmt  */
#line 773 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3991 "raku.tab.c"
    break;

  case 114: /* stmt: loop_stmt  */
#line 774 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3997 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST ';'  */
#line 775 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4003 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT ';'  */
#line 776 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4009 "raku.tab.c"
    break;

  case 117: /* stmt: KW_LAST KW_IF expr ';'  */
#line 778 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4015 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 780 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4021 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 782 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4027 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 784 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4033 "raku.tab.c"
    break;

  case 121: /* stmt: sub_decl  */
#line 785 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4039 "raku.tab.c"
    break;

  case 122: /* stmt: class_decl  */
#line 786 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4045 "raku.tab.c"
    break;

  case 123: /* stmt: role_decl  */
#line 787 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4051 "raku.tab.c"
    break;

  case 124: /* stmt: grammar_decl  */
#line 788 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4057 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF '(' expr ')' block  */
#line 792 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4063 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 794 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4069 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 796 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4075 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block  */
#line 798 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4081 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 800 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4087 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4093 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF expr block elsif_tail  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4099 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 806 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4105 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4111 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4117 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 814 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4123 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 816 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4129 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 818 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4135 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 820 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4141 "raku.tab.c"
    break;

  case 139: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 824 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4147 "raku.tab.c"
    break;

  case 140: /* while_stmt: KW_WHILE expr block  */
#line 826 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4153 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 830 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4159 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 832 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4165 "raku.tab.c"
    break;

  case 143: /* unless_stmt: KW_UNLESS expr block  */
#line 834 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4171 "raku.tab.c"
    break;

  case 144: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 836 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4177 "raku.tab.c"
    break;

  case 145: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4183 "raku.tab.c"
    break;

  case 146: /* until_stmt: KW_UNTIL expr block  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4189 "raku.tab.c"
    break;

  case 147: /* repeat_stmt: KW_REPEAT block  */
#line 846 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4195 "raku.tab.c"
    break;

  case 148: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 848 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4201 "raku.tab.c"
    break;

  case 149: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 850 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4207 "raku.tab.c"
    break;

  case 150: /* loop_stmt: KW_LOOP block  */
#line 854 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4213 "raku.tab.c"
    break;

  case 151: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 856 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4219 "raku.tab.c"
    break;

  case 152: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 858 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4225 "raku.tab.c"
    break;

  case 153: /* loop_incr: expr  */
#line 861 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4231 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 865 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4241 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 871 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4251 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 877 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4260 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 882 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4270 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 888 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4279 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR expr block  */
#line 893 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4286 "raku.tab.c"
    break;

  case 160: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 898 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4298 "raku.tab.c"
    break;

  case 161: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 906 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4311 "raku.tab.c"
    break;

  case 162: /* when_list: %empty  */
#line 916 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4317 "raku.tab.c"
    break;

  case 163: /* when_list: when_list KW_WHEN expr block  */
#line 918 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4325 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 924 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4337 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 932 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4347 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT sub_body  */
#line 938 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4357 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 944 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4369 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 952 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4379 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 958 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4389 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 964 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4402 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 973 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4413 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 980 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4426 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 989 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4439 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 998 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4450 "raku.tab.c"
    break;

  case 175: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1007 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4461 "raku.tab.c"
    break;

  case 176: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1014 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4470 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list '}'  */
#line 1020 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4476 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr '}'  */
#line 1022 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4483 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1025 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4490 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1028 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4496 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1030 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4502 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1032 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4508 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1034 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4514 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1036 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4520 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1038 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4526 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1040 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4532 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1042 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4539 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1045 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4545 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1047 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4551 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1049 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4557 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1051 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4564 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1054 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4571 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1057 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4579 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list '}'  */
#line 1062 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4585 "raku.tab.c"
    break;

  case 195: /* method_body: '{' YADA '}'  */
#line 1063 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4591 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr '}'  */
#line 1065 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4598 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1068 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4605 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1071 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4611 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1073 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4617 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1075 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4623 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1077 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4629 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1079 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4635 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1081 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4641 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1083 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4647 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1085 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4654 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1088 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4660 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1090 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4666 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1092 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4672 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1094 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4679 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1097 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4686 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1100 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4694 "raku.tab.c"
    break;

  case 212: /* pkg_name: IDENT  */
#line 1105 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4700 "raku.tab.c"
    break;

  case 213: /* pkg_name: QIDENT  */
#line 1106 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4706 "raku.tab.c"
    break;

  case 214: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1110 "raku.y"
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
#line 4724 "raku.tab.c"
    break;

  case 215: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1126 "raku.y"
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
#line 4741 "raku.tab.c"
    break;

  case 216: /* is_clauses: %empty  */
#line 1140 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4747 "raku.tab.c"
    break;

  case 217: /* is_clauses: is_clauses IDENT IDENT  */
#line 1142 "raku.y"
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
#line 4763 "raku.tab.c"
    break;

  case 218: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1154 "raku.y"
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
#line 4779 "raku.tab.c"
    break;

  case 219: /* class_body_list: %empty  */
#line 1167 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4785 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1169 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4792 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1172 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4799 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1175 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4806 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1178 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4813 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1181 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4820 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1184 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4827 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1187 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4834 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1190 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4841 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1193 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4848 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1196 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4855 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1199 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4862 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1202 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4869 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1205 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4880 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1212 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4891 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1219 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4902 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1226 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4913 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1233 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4921 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1237 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4929 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1241 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4937 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1245 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4945 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1249 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4956 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1256 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4967 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1263 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4978 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1270 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4989 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1277 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5003 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1287 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5015 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1295 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5027 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1303 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5040 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1312 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5051 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1319 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5062 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1326 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5076 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1336 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5088 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1344 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5100 "raku.tab.c"
    break;

  case 253: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1354 "raku.y"
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
#line 5117 "raku.tab.c"
    break;

  case 254: /* grammar_body_list: %empty  */
#line 1368 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5123 "raku.tab.c"
    break;

  case 255: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1370 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5132 "raku.tab.c"
    break;

  case 256: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1375 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5141 "raku.tab.c"
    break;

  case 257: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1380 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5150 "raku.tab.c"
    break;

  case 258: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1387 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5158 "raku.tab.c"
    break;

  case 259: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1391 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5166 "raku.tab.c"
    break;

  case 260: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1395 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5174 "raku.tab.c"
    break;

  case 261: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1399 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5182 "raku.tab.c"
    break;

  case 262: /* pair_list: IDENT OP_FATARROW expr  */
#line 1405 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5188 "raku.tab.c"
    break;

  case 263: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1407 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5194 "raku.tab.c"
    break;

  case 264: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1409 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5200 "raku.tab.c"
    break;

  case 265: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1411 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5206 "raku.tab.c"
    break;

  case 266: /* param_list: VAR_SCALAR  */
#line 1414 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5212 "raku.tab.c"
    break;

  case 267: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1415 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5218 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1416 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5224 "raku.tab.c"
    break;

  case 269: /* param_list: VAR_ARRAY  */
#line 1417 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5230 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_ARRAY  */
#line 1418 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5236 "raku.tab.c"
    break;

  case 271: /* param_list: IDENT VAR_SCALAR  */
#line 1419 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5242 "raku.tab.c"
    break;

  case 272: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1420 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5248 "raku.tab.c"
    break;

  case 273: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1421 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5254 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR  */
#line 1422 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5260 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1423 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5266 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1424 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5272 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1425 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5278 "raku.tab.c"
    break;

  case 278: /* param_list: VAR_SCALAR '=' expr  */
#line 1426 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5284 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1427 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5290 "raku.tab.c"
    break;

  case 280: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1428 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5296 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1429 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5302 "raku.tab.c"
    break;

  case 282: /* param_list: SLURPY_POS  */
#line 1430 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5308 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' SLURPY_POS  */
#line 1431 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5314 "raku.tab.c"
    break;

  case 284: /* param_list: SLURPY_LOL  */
#line 1432 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5320 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' SLURPY_LOL  */
#line 1433 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5326 "raku.tab.c"
    break;

  case 286: /* param_list: SLURPY_NAMED  */
#line 1434 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5332 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1435 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5338 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list '}'  */
#line 1438 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5344 "raku.tab.c"
    break;

  case 289: /* block: '{' YADA '}'  */
#line 1439 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5350 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr '}'  */
#line 1441 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5356 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1443 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5362 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1445 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5368 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1447 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5374 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1449 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5380 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1451 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5387 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1454 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5393 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1456 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5399 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1458 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5405 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1460 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5411 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1462 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5418 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1465 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5425 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1468 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5433 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1472 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5440 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1475 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5447 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1478 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5454 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1481 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5460 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1483 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5468 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1487 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5475 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1490 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5482 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1493 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5489 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1496 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5496 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1499 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5503 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1502 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5509 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1504 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5515 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1506 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5521 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_LAST '}'  */
#line 1508 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5527 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1510 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5533 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1512 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5539 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1514 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5545 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1516 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5551 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1518 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5557 "raku.tab.c"
    break;

  case 322: /* closure: '{' expr '}'  */
#line 1521 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5563 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR '=' expr  */
#line 1524 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5569 "raku.tab.c"
    break;

  case 324: /* expr: VAR_ARRAY '=' expr  */
#line 1525 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5575 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1526 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5581 "raku.tab.c"
    break;

  case 326: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1527 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5587 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1528 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5593 "raku.tab.c"
    break;

  case 328: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1529 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5599 "raku.tab.c"
    break;

  case 329: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1530 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5605 "raku.tab.c"
    break;

  case 330: /* expr: KW_GATHER block  */
#line 1531 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5615 "raku.tab.c"
    break;

  case 331: /* expr: KW_GATHER for_stmt  */
#line 1536 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5625 "raku.tab.c"
    break;

  case 332: /* expr: tern_expr OP_FATARROW expr  */
#line 1542 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5631 "raku.tab.c"
    break;

  case 333: /* expr: tern_expr  */
#line 1543 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5637 "raku.tab.c"
    break;

  case 334: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1547 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5643 "raku.tab.c"
    break;

  case 335: /* tern_expr: cmp_expr  */
#line 1548 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5649 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1551 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5655 "raku.tab.c"
    break;

  case 337: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1552 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5661 "raku.tab.c"
    break;

  case 338: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1553 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5667 "raku.tab.c"
    break;

  case 339: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1554 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5673 "raku.tab.c"
    break;

  case 340: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1555 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5679 "raku.tab.c"
    break;

  case 341: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1556 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5685 "raku.tab.c"
    break;

  case 342: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1557 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5691 "raku.tab.c"
    break;

  case 343: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1558 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5697 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1559 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5703 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1560 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5709 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1561 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5715 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1562 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5721 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1563 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5727 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1564 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5733 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1566 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5743 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1572 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5753 "raku.tab.c"
    break;

  case 352: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1578 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5763 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr  */
#line 1583 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5769 "raku.tab.c"
    break;

  case 354: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1586 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5775 "raku.tab.c"
    break;

  case 355: /* divis_expr: jct_expr  */
#line 1587 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5781 "raku.tab.c"
    break;

  case 356: /* jct_expr: jct_expr '|' range_expr  */
#line 1590 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5787 "raku.tab.c"
    break;

  case 357: /* jct_expr: jct_expr '&' range_expr  */
#line 1591 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5793 "raku.tab.c"
    break;

  case 358: /* jct_expr: dor_expr  */
#line 1592 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5799 "raku.tab.c"
    break;

  case 359: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1596 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5805 "raku.tab.c"
    break;

  case 360: /* dor_expr: range_expr  */
#line 1597 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5811 "raku.tab.c"
    break;

  case 361: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1600 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5817 "raku.tab.c"
    break;

  case 362: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1601 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5823 "raku.tab.c"
    break;

  case 363: /* range_expr: add_expr  */
#line 1602 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5829 "raku.tab.c"
    break;

  case 364: /* add_expr: add_expr '~' repl_expr  */
#line 1605 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5835 "raku.tab.c"
    break;

  case 365: /* add_expr: repl_expr  */
#line 1606 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5841 "raku.tab.c"
    break;

  case 366: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1609 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5847 "raku.tab.c"
    break;

  case 367: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1610 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5853 "raku.tab.c"
    break;

  case 368: /* repl_expr: addsub_expr  */
#line 1611 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5859 "raku.tab.c"
    break;

  case 369: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1614 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5865 "raku.tab.c"
    break;

  case 370: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1615 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5871 "raku.tab.c"
    break;

  case 371: /* addsub_expr: mul_expr  */
#line 1616 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5877 "raku.tab.c"
    break;

  case 372: /* mul_expr: mul_expr '*' unary_expr  */
#line 1619 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5883 "raku.tab.c"
    break;

  case 373: /* mul_expr: mul_expr '/' unary_expr  */
#line 1620 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5889 "raku.tab.c"
    break;

  case 374: /* mul_expr: mul_expr '%' unary_expr  */
#line 1621 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5895 "raku.tab.c"
    break;

  case 375: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1622 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5901 "raku.tab.c"
    break;

  case 376: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1624 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5907 "raku.tab.c"
    break;

  case 377: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1626 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5913 "raku.tab.c"
    break;

  case 378: /* mul_expr: unary_expr  */
#line 1627 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5919 "raku.tab.c"
    break;

  case 379: /* unary_expr: '-' unary_expr  */
#line 1630 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5925 "raku.tab.c"
    break;

  case 380: /* unary_expr: '+' unary_expr  */
#line 1631 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 5931 "raku.tab.c"
    break;

  case 381: /* unary_expr: '!' unary_expr  */
#line 1632 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5937 "raku.tab.c"
    break;

  case 382: /* unary_expr: CARET unary_expr  */
#line 1633 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5943 "raku.tab.c"
    break;

  case 383: /* unary_expr: OP_REDUCE unary_expr  */
#line 1635 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5952 "raku.tab.c"
    break;

  case 384: /* unary_expr: pow_expr  */
#line 1639 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5958 "raku.tab.c"
    break;

  case 385: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1642 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5964 "raku.tab.c"
    break;

  case 386: /* pow_expr: postfix_expr  */
#line 1643 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5970 "raku.tab.c"
    break;

  case 387: /* scalar_list: VAR_SCALAR  */
#line 1646 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5976 "raku.tab.c"
    break;

  case 388: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1647 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5982 "raku.tab.c"
    break;

  case 389: /* meth_name: IDENT  */
#line 1650 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5988 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_SORT  */
#line 1651 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5994 "raku.tab.c"
    break;

  case 391: /* meth_name: KW_REVERSE  */
#line 1652 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6000 "raku.tab.c"
    break;

  case 392: /* meth_name: KW_MAP  */
#line 1653 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6006 "raku.tab.c"
    break;

  case 393: /* meth_name: KW_GREP  */
#line 1654 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6012 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_SAY  */
#line 1655 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6018 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_PRINT  */
#line 1656 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6024 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_TAKE  */
#line 1657 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6030 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_RETURN  */
#line 1658 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6036 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_EXISTS  */
#line 1659 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6042 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_DELETE  */
#line 1660 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6048 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_JOIN  */
#line 1661 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6054 "raku.tab.c"
    break;

  case 401: /* meth_name: TESTOP  */
#line 1662 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6060 "raku.tab.c"
    break;

  case 402: /* postfix_expr: call_expr  */
#line 1664 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6066 "raku.tab.c"
    break;

  case 403: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1667 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6076 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '(' arg_list ')'  */
#line 1673 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6085 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '(' ')'  */
#line 1677 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6091 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1679 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6097 "raku.tab.c"
    break;

  case 407: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1681 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6103 "raku.tab.c"
    break;

  case 408: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1683 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6112 "raku.tab.c"
    break;

  case 409: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1688 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6118 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1690 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6128 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1696 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6138 "raku.tab.c"
    break;

  case 412: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1702 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6146 "raku.tab.c"
    break;

  case 413: /* call_expr: IDENT '.' KW_NEW  */
#line 1706 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6154 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' IDENT  */
#line 1710 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6163 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1715 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6174 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1722 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6183 "raku.tab.c"
    break;

  case 417: /* call_expr: IDENT '.' CARET IDENT  */
#line 1727 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6193 "raku.tab.c"
    break;

  case 418: /* call_expr: atom '.' CARET IDENT  */
#line 1733 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6203 "raku.tab.c"
    break;

  case 419: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1739 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6214 "raku.tab.c"
    break;

  case 420: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1746 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6225 "raku.tab.c"
    break;

  case 421: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1753 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6234 "raku.tab.c"
    break;

  case 422: /* call_expr: atom '.' meth_name  */
#line 1758 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6243 "raku.tab.c"
    break;

  case 423: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1763 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6254 "raku.tab.c"
    break;

  case 424: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1770 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6263 "raku.tab.c"
    break;

  case 425: /* call_expr: call_expr '.' meth_name  */
#line 1775 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6272 "raku.tab.c"
    break;

  case 426: /* call_expr: KW_DIE expr  */
#line 1780 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6278 "raku.tab.c"
    break;

  case 427: /* call_expr: KW_MAP closure expr  */
#line 1782 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6284 "raku.tab.c"
    break;

  case 428: /* call_expr: KW_MAP closure ',' expr  */
#line 1784 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6290 "raku.tab.c"
    break;

  case 429: /* call_expr: KW_GREP closure expr  */
#line 1786 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6296 "raku.tab.c"
    break;

  case 430: /* call_expr: KW_SORT expr  */
#line 1788 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6302 "raku.tab.c"
    break;

  case 431: /* call_expr: KW_SORT closure expr  */
#line 1790 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6308 "raku.tab.c"
    break;

  case 432: /* call_expr: KW_REVERSE expr  */
#line 1792 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6314 "raku.tab.c"
    break;

  case 433: /* call_expr: atom  */
#line 1793 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6320 "raku.tab.c"
    break;

  case 434: /* arg_list: expr  */
#line 1796 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6326 "raku.tab.c"
    break;

  case 435: /* arg_list: arg_list ',' expr  */
#line 1797 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6332 "raku.tab.c"
    break;

  case 436: /* arg_list: arg_list ','  */
#line 1798 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6338 "raku.tab.c"
    break;

  case 437: /* atom: LIT_INT  */
#line 1801 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6344 "raku.tab.c"
    break;

  case 438: /* atom: LIT_FLOAT  */
#line 1802 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6350 "raku.tab.c"
    break;

  case 439: /* atom: LIT_STR  */
#line 1803 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6356 "raku.tab.c"
    break;

  case 440: /* atom: WORDLIST  */
#line 1805 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6369 "raku.tab.c"
    break;

  case 441: /* atom: LIT_INTERP_STR  */
#line 1813 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6375 "raku.tab.c"
    break;

  case 442: /* atom: VAR_SCALAR  */
#line 1814 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6381 "raku.tab.c"
    break;

  case 443: /* atom: OP_INC VAR_SCALAR  */
#line 1815 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6387 "raku.tab.c"
    break;

  case 444: /* atom: OP_DEC VAR_SCALAR  */
#line 1816 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6393 "raku.tab.c"
    break;

  case 445: /* atom: VAR_SCALAR OP_INC  */
#line 1817 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6399 "raku.tab.c"
    break;

  case 446: /* atom: VAR_SCALAR OP_DEC  */
#line 1818 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6405 "raku.tab.c"
    break;

  case 447: /* atom: VAR_ARRAY  */
#line 1819 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6411 "raku.tab.c"
    break;

  case 448: /* atom: VAR_HASH  */
#line 1820 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6417 "raku.tab.c"
    break;

  case 449: /* atom: VAR_CAPTURE  */
#line 1822 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6425 "raku.tab.c"
    break;

  case 450: /* atom: VAR_FH  */
#line 1826 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6433 "raku.tab.c"
    break;

  case 451: /* atom: VAR_NAMED_CAPTURE  */
#line 1830 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6440 "raku.tab.c"
    break;

  case 452: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1833 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6446 "raku.tab.c"
    break;

  case 453: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1835 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6452 "raku.tab.c"
    break;

  case 454: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1837 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6458 "raku.tab.c"
    break;

  case 455: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1839 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6464 "raku.tab.c"
    break;

  case 456: /* atom: ARR_ALL_SLICE  */
#line 1841 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6470 "raku.tab.c"
    break;

  case 457: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1843 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6476 "raku.tab.c"
    break;

  case 458: /* atom: VAR_HASH '{' expr '}'  */
#line 1845 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6482 "raku.tab.c"
    break;

  case 459: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1847 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6488 "raku.tab.c"
    break;

  case 460: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1849 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6494 "raku.tab.c"
    break;

  case 461: /* atom: IDENT  */
#line 1850 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6500 "raku.tab.c"
    break;

  case 462: /* atom: VAR_TWIGIL  */
#line 1852 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6508 "raku.tab.c"
    break;

  case 463: /* atom: VAR_ARRAY_TWIGIL  */
#line 1856 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6516 "raku.tab.c"
    break;

  case 464: /* atom: VAR_HASH_TWIGIL  */
#line 1860 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6524 "raku.tab.c"
    break;

  case 465: /* atom: '[' ']'  */
#line 1863 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6530 "raku.tab.c"
    break;

  case 466: /* atom: '[' expr ']'  */
#line 1865 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6536 "raku.tab.c"
    break;

  case 467: /* atom: '[' expr ',' ']'  */
#line 1867 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6542 "raku.tab.c"
    break;

  case 468: /* atom: '[' expr ',' arg_list ']'  */
#line 1869 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6549 "raku.tab.c"
    break;

  case 469: /* atom: DOLLAR_LBRACKET ']'  */
#line 1871 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6555 "raku.tab.c"
    break;

  case 470: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1873 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6561 "raku.tab.c"
    break;

  case 471: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1875 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6567 "raku.tab.c"
    break;

  case 472: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1877 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6574 "raku.tab.c"
    break;

  case 473: /* atom: '(' ')'  */
#line 1879 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6580 "raku.tab.c"
    break;

  case 474: /* atom: '(' expr ')'  */
#line 1880 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6586 "raku.tab.c"
    break;

  case 475: /* atom: '(' expr ',' ')'  */
#line 1882 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6592 "raku.tab.c"
    break;

  case 476: /* atom: '(' expr ',' arg_list ')'  */
#line 1884 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6599 "raku.tab.c"
    break;

  case 477: /* atom: block  */
#line 1886 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6605 "raku.tab.c"
    break;

  case 478: /* atom: KW_SUB block  */
#line 1887 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6611 "raku.tab.c"
    break;


#line 6615 "raku.tab.c"

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

#line 1889 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
