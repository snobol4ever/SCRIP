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

#line 476 "raku.tab.c"

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
       0,   496,   496,   507,   508,   511,   513,   515,   517,   521,
     523,   525,   527,   529,   533,   537,   539,   543,   547,   549,
     551,   553,   555,   557,   561,   565,   567,   569,   571,   573,
     575,   577,   579,   581,   583,   585,   587,   589,   597,   599,
     601,   603,   605,   607,   609,   612,   615,   617,   620,   623,
     625,   629,   631,   633,   636,   639,   641,   644,   647,   650,
     652,   657,   661,   665,   670,   674,   675,   677,   679,   681,
     683,   686,   688,   690,   692,   695,   698,   701,   704,   707,
     709,   711,   713,   715,   718,   720,   722,   724,   727,   730,
     734,   737,   739,   741,   743,   746,   749,   752,   755,   758,
     761,   764,   765,   766,   767,   768,   769,   770,   772,   774,
     776,   778,   779,   780,   781,   782,   783,   784,   786,   788,
     790,   792,   793,   794,   795,   796,   799,   801,   803,   805,
     807,   809,   811,   813,   817,   819,   821,   823,   825,   827,
     831,   833,   837,   839,   841,   843,   847,   849,   853,   855,
     857,   861,   863,   865,   869,   872,   878,   884,   889,   895,
     900,   905,   913,   924,   925,   931,   939,   945,   951,   959,
     965,   971,   980,   987,   996,  1005,  1014,  1021,  1028,  1029,
    1032,  1035,  1037,  1039,  1041,  1043,  1045,  1047,  1049,  1052,
    1054,  1056,  1058,  1061,  1064,  1070,  1071,  1072,  1075,  1078,
    1080,  1082,  1084,  1086,  1088,  1090,  1092,  1095,  1097,  1099,
    1101,  1104,  1107,  1113,  1114,  1117,  1133,  1148,  1163,  1164,
    1176,  1190,  1191,  1194,  1197,  1200,  1203,  1206,  1209,  1212,
    1215,  1218,  1221,  1224,  1227,  1234,  1241,  1248,  1255,  1259,
    1263,  1267,  1271,  1278,  1285,  1292,  1299,  1309,  1317,  1325,
    1334,  1341,  1348,  1358,  1366,  1376,  1391,  1392,  1397,  1402,
    1409,  1413,  1417,  1421,  1427,  1429,  1431,  1433,  1437,  1438,
    1439,  1440,  1441,  1442,  1443,  1444,  1445,  1446,  1447,  1448,
    1449,  1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,
    1461,  1462,  1463,  1465,  1467,  1469,  1471,  1473,  1476,  1478,
    1480,  1482,  1484,  1487,  1490,  1494,  1497,  1500,  1503,  1505,
    1509,  1512,  1515,  1518,  1521,  1524,  1526,  1528,  1530,  1532,
    1534,  1536,  1538,  1540,  1544,  1547,  1548,  1549,  1550,  1551,
    1552,  1553,  1554,  1559,  1564,  1566,  1569,  1571,  1574,  1575,
    1578,  1579,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1593,  1594,  1600,  1606,  1612,  1615,  1616,
    1619,  1620,  1621,  1624,  1626,  1629,  1630,  1631,  1634,  1635,
    1638,  1639,  1640,  1643,  1644,  1645,  1648,  1649,  1650,  1651,
    1652,  1654,  1656,  1659,  1660,  1661,  1662,  1663,  1668,  1671,
    1672,  1675,  1676,  1679,  1680,  1681,  1682,  1683,  1684,  1685,
    1686,  1687,  1688,  1689,  1690,  1691,  1693,  1695,  1701,  1706,
    1707,  1709,  1711,  1716,  1718,  1724,  1730,  1734,  1738,  1743,
    1750,  1755,  1761,  1767,  1774,  1781,  1786,  1791,  1798,  1803,
    1808,  1810,  1812,  1814,  1816,  1818,  1820,  1822,  1825,  1826,
    1827,  1830,  1831,  1832,  1833,  1842,  1843,  1844,  1845,  1846,
    1847,  1848,  1849,  1850,  1854,  1858,  1861,  1863,  1865,  1867,
    1869,  1871,  1873,  1875,  1877,  1879,  1880,  1884,  1888,  1892,
    1893,  1895,  1897,  1900,  1901,  1903,  1905,  1908,  1909,  1910,
    1912,  1915,  1916
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
#line 497 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3290 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 507 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3296 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 508 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3302 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 512 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3308 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 514 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3314 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 516 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3320 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 518 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3328 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 522 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3334 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 524 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3340 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 526 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3346 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 528 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3352 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 530 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3360 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 534 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3368 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 538 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3374 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 540 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3382 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 544 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3390 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 548 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3396 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 550 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3402 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 552 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3408 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 554 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3414 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 556 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3420 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 558 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3428 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 562 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3436 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 566 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3442 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 568 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3448 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 570 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3454 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 572 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3460 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 574 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3466 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 576 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3472 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 578 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3478 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 580 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3484 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 582 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3490 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 584 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3496 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 586 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3502 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 588 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3508 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 590 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3520 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 598 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3526 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 600 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3532 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 602 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3538 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 604 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3544 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 606 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3550 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 608 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3556 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 610 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3563 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 613 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3570 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 616 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3576 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 618 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3583 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 621 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3590 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 624 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3596 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 626 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3604 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 630 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3610 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 632 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3616 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 634 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3623 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 637 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3630 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 640 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3636 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 642 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3643 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 645 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3650 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 648 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3657 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 651 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3663 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 653 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3672 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 658 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3680 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 662 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3688 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 666 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3697 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 671 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3705 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 674 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3711 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3717 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 678 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3723 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 680 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3729 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3735 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 684 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3742 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 687 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3748 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 689 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3754 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 691 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3760 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 693 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3767 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 696 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3774 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 699 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3781 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 702 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3788 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 705 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3795 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 708 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3801 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 710 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3807 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 712 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3813 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3819 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 716 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3826 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 719 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3832 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 721 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3838 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 723 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3844 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 725 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3851 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 728 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3858 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 731 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3866 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 735 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3873 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 738 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3879 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 740 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3885 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 742 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3891 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 744 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3898 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 747 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3905 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 750 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3912 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 753 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3919 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 756 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3926 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 759 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3933 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 762 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3940 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 764 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3946 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 765 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3952 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 766 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3958 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 767 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3964 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 768 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3970 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 769 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3976 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 771 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3982 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 773 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3988 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH block  */
#line 775 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3994 "raku.tab.c"
    break;

  case 110: /* stmt: block  */
#line 777 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4000 "raku.tab.c"
    break;

  case 111: /* stmt: unless_stmt  */
#line 778 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4006 "raku.tab.c"
    break;

  case 112: /* stmt: until_stmt  */
#line 779 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4012 "raku.tab.c"
    break;

  case 113: /* stmt: repeat_stmt  */
#line 780 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4018 "raku.tab.c"
    break;

  case 114: /* stmt: loop_stmt  */
#line 781 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4024 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST ';'  */
#line 782 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4030 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT ';'  */
#line 783 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4036 "raku.tab.c"
    break;

  case 117: /* stmt: KW_LAST KW_IF expr ';'  */
#line 785 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4042 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 787 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4048 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 789 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4054 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 791 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4060 "raku.tab.c"
    break;

  case 121: /* stmt: sub_decl  */
#line 792 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4066 "raku.tab.c"
    break;

  case 122: /* stmt: class_decl  */
#line 793 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4072 "raku.tab.c"
    break;

  case 123: /* stmt: role_decl  */
#line 794 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4078 "raku.tab.c"
    break;

  case 124: /* stmt: grammar_decl  */
#line 795 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4084 "raku.tab.c"
    break;

  case 125: /* stmt: module_decl  */
#line 796 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4090 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF '(' expr ')' block  */
#line 800 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4096 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4102 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4108 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block  */
#line 806 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4114 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 808 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4120 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4126 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF expr block elsif_tail  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4132 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 814 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4138 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block  */
#line 818 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4144 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 820 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4150 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 822 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4156 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 824 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4162 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4168 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4174 "raku.tab.c"
    break;

  case 140: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 832 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4180 "raku.tab.c"
    break;

  case 141: /* while_stmt: KW_WHILE expr block  */
#line 834 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4186 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 838 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4192 "raku.tab.c"
    break;

  case 143: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4198 "raku.tab.c"
    break;

  case 144: /* unless_stmt: KW_UNLESS expr block  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4204 "raku.tab.c"
    break;

  case 145: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 844 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4210 "raku.tab.c"
    break;

  case 146: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 848 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4216 "raku.tab.c"
    break;

  case 147: /* until_stmt: KW_UNTIL expr block  */
#line 850 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4222 "raku.tab.c"
    break;

  case 148: /* repeat_stmt: KW_REPEAT block  */
#line 854 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4228 "raku.tab.c"
    break;

  case 149: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 856 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4234 "raku.tab.c"
    break;

  case 150: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 858 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4240 "raku.tab.c"
    break;

  case 151: /* loop_stmt: KW_LOOP block  */
#line 862 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4246 "raku.tab.c"
    break;

  case 152: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 864 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4252 "raku.tab.c"
    break;

  case 153: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 866 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4258 "raku.tab.c"
    break;

  case 154: /* loop_incr: expr  */
#line 869 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4264 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 873 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4274 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 879 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4284 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 885 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4293 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 890 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4303 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 896 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4312 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR expr block  */
#line 901 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4319 "raku.tab.c"
    break;

  case 161: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 906 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4331 "raku.tab.c"
    break;

  case 162: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 914 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4344 "raku.tab.c"
    break;

  case 163: /* when_list: %empty  */
#line 924 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4350 "raku.tab.c"
    break;

  case 164: /* when_list: when_list KW_WHEN expr block  */
#line 926 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4358 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 932 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4370 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 940 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4380 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_SUB IDENT sub_body  */
#line 946 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4390 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 952 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4402 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 960 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4412 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 966 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4422 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 972 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4435 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 981 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4446 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 988 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4459 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 997 "raku.y"
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

  case 175: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1006 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4483 "raku.tab.c"
    break;

  case 176: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1015 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4494 "raku.tab.c"
    break;

  case 177: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1022 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4503 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list '}'  */
#line 1028 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4509 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr '}'  */
#line 1030 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4516 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1033 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4523 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1036 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4529 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1038 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4535 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1040 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4541 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1042 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4547 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1044 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4553 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1046 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4559 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1048 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4565 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1050 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4572 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1053 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4578 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1055 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4584 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1057 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4590 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1059 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4597 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1062 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4604 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1065 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4612 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list '}'  */
#line 1070 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4618 "raku.tab.c"
    break;

  case 196: /* method_body: '{' YADA '}'  */
#line 1071 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4624 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr '}'  */
#line 1073 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4631 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1076 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4638 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1079 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4644 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1081 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4650 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1083 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4656 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1085 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4662 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1087 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4668 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1089 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4674 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1091 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4680 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1093 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4687 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1096 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4693 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1098 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4699 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1100 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4705 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1102 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4712 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1105 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4719 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1108 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4727 "raku.tab.c"
    break;

  case 213: /* pkg_name: IDENT  */
#line 1113 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4733 "raku.tab.c"
    break;

  case 214: /* pkg_name: QIDENT  */
#line 1114 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 4739 "raku.tab.c"
    break;

  case 215: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1118 "raku.y"
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
#line 4757 "raku.tab.c"
    break;

  case 216: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1134 "raku.y"
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
#line 4774 "raku.tab.c"
    break;

  case 217: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1149 "raku.y"
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
#line 4791 "raku.tab.c"
    break;

  case 218: /* is_clauses: %empty  */
#line 1163 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4797 "raku.tab.c"
    break;

  case 219: /* is_clauses: is_clauses IDENT IDENT  */
#line 1165 "raku.y"
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
#line 4813 "raku.tab.c"
    break;

  case 220: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1177 "raku.y"
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
#line 4829 "raku.tab.c"
    break;

  case 221: /* class_body_list: %empty  */
#line 1190 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4835 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1192 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4842 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1195 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4849 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1198 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4856 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1201 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4863 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1204 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4870 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1207 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4877 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1210 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4884 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1213 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4891 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1216 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4898 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1219 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4905 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1222 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4912 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1225 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4919 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1228 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4930 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1235 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4941 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1242 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4952 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1249 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4963 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1256 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4971 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1260 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4979 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1264 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4987 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1268 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4995 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1272 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5006 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1279 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5017 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1286 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5028 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1293 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5039 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1300 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5053 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1310 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5065 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1318 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5077 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1326 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5090 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1335 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5101 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1342 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5112 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1349 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5126 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1359 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5138 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1367 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5150 "raku.tab.c"
    break;

  case 255: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1377 "raku.y"
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
#line 5167 "raku.tab.c"
    break;

  case 256: /* grammar_body_list: %empty  */
#line 1391 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5173 "raku.tab.c"
    break;

  case 257: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1393 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5182 "raku.tab.c"
    break;

  case 258: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1398 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5191 "raku.tab.c"
    break;

  case 259: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1403 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5200 "raku.tab.c"
    break;

  case 260: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1410 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5208 "raku.tab.c"
    break;

  case 261: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1414 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5216 "raku.tab.c"
    break;

  case 262: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1418 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5224 "raku.tab.c"
    break;

  case 263: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1422 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5232 "raku.tab.c"
    break;

  case 264: /* pair_list: IDENT OP_FATARROW expr  */
#line 1428 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5238 "raku.tab.c"
    break;

  case 265: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1430 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5244 "raku.tab.c"
    break;

  case 266: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1432 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5250 "raku.tab.c"
    break;

  case 267: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1434 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5256 "raku.tab.c"
    break;

  case 268: /* param_list: VAR_SCALAR  */
#line 1437 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5262 "raku.tab.c"
    break;

  case 269: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1438 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5268 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1439 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5274 "raku.tab.c"
    break;

  case 271: /* param_list: VAR_ARRAY  */
#line 1440 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 5280 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' VAR_ARRAY  */
#line 1441 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 5286 "raku.tab.c"
    break;

  case 273: /* param_list: IDENT VAR_SCALAR  */
#line 1442 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5292 "raku.tab.c"
    break;

  case 274: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1443 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5298 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1444 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5304 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' VAR_SCALAR  */
#line 1445 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5310 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1446 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5316 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1447 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5322 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1448 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5328 "raku.tab.c"
    break;

  case 280: /* param_list: VAR_SCALAR '=' expr  */
#line 1449 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5334 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1450 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5340 "raku.tab.c"
    break;

  case 282: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1451 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5346 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1452 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5352 "raku.tab.c"
    break;

  case 284: /* param_list: SLURPY_POS  */
#line 1453 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5358 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' SLURPY_POS  */
#line 1454 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5364 "raku.tab.c"
    break;

  case 286: /* param_list: SLURPY_LOL  */
#line 1455 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5370 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' SLURPY_LOL  */
#line 1456 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5376 "raku.tab.c"
    break;

  case 288: /* param_list: SLURPY_NAMED  */
#line 1457 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5382 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1458 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5388 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list '}'  */
#line 1461 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5394 "raku.tab.c"
    break;

  case 291: /* block: '{' YADA '}'  */
#line 1462 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5400 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr '}'  */
#line 1464 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5406 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1466 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5412 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1468 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5418 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1470 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5424 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1472 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5430 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1474 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5437 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1477 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5443 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1479 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5449 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1481 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5455 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1483 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5461 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1485 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5468 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1488 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5475 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1491 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5483 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1495 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5490 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1498 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5497 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1501 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5504 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1504 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5510 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1506 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5518 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1510 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5525 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1513 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5532 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1516 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5539 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1519 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5546 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1522 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5553 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1525 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5559 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1527 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5565 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1529 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5571 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_LAST '}'  */
#line 1531 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5577 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1533 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5583 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1535 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5589 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1537 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5595 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1539 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5601 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1541 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5607 "raku.tab.c"
    break;

  case 324: /* closure: '{' expr '}'  */
#line 1544 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5613 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR '=' expr  */
#line 1547 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5619 "raku.tab.c"
    break;

  case 326: /* expr: VAR_ARRAY '=' expr  */
#line 1548 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5625 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1549 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5631 "raku.tab.c"
    break;

  case 328: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1550 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5637 "raku.tab.c"
    break;

  case 329: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1551 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5643 "raku.tab.c"
    break;

  case 330: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1552 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5649 "raku.tab.c"
    break;

  case 331: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1553 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5655 "raku.tab.c"
    break;

  case 332: /* expr: KW_GATHER block  */
#line 1554 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5665 "raku.tab.c"
    break;

  case 333: /* expr: KW_GATHER for_stmt  */
#line 1559 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5675 "raku.tab.c"
    break;

  case 334: /* expr: tern_expr OP_FATARROW expr  */
#line 1565 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5681 "raku.tab.c"
    break;

  case 335: /* expr: tern_expr  */
#line 1566 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5687 "raku.tab.c"
    break;

  case 336: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1570 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5693 "raku.tab.c"
    break;

  case 337: /* tern_expr: or_expr  */
#line 1571 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5699 "raku.tab.c"
    break;

  case 338: /* or_expr: or_expr OP_OR and_expr  */
#line 1574 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5705 "raku.tab.c"
    break;

  case 339: /* or_expr: and_expr  */
#line 1575 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5711 "raku.tab.c"
    break;

  case 340: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1578 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5717 "raku.tab.c"
    break;

  case 341: /* and_expr: cmp_expr  */
#line 1579 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5723 "raku.tab.c"
    break;

  case 342: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1582 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5729 "raku.tab.c"
    break;

  case 343: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1583 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5735 "raku.tab.c"
    break;

  case 344: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1584 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5741 "raku.tab.c"
    break;

  case 345: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1585 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5747 "raku.tab.c"
    break;

  case 346: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1586 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5753 "raku.tab.c"
    break;

  case 347: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1587 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5759 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1588 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5765 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1589 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5771 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1590 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5777 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1591 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5783 "raku.tab.c"
    break;

  case 352: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1592 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5789 "raku.tab.c"
    break;

  case 353: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1593 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5795 "raku.tab.c"
    break;

  case 354: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1595 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5805 "raku.tab.c"
    break;

  case 355: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1601 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5815 "raku.tab.c"
    break;

  case 356: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1607 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5825 "raku.tab.c"
    break;

  case 357: /* cmp_expr: divis_expr  */
#line 1612 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5831 "raku.tab.c"
    break;

  case 358: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1615 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5837 "raku.tab.c"
    break;

  case 359: /* divis_expr: jct_expr  */
#line 1616 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5843 "raku.tab.c"
    break;

  case 360: /* jct_expr: jct_expr '|' range_expr  */
#line 1619 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5849 "raku.tab.c"
    break;

  case 361: /* jct_expr: jct_expr '&' range_expr  */
#line 1620 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5855 "raku.tab.c"
    break;

  case 362: /* jct_expr: dor_expr  */
#line 1621 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5861 "raku.tab.c"
    break;

  case 363: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1625 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5867 "raku.tab.c"
    break;

  case 364: /* dor_expr: range_expr  */
#line 1626 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5873 "raku.tab.c"
    break;

  case 365: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1629 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5879 "raku.tab.c"
    break;

  case 366: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1630 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5885 "raku.tab.c"
    break;

  case 367: /* range_expr: add_expr  */
#line 1631 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5891 "raku.tab.c"
    break;

  case 368: /* add_expr: add_expr '~' repl_expr  */
#line 1634 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5897 "raku.tab.c"
    break;

  case 369: /* add_expr: repl_expr  */
#line 1635 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5903 "raku.tab.c"
    break;

  case 370: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1638 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5909 "raku.tab.c"
    break;

  case 371: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1639 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5915 "raku.tab.c"
    break;

  case 372: /* repl_expr: addsub_expr  */
#line 1640 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5921 "raku.tab.c"
    break;

  case 373: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1643 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5927 "raku.tab.c"
    break;

  case 374: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1644 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5933 "raku.tab.c"
    break;

  case 375: /* addsub_expr: mul_expr  */
#line 1645 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5939 "raku.tab.c"
    break;

  case 376: /* mul_expr: mul_expr '*' unary_expr  */
#line 1648 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5945 "raku.tab.c"
    break;

  case 377: /* mul_expr: mul_expr '/' unary_expr  */
#line 1649 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5951 "raku.tab.c"
    break;

  case 378: /* mul_expr: mul_expr '%' unary_expr  */
#line 1650 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5957 "raku.tab.c"
    break;

  case 379: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1651 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5963 "raku.tab.c"
    break;

  case 380: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1653 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5969 "raku.tab.c"
    break;

  case 381: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1655 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5975 "raku.tab.c"
    break;

  case 382: /* mul_expr: unary_expr  */
#line 1656 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5981 "raku.tab.c"
    break;

  case 383: /* unary_expr: '-' unary_expr  */
#line 1659 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5987 "raku.tab.c"
    break;

  case 384: /* unary_expr: '+' unary_expr  */
#line 1660 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 5993 "raku.tab.c"
    break;

  case 385: /* unary_expr: '!' unary_expr  */
#line 1661 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5999 "raku.tab.c"
    break;

  case 386: /* unary_expr: CARET unary_expr  */
#line 1662 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6005 "raku.tab.c"
    break;

  case 387: /* unary_expr: OP_REDUCE unary_expr  */
#line 1664 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6014 "raku.tab.c"
    break;

  case 388: /* unary_expr: pow_expr  */
#line 1668 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6020 "raku.tab.c"
    break;

  case 389: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1671 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6026 "raku.tab.c"
    break;

  case 390: /* pow_expr: postfix_expr  */
#line 1672 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6032 "raku.tab.c"
    break;

  case 391: /* scalar_list: VAR_SCALAR  */
#line 1675 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6038 "raku.tab.c"
    break;

  case 392: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1676 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 6044 "raku.tab.c"
    break;

  case 393: /* meth_name: IDENT  */
#line 1679 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6050 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_SORT  */
#line 1680 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 6056 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_REVERSE  */
#line 1681 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 6062 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_MAP  */
#line 1682 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 6068 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_GREP  */
#line 1683 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 6074 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_SAY  */
#line 1684 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 6080 "raku.tab.c"
    break;

  case 399: /* meth_name: KW_PRINT  */
#line 1685 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6086 "raku.tab.c"
    break;

  case 400: /* meth_name: KW_TAKE  */
#line 1686 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6092 "raku.tab.c"
    break;

  case 401: /* meth_name: KW_RETURN  */
#line 1687 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6098 "raku.tab.c"
    break;

  case 402: /* meth_name: KW_EXISTS  */
#line 1688 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6104 "raku.tab.c"
    break;

  case 403: /* meth_name: KW_DELETE  */
#line 1689 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6110 "raku.tab.c"
    break;

  case 404: /* meth_name: KW_JOIN  */
#line 1690 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6116 "raku.tab.c"
    break;

  case 405: /* meth_name: TESTOP  */
#line 1691 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6122 "raku.tab.c"
    break;

  case 406: /* postfix_expr: call_expr  */
#line 1693 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6128 "raku.tab.c"
    break;

  case 407: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1696 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6138 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '(' arg_list ')'  */
#line 1702 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6147 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '(' ')'  */
#line 1706 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6153 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1708 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6159 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1710 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6165 "raku.tab.c"
    break;

  case 412: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1712 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6174 "raku.tab.c"
    break;

  case 413: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1717 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6180 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1719 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6190 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1725 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6200 "raku.tab.c"
    break;

  case 416: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1731 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6208 "raku.tab.c"
    break;

  case 417: /* call_expr: IDENT '.' KW_NEW  */
#line 1735 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6216 "raku.tab.c"
    break;

  case 418: /* call_expr: IDENT '.' IDENT  */
#line 1739 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6225 "raku.tab.c"
    break;

  case 419: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1744 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6236 "raku.tab.c"
    break;

  case 420: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1751 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6245 "raku.tab.c"
    break;

  case 421: /* call_expr: IDENT '.' CARET IDENT  */
#line 1756 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6255 "raku.tab.c"
    break;

  case 422: /* call_expr: atom '.' CARET IDENT  */
#line 1762 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6265 "raku.tab.c"
    break;

  case 423: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1768 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6276 "raku.tab.c"
    break;

  case 424: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1775 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6287 "raku.tab.c"
    break;

  case 425: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1782 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6296 "raku.tab.c"
    break;

  case 426: /* call_expr: atom '.' meth_name  */
#line 1787 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6305 "raku.tab.c"
    break;

  case 427: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1792 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6316 "raku.tab.c"
    break;

  case 428: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1799 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6325 "raku.tab.c"
    break;

  case 429: /* call_expr: call_expr '.' meth_name  */
#line 1804 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6334 "raku.tab.c"
    break;

  case 430: /* call_expr: KW_DIE expr  */
#line 1809 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6340 "raku.tab.c"
    break;

  case 431: /* call_expr: KW_MAP closure expr  */
#line 1811 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6346 "raku.tab.c"
    break;

  case 432: /* call_expr: KW_MAP closure ',' expr  */
#line 1813 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6352 "raku.tab.c"
    break;

  case 433: /* call_expr: KW_GREP closure expr  */
#line 1815 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6358 "raku.tab.c"
    break;

  case 434: /* call_expr: KW_SORT expr  */
#line 1817 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6364 "raku.tab.c"
    break;

  case 435: /* call_expr: KW_SORT closure expr  */
#line 1819 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6370 "raku.tab.c"
    break;

  case 436: /* call_expr: KW_REVERSE expr  */
#line 1821 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6376 "raku.tab.c"
    break;

  case 437: /* call_expr: atom  */
#line 1822 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6382 "raku.tab.c"
    break;

  case 438: /* arg_list: expr  */
#line 1825 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6388 "raku.tab.c"
    break;

  case 439: /* arg_list: arg_list ',' expr  */
#line 1826 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6394 "raku.tab.c"
    break;

  case 440: /* arg_list: arg_list ','  */
#line 1827 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6400 "raku.tab.c"
    break;

  case 441: /* atom: LIT_INT  */
#line 1830 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6406 "raku.tab.c"
    break;

  case 442: /* atom: LIT_FLOAT  */
#line 1831 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6412 "raku.tab.c"
    break;

  case 443: /* atom: LIT_STR  */
#line 1832 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6418 "raku.tab.c"
    break;

  case 444: /* atom: WORDLIST  */
#line 1834 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6431 "raku.tab.c"
    break;

  case 445: /* atom: LIT_INTERP_STR  */
#line 1842 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6437 "raku.tab.c"
    break;

  case 446: /* atom: VAR_SCALAR  */
#line 1843 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6443 "raku.tab.c"
    break;

  case 447: /* atom: OP_INC VAR_SCALAR  */
#line 1844 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6449 "raku.tab.c"
    break;

  case 448: /* atom: OP_DEC VAR_SCALAR  */
#line 1845 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6455 "raku.tab.c"
    break;

  case 449: /* atom: VAR_SCALAR OP_INC  */
#line 1846 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6461 "raku.tab.c"
    break;

  case 450: /* atom: VAR_SCALAR OP_DEC  */
#line 1847 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6467 "raku.tab.c"
    break;

  case 451: /* atom: VAR_ARRAY  */
#line 1848 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6473 "raku.tab.c"
    break;

  case 452: /* atom: VAR_HASH  */
#line 1849 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6479 "raku.tab.c"
    break;

  case 453: /* atom: VAR_CAPTURE  */
#line 1851 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6487 "raku.tab.c"
    break;

  case 454: /* atom: VAR_FH  */
#line 1855 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6495 "raku.tab.c"
    break;

  case 455: /* atom: VAR_NAMED_CAPTURE  */
#line 1859 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6502 "raku.tab.c"
    break;

  case 456: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1862 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6508 "raku.tab.c"
    break;

  case 457: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1864 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6514 "raku.tab.c"
    break;

  case 458: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1866 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6520 "raku.tab.c"
    break;

  case 459: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1868 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6526 "raku.tab.c"
    break;

  case 460: /* atom: ARR_ALL_SLICE  */
#line 1870 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6532 "raku.tab.c"
    break;

  case 461: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1872 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6538 "raku.tab.c"
    break;

  case 462: /* atom: VAR_HASH '{' expr '}'  */
#line 1874 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6544 "raku.tab.c"
    break;

  case 463: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1876 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6550 "raku.tab.c"
    break;

  case 464: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1878 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6556 "raku.tab.c"
    break;

  case 465: /* atom: IDENT  */
#line 1879 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6562 "raku.tab.c"
    break;

  case 466: /* atom: VAR_TWIGIL  */
#line 1881 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6570 "raku.tab.c"
    break;

  case 467: /* atom: VAR_ARRAY_TWIGIL  */
#line 1885 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6578 "raku.tab.c"
    break;

  case 468: /* atom: VAR_HASH_TWIGIL  */
#line 1889 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6586 "raku.tab.c"
    break;

  case 469: /* atom: '[' ']'  */
#line 1892 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6592 "raku.tab.c"
    break;

  case 470: /* atom: '[' expr ']'  */
#line 1894 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6598 "raku.tab.c"
    break;

  case 471: /* atom: '[' expr ',' ']'  */
#line 1896 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6604 "raku.tab.c"
    break;

  case 472: /* atom: '[' expr ',' arg_list ']'  */
#line 1898 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6611 "raku.tab.c"
    break;

  case 473: /* atom: DOLLAR_LBRACKET ']'  */
#line 1900 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 6617 "raku.tab.c"
    break;

  case 474: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1902 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6623 "raku.tab.c"
    break;

  case 475: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1904 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6629 "raku.tab.c"
    break;

  case 476: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1906 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6636 "raku.tab.c"
    break;

  case 477: /* atom: '(' ')'  */
#line 1908 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6642 "raku.tab.c"
    break;

  case 478: /* atom: '(' expr ')'  */
#line 1909 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6648 "raku.tab.c"
    break;

  case 479: /* atom: '(' expr ',' ')'  */
#line 1911 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6654 "raku.tab.c"
    break;

  case 480: /* atom: '(' expr ',' arg_list ')'  */
#line 1913 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6661 "raku.tab.c"
    break;

  case 481: /* atom: block  */
#line 1915 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6667 "raku.tab.c"
    break;

  case 482: /* atom: KW_SUB block  */
#line 1916 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6673 "raku.tab.c"
    break;


#line 6677 "raku.tab.c"

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

#line 1918 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
